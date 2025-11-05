#ifndef _CGLRUBIKSAPPCONFIG_H_
#define _CGLRUBIKSAPPCONFIG_H_

#include <string>
#include <iostream>
#include <map>

namespace cgl {
  namespace rubiks {

    class AppConfig {
    public:
      AppConfig( );
      bool updateOption( const std::string& option, const std::string& value );
      std::string listOptions( ) const;
      void showConfiguration( std::ostream& os ) const;
      
      // show a prompt for the user
      void enablePrompt( );
      void disablePrompt( );
      bool prompt( ) const;

      // allow 180 degree turns as part of the legal moves
      void enableHalfTurns( );
      void disableHalfTurns( );
      bool halfTurns( ) const;

      // allow slice turns as part of the legal moves
      void enableSliceTurns( );
      void disableSliceTurns( );
      bool sliceTurns( ) const;

      // how to display cubes
      void displayCubeAsT( );
      void displayCubeAsOneWord( );
      void displayCubeAsOneLine( );
      bool cubeDisplayT( ) const;
      bool cubeDisplayOneWord( ) const;
      bool cubeDisplayOneLine( ) const;

      // how to display moves
      void displayMoveAsName( );
      void displayMoveAsSubMoves( );
      bool moveDisplayAsName( ) const;
      bool moveDisplayAsSubMoves( ) const;

      // search options
      int depthLimit( ) const;
      void setDepthLimit( const int limit );
      int idsDepth( ) const;
      void setIDSDepth( const int depth );
      int idsLimit( ) const;
      void setIDSLimit( const int limit );
      double aStarLimit( ) const;
      void setAStarLimit( const double limit );
      double generationLimit( ) const;
      void setGenerationLimit( const double limit );
      double storageLimit( ) const;
      void setStorageLimit( const double limit );

      void enableApplySolution( );
      void disableApplySolution( );
      bool applySolution( ) const;

      // heuristic options
      void setZeroHeuristic( );
      void setFaceletHeuristic( );
      void setEdgeFaceletHeuristic( );
      void setCornerFaceletHeuristic( );
      void setFaceletMaxHeuristic( );
      void setColorCountHeuristic( );
      int heuristic( ) const;

      // how to display search results
      void setDisplaySolutionHuman( );
      void setDisplaySolutionMachine( );
      bool displaySolutionHuman( ) const;
      bool displaySolutionMachine( ) const;

      // auto transform starting cube's *s to xs
      void enableInitialCubeAutoRemoveWildcard( );
      void disableInitialCubeAutoRemoveWildcard( );
      bool initialCubeAutoRemoveWildcard( ) const;
      
      // compile plan action preconditions
      void enableCompilePreconditions( );
      void disableCompilePreconditions( );
      bool compilePreconditions( ) const;
      
    protected:
      std::map< std::string, double > mOptions;

    private:
    };

  }
}


#endif /* _CGLRUBIKSAPPCONFIG_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
