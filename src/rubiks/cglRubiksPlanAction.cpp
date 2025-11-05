#include "cglRubiksPlanAction.h"
#include "cglRubiksCommand.h"

namespace cgl {
  namespace rubiks {
    
    /************************************************************************
     * PlanAction
     */

    PlanAction::PlanAction( ) 
      : mName( "default" ) {
      // empty
    }

    PlanAction::PlanAction( const std::string& name ) 
      : mName( name ) {
      // empty
    }

    PlanAction::PlanAction( const std::string& name, const std::vector< Cube >& preconditions, const Move& effect ) 
      : mName( name ), mPreconditions( preconditions ), mEffect( effect ) {
      // empty
    }

    void PlanAction::clearPreconditions( ) {
      mPreconditions.clear( );
    }

    void PlanAction::setPreconditions( const std::vector< Cube >& preconditions ) {
      mPreconditions = preconditions;
    }

    void PlanAction::addPrecondition( const Cube& precondition ) {
      mPreconditions.push_back( precondition );
    }

    void PlanAction::setEffect( const Move& effect ) {
      mEffect = effect;
    }
      
    const std::string& PlanAction::getName( ) const {
      return mName;
    }

    const std::vector< Cube >& PlanAction::getPreconditions( ) const {
      return mPreconditions;
    }

    const Move& PlanAction::getEffect( ) const {
      return mEffect;
    }
    
    bool PlanAction::conditionsAreMet( const Cube& cube ) const {
      bool eq = true;
      for( auto it = mPreconditions.begin( ); it != mPreconditions.end( ); it++ ) {
        eq = eq && ( cube == *it );
      }
      return eq;
    }

    /*
     * PlanAction
     ************************************************************************/

    /************************************************************************
     * PlanActionSet
     */

    PlanActionSet::PlanActionSet( ) {
      // empty
    }

    void PlanActionSet::clearDefinedActions( ) {
      mActions.clear( );
    }

    int PlanActionSet::defineAction( const std::string& action_name, const PlanAction& action ) {
      if( actionIsDefined( action_name ) ) {
        return EXIT_USAGE;
      }
      mActions[ action_name ] = action;
      return EXIT_OK;
    }

    const std::map< std::string, PlanAction >& PlanActionSet::getDefinedActions( ) const {
      return mActions;
    }

    bool PlanActionSet::actionIsDefined( const std::string& action_name ) const {
      return mActions.count( action_name ) > 0;
    }

    const PlanAction& PlanActionSet::getDefinedAction( const std::string& action_name ) const {
      return mActions.at( action_name );
    }

    /*
     * PlanActionSet
     ************************************************************************/




  }
}
