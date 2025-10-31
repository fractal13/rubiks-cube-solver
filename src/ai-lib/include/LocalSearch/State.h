#ifndef _STATE_H_
#define _STATE_H_
#include <iostream>
namespace ai
{
  namespace LocalSearch
  {
    class State
    {
    public:
      /* virtual to allow destructor chaining */
      virtual ~State();

      /*
       * Calculates value of the state
       * Higher values represent states that are closer
       * to the goal state.
       */
      virtual double Value() const = 0;

      /* Text display of state */
      virtual std::ostream &Display(std::ostream &os) const;

    protected:
    private:
    };
  }
}

#endif /* _STATE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
