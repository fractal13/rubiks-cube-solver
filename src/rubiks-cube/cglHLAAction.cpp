#include "cglHLAAction.h"
#include "cglCubeAction.h"
#include <iostream>
#include <sstream>

namespace cgl {
  namespace cube {

    bool HLAAction::gInitialized = false;
    std::vector< RubiksCube > HLAAction::gCubies;

    HLAAction::HLAAction( const std::vector< std::pair< Color, Direction > >& rotations, const RubiksCube& current_goal )
      : mRotations( rotations ), mCurrentGoal( current_goal ) {
      if ( ! gInitialized ) {
        gInitialized = true;
        initializeCubies( );
      }
    }

    HLAAction::~HLAAction( ) {
    }

    void HLAAction::Display() const {
      std::cout << "# goal: " << mCurrentGoal.oneLineDisplay( ) << std::endl;
      for( auto it = mRotations.begin( ); it != mRotations.end( ); it++ ) {
        std::stringstream ss;
        ss << "rotate ";
        switch( it->first ) {
        case cgl::cube::Action::FACE_WHITE: ss << "white"; break;
        case cgl::cube::Action::FACE_GREEN: ss << "green"; break;
        case cgl::cube::Action::FACE_RED: ss << "red"; break;
        case cgl::cube::Action::FACE_BLUE: ss << "blue"; break;
        case cgl::cube::Action::FACE_ORANGE: ss << "orange"; break;
        case cgl::cube::Action::FACE_YELLOW: ss << "yellow"; break;
        default: ss << "unknown";  break;
        }
        ss << " ";

        switch( it->second ) {
        case cgl::cube::Action::ROTATE_CW: ss << "cw"; break;
        case cgl::cube::Action::ROTATE_CCW: ss << "ccw"; break;
        default: ss << "unknown";  break;
        }

        std::cout << ss.str( ) << std::endl;
      }
    }
    
    const std::vector< std::pair< Color, Direction > >& HLAAction::getRotations( ) const {
      return mRotations;
    }

    const double HLAAction::getCost( ) const {
      return static_cast< double >( mRotations.size( ) );
    }
    
    const RubiksCube& HLAAction::getCurrentGoal( ) const {
      return mCurrentGoal;
    }

    const std::vector< RubiksCube >& HLAAction::getCubies( ) {
      if ( ! gInitialized ) {
        gInitialized = true;
        initializeCubies( );
      }
      return gCubies;
    }
    
    void HLAAction::initializeCubies( ) {
      RubiksCube cube;

      // top face's cubies
      cube.setFromOneLineDisplay( "w******** g******** ********* ********* **o****** *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "*w******* ********* ********* ********* *o******* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "**w****** ********* ********* **b****** o******** *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "***w***** *g******* ********* ********* ********* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "*****w*** ********* ********* *b******* ********* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "******w** **g****** r******** ********* ********* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "*******w* ********* *r******* ********* ********* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********w ********* **r****** b******** ********* *********" );
      gCubies.push_back( cube );
      
      // bottom face's cubies
      cube.setFromOneLineDisplay( "********* ********g ******r** ********* ********* y********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* *******r* ********* ********* *y*******" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* ********r ******b** ********* **y******" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* *******g* ********* ********* ********* ***y*****" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* ********* *******b* ********* *****y***" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ******g** ********* ********* ********o ******y**" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* ********* ********* *******o* *******y*" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* ********* ********b ******o** ********y" );
      gCubies.push_back( cube );

      // middle row's cubies
      cube.setFromOneLineDisplay( "********* *****g*** ***r***** ********* ********* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* *****r*** ***b***** ********* *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ********* ********* *****b*** ***o***** *********" );
      gCubies.push_back( cube );
      cube.setFromOneLineDisplay( "********* ***g***** ********* ********* *****o*** *********" );
      gCubies.push_back( cube );
    }
  }
}
