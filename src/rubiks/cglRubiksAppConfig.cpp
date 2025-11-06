#include "cglRubiksProblem.h"
#include "cglRubiksAppConfig.h"
#include <sstream>

namespace cgl {
  namespace rubiks {

    AppConfig::AppConfig( ) {
      /* Initialize configuration parameters */
      mOptions[ "enable_prompt" ] = 1;
      mOptions[ "enable_half" ] = 0;
      mOptions[ "enable_slice" ] = 0;
      mOptions[ "cube_display" ] = 1;
      mOptions[ "move_display" ] = 2;
      mOptions[ "depth_limit" ] = 5;
      mOptions[ "ids_depth" ] = 0;
      mOptions[ "ids_limit" ] = 5;
      mOptions[ "astar_limit" ] = 10.0;
      mOptions[ "generation_limit" ] = 1e6;
      mOptions[ "storage_limit" ] = 1e6;
      mOptions[ "apply_solution" ] = 0;
      mOptions[ "heuristic" ] = Problem::FACELET_HEURISTIC;
      mOptions[ "solution_display" ] = 0;
      mOptions[ "initial_cube_remove_wildcard" ] = 0;
      mOptions[ "compile_preconditions" ] = 1;

      // Populate mOptionStringToValues and mOptionValueToStrings
      // Boolean options
      mOptionStringToValues["enable_prompt"]["enable"] = 1.0;
      mOptionStringToValues["enable_prompt"]["disable"] = 0.0;
      mOptionValueToStrings["enable_prompt"][1.0] = "enabled";
      mOptionValueToStrings["enable_prompt"][0.0] = "disabled";

      mOptionStringToValues["enable_half"]["enable"] = 1.0;
      mOptionStringToValues["enable_half"]["disable"] = 0.0;
      mOptionValueToStrings["enable_half"][1.0] = "enabled";
      mOptionValueToStrings["enable_half"][0.0] = "disabled";

      mOptionStringToValues["enable_slice"]["enable"] = 1.0;
      mOptionStringToValues["enable_slice"]["disable"] = 0.0;
      mOptionValueToStrings["enable_slice"][1.0] = "enabled";
      mOptionValueToStrings["enable_slice"][0.0] = "disabled";

      mOptionStringToValues["apply_solution"]["enable"] = 1.0;
      mOptionStringToValues["apply_solution"]["disable"] = 0.0;
      mOptionValueToStrings["apply_solution"][1.0] = "enabled";
      mOptionValueToStrings["apply_solution"][0.0] = "disabled";

      mOptionStringToValues["initial_cube_remove_wildcard"]["enable"] = 1.0;
      mOptionStringToValues["initial_cube_remove_wildcard"]["disable"] = 0.0;
      mOptionValueToStrings["initial_cube_remove_wildcard"][1.0] = "enabled";
      mOptionValueToStrings["initial_cube_remove_wildcard"][0.0] = "disabled";

      mOptionStringToValues["compile_preconditions"]["enable"] = 1.0;
      mOptionStringToValues["compile_preconditions"]["disable"] = 0.0;
      mOptionValueToStrings["compile_preconditions"][1.0] = "enabled";
      mOptionValueToStrings["compile_preconditions"][0.0] = "disabled";

      // Enumerated options
      mOptionStringToValues["cube_display"]["t"] = 1.0;
      mOptionStringToValues["cube_display"]["one_word"] = 2.0;
      mOptionStringToValues["cube_display"]["one_line"] = 3.0;
      mOptionValueToStrings["cube_display"][1.0] = "t";
      mOptionValueToStrings["cube_display"][2.0] = "one_word";
      mOptionValueToStrings["cube_display"][3.0] = "one_line";

      mOptionStringToValues["move_display"]["name"] = 1.0;
      mOptionStringToValues["move_display"]["sub_moves"] = 2.0;
      mOptionValueToStrings["move_display"][1.0] = "name";
      mOptionValueToStrings["move_display"][2.0] = "sub_moves";

      mOptionStringToValues["heuristic"]["zero"] = Problem::ZERO_HEURISTIC;
      mOptionStringToValues["heuristic"]["facelet"] = Problem::FACELET_HEURISTIC;
      mOptionStringToValues["heuristic"]["edge_facelet"] = Problem::EDGE_FACELET_HEURISTIC;
      mOptionStringToValues["heuristic"]["corner_facelet"] = Problem::CORNER_FACELET_HEURISTIC;
      mOptionStringToValues["heuristic"]["facelet_max"] = Problem::FACELET_MAX_HEURISTIC;
      mOptionStringToValues["heuristic"]["color_count"] = Problem::COLOR_COUNT_HEURISTIC;
      mOptionValueToStrings["heuristic"][Problem::ZERO_HEURISTIC] = "zero";
      mOptionValueToStrings["heuristic"][Problem::FACELET_HEURISTIC] = "facelet";
      mOptionValueToStrings["heuristic"][Problem::EDGE_FACELET_HEURISTIC] = "edge_facelet";
      mOptionValueToStrings["heuristic"][Problem::CORNER_FACELET_HEURISTIC] = "corner_facelet";
      mOptionValueToStrings["heuristic"][Problem::FACELET_MAX_HEURISTIC] = "facelet_max";
      mOptionValueToStrings["heuristic"][Problem::COLOR_COUNT_HEURISTIC] = "color_count";

      mOptionStringToValues["solution_display"]["human"] = 0.0;
      mOptionStringToValues["solution_display"]["machine"] = 1.0;
      mOptionValueToStrings["solution_display"][0.0] = "human";
      mOptionValueToStrings["solution_display"][1.0] = "machine";
    }

    bool AppConfig::updateOption( const std::string& option, const std::string& value ) {
      auto it_options = mOptions.find(option);
      if( it_options != mOptions.end() ) {
        auto it_string_to_values = mOptionStringToValues.find(option);
        if ( it_string_to_values != mOptionStringToValues.end() ) {
          // This option has string mappings
          auto it_value_map = it_string_to_values->second.find(value);
          if ( it_value_map != it_string_to_values->second.end() ) {
            mOptions[ option ] = it_value_map->second;
            return true;
          } else {
            return false; // Invalid string value for this option
          }
        } else {
          // No string mappings, try to convert to double
          std::stringstream ss( value );
          double number;
          ss >> number;
          if (ss.fail()) {
            return false; // Could not convert to double
          }
          mOptions[ option ] = number;
          return true;
        }
      } else {
        return false;
      }
    }

    std::string AppConfig::getUpdateOptionError(const std::string& option, const std::string& value) const {
      std::stringstream ss_error;
      auto it_options = mOptions.find(option);
      if( it_options == mOptions.end() ) {
        ss_error << "Unknown configuration option: '" << option << "'.";
        return ss_error.str();
      }

      auto it_string_to_values = mOptionStringToValues.find(option);
      if ( it_string_to_values != mOptionStringToValues.end() ) {
        // This option has string mappings
        auto it_value_map = it_string_to_values->second.find(value);
        if ( it_value_map == it_string_to_values->second.end() ) {
          // Invalid string value, provide a helpful error message
          ss_error << "Invalid value '" << value << "' for option '" << option << "'. ";
          ss_error << "Allowed values are: ";
          bool first_value = true;
          for (const auto& pair : it_string_to_values->second) {
              if (!first_value) {
                  ss_error << ", ";
              }
              ss_error << "'" << pair.first << "'";
              first_value = false;
          }
          return ss_error.str();
        }
      } else {
        // No string mappings, try to convert to double
        std::stringstream ss( value );
        double number;
        ss >> number;
        if (ss.fail()) {
          ss_error << "Invalid value '" << value << "' for option '" << option << "'. Expected a numeric value.";
          return ss_error.str();
        }
      }
      return ""; // Should not reach here if updateOption returned false
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
        os << it->first << " ";
        auto it_value_to_strings = mOptionValueToStrings.find(it->first);
        if ( it_value_to_strings != mOptionValueToStrings.end() ) {
          // This option has string mappings, display the string representation
          auto it_string_value = it_value_to_strings->second.find(it->second);
          if ( it_string_value != it_value_to_strings->second.end() ) {
            os << it_string_value->second;
          } else {
            os << it->second << " (unknown string value)"; // Fallback if value not in map
          }
        } else {
          // No string mappings, display the raw double value
          os << it->second;
        }
        os << std::endl;
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
