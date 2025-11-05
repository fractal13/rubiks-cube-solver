#include "cglRubiksPlanProblem.h"
#include <cstdlib>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>


namespace cgl {
  namespace rubiks {

    PlanProblem::PlanProblem( ai::Search::State *initial_state_in, const PlanActionSet& action_set, const std::vector< Cube >& goal_cube_in )
      : ai::Search::Problem( initial_state_in ),
      mGoalCubes( goal_cube_in ), 
      mActionSet( action_set ) {
      // empty
    }

    PlanProblem::~PlanProblem( ) {
      // empty
    }
    
    bool PlanProblem::GoalTest( const ai::Search::State * const state_in ) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      for( auto it = mGoalCubes.begin( ); it != mGoalCubes.end( ); it++ ) {
        if( state->getCube( ) == *it ) {
          return true;
        }
      }
      return false;
    }

    std::vector<ai::Search::Action *> PlanProblem::Actions( const ai::Search::State * const state_in ) {
      const State * const state = dynamic_cast< const State * const >( state_in );
      const Cube& cube = state->getCube( );
      const std::map< std::string, PlanAction >& plan_actions = mActionSet.getDefinedActions( );
      std::vector<ai::Search::Action *> actions;
      for( auto it = plan_actions.begin( ); it != plan_actions.end( ); it++ ) {
        if( it->second.conditionsAreMet( cube ) ) {
          actions.push_back( new Action( it->second.getEffect( ) ) );
        }
      }
      std::shuffle( actions.begin( ), actions.end( ), std::default_random_engine(std::chrono::steady_clock::now().time_since_epoch().count()) );
      return actions;
    }
    
    ai::Search::State *PlanProblem::Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in ) {
      const State * const state = dynamic_cast< const State * const >( state_in );
      const Action * const action = dynamic_cast< const Action * const >( action_in );
      
      return new State( action->getMove( ) * state->getCube( ) );
    }
    
    double PlanProblem::StepCost( const ai::Search::State  * const /*state1_in*/,
                                  const ai::Search::Action * const action_in,
                                  const ai::Search::State  * const /*state2_in*/ ) const {
      const Action * const action = dynamic_cast< const Action * const >( action_in );
      return action->getMove( ).getCost( );
    }

    double PlanProblem::Heuristic(const ai::Search::State  * const /*state_in*/) const {
      //const State * const state = dynamic_cast< const State * const >( state_in );
      double h = 0.0;
      return h;
    }

    const std::vector< Cube >& PlanProblem::getGoalCubes( ) const {
      return mGoalCubes;
    }
    
  }
}
