#include "cglRubiksAction.h"

namespace cgl {
  namespace rubiks {

    Action::Action( const Move& move )
      : mMove( move ) {
    }

    Action::~Action( ) {
    }

    void Action::Display( ) const {
      std::cout << mMove.getName( ) << std::endl;
    }
    
    const Move& Action::getMove( ) const {
      return mMove;
    }

  }
}

std::ostream& operator<<( std::ostream& os, const cgl::rubiks::Action& action ) {
  os << action.getMove( ).getName( );
  return os;
}
