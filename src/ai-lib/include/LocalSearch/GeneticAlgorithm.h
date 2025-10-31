#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_
#include "LocalSearch/Algorithm.h"
#include <vector>
namespace ai {
  namespace LocalSearch {
    /* 
     *
     */
    class GeneticAlgorithm : public Algorithm {
    public:

      class Individual {
      public:
        Individual( State *state_in, double value_in );
        Individual( State *state_in );
        void Delete( );
        void CalculateValue( );
        bool operator<( const Individual& rhs ) const;
        void Mutate( );
        
        State *state;
        double value;
      };

      GeneticAlgorithm( Problem *problem_in );
      virtual ~GeneticAlgorithm();
      virtual bool Search();
      const Individual& RandomSelection( const std::vector< Individual >& population_in) const;
      Individual Reproduce( const Individual& x, const Individual& y ) const;
      double RandomNumber( ) const;
      bool SetPopulationSize( int population_size_in );
      bool SetMutateProbability( double mutate_probability_in );
    protected:
      int mPopulationSize;
      double mMutateProbability;
    private:
    };
  }
}

#endif /* _GENETICALGORITHM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
