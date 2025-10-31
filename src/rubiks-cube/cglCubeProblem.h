#ifndef _CGLCUBEPROBLEM_H_
#define _CGLCUBEPROBLEM_H_

#include "ai_search.h"
#include "cglCubeAction.h"
#include "cglCubeState.h"

namespace cgl {
  namespace cube {

    enum HeuristicEnum { EDGE_HEURISTIC, CORNER_HEURISTIC, TILE_HEURISTIC, END_GAME_HEURISTIC, EDGE_CORNER_HEURISTIC, ALL_HEURISTIC, ZERO_HEURISTIC };

    class Problem : public ai::Search::Problem {
    public:
      Problem( ai::Search::State *initial_state_in, const RubiksCube& goal_cube_in );
      virtual ~Problem( );

      virtual bool GoalTest( const ai::Search::State * const state_in ) const;
      virtual std::vector<ai::Search::Action *> Actions( const ai::Search::State * const state_in );
      virtual ai::Search::State *Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in );

      virtual double StepCost( const ai::Search::State  * const state1_in,
                               const ai::Search::Action * const action_in,
                               const ai::Search::State  * const state2_in ) const;

      virtual double Heuristic(const ai::Search::State  * const state_in) const;

      const RubiksCube& getGoalCube( ) const;
#if ALLOW_180
      bool getAllow180( ) const;
      void allow180( );
      void disallow180( );
#endif
#if ALLOW_MIDDLE_ROTATE
      bool getAllowMiddleRotate( ) const;
      void allowMiddleRotate( );
      void disallowMiddleRotate( );
#endif
      
      HeuristicEnum getHeuristic( ) const;
      void setHeuristic( const HeuristicEnum& heuristic );
      
      
    protected:
      State mGoalState;

#if ALLOW_180
      bool mAllow180;
#endif
#if ALLOW_MIDDLE_ROTATE
      bool mAllowMiddleRotate;
#endif
      HeuristicEnum mHeuristic;
      
    private:
      
    };

  }
}

#endif /* _CGLCUBEPROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
