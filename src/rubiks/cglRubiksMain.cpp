#include "cglRubiksProblem.h"
#include "cglRubiksState.h"
#include "cglRubiksAction.h"
#include "cglRubiksCube.h"
#include "cglRubiksPlanAction.h"
#include "cglRubiksCommand.h"
#include "cglRubiksAppConfig.h"
#include "cglRubiksMoveSet.h"
#include "cglRubiksCubeSet.h"
#include "cglRubiksAppData.h"
#include "ai_search.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <chrono>
#include <map>
#include <algorithm>
#include <unistd.h> // For isatty
#include <cstdio>   // For fileno

namespace cgl {
  namespace rubiks {

    void process_input_stream_aux( AppData& data );

    /*********************************************
     * Problem Generation
     */
    /* generate all cubes within `depth` moves of cube */
    void generate_cubes_aux( cgl::rubiks::Cube& cube, std::map< cgl::rubiks::Cube, std::pair< int, int > >& cubes, const int& depth, const int& max_depth, AppData& data ) {
      if( depth >= max_depth ) {
      } else {
        for( auto it = data.move_set.getMoves( ).begin( );
             it != data.move_set.getMoves( ).end( );
             it++ ) {

          const Move& move = it->second;
          cube = move * cube;
          
          if( cubes.find( cube ) == cubes.end( ) ) {
            cubes[ cube ].first = 1;
            cubes[ cube ].second = depth+1;
          } else {
            cubes[ cube ].first += 1;
            if( cubes[ cube ].second > depth + 1 ) {
              cubes[ cube ].second = depth+1;
            }
          }

          generate_cubes_aux( cube, cubes, depth+1, max_depth, data );
          cube = cube * move;
        }
      }
    }

    int generate_cubes( AppData& data ) {
      if ( data.words.size( ) < 3 ) {
        return EXIT_USAGE;
      }
      std::stringstream ss( data.words[ 1 ] );
      int max_depth = 0;
      ss >> max_depth;

      std::string filename;
      ss.clear( );
      ss.str( data.words[ 2 ] );
      ss >> filename;

      std::map< cgl::rubiks::Cube, std::pair< int, int > > cubes;

      cubes[ data.cube ].first = 1;
      cubes[ data.cube ].second = 0;
      generate_cubes_aux( data.cube, cubes, 0, max_depth, data );

      std::ofstream fout( filename );
      if( !fout  ) {
        data.os << "Unable to open " << filename << "." << std::endl;
        return EXIT_USAGE_HELP;
      }
      std::map< cgl::rubiks::Cube, std::pair< int, int > >::iterator it;
      for( it = cubes.begin( ); it != cubes.end( ); it++ ) {
        it->first.oneWordDisplay( fout );
        fout << " " << it->second.second << std::endl;
      }
      fout.close( );
      data.os << "Saved " << cubes.size( ) << " entries to " << filename << "." << std::endl;
      return EXIT_OK;
    }
    /*
     * Problem Generation
     *********************************************/  

    /*********************************************
     * Commands
     */
    // send a message to the output stream
    int echo( AppData& data ) {
      unsigned int i;
      bool first = true;
      for( i = 1; i < data.words.size( ); i++ ) {
        if( !first ) {
          data.os << " ";
        }
        data.os << data.words[ i ];
        first = false;
      }
      data.os << std::endl;
      return EXIT_OK;
    }

    int config( AppData& data ) {
      if( data.words.size( ) < 3 ) {
        return EXIT_USAGE;
      }
      if( !data.config.updateOption( data.words[ 1 ], data.words[ 2 ] ) ) {
        data.os << "Unknown configuration option: " << data.words[ 1 ] << std::endl;
        data.os << "Known options: " << data.config.listOptions( ) << std::endl;
        return EXIT_USAGE_HELP;
      } else {
        if( ( data.words[ 1 ] == "enable_half" ) ||
            ( data.words[ 1 ] == "enable_slice" ) ) {
          data.setLegalMoves( );
        }
      }
      return EXIT_OK;
    }

    int show_cube( AppData& data, const Cube& cube, bool show_endl=true ) {
      if( data.config.cubeDisplayT( ) ) {
        data.os << std::endl;
        cube.tDisplay( data.os );
      } else if( data.config.cubeDisplayOneWord( ) ) {
        cube.oneWordDisplay( data.os );
      } else if( data.config.cubeDisplayOneLine( ) ) {
        cube.oneLineDisplay( data.os );
      } else {
        data.os << "Unexpected cube display configuration." << std::endl;
        return EXIT_THROW;
      }
      if( show_endl ) {
        data.os << std::endl;
      }
      return EXIT_OK;
    }

    int show_cubes( AppData& data, const std::map< std::string, Cube >& cubes ) {
      for( auto it = cubes.begin( ); it != cubes.end( ); it++ ) {
        data.os << it->first << " ";
        show_cube( data, it->second );
      }
      return EXIT_OK;
    } 

    int show_move( AppData& data, const std::string& name, const Move& move ) {
      if( data.config.moveDisplayAsName( ) ) {
        data.os << name;
      } else if( data.config.moveDisplayAsSubMoves( ) ) {
        data.os << name << " : " << move.getName( ) << std::endl;
      } else {
        data.os << "Unexpected move display configuration." << std::endl;
        return EXIT_THROW;
      }
      return EXIT_OK;
    }

    int show_moves( AppData& data, const std::map< std::string, Move >& moves ) {
      if( data.config.moveDisplayAsSubMoves( ) ) {
        data.os << std::endl;
      }
      for( auto it = moves.begin( ); it != moves.end( ); it++ ) {
        if( false ) {
          if( it != moves.begin( ) ) {
            data.os << " ";
          }
        }
        show_move( data, it->first, it->second );
      }
      if( data.config.moveDisplayAsName( ) ) {
        data.os << std::endl;
      }
      return EXIT_OK;
    } 

    int show_plan_action( AppData& data, const PlanAction& action ) {
      data.os << "define plan_action ";
      data.os << action.getName( ) << " ";
      data.os << "precondition ";

      for( auto it = action.getPreconditions( ).begin( ); it != action.getPreconditions( ).end( ); it++ ) {
        int r = show_cube( data, *it, false );
        if( r != EXIT_OK ) {
          return r;
        }
        data.os << " ";
      }
      data.os << " effect " << action.getEffect( ).getName( );
      data.os << std::endl;
      return EXIT_OK;
    }

    int show_plan_actions( AppData& data, const std::map< std::string, PlanAction >& actions ) {
      for( auto it = actions.begin( ); it != actions.end( ); it++ ) {
        show_plan_action( data, it->second );
      }
      return EXIT_OK;
    } 

    int show( AppData& data ) {
      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      } else if( data.words[ 1 ] == "config" ) {
        data.os << "Configuration" << std::endl;
        data.config.showConfiguration( data.os );
        return EXIT_OK;
      } else if( data.words[ 1 ] == "cube" ) {
        data.os << "Cube: ";
        return show_cube( data, data.cube );
      } else if( data.words[ 1 ] == "goal" ) {
        int exit_code = EXIT_OK;
        data.os << "Goal: ";
        for( auto it = data.goal_cubes.begin( ); it != data.goal_cubes.end( ); it++ ) {
          data.os << " ";
          int r = show_cube( data, *it );
          if( r != EXIT_OK ) {
            exit_code = r;
          }
        }
        return exit_code;
      } else if( data.words[ 1 ] == "moves" ) {
        data.os << "Moves:";
        return show_moves( data, data.move_set.getMoves( ) );
      } else if( data.words[ 1 ] == "defined_moves" ) {
        data.os << "Defined Moves:";
        return show_moves( data, data.move_set.getDefinedMoves( ) );
      } else if( data.words[ 1 ] == "defined_cubes" ) {
        data.os << "Defined Cubes:" << std::endl;
        return show_cubes( data, data.cube_set.getDefinedCubes( ) );
      } else if( data.words[ 1 ] == "defined_plan_actions" ) {
        data.os << "Defined Plan Actions:" << std::endl;
        return show_plan_actions( data, data.plan_action_set.getDefinedActions( ) );
      }
      return EXIT_USAGE_HELP;
    }

    int init_cube( AppData& data, Cube& cube, unsigned int first_word, unsigned int last_word ) {
      std::stringstream ss;
      unsigned int i;
      for( i = first_word; i < last_word; i++ ) {
        ss << data.words[ i ];
      }
      if( ss.str( ) == "solved" ) {
        cube.assignSolved( );
      } else if( data.cube_set.cubeIsDefined( ss.str( ) ) ) {
        cube = data.cube_set.getDefinedCube( ss.str( ) );
      } else {
        if( ss.str( ).size( ) != cube.getFacelets( ).size( ) ) {
          data.os << "Initialization string of incorrect size: \"" << ss.str( ) << "\"" << std::endl;
          return EXIT_USAGE_HELP;
        }
        cube.assignFromString( ss.str( ) );
      }
      return EXIT_OK;
    }

    int init_cube_vector( AppData& data, std::vector< Cube >& cubes, unsigned int first_word_in, unsigned int last_word_in ) {
      /* expects 1 or more cube specifications from [ first_word, last_word ) with nothing left over */

      cubes.clear( );

      unsigned int first_word = first_word_in;
      unsigned int last_word = first_word;
      unsigned int i;
      unsigned int count = 0;
      int exit_code = EXIT_OK;
      for( i = first_word_in; i < last_word_in; i++ ) {
        if( count == 0 && ( data.cube_set.cubeIsDefined( data.words[ i ] ) || data.words[ i ] == "solved" ) ) {
          /* found a defined cube name */
          last_word = i + 1;
          cubes.resize( cubes.size( ) + 1 );
          
          int r = init_cube( data, cubes.back( ), first_word, last_word );
          if( r != EXIT_OK ) {
            exit_code = r;
            break;
          }
          first_word = i + 1;
          count = 0;
        } else {
          count += data.words[ i ].size( );
          if( count >= data.cube.getFacelets( ).size( ) ) {
            last_word = i + 1;
            cubes.resize( cubes.size( ) + 1 );
            int r = init_cube( data, cubes.back( ), first_word, last_word );
            if( r != EXIT_OK ) {
              exit_code = r;
              break;
            }
            first_word = i + 1;
            count = 0;
          }
        }
      }
      if( count != 0 ) {
        exit_code = EXIT_USAGE;
        data.os << "Cube list specification ended with incomplete cube configurations." << std::endl;
      }
      return exit_code;
    }

    int init( AppData& data ) {
      if( data.words.size( ) < 3 ) {
        return EXIT_USAGE;
      } else if( data.words[ 1 ] == "cube" ) {
        int r = init_cube( data, data.cube, 2, data.words.size( ) );
        if( r != EXIT_OK ) {
          return r;
        }
        if( data.config.initialCubeAutoRemoveWildcard( ) ) {
          data.cube.wildcardToX( );
        }
        return r;
      } else if( data.words[ 1 ] == "goal" ) {
        return init_cube_vector( data, data.goal_cubes, 2, data.words.size( ) );
      }
      return EXIT_USAGE_HELP;
    }

    int shuffle_cube_aux( AppData& data, int count ) {
      int i;
      for( i = 0 ; i < count ; i++ ) {
        data.cube = data.move_set.getRandomMove( ) * data.cube;
      }
      return EXIT_OK;
    }

    int shuffle( AppData& data ) {
      int num1, num2;
      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      } else if( data.words.size( ) > 3 ) {
        return EXIT_USAGE;
      } else if( data.words.size( ) == 2 ) {
        std::stringstream ss( data.words[ 1 ] );
        ss >> num1;
        num2 = num1;
      } else if( data.words.size( ) == 3 ) {
        std::stringstream ss( data.words[ 1 ] );
        ss >> num1;
        ss.clear( );
        ss.str( data.words[ 2 ] );
        ss >> num2;
      } else {
        return EXIT_USAGE_HELP;
      }
      if( num1 > num2 || num1 < 0) {
        return EXIT_USAGE_HELP;
      }

      int count = num1 + ( std::rand( ) % ( num2 - num1 + 1 ) );
      return shuffle_cube_aux( data, count );
    }

    bool equal_goal( AppData& data ) {
      bool eq = false;
      for( auto it = data.goal_cubes.begin( ); it != data.goal_cubes.end( ); it++ ) {
        if( data.cube == *it ) {
          eq = true;
          break;
        }
      }
      return eq;
    }

    int equal( AppData& data ) {
      Cube rhs;
      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      } else if( data.words[ 1 ] == "solved" ) {
        rhs.assignSolved( );
      } else if( data.words[ 1 ] == "goal" ) {
        bool eq = equal_goal( data );
        if( eq ) {
          data.os << "TRUE" << std::endl;
        } else {
          data.os << "FALSE" << std::endl;
        }
        return EXIT_OK;
      } else if( data.cube_set.cubeIsDefined( data.words[ 1 ] ) ) {
        rhs = data.cube_set.getDefinedCube( data.words[ 1 ] );
      } else {
        if( init_cube( data, rhs, 1, data.words.size( ) ) != EXIT_OK ) {
          return EXIT_USAGE_HELP;
        }
      }
      if( data.cube == rhs ) {
        data.os << "TRUE" << std::endl;
      } else {
        data.os << "FALSE" << std::endl;
      }
      return EXIT_OK;
    }
    
    int exit_if( AppData& data ) {
      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      } else if( data.words[ 1 ] == "not_equal_goal" ) {
        if( !equal_goal( data ) ) {
          return EXIT_EXIT;
        }
      } else {
        data.os << "Unexpected exit_if option: " << data.words[ 1 ] << std::endl;
        return EXIT_USAGE_HELP;
      }
      
      return EXIT_OK;
    }

    unsigned int next_word_length( const std::string& help, const unsigned int i ) {
      unsigned int j;
      for( j = i; j < help.size( ); j++ ) {
        if( help[ j ] == ' ' || help[ j ] == '\t' || help[ j ] == '\n' ) {
          break;
        }
      }
      return j - i;
    }

    void show_help( std::ostream& os, AppData& /*data*/, unsigned int offset, const std::string& help ) {
      unsigned int screen_width = 60;
      unsigned int indent = 5;
      unsigned int length;
      if( offset >= screen_width ) {
        offset = screen_width;
      }
      if( offset == 0 ) {
        offset = screen_width+1;
      }
      unsigned int line_length = offset;
      
      unsigned int i, j;
      for( i = 0; i < help.size( ); i++ ) {
        length = next_word_length( help, i );
        if( line_length + length > screen_width ) {
          os << std::endl;
          for( j = 0; j < indent; j++ ) {
            os << " ";
          }
          line_length = indent;
        }
        for( ; i < help.size( ) && length > 0; i++, length--, line_length++ ) {
          os << help[ i ];
        }
        if( i < help.size( ) ) {
          os << help[ i ];
          line_length++;
        }
      }
    }

    int help( AppData& data ) {
      if( data.words.size( ) == 1 ) {
        for( auto it = data.actions.begin( ); it != data.actions.end( ); it++ ) {
          data.os << it->second.usage << " :: ";
          show_help( data.os, data, 0, it->second.help );
          data.os << std::endl;
        }
      } else if( data.words.size( ) == 2 ) {
        const std::string& command = data.words[ 1 ];
        if( data.actions.count( command ) > 0 ) {
          data.os << data.actions.at( command ).usage << " :: ";
          show_help( data.os, data, 0, data.actions.at( command ).help );
          data.os << std::endl;
        } else {
          data.os << "Unknown command: '" << command << "'. No help available." << std::endl;
          return EXIT_USAGE_HELP;
        }
      } else {
        return EXIT_USAGE;
      }
      return EXIT_OK;
    }

    int run( AppData& data ) {
      int exit_status = EXIT_OK;

      bool old_prompt = data.config.prompt( );
      data.config.disablePrompt( );

      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      }
      std::stringstream ss( data.words[ 1 ] );
      std::string filename;
      ss >> filename;

      std::ifstream fin( filename );
      if( !fin ) {
        data.os << "Unable to open the file '" << filename << "'." << std::endl;
        exit_status = EXIT_USAGE;
      } else {
        AppData tmp_data( fin, data.os );
        tmp_data.config = data.config;
        tmp_data.cube = data.cube;
        tmp_data.goal_cubes = data.goal_cubes;
        tmp_data.actions = data.actions;
        tmp_data.move_set = data.move_set;
        tmp_data.cube_set = data.cube_set;
        tmp_data.done = data.done;
        tmp_data.plan_action_set = data.plan_action_set;
        tmp_data.solution_valid = data.solution_valid;
        tmp_data.solution = data.solution;

        process_input_stream_aux( tmp_data );
        fin.close( );

        data.config = tmp_data.config;
        data.cube = tmp_data.cube;
        data.goal_cubes = tmp_data.goal_cubes;
        data.move_set = tmp_data.move_set;
        data.cube_set = tmp_data.cube_set;
        data.done = tmp_data.done;
        data.plan_action_set = tmp_data.plan_action_set;
        data.solution_valid = tmp_data.solution_valid;
        data.solution = tmp_data.solution;
      }

      if( old_prompt ) {
        data.config.enablePrompt( );
      } else {
        data.config.disablePrompt( );
      }
      return exit_status;
    }

    int rotate( AppData& data ) {
      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      } else {
        unsigned int i;
        for( i = 1; i < data.words.size( ); i++ ) {
          if( !data.move_set.moveExists( data.words[ i ] ) ) {
            data.os << "Unexpected move: ( " << data.words[ i ] << " )" << std::endl;
            return EXIT_USAGE_HELP;
          }
          data.cube = data.move_set.getMove( data.words[ i ] ) * data.cube;
        }
      }
      
      return EXIT_OK;
    }
    
    int moves( AppData& data ) {
      if( data.words.size( ) < 2 ) {
        return EXIT_USAGE;
      } else if( data.words[ 1 ] == "basic" ) {
        data.setLegalMoves( );
      } else {
        unsigned int i;
        std::vector< std::string > move_names;
        for( i = 1; i < data.words.size( ); i++ ) {
          move_names.push_back( data.words[ i ] );
        }
        data.move_set.clearMoves( );
        int r = data.move_set.addMovesByName( move_names );
        if( r != EXIT_OK ) {
          for( i = 0; i < move_names.size( ); i++ ) {
            if( !data.move_set.moveIsDefined( move_names[ i ] ) ) {
              data.os << "Invalid move name used: " << move_names[ i ] << std::endl;
            }
          }
          return r;
        }
      }
      
      return EXIT_OK;
    }

    int init_move( AppData& data, Move& move, unsigned int first_word, unsigned int last_word ) {

      unsigned int i;
      std::vector< std::string > move_names;
      move = Move( Move::M_IDENTITY );
      for( i = first_word; i < last_word; i++ ) {
        if( ! data.move_set.moveIsDefined( data.words[ i ] ) ) {
          data.os << data.words[ i ] << " is not a defined move." << std::endl;
          return EXIT_USAGE;
        }
        move = data.move_set.getDefinedMove( data.words[ i ] ) * move;
      }
      return EXIT_OK;
    }

    int define_move( AppData& data ) {
      const unsigned int move_name_index = 2;
      if( data.words.size( ) < move_name_index + 2 ) {
        return EXIT_USAGE;
      } else if( data.move_set.moveIsDefined( data.words[ move_name_index ] ) ) {
        data.os << data.words[ move_name_index ] << " already defines a move." << std::endl;
        return EXIT_USAGE;
      } else if( data.words[ move_name_index + 1 ] == "solution" ) {
        if( !data.solution_valid ) {
          data.os << "Solution is not valid." << std::endl;
          return EXIT_USAGE;
        } else {
          std::stringstream ss( data.solution );
          std::string m;
          Move move( Move::M_IDENTITY );
          ss >> m;
          while( ss ) {
            move = data.move_set.getDefinedMove( m ) * move;
            ss >> m;
          }
          return data.move_set.defineMove( data.words[ move_name_index ], move );
        }
      } else {
        Move move( Move::M_IDENTITY );
        int r = init_move( data, move, move_name_index + 1, data.words.size( ) );
        if( r != EXIT_OK ) {
          return r;
        }
        return data.move_set.defineMove( data.words[ move_name_index ], move );
      }
    }

    int define_cube( AppData& data ) {
      const unsigned int cube_name_index = 2;
      if( data.words.size( ) < cube_name_index + 2 ) {
        return EXIT_USAGE;
      } else if( data.cube_set.cubeIsDefined( data.words[ cube_name_index ] ) ) {
        data.os << data.words[ cube_name_index ] << " already defines a cube." << std::endl;
        return EXIT_USAGE;
      } else {
        std::vector< Cube > cubes;
        int r =  init_cube_vector( data, cubes, cube_name_index + 1, data.words.size( ) );
        if( r != EXIT_OK ) {
          return r;
        }
        Cube cube = cubes[ 0 ];
        unsigned int i;
        for( i = 1; i < cubes.size( ); i++ ) {
          cube = cubes[ i ] + cube;
        }

        return data.cube_set.defineCube( data.words[ cube_name_index ], cube );
      }
    }

    int define_plan_action( AppData& data ) {
      const unsigned int action_name_index = 2;
      if( data.words.size( ) < action_name_index + 5 ) {
        return EXIT_USAGE;
      } else if( data.plan_action_set.actionIsDefined( data.words[ action_name_index ] ) ) {
        data.os << data.words[ action_name_index ] << " already defines a plan action." << std::endl;
        return EXIT_USAGE;
      } else {
        /*
         * Sample plan_action defininition:
         * define plan_action place_wg_edge_wr 
         *    precondition wg_edge_at_wr       
         *    effect U
         *
         * 1- Find precondition and effect keywords.  
         * 2- Everything in between must be one or more cube configurations, either by tile specification or by name.
         * 3- Everything after effect is a sequence of moves, by name.
         *
         */
        PlanAction action( data.words[ action_name_index ] );

        unsigned int precondition_index = action_name_index + 1;
        for( ; precondition_index < data.words.size( ); precondition_index++ ) {
          if( data.words[ precondition_index ] == "precondition" ) {
            break;
          }
        }
        if( precondition_index == data.words.size( ) ) {
          data.os << "Missing precondition keyword." << std::endl;
          return EXIT_USAGE_HELP;
        }

        unsigned int effect_index = precondition_index + 1;
        for( ; effect_index < data.words.size( ); effect_index++ ) {
          if( data.words[ effect_index ] == "effect" ) {
            break;
          }
        }
        if( effect_index == data.words.size( ) ) {
          data.os << "Missing effect keyword." << std::endl;
          return EXIT_USAGE_HELP;
        }
            
        std::vector< Cube > preconditions;
        int r = init_cube_vector( data, preconditions, precondition_index + 1, effect_index );
        if( r != EXIT_OK ) {
          return r;
        }
        if( data.config.compilePreconditions( ) ) {
          Cube cube = preconditions[ 0 ];
          unsigned int i;
          for( i = 1; i < preconditions.size( ); i++ ) {
            cube = preconditions[ i ] + cube;
          }
          action.clearPreconditions( );
          action.addPrecondition( cube );
        } else {
          action.setPreconditions( preconditions );
        }

        Move effect( Move::M_IDENTITY );
        r = init_move( data, effect, effect_index + 1, data.words.size( ) );
        if( r != EXIT_OK ) {
          return r;
        }
        action.setEffect( effect );

        return data.plan_action_set.defineAction( data.words[ action_name_index ], action );
      }
    }

    int define( AppData& data ) {
      if( data.words.size( ) < 4 ) {
        return EXIT_USAGE;
      } else if( data.words[ 1 ] == "cube" ) {
        return define_cube( data );
      } else if( data.words[ 1 ] == "move" ) {
        return define_move( data );
      } else if( data.words[ 1 ] == "plan_action" ) {
        return define_plan_action( data );
      } else {
        data.os << "Unrecognized object type '" << data.words[ 1 ] << "'." << std::endl;
        return EXIT_USAGE_HELP;
      }
    }

    void makeActionMap( AppData& data ) {
      data.actions.clear( );
      data.actions[ "echo" ] = { echo, "echo", "echo word*", "Displays the words of its line separated by a single space." };
      data.actions[ "config" ] =  { config, "config", "config option value", "Sets the option to value.  Only predefined options are allowed.  All values are interpreted as floating point.  Changing enable_half or enable_slice will cause the moves to be reset to basic moves, with application of these configurations." };
      data.actions[ "init" ] =  { init, "init", "init cube|goal solved|cube_configuration+", "Assigns the current state of either the working cube or the goal cube.  Goal cube may contain * for wildcards. May use the solved keyword instead of an actual configuration. A cube_configuration is either 54 facelet symbols, or the name of a defined cube. For goal, more than one configuration may be specified, space separated." };
      data.actions[ "shuffle" ] =  { shuffle, "shuffle", "shuffle num1 [num2]?", "Randomly applies rotations to the current cube num1 times. If num2 is present, a number in the range [num1, num2] (inclusive) is selected as the number of rotations to apply.  Configured moves are selected with equal probability." };
      data.actions[ "generate" ] =  { generate_cubes, "generate", "generate depth filename", "Create all unique cubes up to 'depth' steps from the current cube.  Saves the results in 'filename'." };
      data.actions[ "equal" ] =  { equal, "equal", "equal cube_configuration|solved|goal", "Checks if the current state matches the specified cube configuration, a solved cube, or one of the goal cubes.  If so, displays the string TRUE, if not, displays the string FALSE.  Note that the cube_configuration or the goal cube may contain wildcards, which match anything.  A cube_configuration is either 54 facelet symbols, or the name of a defined cube." };
      data.actions[ "search" ] =  { search, "search", "search tree|graph astar|bfs|dfs|dl|ids|bl|al|greedy|uc", "Searches for a path from the current cube to the goal cube, which may contain wildcards. Uses the search algorithm and frontier specified. dl and bl require the depth_limit configuration. ids requires the ids_limit configuration. al requires the astar_limit configuration.  All algorithms and frontiers require the generation_limit and storage_limit configurations.  The apply_solution configuration controls whether a solution is applied to the current cube." };
      data.actions[ "plan" ] =  { plan, "plan", "plan tree|graph astar|bfs|dfs|dl|ids|al|greedy|uc", "Searches for a plan from the current cube to the goal cube. Uses only the defined plan actions.  Uses the search algorithm and frontier specified. dl requires the depth_limit configuration. ids requires the ids_limit configuration. al requires the astar_limit configuration.  All algorithms and frontiers require the generation_limit and storage_limit configurations.  The apply_solution configuration controls whether a solution is applied to the current cube." };
      data.actions[ "rotate" ] =  { rotate, "rotate", "rotate [U|U'|L|L'|F|F'|R|R'|B|B'|D|D']+", "Rotates through the given sequence.  The moves must be separated by whitespace." };
      data.actions[ "moves" ] = { moves, "moves", "moves [U|U'|L|L'|F|F'|R|R'|B|B'|D|D']+|basic", "Restricts legal moves to those listed, or the basic 12 moves augmented by enable_half and enable_slice configuration options." };
      data.actions[ "show" ]   = { show, "show", "show config|cube|goal|moves|defined_moves|defined_cubes|defined_plan_actions", "Displays the requested item." };
      data.actions[ "define" ] = { define, "define", "define cube|move|plan_action object_name object_configuration", "Defines a named object of the specified type.  Cubes are defined by the 54 tile characters.  Spaces and tabs are skipped. Wildcard characters are allowed. Cubes may also be defined by a list of space separated cubes, 54 tile characters or names of defined cubes.  The result is the logical AND of all cubes in the list.  Moves are defined by one or more already defined moves, e.g. F U R U' R' F'.  Plan actions are defined as 'precondition cube1 cube2 ... effect move1 move2 ...' There must be one or more cubes in the precondition, and 1 or more moves in the effect." };
      data.actions[ "help" ] = { help, "help", "help command?", "Displays a list of commands.  If a command is given, only help on that command is displayed." };
      data.actions[ "run" ] = { run, "run", "run filename", "Runs the commands in filename as if they were typed.  Turns off the prompt while reading the file." };
      data.actions[ "exit_if" ] = { exit_if, "exit_if", "exit_if not_equal_goal", "Stops execution if the condition holds." };
    }
    /*
     * Commands
     *********************************************/

    /*********************************************
     * Input processing
     */
    /* Read until newline character or end of file
     * If the last character of the line (except for the newline) is \, then remove the \ and read the next line.
     * Store whitespace separated words in output vector
     * return true if something was read, false if any error occurred, including read after EOF was reached
     */
    bool read_one_line( std::istream& is, std::vector< std::string >& words ) {
      words.clear( );

      bool done = false;
      while( !done ) {

        std::string line;
        std::getline( is, line );
        if ( ! is ) { 
          return false;
        }
  
        std::stringstream ss( line );
        std::string word;
        while ( ss >> word ) {
          words.push_back( word );
        }
        
        if( words.size( ) > 0 && words[ words.size( ) - 1 ] == "\\" ) {
          words.pop_back( );
          done = false;
        } else {
          done = true;
        }
      }

      return true;
    }

    void process_input_stream_aux( AppData& data ) {
      
      if( data.config.prompt( ) ) {
        data.os << "cube> " << std::flush;
      }
      while( ( !data.done ) && read_one_line( data.is, data.words ) ) {
        if ( data.words.size( ) == 0 ) {
          // empty (whitespace only) lines
          continue;
        } else if ( data.words[ 0 ].size( ) == 0 ) {
          // first word is empty, how does this happen?
          continue;
        } else if ( data.words[ 0 ][ 0 ] == '#' ) {
          // comment lines begin with # as first non-whitespace character
          continue;
        } else if ( data.actions.count( data.words[ 0 ] ) > 0 ) {
          int exit_status = data.actions.at( data.words[ 0 ] ).action( data );
          switch( exit_status ) {
          case EXIT_OK:
            // empty
            break;
          case EXIT_USAGE:
            data.os << "usage: " << data.actions.at( data.words[ 0 ] ).usage << std::endl;
            break;
          case EXIT_HELP:
            data.os << "help: " << data.words[ 0 ] << " :: ";
            show_help( data.os, data, 0, data.actions.at( data.words[ 0 ] ).help );
            data.os << std::endl;
            break;
          case EXIT_USAGE_HELP:
            data.os << "usage: " << data.actions.at( data.words[ 0 ] ).usage << std::endl;
            data.os << "help: " << data.words[ 0 ] << " :: ";
            show_help( data.os, data, 0, data.actions.at( data.words[ 0 ] ).help );
            data.os << std::endl;
            break;
          case EXIT_EXIT:
            data.os << "Early termination of program." << std::endl;
            data.done = true;
            break;
          case EXIT_THROW: 
            {
              std::stringstream ss;
              ss << "Error in command: '" << data.words[ 0 ] << "'.  Exit status " << exit_status << "." << std::endl;
              ss << "usage: " << data.actions.at( data.words[ 0 ] ).usage << std::endl;
              ss << "help: " << data.actions.at( data.words[ 0 ] ).help << std::endl;
              ss << "help: " << data.words[ 0 ] << " :: ";
              show_help( ss, data, 0, data.actions.at( data.words[ 0 ] ).help );
              ss << std::endl;
              
              throw Exception( ss.str( ) );
            }
            break;
          default:
            {
              std::stringstream ss;
              ss << "Unexpected exit status: " << exit_status << std::endl;
              ss << "Error in command: '" << data.words[ 0 ] << "'.  Exit status " << exit_status << "." << std::endl;
              ss << "usage: " << data.actions.at( data.words[ 0 ] ).usage << std::endl;
              ss << "help: " << data.actions.at( data.words[ 0 ] ).help << std::endl;
              throw Exception( ss.str( ) );
            }
            break;
          }
          /*
        } else if ( data.words[ 0 ] == "hla_solve" ) { 
          hla_solve_cube( data );
        } else if ( data.words[ 0 ] == "solve_save" ) { 
          solve_save_cube( data.cube, data.words, data.config, data.goal_cube );
        } else if ( data.words[ 0 ] == "save_known_cubes" ) { 
          save_known_cubes( data.cube, data.words, data.config );
        } else if ( data.words[ 0 ] == "repeat" ) { 
          repeat_file( data );
        } else if ( data.words[ 0 ] == "find" ) { 
          find( data.cube, data.words );
        } else if ( data.words[ 0 ] == "dump_known_cubes" ) { 
          dump_known_cubes( data.cube, data.words, data.config );
          */
        } else {
          /*
          std::stringstream ss;
          ss << "Unexpected command: '" << data.words[ 0 ] << "'";
          throw Exception( ss.str( ) );
          */
          data.os << std::endl;
          data.os << "Unexpected command: '" << data.words[ 0 ] << "'" << std::endl;
          data.os << "The help command will list all available commands." << std::endl;
          data.os << std::endl;
        }
        if( data.config.prompt( ) ) {
          data.os << "cube> ";
        }
        data.os << std::flush;
      }
    }


    // "main" program control
    void process_input_stream( std::istream& is, std::ostream& os, bool is_tty ) {
      /* srand seeds at 100 usecond resolution */
      struct timeval time;
      gettimeofday(&time,NULL);
      srand((time.tv_sec * 10000) + (time.tv_usec / 100));
      
      AppData data( is, os );
      if( !is_tty ) {
        data.config.disablePrompt( );
      }
      makeActionMap( data );

      process_input_stream_aux( data );
    }
    /*
     * Input processing
     *********************************************/

  }
}

int main( ) {
  cgl::rubiks::process_input_stream( std::cin, std::cout, isatty( fileno( stdin ) ) );
  return 0;
}
