#ifndef _CGLRUBIKSAPPDATA_H_
#define _CGLRUBIKSAPPDATA_H_

#include "cglRubiksAppConfig.h"
#include "cglRubiksMoveSet.h"
#include "cglRubiksCubeSet.h"
#include "cglRubiksPlanAction.h"
#include "cglRubiksCube.h"
#include "cglRubiksCommand.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace cgl {
  namespace rubiks {

    struct AppData {
      AppData( std::istream &is_in, std::ostream &os_in );
      int setLegalMoves( );
      AppConfig config;
      Cube cube;
      std::vector< Cube > goal_cubes;
      std::istream &is;
      std::ostream &os;
      std::vector< std::string > words;  // for line-by-line processing
      std::map< std::string, ActionRecord > actions;
      MoveSet move_set;
      CubeSet cube_set;
      bool    done;
      PlanActionSet plan_action_set;
      //
      bool        solution_valid;
      std::string solution;
    };

  }
}

#endif /* _CGLRUBIKSAPPDATA_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
