#ifndef _AGENTDRIVER_H_
#define _AGENTDRIVER_H_
#include <vector>

namespace ai
{
  namespace Agent
  {
    AgentProgram * AgentProgramSelect(Options * opts);

    class AgentDriver
    {
    public:
      AgentDriver(int argc, char **argv);
      ~AgentDriver( );
      void Run();
    protected:
      AgentProgram * mAgentProgram;
      Options mOptions;
      Socket mSocket;
    private:
    };
  }
}

#endif /* _AGENTDRIVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
