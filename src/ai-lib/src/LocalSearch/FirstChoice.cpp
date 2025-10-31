#include "ai_local_search.h"

namespace ai
{
  namespace LocalSearch
  {
    FirstChoice::FirstChoice(Problem *problem_in)
      : Algorithm(problem_in)
    {
    }
    FirstChoice::~FirstChoice()
    {
    }
    bool FirstChoice::Search()
    {
      State *state = this->problem->RandomState();
      this->states_generated++;
      double current_value  = state->Value();

      State *random_state = this->problem->RandomNeighbor(state);
      this->states_generated++;
      double random_value = random_state->Value();

      while(random_value > current_value &&
            (generation_limit == 0 || generation_limit > this->states_generated))
        {
          delete state;
          state = random_state;
          current_value = random_value;

          random_state = this->problem->RandomNeighbor(state);
          this->states_generated++;
          random_value = random_state->Value();
        }
      if(!this->solution)
        {
          this->solution = state;
        }
      else
        {
          if(this->solution->Value() < state->Value())
            {
              delete this->solution;
              this->solution = state;
            }
          else
            {
              delete state;
            }
        }
      delete random_state;
      return true;
    }
  }
}
