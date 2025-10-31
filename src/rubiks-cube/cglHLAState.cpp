#include "cglHLAState.h"
#include <iostream>

namespace cgl {
  namespace cube {

    HLAState::HLAState( const RubiksCube& cube, const RubiksCube& current_goal )
      : State( cube ), mCurrentGoal( current_goal ) {
    }
    
    HLAState::HLAState( const HLAState& rhs )
      : State( rhs ), mCurrentGoal( rhs.mCurrentGoal ) {
    }
    
    HLAState::~HLAState( ) {
    }
    
    const RubiksCube& HLAState::getCurrentGoal( ) const {
      return mCurrentGoal;
    }

    RubiksCube& HLAState::getCurrentGoal( ) {
      return mCurrentGoal;
    }

  }
}

std::ostream& operator<<( std::ostream& os, const cgl::cube::HLAState& state ) {
  os << "state: " << state.getCube( );
  os << "  goal: " << state.getCurrentGoal( );
  return os;
}
