#ifndef _AGENTPROGRAM_H_
#define _AGENTPROGRAM_H_
#include <string>
#include <ostream>


namespace ai
{
  namespace Agent
  {
    class AgentProgram
    {
    public:
      AgentProgram();
      virtual ~AgentProgram();
      virtual Action * Program(const Percept * percept) = 0;

      virtual bool TextDisplay(std::ostream & os) const;
      void SetName(const std::string &name_in);
      const std::string & GetName() const;
    protected:
      std::string name;
    private:
    };
  }
}

#endif /* _AGENTPROGRAM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
