#ifndef _CGLRUBIKSCUBE_H_
#define _CGLRUBIKSCUBE_H_

#include <vector>
#include <string>
#include <exception>
#include <iostream>

namespace cgl {
  namespace rubiks {

    const int NUM_SIDES = 6;
    const int NUM_POSITIONS_PER_SIDE = 9;
    const int NUM_POSITIONS_TOTAL = NUM_SIDES * NUM_POSITIONS_PER_SIDE;

    /* By storing the facelets as chars, each cube requires NUM_POSITIONS_TOTAL bytes 
     * This is the best we can do without bit-packing, which would slow
     * down operations on the cube.
     */
    typedef char Facelet; // wgrboy*

    // forward declare the Cube class
    class Cube;

    /*
     * The Move class represents the abstract concept of 0 or more
     * 90 degree rotations of a face or slice.  For the 3x3x3 cube,
     * the base rotations are the 2 rotations of each of the
     * 6 faces, for 12 basic moves.  All other moves are compositions
     * of these 12 moves.  If this code is ever adapted for higher
     * dimension cubes, this concept will need to be amended, to
     * allow for middle slice moves.
     *
     * We add 6 additional basic moves.  These are the rotations of
     * the entire cube about one of the major axes: x, y, z, in both
     * directions.
     *
     * In concept, the move would be represented by a matrix
     * to map old state into new state.  However, since each facelet
     * goes from exactly one place to exactly one place, we
     * will just use a mapping for where each facelet moves.
     *
     */
    class Move {
    public:
      // M_EQ is named strangely due to cmath's M_E, the constant for the numeric constant e.
      // Don't know why we can't scope it away, but we can't.
      enum BasicMove { M_IDENTITY, M_U, M_UP, M_L, M_LP, M_F, M_FP, M_R, M_RP, M_B, M_BP, M_D, M_DP, 
                       M_EQ, M_EP, M_M, M_MP, M_S, M_SP, M_X, M_XP, M_Y, M_YP, M_Z, M_ZP };

      // Default constructor, no initialization
      Move( );
      // Construct one of the basic moves
      Move( const BasicMove& basic_move );

      const std::vector< int >& getTransform( ) const;
      double getCost( ) const;
      const std::string& getName( ) const;

      /* replace this->mTransform contents with the composite of move1 followed by move2
       * It's OK if move1 == *this.
       * Don't get too tricky.  If move2 == *this, errors will occur.
       * Returns *this by reference.
       */
      Move& compose( const Move& move2, const Move& move1 );
      Move operator*( const Move& rhs ) const;

      Cube operator*( const Cube& rhs ) const;

    protected:
      void buildIdentity( );
      void buildU( );
      void buildUPrime( );
      void buildL( );
      void buildLPrime( );
      void buildF( );
      void buildFPrime( );
      void buildR( );
      void buildRPrime( );
      void buildB( );
      void buildBPrime( );
      void buildD( );
      void buildDPrime( );
      void buildE( );
      void buildEPrime( );
      void buildM( );
      void buildMPrime( );
      void buildS( );
      void buildSPrime( );
      void buildX( );
      void buildXPrime( );
      void buildY( );
      void buildYPrime( );
      void buildZ( );
      void buildZPrime( );

      /* dst_index = mTransform[ src_index ] */
      std::vector< int > mTransform;

      double mCost; // quarter-turn metric cost

      std::string mName; // notational name of the move
    private:
    };

    class Cube {
    public:
      // Default, constructs a solved cube
      Cube( );

      bool operator==( const Cube& rhs ) const;
      bool operator!=( const Cube& rhs ) const;
      bool operator<( const Cube& rhs ) const;

      // changes the cube into 54 unique facelet values
      void buildTestPattern( );
      
      // assigns to the facelets the values in the string
      void assignFromString( const std::string& s );

      // assigns to the facelets the solved values
      void assignSolved( );

      // returns pointer of array of facelets
      const std::vector< Facelet >& getFacelets( ) const;

      /* copies cube.mFacelets, being transformed by move.mTransform,
       * into this->mFacelets.
       * *this and cube must be different cubes.
       * returns *this by reference
       */
      Cube& applyMove( const Move& move, const Cube& cube );
      /* reverses the work of applyMove
       */
      Cube& unapplyMove( const Move& move, const Cube& cube );
      /* 
       * Cube * Move safely does unapplyMove( )
       */
      Cube operator*( const Move& rhs ) const;
      /* 
       * mergeCube modifies the current cube,
       * Cube + Cube produces a new cube, where 
       *  '*' + 'x' -> 'x'
       *  'x' + '*' -> 'x'
       *  'x' + 'x' -> 'x'
       *  '*' + '*' -> '*'
       *  'x' + 'y' -> exception
       */
      Cube& mergeCube( const Cube& cube );
      Cube operator+( const Cube& rhs ) const;

      std::ostream& tDisplay( std::ostream& os ) const;
      std::ostream& oneWordDisplay( std::ostream& os ) const;
      std::ostream& oneLineDisplay( std::ostream& os ) const;

      /* replaces any '*' facelets with 'x' facelets */
      void wildcardToX( );

    protected:
      std::vector< Facelet > mFacelets;

    private:
    };

#if 0
    // apply move to cube and produce the new cube.
    Cube operator*( const Move& move, const Cube& cube );


    enum Rotation {
      ROTATE_BEGIN,
      
      /*
       * Face color based moves
       */

      /*
       * Face quarter turns 
       */
      ROTATE_WHITE_CW, // U
      ROTATE_WHITE_CCW, // U'
      ROTATE_GREEN_CW, // L
      ROTATE_GREEN_CCW, // L'
      ROTATE_RED_CW, // F
      ROTATE_RED_CCW, // F'
      ROTATE_BLUE_CW, // R
      ROTATE_BLUE_CCW, // R'
      ROTATE_ORANGE_CW, // B
      ROTATE_ORANGE_CCW, // B'
      ROTATE_YELLOW_CW, // D
      ROTATE_YELLOW_CCW, // D'

      /* 
       * Slice quarter turns 
       */
      ROTATE_EQUATOR_SLICE_CW, // E (same direction as D) non-white/yellow slice
      ROTATE_EQUATOR_SLICE_CCW, // E' (same direction as U)
      ROTATE_MIDDLE_SLICE_CW, // M (same direction as L) non-green/blue slice
      ROTATE_MIDDLE_SLICE_CCW, // M' (same direction as R)
      ROTATE_STANDING_SLICE_CW, // S (same direction as F) non-red/orange slice
      ROTATE_STANDING_SLICE_CCW, // S' (same direction as B)
      
      /* 
       * Face half turns 
       */
      ROTATE_WHITE_2, // U2
      ROTATE_GREEN_2, // L2
      ROTATE_RED_2, // F2
      ROTATE_BLUE_2, // R2
      ROTATE_ORANGE_2, // B2
      ROTATE_YELLOW_2, // D2

      /* 
       * Slice half turns 
       */
      ROTATE_EQUATOR_SLICE_2, // E2
      ROTATE_MIDDLE_SLICE_2, // M2
      ROTATE_STANDING_SLICE_2, // S2

      /* 
       * Double quarter turns
       */
      ROTATE_WHITE_DOUBLE_CW, // u
      ROTATE_WHITE_DOUBLE_CCW, // u'
      ROTATE_GREEN_DOUBLE_CW, // l
      ROTATE_GREEN_DOUBLE_CCW, // l'
      ROTATE_RED_DOUBLE_CW, // f
      ROTATE_RED_DOUBLE_CCW, // f'
      ROTATE_BLUE_DOUBLE_CW, // r
      ROTATE_BLUE_DOUBLE_CCW, // r'
      ROTATE_ORANGE_DOUBLE_CW, // b
      ROTATE_ORANGE_DOUBLE_CCW, // b'
      ROTATE_YELLOW_DOUBLE_CW, // d
      ROTATE_YELLOW_DOUBLE_CCW, // d'
      
      /* 
       * Double half turns 
       */
      ROTATE_WHITE_DOUBLE_2, // u2
      ROTATE_GREEN_DOUBLE_2, // l2
      ROTATE_RED_DOUBLE_2, // f2
      ROTATE_BLUE_DOUBLE_2, // r2
      ROTATE_ORANGE_DOUBLE_2, // b2
      ROTATE_YELLOW_DOUBLE_2, // d2

      /* 
       * Cube rotations 
       */
      ROTATE_CUBE_X_CW,  // x (bottom is now front)
      ROTATE_CUBE_X_CCW, // x' (top is now front)
      ROTATE_CUBE_X_2, // x2 (back is now front)
      ROTATE_CUBE_Y_CW,  // y (right is now front)
      ROTATE_CUBE_Y_CCW, // y' (left is now front)
      ROTATE_CUBE_Y_2, // y2 (back is now front)
      ROTATE_CUBE_Z_CW,  // z (top is now right)
      ROTATE_CUBE_Z_CCW, // z' (bottom is now right)
      ROTATE_CUBE_Z_2, // z2 (left is now right)


      /* 
       * Quarter-turn metric moves
       */
      ROTATE_U,
      ROTATE_U_PRIME,
      ROTATE_L,
      ROTATE_L_PRIME,
      ROTATE_F,
      ROTATE_F_PRIME,
      ROTATE_R,
      ROTATE_R_PRIME,
      ROTATE_B,
      ROTATE_B_PRIME,
      ROTATE_D,
      ROTATE_D_PRIME,

      /*
       * Moves for additional notations
       * Not unique, but convenient
       */
      ROTATE_E,
      ROTATE_E_PRIME,
      ROTATE_M,
      ROTATE_M_PRIME,
      ROTATE_S,
      ROTATE_S_PRIME,
      ROTATE_U2,
      ROTATE_L2,
      ROTATE_F2,
      ROTATE_R2,
      ROTATE_B2,
      ROTATE_D2,
      ROTATE_E2,
      ROTATE_M2,
      ROTATE_S2,
      ROTATE_u,
      ROTATE_u_PRIME,
      ROTATE_l,
      ROTATE_l_PRIME,
      ROTATE_f,
      ROTATE_f_PRIME,
      ROTATE_r,
      ROTATE_r_PRIME,
      ROTATE_b,
      ROTATE_b_PRIME,
      ROTATE_d,
      ROTATE_d_PRIME,
      ROTATE_u2,
      ROTATE_l2,
      ROTATE_f2,
      ROTATE_r2,
      ROTATE_b2,
      ROTATE_d2,
      ROTATE_x,
      ROTATE_x_prime,
      ROTATE_x2,
      ROTATE_y,
      ROTATE_y_prime,
      ROTATE_y2,
      ROTATE_z,
      ROTATE_z_prime,
      ROTATE_z2,

      ROTATE_END
    };

    enum CubeDirection {
      CUBE_DIRECTION_BEGIN,
      /* Naming scheme: face up _ face left */
      /* 4 directions with white up */
      CUBE_DIRECTION_WHITE_GREEN,
      CUBE_DIRECTION_WHITE_RED,
      CUBE_DIRECTION_WHITE_BLUE,
      CUBE_DIRECTION_WHITE_ORANGE,

      /* 4 directions with green up */
      CUBE_DIRECTION_GREEN_YELLOW,
      CUBE_DIRECTION_GREEN_RED,
      CUBE_DIRECTION_GREEN_WHITE,
      CUBE_DIRECTION_GREEN_ORANGE,
      
      /* 4 directions with red up */
      CUBE_DIRECTION_RED_GREEN,
      CUBE_DIRECTION_RED_YELLOW,
      CUBE_DIRECTION_RED_BLUE,
      CUBE_DIRECTION_RED_WHITE,

      /* 4 directions with blue up */
      CUBE_DIRECTION_BLUE_WHITE,
      CUBE_DIRECTION_BLUE_RED,
      CUBE_DIRECTION_BLUE_YELLOW,
      CUBE_DIRECTION_BLUE_ORANGE,

      /* 4 directions with orange up */
      CUBE_DIRECTION_ORANGE_GREEN,
      CUBE_DIRECTION_ORANGE_WHITE,
      CUBE_DIRECTION_ORANGE_BLUE,
      CUBE_DIRECTION_ORANGE_YELLOW,

      /* 4 directions with yellow up */
      CUBE_DIRECTION_YELLOW_GREEN,
      CUBE_DIRECTION_YELLOW_ORANGE,
      CUBE_DIRECTION_YELLOW_BLUE,
      CUBE_DIRECTION_YELLOW_RED,

      CUBE_DIRECTION_END,
    };
    
#endif // 0
    
    /**************************************************************
     *
     * Support classes
     *
     */

    class Exception: public std::exception {
    public:
      Exception( const std::string& msg );
      virtual const char* what() const throw();
    protected:
      std::string mMsg;
    };

    /*
     *
     * Support classes
     *
     **************************************************************/

  }
}

std::ostream& operator<<( std::ostream& os, const cgl::rubiks::Move& move );
std::ostream& operator<<( std::ostream& os, const cgl::rubiks::Cube& cube );


#endif /* _CGLRUBIKSCUBE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
