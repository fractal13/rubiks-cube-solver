#include "cglRubiksProblem.h"
#include "cglRubiksAppConfig.h"
#include <sstream>

namespace cgl {
  namespace rubiks {

    AppConfig::AppConfig( ) {
      /* Initialize configuration parameters */
      enablePrompt( );
      disableHalfTurns( );
      disableSliceTurns( );
      displayCubeAsT( );
      displayMoveAsSubMoves( );
      setDepthLimit( 5 );
      setIDSDepth( 0 );
      setIDSLimit( 5 );
      setAStarLimit( 10.0 );
      setGenerationLimit( 1e6 );
      setStorageLimit( 1e6 );
      disableApplySolution( );
      setFaceletHeuristic( );
      setDisplaySolutionHuman( );
      disableInitialCubeAutoRemoveWildcard( );
      enableCompilePreconditions( );
    }

    bool AppConfig::updateOption( const std::string& option, const std::string& value ) {
      if( mOptions.count( option ) > 0 ) {
        std::stringstream ss( value );
        double number;
        ss >> number;
        mOptions[ option ] = number;
        return true;
      } else {
        return false;
      }
    }

    std::string AppConfig::listOptions( ) const {
      std::stringstream ss;
      bool first = true;
      for( auto it = mOptions.begin( ); it != mOptions.end( ); it++ ) {
        if( !first ) {
          ss << " ";
        }
        ss << it->first;
        first = false;
      }
      return ss.str( );
    }

    void AppConfig::showConfiguration( std::ostream& os ) const {
      for( auto it = mOptions.begin( ); it != mOptions.end( ); it++ ) {
        os << it->first << " " << it->second << std::endl;
      }
    }

    // show a prompt for the user
    void AppConfig::enablePrompt( ) {
      mOptions[ "enable_prompt" ] = 1;
    }
    void AppConfig::disablePrompt( ) {
      mOptions[ "enable_prompt" ] = 0;
    }
    bool AppConfig::prompt( ) const {
      return static_cast< int >( mOptions.at( "enable_prompt" ) ) != 0;
    }
    
    // allow 180 degree turns as part of the legal moves
    void AppConfig::enableHalfTurns( ) {
      mOptions[ "enable_half" ] = 1;
    }
    void AppConfig::disableHalfTurns( ) {
      mOptions[ "enable_half" ] = 0;
    }
    bool AppConfig::halfTurns( ) const {
      return static_cast< int >( mOptions.at( "enable_half" ) ) != 0;
    }
    
    // allow slice turns as part of the legal moves
    void AppConfig::enableSliceTurns( ) {
      mOptions[ "enable_slice" ] = 1;
    }
    void AppConfig::disableSliceTurns( ) {
      mOptions[ "enable_slice" ] = 0;
    }
    bool AppConfig::sliceTurns( ) const {
      return static_cast< int >( mOptions.at( "enable_slice" ) ) != 0;
    }

    // how to display cubes
    void AppConfig::displayCubeAsT( ) {
      mOptions[ "cube_display" ] = 1;
    }
    void AppConfig::displayCubeAsOneWord( ) {
      mOptions[ "cube_display" ] = 2;
    }
    void AppConfig::displayCubeAsOneLine( ) {
      mOptions[ "cube_display" ] = 3;
    }
    bool AppConfig::cubeDisplayT( ) const {
      return static_cast< int >( mOptions.at( "cube_display" ) ) == 1;
    }
    bool AppConfig::cubeDisplayOneWord( ) const {
      return static_cast< int >( mOptions.at( "cube_display" ) ) == 2;
    }
    bool AppConfig::cubeDisplayOneLine( ) const {
      return static_cast< int >( mOptions.at( "cube_display" ) ) == 3;
    }

    // how to display moves
    void AppConfig::displayMoveAsName( ) {
      mOptions[ "move_display" ] = 1;
    }

    void AppConfig::displayMoveAsSubMoves( ) {
      mOptions[ "move_display" ] = 2;
    }

    bool AppConfig::moveDisplayAsName( ) const {
      return static_cast< int >( mOptions.at( "move_display" ) ) == 1;
    }

    bool AppConfig::moveDisplayAsSubMoves( ) const {
      return static_cast< int >( mOptions.at( "move_display" ) ) == 2;
    }
    
    // search options
    int AppConfig::depthLimit( ) const {
      return static_cast< int >( mOptions.at( "depth_limit" ) );
    }
    void AppConfig::setDepthLimit( const int limit ) {
      mOptions[ "depth_limit" ] = limit;
    }
    int AppConfig::idsDepth( ) const {
      return static_cast< int >( mOptions.at( "ids_depth" ) );
    }
    void AppConfig::setIDSDepth( const int depth ) {
      mOptions[ "ids_depth" ] = depth;
    }
    int AppConfig::idsLimit( ) const {
      return static_cast< int >( mOptions.at( "ids_limit" ) );
    }
    void AppConfig::setIDSLimit( const int limit ) {
      mOptions[ "ids_limit" ] = limit;
    }
    double AppConfig::aStarLimit( ) const {
      return mOptions.at( "astar_limit" );
    }
    void AppConfig::setAStarLimit( const double limit ) {
      mOptions[ "astar_limit" ] = limit;
    }
    double AppConfig::generationLimit( ) const {
      return mOptions.at( "generation_limit" );
    }
    void AppConfig::setGenerationLimit( const double limit ) {
      mOptions[ "generation_limit" ] = limit;
    }
    double AppConfig::storageLimit( ) const {
      return mOptions.at( "storage_limit" );
    }
    void AppConfig::setStorageLimit( const double limit ) {
      mOptions[ "storage_limit" ] = limit;
    }
    void AppConfig::enableApplySolution( ) {
      mOptions[ "apply_solution" ] = 1;
    }
    void AppConfig::disableApplySolution( ) {
      mOptions[ "apply_solution" ] = 0;
    }
    bool AppConfig::applySolution( ) const {
      return static_cast< int >( mOptions.at( "apply_solution" ) ) != 0;
    }

    // heuristic options
    void AppConfig::setZeroHeuristic( ) {
      mOptions[ "heuristic" ] = Problem::ZERO_HEURISTIC;
    }
    void AppConfig::setFaceletHeuristic( ) {
      mOptions[ "heuristic" ] = Problem::FACELET_HEURISTIC;
    }
    void AppConfig::setEdgeFaceletHeuristic( ) {
      mOptions[ "heuristic" ] = Problem::EDGE_FACELET_HEURISTIC;
    }
    void AppConfig::setCornerFaceletHeuristic( ) {
      mOptions[ "heuristic" ] = Problem::CORNER_FACELET_HEURISTIC;
    }
    void AppConfig::setFaceletMaxHeuristic( ) {
      mOptions[ "heuristic" ] = Problem::FACELET_MAX_HEURISTIC;
    }
    void AppConfig::setColorCountHeuristic( ) {
      mOptions[ "heuristic" ] = Problem::COLOR_COUNT_HEURISTIC;
    }
    int AppConfig::heuristic( ) const {
      return static_cast< int >( mOptions.at( "heuristic" ) );
    }


    // how to display search results
    void AppConfig::setDisplaySolutionHuman( ) {
      mOptions[ "solution_display" ] = 0;
    }
    void AppConfig::setDisplaySolutionMachine( ) {
      mOptions[ "solution_display" ] = 1;
    }
    bool AppConfig::displaySolutionHuman( ) const {
      return static_cast< int >( mOptions.at( "solution_display" ) ) == 0;
    }
    bool AppConfig::displaySolutionMachine( ) const {
      return static_cast< int >( mOptions.at( "solution_display" ) ) == 1;
    }

    // auto transform starting cube's *s to xs
    void AppConfig::enableInitialCubeAutoRemoveWildcard( ) {
      mOptions[ "initial_cube_remove_wildcard" ] = 1;
    }

    void AppConfig::disableInitialCubeAutoRemoveWildcard( ) {
      mOptions[ "initial_cube_remove_wildcard" ] = 0;
    }

    bool AppConfig::initialCubeAutoRemoveWildcard( ) const {
      return static_cast< int >( mOptions.at( "initial_cube_remove_wildcard" ) ) == 1;
    }

    // compile plan action preconditions
    void AppConfig::enableCompilePreconditions( ) {
      mOptions[ "compile_preconditions" ] = 1;
    }

    void AppConfig::disableCompilePreconditions( ) {
      mOptions[ "compile_preconditions" ] = 0;
    }

    bool AppConfig::compilePreconditions( ) const {
      return static_cast< int >( mOptions.at( "compile_preconditions" ) ) == 1;
    }
    
  }
}
