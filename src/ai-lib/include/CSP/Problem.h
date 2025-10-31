#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <vector>
#include <map>
#include <string>
#include <set>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Problem
    {
    public:
      /* virtual to allow destructor chaining */
      virtual ~Problem();

    protected:
      std::map<Variable, int>    variables;
      std::map<Variable, Domain> domains;
      std::vector<Constraint>    constraints;
    private:
    };
  }
}

#endif /* _PROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
