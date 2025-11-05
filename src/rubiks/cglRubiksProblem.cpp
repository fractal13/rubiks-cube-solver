#include "cglRubiksProblem.h"
#include <cstdlib>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>


namespace cgl {
  namespace rubiks {

    Problem::Problem( ai::Search::State *initial_state_in, const std::vector< Cube >& goal_cube_in )
      : ai::Search::Problem( initial_state_in ), mGoalCubes( goal_cube_in ),
        mHalfTurns( false ), mSliceTurns( false ),
        mHeuristic( FACELET_HEURISTIC ) {
      // empty
    }

    Problem::~Problem( ) {
      // empty
    }
    
    bool Problem::GoalTest( const ai::Search::State * const state_in ) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      for( auto it = mGoalCubes.begin( ); it != mGoalCubes.end( ); it++ ) {
        if( state->getCube( ) == *it ) {
          return true;
        }
      }
      return false;
    }

    std::vector<ai::Search::Action *> Problem::Actions( const ai::Search::State * const /*state_in*/ ) {
      static const std::map< std::string, Move > basic_moves = {
        /* { "I", Move( Move::M_IDENTITY ) }, */
        { "U", Move( Move::M_U ) }, { "U'", Move( Move::M_UP ) },
        { "L", Move( Move::M_L ) }, { "L'", Move( Move::M_LP ) },
        { "F", Move( Move::M_F ) }, { "F'", Move( Move::M_FP ) },
        { "R", Move( Move::M_R ) }, { "R'", Move( Move::M_RP ) },
        { "B", Move( Move::M_B ) }, { "B'", Move( Move::M_BP ) },
        { "D", Move( Move::M_D ) }, { "D'", Move( Move::M_DP ) },
      };
      static const std::map< std::string, Move > half_turn_moves = {
        { "U2", Move( Move::M_U ) * Move( Move::M_U ) },
        { "L2", Move( Move::M_L ) * Move( Move::M_L ) },
        { "F2", Move( Move::M_F ) * Move( Move::M_F ) },
        { "R2", Move( Move::M_R ) * Move( Move::M_R ) },
        { "B2", Move( Move::M_B ) * Move( Move::M_B ) },
        { "D2", Move( Move::M_D ) * Move( Move::M_D ) },
      };
      static const std::map< std::string, Move > slice_moves = {
        { "E", Move( Move::M_EQ) }, { "E'", Move( Move::M_EP ) },
        { "M", Move( Move::M_M ) }, { "M'", Move( Move::M_MP ) },
        { "S", Move( Move::M_S ) }, { "S'", Move( Move::M_SP ) },
      };
      static const std::map< std::string, Move > half_turn_slice_moves = {
        { "E2", Move( Move::M_EQ) * Move( Move::M_EQ) },
        { "M2", Move( Move::M_M ) * Move( Move::M_M ) },
        { "S2", Move( Move::M_S ) * Move( Move::M_S ) },
      };

      std::vector<ai::Search::Action *> actions;
      if( mAllowedMoves.size( ) == 0 ) {
        for( auto it = basic_moves.begin( ); it != basic_moves.end( ); it++ ) {
          actions.push_back( new Action( it->second ) );
        }
        if( mHalfTurns ) {
          for( auto it = half_turn_moves.begin( ); it != half_turn_moves.end( ); it++ ) {
            actions.push_back( new Action( it->second ) );
          }
        }
        if( mSliceTurns ) {
          for( auto it = slice_moves.begin( ); it != slice_moves.end( ); it++ ) {
            actions.push_back( new Action( it->second ) );
          }
        }
        if( mHalfTurns && mSliceTurns ) {
          for( auto it = half_turn_slice_moves.begin( ); it != half_turn_slice_moves.end( ); it++ ) {
            actions.push_back( new Action( it->second ) );
          }
        }
      } else {
        for( auto it = mAllowedMoves.begin( ); it != mAllowedMoves.end( ); it++ ) {
          actions.push_back( new Action( it->second ) );
        }
      }
      
      std::shuffle( actions.begin( ), actions.end( ), std::default_random_engine(std::chrono::steady_clock::now().time_since_epoch().count()) );
      return actions;
    }
    
    ai::Search::State *Problem::Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in ) {
      const State * const state = dynamic_cast< const State * const >( state_in );
      const Action * const action = dynamic_cast< const Action * const >( action_in );

      return new State( action->getMove( ) * state->getCube( ) );
    }

    double Problem::StepCost( const ai::Search::State  * const /*state1_in*/,
                              const ai::Search::Action * const action_in,
                              const ai::Search::State  * const /*state2_in*/ ) const {
      const Action * const action = dynamic_cast< const Action * const >( action_in );
      return action->getMove( ).getCost( );
    }

    double Problem::Heuristic(const ai::Search::State  * const state_in) const {
      //const State * const state = dynamic_cast< const State * const >( state_in );
      double h = 0.0;
      switch( mHeuristic ) {
      case ZERO_HEURISTIC:
      default: // fall through
        h = zeroHeuristic( state_in );
        break;
      case FACELET_HEURISTIC:
        h = faceletHeuristic( state_in );
        break;
      case EDGE_FACELET_HEURISTIC:
        h = edgeFaceletHeuristic( state_in );
        break;
      case CORNER_FACELET_HEURISTIC:
        h = cornerFaceletHeuristic( state_in );
        break;
      case FACELET_MAX_HEURISTIC:
        h = faceletMaxHeuristic( state_in );
        break;
      case COLOR_COUNT_HEURISTIC:
        h = colorCountHeuristic( state_in );
        break;
      }
      return h;
    }

    const std::vector< Cube >& Problem::getGoalCubes( ) const {
      return mGoalCubes;
    }

    bool Problem::halfTurns( ) const {
      return mHalfTurns;
    }
    void Problem::enableHalfTurns( ) {
      mHalfTurns = true;
    }
    void Problem::disableHalfTurns( ) {
      mHalfTurns = false;
    }
    
    bool Problem::sliceTurns( ) const {
      return mSliceTurns;
    }
    void Problem::enableSliceTurns( ) {
      mSliceTurns = true;
    }
    void Problem::disableSliceTurns( ) {
      mSliceTurns = false;
    }

    Problem::HeuristicEnum Problem::getHeuristic( ) const {
      return mHeuristic;
    }
    
    void Problem::setHeuristic( const HeuristicEnum& heuristic ) {
      mHeuristic = heuristic;
    }

    void Problem::setAllowedMoves( const std::map< std::string, Move >& allowed_moves ) {
      mAllowedMoves = allowed_moves;
    }

    double Problem::zeroHeuristic(const ai::Search::State  * const /*state_in*/) const {
      return 0;
    }

    double Problem::faceletHeuristic(const ai::Search::State  * const state_in) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      const std::vector< Facelet >& cube = state->getCube( ).getFacelets( );

      double best_count = 2 * cube.size( );
        
      for( auto it = mGoalCubes.begin( ); it != mGoalCubes.end( ); it++ ) {
        const std::vector< Facelet >& goal = it->getFacelets( );
        double count = 0.0;
        unsigned int i;
        for( i = 0; i < cube.size( ); i++ ) {
          if( goal[ i ] != '*' && cube[ i ] != goal[ i ] ) {
            count += 1.0;
          }
        }
        if( count < best_count ) {
          best_count = count;
        }
      }
      return best_count / 20.0;
    }
    
    double Problem::edgeFaceletHeuristic(const ai::Search::State  * const state_in) const {
      const std::vector<unsigned int> edge_indexes = { 1,3,5,7, 10,12,14,16, 19,21,23,25, 28,30,32,34, 37,39,41,43, 46,48,50,52 };
      const State * const state = dynamic_cast< const State * const >( state_in );
      const std::vector< Facelet >& cube = state->getCube( ).getFacelets( );
      
      double best_count = 2 * cube.size( );
        
      for( auto it = mGoalCubes.begin( ); it != mGoalCubes.end( ); it++ ) {
        const std::vector< Facelet >& goal = it->getFacelets( );
        double count = 0.0;
        unsigned int i;
        for( i = 0; i < edge_indexes.size( ); i++ ) {
          if( goal[ edge_indexes[i] ] != '*' && cube[ edge_indexes[i] ] != goal[ edge_indexes[i] ] ) {
            count += 1.0;
          }
        }
        if( count < best_count ) {
          best_count = count;
        }
      }
      return best_count / 8.0;
    }

    double Problem::cornerFaceletHeuristic(const ai::Search::State  * const state_in) const {
      const std::vector<unsigned int> corner_indexes = { 0,2,6,8, 9,11,15,17, 18,20,24,26, 27,29,33,35, 36,38,42,44, 45,47,51,53 };
      const State * const state = dynamic_cast< const State * const >( state_in );
      const std::vector< Facelet >& cube = state->getCube( ).getFacelets( );
      
      double best_count = 2 * cube.size( );
        
      for( auto it = mGoalCubes.begin( ); it != mGoalCubes.end( ); it++ ) {
        const std::vector< Facelet >& goal = it->getFacelets( );
        double count = 0.0;
        unsigned int i;
        for( i = 0; i < corner_indexes.size( ); i++ ) {
          if( goal[ corner_indexes[i] ] != '*' && cube[ corner_indexes[i] ] != goal[ corner_indexes[i] ] ) {
            count += 1.0;
          }
        }
        if( count < best_count ) {
          best_count = count;
        }
      }
      return best_count / 8.0;
    }

    double Problem::faceletMaxHeuristic(const ai::Search::State  * const state_in) const {
      return std::max( edgeFaceletHeuristic( state_in ), cornerFaceletHeuristic( state_in ) );
    }

    double Problem::colorCountHeuristic(const ai::Search::State  * const state_in) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      const std::vector< Facelet >& cube = state->getCube( ).getFacelets( );
      unsigned int i, j;
      double total = 0.0;
      std::map<Facelet,int> counts;
      
      for(i = 0; i < 6; i++) {
        counts.clear();
        for(j = 0; j < 9; j++ ) {
          counts[cube[i*9+j]] += 1;
        }
        //std::cout << "counts: " << counts.size() << std::endl;
        total += counts.size();
      }
      //std::cout << "total: " << total << std::endl;
      return total / 4.0;
    }

  }
}
