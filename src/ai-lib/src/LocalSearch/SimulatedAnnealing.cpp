#include <cmath>
#include <cstdlib>
#include "ai_local_search.h"

namespace ai
{
  namespace LocalSearch
  {
    SimulatedAnnealing::SimulatedAnnealing(Problem *problem_in)
      : Algorithm(problem_in)
    {
      start_temperature     =    30.0;
      min_temperature       =     0.00001;
      temperature_reduction =   100.0;
      resolution            = 10000.0;
    }
    SimulatedAnnealing::~SimulatedAnnealing()
    {
    }
    bool SimulatedAnnealing::Search()
    {
      State *state = this->problem->RandomState();
      this->states_generated++;
      double current_value  = state->Value();

      State *random_state = this->problem->RandomNeighbor(state);
      this->states_generated++;
      double random_value = random_state->Value();

      double temperature = this->start_temperature;

      while(temperature > this->min_temperature &&
            (generation_limit == 0 || generation_limit > this->states_generated))
        {
          double p = random_value - current_value;
          p = std::exp(p/temperature);
          double r = ((double)(std::rand() % ((int)this->resolution))) / this->resolution;

          /* If this is a better state, OR
           * the random roll allows travel to worse state */
          if((random_value > current_value) ||
             (r < p))
            {
              delete state;
              state = random_state;
              current_value = random_value;
            }
          else
            {
              delete random_state;
            }

          temperature -= temperature/this->temperature_reduction;

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

    bool SimulatedAnnealing::SetStartTemperature(double start_temperature_in)
    {
      this->start_temperature = start_temperature_in;
      return true;
    }
    bool SimulatedAnnealing::SetMinimumTemperature(double min_temperature_in)
    {
      this->min_temperature = min_temperature_in;
      return true;
    }
    bool SimulatedAnnealing::SetTemperatureReduction(double temperature_reduction_in)
    {
      this->temperature_reduction = temperature_reduction_in;
      return true;
    }
    bool SimulatedAnnealing::SetResolution(double resolution_in)
    {
      this->resolution = resolution_in;
      return true;
    }
  }
}
