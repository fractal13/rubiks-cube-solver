#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    static struct Option options[] =
      {
        {"agent", 'a', "?", "Agent program"},
        {"host",  'h', "localhost", "Environment hostname or IP address"},
        {"port",  'p', "7777",      "Environment TCP port"},
        {"trace", 't', "1", "Enable/disable tracing"},
        {"user1", 'U', "0", "User variable 1"},
        {"user2", 'V', "0", "User variable 2"},
        {"user3", 'W', "0", "User variable 3"},
        {"user4", 'X', "0", "User variable 4"},
        {"user5", 'Y', "0", "User variable 5"},
        {"user6", 'Z', "0", "User variable 6"},
        {"", 0, "", ""}
      };

    AgentDriver::AgentDriver(int argc, char **argv)
    {
      mOptions.AddOptions(options);
      mOptions.Initialize(argc, argv);
      mAgentProgram = AgentProgramSelect(&mOptions);
    }

    AgentDriver::~AgentDriver( ) {
      if( mAgentProgram ) {
        delete mAgentProgram;
        mAgentProgram = 0;
      }
    }
    
    void AgentDriver::Run()
    {
      if(!mSocket.Connect(mOptions.GetArg("host"),mOptions.GetArgInt("port")))
        exit(1);
      std::cerr << "Connected! Waiting for environment..." << std::endl;

      std::string buffer;
      Message imsg, omsg;
      while(1)
        {
          if(!mAgentProgram)
            mAgentProgram = AgentProgramSelect(&mOptions);
          if(!mSocket.Recv(buffer) || !imsg.SetFromString(buffer))
            {
              break;
            }
          if(imsg.GetName() == MESSAGE_IDENT_NAME)
            {
              AgentMessage tmsg;
              if(!mSocket.Send(tmsg.ToString()))
                break;
            }
          else if(imsg.GetName() == MESSAGE_DIE_NAME)
            {
              delete mAgentProgram;
              mAgentProgram = 0;
            }
          else if(imsg.GetName() == MESSAGE_NAME_NAME)
            {
              AgentNameMessage tmsg;
              if(!tmsg.AddKeyValue(MESSAGE_NAME_NAME, mAgentProgram->GetName()))
                {
                  std::cerr << "Bad agent name message to server.  Quitting." << std::endl;
                  break;
                }
              if(!mSocket.Send(tmsg.ToString()))
                break;
            }
          else if(imsg.GetName() == MESSAGE_PERCEPT_NAME)
            {
              Percept percept;
              if(!percept.FromMessage(imsg))
                {
                  std::cerr << "Bad percept from server.  Quitting." << std::endl;
                  break;
                }
              Action * action = mAgentProgram->Program(&percept);
              if(mOptions.GetArgInt("trace"))
                {
                  std::cout << mAgentProgram->GetName() << " perceives "
                            << &percept << " and does "
                            << action << std::endl;
                }

              if(action)
                {
                  omsg.Clear();
                  action->ToMessage(omsg);
                  delete action;
                  action = 0;
                  if(!mSocket.Send(omsg.ToString()))
                    {
                      break;
                    }
                }
            }
          else
            {
              std::cerr << "Unexpected command from environment!" << std::endl;
              break;
            }
        }
    }
  }
}
