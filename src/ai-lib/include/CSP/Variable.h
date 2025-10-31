#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Variable
    {
    public:
      /* empty string name */
      Variable();

      /* set the name */
      Variable(const std::string &name_in);

      /* copy constructor */
      Variable(const Variable &rhs);

      /* assignment operator */
      Variable &operator=(const Variable &rhs);

      /* virtual to allow destructor chaining */
      virtual ~Variable();

      /* allow for comparison */
      virtual bool operator<(const Variable &rhs) const;

      /* set the name */
      void setName(std::string &name_in);

      /* get the name */
      const std::string &getName() const;

    protected:
      std::string name;
    private:
    };
  }
}

#endif /* _VARIABLE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
