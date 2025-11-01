#include "cglCubeProblem.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <chrono>

namespace cgl {
  namespace cube {

    Problem::Problem( ai::Search::State *initial_state_in, const RubiksCube& goal_cube_in )
      : ai::Search::Problem( initial_state_in ), mGoalState( goal_cube_in ),
#if ALLOW_180
      mAllow180( false ),
#endif
#if ALLOW_MIDDLE_ROTATE
      mAllowMiddleRotate( false ),
#endif
      mHeuristic( ALL_HEURISTIC ) {
    }

    Problem::~Problem( ) {
    }
    
    bool Problem::GoalTest( const ai::Search::State * const state_in ) const {
      return mGoalState.IsEqual( state_in );
    }

    std::vector<ai::Search::Action *> Problem::Actions( const ai::Search::State * const /* state_in */ ) {
      std::vector<ai::Search::Action *> actions;
      actions.push_back( new Action( Action::FACE_WHITE, Action::ROTATE_CW ) );
      actions.push_back( new Action( Action::FACE_WHITE, Action::ROTATE_CCW ) );
      actions.push_back( new Action( Action::FACE_GREEN, Action::ROTATE_CW ) );
      actions.push_back( new Action( Action::FACE_GREEN, Action::ROTATE_CCW ) );
      actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_CW ) );
      actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_CCW ) );
      actions.push_back( new Action( Action::FACE_BLUE, Action::ROTATE_CW ) );
      actions.push_back( new Action( Action::FACE_BLUE, Action::ROTATE_CCW ) );
      actions.push_back( new Action( Action::FACE_ORANGE, Action::ROTATE_CW ) );
      actions.push_back( new Action( Action::FACE_ORANGE, Action::ROTATE_CCW ) );
      actions.push_back( new Action( Action::FACE_YELLOW, Action::ROTATE_CW ) );
      actions.push_back( new Action( Action::FACE_YELLOW, Action::ROTATE_CCW ) );
      
#if ALLOW_180
      if( mAllow180 ) {
        actions.push_back( new Action( Action::FACE_WHITE, Action::ROTATE_180 ) );
        actions.push_back( new Action( Action::FACE_GREEN, Action::ROTATE_180 ) );
        actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_180 ) );
        actions.push_back( new Action( Action::FACE_BLUE, Action::ROTATE_180 ) );
        actions.push_back( new Action( Action::FACE_ORANGE, Action::ROTATE_180 ) );
        actions.push_back( new Action( Action::FACE_YELLOW, Action::ROTATE_180 ) );
      }
#endif
#if ALLOW_MIDDLE_ROTATE
      if( mAllowMiddleRotate ) {
        actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_MIDDLE_UP ) );
        actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_MIDDLE_DOWN ) );
        actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_MIDDLE_LEFT ) );
        actions.push_back( new Action( Action::FACE_RED, Action::ROTATE_MIDDLE_RIGHT ) );
        actions.push_back( new Action( Action::FACE_BLUE, Action::ROTATE_MIDDLE_UP ) );
        actions.push_back( new Action( Action::FACE_BLUE, Action::ROTATE_MIDDLE_DOWN ) );
      }
#endif
      
      std::shuffle( actions.begin( ), actions.end( ), std::default_random_engine(std::chrono::steady_clock::now().time_since_epoch().count()) );
      return actions;
    }
    
    ai::Search::State *Problem::Result( const ai::Search::State * const state_in, const ai::Search::Action * const action_in ) {
      const State * const state = dynamic_cast< const State * const >( state_in );
      const Action * const action = dynamic_cast< const Action * const >( action_in );

      RubiksCube cube = state->getCube( );
      Color color = NO_COLOR;
      Direction direction = NO_DIRECTION;

      switch ( action->getFace( ) ) {
      case Action::FACE_WHITE: color = WHITE; break;
      case Action::FACE_GREEN: color = GREEN; break;
      case Action::FACE_RED: color = RED; break;
      case Action::FACE_BLUE: color = BLUE; break;
      case Action::FACE_ORANGE: color = ORANGE; break;
      case Action::FACE_YELLOW: color = YELLOW; break;
      default: throw Exception( "unknown face" );  break;
      }

      switch ( action->getDirection( ) ) {
      case Action::ROTATE_CW: direction = CLOCK_WISE; break;
      case Action::ROTATE_CCW: direction = COUNTER_CLOCK_WISE; break;
#if ALLOW_180
      case Action::ROTATE_180: direction = ROTATE_180; break;
#endif
#if ALLOW_MIDDLE_ROTATE
      case Action::ROTATE_MIDDLE_UP: direction = ROTATE_MIDDLE_UP; break;
      case Action::ROTATE_MIDDLE_DOWN: direction = ROTATE_MIDDLE_DOWN; break;
      case Action::ROTATE_MIDDLE_LEFT: direction = ROTATE_MIDDLE_LEFT; break;
      case Action::ROTATE_MIDDLE_RIGHT: direction = ROTATE_MIDDLE_RIGHT; break;
#endif
      default: throw Exception( "unknown direction" );  break;
      }

      cube.rotate( color, direction );
      
      return new State( cube );
    }

    double Problem::StepCost( const ai::Search::State  * const /*state1_in*/,
                              const ai::Search::Action * const /*action_in*/,
                              const ai::Search::State  * const /*state2_in*/ ) const {
      return 1.0;
    }

    double Problem::Heuristic(const ai::Search::State  * const state_in) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      double h = 0.0;
      switch( mHeuristic ) {
      case ZERO_HEURISTIC: {
        h = 0.0;
      }
        break;
      case EDGE_HEURISTIC: {
        double e = std::ceil( state->getCube( ).edgeHeuristic( mGoalState.getCube( ) ) );
        h = e;
      }
        break;
      case CORNER_HEURISTIC: {
        double c = std::ceil( state->getCube( ).cornerHeuristic( mGoalState.getCube( ) ) );
        h = c;
      }
        break;
      case TILE_HEURISTIC: {
        double t = std::ceil( state->getCube( ).misplacedTiles( mGoalState.getCube( ) ) / 20.0 );
        h = t;
      }
        break;
      case END_GAME_HEURISTIC: {
        double k = state->getCube( ).knownCubeHeuristic( );
        h = k;
      }
        break;
      case EDGE_CORNER_HEURISTIC: {
        double e = std::ceil( state->getCube( ).edgeHeuristic( mGoalState.getCube( ) ) );
        double c = std::ceil( state->getCube( ).cornerHeuristic( mGoalState.getCube( ) ) );
        h = std::max( { e, c } );
      }
        break;
      case ALL_HEURISTIC:
      default: {
        double e = std::ceil( state->getCube( ).edgeHeuristic( mGoalState.getCube( ) ) );
        double c = std::ceil( state->getCube( ).cornerHeuristic( mGoalState.getCube( ) ) );
        double t = std::ceil( state->getCube( ).misplacedTiles( mGoalState.getCube( ) ) / 20.0 );
        double k = state->getCube( ).knownCubeHeuristic( );
        h = std::max( { e, c, t, k } );
      }
        break;
      }
      
      return h;
    }

    const RubiksCube& Problem::getGoalCube( ) const {
      return mGoalState.getCube( );
    }

#if ALLOW_180
    bool Problem::getAllow180( ) const {
      return mAllow180;
    }
    void Problem::allow180( ) {
      mAllow180 = true;
    }
    void Problem::disallow180( ) {
      mAllow180 = false;
    }
#endif
    
#if ALLOW_MIDDLE_ROTATE
    bool Problem::getAllowMiddleRotate( ) const {
      return mAllowMiddleRotate;
    }
    void Problem::allowMiddleRotate( ) {
      mAllowMiddleRotate = true;
    }
    void Problem::disallowMiddleRotate( ) {
      mAllowMiddleRotate = false;
    }
#endif
    
    HeuristicEnum Problem::getHeuristic( ) const {
      return mHeuristic;
    }
    
    void Problem::setHeuristic( const HeuristicEnum& heuristic ) {
      mHeuristic = heuristic;
    }
      
    
  }
}
