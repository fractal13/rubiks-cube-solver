#include "cglRubiksCube.h"

#include <sstream>
#include <vector>

namespace cgl {
  namespace rubiks {

    /**************************************************************
     *
     * Move class
     *
     */
    // Default constructor, no initialization
    Move::Move( ) 
      : mTransform( NUM_POSITIONS_TOTAL ), mCost( 0.0 ), mName( "Uninitialized" ) {
      // empty
    }

    // Construct one of the basic moves
    Move::Move( const BasicMove& basic_move )
      : mTransform( NUM_POSITIONS_TOTAL ), mCost( 0.0 ), mName( "Uninitialized" ) {
      switch( basic_move ) {
      case M_IDENTITY: buildIdentity( ); break;
      case M_U:  buildU( ); break;
      case M_UP: buildUPrime( ); break;
      case M_L:  buildL( ); break;
      case M_LP: buildLPrime( ); break;
      case M_F:  buildF( ); break;
      case M_FP: buildFPrime( ); break;
      case M_R:  buildR( ); break;
      case M_RP: buildRPrime( ); break;
      case M_B:  buildB( ); break;
      case M_BP: buildBPrime( ); break;
      case M_D:  buildD( ); break;
      case M_DP: buildDPrime( ); break;
      case M_EQ: buildE( ); break;
      case M_EP: buildEPrime( ); break;
      case M_M:  buildM( ); break;
      case M_MP: buildMPrime( ); break;
      case M_S:  buildS( ); break;
      case M_SP: buildSPrime( ); break;
      case M_X:  buildX( ); break;
      case M_XP: buildXPrime( ); break;
      case M_Y:  buildY( ); break;
      case M_YP: buildYPrime( ); break;
      case M_Z:  buildZ( ); break;
      case M_ZP: buildZPrime( ); break;
      default: 
        std::stringstream ss;
        ss << "Unknown BasicMove: ( " << (int)basic_move << " )";
        throw Exception( ss.str( ) );
        break;
      }
    }

    const std::vector< int >& Move::getTransform( ) const {
      return mTransform;
    }

    double Move::getCost( ) const {
      return mCost;
    }

    const std::string& Move::getName( ) const {
      return mName;
    }

    Move& Move::compose( const Move& move2, const Move& move1 ) {
      /* move1 == *this is ok, move2 == *this is not ok */
      if ( &move2 == this ) {
        std::stringstream ss;
        ss << "move2 can not be *this, will cause errors.";
        throw Exception( ss.str( ) );
      }
      unsigned int i1, i2, i3;
      for( i1 = 0; i1 < mTransform.size( ); i1++ ) {
        i2 = move1.mTransform[ i1 ];
        i3 = move2.mTransform[ i2 ];
        this->mTransform[ i1 ] = i3;
      }
      this->mCost = move2.mCost + move1.mCost;
      this->mName = move1.mName + " " + move2.mName;
      return *this;
    }

    Move Move::operator*( const Move& rhs ) const {
      Move m;
      return m.compose( *this, rhs );
    }
    
    Cube Move::operator*( const Cube& rhs ) const {
      Cube c;
      return c.applyMove( *this, rhs );
    }

    void Move::buildIdentity( ) {
      mName = "";
      unsigned int i;
      for( i = 0; i < mTransform.size( ); i++ ) {
        mTransform[ i ] = i;
      }
    }

    void Move::buildU( ) {
      mCost = 1.0;
      buildIdentity( );
      mName = "U";
      mTransform[ 0 ] = 2;
      mTransform[ 1 ] = 5;
      mTransform[ 2 ] = 8;
      mTransform[ 3 ] = 1;
      mTransform[ 5 ] = 7;
      mTransform[ 6 ] = 0;
      mTransform[ 7 ] = 3;
      mTransform[ 8 ] = 6;

      mTransform[ 9 ] = 36;
      mTransform[ 10 ] = 37;
      mTransform[ 11 ] = 38;
      mTransform[ 18 ] = 9;
      mTransform[ 19 ] = 10;
      mTransform[ 20 ] = 11;
      mTransform[ 27 ] = 18;
      mTransform[ 28 ] = 19;
      mTransform[ 29 ] = 20;
      mTransform[ 36 ] = 27;
      mTransform[ 37 ] = 28;
      mTransform[ 38 ] = 29;
    }

    void Move::buildUPrime( ) {
      Move u( M_U );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 1.0;
      mName = "U'";
    }

    void Move::buildL( ) {
      mCost = 1.0;
      buildIdentity( );
      mName = "L";
      mTransform[ 9 ] = 11;
      mTransform[ 10 ] = 14;
      mTransform[ 11 ] = 17;
      mTransform[ 12 ] = 10;
      mTransform[ 14 ] = 16;
      mTransform[ 15 ] = 9;
      mTransform[ 16 ] = 12;
      mTransform[ 17 ] = 15;

      mTransform[ 0 ] = 18;
      mTransform[ 3 ] = 21;
      mTransform[ 6 ] = 24;
      mTransform[ 18 ] = 45;
      mTransform[ 21 ] = 48;
      mTransform[ 24 ] = 51;
      mTransform[ 45 ] = 44;
      mTransform[ 48 ] = 41;
      mTransform[ 51 ] = 38;
      mTransform[ 38 ] = 6;
      mTransform[ 41 ] = 3;
      mTransform[ 44 ] = 0;
    }

    void Move::buildLPrime( ) {
      Move u( M_L );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 1.0;
      mName = "L'";
    }

    void Move::buildF( ) {
      mCost = 1.0;
      buildIdentity( );
      mName = "F";
      mTransform[ 18 ] = 20;
      mTransform[ 19 ] = 23;
      mTransform[ 20 ] = 26;
      mTransform[ 21 ] = 19;
      mTransform[ 23 ] = 25;
      mTransform[ 24 ] = 18;
      mTransform[ 25 ] = 21;
      mTransform[ 26 ] = 24;

      mTransform[ 6 ] = 27;
      mTransform[ 7 ] = 30;
      mTransform[ 8 ] = 33;
      mTransform[ 27 ] = 47;
      mTransform[ 30 ] = 46;
      mTransform[ 33 ] = 45;
      mTransform[ 47 ] = 17;
      mTransform[ 46 ] = 14;
      mTransform[ 45 ] = 11;
      mTransform[ 17 ] = 6;
      mTransform[ 14 ] = 7;
      mTransform[ 11 ] = 8;
    }

    void Move::buildFPrime( ) {
      Move u( M_F );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 1.0;
      mName = "F'";
    }

    void Move::buildR( ) {
      buildIdentity( );
      mCost = 1.0;
      mName = "R";
      mTransform[ 27 ] = 29;
      mTransform[ 28 ] = 32;
      mTransform[ 29 ] = 35;
      mTransform[ 30 ] = 28;
      mTransform[ 32 ] = 34;
      mTransform[ 33 ] = 27;
      mTransform[ 34 ] = 30;
      mTransform[ 35 ] = 33;

      mTransform[ 8 ] = 36;
      mTransform[ 5 ] = 39;
      mTransform[ 2 ] = 42;
      mTransform[ 36 ] = 53;
      mTransform[ 39 ] = 50;
      mTransform[ 42 ] = 47;
      mTransform[ 53 ] = 26;
      mTransform[ 50 ] = 23;
      mTransform[ 47 ] = 20;
      mTransform[ 26 ] = 8;
      mTransform[ 23 ] = 5;
      mTransform[ 20 ] = 2;
    }

    void Move::buildRPrime( ) {
      Move u( M_R );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 1.0;
      mName = "R'";
    }

    void Move::buildB( ) {
      buildIdentity( );
      mCost = 1.0;
      mName = "B";
      mTransform[ 36 ] = 38;
      mTransform[ 37 ] = 41;
      mTransform[ 38 ] = 44;
      mTransform[ 39 ] = 37;
      mTransform[ 41 ] = 43;
      mTransform[ 42 ] = 36;
      mTransform[ 43 ] = 39;
      mTransform[ 44 ] = 42;

      mTransform[ 2 ] = 9;
      mTransform[ 1 ] = 12;
      mTransform[ 0 ] = 15;
      mTransform[ 9 ] = 51;
      mTransform[ 12 ] = 52;
      mTransform[ 15 ] = 53;
      mTransform[ 51 ] = 35;
      mTransform[ 52 ] = 32;
      mTransform[ 53 ] = 29;
      mTransform[ 35 ] = 2;
      mTransform[ 32 ] = 1;
      mTransform[ 29 ] = 0;
    }

    void Move::buildBPrime( ) {
      Move u( M_B );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 1.0;
      mName = "B'";
    }

    void Move::buildD( ) {
      mCost = 1.0;
      buildIdentity( );
      mName = "D";
      mTransform[ 45 ] = 47;
      mTransform[ 46 ] = 50;
      mTransform[ 47 ] = 53;
      mTransform[ 48 ] = 46;
      mTransform[ 50 ] = 52;
      mTransform[ 51 ] = 45;
      mTransform[ 52 ] = 48;
      mTransform[ 53 ] = 51;

      mTransform[ 24 ] = 33;
      mTransform[ 25 ] = 34;
      mTransform[ 26 ] = 35;
      mTransform[ 33 ] = 42;
      mTransform[ 34 ] = 43;
      mTransform[ 35 ] = 44;
      mTransform[ 42 ] = 15;
      mTransform[ 43 ] = 16;
      mTransform[ 44 ] = 17;
      mTransform[ 15 ] = 24;
      mTransform[ 16 ] = 25;
      mTransform[ 17 ] = 26;
    }

    void Move::buildDPrime( ) {
      Move u( M_D );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 1.0;
      mName = "D'";
    }

    void Move::buildE( ) {
      mCost = 2.0;
      buildIdentity( );
      mName = "E";
      mTransform[ 12 ] = 21;
      mTransform[ 13 ] = 22;
      mTransform[ 14 ] = 23;
      mTransform[ 21 ] = 30;
      mTransform[ 22 ] = 31;
      mTransform[ 23 ] = 32;
      mTransform[ 30 ] = 39;
      mTransform[ 31 ] = 40;
      mTransform[ 32 ] = 41;
      mTransform[ 39 ] = 12;
      mTransform[ 40 ] = 13;
      mTransform[ 41 ] = 14;
    }

    void Move::buildEPrime( ) {
      Move u( M_EQ );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 2.0;
      mName = "E'";
    }

    void Move::buildM( ) {
      mCost = 2.0;
      buildIdentity( );
      mName = "M";
      mTransform[ 1 ] = 19;
      mTransform[ 4 ] = 22;
      mTransform[ 7 ] = 25;
      mTransform[ 19 ] = 46;
      mTransform[ 22 ] = 49;
      mTransform[ 25 ] = 52;
      mTransform[ 46 ] = 43;
      mTransform[ 49 ] = 40;
      mTransform[ 52 ] = 37;
      mTransform[ 43 ] = 1;
      mTransform[ 40 ] = 4;
      mTransform[ 37 ] = 7;
    }

    void Move::buildMPrime( ) {
      Move u( M_M );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 2.0;
      mName = "M'";
    }

    void Move::buildS( ) {
      mCost = 2.0;
      buildIdentity( );
      mName = "S";
      mTransform[ 3 ] = 28;
      mTransform[ 4 ] = 31;
      mTransform[ 5 ] = 34;
      mTransform[ 28 ] = 50;
      mTransform[ 31 ] = 49;
      mTransform[ 34 ] = 48;
      mTransform[ 50 ] = 16;
      mTransform[ 49 ] = 13;
      mTransform[ 48 ] = 10;
      mTransform[ 16 ] = 3;
      mTransform[ 13 ] = 4;
      mTransform[ 10 ] = 5;
    }

    void Move::buildSPrime( ) {
      Move u( M_S );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 2.0;
      mName = "S'";
    }

    void Move::buildX( ) {
      mName = "x";
      mCost = 0.001;
      // Up
      mTransform[ 0 ] = 44;
      mTransform[ 1 ] = 43;
      mTransform[ 2 ] = 42;
      mTransform[ 3 ] = 41;
      mTransform[ 4 ] = 40;
      mTransform[ 5 ] = 39;
      mTransform[ 6 ] = 38;
      mTransform[ 7 ] = 37;
      mTransform[ 8 ] = 36;

      // Left
      mTransform[ 9 ] = 15;
      mTransform[ 10 ] = 12;
      mTransform[ 11 ] = 9;
      mTransform[ 12 ] = 16;
      mTransform[ 13 ] = 13;
      mTransform[ 14 ] = 10;
      mTransform[ 15 ] = 17;
      mTransform[ 16 ] = 14;
      mTransform[ 17 ] = 11;

      // Front
      mTransform[ 18 ] = 0;
      mTransform[ 19 ] = 1;
      mTransform[ 20 ] = 2;
      mTransform[ 21 ] = 3;
      mTransform[ 22 ] = 4;
      mTransform[ 23 ] = 5;
      mTransform[ 24 ] = 6;
      mTransform[ 25 ] = 7;
      mTransform[ 26 ] = 8;

      // Right
      mTransform[ 27 ] = 29;
      mTransform[ 28 ] = 32;
      mTransform[ 29 ] = 35;
      mTransform[ 30 ] = 28;
      mTransform[ 31 ] = 31;
      mTransform[ 32 ] = 34;
      mTransform[ 33 ] = 27;
      mTransform[ 34 ] = 30;
      mTransform[ 35 ] = 33;

      // Back
      mTransform[ 36 ] = 53;
      mTransform[ 37 ] = 52;
      mTransform[ 38 ] = 51;
      mTransform[ 39 ] = 50;
      mTransform[ 40 ] = 49;
      mTransform[ 41 ] = 48;
      mTransform[ 42 ] = 47;
      mTransform[ 43 ] = 46;
      mTransform[ 44 ] = 45;

      // Down
      mTransform[ 45 ] = 18;
      mTransform[ 46 ] = 19;
      mTransform[ 47 ] = 20;
      mTransform[ 48 ] = 21;
      mTransform[ 49 ] = 22;
      mTransform[ 50 ] = 23;
      mTransform[ 51 ] = 24;
      mTransform[ 52 ] = 25;
      mTransform[ 53 ] = 26;
    }

    void Move::buildXPrime( ) {
      Move u( M_X );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 0.001;
      mName = "x'";
    }

    void Move::buildY( ) {
      mName = "y";
      mCost = 0.001;
      // Up
      mTransform[ 0 ] = 2;
      mTransform[ 1 ] = 5;
      mTransform[ 2 ] = 8;
      mTransform[ 3 ] = 1;
      mTransform[ 4 ] = 4;
      mTransform[ 5 ] = 7;
      mTransform[ 6 ] = 0;
      mTransform[ 7 ] = 3;
      mTransform[ 8 ] = 6;

      // Left
      mTransform[ 9 ] = 36;
      mTransform[ 10 ] = 37;
      mTransform[ 11 ] = 38;
      mTransform[ 12 ] = 39;
      mTransform[ 13 ] = 40;
      mTransform[ 14 ] = 41;
      mTransform[ 15 ] = 42;
      mTransform[ 16 ] = 43;
      mTransform[ 17 ] = 44;

      // Front
      mTransform[ 18 ] = 9;
      mTransform[ 19 ] = 10;
      mTransform[ 20 ] = 11;
      mTransform[ 21 ] = 12;
      mTransform[ 22 ] = 13;
      mTransform[ 23 ] = 14;
      mTransform[ 24 ] = 15;
      mTransform[ 25 ] = 16;
      mTransform[ 26 ] = 17;

      // Right
      mTransform[ 27 ] = 18;
      mTransform[ 28 ] = 19;
      mTransform[ 29 ] = 20;
      mTransform[ 30 ] = 21;
      mTransform[ 31 ] = 22;
      mTransform[ 32 ] = 23;
      mTransform[ 33 ] = 24;
      mTransform[ 34 ] = 25;
      mTransform[ 35 ] = 26;

      // Back
      mTransform[ 36 ] = 27;
      mTransform[ 37 ] = 28;
      mTransform[ 38 ] = 29;
      mTransform[ 39 ] = 30;
      mTransform[ 40 ] = 31;
      mTransform[ 41 ] = 32;
      mTransform[ 42 ] = 33;
      mTransform[ 43 ] = 34;
      mTransform[ 44 ] = 35;

      // Down
      mTransform[ 45 ] = 51;
      mTransform[ 46 ] = 48;
      mTransform[ 47 ] = 45;
      mTransform[ 48 ] = 52;
      mTransform[ 49 ] = 49;
      mTransform[ 50 ] = 46;
      mTransform[ 51 ] = 53;
      mTransform[ 52 ] = 50;
      mTransform[ 53 ] = 47;
    }

    void Move::buildYPrime( ) {
      Move u( M_Y );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 0.001;
      mName = "y'";
    }

    void Move::buildZ( ) {
      mName = "z";
      mCost = 0.001;
      // Up
      mTransform[ 0 ] = 29;
      mTransform[ 1 ] = 32;
      mTransform[ 2 ] = 35;
      mTransform[ 3 ] = 28;
      mTransform[ 4 ] = 31;
      mTransform[ 5 ] = 34;
      mTransform[ 6 ] = 27;
      mTransform[ 7 ] = 30;
      mTransform[ 8 ] = 33;

      // Left
      mTransform[ 9 ] = 2;
      mTransform[ 10 ] = 5;
      mTransform[ 11 ] = 8;
      mTransform[ 12 ] = 1;
      mTransform[ 13 ] = 4;
      mTransform[ 14 ] = 7;
      mTransform[ 15 ] = 0;
      mTransform[ 16 ] = 3;
      mTransform[ 17 ] = 6;

      // Front
      mTransform[ 18 ] = 20;
      mTransform[ 19 ] = 23;
      mTransform[ 20 ] = 26;
      mTransform[ 21 ] = 19;
      mTransform[ 22 ] = 22;
      mTransform[ 23 ] = 25;
      mTransform[ 24 ] = 18;
      mTransform[ 25 ] = 21;
      mTransform[ 26 ] = 24;

      // Right
      mTransform[ 27 ] = 47;
      mTransform[ 28 ] = 50;
      mTransform[ 29 ] = 53;
      mTransform[ 30 ] = 46;
      mTransform[ 31 ] = 49;
      mTransform[ 32 ] = 52;
      mTransform[ 33 ] = 45;
      mTransform[ 34 ] = 48;
      mTransform[ 35 ] = 51;

      // Back
      mTransform[ 36 ] = 42;
      mTransform[ 37 ] = 39;
      mTransform[ 38 ] = 36;
      mTransform[ 39 ] = 43;
      mTransform[ 40 ] = 40;
      mTransform[ 41 ] = 37;
      mTransform[ 42 ] = 44;
      mTransform[ 43 ] = 41;
      mTransform[ 44 ] = 38;

      // Down
      mTransform[ 45 ] = 11;
      mTransform[ 46 ] = 14;
      mTransform[ 47 ] = 17;
      mTransform[ 48 ] = 10;
      mTransform[ 49 ] = 13;
      mTransform[ 50 ] = 16;
      mTransform[ 51 ] = 9;
      mTransform[ 52 ] = 12;
      mTransform[ 53 ] = 15;
    }

    void Move::buildZPrime( ) {
      Move u( M_Z );
      buildIdentity( );
      compose( u, *this );
      compose( u, *this );
      compose( u, *this );
      mCost = 0.001;
      mName = "z'";
    }


#if 0
    static void sample_compose_code( ) {
      Move m_a( Move::M_U );
      Move m_b( Move::M_U );
      Move u2;
      u2.compose( m_a, m_b );
      
      Move r( Move::M_R );
      Move lp( Move::M_LP );
      Move r2, lp2;
      Move m2;
      m2.compose( r2.compose( r, r ), lp2.compose( lp, lp ) );

      Move complex( Move::BasicMove::M_RP );
      Move t1( Move::BasicMove::M_DP );
      Move t2( Move::BasicMove::M_R );
      Move t3( Move::BasicMove::M_D );

      // R' D' R D can be built with 
      // EITHER
      complex.compose( t1, complex );
      complex.compose( t2, complex );
      complex.compose( t3, complex );
      // OR
      complex.compose( t3, complex.compose( t2, complex.compose( t1, complex ) ) );

    }
#endif // 0
    /*
     *
     * Move class
     *
     **************************************************************/

    /**************************************************************
     *
     * Cube class
     *
     */

    // Solved cube constructed
    Cube::Cube( ) 
      : mFacelets( NUM_POSITIONS_TOTAL ) {
      const char * const facelets = "wgrboy";
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        mFacelets[ i ] = facelets[ i / NUM_POSITIONS_PER_SIDE ];
      }
    }

    bool Cube::operator==( const Cube& rhs ) const {
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        if( ( mFacelets[ i ] != rhs.mFacelets[ i ] ) &&
            ( rhs.mFacelets[ i ] != '*' ) &&
            ( mFacelets[ i ] != '*' ) ) {
          return false;
        }
      }
      return true;
    }

    bool Cube::operator!=( const Cube& rhs ) const {
      return !( *this == rhs );
    }

    bool Cube::operator<( const Cube& rhs ) const {
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        if ( mFacelets[ i ] < rhs.mFacelets[ i ] ) {
          // less than
          return true;
        } else if ( mFacelets[ i ] > rhs.mFacelets[ i ] ) {
          // greater than
          return false;
        }
      }
      // equal
      return false;
    }

    void Cube::buildTestPattern( ) {
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        mFacelets[ i ] = 'A' + i;
      }
    }

    void Cube::assignFromString( const std::string& s ) {
      if( s.size( ) != mFacelets.size( ) ) {
        std::stringstream ss;
        ss << "Assignment string of incorrect length: \"" << s << "\"";
        throw Exception( ss.str( ) );
      }
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        mFacelets[ i ] = s[ i ];
      }
    }

    void Cube::assignSolved( ) {
      assignFromString( "wwwwwwwwwgggggggggrrrrrrrrrbbbbbbbbboooooooooyyyyyyyyy" );
    }

    const std::vector< Facelet >& Cube::getFacelets( ) const {
      return mFacelets;
    }
 
    Cube& Cube::applyMove( const Move& move, const Cube& cube ) {
      if ( &cube == this ) {
        std::stringstream ss;
        ss << "cube can not be *this, will cause errors.";
        throw Exception( ss.str( ) );
      }
      const std::vector< int >& transform = move.getTransform( );
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        this->mFacelets[ transform[ i ] ] = cube.mFacelets[ i ];
      }
      return *this;
    }

    Cube& Cube::unapplyMove( const Move& move, const Cube& cube ) {
      if ( &cube == this ) {
        std::stringstream ss;
        ss << "cube can not be *this, will cause errors.";
        throw Exception( ss.str( ) );
      }
      const std::vector< int >& transform = move.getTransform( );
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        this->mFacelets[ i ] = cube.mFacelets[ transform[ i ] ];
      }
      return *this;
    }

    Cube Cube::operator*( const Move& rhs ) const {
      Cube c;
      return c.unapplyMove( rhs, *this );
    }

    Cube& Cube::mergeCube( const Cube& cube ) {
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        if( mFacelets[ i ] == '*' ) {
          mFacelets[ i ] = cube.mFacelets[ i ];
        } else if( cube.mFacelets[ i ] == '*' ) {
          // no change
        } else if( mFacelets[ i ] == cube.mFacelets[ i ] ) {
          // no change
        } else {
          std::stringstream ss;
          ss << "Can not merge cubes with non-matching non-wildcards" << std::endl;
          oneLineDisplay( ss );
          ss << std::endl;
          cube.oneLineDisplay( ss );
          ss << std::endl;
          throw Exception( ss.str( ) );
        }
      }
      return *this;
    }

    Cube Cube::operator+( const Cube& rhs ) const {
      Cube c = *this;
      return c.mergeCube( rhs );
    }


    std::ostream& Cube::tDisplay( std::ostream& os ) const {
      unsigned int i, j;
      
      /* Up face */
      os << "     ";
      for( i = 0; i < 3; i++ ) {
        os << mFacelets[ i ];
      }
      os << std::endl;

      os << "     ";
      for( i = 3; i < 6; i++ ) {
        os << mFacelets[ i ];
      }
      os << std::endl;

      os << "     ";
      for( i = 6; i < 9; i++ ) {
        os << mFacelets[ i ];
      }
      os << std::endl;


      /* Middle faces */
      os << std::endl;
      for( j = 0; j < 9; j+=3 ) {
        for( i = 9+j; i < 12+j; i++ ) {
          os << mFacelets[ i ];
        }
        os << "  ";
        for( i = 18+j; i < 21+j; i++ ) {
          os << mFacelets[ i ];
        }
        os << "  ";
        for( i = 27+j; i < 30+j; i++ ) {
          os << mFacelets[ i ];
        }
        os << "  ";
        for( i = 36+j; i < 39+j; i++ ) {
          os << mFacelets[ i ];
        }
        os << std::endl;
      }
      os << std::endl;

      /* Down face */
      os << "     ";
      for( i = 45; i < 48; i++ ) {
        os << mFacelets[ i ];
      }
      os << std::endl;

      os << "     ";
      for( i = 48; i < 51; i++ ) {
        os << mFacelets[ i ];
      }
      os << std::endl;

      os << "     ";
      for( i = 51; i < 54; i++ ) {
        os << mFacelets[ i ];
      }
      os << std::endl;


      return os;
    }

    std::ostream& Cube::oneWordDisplay( std::ostream& os ) const {
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        os << mFacelets[ i ];
      }
      return os;
    }

    std::ostream& Cube::oneLineDisplay( std::ostream& os ) const {
      unsigned int i;
      for( i = 0; i < mFacelets.size( ); i++ ) {
        if( i > 0 && i % NUM_POSITIONS_PER_SIDE == 0 ) {
          os << " ";
        }
        os << mFacelets[ i ];
      }
      return os;
    }

    void Cube::wildcardToX( ) {
      for( auto it = mFacelets.begin( ); it != mFacelets.end( ); it++ ) {
        if( *it == '*' ) {
          *it = 'x';
        }
      }
    }

    /*
     *
     * Cube class
     *
     **************************************************************/

    /**************************************************************
     *
     * Support classes and functions
     *
     */

    Exception::Exception( const std::string& msg ) 
      : std::exception(), mMsg( msg ) {
    }
    const char* Exception::what() const throw() {
      return mMsg.c_str( );
    }

    /*
     *
     * Support classes
     *
     **************************************************************/


      
  }
}

std::ostream& operator<<( std::ostream& os, const cgl::rubiks::Move& move ) {
  /*
    const std::vector< int >& transform = move.getTransform( );
    unsigned int i;
    for( i = 0; i < transform.size( ); i++ ) {
    os << transform[ i ] << " ";
    }
  */
  os << move.getName( ) << " " << move.getCost( );
  return os;
}

std::ostream& operator<<( std::ostream& os, const cgl::rubiks::Cube& cube ) {
  /*
    const std::vector< Facelet >& facelets = cube.getFacelets( );
    unsigned int i;
    for( i = 0; i < facelets.size( ); i++ ) {
    os << facelets[ i ];
    }
    return os;
  */
  return cube.tDisplay( os );
}

