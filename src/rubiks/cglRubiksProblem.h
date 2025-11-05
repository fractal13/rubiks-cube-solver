#ifndef _CGLRUBIKSPROBLEM_H_
#define _CGLRUBIKSPROBLEM_H_

#include "ai_search.h"
#include "cglRubiksAction.h"
#include "cglRubiksState.h"
#include <map>
#include <vector>
#include <string>

namespace cgl {
  namespace rubiks {

    class Problem : public ai::Search::Problem {
    public:
      //                   0                  1               2                       3                         4
      enum HeuristicEnum { FACELET_HEURISTIC, ZERO_HEURISTIC, EDGE_FACELET_HEURISTIC, CORNER_FACELET_HEURISTIC, FACELET_MAX_HEURISTIC,
                           COLOR_COUNT_HEURISTIC=10 };

      Problem( ai::Search::State *initial_state_in, const std::vector< Cube >& goal_cube_in );
      virtual ~Problem( );

      virtual bool GoalTest( const ai::Search::State * const state_in ) const;
      virtual std::vector<ai::Search::Action *> Actions( const ai::Search::State * const state_in );
      virtual ai::Search::State *Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in );

      virtual double StepCost( const ai::Search::State  * const state1_in,
                               const ai::Search::Action * const action_in,
                               const ai::Search::State  * const state2_in ) const;

      virtual double Heuristic(const ai::Search::State  * const state_in) const;

      const std::vector< Cube >& getGoalCubes( ) const;

      bool halfTurns( ) const;
      void enableHalfTurns( );
      void disableHalfTurns( );

      bool sliceTurns( ) const;
      void enableSliceTurns( );
      void disableSliceTurns( );

      HeuristicEnum getHeuristic( ) const;
      void setHeuristic( const HeuristicEnum& heuristic );
      void setAllowedMoves( const std::map< std::string, Move >& allowed_moves );
      
    protected:
      double zeroHeuristic(const ai::Search::State  * const state_in) const;
      double faceletHeuristic(const ai::Search::State  * const state_in) const;
      double edgeFaceletHeuristic(const ai::Search::State  * const state_in) const;
      double cornerFaceletHeuristic(const ai::Search::State  * const state_in) const;
      double faceletMaxHeuristic(const ai::Search::State  * const state_in) const;
      double colorCountHeuristic(const ai::Search::State  * const state_in) const;
      
      std::vector< Cube > mGoalCubes;

      bool mHalfTurns;
      bool mSliceTurns;
      std::map< std::string, Move > mAllowedMoves;

      HeuristicEnum mHeuristic;
      
    private:
      
    };

  }
}

#endif /* _CGLRUBIKSPROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
