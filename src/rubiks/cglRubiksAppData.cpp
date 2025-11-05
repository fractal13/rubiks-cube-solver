#include "cglRubiksAppData.h"
#include "cglRubiksCommand.h"

namespace cgl {
  namespace rubiks {

    AppData::AppData( std::istream &is_in, std::ostream &os_in ) 
      : goal_cubes( 1 ), 
        is( is_in ),
        os( os_in ),
        done( false ),
        solution_valid( false ),
        solution( "" ) {
      goal_cubes[ 0 ].assignSolved( );
      setLegalMoves( );
    }

    int AppData::setLegalMoves( ) {
      move_set.clearMoves( );
      move_set.addBasicMoves( );
      if( config.halfTurns( ) ) {
        move_set.addHalfTurnMoves( );
      }
      if( config.sliceTurns( ) ) {
        move_set.addSliceMoves( );
      }
      if( config.halfTurns( ) && config.sliceTurns( ) ) {
        move_set.addHalfTurnSliceMoves( );
      }
      return EXIT_OK;
    }

  }
}
