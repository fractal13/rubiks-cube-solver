#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    PerceptAtom::PerceptAtom()
    {
      name  = "";
      value = "";
    }
    PerceptAtom::PerceptAtom(const std::string &name_in,
                             const std::string &value_in)
    {
      name  = name_in;
      value = value_in;
    }
    const std::string &PerceptAtom::GetName() const
    {
      return name;
    }
    const std::string &PerceptAtom::GetValue() const
    {
      return value;
    }
    void PerceptAtom::SetValue(const std::string &value_in)
    {
      value = value_in;
    }
  }
}
