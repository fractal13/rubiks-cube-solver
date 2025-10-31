#ifndef _SEARCH_STATE_H_
#define _SEARCH_STATE_H_
#include <string>

namespace ai
{
  namespace Search
  {
    /**
     * Abstract base class to enforce required operations for
     * a state object in a search.
     */
    class State
    {
    public:

      /* virtual to insure destructor chaining */
      virtual ~State();

      /* Used for text display of state information
       * to std::cout */
      virtual void Display() const;

      /* Used for equality testing. Must use dynamic_cast
       * to convert state_in to the correct pointer type.
       * True if the current object and state_in are the same value.
       */
      virtual bool IsEqual(const State * const state_in) const = 0;

      /* Used for ordered storing. Must use dynamic_cast
       * to convert state_in to the correct pointer type.
       * True if current object is less than state_in.
       */
      virtual bool IsLessThan(const State * const state_in) const = 0;

    protected:
      /* Choose data values appropriate for your problem.
       */

    private:
    };
  }
}
#endif /* _STATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
