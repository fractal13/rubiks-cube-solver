#ifndef _ACTION_H_
#define _ACTION_H_
#include <ostream>
#include <string>

namespace ai
{
  namespace Agent
  {
    class Action;
    std::ostream & operator<<(std::ostream &os, const Action * action);

    class Action
    {
    public:
      Action();
      virtual ~Action();
      virtual bool TextDisplay(std::ostream &os) const = 0;
      int GetCode() const;
      void SetCode(int code_in);
      virtual bool ToMessage(Message &omsg) const = 0;
      virtual bool FromMessage(const Message &imsg) = 0;
      // Add Object's data to omsg, if values are different than stored in old_msg
      virtual bool AddToMessageIfChanged(const std::string &keyprefix, ai::Agent::Message &omsg, ai::Agent::Message &old_msg) = 0;
      // Set Object's data from imsg, if values associated with id are present
      virtual bool SetFromMessageIfExists(const std::string &keyprefix, ai::Agent::Message &imsg) = 0;
    protected:
      int action_code;
    private:
    };
  }
}

#endif /* _ACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
