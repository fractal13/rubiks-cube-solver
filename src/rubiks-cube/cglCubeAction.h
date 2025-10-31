#ifndef _CGLCUBEACTION_H_
#define _CGLCUBEACTION_H_

#include "ai_search.h"
#include "RubiksCube.h"

namespace cgl {
  namespace cube {

    class Action : public ai::Search::Action {
    public:
      enum Face { FACE_WHITE, FACE_GREEN, FACE_RED, FACE_BLUE, FACE_ORANGE, FACE_YELLOW };
      enum Direction { ROTATE_CW, ROTATE_CCW
#if ALLOW_180
                       , ROTATE_180
#endif
#if ALLOW_MIDDLE_ROTATE
                       , ROTATE_MIDDLE_UP
                       , ROTATE_MIDDLE_DOWN
                       , ROTATE_MIDDLE_LEFT
                       , ROTATE_MIDDLE_RIGHT
#endif
      };
    
      Action( const Face& face, const Direction& direction );
      virtual ~Action( );
      virtual void Display() const;
      
      Face getFace( ) const;
      Direction getDirection( ) const;
      
    protected:
      Face mFace;
      Direction mDirection;
      
    private:
      
    };

  }
}

#endif /* _CGLCUBEACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
