#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "LocalSearch/State.h"

namespace ai
{
  namespace LocalSearch
  {
    class Problem
    {
    public:
      /* virtual to allow destructor chaining */
      virtual ~Problem();

      /* evaluates state_in to see if random restart
       * should stop.
       * Note that a state may be good enough because
       * it has reached some critical value, or because
       * enough restarts have occurred.
       *
       * returns true if restart should stop,
       * returns false if restart should continue.
       */
      virtual bool GoodEnough(const State * const state_in) = 0;

      /* Allocate a random state from the legal state space.
       * The returned state must be 'delete'd by the
       * calling code. This is typically the local search Algorithm.
       */
      virtual State *RandomState() const = 0;

      /* Select the neighbor of state_in with the
       * highest State::Value().  Allocate the
       * state and return it.
       * The returned state must be 'delete'd by the
       * calling code. This is typically the local search Algorithm.
       *
       * neighbors_generated must be set to the number of states generated,
       * including the state returned.  This contributes to the accounting system
       * for total number of nodes generated.
       */
      virtual State *BestNeighbor(const State * const state_in, int& neighbors_generated ) const = 0;

      /* Allocate a neighbor of state_in and return it.
       * The returned state must be 'delete'd by the
       * calling code. This is typically the local search Algorithm.
       */
      virtual State *RandomNeighbor(const State * const state_in) const = 0;

      /* Allocate a child state from mixing the 
       * The returned state must be 'delete'd by the
       * calling code. This is typically the local search Algorithm.
       */
      virtual State *ChildState(const State * const state1_in, const State * const state2_in, double mix_point) const = 0;
      
    protected:
    private:
    };
  }
}

#endif /* _PROBLEM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
