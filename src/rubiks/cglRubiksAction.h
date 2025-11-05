#ifndef _CGLRUBIKSACTION_H_
#define _CGLRUBIKSACTION_H_

#include "ai_search.h"
#include "cglRubiksCube.h"

namespace cgl {
  namespace rubiks {

    class Action : public ai::Search::Action {
    public:

      Action( const Move& move );
      virtual ~Action( );
      virtual void Display( ) const;
      
      const Move& getMove( ) const;
      
    protected:
      Move mMove;
      
    private:
      
    };

  }
}

std::ostream& operator<<( std::ostream& os, const cgl::rubiks::Action& action );



#endif /* _CGLRUBIKSACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
