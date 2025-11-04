#include "cglCubeSolver.h"
#include "cglCubeProblem.h"
#include "cglHLAProblem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <sys/time.h>
#include <chrono>

namespace cgl {
  namespace cube {

    struct AppData {
      AppData( ) {
        cube.setSolved( );
        goal_cube.setSolved( );
      }
      std::map< std::string, double > config;
      std::vector< std::string > words;
      cgl::cube::RubiksCube cube;
      cgl::cube::RubiksCube goal_cube;
    };


    bool read_one_line( std::istream& is, std::vector< std::string >& words ) {
      words.clear( );

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
      return true;
    }

    void echo( AppData& data ) {
      unsigned int i;
      bool first = true;
      for( i = 1; i < data.words.size( ); i++ ) {
        if( !first ) {
          std::cout << " ";
        }
        std::cout << data.words[ i ];
        first = false;
      }
      std::cout << std::endl;
    }

    void initialize_solved_cube( std::istream& /*is*/, cgl::cube::RubiksCube& cube ) {
      cube.setSolved( );
    }

    void initialize_cube( std::istream& is, cgl::cube::RubiksCube& cube ) {
      is >> cube;
    }

    void initialize_line_cube( std::istream& /*is*/, AppData& data ) {
      if ( data.words.size( ) < 2 ) {
        throw cgl::cube::Exception( std::string( "Bad initial_line command: '" ) + data.words[ 0 ] + std::string( "'." ) );
      }
      data.cube.setFromOneLineDisplay( data.words[ 1 ] );
    }

    void initialize_line_goal_cube( std::istream& /*is*/, AppData& data ) {
      if ( data.words.size( ) < 2 ) {
        throw cgl::cube::Exception( std::string( "Bad goal_line command: '" ) + data.words[ 0 ] + std::string( "'." ) );
      }
      data.goal_cube.setFromOneLineDisplay( data.words[ 1 ] );
    }

    void is_equal_cube( std::istream& is, const cgl::cube::RubiksCube& cube ) {
      cgl::cube::RubiksCube cmp_cube;
      is >> cmp_cube;
      if ( cube == cmp_cube ) {
        std::cout << "TRUE" << std::endl;
      } else {
        std::cout << "FALSE" << std::endl;
      }
    }

    void show_cube( const cgl::cube::RubiksCube& cube ) {
      std::cout << cube;
    }

    void show_cube_line( const cgl::cube::RubiksCube& cube ) {
      std::cout << cube.oneLineDisplay( ) << std::endl;
    }

    void hla_solve_cube( AppData& data ) {
      cgl::cube::RubiksCube wildcard;
      wildcard.setFromOneLineDisplay( "********* ********* ********* ********* ********* ********* " );
      cgl::cube::HLAState *initial_state = new cgl::cube::HLAState( data.cube, wildcard );
      cgl::cube::HLAProblem *problem = new cgl::cube::HLAProblem( initial_state, data.goal_cube );
      ai::Search::Frontier *frontier  = new ai::Search::AStarFrontier;
      //ai::Search::Frontier *frontier  = new ai::Search::DLFrontier( 21 );
      //ai::Search::Frontier *frontier  = new ai::Search::BFFrontier( );
      ai::Search::Algorithm *search = new ai::Search::Graph( problem, frontier );
      //ai::Search::Algorithm *search = new ai::Search::Tree( problem, frontier );
      search->SetGenerationLimit( static_cast< int >( data.config.at( "generation_limit" ) ) );
      search->SetStoreLimit( static_cast< int >( data.config.at( "storage_limit" ) ) );

      if( search->Search( ) ) {
        std::cout << "Found solution." << std::endl;
        double cost = -1.0;
        std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
        std::list<ai::Search::Node *>::const_iterator it;

        for( it = solution->begin( ); it != solution->end( ); it++ ) {
          if( (*it)->GetAction( ) ) {
            cgl::cube::HLAAction *action = dynamic_cast< cgl::cube::HLAAction * >( (*it)->GetAction( ) );
            action->Display( );
            std::cout << std::endl;
          }
          cost = (*it)->GetPathCost( );
        }
        std::cout << "Cost: " << cost << std::endl;
      } else {
        std::cout << "Did not find solution." << std::endl;
      }

      delete search;
      search = 0; frontier = 0; problem = 0; initial_state = 0;
    }

    void show_config( const std::map< std::string, double >& config );

    void shuffle_cube_aux( cgl::cube::RubiksCube& cube, const int& count, std::map< std::string, double >& config ) {
      int i;
      for( i = 0 ; i < count ; i++ ) {
        cgl::cube::Color face = ( cgl::cube::Color ) ( std::rand( ) % NUM_COLOR );
        cgl::cube::Direction direction = CLOCK_WISE;
#if ALLOW_180
        if( config.at( "allow_180" ) > 0 ) {
          // includes 180
          direction = ( cgl::cube::Direction ) ( std::rand( ) % NUM_DIRECTION );
        } else {
          // excludes 180
          direction = ( cgl::cube::Direction ) ( std::rand( ) % ROTATE_180 );
        }
#else
        direction = ( cgl::cube::Direction ) ( std::rand( ) % ( COUNTER_CLOCK_WISE + 1 ) );
#endif
        cube.rotate( face, direction );
      }
    }

    void shuffle_cube( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, std::map< std::string, double >& config ) {
      if ( words.size( ) < 2 ) {
        throw cgl::cube::Exception( std::string( "Bad shuffle command: '" ) + words[ 0 ] + std::string( "'." ) );
      }
      std::stringstream ss( words[ 1 ] );
      int count = 0;
      ss >> count;

      shuffle_cube_aux( cube, count, config );
    }
      

    void shuffle_range_cube( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, std::map< std::string, double >& config ) {
      if ( words.size( ) < 3 ) {
        throw cgl::cube::Exception( std::string( "Bad shuffle_range command: '" ) + words[ 0 ] + std::string( "'." ) );
      }
      std::stringstream ss( words[ 1 ] );
      int count_min = 0;
      ss >> count_min;

      ss.clear( );
      ss.str( words[ 2 ] );
      int count_max = 0;
      ss >> count_max;
      
      int count = count_min + ( std::rand( ) % ( count_max - count_min + 1 ) );
      //std::cout << "Shuffling " << count << " times." << std::endl;
      shuffle_cube_aux( cube, count, config );
    }
      
    void rotate_cube( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words ) {
      if ( words.size( ) < 2 ) {
        throw cgl::cube::Exception( std::string( "Bad rotate command: '" ) + words[ 0 ] + std::string( "'." ) );
      } else if ( words.size( ) == 2 && words[ 1 ] == "done" ) {
        // noop
      } else if ( words.size( ) == 2  ) {
        throw cgl::cube::Exception( std::string( "Bad rotate command: '" ) + words[ 0 ] + " " + words[ 1 ] + std::string( "'." ) );
      } else if ( words.size( ) > 3  ) {
        throw cgl::cube::Exception( std::string( "Bad rotate command, too many words." ) );
      } else {
    
        cgl::cube::Color face = cgl::cube::NO_COLOR;
        if ( words[ 1 ] == "white" ) {
          face = cgl::cube::WHITE;
        } else if ( words[ 1 ] == "green" ) {
          face = cgl::cube::GREEN;
        } else if ( words[ 1 ] == "red" ) {
          face = cgl::cube::RED;
        } else if ( words[ 1 ] == "blue" ) {
          face = cgl::cube::BLUE;
        } else if ( words[ 1 ] == "orange" ) {
          face = cgl::cube::ORANGE;
        } else if ( words[ 1 ] == "yellow" ) {
          face = cgl::cube::YELLOW;
        } else {
          throw cgl::cube::Exception( std::string( "Bad rotate command color: '" ) + words[ 1 ] + std::string( "'." ) );
        }
    
        cgl::cube::Direction direction = cgl::cube::NO_DIRECTION;
        if ( words[ 2 ] == "cw" ) {
          direction = cgl::cube::CLOCK_WISE;
        } else if ( words[ 2 ] == "ccw" ) {
          direction = cgl::cube::COUNTER_CLOCK_WISE;
#if ALLOW_180
        } else if ( words[ 2 ] == "180" ) {
          direction = cgl::cube::ROTATE_180;
#endif
#if ALLOW_MIDDLE_ROTATE
        } else if ( words[ 2 ] == "up" ) {
          direction = cgl::cube::ROTATE_MIDDLE_UP;
        } else if ( words[ 2 ] == "down" ) {
          direction = cgl::cube::ROTATE_MIDDLE_DOWN;
        } else if ( words[ 2 ] == "left" ) {
          direction = cgl::cube::ROTATE_MIDDLE_LEFT;
        } else if ( words[ 2 ] == "right" ) {
          direction = cgl::cube::ROTATE_MIDDLE_RIGHT;
#endif
        } else {
          throw cgl::cube::Exception( std::string( "Bad rotate command direction: '" ) + words[ 2 ] + std::string( "'." ) );
        }

        cube.rotate( face, direction );
      }

    }

    void find( const cgl::cube::RubiksCube& cube, const std::vector< std::string >& words ) {
      if ( words.size( ) < 2 ) {
        throw cgl::cube::Exception( std::string( "Bad find command, too few words.: '" ) + words[ 0 ] + std::string( "'." ) );
      } else if ( words.size( ) > 3  ) {
        throw cgl::cube::Exception( std::string( "Bad find command, too many words." ) );
      } else {
    
        if ( words[ 1 ] == "corner" ) {
          // Looking for location/spin of white, red, green corner
          cgl::cube::Location position = cube.findCorner( cgl::cube::WHITE, cgl::cube::RED, cgl::cube::GREEN );
          std::cout << "corner position " << (int)position.face << " " << (int)position.position << std::endl;
        } else if ( words[ 1 ] == "edge" ) {
          // Looking for location/spin of white, red edge
          cgl::cube::Location position = cube.findEdge( cgl::cube::WHITE, cgl::cube::RED );
          std::cout << "edge position " << (int)position.face << " " << (int)position.position << std::endl;
        } else {
          throw cgl::cube::Exception( std::string( "Bad find command action: '" ) + words[ 1 ] + std::string( "'." ) );
        }
      }
    }

    
    ai::Search::Algorithm* solve_cube_init( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, const std::map< std::string, double >& config, const cgl::cube::RubiksCube& goal_cube, int ids_depth_limit=0 ) {
      // initial state
      cgl::cube::State *initial_state = new cgl::cube::State( cube );
      // problem
      cgl::cube::Problem *problem = new cgl::cube::Problem( initial_state, goal_cube );
#if ALLOW_180
      if( config.at( "allow_180" ) > 0 ) {
        problem->allow180( );
      } else {
        problem->disallow180( );
      }
#endif
#if ALLOW_MIDDLE_ROTATE
      if( config.at( "allow_middle_rotate" ) > 0 ) {
        problem->allowMiddleRotate( );
      } else {
        problem->disallowMiddleRotate( );
      }
#endif
      if( config.at( "edge_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::EDGE_HEURISTIC );
      }
      if( config.at( "corner_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::CORNER_HEURISTIC );
      }
      if( config.at( "tile_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::TILE_HEURISTIC );
      }
      if( config.at( "end_game_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::END_GAME_HEURISTIC );
      }
      if( config.at( "edge_corner_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::EDGE_CORNER_HEURISTIC );
      }
      if( config.at( "all_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::ALL_HEURISTIC );
      }
      if( config.at( "zero_heuristic" ) > 0 ) {
        problem->setHeuristic( cgl::cube::ZERO_HEURISTIC );
      }
      if( config.at( "known_cube_minimum" ) > 0 ) {
        cgl::cube::RubiksCube::setKnownCubeMinimum( config.at( "known_cube_minimum" ) );
      }
      
      // search frontier style
      ai::Search::Frontier *frontier  = 0;
      if ( words.size( ) > 2 ) {
        if ( words[ 2 ] == "astar" ) {
          frontier  = new ai::Search::AStarFrontier;
        } else if ( words[ 2 ] == "bfs" ) {
          frontier  = new ai::Search::BFFrontier;
        } else if ( words[ 2 ] == "dfs" ) {
          frontier  = new ai::Search::DFFrontier;
        } else if ( words[ 2 ] == "dl" ) {
          frontier  = new ai::Search::DLFrontier( static_cast< int >( config.at( "depth_limit" ) ) );
        } else if ( words[ 2 ] == "ids" ) {
          frontier  = new ai::Search::DLFrontier( ids_depth_limit );
        } else if ( words[ 2 ] == "al" ) {
          frontier  = new ai::Search::AStarLimitedFrontier( config.at( "astar_limit" ) );
        } else if ( words[ 2 ] == "greedy" ) {
          frontier  = new ai::Search::GreedyFrontier;
        } else if ( words[ 2 ] == "uc" ) {
          frontier  = new ai::Search::UCFrontier;
        } else {
          delete problem;
          throw cgl::cube::Exception( std::string( "usage: solve tree|graph astar|bfs|dfs|dl|al|greedy|uc\nfrontier name (" ) + words[ 2 ] + ( ") not recognized." ) );
          return 0;
        }

      } else {
        delete problem;
        throw cgl::cube::Exception( std::string( "usage: solve tree|graph astar|bfs|dfs|dl|al|greedy|uc\nfrontier not found" ) );
        return 0;
      }
      
      // search algorithm
      ai::Search::Algorithm *search = 0;
      if ( words.size( ) > 1 && words[ 1 ] == "tree" ) {
        search = new ai::Search::Tree( problem, frontier );
      } else if ( words.size( ) > 1 && words[ 1 ] == "graph" ) {
        search = new ai::Search::Graph( problem, frontier );
      } else {
        delete problem;
        delete frontier;
        throw cgl::cube::Exception( std::string( "usage: solve tree|graph astar|bfs|dfs|dl|ids|al|greedy|uc\ntree|graph not found" ) );
        return 0;
      }
      search->SetGenerationLimit( static_cast< int >( config.at( "generation_limit" ) ) );
      search->SetStoreLimit( static_cast< int >( config.at( "storage_limit" ) ) );

      return search;
    }

    void solve_save_cube( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, const std::map< std::string, double >& config, const cgl::cube::RubiksCube& goal_cube ) {
      ai::Search::Algorithm *search = solve_cube_init( cube, words, config, goal_cube );
      static int source_count = 0;
      source_count++;
      
      // Execute the search
      int steps = -1;
      if( search->Search() ) {
        std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
        std::list<ai::Search::Node *>::const_iterator it;

        // count total number of actions.
        steps = 0;
        for( it = solution->begin( ); it != solution->end( ); it++ ) {
          if( (*it)->GetAction( ) ) {
            steps += 1;
          }
        }

        // Record all intermediate configurations and their distance
        int count = 0;
        for( it = solution->begin( ); it != solution->end( ); it++ ) {
          if( (*it)->GetAction( ) ) {
            count += 1;
          }
          if( (*it)->GetState( ) ) {
            cgl::cube::State *state = dynamic_cast< cgl::cube::State * >( (*it)->GetState( ) );
            if( cgl::cube::RubiksCube::addKnownCube( state->getCube( ), steps - count ) ) {
              static int save_count = 0;
              save_count++;
              if( config.at( "known_cube_report_count" ) > 0 &&
                  save_count % (int)config.at( "known_cube_report_count" ) == 0 ) {
                std::cout << "Saved: " << state->getCube( ).oneLineDisplay( ) << " " << steps-count << " source_count: " << source_count << " save_count: " << save_count << std::endl;
              }
            }
          }
        }

      } else {
        steps = -1;
        std::cout << "No solution found for: " << cube.oneLineDisplay( ) << std::endl;
      }
      //std::cout << "Generated " << search->GetNumberNodesGenerated() << " nodes." << std::endl;
      //std::cout << "Maximum frontier size " << search->GetMaxNodesStored() << " nodes." << std::endl;
      
      delete search;
      search = 0;
    }


    void solve_cube( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, const std::map< std::string, double >& config, const cgl::cube::RubiksCube& goal_cube ) {
      int max_iterations = 1;
      int i;
      bool solved = false;
      if( words.size( ) >= 3 && words[ 2 ] == "ids" ) {
        max_iterations = static_cast< int >( config.at( "depth_limit" ) );
      }
      for( i = 1; i <= max_iterations && !solved; i++ ) {

        ai::Search::Algorithm *search = solve_cube_init( cube, words, config, goal_cube, i );

        std::cout << words[ 0 ] << " " << words[ 1 ] << " " << words[ 2 ] << std::endl;
      
        // Execute the search
        if( search->Search() ) {
          solved = true;
          std::cout << "Found solution." << std::endl;

          std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
          std::list<ai::Search::Node *>::const_iterator it;

          for( it = solution->begin( ); it != solution->end( ); it++ ) {
            if( (*it)->GetAction( ) ) {
              cgl::cube::Action *action = dynamic_cast< cgl::cube::Action * >( (*it)->GetAction( ) );
              cgl::cube::Color face = 127;
              cgl::cube::Direction direction = cgl::cube::Direction::NO_DIRECTION;
            
              std::cout << "rotate ";
          
              switch( action->getFace( ) ) {
              case cgl::cube::Action::FACE_WHITE: std::cout << "white"; face = 0; break;
              case cgl::cube::Action::FACE_GREEN: std::cout << "green"; face = 1; break;
              case cgl::cube::Action::FACE_RED: std::cout << "red"; face = 2; break;
              case cgl::cube::Action::FACE_BLUE: std::cout << "blue"; face = 3; break;
              case cgl::cube::Action::FACE_ORANGE: std::cout << "orange"; face = 4; break;
              case cgl::cube::Action::FACE_YELLOW: std::cout << "yellow"; face = 5; break;
              default: std::cout << "unknown";  break;
              }
          
              std::cout << " ";
          
              switch( action->getDirection( ) ) {
              case cgl::cube::Action::ROTATE_CW: std::cout << "cw"; direction = cgl::cube::Direction::CLOCK_WISE; break;
              case cgl::cube::Action::ROTATE_CCW: std::cout << "ccw"; direction = cgl::cube::Direction::COUNTER_CLOCK_WISE; break;
#if ALLOW_180
              case cgl::cube::Action::ROTATE_180: std::cout << "180"; direction = cgl::cube::Direction::ROTATE_180; break;
#endif
#if ALLOW_MIDDLE_ROTATE
              case cgl::cube::Action::ROTATE_MIDDLE_UP: std::cout << "up"; direction = cgl::cube::Direction::ROTATE_MIDDLE_UP; break;
              case cgl::cube::Action::ROTATE_MIDDLE_DOWN: std::cout << "down"; direction = cgl::cube::Direction::ROTATE_MIDDLE_DOWN; break;
              case cgl::cube::Action::ROTATE_MIDDLE_LEFT: std::cout << "left"; direction = cgl::cube::Direction::ROTATE_MIDDLE_LEFT; break;
              case cgl::cube::Action::ROTATE_MIDDLE_RIGHT: std::cout << "right"; direction = cgl::cube::Direction::ROTATE_MIDDLE_RIGHT; break;
#endif
              default: std::cout << "unknown";  break;
              }

              std::cout << std::endl;
              if( config.at( "apply_solution" ) > 0 ) {
                cube.rotate( face, direction );
              }
            }
          }
        } else {
          std::cout << "Could not find solution." << std::endl;
        }

        std::cout << "Generated " << search->GetNumberNodesGenerated() << " nodes." << std::endl;
        std::cout << "Maximum frontier size " << search->GetMaxNodesStored() << " nodes." << std::endl;
        std::cout << std::endl << std::flush;
        delete search;
        search = 0;
      }
    }

    void configure_search( std::map< std::string, double >& config, const std::vector< std::string >& words ) {
      if ( words.size( ) > 2 ) {
        if ( words[ 1 ] == "generation_limit" ) {
        } else if ( words[ 1 ] == "storage_limit" ) {
        } else if ( words[ 1 ] == "depth_limit" ) {
        } else if ( words[ 1 ] == "astar_limit" ) {
#if ALLOW_180
        } else if ( words[ 1 ] == "allow_180" ) {
#endif
#if ALLOW_MIDDLE_ROTATE
        } else if ( words[ 1 ] == "allow_middle_rotate" ) {
#endif
        } else if ( words[ 1 ] == "edge_heuristic" ) {
        } else if ( words[ 1 ] == "corner_heuristic" ) {
        } else if ( words[ 1 ] == "tile_heuristic" ) {
        } else if ( words[ 1 ] == "end_game_heuristic" ) {
        } else if ( words[ 1 ] == "edge_corner_heuristic" ) {
        } else if ( words[ 1 ] == "all_heuristic" ) {
        } else if ( words[ 1 ] == "zero_heuristic" ) {
        } else if ( words[ 1 ] == "known_cube_minimum" ) {
        } else if ( words[ 1 ] == "known_cube_report_count" ) {
        } else if ( words[ 1 ] == "apply_solution" ) {
        } else {
#if ALLOW_180          
          throw cgl::cube::Exception( std::string( "usage: config generation_limit|storage_limit|depth_limit|astar_limit|allow_180 number.\nparameter name (" ) + words[ 1 ] + ( ") not recognized." ) );
#else
          throw cgl::cube::Exception( std::string( "usage: config generation_limit|storage_limit|depth_limit|astar_limit number.\nparameter name (" ) + words[ 1 ] + ( ") not recognized." ) );
#endif
        }
        
        std::stringstream ss( words[ 2 ] );
        double number;
        ss >> number;
        
        config[ words[ 1 ] ] = number;

      } else {
#if ALLOW_180          
        throw cgl::cube::Exception( std::string( "usage: config generation_limit|storage_limit|depth_limit|astar_limit|allow_180|allow_middle_rotate number.\nNot enough parameters in command." ) );
#else
        throw cgl::cube::Exception( std::string( "usage: config generation_limit|storage_limit|depth_limit|astar_limit number.\nNot enough parameters in command." ) );
#endif
      }
    }


    /* generate all cubes within `depth` moves of cube */
    void generate_cubes_aux( cgl::cube::RubiksCube& cube, std::map< cgl::cube::RubiksCube, std::pair< int, int > >& cubes, int depth, std::map< std::string, double >& config ) {
      if( depth >= config.at( "max_depth" ) ) {
      } else {
        int face;
        int dir;
        cgl::cube::Direction min_dir = cgl::cube::CLOCK_WISE, max_dir = cgl::cube::COUNTER_CLOCK_WISE;
        
#if ALLOW_180          
        if( config.at( "allow_180" ) > 0 ) {
          max_dir = cgl::cube::ROTATE_180;
        }
#endif
#if ALLOW_MIDDLE_ROTATE
        std::cerr << "MIDDLE ROTATE NOT SUPPORTED YET." << std::endl;
#endif
        
        for( face = cgl::cube::WHITE; face <= cgl::cube::YELLOW; face ++ ) {
          for( dir = min_dir; dir <= max_dir; dir ++ ) {
            cube.rotate( static_cast< cgl::cube::ColorEnum >( face ), static_cast< cgl::cube::Direction >( dir ) );
            if( cubes.find( cube ) == cubes.end( ) ) {
              cubes[ cube ].first = 1;
              cubes[ cube ].second = depth+1;
            } else {
              cubes[ cube ].first += 1;
              if( cubes[ cube ].second > depth + 1 ) {
                cubes[ cube ].second = depth+1;
              }
            }
            generate_cubes_aux( cube, cubes, depth+1, config );
            cube.unrotate( static_cast< cgl::cube::ColorEnum >( face ), static_cast< cgl::cube::Direction >( dir ) );
          }
        }
      }
    }

    void generate_cubes( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, std::map< std::string, double >& config ) {
      if ( words.size( ) < 2 ) {
        throw cgl::cube::Exception( std::string( "Bad generate command: '" ) + words[ 0 ] + std::string( "'." ) );
      }
      std::stringstream ss( words[ 1 ] );
      int depth = 0;
      ss >> depth;
      config[ "max_depth" ] = depth;
      std::map< cgl::cube::RubiksCube, std::pair< int, int > > cubes;
      cubes[ cube ].first = 1;
      cubes[ cube ].second = 0;
      generate_cubes_aux( cube,  cubes, 0, config );

      std::map< cgl::cube::RubiksCube, std::pair< int, int > >::iterator it;
      for( it = cubes.begin( ); it != cubes.end( ); it++ ) {
        //it->first;
        //std::cout << it->first.oneLineDisplay( ) << " ";
        //std::cout << it->second.first << " " << it->second.second << std::endl;
        cgl::cube::RubiksCube::addKnownCube( it->first, it->second.second );
      }
      //std::cout << std::endl;
      cgl::cube::RubiksCube::saveKnownCubes( );
    }

    void dump_known_cubes( cgl::cube::RubiksCube& cube, const std::vector< std::string >& words, std::map< std::string, double >& /*config*/ ) {
      int do_label = 0;
      if ( words.size( ) < 2 ) {
        do_label = 0;
      } else {
        std::stringstream ss( words[ 1 ] );
        ss >> do_label;
      }

      std::cout << cube.csvHeader( );
      if( do_label ) {
        std::cout << "level";
      }
      std::cout << std::endl;

      const std::map< RubiksCube, double >& cubes = cube.getKnownCubes( );
      for( auto it = cubes.begin( ); it != cubes.end( ); it++ ) {
        std::cout << it->first.csvDisplay( );
        if( do_label ) {
          std::cout << it->second;
        }
        std::cout << std::endl;
      }
  
    }

    void show_config( const std::map< std::string, double >& config ) {
      std::cout << "Current Configuration:" << std::endl;
      for (const auto& pair : config) {
        std::cout << "  " << pair.first << ": " << pair.second << std::endl;
      }
      std::cout << std::endl;
    }

    void help_command() {
      std::cout << "Rubiks Cube Solver Help" << std::endl;
      std::cout << "=======================" << std::endl;
      std::cout << "Available commands:" << std::endl;
      std::cout << "  echo <text>                      - Display text" << std::endl;
      std::cout << "  initial_solved                   - Set cube to solved state" << std::endl;
      std::cout << "  initial_line <state>             - Set cube from one-line display" << std::endl;
      std::cout << "  initial <file>                   - Set cube from file" << std::endl;
      std::cout << "  show                             - Display current cube" << std::endl;
      std::cout << "  show_line                        - Display cube in one-line format" << std::endl;
      std::cout << "  goal_solved                      - Set goal cube to solved state" << std::endl;
      std::cout << "  goal_line <state>                - Set goal cube from one-line display" << std::endl;
      std::cout << "  show_goal_line                   - Display goal cube in one-line format" << std::endl;
      std::cout << "  goal <file>                      - Set goal cube from file" << std::endl;
      std::cout << "  rotate <face> <direction>        - Rotate a face (cw, ccw, 180)" << std::endl;
      std::cout << "  shuffle <count>                  - Shuffle cube with random rotations" << std::endl;
      std::cout << "  shuffle_range <min> <max>        - Shuffle with random count between min and max" << std::endl;
      std::cout << "  solve <tree|graph> <frontier>    - Solve the cube using search algorithm" << std::endl;
      std::cout << "  solve_save <tree|graph> <frontier> - Solve and save solution for later use" << std::endl;
      std::cout << "  hla_solve                        - Solve using HLA algorithm" << std::endl;
      std::cout << "  config <param> <value>           - Configure search parameters" << std::endl;
      std::cout << "  show_config                      - Display current configuration settings" << std::endl;
      std::cout << "  find <corner|edge>               - Find position of corner or edge piece" << std::endl;
      std::cout << "  isequal <cube>                   - Check if current cube equals given cube" << std::endl;
      std::cout << "  generate <depth>                 - Generate all cubes within given depth" << std::endl;
      std::cout << "  dump_known_cubes [<label>]       - Display known cubes" << std::endl;
      std::cout << "  save_known_cubes                 - Save known cubes to file" << std::endl;
      std::cout << "  repeat <count> <filename>        - Repeat commands from a file" << std::endl;
      std::cout << "  help                             - Display this help message" << std::endl;
      std::cout << "  quit/exit                        - Terminate the program" << std::endl;
      std::cout << std::endl;
      std::cout << "Supported faces: white, green, red, blue, orange, yellow" << std::endl;
      std::cout << "Supported directions: cw, ccw, 180 (if enabled), up, down, left, right (for middle slices, if enabled)" << std::endl;
      std::cout << "Supported frontiers: astar, bfs, dfs, dl, ids, al, greedy, uc" << std::endl;
      std::cout << std::endl;
    }

    void save_known_cubes( cgl::cube::RubiksCube& /*cube*/, const std::vector< std::string >& /*words*/, std::map< std::string, double >& /*config*/ ) {
      //std::cout << "Saving known cubes..." << std::flush;
      std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now( );
      cgl::cube::RubiksCube::saveKnownCubes( );
      std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now( );
      std::chrono::duration< double > time_span = 
        std::chrono::duration_cast< std::chrono::duration< double > >( t1 - t0 );
      //std::cout << "Saved in " << time_span.count( ) << " seconds." << std::endl;
    }
    
    void process_input_stream_aux( std::istream& is, AppData& data, bool show_prompt );

    void repeat_file( AppData& data ) {
      if ( data.words.size( ) < 3 ) {
        throw cgl::cube::Exception( std::string( "Bad repeat command: '" ) + data.words[ 0 ] + std::string( "'.  e.g. repeat 10 commands.txt" ) );
      }
      std::stringstream ss( data.words[ 1 ] );
      int count = 0;
      ss >> count;

      std::string filename;
      ss.clear( );
      ss.str( data.words[ 2 ] );
      ss >> filename;

      std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now( );
      int i;
      for( i = 0; i < count; i ++ ) {
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now( );
        std::ifstream fin( filename );
        if( fin ) {
          process_input_stream_aux( fin, data, false );
          fin.close( );
        } else {
          throw cgl::cube::Exception( std::string( "Bad repeat command file: '" ) + filename + std::string( "'." ) );
        }
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now( );
        std::chrono::duration< double > time_span = 
          std::chrono::duration_cast< std::chrono::duration< double > >( t2 - t1 );
        if( (i+1) % 100 == 0 ) {
          std::cout << "Iteration " << i << " took " << time_span.count( ) << " seconds." << std::endl;
          
          {
            std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now( );
            std::chrono::duration< double > time_span = 
              std::chrono::duration_cast< std::chrono::duration< double > >( t3 - t0 );
            std::cout << (i+1) << " of " << count << " iterations took " << time_span.count( ) << " seconds. " << "Average of " << ( time_span.count( ) / count ) << " seconds each." << std::endl;
          }
        }
      }
      std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now( );
      std::chrono::duration< double > time_span = 
        std::chrono::duration_cast< std::chrono::duration< double > >( t3 - t0 );
      std::cout << count << " iterations took " << time_span.count( ) << " seconds. " << "Average of " << ( time_span.count( ) / count ) << " seconds each." << std::endl;

    }

    bool prompt(bool show_prompt) {
      if ( show_prompt ) {
        std::cout << "rubiks> ";
        std::cout.flush();
      }
      return true;
    }

    void process_input_stream_aux( std::istream& is, AppData& data, bool show_prompt ) {
      
      while( prompt(show_prompt) && read_one_line( is, data.words ) ) {
    
        if ( data.words.size( ) == 0 ) {
          continue;
        } else if ( data.words[ 0 ].size( ) == 0 ) {
          continue;
        } else if ( data.words[ 0 ][ 0 ] == '#' ) {
          continue;
        } else if ( data.words[ 0 ] == "echo" ) {
          echo( data );
        } else if ( data.words[ 0 ] == "initial_line" ) { 
          initialize_line_cube( is, data );
        } else if ( data.words[ 0 ] == "initial_solved" ) { 
          initialize_solved_cube( is, data.cube );
        } else if ( data.words[ 0 ] == "goal_solved" ) { 
          initialize_solved_cube( is, data.goal_cube );
        } else if ( data.words[ 0 ] == "goal_line" ) { 
          initialize_line_goal_cube( is, data );
        } else if ( data.words[ 0 ] == "show_goal_line" ) { 
          show_cube_line( data.goal_cube );
        } else if ( data.words[ 0 ] == "hla_solve" ) { 
          hla_solve_cube( data );
        } else if ( data.words[ 0 ] == "solve_save" ) { 
          solve_save_cube( data.cube, data.words, data.config, data.goal_cube );
        } else if ( data.words[ 0 ] == "save_known_cubes" ) { 
          save_known_cubes( data.cube, data.words, data.config );
        } else if ( data.words[ 0 ] == "shuffle_range" ) { 
          shuffle_range_cube( data.cube, data.words, data.config );
        } else if ( data.words[ 0 ] == "repeat" ) { 
          repeat_file( data );
        } else if ( data.words[ 0 ] == "initial" ) { 
          initialize_cube( is, data.cube );
        } else if ( data.words[ 0 ] == "goal" ) { 
          initialize_cube( is, data.goal_cube );
        } else if ( data.words[ 0 ] == "show" ) { 
          show_cube( data.cube );
        } else if ( data.words[ 0 ] == "show_line" ) { 
          show_cube_line( data.cube );
        } else if ( data.words[ 0 ] == "rotate" ) { 
          rotate_cube( data.cube, data.words );
        } else if ( data.words[ 0 ] == "shuffle" ) { 
          shuffle_cube( data.cube, data.words, data.config );
        } else if ( data.words[ 0 ] == "solve" ) { 
          solve_cube( data.cube, data.words, data.config, data.goal_cube );
        } else if ( data.words[ 0 ] == "config" ) { 
          configure_search( data.config, data.words );
        } else if ( data.words[ 0 ] == "find" ) { 
          find( data.cube, data.words );
        } else if ( data.words[ 0 ] == "isequal" ) { 
          is_equal_cube( is, data.cube );
        } else if ( data.words[ 0 ] == "generate" ) { 
          generate_cubes( data.cube, data.words, data.config );
        } else if ( data.words[ 0 ] == "dump_known_cubes" ) { 
          dump_known_cubes( data.cube, data.words, data.config );
        } else if ( data.words[ 0 ] == "show_config" ) { 
          show_config( data.config );
        } else if ( data.words[ 0 ] == "help" ) { 
          help_command();
        } else if ( data.words[ 0 ] == "quit" || data.words[ 0 ] == "exit" ) { 
          return;
        } else { 
          throw cgl::cube::Exception( std::string( "Unexpected command: '" ) + data.words[ 0 ] + std::string( "'." ) );
        }
      }
    }

    void process_input_stream( std::istream& is, bool show_prompt ) {
      /* srand seeds at 100 usecond resolution */
      struct timeval time; 
      gettimeofday(&time,NULL);
      srand((time.tv_sec * 10000) + (time.tv_usec / 100));
      //std::srand( std::time( 0 ) );
      
      //std::cout << "Loading known cubes." << std::flush;
      std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now( );
      AppData data;
      std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now( );
      std::chrono::duration< double > time_span = 
        std::chrono::duration_cast< std::chrono::duration< double > >( t1 - t0 );
      //std::cout << "Loaded in " << time_span.count( ) << " seconds." << std::endl;

      /* Initialize configuration parameters */
      std::map< std::string, double >& config = data.config;
      config[ "generation_limit" ] = 0;
      config[ "storage_limit" ] = 0;
      config[ "depth_limit" ] = 0;
      config[ "astar_limit" ] = 0;
#if ALLOW_180          
      config[ "allow_180" ] = 0;
      //config[ "allow_180" ] = 1;
#endif
#if ALLOW_MIDDLE_ROTATE
      config[ "allow_middle_rotate" ] = 0;
      //config[ "allow_middle_rotate" ] = 1;
#endif
      config[ "edge_heuristic" ] = 0;
      config[ "corner_heuristic" ] = 0;
      config[ "tile_heuristic" ] = 0;
      config[ "end_game_heuristic" ] = 0;
      config[ "edge_corner_heuristic" ] = 0;
      config[ "all_heuristic" ] = 0;
      config[ "zero_heuristic" ] = 0;
      config[ "known_cube_minimum" ] = 0.0;
      config[ "known_cube_report_count" ] = 0.0;
      config[ "apply_solution" ] = 0;

      process_input_stream_aux( is, data, show_prompt );
    }
  }
}
