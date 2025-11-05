#include "cglRubiksCommand.h"
#include "cglRubiksMoveSet.h"
#include <map>
#include <string>
#include <vector>
#include <cstdlib>


namespace cgl {
  namespace rubiks {

    static const std::map< std::string, Move > g_predefined_moves = {
      { "I", Move( Move::M_IDENTITY ) },
      { "U", Move( Move::M_U ) }, { "U'", Move( Move::M_UP ) },
      { "L", Move( Move::M_L ) }, { "L'", Move( Move::M_LP ) },
      { "F", Move( Move::M_F ) }, { "F'", Move( Move::M_FP ) },
      { "R", Move( Move::M_R ) }, { "R'", Move( Move::M_RP ) },
      { "B", Move( Move::M_B ) }, { "B'", Move( Move::M_BP ) },
      { "D", Move( Move::M_D ) }, { "D'", Move( Move::M_DP ) },
      { "E", Move( Move::M_EQ) }, { "E'", Move( Move::M_EP ) },
      { "M", Move( Move::M_M ) }, { "M'", Move( Move::M_MP ) },
      { "S", Move( Move::M_S ) }, { "S'", Move( Move::M_SP ) },
      { "U2", Move( Move::M_U ) * Move( Move::M_U ) },
      { "L2", Move( Move::M_L ) * Move( Move::M_L ) },
      { "F2", Move( Move::M_F ) * Move( Move::M_F ) },
      { "R2", Move( Move::M_R ) * Move( Move::M_R ) },
      { "B2", Move( Move::M_B ) * Move( Move::M_B ) },
      { "D2", Move( Move::M_D ) * Move( Move::M_D ) },
      { "E2", Move( Move::M_EQ) * Move( Move::M_EQ) },
      { "M2", Move( Move::M_M ) * Move( Move::M_M ) },
      { "S2", Move( Move::M_S ) * Move( Move::M_S ) },
      { "x", Move( Move::M_X ) }, { "x'", Move( Move::M_XP ) },
      { "y", Move( Move::M_Y ) }, { "y'", Move( Move::M_YP ) },
      { "z", Move( Move::M_Z ) }, { "z'", Move( Move::M_ZP ) },
    };

    MoveSet::MoveSet( )
      : mDefinedMoves( g_predefined_moves ) {
      clearMoves( );
      addBasicMoves( );
    }

    void MoveSet::clearMoves( ) {
      mMoves.clear( );
      mKeys.clear( );
    }

    void MoveSet::addBasicMoves( ) {
      static const std::vector< std::string > names ( { "U", "L", "F", "R", "B", "D", "U'", "L'", "F'", "R'", "B'", "D'" } );
      for( auto it = names.begin( ); it != names.end( ); it++ ) {
        addMove( *it, mDefinedMoves.at( *it ) );
      }
    }

    void MoveSet::addHalfTurnMoves( ) {
      static const std::vector< std::string > names ( { "U2", "L2", "F2", "R2", "B2", "D2" } );
      for( auto it = names.begin( ); it != names.end( ); it++ ) {
        addMove( *it, mDefinedMoves.at( *it ) );
      }
    }

    void MoveSet::addSliceMoves( ) {
      static const std::vector< std::string > names ( { "E", "M", "S" } );
      for( auto it = names.begin( ); it != names.end( ); it++ ) {
        addMove( *it, mDefinedMoves.at( *it ) );
      }
    }

    void MoveSet::addHalfTurnSliceMoves( ) {
      static const std::vector< std::string > names ( { "E2", "M2", "S2" } );
      for( auto it = names.begin( ); it != names.end( ); it++ ) {
        addMove( *it, mDefinedMoves.at( *it ) );
      }
    }

    void MoveSet::addMove( const std::string& name, const Move& move ) {
      mMoves[ name ] = move;
      mKeys.push_back( name );
    }

    int MoveSet::addMovesByName( const std::vector< std::string >& move_names ) {
      unsigned int i;
      for( i = 0; i < move_names.size( ); i++ ) {
        if( ! moveIsDefined( move_names[ i ] ) ) {
          return EXIT_USAGE;
        }
        addMove( move_names[ i ], mDefinedMoves.at( move_names[ i ] ) );
      }
      return EXIT_OK;
    }

    const std::map< std::string, Move >& MoveSet::getMoves( ) const {
      return mMoves;
    }
    
    bool MoveSet::moveExists( const std::string& move_name ) const {
      return mMoves.count( move_name ) > 0;
    }

    const Move& MoveSet::getMove( const std::string& move_name ) const {
      return mMoves.at( move_name );
    }

    const Move& MoveSet::getRandomMove( ) const {
      int index = std::rand( ) % mKeys.size( );
      return getMove( mKeys[ index ] );
    }

    bool MoveSet::moveIsDefined( const std::string& name ) const {
      return mDefinedMoves.count( name ) > 0;
    }

    int MoveSet::defineMove( const std::string& name, const Move& move ) {
      if( moveIsDefined( name ) ) {
        return EXIT_USAGE;
      }
      mDefinedMoves[ name ] = move;
      return EXIT_OK;
    }

    int  MoveSet::defineMove( const std::string& name, const std::vector< std::string >& move_names ) {
      if( moveIsDefined( name ) ) {
        return EXIT_USAGE;
      }
      Move move( Move::M_IDENTITY );
      for( auto it = move_names.begin( ); it != move_names.end( ); it++ ) {
        if( !moveIsDefined( *it ) ) {
          return EXIT_USAGE;
        }
        move = mDefinedMoves.at( *it ) * move;
      }
      mDefinedMoves[ name ] = move;
      return EXIT_OK;
    }

    const std::map< std::string, Move >& MoveSet::getDefinedMoves( ) const {
      return mDefinedMoves;
    }

    const Move& MoveSet::getDefinedMove( const std::string& move_name ) const {
      return mDefinedMoves.at( move_name );
    }

  }
}

