#ifndef _AGENT_H_
#define _AGENT_H_
#include <ostream>

namespace ai
{
  namespace Agent
  {

    class Environment;
    class Agent;
    std::ostream & operator<<(std::ostream &os, const Agent * agent);

    class Agent: public Object
    {
    public:
      Agent();
      Agent(Socket * sock_in);
      virtual ~Agent();
      virtual bool IsAlive() const;
      virtual void Kill();
      virtual Action * Program(const Percept *percept);
      virtual bool TextDisplay(std::ostream & os) const;
      virtual Action * MessageToAction(const Message &imsg) const = 0;
      // Add Object's data to omsg, if values are different than stored in old_msg
      virtual bool AddToMessageIfChanged(ai::Agent::Message &omsg, ai::Agent::Message &old_msg);
      // Set Object's data from imsg, if values associated with id are present
      virtual bool SetFromMessageIfExists(unsigned int id, ai::Agent::Message &imsg);
      void Trace();
      void NoTrace();
      void SetPerformance(int value_in);
      void AddPerformance(int value_in);
      int GetPerformance() const;
      Socket * GetSocket() const;
    protected:
      Socket * sock;
      bool alive;
      bool trace;
      int  performance;
    private:
    };
  }
}

#endif /* _AGENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
