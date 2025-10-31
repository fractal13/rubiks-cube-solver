#include "cglHLAProblem.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace cgl {
  namespace cube {

    HLAProblem::HLAProblem( ai::Search::State *initial_state_in, const RubiksCube& goal_cube_in )
      : Problem( initial_state_in, goal_cube_in ) {
    }

    HLAProblem::~HLAProblem( ) {
    }
    
    static int myrandom ( int i ) { return std::rand() % i; }

    static bool find_path( const RubiksCube& initial_cube, const RubiksCube& goal_cube, std::vector< std::pair< Color, Direction > >& rotations ) {
      rotations.clear( );
      rotations.resize( 0 );
      cgl::cube::State *initial_state = new cgl::cube::State( initial_cube );
      cgl::cube::Problem *problem = new cgl::cube::Problem( initial_state, goal_cube );
      problem->setHeuristic( cgl::cube::EDGE_CORNER_HEURISTIC );
      ai::Search::Frontier *frontier  = new ai::Search::AStarFrontier;;
      ai::Search::Algorithm *search = new ai::Search::Graph( problem, frontier );
      search->SetGenerationLimit( (int)(2e7) );
      search->SetStoreLimit( (int)(2e7) );
      if( search->Search() ) {
        std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
        std::list<ai::Search::Node *>::const_iterator it;
        for( it = solution->begin( ); it != solution->end( ); it++ ) {
          if( (*it)->GetAction( ) ) {
            cgl::cube::Action *action = dynamic_cast< cgl::cube::Action * >( (*it)->GetAction( ) );
            cgl::cube::Color face = 127;
            cgl::cube::Direction direction = cgl::cube::Direction::NO_DIRECTION;

            switch( action->getFace( ) ) {
            case cgl::cube::Action::FACE_WHITE: face = 0; break;
            case cgl::cube::Action::FACE_GREEN: face = 1; break;
            case cgl::cube::Action::FACE_RED: face = 2; break;
            case cgl::cube::Action::FACE_BLUE: face = 3; break;
            case cgl::cube::Action::FACE_ORANGE: face = 4; break;
            case cgl::cube::Action::FACE_YELLOW: face = 5; break;
            default: break;
            }
            
            switch( action->getDirection( ) ) {
            case cgl::cube::Action::ROTATE_CW: direction = cgl::cube::Direction::CLOCK_WISE; break;
            case cgl::cube::Action::ROTATE_CCW: direction = cgl::cube::Direction::COUNTER_CLOCK_WISE; break;
            default: break;
            }
            rotations.push_back( std::pair< Color, Direction >( face, direction ) );
          }
        }
        delete search;
        return true;
      } else {
        rotations.resize( 0 );
        delete search;
        return false;
      }
    }

    std::vector<ai::Search::Action *> HLAProblem::Actions( const ai::Search::State * const state_in ) {
      //std::cout << "HLAProblem::Actions() started." << std::endl;
      const HLAState * const state = dynamic_cast< const HLAState * const >( state_in );
      const RubiksCube& cube = state->getCube( );

      std::vector<ai::Search::Action *> actions;
      std::vector< std::pair< Color, Direction > > rotations;

#if STAY_SOLVED_CUBIES
      /* For each solved cubie, add it to the goal, if it isn't already there */
      RubiksCube solved_cubies;
      solved_cubies.setFromOneLineDisplay( "********* ********* ********* ********* ********* *********" );
      for( auto it = HLAAction::getCubies( ).begin( ); it != HLAAction::getCubies( ).end( ); it++ ) {
        if( (*it) == cube ) {
          solved_cubies.mergeWildcard( *it );
        }
      }
#endif // STAY_SOLVED_CUBIES

      /* For each unplaced cubie, do a limited A* search, if a solution is found, record the solution and add it as an action */
      for( auto it = HLAAction::getCubies( ).begin( ); it != HLAAction::getCubies( ).end( ); it++ ) {
        if( (*it) != cube ) {
          // this cubie has not been solved yet.
          // Second, search for placement rotations
          //TODO( );
          RubiksCube next_goal = state->getCurrentGoal( );
#if STAY_SOLVED_CUBIES
          next_goal.mergeWildcard( solved_cubies );
#endif // STAY_SOLVED_CUBIES
          next_goal.mergeWildcard( *it );
          rotations.resize( 0 );
          if( find_path( cube, next_goal, rotations ) ) {
            //std::cout << "adding action: " << it->oneLineDisplay( ) << std::endl;
            //std::cout << state->getCurrentGoal( ).oneLineDisplay( ) << std::endl;
            //std::cout << it->oneLineDisplay( ) << std::endl;
            std::cout << "made it this far: " << next_goal.oneLineDisplay( ) << std::endl;
            actions.push_back( new HLAAction( rotations, next_goal ) );
          } else {
            //std::cout << "unsolved: " << next_goal.oneLineDisplay( ) << std::endl;
          }
        }
      }
      
      // shuffle the actions to not prefer particular cubies
      std::random_shuffle( actions.begin( ), actions.end( ), myrandom );
      //std::cout << "HLAProblem::Actions() finished." << std::endl;
      return actions;
    }
    
    ai::Search::State *HLAProblem::Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in ) {
      //std::cout << "HLAProblem::Result() started." << std::endl;
      const HLAState * const state = dynamic_cast< const HLAState * const >( state_in );
      const HLAAction * const action = dynamic_cast< const HLAAction * const >( action_in );

      RubiksCube cube = state->getCube( );
      for( auto it = action->getRotations( ).begin( ); it != action->getRotations( ).end( ); it++ ) {
        Color color = it->first;
        Direction direction = it->second;
        cube.rotate( color, direction );
      }
      
      //std::cout << "new cube:         " << cube.oneLineDisplay( ) << std::endl;
      //std::cout << "current_goal:     " << action->getCurrentGoal( ).oneLineDisplay( ) << std::endl;
      //std::cout << "HLAProblem::Result() finished." << std::endl;
      return new HLAState( cube, action->getCurrentGoal( ) );
    }

    double HLAProblem::StepCost( const ai::Search::State  * const state1_in,
                                 const ai::Search::Action * const action_in,
                                 const ai::Search::State  * const state2_in ) const {
      //std::cout << "HLAProblem::StepCost() started." << std::endl;
      const HLAAction * const action = dynamic_cast< const HLAAction * const >( action_in );
      //std::cout << "HLAProblem::StepCost() finished." << std::endl;
      return action->getCost( );
    }
    
  }
}
