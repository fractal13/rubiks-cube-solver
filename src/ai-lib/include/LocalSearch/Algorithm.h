#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "LocalSearch/Problem.h"

namespace ai
{
  namespace LocalSearch
  {
    class Algorithm
    {
    public:
      enum AlgorithmType
        {
          T_HillClimbing,
          T_FirstChoice,
          T_SimulatedAnnealing
        };
      Algorithm(Problem *problem_in);
      virtual ~Algorithm();
      static Algorithm *MakeAlgorithm(AlgorithmType algorithm_type, Problem *the_problem);
      virtual bool Search() = 0;
      virtual bool RandomRestartSearch(int max_restarts);
      /* Do not delete this pointer */
      const State * const GetSolution() const;
      int GetRestartCount() const;
      int GetStatesGenerated() const;

      /* Set limits to stop run-away searches.
       */
      bool SetGenerationLimit(int limit);
      
    protected:
      Problem *problem;
      State   *solution;
      int      restarts;
      int      states_generated;

      /* Search limits.
       */
      int      generation_limit;

    private:
    };
  }
}

#endif /* _ALGORITHM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
