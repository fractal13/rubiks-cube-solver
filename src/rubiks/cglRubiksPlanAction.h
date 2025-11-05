#ifndef _CGLRUBIKSPLANACTION_H_
#define _CGLRUBIKSPLANACTION_H_

#include "cglRubiksCube.h"
#include <string>
#include <vector>
#include <map>

namespace cgl {
  namespace rubiks {

    /************************************************************************
     * PlanAction
     */

    class PlanAction {
    public:
      PlanAction( );
      PlanAction( const std::string& name );
      PlanAction( const std::string& name, const std::vector< Cube >& preconditions, const Move& effect );
      void clearPreconditions( );
      void setPreconditions( const std::vector< Cube >& preconditions );
      void addPrecondition( const Cube& precondition );
      void setEffect( const Move& effect );
      
      const std::string& getName( ) const;
      const std::vector< Cube >& getPreconditions( ) const;
      const Move& getEffect( ) const;

      bool conditionsAreMet( const Cube& cube ) const;

    protected:
      std::string         mName;
      std::vector< Cube > mPreconditions;
      Move                mEffect;

    private:
    };

    /*
     * PlanAction
     ************************************************************************/

    /************************************************************************
     * PlanActionSet
     */

    class PlanActionSet {
    public:
      PlanActionSet( );
      void clearDefinedActions( );
      int defineAction( const std::string& action_name, const PlanAction& action );
      const std::map< std::string, PlanAction >& getDefinedActions( ) const;
      bool actionIsDefined( const std::string& action_name ) const;
      const PlanAction& getDefinedAction( const std::string& action_name ) const;
      
    protected:
      std::map< std::string, PlanAction > mActions;

    private:
      
    };

    /*
     * PlanActionSet
     ************************************************************************/

  }
}


#endif /* _CGLRUBIKSPLANACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
