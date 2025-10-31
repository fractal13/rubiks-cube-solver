#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    Action::Action()
    {
      action_code = -1;
    }
    Action::~Action()
    {
    }
    int Action::GetCode() const
    {
      return action_code;
    }
    void Action::SetCode(int code_in)
    {
      action_code = code_in;
    }

    std::ostream & operator<<(std::ostream &os, const Action * action)
    {
      if(action)
        {
          action->TextDisplay(os);
        }
      else
        {
          os << "NULL_ACTION";
        }
      return os;
    }
  }
}
