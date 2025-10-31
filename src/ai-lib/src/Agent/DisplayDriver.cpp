#include <ai_agent.h>
#include <Agent/DisplayDriver.h>
#include <typeinfo>
#include <cstdio>
#include <thread>

#include <ai_glut.h>

namespace ai
{
  namespace Agent
  {
    static struct Option options[] =
      {
        {"host",   'h', "localhost", "Environment hostname or IP address"},
        {"port",   'p', "7777",      "Environment TCP port"},
        {"width",  'W', "700",       "Width of display"},
        {"height", 'H', "700",       "Height of display"},
        {"display", 'd', "0", "Type of display, (0) redraw on messages or (1) redraw all the time"},
        {"", 0, "", ""}
      };

    DisplayDriver::DisplayDriver(int argc, char **argv)
      : mEnvironment(0), mEnvironmentSet(false), mEnvironmentDisplay(0), mEnvironmentDisplaySet(false), mClose(false)
    {
      mOptions.AddOptions(options);
      mOptions.Initialize(argc, argv);
    }

    DisplayDriver::~DisplayDriver()
    {
      if(mEnvironmentDisplay)
        {
          delete mEnvironmentDisplay;
          mEnvironmentDisplay = 0;
        }
      if(mEnvironment)
        {
          delete mEnvironment;
          mEnvironment = 0;
        }
    }

    void DisplayDriver::SetEnvironment(Environment * env)
    {
      mEnvironment = env;
    }
    
    void DisplayDriver::DrawFrame()
    {
      while(1)
        {
          // wait for an environment update to be completed by other thread.
          while ( !mEnvironmentSet )
            {
            }
          
          // create the environment display instance in this thread.
          SetEnvironmentDisplay(mEnvironment);
          
          // make sure it was successfully set
          if ( !mEnvironmentDisplaySet )
            {
              DBGP_E(LOG_STREAM(std::cerr) << "mEnvironmentDisplay is not set." << std::endl);
              exit(1);
            }
          
          // update frames until game is over
          while ( !mClose )
            {
              if( mEnvironmentDisplay )
                {
                  mEnvironmentDisplay->Redraw();
                }
            }
        }
    }

    void DisplayDriver::GetEnvironment()
    {
      std::string buffer;
      Message imsg, omsg;
      while(1)
        {

          if(!mSocket.Recv(buffer))
            {
              DBGP_DPY(LOG_STREAM(std::cerr) << "!Recv()" << std::endl);
              mClose = true;
              break;
            }
          
          if(!imsg.SetFromString(buffer))
            {
              DBGP_DPY(LOG_STREAM(std::cerr) << "!SetFromString()" << std::endl);
              mClose = true;
              break;
            }

          if(imsg.GetName() == MESSAGE_IDENT_NAME)
            {
              DisplayMessage tmsg;
              if(!mSocket.Send(tmsg.ToString()))
                {
                  DBGP_DPY(LOG_STREAM(std::cerr) << "!Send()" << std::endl);
                  mClose = true;
                  break;
                }
            }
          else if(imsg.GetName() == MESSAGE_DIE_NAME)
            {
              mClose = true;
              delete mEnvironmentDisplay;
              mEnvironmentDisplay = NULL;
              mEnvironmentDisplaySet = false;
              mEnvironmentSet = false;
              if(mEnvironment) 
                {
                  mEnvironment->Reset();
                }
              DBGP_DPY(LOG_STREAM(std::cout) << "DIE" << std::endl);
            }
          else if(imsg.GetName() == MESSAGE_UPDATE_NAME)
            {
              int display = mOptions.GetArgInt("display");
              if(!mEnvironment)
                {
                  DBGP_E(LOG_STREAM(std::cerr) << "No environment instance to update!" << std::endl);
                  exit(1);
                }
              mEnvironment->UpdateFromMessage(imsg);
              if(!mEnvironmentSet)
                {
                  mEnvironmentSet = true;
                  DBGP_DPY(LOG_STREAM(std::cout) << "!mEnvironmentSet" << std::endl);
                }
              else if(mEnvironmentDisplaySet && display != 0)
                {
                  DBGP_DPY(LOG_STREAM(std::cerr) << "SetEnv()" << std::endl);
                  mEnvironmentDisplay->SetEnv(mEnvironment);
                }

              if(!mEnvironmentDisplaySet && display == 0)
                {
                  DBGP_DPY(LOG_STREAM(std::cout) << "!mEnvironmentDisplaySet" << std::endl);
                  SetEnvironmentDisplay(mEnvironment);
                }
              
              if(display == 0 && mEnvironmentDisplaySet)
                {
                  DBGP_DPY(LOG_STREAM(std::cout) << "Redraw() start." << std::endl);
                  mEnvironmentDisplay->Redraw();
                  DBGP_DPY(LOG_STREAM(std::cout) << "Redraw() done." << std::endl);
                }
            }
          else
            {
              mClose = true;
              DBGP_E(LOG_STREAM(std::cerr) << "Unexpected command (" << imsg.GetName() << ") from environment!  Quitting." << std::endl);
              break;
            }
        }
    }

    void DisplayDriver::Run()
    {
      if(!mSocket.Connect(mOptions.GetArg("host"),mOptions.GetArgInt("port")))
        exit(1);
      std::cerr << "Display connected! Waiting for server..." << std::endl;

      if(mOptions.GetArgInt("display") == 0)
        {
          GetEnvironment();
        }
      else
        {
          std::thread( &DisplayDriver::DrawFrame, this ).detach();
          std::thread t( &DisplayDriver::GetEnvironment, this );
          t.join();
        }
    }
  }
}
