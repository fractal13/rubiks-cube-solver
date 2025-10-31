#ifndef _SIMULATEDANNEALING_H_
#define _SIMULATEDANNEALING_H_
#include "LocalSearch/Algorithm.h"

namespace ai
{
  namespace LocalSearch
  {
    /* Temperature starts at start_temperature.
     * The algorithm continues searching until
     * temperature <= min_temperature.
     *
     * The temperature changes each time a neighbor is evaluated by:
     * temperature = temperature - temperature / temperature_reduction
     *
     * If a neighbor is better than the current state, it is selected.
     * If a neighbor is worse than the current state, the probability
     * of the neighbor being selected is:
     * probability = exp((neighbor_value - current_value) / temperature)
     *
     * The resolution of the random number selection is controlled by resolution:
     * random = (rand() % resolution) / resolution;
     *
     * Default values:
     * start_temperature     =    30.0;
     * min_temperature       =     0.00001;
     * temperature_reduction =   100.0;
     * resolution            = 10000.0;
     *
     */
    class SimulatedAnnealing : public Algorithm
    {
    public:
      SimulatedAnnealing(Problem *problem_in);
      virtual ~SimulatedAnnealing();
      virtual bool Search();
      bool SetStartTemperature(double start_temperature_in);
      bool SetMinimumTemperature(double min_temperature_in);
      bool SetTemperatureReduction(double temperature_reduction_in);
      bool SetResolution(double resolution_in);
    protected:
      double start_temperature;
      double min_temperature;
      double temperature_reduction;
      double resolution;
    private:
    };
  }
}

#endif /* _SIMULATEDANNEALING_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
