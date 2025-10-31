#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    /*
     * ACTION STATE PAIR CLASS
     */
    ActionStatePair::ActionStatePair()
    {
      state  = 0;
      action = 0;
    }

    ActionStatePair::ActionStatePair(State *state_in, Action *action_in)
    {
      state  = state_in;
      action = action_in;
    }
    ActionStatePair::~ActionStatePair()
    {
    }

    void   ActionStatePair::Display() const
    {
      if(action)
        {
          action->Display();
        }
      if(state)
        {
          state->Display();
        }
    }


    State  *ActionStatePair::GetState() const
    {
      return state;
    }

    Action *ActionStatePair::GetAction() const
    {
      return action;
    }

  }
}
