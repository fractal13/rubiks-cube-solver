#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    static struct Option options[] =
      {
        {"delay",    'd', "1",    "Number of seconds to delay per step"},
        {"agents",   'a', "1",    "The number of agents to connect"},
        {"displays", 'D', "1",    "Number of display connections to accept"},
        {"port",     'p', "7777", "TCP listening port for all connections"},
        {"runs",     'R', "1",    "Number of runs"},
        {"sleep",    'S', "1",    "Number of seconds to sleep between runs"},
        {"steps",    's', "1000", "Number of steps allowed per agent"},
        {"trace",    't', "1",    "Enable/disable agent tracing"},
        {"seed",     'z', "time", "Random seed"},
        {"", 0, "", ""}
      };

    EnvironmentDriver::EnvironmentDriver(int argc, char **argv, bool init_options)
    {
      mListenSocket = new Socket();
      mOptions.AddOptions(options);
      if(init_options)
        {
          mOptions.Initialize(argc, argv);
          InitRand();
        }
    }

    EnvironmentDriver::~EnvironmentDriver()
    {
      unsigned int i;
      for(i = 0; i < mAgentSockets.size(); i++)
        delete mAgentSockets[i];
      for(i = 0; i < mDisplaySockets.size(); i++)
        delete mDisplaySockets[i];
      delete mListenSocket;
    }

    void EnvironmentDriver::InitRand()
    {
      int seed = mOptions.GetArgInt("seed");
      if(!seed)
        seed = static_cast<int>(time(0));
      std::cerr << "Random seed is: " << seed << std::endl;
      srand(seed);
      RandomNumber r(seed);
      mRandomNumber = r;
    }

    void EnvironmentDriver::InitSockets(int num_agents, int num_displays)
    {
      if(!mListenSocket->Listen(mOptions.GetArgInt("port")))
        exit(1);

      std::string buffer;
      IdentMessage identmsg;
      Message imsg;
      while(num_agents > 0 || num_displays > 0)
        {
          std::cerr << "Waiting for " << num_agents << " agent(s) and " << num_displays << " display(s) to connect..." << std::endl;
          Socket * client = mListenSocket->Accept();
          if(!client)
            {
              continue;
            }
          if(!client->Send(identmsg.ToString()) || !client->Recv(buffer) || !imsg.SetFromString(buffer))
            {
              delete client;
              continue;
            }
          if(imsg.GetName() == MESSAGE_AGENT_NAME)
            {
              if(num_agents > 0)
                {
                  mAgentSockets.push_back(client);
                  num_agents--;
                }
              else
                {
                  std::cerr << "Extra agent tried to connect." << std::endl;
                  delete client;
                }
            }
          else if(imsg.GetName() == MESSAGE_DISPLAY_NAME)
            {
              if(num_displays > 0)
                {
                  mDisplaySockets.push_back(client);
                  num_displays--;
                }
              else
                {
                  std::cerr << "Extra display tried to connect." << std::endl;
                  delete client;
                }
            }
          else
            {
              std::cerr << "Bad message from connecting client." << std::endl;
              delete client;
            }
        }
      std::cerr << "All agents and displays connected" << std::endl;
    }
  }
}
