#ifndef _CGLHLAPROBLEM_H_
#define _CGLHLAPROBLEM_H_

#include "ai_search.h"
#include "cglHLAAction.h"
#include "cglHLAState.h"

#include "cglCubeProblem.h"

namespace cgl {
  namespace cube {

    class HLAProblem : public Problem {
    public:
      HLAProblem( ai::Search::State *initial_state_in, const RubiksCube& goal_cube_in );
      virtual ~HLAProblem( );

      virtual std::vector<ai::Search::Action *> Actions( const ai::Search::State * const state_in );
      virtual ai::Search::State *Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in );

      virtual double StepCost( const ai::Search::State  * const state1_in,
                               const ai::Search::Action * const action_in,
                               const ai::Search::State  * const state2_in ) const;
      
    protected:
      
    private:
      
    };

  }
}

#endif /* _CGLHLAPROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
