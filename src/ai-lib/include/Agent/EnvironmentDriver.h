#ifndef _ENVIRONMENTDRIVER_H_
#define _ENVIRONMENTDRIVER_H_
#include <vector>

namespace ai
{
  namespace Agent
  {
    class EnvironmentDriver
    {
    public:
      EnvironmentDriver(int argc, char **argv, bool init_options = true);
      virtual ~EnvironmentDriver();
      void InitRand();
      void InitSockets(int num_agents, int num_displays);
      virtual void Run() = 0;
    protected:
      Options mOptions;
      RandomNumber mRandomNumber;
      std::vector<Socket *> mAgentSockets;
      std::vector<Socket *> mDisplaySockets;
      Socket * mListenSocket;
    private:
    };
  }
}

#endif /* _ENVIRONMENTDRIVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
