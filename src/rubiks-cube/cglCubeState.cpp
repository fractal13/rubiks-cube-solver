#include "cglCubeState.h"
#include <iostream>

namespace cgl {
  namespace cube {

    State::State( const RubiksCube& cube )
      : mCube( cube ) {
    }
    
    State::State( const State& rhs ) {
      *this = rhs;
    }
    
    State::~State( ) {
    }
    
    void State::Display( ) const {
      std::cout << mCube.oneLineDisplay( ) << std::endl;
      //std::cout << mCube << std::endl;
    }
    
    bool State::IsEqual( const ai::Search::State * const state_in ) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      return mCube == state->mCube;
    }
    
    bool State::IsLessThan( const ai::Search::State * const state_in ) const {
      const State * const state = dynamic_cast< const State * const >( state_in );
      return mCube < state->mCube;
    }

    bool State::operator==( const State& rhs ) const {
      return this->IsEqual( &rhs );
    }
    
    State& State::operator=( const State& rhs) {
      mCube = rhs.mCube;
      return *this;
    }
    
    const RubiksCube& State::getCube( ) const {
      return mCube;
    }
    
    RubiksCube& State::getCube( ) {
      return mCube;
    }
    
  }
}

std::ostream& operator<<( std::ostream& os, const cgl::cube::State& state ) {
  os << state.getCube( );
  return os;
}
