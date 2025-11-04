#ifndef _RUBIKSCUBE_H_
#define _RUBIKSCUBE_H_
#include <exception>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#define ALLOW_180 1
#define ALLOW_MIDDLE_ROTATE 1

namespace cgl {
  namespace cube {

    class Exception: public std::exception {
    public:
      Exception( const std::string& msg );
      virtual const char* what() const throw();
    protected:
      std::string mMsg;
    };
    
    typedef char Color;
    enum ColorEnum { WHITE, GREEN, RED, BLUE, ORANGE, YELLOW, NUM_COLOR, NO_COLOR, ANY_COLOR };
    enum Position { TOP_LEFT, TOP_MIDDLE, TOP_RIGHT,
                    MID_LEFT, MID_MIDDLE, MID_RIGHT,
                    BOT_LEFT, BOT_MIDDLE, BOT_RIGHT, 
                    NUM_POSITION };
    enum Direction { CLOCK_WISE, COUNTER_CLOCK_WISE,
#if ALLOW_180
                     ROTATE_180,
#endif
#if ALLOW_MIDDLE_ROTATE
                     ROTATE_MIDDLE_UP,
                     ROTATE_MIDDLE_DOWN,
                     ROTATE_MIDDLE_LEFT,
                     ROTATE_MIDDLE_RIGHT,
#endif
                     NUM_DIRECTION, NO_DIRECTION };

    struct Location {
      Location( const Color& f, const Position& p ) : face( f ), position( p ) { };
      Color face;
      Position position;
    };
    
    struct MoveGroup {
      std::vector< Location > locations;
      MoveGroup& add( const Location& loc ) { locations.push_back( loc ); return *this; };
      void clear( ) { locations.clear( ); };
    };

    struct CubiePosition {
      std::vector< Location > locations;
      size_t size( ) const { return locations.size( ); }
      const Location& operator[]( const int& i ) const { return locations[ i ]; }
      CubiePosition& add( const Location& loc ) { locations.push_back( loc ); return *this; };
      void clear( ) { locations.clear( ); };
    };
    
    class RubiksCube {
    public:
      
      RubiksCube( );
      void setTile( const Color& face, const Position& position, const Color& value);
      Color getTile( const Color& face, const Position& position) const;
      void rotate( const Color& face, const Direction& direction );
      void unrotate( const Color& face, const Direction& direction );
      bool operator==( const RubiksCube& rhs ) const;
      bool operator!=( const RubiksCube& rhs ) const;
      bool operator<( const RubiksCube& rhs ) const;
      RubiksCube( const RubiksCube& src );
      RubiksCube& operator=( const RubiksCube& rhs );
      void mergeWildcard( const RubiksCube& wildcard );
      void setSolved( );
      int misplacedTiles( ) const;
      int misplacedTiles( const RubiksCube& goal ) const;
      std::string oneLineDisplay( ) const;
      std::string csvDisplay( ) const;
      static std::string csvHeader( );
      void setFromOneLineDisplay( const std::string& line );

      Location findCorner( const Color& c1, const Color& c2, const Color& c3 ) const;
      double cornerHeuristic( ) const;
      double cornerHeuristic( const RubiksCube& goal ) const;
      Location findEdge( const Color& c1, const Color& c2 ) const;
      double edgeHeuristic( ) const;
      double edgeHeuristic( const RubiksCube& goal ) const;
      double knownCubeHeuristic( ) const;

      static bool addKnownCube( const RubiksCube& cube, const double& distance );
      static void setKnownCubeMinimum( const double& minimum );
      static double getKnownCubeMinimum ( );
      static void saveKnownCubes( );
      static bool mInitialized;
      static const std::map< RubiksCube, double >& getKnownCubes( );
    protected:
      
      Color mCube[ NUM_COLOR ][ NUM_POSITION ];

      static void initializeMoveGroups( );
      static std::vector< MoveGroup > mGroups[ NUM_COLOR ][ NUM_DIRECTION ];
      static void initializeCubiePositions( );
      static std::vector< CubiePosition > mCornerPositions;
      static std::vector< CubiePosition > mEdgePositions;
      static void initializeCubieCosts( );
      static std::vector< std::vector< std::vector< std::vector< double > > > > mCornerCosts;
      static std::vector< std::vector< std::vector< std::vector< double > > > > mEdgeCosts;
      static void initializeKnownCubes( );
      static std::map< RubiksCube, double > smKnownCubes;
      static double smKnownCubeMinimum; // smallest cost not computed

    private:
      
    }; // cgl::cube::RubiksCube

    std::ostream& operator<<( std::ostream& os, const RubiksCube& cube );
    std::istream& operator>>( std::istream& is, RubiksCube& cube );
    
  }; // cgl::cube
}; // cgl



#endif /* _RUBIKSCUBE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
