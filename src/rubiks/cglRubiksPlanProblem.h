#ifndef _CGLRUBIKSPLANPROBLEM_H_
#define _CGLRUBIKSPLANPROBLEM_H_

#include "ai_search.h"
#include "cglRubiksAction.h"
#include "cglRubiksState.h"
#include "cglRubiksPlanAction.h"
#include <map>
#include <vector>
#include <string>

namespace cgl {
  namespace rubiks {

    class PlanProblem : public ai::Search::Problem {
    public:
  
      PlanProblem( ai::Search::State *initial_state_in, const PlanActionSet& action_set, const std::vector< Cube >& goal_cubes_in );
      virtual ~PlanProblem( );

      virtual bool GoalTest( const ai::Search::State * const state_in ) const;
      virtual std::vector<ai::Search::Action *> Actions( const ai::Search::State * const state_in );
      virtual ai::Search::State *Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in );

      virtual double StepCost( const ai::Search::State  * const state1_in,
                               const ai::Search::Action * const action_in,
                               const ai::Search::State  * const state2_in ) const;

      virtual double Heuristic(const ai::Search::State  * const state_in) const;

      const std::vector< Cube >& getGoalCubes( ) const;

    protected:
      std::vector< Cube > mGoalCubes;
      const PlanActionSet& mActionSet;

    private:
      
    };

  }
}

#endif /* _CGLRUBIKSPLANPROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
