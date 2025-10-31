#ifndef _SOCKET_H_
#define _SOCKET_H_
#include <string>
#include <vector>

namespace ai
{
  namespace Agent
  {
    class Socket
    {
    public:
      Socket();
      Socket(int socket_in);
      ~Socket();
      int GetSocket();
      Socket * Accept();
      std::vector<Socket *> AcceptMulti(int n);
      bool Connect(std::string hostname, int port);
      bool Listen(int port);
      bool Recv(std::string &msg);
      bool Send(const std::string &msg);
      static bool Select(std::vector<Socket *> &sockets, std::vector<Socket *> &ready, double timeout);
      bool IsConnected() const;
    protected:
      int mSocket;
      bool mSocketOK;
    private:
    };
  }
}

#endif /* _SOCKET_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
