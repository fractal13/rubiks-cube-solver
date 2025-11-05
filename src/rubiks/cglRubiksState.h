#ifndef _CGLRUBIKSSTATE_H_
#define _CGLRUBIKSSTATE_H_

#include "ai_search.h"
#include "cglRubiksCube.h"

namespace cgl {
  namespace rubiks {

    class State : public ai::Search::State {
    public:
      State( const Cube& cube );
      State( const State& rhs );
      virtual ~State( );
      virtual void Display( ) const;
      virtual bool IsEqual(const ai::Search::State * const state_in) const;
      virtual bool IsLessThan(const ai::Search::State * const state_in) const;

      bool operator==( const State& rhs ) const;
      State& operator=( const State& rhs);

      const Cube& getCube( ) const;
      Cube& getCube( );
      
    protected:
      Cube mCube;
      
    private:
      
    };
    
    
  }
}

std::ostream& operator<<( std::ostream& os, const cgl::rubiks::State& state );


#endif /* _CGLRUBIKSSTATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
