#ifndef _CGLHLAACTION_H_
#define _CGLHLAACTION_H_

#include "ai_search.h"
#include "RubiksCube.h"
#include <vector>
#include <map>

namespace cgl {
  namespace cube {

    class HLAAction : public ai::Search::Action {
    public:
    
      HLAAction( const std::vector< std::pair< Color, Direction > >& rotations, const RubiksCube& current_goal );
      virtual ~HLAAction( );
      virtual void Display() const;

      const std::vector< std::pair< Color, Direction > >& getRotations( ) const;
      double getCost( ) const;

      const RubiksCube& getCurrentGoal( ) const;

      static const std::vector< RubiksCube >& getCubies( );
      
    protected:
      std::vector< std::pair< Color, Direction > > mRotations;
      RubiksCube mCurrentGoal;

      static bool gInitialized;
      static void initializeCubies( );
      // each entry describes one cubie in correct placement, all others are wildcarded
      static std::vector< RubiksCube > gCubies;
      
    private:
      
    };

  }
}

#endif /* _CGLHLAACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
