#include <ai_agent.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdio>
#include <unistd.h>
#include <signal.h>

#define HDR_FORMAT "%08X"
#define HDR_LENGTH 9

namespace ai
{
  namespace Agent
  {
    static void ignore_sigpipe( ) {
      static bool ignoring_sigpipe = false;
      if( !ignoring_sigpipe ) {
        struct sigaction act;

        // ignore SIGPIPE
        act.sa_handler=SIG_IGN;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        sigaction(SIGPIPE, &act, NULL); 

        ignoring_sigpipe = true;
      }
    }


    Socket::Socket()
    {
      ignore_sigpipe( );
      mSocket = -1;
      mSocketOK = false;
    }
    Socket::Socket(int socket_in)
    {
      ignore_sigpipe( );
      mSocket = socket_in;
      mSocketOK = true;
    }
    Socket::~Socket()
    {
      close(mSocket);
    }
    int Socket::GetSocket()
    {
      return mSocket;
    }
    Socket * Socket::Accept()
    {
      sockaddr_in client_addr;
      socklen_t sin_size = sizeof(client_addr);
      int client_sock;
      if((client_sock = accept(mSocket, (sockaddr *)&client_addr, &sin_size)) == -1)
        {
          perror("accept");
          return NULL;
        }
      std::cerr << "Client connected from: " << inet_ntoa(client_addr.sin_addr) << std::endl;
      return new Socket(client_sock);
    }
    std::vector<Socket *> Socket::AcceptMulti(int n)
    {
      std::vector<Socket *> clients;
      for(int i = 0; i < n; i++)
        {
          clients.push_back(Accept());
        }
      return clients;
    }
    bool Socket::Connect(std::string hostname, int port)
    {
      hostent * h;
      if((h = gethostbyname(hostname.c_str())) == NULL)
        {
          herror("gethostbyname");
          return false;
        }
      if((mSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        {
          perror("socket");
          return false;
        }
      sockaddr_in dest_addr;
      dest_addr.sin_family = AF_INET;
      dest_addr.sin_port = htons(port);
      dest_addr.sin_addr = *(in_addr *)h->h_addr;
      memset(dest_addr.sin_zero, '\0', sizeof(dest_addr.sin_zero));
      if(connect(mSocket, (sockaddr *)&dest_addr, sizeof(dest_addr)) == -1)
        {
          perror("connect");
          return false;
        }
      mSocketOK = true;
      return true;
    }
    bool Socket::Listen(int port)
    {
      if((mSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        {
          perror("socket");
          return false;
        }
      int yes = 1;
      if(setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
          perror("setsockopt");
          return false;
        }
      sockaddr_in local_addr;
      local_addr.sin_family = AF_INET;
      local_addr.sin_port = htons(port);
      local_addr.sin_addr.s_addr = INADDR_ANY;
      memset(local_addr.sin_zero, '\0', sizeof(local_addr.sin_zero));
      if(bind(mSocket, (sockaddr *)&local_addr, sizeof(local_addr)) == -1)
        {
          perror("bind");
          return false;
        }
      if(listen(mSocket, 100) == -1)
        {
          perror("listen");
          return false;
        }
      mSocketOK = true;
      return true;
    }
    bool Socket::Recv(std::string &msg)
    {
      int rx;
      char header[HDR_LENGTH];
      DBGP_SOCKET(LOG_STREAM(std::cerr) << "Pre header recv" << std::endl);
      if((rx = recv(mSocket, header, HDR_LENGTH, 0)) == -1)
        {
          perror("recv");
          DBGP_E( LOG_STREAM( std::cerr )
                  << "Receive from agent failed." 
                  << std::endl );
          mSocketOK = false;
          return false;
        }
      else if(!rx)
        {
          std::cerr << "Disconnected from peer" << std::endl;
          DBGP_E( LOG_STREAM( std::cerr )
                  << "Disconnected from peer." 
                  << std::endl );
          mSocketOK = false;
          return false;
        }
      DBGP_SOCKET(LOG_STREAM(std::cerr) << "Post header recv" << std::endl);
      unsigned int length;
      sscanf(header, HDR_FORMAT, &length);
      char * buffer = new char[length];
      unsigned int total = 0;
      while(total < length)
        {
          DBGP_SOCKET(LOG_STREAM(std::cerr) << "Pre recv " << total << " " << length << std::endl);
          if((rx = recv(mSocket, buffer+total, length-total, 0)) == -1)
            {
              perror("recv");
              DBGP_E( LOG_STREAM( std::cerr )
                      << "Receive continuation from agent failed." 
                      << std::endl );
              mSocketOK = false;
              return false;
            }
          else if(!rx)
            {
              std::cerr << "Disconnected from peer" << std::endl;
              DBGP_E( LOG_STREAM( std::cerr )
                      << "Disconnected from peer." 
                      << std::endl );
              mSocketOK = false;
              return false;
            }
          DBGP_SOCKET(LOG_STREAM(std::cerr) << "Post recv " << total << " " << length << " " << rx << std::endl);
          total += rx;
        }
      msg = buffer;
      delete [] buffer;
      DBGP_SOCKET(LOG_STREAM(std::cerr) << "Message: " << msg << std::endl);
      return true;
    }
    bool Socket::Send(const std::string &msg)
    {
      DBGP_SOCKET(LOG_STREAM(std::cerr) << "Message: " << msg << std::endl);
      int tx;
      char header[HDR_LENGTH];
      int length = msg.length()+1;
      sprintf(header, HDR_FORMAT, length);
      if((tx = send(mSocket, header, HDR_LENGTH, 0)) == -1)
        {
          perror("send");
          DBGP_E( LOG_STREAM( std::cerr )
                  << "Send to agent failed." 
                  << std::endl );
          mSocketOK = false;
          return false;
        }
      int total = 0;
      while(total < length)
        {
          if((tx = send(mSocket, msg.c_str()+total, length-total, 0)) == -1)
            {
              perror("send");
              DBGP_E( LOG_STREAM( std::cerr )
                      << "Send continuation to agent failed." 
                      << std::endl );
              mSocketOK = false;
              return false;
            }
          total += tx;
        }
      return true;
    }
    bool Socket::Select(std::vector<Socket *> &sockets, std::vector<Socket *> &ready, double timeout)
    {
      fd_set readfds;
      FD_ZERO(&readfds);
      int s, maxsock = -1;
      for(unsigned int i = 0; i < sockets.size(); i++)
        {
          s = sockets[i]->GetSocket();
          FD_SET(s, &readfds);
          if(s > maxsock)
            maxsock = s;
        }
      if(maxsock < 0)
        return false;
      struct timeval tv;
      struct timeval * tvp = NULL;
      if(timeout >= 0)
        {
          tvp = &tv;
          tv.tv_sec = (int)timeout;
          tv.tv_usec = (int)((timeout-tv.tv_sec)*1000000);
        }
      if(select(maxsock+1, &readfds, NULL, NULL, tvp) == -1)
        {
          perror("select");
          return false;
        }
      ready.clear();
      for(unsigned int i = 0; i < sockets.size(); i++)
        {
          if(FD_ISSET(sockets[i]->GetSocket(), &readfds))
            ready.push_back(sockets[i]);
        }
      return true;
    }
    bool Socket::IsConnected() const
    {
      return (mSocket > -1) && mSocketOK;
    }
  }
}
