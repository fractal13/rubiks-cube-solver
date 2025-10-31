#ifndef _SEARCH_PROBLEM_H_
#define _SEARCH_PROBLEM_H_
#include <vector>
namespace ai
{
  namespace Search
  {
    /**
     * Abstract base class to enforce all required operations
     * for a search problem.
     */
    class Problem
    {
    public:
      /* The input parameter, initial_state_in, must be allocated
       * with `new`.  It will be `delete`d automatically in the
       * destrector of this class.  Do not use it or delete it
       * elsewhere.
       */
      Problem(State *initial_state_in);

      /* Deletes initial_state.
       */
      virtual ~Problem();

      /* Retrieves initial_state
       */
      State *GetInitialState();

      /* Text display of the state, for "print debugging".
       */
      virtual void   Display() const;

      /* Legal and desired actions to consider given state_in.
       * Each of the `Action *` objects must have been allocated
       * with `new`.  They will be deleted elsewhere.
       */
      virtual std::vector<Action *> Actions(const State * const state_in) = 0;

      /* State that results if in state_in, action_in is taken.
       * The `State *` object must have been allocated with `new`.
       * It will be deleted elsewhere.
       */
      virtual State *Result(const State * const state_in, const Action * const action_in) = 0;

      /* True if state_in is a goal state.  False otherwise.
       */
      virtual bool   GoalTest(const State * const state_in) const = 0;

      /* Calculate path cost for the taking action_in in state1_in,
       * producing state2_in.
       */
      virtual double StepCost(const State  * const state1_in,
                              const Action * const action_in,
                              const State  * const state2_in) const = 0;

      /* Estimated cost from state_in to a goal state.
       * Defaults to 0, which is admissible, but useless.
       * Used for Greedy and A* searches.
       */
      virtual double Heuristic(const State  * const state_in) const;

    protected:
      /* Set from the constructor, and deleted from destructor, as documented above.
       */
      State             *initial_state;

    private:
    };
  }
}

#endif /* _PROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
