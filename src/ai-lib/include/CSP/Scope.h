#ifndef _SCOPE_H_
#define _SCOPE_H_

#include <vector>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Scope
    {
    public:
      /* empty scope */
      Scope();

      /* set scope from vector */
      Scope(const std::vector<Variable> &variables_in);

      /* copy constructor */
      Scope(const Scope &rhs);

      /* assignment operator */
      Scope &operator=(const Scope &rhs);

      /* virtual to allow destructor chaining */
      virtual ~Scope();

      /* set the variables */
      void setVariables(const std::vector<Variable> &variables_in);

      /* get the variables */
      const std::vector<Variable> &getVariables() const;

    protected:
      std::vector<Variable> variables;
    private:
    };
  }
}

#endif /* _SCOPE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
