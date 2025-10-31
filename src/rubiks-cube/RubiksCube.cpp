#include "RubiksCube.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>

namespace cgl {
  namespace cube {

    bool RubiksCube::mInitialized = false;
    std::vector< MoveGroup > RubiksCube::mGroups[ NUM_COLOR ][ NUM_DIRECTION ];
    std::vector< CubiePosition > RubiksCube::mCornerPositions;
    std::vector< CubiePosition > RubiksCube::mEdgePositions;
    std::vector< std::vector< std::vector< std::vector< double > > > > RubiksCube::mCornerCosts;
    std::vector< std::vector< std::vector< std::vector< double > > > > RubiksCube::mEdgeCosts;
    std::map< RubiksCube, double > RubiksCube::smKnownCubes;
    double                         RubiksCube::smKnownCubeMinimum = 0.0;
    
    Exception::Exception( const std::string& msg ) 
      : std::exception(), mMsg( msg ) {
    }
    const char* Exception::what() const throw() {
      return mMsg.c_str( );
    }

    RubiksCube::RubiksCube( ) {
      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          mCube[i][j] = NO_COLOR;
        }
      }
      if ( ! mInitialized ) {
        mInitialized = true;
        initializeMoveGroups( );
        initializeCubiePositions( );
        initializeCubieCosts( );
        initializeKnownCubes( );
      }
    }
    
    void RubiksCube::setTile( const Color& face, const Position& position, const Color& value) {
      if( face < WHITE || face > YELLOW ) {
        throw Exception("Face out of range");
        return;
      }
      if( position < TOP_LEFT || position > BOT_RIGHT ) {
        throw Exception("Position out of range");
        return;
      }
      if( ( value < WHITE || value > YELLOW ) && ( value != ANY_COLOR ) ) {
        throw Exception("Color out of range");
        return;
      }
      mCube[ (int)face ][ position ] = value;
    }
    
    Color RubiksCube::getTile( const Color& face, const Position& position) const {
      if( face < WHITE || face > YELLOW ) {
        return NO_COLOR;
      }
      if( position < TOP_LEFT || position > BOT_RIGHT ) {
        return NO_COLOR;
      }
      return mCube[ (int)face ][ position ];
    }

    void RubiksCube::initializeMoveGroups( ) {
      std::vector< MoveGroup > groups;
      MoveGroup g;
      Color face;
      Direction direction;

      /************************************* WHITE *************************************/
      {
        face = WHITE;  direction = CLOCK_WISE;
        groups.clear( );
        /* white corners */
        g.clear( );
        g.add( Location( WHITE, TOP_LEFT ) )
          .add( Location( WHITE, TOP_RIGHT ) )
          .add( Location( WHITE, BOT_RIGHT ) )
          .add( Location( WHITE, BOT_LEFT ) );
        groups.push_back( g );

        /* white edges */
        g.clear( );
        g.add( Location( WHITE, TOP_MIDDLE ) )
          .add( Location( WHITE, MID_RIGHT ) )
          .add( Location( WHITE, BOT_MIDDLE ) )
          .add( Location( WHITE, MID_LEFT ) );
        groups.push_back( g );

        /* right corners */
        g.clear( );
        g.add( Location( RED, TOP_RIGHT ) )
          .add( Location( GREEN, TOP_RIGHT ) )
          .add( Location( ORANGE, TOP_RIGHT ) )
          .add( Location( BLUE, TOP_RIGHT ) );
        groups.push_back( g );
        
        /* left corners */
        g.clear( );
        g.add( Location( RED, TOP_LEFT ) )
          .add( Location( GREEN, TOP_LEFT ) )
          .add( Location( ORANGE, TOP_LEFT ) )
          .add( Location( BLUE, TOP_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( RED, TOP_MIDDLE ) )
          .add( Location( GREEN, TOP_MIDDLE ) )
          .add( Location( ORANGE, TOP_MIDDLE ) )
          .add( Location( BLUE, TOP_MIDDLE ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }
      
      {
        face = WHITE;  direction = COUNTER_CLOCK_WISE;
        groups.clear( );
        /* white corners */
        g.clear( );
        g.add( Location( WHITE, TOP_LEFT ) )
          .add( Location( WHITE, BOT_LEFT ) )
          .add( Location( WHITE, BOT_RIGHT ) )
          .add( Location( WHITE, TOP_RIGHT ) );
        groups.push_back( g );

        /* white edges */
        g.clear( );
        g.add( Location( WHITE, TOP_MIDDLE ) )
          .add( Location( WHITE, MID_LEFT ) )
          .add( Location( WHITE, BOT_MIDDLE ) )
          .add( Location( WHITE, MID_RIGHT ) );
        groups.push_back( g );

        /* right corners */
        g.clear( );
        g.add( Location( RED, TOP_RIGHT ) )
          .add( Location( BLUE, TOP_RIGHT ) )
          .add( Location( ORANGE, TOP_RIGHT ) )
          .add( Location( GREEN, TOP_RIGHT ) );
        groups.push_back( g );
        
        /* left corners */
        g.clear( );
        g.add( Location( RED, TOP_LEFT ) )
          .add( Location( BLUE, TOP_LEFT ) )
          .add( Location( ORANGE, TOP_LEFT ) )
          .add( Location( GREEN, TOP_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( RED, TOP_MIDDLE ) )
          .add( Location( BLUE, TOP_MIDDLE ) )
          .add( Location( ORANGE, TOP_MIDDLE ) )
          .add( Location( GREEN, TOP_MIDDLE ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }


      /************************************* GREEN *************************************/
      {
        face = GREEN;  direction = CLOCK_WISE;
        groups.clear( );
        /* green corners */
        g.clear( );
        g.add( Location( GREEN, TOP_LEFT ) )
          .add( Location( GREEN, TOP_RIGHT ) )
          .add( Location( GREEN, BOT_RIGHT ) )
          .add( Location( GREEN, BOT_LEFT ) );
        groups.push_back( g );

        /* green edges */
        g.clear( );
        g.add( Location( GREEN, TOP_MIDDLE ) )
          .add( Location( GREEN, MID_RIGHT ) )
          .add( Location( GREEN, BOT_MIDDLE ) )
          .add( Location( GREEN, MID_LEFT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( RED, TOP_LEFT ) )
          .add( Location( YELLOW, TOP_LEFT ) )
          .add( Location( ORANGE, BOT_RIGHT ) )
          .add( Location( WHITE, TOP_LEFT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( RED, BOT_LEFT ) )
          .add( Location( YELLOW, BOT_LEFT ) )
          .add( Location( ORANGE, TOP_RIGHT ) )
          .add( Location( WHITE, BOT_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( RED, MID_LEFT ) )
          .add( Location( YELLOW, MID_LEFT ) )
          .add( Location( ORANGE, MID_RIGHT ) )
          .add( Location( WHITE, MID_LEFT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }

      {
        face = GREEN;  direction = COUNTER_CLOCK_WISE;
        groups.clear( );
        /* green corners */
        g.clear( );
        g.add( Location( GREEN, TOP_LEFT ) )
          .add( Location( GREEN, BOT_LEFT ) )
          .add( Location( GREEN, BOT_RIGHT ) )
          .add( Location( GREEN, TOP_RIGHT ) );
        groups.push_back( g );

        /* green edges */
        g.clear( );
        g.add( Location( GREEN, TOP_MIDDLE ) )
          .add( Location( GREEN, MID_LEFT ) )
          .add( Location( GREEN, BOT_MIDDLE ) )
          .add( Location( GREEN, MID_RIGHT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( RED, TOP_LEFT ) )
          .add( Location( WHITE, TOP_LEFT ) )
          .add( Location( ORANGE, BOT_RIGHT ) )
          .add( Location( YELLOW, TOP_LEFT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( RED, BOT_LEFT ) )
          .add( Location( WHITE, BOT_LEFT ) )
          .add( Location( ORANGE, TOP_RIGHT ) )
          .add( Location( YELLOW, BOT_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( RED, MID_LEFT ) )
          .add( Location( WHITE, MID_LEFT ) )
          .add( Location( ORANGE, MID_RIGHT ) )
          .add( Location( YELLOW, MID_LEFT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }

      /************************************* RED *************************************/
      {
        face = RED;  direction = CLOCK_WISE;
        groups.clear( );
        /* red corners */
        g.clear( );
        g.add( Location( RED, TOP_LEFT ) )
          .add( Location( RED, TOP_RIGHT ) )
          .add( Location( RED, BOT_RIGHT ) )
          .add( Location( RED, BOT_LEFT ) );
        groups.push_back( g );

        /* red edges */
        g.clear( );
        g.add( Location( RED, TOP_MIDDLE ) )
          .add( Location( RED, MID_RIGHT ) )
          .add( Location( RED, BOT_MIDDLE ) )
          .add( Location( RED, MID_LEFT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( WHITE, BOT_LEFT ) )
          .add( Location( BLUE, TOP_LEFT ) )
          .add( Location( YELLOW, TOP_RIGHT ) )
          .add( Location( GREEN, BOT_RIGHT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( WHITE, BOT_RIGHT ) )
          .add( Location( BLUE, BOT_LEFT ) )
          .add( Location( YELLOW, TOP_LEFT ) )
          .add( Location( GREEN, TOP_RIGHT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( WHITE, BOT_MIDDLE ) )
          .add( Location( BLUE, MID_LEFT ) )
          .add( Location( YELLOW, TOP_MIDDLE ) )
          .add( Location( GREEN, MID_RIGHT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }
      
      {
        face = RED;  direction = COUNTER_CLOCK_WISE;
        groups.clear( );
        /* red corners */
        g.clear( );
        g.add( Location( RED, TOP_LEFT ) )
          .add( Location( RED, BOT_LEFT ) )
          .add( Location( RED, BOT_RIGHT ) )
          .add( Location( RED, TOP_RIGHT ) );
        groups.push_back( g );

        /* red edges */
        g.clear( );
        g.add( Location( RED, TOP_MIDDLE ) )
          .add( Location( RED, MID_LEFT ) )
          .add( Location( RED, BOT_MIDDLE ) )
          .add( Location( RED, MID_RIGHT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( WHITE, BOT_LEFT ) )
          .add( Location( GREEN, BOT_RIGHT ) )
          .add( Location( YELLOW, TOP_RIGHT ) )
          .add( Location( BLUE, TOP_LEFT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( WHITE, BOT_RIGHT ) )
          .add( Location( GREEN, TOP_RIGHT ) )
          .add( Location( YELLOW, TOP_LEFT ) )
          .add( Location( BLUE, BOT_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( WHITE, BOT_MIDDLE ) )
          .add( Location( GREEN, MID_RIGHT ) )
          .add( Location( YELLOW, TOP_MIDDLE ) )
          .add( Location( BLUE, MID_LEFT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }

      /************************************* BLUE *************************************/
      {
        face = BLUE;  direction = CLOCK_WISE;
        groups.clear( );
        /* blue corners */
        g.clear( );
        g.add( Location( BLUE, TOP_LEFT ) )
          .add( Location( BLUE, TOP_RIGHT ) )
          .add( Location( BLUE, BOT_RIGHT ) )
          .add( Location( BLUE, BOT_LEFT ) );
        groups.push_back( g );

        /* blue edges */
        g.clear( );
        g.add( Location( BLUE, TOP_MIDDLE ) )
          .add( Location( BLUE, MID_RIGHT ) )
          .add( Location( BLUE, BOT_MIDDLE ) )
          .add( Location( BLUE, MID_LEFT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( WHITE, BOT_RIGHT ) )
          .add( Location( ORANGE, TOP_LEFT ) )
          .add( Location( YELLOW, BOT_RIGHT ) )
          .add( Location( RED, BOT_RIGHT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( WHITE, TOP_RIGHT ) )
          .add( Location( ORANGE, BOT_LEFT ) )
          .add( Location( YELLOW, TOP_RIGHT ) )
          .add( Location( RED, TOP_RIGHT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( WHITE, MID_RIGHT ) )
          .add( Location( ORANGE, MID_LEFT ) )
          .add( Location( YELLOW, MID_RIGHT ) )
          .add( Location( RED, MID_RIGHT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }
      
      {
        face = BLUE;  direction = COUNTER_CLOCK_WISE;
        groups.clear( );
        /* blue corners */
        g.clear( );
        g.add( Location( BLUE, TOP_LEFT ) )
          .add( Location( BLUE, BOT_LEFT ) )
          .add( Location( BLUE, BOT_RIGHT ) )
          .add( Location( BLUE, TOP_RIGHT ) );
        groups.push_back( g );

        /* blue edges */
        g.clear( );
        g.add( Location( BLUE, TOP_MIDDLE ) )
          .add( Location( BLUE, MID_LEFT ) )
          .add( Location( BLUE, BOT_MIDDLE ) )
          .add( Location( BLUE, MID_RIGHT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( WHITE, BOT_RIGHT ) )
          .add( Location( RED, BOT_RIGHT ) )
          .add( Location( YELLOW, BOT_RIGHT ) )
          .add( Location( ORANGE, TOP_LEFT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( WHITE, TOP_RIGHT ) )
          .add( Location( RED, TOP_RIGHT ) )
          .add( Location( YELLOW, TOP_RIGHT ) )
          .add( Location( ORANGE, BOT_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( WHITE, MID_RIGHT ) )
          .add( Location( RED, MID_RIGHT ) )
          .add( Location( YELLOW, MID_RIGHT ) )
          .add( Location( ORANGE, MID_LEFT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }
      
      /************************************* ORANGE *************************************/
      {
        face = ORANGE;  direction = CLOCK_WISE;
        groups.clear( );
        /* red corners */
        g.clear( );
        g.add( Location( ORANGE, TOP_LEFT ) )
          .add( Location( ORANGE, TOP_RIGHT ) )
          .add( Location( ORANGE, BOT_RIGHT ) )
          .add( Location( ORANGE, BOT_LEFT ) );
        groups.push_back( g );

        /* red edges */
        g.clear( );
        g.add( Location( ORANGE, TOP_MIDDLE ) )
          .add( Location( ORANGE, MID_RIGHT ) )
          .add( Location( ORANGE, BOT_MIDDLE ) )
          .add( Location( ORANGE, MID_LEFT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( WHITE, TOP_LEFT ) )
          .add( Location( GREEN, BOT_LEFT ) )
          .add( Location( YELLOW, BOT_RIGHT ) )
          .add( Location( BLUE, TOP_RIGHT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( WHITE, TOP_RIGHT ) )
          .add( Location( GREEN, TOP_LEFT ) )
          .add( Location( YELLOW, BOT_LEFT ) )
          .add( Location( BLUE, BOT_RIGHT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( WHITE, TOP_MIDDLE ) )
          .add( Location( GREEN, MID_LEFT ) )
          .add( Location( YELLOW, BOT_MIDDLE ) )
          .add( Location( BLUE, MID_RIGHT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }

      {
        face = ORANGE;  direction = COUNTER_CLOCK_WISE;
        groups.clear( );
        /* red corners */
        g.clear( );
        g.add( Location( ORANGE, TOP_LEFT ) )
          .add( Location( ORANGE, BOT_LEFT ) )
          .add( Location( ORANGE, BOT_RIGHT ) )
          .add( Location( ORANGE, TOP_RIGHT ) );
        groups.push_back( g );

        /* red edges */
        g.clear( );
        g.add( Location( ORANGE, TOP_MIDDLE ) )
          .add( Location( ORANGE, MID_LEFT ) )
          .add( Location( ORANGE, BOT_MIDDLE ) )
          .add( Location( ORANGE, MID_RIGHT ) );
        groups.push_back( g );

        /* corners 1 */
        g.clear( );
        g.add( Location( WHITE, TOP_LEFT ) )
          .add( Location( BLUE, TOP_RIGHT ) )
          .add( Location( YELLOW, BOT_RIGHT ) )
          .add( Location( GREEN, BOT_LEFT ) );
        groups.push_back( g );
        
        /* corners 2 */
        g.clear( );
        g.add( Location( WHITE, TOP_RIGHT ) )
          .add( Location( BLUE, BOT_RIGHT ) )
          .add( Location( YELLOW, BOT_LEFT ) )
          .add( Location( GREEN, TOP_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( WHITE, TOP_MIDDLE ) )
          .add( Location( BLUE, MID_RIGHT ) )
          .add( Location( YELLOW, BOT_MIDDLE ) )
          .add( Location( GREEN, MID_LEFT ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }

      /************************************* YELLOW *************************************/
      {
        face = YELLOW;  direction = CLOCK_WISE;
        groups.clear( );
        /* yellow corners */
        g.clear( );
        g.add( Location( YELLOW, TOP_LEFT ) )
          .add( Location( YELLOW, TOP_RIGHT ) )
          .add( Location( YELLOW, BOT_RIGHT ) )
          .add( Location( YELLOW, BOT_LEFT ) );
        groups.push_back( g );

        /* yellow edges */
        g.clear( );
        g.add( Location( YELLOW, TOP_MIDDLE ) )
          .add( Location( YELLOW, MID_RIGHT ) )
          .add( Location( YELLOW, BOT_MIDDLE ) )
          .add( Location( YELLOW, MID_LEFT ) );
        groups.push_back( g );

        /* right corners */
        g.clear( );
        g.add( Location( RED, BOT_RIGHT ) )
          .add( Location( BLUE, BOT_RIGHT ) )
          .add( Location( ORANGE, BOT_RIGHT ) )
          .add( Location( GREEN, BOT_RIGHT ) );
        groups.push_back( g );
        
        /* left corners */
        g.clear( );
        g.add( Location( RED, BOT_LEFT ) )
          .add( Location( BLUE, BOT_LEFT ) )
          .add( Location( ORANGE, BOT_LEFT ) )
          .add( Location( GREEN, BOT_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( RED, BOT_MIDDLE ) )
          .add( Location( BLUE, BOT_MIDDLE ) )
          .add( Location( ORANGE, BOT_MIDDLE ) )
          .add( Location( GREEN, BOT_MIDDLE ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }


      {
        face = YELLOW;  direction = COUNTER_CLOCK_WISE;
        groups.clear( );
        /* yellow corners */
        g.clear( );
        g.add( Location( YELLOW, TOP_LEFT ) )
          .add( Location( YELLOW, BOT_LEFT ) )
          .add( Location( YELLOW, BOT_RIGHT ) )
          .add( Location( YELLOW, TOP_RIGHT ) );
        groups.push_back( g );

        /* yellow edges */
        g.clear( );
        g.add( Location( YELLOW, TOP_MIDDLE ) )
          .add( Location( YELLOW, MID_LEFT ) )
          .add( Location( YELLOW, BOT_MIDDLE ) )
          .add( Location( YELLOW, MID_RIGHT ) );
        groups.push_back( g );

        /* right corners */
        g.clear( );
        g.add( Location( RED, BOT_RIGHT ) )
          .add( Location( GREEN, BOT_RIGHT ) )
          .add( Location( ORANGE, BOT_RIGHT ) )
          .add( Location( BLUE, BOT_RIGHT ) );
        groups.push_back( g );
        
        /* left corners */
        g.clear( );
        g.add( Location( RED, BOT_LEFT ) )
          .add( Location( GREEN, BOT_LEFT ) )
          .add( Location( ORANGE, BOT_LEFT ) )
          .add( Location( BLUE, BOT_LEFT ) );
        groups.push_back( g );

        /* edges */
        g.clear( );
        g.add( Location( RED, BOT_MIDDLE ) )
          .add( Location( GREEN, BOT_MIDDLE ) )
          .add( Location( ORANGE, BOT_MIDDLE ) )
          .add( Location( BLUE, BOT_MIDDLE ) );
        groups.push_back( g );

        mGroups[ (int)face ][ direction ] = groups;

      }
      
      
    }

    void RubiksCube::initializeCubiePositions( ) {
      CubiePosition p;
      
      /* white corners */
      p.clear( );
      p.add( Location( WHITE, TOP_LEFT ) )
        .add( Location( GREEN, TOP_LEFT ) )
        .add( Location( ORANGE, TOP_RIGHT ) );
      mCornerPositions.push_back( p );

      p.clear( );
      p.add( Location( WHITE, TOP_RIGHT ) )
        .add( Location( ORANGE, TOP_LEFT ) )
        .add( Location( BLUE, TOP_RIGHT ) );
      mCornerPositions.push_back( p );

      p.clear( );
      p.add( Location( WHITE, BOT_LEFT ) )
        .add( Location( RED, TOP_LEFT ) )
        .add( Location( GREEN, TOP_RIGHT ) );
      mCornerPositions.push_back( p );

      p.clear( );
      p.add( Location( WHITE, BOT_RIGHT ) )
        .add( Location( BLUE, TOP_LEFT ) )
        .add( Location( RED, TOP_RIGHT ) );
      mCornerPositions.push_back( p );

      /* yellow corners */
      p.clear( );
      p.add( Location( YELLOW, TOP_LEFT ) )
        .add( Location( GREEN, BOT_RIGHT ) )
        .add( Location( RED, BOT_LEFT ) );
      mCornerPositions.push_back( p );

      p.clear( );
      p.add( Location( YELLOW, TOP_RIGHT ) )
        .add( Location( RED, BOT_RIGHT ) )
        .add( Location( BLUE, BOT_LEFT ) );
      mCornerPositions.push_back( p );

      p.clear( );
      p.add( Location( YELLOW, BOT_LEFT ) )
        .add( Location( ORANGE, BOT_RIGHT ) )
        .add( Location( GREEN, BOT_LEFT ) );
      mCornerPositions.push_back( p );

      p.clear( );
      p.add( Location( YELLOW, BOT_RIGHT ) )
        .add( Location( BLUE, BOT_RIGHT ) )
        .add( Location( ORANGE, BOT_LEFT ) );
      mCornerPositions.push_back( p );



      /* white edges */
      p.clear( );
      p.add( Location( WHITE, TOP_MIDDLE ) )
        .add( Location( ORANGE, TOP_MIDDLE ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( WHITE, MID_LEFT ) )
        .add( Location( GREEN, TOP_MIDDLE ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( WHITE, MID_RIGHT ) )
        .add( Location( BLUE, TOP_MIDDLE ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( WHITE, BOT_MIDDLE ) )
        .add( Location( RED, TOP_MIDDLE ) );
      mEdgePositions.push_back( p );
      
      /* green edges */
      p.clear( );
      p.add( Location( GREEN, MID_LEFT ) )
        .add( Location( ORANGE, MID_RIGHT ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( GREEN, MID_RIGHT ) )
        .add( Location( RED, MID_LEFT ) );
      mEdgePositions.push_back( p );

      /* blue edges */
      p.clear( );
      p.add( Location( BLUE, MID_LEFT ) )
        .add( Location( RED, MID_RIGHT ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( BLUE, MID_RIGHT ) )
        .add( Location( ORANGE, MID_LEFT ) );
      mEdgePositions.push_back( p );

      
      /* yellow edges */
      p.clear( );
      p.add( Location( YELLOW, TOP_MIDDLE ) )
        .add( Location( RED, BOT_MIDDLE ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( YELLOW, MID_LEFT ) )
        .add( Location( GREEN, BOT_MIDDLE ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( YELLOW, MID_RIGHT ) )
        .add( Location( BLUE, BOT_MIDDLE ) );
      mEdgePositions.push_back( p );

      p.clear( );
      p.add( Location( YELLOW, BOT_MIDDLE ) )
        .add( Location( ORANGE, BOT_MIDDLE ) );
      mEdgePositions.push_back( p );
      
    }

    static void createOneCornerSpace( std::vector< std::vector< double > >& costs ) {
      const int NUM_CORNER = 8;
      const int NUM_CORNER_ORIENTATION = 3;
      int i;
      costs.resize( NUM_CORNER );
      for ( i = 0; i < NUM_CORNER; i++ ) {
        costs[ i ].resize( NUM_CORNER_ORIENTATION );
      }
    }
    
    static void createOneEdgeSpace( std::vector< std::vector< double > >& costs ) {
      const int NUM_EDGE = 12;
      const int NUM_EDGE_ORIENTATION = 2;
      int i;
      costs.resize( NUM_EDGE );
      for ( i = 0; i < NUM_EDGE; i++ ) {
        costs[ i ].resize( NUM_EDGE_ORIENTATION );
      }
    }

    void RubiksCube::initializeCubieCosts( ) {
      /* These values were calculated by scrambling and solving the cubie optimally */
      mCornerCosts.resize( NUM_COLOR );

      mCornerCosts[ WHITE ].resize( NUM_POSITION );
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ WHITE ][ TOP_LEFT ] );

      mCornerCosts[ WHITE ][ TOP_LEFT ][ 0 ][ 0 ] = 0.0; // 0,0
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 0 ][ 1 ] = 2.0; // 1,0
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 0 ][ 2 ] = 2.0; // 4,2
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 1 ][ 0 ] = 1.0; // 0,2
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 1 ][ 1 ] = 3.0; // 4,0
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 1 ][ 2 ] = 1.0; // 3,2
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 2 ][ 0 ] = 1.0; // 0,6
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 2 ][ 1 ] = 1.0; // 2,0
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 2 ][ 2 ] = 3.0; // 1,2
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 3 ][ 0 ] = 2.0; // 0,8
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 3 ][ 1 ] = 2.0; // 3,0
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 3 ][ 2 ] = 2.0; // 2,2
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 4 ][ 0 ] = 2.0; // 5,0
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 4 ][ 1 ] = 2.0; // 1,8
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 4 ][ 2 ] = 2.0; // 2,6
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 5 ][ 0 ] = 3.0; // 5,2
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 5 ][ 1 ] = 3.0; // 2,8
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 5 ][ 2 ] = 3.0; // 3,6
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 6 ][ 0 ] = 3.0; // 5,6
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 6 ][ 1 ] = 1.0; // 4,8
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 6 ][ 2 ] = 1.0; // 1,6
      
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 7 ][ 0 ] = 2.0; // 5,8
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 7 ][ 1 ] = 2.0; // 3,8
      mCornerCosts[ WHITE ][ TOP_LEFT ][ 7 ][ 2 ] = 2.0; // 4,6
      
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ WHITE ][ TOP_RIGHT ] );

      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 0 ][ 0 ] = 1.0; // 0,0
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 0 ][ 1 ] = 1.0; // 1,0
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 0 ][ 2 ] = 3.0; // 4,2
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 1 ][ 0 ] = 0.0; // 0,2
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 1 ][ 1 ] = 2.0; // 4,0
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 1 ][ 2 ] = 2.0; // 3,2
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 2 ][ 0 ] = 2.0; // 0,6
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 2 ][ 1 ] = 2.0; // 2,0
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 2 ][ 2 ] = 2.0; // 1,2
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 3 ][ 0 ] = 1.0; // 0,8
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 3 ][ 1 ] = 3.0; // 3,0
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 3 ][ 2 ] = 1.0; // 2,2
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 4 ][ 0 ] = 3.0; // 5,0
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 4 ][ 1 ] = 3.0; // 1,8
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 4 ][ 2 ] = 3.0; // 2,6
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 5 ][ 0 ] = 2.0; // 5,2
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 5 ][ 1 ] = 2.0; // 2,8
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 5 ][ 2 ] = 2.0; // 3,6
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 6 ][ 0 ] = 2.0; // 5,6
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 6 ][ 1 ] = 2.0; // 4,8
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 6 ][ 2 ] = 2.0; // 1,6
      
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 7 ][ 0 ] = 3.0; // 5,8
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 7 ][ 1 ] = 1.0; // 3,8
      mCornerCosts[ WHITE ][ TOP_RIGHT ][ 7 ][ 2 ] = 1.0; // 4,6
      
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ WHITE ][ BOT_LEFT ] );

      // same side clockwise
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 0 ][ 0 ] = 1.0; // 0,0
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 0 ][ 1 ] = 3.0; // 1,0
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 0 ][ 2 ] = 1.0; // 4,2

      // same side, opposite
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 1 ][ 0 ] = 2.0; // 0,2
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 1 ][ 1 ] = 2.0; // 4,0
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 1 ][ 2 ] = 2.0; // 3,2
      
      // self
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 2 ][ 0 ] = 0.0; // 0,6
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 2 ][ 1 ] = 2.0; // 2,0
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 2 ][ 2 ] = 2.0; // 1,2
      
      // same side counter-clockwise
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 3 ][ 0 ] = 1.0; // 0,8
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 3 ][ 1 ] = 1.0; // 3,0
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 3 ][ 2 ] = 3.0; // 2,2
      
      // under
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 4 ][ 0 ] = 3.0; // 5,0
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 4 ][ 1 ] = 1.0; // 1,8
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 4 ][ 2 ] = 1.0; // 2,6
      
      // under-2
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 5 ][ 0 ] = 2.0; // 5,2
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 5 ][ 1 ] = 2.0; // 2,8
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 5 ][ 2 ] = 2.0; // 3,6
      
      // under-2
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 6 ][ 0 ] = 2.0; // 5,6
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 6 ][ 1 ] = 2.0; // 4,8
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 6 ][ 2 ] = 2.0; // 1,6
      
      // opposite
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 7 ][ 0 ] = 3.0; // 5,8
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 7 ][ 1 ] = 3.0; // 3,8
      mCornerCosts[ WHITE ][ BOT_LEFT ][ 7 ][ 2 ] = 3.0; // 4,6
      
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ WHITE ][ BOT_RIGHT ] );
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 0 ][ 0 ] = 2.0; // 0,0
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 0 ][ 1 ] = 2.0; // 1,0
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 0 ][ 2 ] = 2.0; // 4,2
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 1 ][ 0 ] = 1.0; // 0,2
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 1 ][ 1 ] = 1.0; // 4,0
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 1 ][ 2 ] = 3.0; // 3,2
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 2 ][ 0 ] = 1.0; // 0,6
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 2 ][ 1 ] = 3.0; // 2,0
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 2 ][ 2 ] = 1.0; // 1,2
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 3 ][ 0 ] = 0.0; // 0,8
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 3 ][ 1 ] = 2.0; // 3,0
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 3 ][ 2 ] = 2.0; // 2,2
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 4 ][ 0 ] = 2.0; // 5,0
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 4 ][ 1 ] = 2.0; // 1,8
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 4 ][ 2 ] = 2.0; // 2,6
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 5 ][ 0 ] = 3.0; // 5,2
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 5 ][ 1 ] = 1.0; // 2,8
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 5 ][ 2 ] = 1.0; // 3,6
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 6 ][ 0 ] = 3.0; // 5,6
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 6 ][ 1 ] = 3.0; // 4,8
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 6 ][ 2 ] = 3.0; // 1,6
      
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 7 ][ 0 ] = 2.0; // 5,8
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 7 ][ 1 ] = 2.0; // 3,8
      mCornerCosts[ WHITE ][ BOT_RIGHT ][ 7 ][ 2 ] = 2.0; // 4,6
      
      
      mCornerCosts[ YELLOW ].resize( NUM_POSITION );
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ YELLOW ][ TOP_LEFT ] );

      // under-2
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 0 ][ 0 ] = 2.0; // 0,0
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 0 ][ 1 ] = 2.0; // 1,0
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 0 ][ 2 ] = 2.0; // 4,2

      // opposite
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 1 ][ 0 ] = 3.0; // 0,2
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 1 ][ 1 ] = 3.0; // 4,0
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 1 ][ 2 ] = 3.0; // 3,2
      
      // under
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 2 ][ 0 ] = 3.0; // 0,6
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 2 ][ 1 ] = 1.0; // 2,0
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 2 ][ 2 ] = 1.0; // 1,2
      
      // under-2
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 3 ][ 0 ] = 2.0; // 0,8
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 3 ][ 1 ] = 2.0; // 3,0
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 3 ][ 2 ] = 2.0; // 2,2
      
      // self
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 4 ][ 0 ] = 0.0; // 5,0
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 4 ][ 1 ] = 2.0; // 1,8
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 4 ][ 2 ] = 2.0; // 2,6
      
      // same side clockwise
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 5 ][ 0 ] = 1.0; // 5,2
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 5 ][ 1 ] = 3.0; // 2,8
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 5 ][ 2 ] = 1.0; // 3,6
      
      // same side counter-clockwise
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 6 ][ 0 ] = 1.0; // 5,6
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 6 ][ 1 ] = 1.0; // 4,8
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 6 ][ 2 ] = 3.0; // 1,6
      
      // same side, opposite
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 7 ][ 0 ] = 2.0; // 5,8
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 7 ][ 1 ] = 2.0; // 3,8
      mCornerCosts[ YELLOW ][ TOP_LEFT ][ 7 ][ 2 ] = 2.0; // 4,6
      
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ YELLOW ][ TOP_RIGHT ] );

      // opposite
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 0 ][ 0 ] = 3.0; // 0,0
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 0 ][ 1 ] = 3.0; // 1,0
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 0 ][ 2 ] = 3.0; // 4,2

      // under-2
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 1 ][ 0 ] = 2.0; // 0,2
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 1 ][ 1 ] = 2.0; // 4,0
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 1 ][ 2 ] = 2.0; // 3,2
      
      // under-2
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 2 ][ 0 ] = 2.0; // 0,6
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 2 ][ 1 ] = 2.0; // 2,0
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 2 ][ 2 ] = 2.0; // 1,2
      
      // under
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 3 ][ 0 ] = 3.0; // 0,8
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 3 ][ 1 ] = 1.0; // 3,0
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 3 ][ 2 ] = 1.0; // 2,2
      
      // same side counter-clockwise
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 4 ][ 0 ] = 1.0; // 5,0
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 4 ][ 1 ] = 1.0; // 1,8
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 4 ][ 2 ] = 3.0; // 2,6
      
      // self
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 5 ][ 0 ] = 0.0; // 5,2
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 5 ][ 1 ] = 2.0; // 2,8
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 5 ][ 2 ] = 2.0; // 3,6
      
      // same side, opposite
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 6 ][ 0 ] = 2.0; // 5,6
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 6 ][ 1 ] = 2.0; // 4,8
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 6 ][ 2 ] = 2.0; // 1,6
      
      // same side clockwise
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 7 ][ 0 ] = 1.0; // 5,8
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 7 ][ 1 ] = 3.0; // 3,8
      mCornerCosts[ YELLOW ][ TOP_RIGHT ][ 7 ][ 2 ] = 1.0; // 4,6
      
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ YELLOW ][ BOT_LEFT ] );

      // under
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 0 ][ 0 ] = 3.0; // 0,0
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 0 ][ 1 ] = 1.0; // 1,0
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 0 ][ 2 ] = 1.0; // 4,2

      // under-2
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 1 ][ 0 ] = 2.0; // 0,2
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 1 ][ 1 ] = 2.0; // 4,0
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 1 ][ 2 ] = 2.0; // 3,2
      
      // under-2
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 2 ][ 0 ] = 2.0; // 0,6
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 2 ][ 1 ] = 2.0; // 2,0
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 2 ][ 2 ] = 2.0; // 1,2
      
      // opposite
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 3 ][ 0 ] = 3.0; // 0,8
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 3 ][ 1 ] = 3.0; // 3,0
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 3 ][ 2 ] = 3.0; // 2,2
      
      // same side clockwise
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 4 ][ 0 ] = 1.0; // 5,0
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 4 ][ 1 ] = 3.0; // 1,8
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 4 ][ 2 ] = 1.0; // 2,6
      
      // same side, opposite
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 5 ][ 0 ] = 2.0; // 5,2
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 5 ][ 1 ] = 2.0; // 2,8
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 5 ][ 2 ] = 2.0; // 3,6
      
      // self
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 6 ][ 0 ] = 0.0; // 5,6
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 6 ][ 1 ] = 2.0; // 4,8
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 6 ][ 2 ] = 2.0; // 1,6
      
      // same side counter-clockwise
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 7 ][ 0 ] = 1.0; // 5,8
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 7 ][ 1 ] = 1.0; // 3,8
      mCornerCosts[ YELLOW ][ BOT_LEFT ][ 7 ][ 2 ] = 3.0; // 4,6
      
      /////////////////////////////////////////////////////////
      createOneCornerSpace( mCornerCosts[ YELLOW ][ BOT_RIGHT ] );
      
      // under-2
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 0 ][ 0 ] = 2.0; // 0,0
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 0 ][ 1 ] = 2.0; // 1,0
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 0 ][ 2 ] = 2.0; // 4,2

      // under
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 1 ][ 0 ] = 3.0; // 0,2
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 1 ][ 1 ] = 1.0; // 4,0
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 1 ][ 2 ] = 1.0; // 3,2
      
      // opposite
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 2 ][ 0 ] = 3.0; // 0,6
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 2 ][ 1 ] = 3.0; // 2,0
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 2 ][ 2 ] = 3.0; // 1,2
      
      // under-2
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 3 ][ 0 ] = 2.0; // 0,8
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 3 ][ 1 ] = 2.0; // 3,0
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 3 ][ 2 ] = 2.0; // 2,2
      
      // same side, opposite
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 4 ][ 0 ] = 2.0; // 5,0
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 4 ][ 1 ] = 2.0; // 1,8
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 4 ][ 2 ] = 2.0; // 2,6
      
      // same side counter-clockwise
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 5 ][ 0 ] = 1.0; // 5,2
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 5 ][ 1 ] = 1.0; // 2,8
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 5 ][ 2 ] = 3.0; // 3,6
      
      // same side clockwise
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 6 ][ 0 ] = 1.0; // 5,6
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 6 ][ 1 ] = 3.0; // 4,8
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 6 ][ 2 ] = 1.0; // 1,6
      
      // self
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 7 ][ 0 ] = 0.0; // 5,8
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 7 ][ 1 ] = 2.0; // 3,8
      mCornerCosts[ YELLOW ][ BOT_RIGHT ][ 7 ][ 2 ] = 2.0; // 4,6


      ///////////////////////////////////////////////////////////////////
      // Edges
      ///////////////////////////////////////////////////////////////////
      int face = NO_COLOR, position = NUM_POSITION;
      
      mEdgeCosts.resize( NUM_COLOR );
      mEdgeCosts[ WHITE ].resize( NUM_POSITION );

      /////////////////////////////////////////////////////////
      face = WHITE; position = TOP_MIDDLE;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );
      
      // self
      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 0.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 3.0; // 4,1
      // same face counter clockwise
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 1.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 2.0; // 1,1
      // same face clockwise
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 1.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 2.0; // 3,1
      // same face opposite
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 2.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 3.0; // 2,1
      // counter clockwise close
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 1.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 2.0; // 4,5
      // counter clockwise away
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 3.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 2.0; // 2,3
      // clockwise away
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 3.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 2.0; // 2,5
      // clockwise close
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 1.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 2.0; // 4,3
      // opposite
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 4.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 3.0; // 2,7
      // counter clockwise under
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 3.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 2.0; // 1,7
      // clockwise under
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 3.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 2.0; // 3,7
      // under
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 2.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 3.0; // 4,7

      /////////////////////////////////////////////////////////
      face = WHITE; position = MID_LEFT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );
      
      // same face clockwise
      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 1.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      // self
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 0.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 3.0; // 1,1
      // same face opposite
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 2.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 3.0; // 3,1
      // same face counter clockwise
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 1.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      
      // clockwise close
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 3.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 1.0; // 4,5
      // counter clockwise close
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 2.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 1.0; // 2,3
      // counter clockwise away
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 2.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 3.0; // 2,5
      // clockwise away
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 2.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 3.0; // 4,3
      
      // counter clockwise under
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 3.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      // under
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 2.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 3.0; // 1,7
      // opposite
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 4.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 3.0; // 3,7
      // clockwise under
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 3.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7

      /////////////////////////////////////////////////////////
      face = WHITE; position = MID_RIGHT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );
      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 1.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 2.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 3.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 0.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 3.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 1.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 2.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 3.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 2.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 3.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 2.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 1.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 2.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 1.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 3.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 4.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 3.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 2.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 3.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 3.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7
      
      /////////////////////////////////////////////////////////
      face = WHITE; position = BOT_MIDDLE;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );
      
      // same face opposite
      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 2.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 3.0; // 4,1
      // same face clockwise
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 1.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 2.0; // 1,1
      // same face counter clockwise
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 1.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 2.0; // 3,1
      // self
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 0.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 3.0; // 2,1
      // clockwise away
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 3.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 2.0; // 4,5
      // clockwise close
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 1.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 2.0; // 2,3
      // counter clockwise close
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 1.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 2.0; // 2,5
      // counter clockwise away
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 3.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 2.0; // 4,3
      // under
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 2.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 3.0; // 2,7
      // clockwise under
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 3.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 2.0; // 1,7
      // counter clockwise under
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 3.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 2.0; // 3,7
      // opposite
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 4.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 3.0; // 4,7

      
      
      mEdgeCosts[ YELLOW ].resize( NUM_POSITION );
      
      /////////////////////////////////////////////////////////
      face = YELLOW; position = TOP_MIDDLE;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );
      
      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 4.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 3.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 3.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 2.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 3.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 2.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 2.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 3.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 3.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 2.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 1.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 2.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 1.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 2.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 3.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 2.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 0.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 3.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 1.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 2.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 1.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 2.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 2.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 3.0; // 4,7

      /////////////////////////////////////////////////////////
      face = YELLOW; position = MID_LEFT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );

      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 3.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 2.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 3.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 4.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 3.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 3.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 2.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 1.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 2.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 1.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 2.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 3.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 2.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 3.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 1.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 0.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 3.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 2.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 3.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 1.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7

      /////////////////////////////////////////////////////////
      face = YELLOW; position = MID_RIGHT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );
      
      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 3.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 4.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 3.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 2.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 3.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 3.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 2.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 3.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 2.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 3.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 2.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 1.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 2.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 1.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 1.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 2.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 3.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 0.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 3.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 1.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7

      /////////////////////////////////////////////////////////
      face = YELLOW; position = BOT_MIDDLE;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );

      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 2.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 3.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 3.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 2.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 3.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 2.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 4.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 3.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 1.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 2.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 3.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 2.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 3.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 2.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 1.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 2.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 2.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 3.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 1.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 2.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 1.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 2.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 0.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 3.0; // 4,7

      
      mEdgeCosts[ GREEN ].resize( NUM_POSITION );
      /////////////////////////////////////////////////////////
      face = GREEN; position = MID_LEFT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );

      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 1.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 2.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 1.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 2.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 3.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 3.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 0.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 3.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 2.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 3.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 4.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 3.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 2.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 3.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 3.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 2.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 1.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 2.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 3.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 1.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7

      /////////////////////////////////////////////////////////
      face = GREEN; position = MID_RIGHT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );

      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 3.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 2.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 1.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 2.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 3.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 1.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 2.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 3.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 0.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 3.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 2.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 3.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 4.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 3.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 1.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 2.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 1.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 2.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 3.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 3.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7

      
      mEdgeCosts[ BLUE ].resize( NUM_POSITION );
      /////////////////////////////////////////////////////////
      face = BLUE; position = MID_LEFT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );

      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 3.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 2.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 3.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 2.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 1.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 1.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 4.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 3.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 2.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 3.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 0.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 3.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 2.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 3.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 1.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 2.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 3.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 2.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 1.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 3.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7


      /////////////////////////////////////////////////////////
      face = BLUE; position = MID_RIGHT;
      createOneEdgeSpace( mEdgeCosts[ face ][ position ] );

      mEdgeCosts[ face ][ position ][ 0 ][ 0 ] = 1.0; // 0,1
      mEdgeCosts[ face ][ position ][ 0 ][ 1 ] = 2.0; // 4,1
      mEdgeCosts[ face ][ position ][ 1 ][ 0 ] = 2.0; // 0,3
      mEdgeCosts[ face ][ position ][ 1 ][ 1 ] = 3.0; // 1,1
      mEdgeCosts[ face ][ position ][ 2 ][ 0 ] = 2.0; // 0,5
      mEdgeCosts[ face ][ position ][ 2 ][ 1 ] = 1.0; // 3,1
      mEdgeCosts[ face ][ position ][ 3 ][ 0 ] = 3.0; // 0,7
      mEdgeCosts[ face ][ position ][ 3 ][ 1 ] = 2.0; // 2,1
      mEdgeCosts[ face ][ position ][ 4 ][ 0 ] = 2.0; // 1,3
      mEdgeCosts[ face ][ position ][ 4 ][ 1 ] = 3.0; // 4,5
      mEdgeCosts[ face ][ position ][ 5 ][ 0 ] = 4.0; // 1,5
      mEdgeCosts[ face ][ position ][ 5 ][ 1 ] = 3.0; // 2,3
      mEdgeCosts[ face ][ position ][ 6 ][ 0 ] = 2.0; // 3,3
      mEdgeCosts[ face ][ position ][ 6 ][ 1 ] = 3.0; // 2,5
      mEdgeCosts[ face ][ position ][ 7 ][ 0 ] = 0.0; // 3,5
      mEdgeCosts[ face ][ position ][ 7 ][ 1 ] = 3.0; // 4,3
      mEdgeCosts[ face ][ position ][ 8 ][ 0 ] = 3.0; // 5,1
      mEdgeCosts[ face ][ position ][ 8 ][ 1 ] = 2.0; // 2,7
      mEdgeCosts[ face ][ position ][ 9 ][ 0 ] = 2.0; // 5,3
      mEdgeCosts[ face ][ position ][ 9 ][ 1 ] = 3.0; // 1,7
      mEdgeCosts[ face ][ position ][ 10 ][ 0 ] = 2.0; // 5,5
      mEdgeCosts[ face ][ position ][ 10 ][ 1 ] = 1.0; // 3,7
      mEdgeCosts[ face ][ position ][ 11 ][ 0 ] = 1.0; // 5,7
      mEdgeCosts[ face ][ position ][ 11 ][ 1 ] = 2.0; // 4,7
    }

    const std::string SM_KNOWN_CUBES_FILENAME = "smKnownCubes.dat";
    
    bool RubiksCube::addKnownCube( const RubiksCube& cube, const double& distance ) {
      bool r = false;
      if( smKnownCubes.find( cube ) == smKnownCubes.end( ) ) {
        smKnownCubes[ cube ] = distance;
        r = true;
      } else if( smKnownCubes[ cube ] > distance ) {
        smKnownCubes[ cube ] = distance;
        r = true;
      }
      /*
      if( distance + 1 > smKnownCubeMinimum ) {
        smKnownCubeMinimum = distance + 1;
      }
      */
      return r;
    }

    void RubiksCube::initializeKnownCubes( ) {
      
      std::ifstream fin( SM_KNOWN_CUBES_FILENAME );
      if( fin ) {
        std::string cube_line;
        double distance;
        RubiksCube cube;

        while( fin >> cube_line >> distance ) {
          cube.setFromOneLineDisplay( cube_line );
          smKnownCubes[ cube ] = distance;
          /*
          if( distance + 1 > smKnownCubeMinimum ) {
            smKnownCubeMinimum = distance + 1;
          }
          */
        }

        fin.close( );
      } 
    }

    void RubiksCube::setKnownCubeMinimum( const double& minimum ) {
      smKnownCubeMinimum = minimum;
    }

    double RubiksCube::getKnownCubeMinimum ( ) {
      return smKnownCubeMinimum;
    }

    void RubiksCube::saveKnownCubes( ) {

      std::ofstream fout( SM_KNOWN_CUBES_FILENAME );
      if( fout ) {

        std::map< RubiksCube, double >::const_iterator it;
        for( it = smKnownCubes.begin( ); it != smKnownCubes.end( ); it++ ) {
          fout << it->first.oneLineDisplay( ) << " " << it->second << std::endl;
        }

        fout.close( );
      } 
    }

    const std::map< RubiksCube, double >& RubiksCube::getKnownCubes( ) {
      return smKnownCubes;
    }
    
    void RubiksCube::rotate( const Color& face, const Direction& direction ) {

      if( face < WHITE || face > YELLOW ) {
        throw Exception("Face out of range");
        return;
      }
#if ALLOW_180
      if( direction == ROTATE_180 ) {
        rotate( face, CLOCK_WISE );
        rotate( face, CLOCK_WISE );
        return;
      }
#endif
#if ALLOW_MIDDLE_ROTATE
      if( direction == ROTATE_MIDDLE_UP && face == RED ) {
        rotate( BLUE, COUNTER_CLOCK_WISE );
        rotate( GREEN, CLOCK_WISE );
        return;
      } else if( direction == ROTATE_MIDDLE_DOWN && face == RED ) {
        rotate( BLUE, CLOCK_WISE );
        rotate( GREEN, COUNTER_CLOCK_WISE );
        return;
      } else if( direction == ROTATE_MIDDLE_LEFT && face == RED ) {
        rotate( WHITE, COUNTER_CLOCK_WISE );
        rotate( YELLOW, CLOCK_WISE );
        return;
      } else if( direction == ROTATE_MIDDLE_RIGHT && face == RED ) {
        rotate( WHITE, CLOCK_WISE );
        rotate( YELLOW, COUNTER_CLOCK_WISE );
        return;
      } else if( direction == ROTATE_MIDDLE_UP && face == BLUE ) {
        rotate( ORANGE, COUNTER_CLOCK_WISE );
        rotate( RED, CLOCK_WISE );
        return;
      } else if( direction == ROTATE_MIDDLE_DOWN && face == BLUE ) {
        rotate( ORANGE, CLOCK_WISE );
        rotate( RED, COUNTER_CLOCK_WISE );
        return;
      }
#endif
      if( direction < CLOCK_WISE || direction > COUNTER_CLOCK_WISE ) {
        std::stringstream ss;
        ss << "Direction out of range: " << direction << " face: " << (int)face;
        throw Exception( ss.str( ) );
        return;
      }

      const std::vector< MoveGroup >& groups = mGroups[ (int)face ][ direction ];
      for( auto move_group = groups.begin( ); move_group != groups.end( ); move_group++ ) {
        Color    prev_value = NO_COLOR, tmp_value = NO_COLOR;
        for( auto location = move_group->locations.begin( ); location != move_group->locations.end( ); location++ ) {
          tmp_value = mCube[ (int)location->face ][ location->position ];
          if ( prev_value != NO_COLOR ) {
            mCube[ (int)location->face ][ location->position ] = prev_value;
          }
          prev_value = tmp_value;
        }
        auto location = move_group->locations.begin( );
        if( prev_value != NO_COLOR ) {
          mCube[ (int)location->face ][ location->position ] = prev_value;
        }
      }

    }

    void RubiksCube::unrotate( const Color& face, const Direction& direction ) {
      Direction undirection = NO_DIRECTION;
      if( direction == CLOCK_WISE ) {
        undirection = COUNTER_CLOCK_WISE;
      } else if( direction == COUNTER_CLOCK_WISE ) {
        undirection = CLOCK_WISE;
#if ALLOW_180
      } else if( direction == ROTATE_180 ) {
        undirection = ROTATE_180;
#endif
#if ALLOW_MIDDLE_ROTATE
      } else if( direction == ROTATE_MIDDLE_UP && face == RED ) {
        undirection = ROTATE_MIDDLE_DOWN;
      } else if( direction == ROTATE_MIDDLE_DOWN && face == RED ) {
        undirection = ROTATE_MIDDLE_UP;
      } else if( direction == ROTATE_MIDDLE_LEFT && face == RED ) {
        undirection = ROTATE_MIDDLE_RIGHT;
      } else if( direction == ROTATE_MIDDLE_RIGHT && face == RED ) {
        undirection = ROTATE_MIDDLE_LEFT;
      } else if( direction == ROTATE_MIDDLE_UP && face == BLUE ) {
        undirection = ROTATE_MIDDLE_DOWN;
      } else if( direction == ROTATE_MIDDLE_DOWN && face == BLUE ) {
        undirection = ROTATE_MIDDLE_UP;
#endif
      }
    rotate( face, undirection );
    }

    bool RubiksCube::operator==( const RubiksCube& rhs ) const {
      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          if ( mCube[ i ][ j ] != rhs.mCube[ i ][ j ] ) {
            if ( mCube[ i ][ j ] != ANY_COLOR && rhs.mCube[ i ][ j ] != ANY_COLOR ) {
              return false;
            }
          }
        }
      }
      return true;
    }

    bool RubiksCube::operator!=( const RubiksCube& rhs ) const {
      return !( *this == rhs );
    }

    bool RubiksCube::operator<( const RubiksCube& rhs ) const {
      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          if ( mCube[ i ][ j ] < rhs.mCube[ i ][ j ] ) {
            return true;
          } else if ( mCube[ i ][ j ] > rhs.mCube[ i ][ j ] ) {
            return false;
          }
        }
      }
      // equal
      return false;
    }

    RubiksCube& RubiksCube::operator=( const RubiksCube& rhs ) {
      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          mCube[ i ][ j ] = rhs.mCube[ i ][ j ];
        }
      }
      return *this;
    }

    void RubiksCube::mergeWildcard( const RubiksCube& wildcard ) {
      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          if( mCube[ i ][ j ] == ANY_COLOR && wildcard.mCube[ i ][ j ] != ANY_COLOR ) {
            mCube[ i ][ j ] = wildcard.mCube[ i ][ j ];
          }
        }
      }
    }

    void RubiksCube::setSolved( ) {
      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          mCube[ i ][ j ] = ( Color ) i;
        }
      }
    }

    int RubiksCube::misplacedTiles( ) const {
      int count = 0;

      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          if ( mCube[ i ][ j ] != ( Color ) i ) {
            count ++;
          }
        }
      }

      return count;
    }

    int RubiksCube::misplacedTiles( const RubiksCube& goal ) const {

      int count = 0;

      int i, j;
      for( i = 0; i < NUM_COLOR; i++ ) {
        for( j = 0; j < NUM_POSITION; j++ ) {
          if ( mCube[ i ][ j ] != goal.mCube[ i ][ j ] && goal.mCube[ i ][ j ] != ANY_COLOR ) {
            count ++;
          }
        }
      }

      return count;
      
    }

    Location RubiksCube::findCorner( const Color& c1, const Color& c2, const Color& c3 ) const {
      size_t i, j;
      for ( i = 0; i < mCornerPositions.size( ); i++ ) {
        for ( j = 0; j < mCornerPositions[ i ].size( ); j++ ) {
          const Location& l1 = mCornerPositions[ i ][ ( j + 0 ) % 3 ];
          const Location& l2 = mCornerPositions[ i ][ ( j + 1 ) % 3 ];
          const Location& l3 = mCornerPositions[ i ][ ( j + 2 ) % 3 ];
          if ( mCube[ (int)l1.face ][ (int)l1.position ] == c1 &&
               mCube[ (int)l2.face ][ (int)l2.position ] == c2 &&
               mCube[ (int)l3.face ][ (int)l3.position ] == c3 ) {
            return l1;
          }
        }
      }
      std::cout << *this << std::endl;
      throw Exception("Corner not found.");
      return Location( NO_COLOR, NUM_POSITION );
    }

    double RubiksCube::cornerHeuristic( ) const {
      // for each of the 8 corners, lookup how many moves to put it to the correct location and orientation
      size_t i, j;
      Color target_face[ 3 ] = { NO_COLOR, NO_COLOR, NO_COLOR };
      Color dst_face = NO_COLOR /*, src_face */;
      Position dst_position = NUM_POSITION /*, src_position */;
      double cost = 0.0;
      for ( i = 0; i < mCornerPositions.size( ); i++ ) {
        for ( j = 0; j < mCornerPositions[ i ].size( ); j++ ) {
          const Location& l0 = mCornerPositions[ i ][ j ];
          if ( mCube[ (int)l0.face ][ (int)l0.position ] == WHITE ||
               mCube[ (int)l0.face ][ (int)l0.position ] == YELLOW ) {
            const Location& l1 = mCornerPositions[ i ][ ( j + 1 ) % 3 ];
            const Location& l2 = mCornerPositions[ i ][ ( j + 2 ) % 3 ];
            target_face[ 0 ] = mCube[ (int)l0.face ][ (int)l0.position ];
            target_face[ 1 ] = mCube[ (int)l1.face ][ (int)l1.position ];
            target_face[ 2 ] = mCube[ (int)l2.face ][ (int)l2.position ];
            
            dst_face = target_face[ 0 ];
            //src_face = l0.face;
            //src_position = l0.position;

            if ( dst_face == WHITE ) {
              switch ( target_face[ 1 ] ) {
              case RED: dst_position = BOT_LEFT; break;
              case GREEN: dst_position = TOP_LEFT; break;
              case ORANGE: dst_position = TOP_RIGHT; break;
              case BLUE: dst_position = BOT_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = BOT_LEFT; break;
              case GREEN: dst_position = TOP_LEFT; break;
              case RED: dst_position = TOP_RIGHT; break;
              case BLUE: dst_position = BOT_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            }
            break;
          }
        }
        // SRC
        // i specifies the which of the 8 corner locations
        // j specifies which face of the corner cubie is white/yellow
        // DST
        // dst_face specifies face this cubie belongs to
        // dst_position specifies the location on the face the cubie needs to occupy
        cost += mCornerCosts[ (int)dst_face ][ (int)dst_position ][ i ][ j ];
      }

      // 4 corners move each rotation
      return std::ceil( cost / 4.0 );
    }
    
    double RubiksCube::cornerHeuristic( const RubiksCube& goal ) const {
      // for each of the 8 corners, lookup how many moves to put it to the correct location and orientation
      size_t i, j;
      Color target_face[ 3 ] = { NO_COLOR, NO_COLOR, NO_COLOR };
      Color dst_face = NO_COLOR /*, src_face */;
      Position dst_position = NUM_POSITION /*, src_position */;
      double cost = 0.0;
      for ( i = 0; i < mCornerPositions.size( ); i++ ) {
        for ( j = 0; j < mCornerPositions[ i ].size( ); j++ ) {
          const Location& l0 = mCornerPositions[ i ][ j ];
          if ( mCube[ (int)l0.face ][ (int)l0.position ] == WHITE ||
               mCube[ (int)l0.face ][ (int)l0.position ] == YELLOW ) {
            const Location& l1 = mCornerPositions[ i ][ ( j + 1 ) % 3 ];
            const Location& l2 = mCornerPositions[ i ][ ( j + 2 ) % 3 ];
            target_face[ 0 ] = mCube[ (int)l0.face ][ (int)l0.position ];
            target_face[ 1 ] = mCube[ (int)l1.face ][ (int)l1.position ];
            target_face[ 2 ] = mCube[ (int)l2.face ][ (int)l2.position ];
            
            dst_face = target_face[ 0 ];
            //src_face = l0.face;
            //src_position = l0.position;

            if ( dst_face == WHITE ) {
              switch ( target_face[ 1 ] ) {
              case RED: dst_position = BOT_LEFT; break;
              case GREEN: dst_position = TOP_LEFT; break;
              case ORANGE: dst_position = TOP_RIGHT; break;
              case BLUE: dst_position = BOT_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = BOT_LEFT; break;
              case GREEN: dst_position = TOP_LEFT; break;
              case RED: dst_position = TOP_RIGHT; break;
              case BLUE: dst_position = BOT_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            }
            break;
          }
        }
        // SRC
        // i specifies the which of the 8 corner locations
        // j specifies which face of the corner cubie is white/yellow
        // DST
        // dst_face specifies face this cubie belongs to
        // dst_position specifies the location on the face the cubie needs to occupy
        if ( goal.mCube[ (int)dst_face ][ (int)dst_position ] != ANY_COLOR ) {
          cost += mCornerCosts[ (int)dst_face ][ (int)dst_position ][ i ][ j ];
        }
      }

      // 4 corners move each rotation
      return std::ceil( cost / 4.0 );
    }
    
    Location RubiksCube::findEdge( const Color& c1, const Color& c2 ) const {
      size_t i, j;
      for ( i = 0; i < mEdgePositions.size( ); i++ ) {
        for ( j = 0; j < mEdgePositions[ i ].size( ); j++ ) {
          const Location& l1 = mEdgePositions[ i ][ ( j + 0 ) % 2 ];
          const Location& l2 = mEdgePositions[ i ][ ( j + 1 ) % 2 ];
          if ( mCube[ (int)l1.face ][ (int)l1.position ] == c1 &&
               mCube[ (int)l2.face ][ (int)l2.position ] == c2 ) {
            return l1;
          }
        }
      }
      std::cout << *this << std::endl;
      throw Exception("Edge not found.");
      return Location( NO_COLOR, NUM_POSITION );
    }

    double RubiksCube::edgeHeuristic( ) const {
      // for each of the 12 edges, lookup how many moves to put it to the correct location and orientation
      size_t i, j;
      Color target_face[ 2 ] = { NO_COLOR, NO_COLOR };
      Color dst_face = NO_COLOR;
      Position dst_position = NUM_POSITION;
      double cost = 0.0;
      for ( i = 0; i < mEdgePositions.size( ); i++ ) {
        for ( j = 0; j < mEdgePositions[ i ].size( ); j++ ) {
          const Location& l0 = mEdgePositions[ i ][ j ];
          const Location& l1 = mEdgePositions[ i ][ ( j + 1 ) % 2 ];
          if ( mCube[ (int)l0.face ][ (int)l0.position ] == WHITE ||
               mCube[ (int)l0.face ][ (int)l0.position ] == YELLOW ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == GREEN && mCube[ (int)l1.face ][ (int)l1.position ] == ORANGE ) ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == GREEN && mCube[ (int)l1.face ][ (int)l1.position ] == RED ) ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == BLUE && mCube[ (int)l1.face ][ (int)l1.position ] == ORANGE ) ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == BLUE && mCube[ (int)l1.face ][ (int)l1.position ] == RED ) ) {
            target_face[ 0 ] = mCube[ (int)l0.face ][ (int)l0.position ];
            target_face[ 1 ] = mCube[ (int)l1.face ][ (int)l1.position ];
            
            dst_face = target_face[ 0 ];

            if ( dst_face == WHITE ) {
              switch ( target_face[ 1 ] ) {
              case RED: dst_position = BOT_MIDDLE; break;
              case GREEN: dst_position = MID_LEFT; break;
              case ORANGE: dst_position = TOP_MIDDLE; break;
              case BLUE: dst_position = MID_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else if ( dst_face == YELLOW ) {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = BOT_MIDDLE; break;
              case GREEN: dst_position = MID_LEFT; break;
              case RED: dst_position = TOP_MIDDLE; break;
              case BLUE: dst_position = MID_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else if ( dst_face == GREEN ) {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = MID_LEFT; break;
              case RED: dst_position = MID_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else if ( dst_face == BLUE ) {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = MID_RIGHT; break;
              case RED: dst_position = MID_LEFT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else {
              throw Exception("dst_face out of range");
            }
            break;
          }
        }
        // SRC
        // i specifies the which of the 12 edge locations
        // j specifies which face of the edge cubie is primary white/yellow/green/blue
        // DST
        // dst_face specifies face this cubie belongs to
        // dst_position specifies the location on the face the cubie needs to occupy
        cost += mEdgeCosts[ (int)dst_face ][ (int)dst_position ][ i ][ j ];
      }

      // 4 edges move each rotation
      return std::ceil( cost / 4.0 );
    }
    
    double RubiksCube::edgeHeuristic( const RubiksCube& goal ) const {
      // for each of the 12 edges, lookup how many moves to put it to the correct location and orientation
      size_t i, j;
      Color target_face[ 2 ] = { NO_COLOR, NO_COLOR };
      Color dst_face = NO_COLOR;
      Position dst_position = NUM_POSITION;
      double cost = 0.0;
      for ( i = 0; i < mEdgePositions.size( ); i++ ) {
        for ( j = 0; j < mEdgePositions[ i ].size( ); j++ ) {
          const Location& l0 = mEdgePositions[ i ][ j ];
          const Location& l1 = mEdgePositions[ i ][ ( j + 1 ) % 2 ];
          if ( mCube[ (int)l0.face ][ (int)l0.position ] == WHITE ||
               mCube[ (int)l0.face ][ (int)l0.position ] == YELLOW ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == GREEN && mCube[ (int)l1.face ][ (int)l1.position ] == ORANGE ) ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == GREEN && mCube[ (int)l1.face ][ (int)l1.position ] == RED ) ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == BLUE && mCube[ (int)l1.face ][ (int)l1.position ] == ORANGE ) ||
               ( mCube[ (int)l0.face ][ (int)l0.position ] == BLUE && mCube[ (int)l1.face ][ (int)l1.position ] == RED ) ) {
            target_face[ 0 ] = mCube[ (int)l0.face ][ (int)l0.position ];
            target_face[ 1 ] = mCube[ (int)l1.face ][ (int)l1.position ];
            
            dst_face = target_face[ 0 ];

            if ( dst_face == WHITE ) {
              switch ( target_face[ 1 ] ) {
              case RED: dst_position = BOT_MIDDLE; break;
              case GREEN: dst_position = MID_LEFT; break;
              case ORANGE: dst_position = TOP_MIDDLE; break;
              case BLUE: dst_position = MID_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else if ( dst_face == YELLOW ) {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = BOT_MIDDLE; break;
              case GREEN: dst_position = MID_LEFT; break;
              case RED: dst_position = TOP_MIDDLE; break;
              case BLUE: dst_position = MID_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else if ( dst_face == GREEN ) {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = MID_LEFT; break;
              case RED: dst_position = MID_RIGHT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else if ( dst_face == BLUE ) {
              switch ( target_face[ 1 ] ) {
              case ORANGE: dst_position = MID_RIGHT; break;
              case RED: dst_position = MID_LEFT; break;
              default:
                throw Exception("Face out of range");
                break;
              }
            } else {
              throw Exception("dst_face out of range");
            }
            break;
          }
        }
        // SRC
        // i specifies the which of the 12 edge locations
        // j specifies which face of the edge cubie is primary white/yellow/green/blue
        // DST
        // dst_face specifies face this cubie belongs to
        // dst_position specifies the location on the face the cubie needs to occupy
        if ( goal.mCube[ (int)dst_face ][ (int)dst_position ] != ANY_COLOR ) {
          cost += mEdgeCosts[ (int)dst_face ][ (int)dst_position ][ i ][ j ];
        }
      }

      // 4 edges move each rotation
      return std::ceil( cost / 4.0 );
    }

    double RubiksCube::knownCubeHeuristic( ) const {
      double h = 0;
      if( smKnownCubes.find( *this ) != smKnownCubes.end( ) ) {
        h = smKnownCubes[ *this ];
      } else {
        h = smKnownCubeMinimum;
      }
      return h;
    }
    
    std::string RubiksCube::oneLineDisplay( ) const {
      std::stringstream ss;
      
      std::string word;
      int i, j;
      for( i = WHITE; i < NUM_COLOR; i++ ) {
        for( j = TOP_LEFT; j < NUM_POSITION; j++ ) {
          switch( getTile( (Color)i, (Position)j ) ) {
          case WHITE: word = "w"; break;
          case GREEN: word = "g"; break;
          case RED: word = "r"; break;
          case BLUE: word = "b"; break;
          case ORANGE: word = "o"; break;
          case YELLOW: word = "y"; break;
          case ANY_COLOR: word = "*"; break;
          default: {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__ << "Unexpected tile value: " << getTile( (Color)i, (Position)j );
            throw Exception( ss.str( ) );
          }
          }
          ss << word;
        }
      }
      return ss.str( );
    }

    std::string RubiksCube::csvDisplay( ) const {
      std::stringstream ss;
      
      int word = -1;
      int i, j;
      for( i = WHITE; i < NUM_COLOR; i++ ) {
        for( j = TOP_LEFT; j < NUM_POSITION; j++ ) {
          switch( getTile( (Color)i, (Position)j ) ) {
          case WHITE:
          case GREEN:
          case RED:
          case BLUE:
          case ORANGE:
          case YELLOW:
            word = getTile( (Color)i, (Position)j );
            break;
          case ANY_COLOR: word = NUM_COLOR; break;
          default: {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__<< "Unexpected tile value: " << getTile( (Color)i, (Position)j );
            throw Exception( ss.str( ) );
          }
          }
          ss << word << ",";
        }
      }
      return ss.str( );
    }

    std::string RubiksCube::csvHeader( ) {
      std::stringstream ss;
      int i, j;
      for( i = WHITE; i < NUM_COLOR; i++ ) {
        for( j = TOP_LEFT; j < NUM_POSITION; j++ ) {
          ss << "var_" << i << "_" << j << ",";
        }
      }
      return ss.str( );
    }

    void RubiksCube::setFromOneLineDisplay( const std::string& line ) {

      int i, j, k;
      int tile;
      for( k = 0, i = WHITE; i < NUM_COLOR; i++ ) {
        for( j = TOP_LEFT; j < NUM_POSITION; j++, k++ ) {
          switch( line[ k ] ) {

          case 'w': tile = WHITE; break;
          case 'g': tile = GREEN; break;
          case 'r': tile = RED; break;
          case 'b': tile = BLUE; break;
          case 'o': tile = ORANGE; break;
          case 'y': tile = YELLOW; break;
          case '*': tile = ANY_COLOR; break;
          case ' ': j--; continue; break;
          default: {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__<< " Unexpected tile value: " << line[ k ];
            ss << std::endl << line;
            throw Exception( ss.str( ) );
          }
          }
          setTile( (Color)i, (Position)j, (Color) tile );
        }
      }
    }
    
    std::ostream& operator<<( std::ostream& os, const RubiksCube& cube ) {
      std::string word;
      int i, j;
      for( i = WHITE; i < NUM_COLOR; i++ ) {
        if ( i > WHITE ) { 
          os << std::endl;
        }
        for( j = TOP_LEFT; j < NUM_POSITION; j++ ) {
          if ( j == MID_LEFT || j == BOT_LEFT ) {
            os << std::endl;
          } else if ( j > TOP_LEFT ) {
            os << " ";
          }
          switch( cube.getTile( (Color)i, (Position)j ) ) {
          case WHITE: word = "white"; break;
          case GREEN: word = "green"; break;
          case RED: word = "red"; break;
          case BLUE: word = "blue"; break;
          case ORANGE: word = "orange"; break;
          case YELLOW: word = "yellow"; break;
          case ANY_COLOR: word = "*"; break;
          default: {
            std::stringstream ss;
            ss << __FILE__ << ":" << __LINE__<< "Unexpected tile value: " << cube.getTile( (Color)i, (Position)j );
            throw Exception( ss.str( ) );
          }
          }
          os << word;
        }
        os << std::endl;
      }
      return os;
    }

    std::istream& operator>>( std::istream& is, RubiksCube& cube ) {
      std::string word;
      int face = WHITE;
      int position = TOP_LEFT;
      Color color = WHITE;
      while( face < NUM_COLOR && is ) {
        word = "";
        is >> word;
        if ( word == "white" ) {
          color = WHITE;
        } else if ( word == "green" ) {
          color = GREEN;
        } else if ( word == "red" ) {
          color = RED;
        } else if ( word == "blue" ) {
          color = BLUE;
        } else if ( word == "orange" ) {
          color = ORANGE;
        } else if ( word == "yellow" ) {
          color = YELLOW;
        } else if ( word == "*" ) {
          color = ANY_COLOR;
        } else {
          std::stringstream ss;
          ss << "Unexpected tile color: " << word << "." << std::endl;
          ss << "face: " << face << " position: " << position;
          throw Exception( ss.str( ) );
        }
        cube.setTile( (Color)face, (Position)position, color );
        position ++;
        if ( position >= NUM_POSITION ) {
          position = TOP_LEFT;
          face ++;
        }
      }
      return is;
    }

    
  };
};
