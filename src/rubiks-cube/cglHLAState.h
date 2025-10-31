#ifndef _CGLHLASTATE_H_
#define _CGLHLASTATE_H_

#include "ai_search.h"
#include "RubiksCube.h"
#include "cglCubeState.h"

namespace cgl {
  namespace cube {

    class HLAState : public State {
    public:
      HLAState( const RubiksCube& cube, const RubiksCube& current_goal );
      HLAState( const HLAState& rhs );
      virtual ~HLAState( );
      
      const RubiksCube& getCurrentGoal( ) const;
      RubiksCube& getCurrentGoal( );

    protected:
      RubiksCube mCurrentGoal;

    private:
      
    };

  }
}

std::ostream& operator<<( std::ostream& os, const cgl::cube::HLAState& state );

#endif /* _CGLHLASTATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
