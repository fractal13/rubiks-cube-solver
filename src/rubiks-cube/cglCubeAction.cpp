#include "cglCubeAction.h"
#include <iostream>

namespace cgl {
  namespace cube {

    Action::Action( const Face& face, const Direction& direction )
      : mFace( face ), mDirection( direction ) {
    }

    Action::~Action( ) {
    }

    void Action::Display() const {
      std::cout << mFace << "." << mDirection << std::endl;
    }
    
    Action::Face Action::getFace( ) const {
      return mFace;
    }

    Action::Direction Action::getDirection( ) const {
      return mDirection;
    }
    

  }
}
