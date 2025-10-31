#ifndef _FIRSTCHOICE_H_
#define _FIRSTCHOICE_H_
#include "LocalSearch/Algorithm.h"

namespace ai
{
  namespace LocalSearch
  {
    class FirstChoice : public Algorithm
    {
    public:
      FirstChoice(Problem *problem_in);
      virtual ~FirstChoice();
      virtual bool Search();
    protected:
    private:
    };
  }
}

#endif /* _FIRSTCHOICE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
