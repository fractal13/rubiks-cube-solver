#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {

    AgentProgram::AgentProgram()
    {
    }
    AgentProgram::~AgentProgram()
    {
    }
    bool AgentProgram::TextDisplay(std::ostream &os) const
    {
      os << name;
      return true;
    }
    void AgentProgram::SetName(const std::string &name_in)
    {
      name = name_in;
    }
    const std::string & AgentProgram::GetName() const
    {
      return name;
    }
  }
}
