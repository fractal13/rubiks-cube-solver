#ifndef _HILLCLIMBING_H_
#define _HILLCLIMBING_H_
#include "LocalSearch/Algorithm.h"

namespace ai
{
  namespace LocalSearch
  {
    class HillClimbing : public Algorithm
    {
    public:
      HillClimbing(Problem *problem_in);
      virtual ~HillClimbing();
      virtual bool Search();
    protected:
    private:
    };
  }
}

#endif /* _HILLCLIMBING_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
