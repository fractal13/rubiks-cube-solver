#ifndef _CGLCUBESTATE_H_
#define _CGLCUBESTATE_H_

#include "ai_search.h"
#include "RubiksCube.h"

namespace cgl {
  namespace cube {

    class State : public ai::Search::State {
    public:
      State( const RubiksCube& cube );
      State( const State& rhs );
      virtual ~State( );
      virtual void Display( ) const;
      virtual bool IsEqual(const ai::Search::State * const state_in) const;
      virtual bool IsLessThan(const ai::Search::State * const state_in) const;

      bool operator==( const State& rhs ) const;
      State& operator=( const State& rhs);

      const RubiksCube& getCube( ) const;
      RubiksCube& getCube( );
      
    protected:
      RubiksCube mCube;
      
    private:
      
    };

  }
}

std::ostream& operator<<( std::ostream& os, const cgl::cube::State& state );

#endif /* _CGLCUBESTATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
