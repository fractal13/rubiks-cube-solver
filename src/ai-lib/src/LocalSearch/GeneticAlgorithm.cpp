#include <cmath>
#include <cstdlib>
#include <vector>
#include "ai_local_search.h"

namespace ai {
  namespace LocalSearch  {
    
    GeneticAlgorithm::Individual::Individual( State *state_in, double value_in ) 
      : state( state_in ), value( value_in ) {
    }
      
    GeneticAlgorithm::Individual::Individual( State *state_in ) 
      : state( state_in ), value( 0.0 ) {
      CalculateValue( );
    }

    void GeneticAlgorithm::Individual::Delete( ) {
      if( state ) {
        delete state;
        state = 0;
      }
    }

    void GeneticAlgorithm::Individual::CalculateValue( ) {
      value = state->Value( );
    }
    
    bool GeneticAlgorithm::Individual::operator<( const Individual& rhs ) const {
      return value < rhs.value;
    }

    void GeneticAlgorithm::Individual::Mutate( ) {
    }

    GeneticAlgorithm::GeneticAlgorithm( Problem *problem_in )
      : Algorithm(problem_in), mPopulationSize( 20 ), mMutateProbability( 0.10 ) {
      // Empty
    }
    
    GeneticAlgorithm::~GeneticAlgorithm() {
      // Empty
    }
    
    bool GeneticAlgorithm::Search() {
      std::vector< Individual > population;
      int i;
      for( i = 0; i < mPopulationSize; i++ ) {
        population.push_back( Individual( this->problem->RandomState( ) ) );
        this->states_generated++;
      }
      this->solution = population[ 0 ].state;

      while( true  &&
             ( generation_limit == 0 || generation_limit > this->states_generated ) ) {
        std::vector< Individual > new_population;
        for( i = 0; i < mPopulationSize; i++ ) {
          const Individual& x = RandomSelection( population );
          const Individual& y = RandomSelection( population );
          Individual z = Reproduce( x, y );
          this->states_generated++;
          double r = RandomNumber( );
          if( r < mMutateProbability ) {
            z.Mutate( );
          }
          new_population.push_back( z );
        }
        for( auto it = population.begin( ); it != population.end( ); it++ ) {
          if( it->state->Value( ) > this->solution->Value( ) ) {
            delete this->solution;
            this->solution = it->state;
            it->state = 0;
          }
          it->Delete( );
        }
        population.clear( );
        population = new_population;
        new_population.clear( );
      }

      for( auto it = population.begin( ); it != population.end( ); it++ ) {
        if( it->state->Value( ) > this->solution->Value( ) ) {
          delete this->solution;
          this->solution = it->state;
          it->state = 0;
        }
        it->Delete( );
      }

      return true;
    }

    const GeneticAlgorithm::Individual& GeneticAlgorithm::RandomSelection( const std::vector< Individual >& population_in) const {
      return population_in[ std::rand( ) % population_in.size( ) ];
    }

    GeneticAlgorithm::Individual GeneticAlgorithm::Reproduce( const Individual& x, const Individual& y ) const {
      State *state = this->problem->ChildState( x.state, y.state, RandomNumber( ) );
      return Individual( state );
    }
    
    double GeneticAlgorithm::RandomNumber( ) const {
      return 0;
    }

    bool GeneticAlgorithm::SetPopulationSize( int population_size_in ) {
      this->mPopulationSize = population_size_in;
      return true;
    }
    
    bool GeneticAlgorithm::SetMutateProbability( double mutate_probability_in ) {
      this->mMutateProbability = mutate_probability_in;
      return true;
    }
    
  }
}
