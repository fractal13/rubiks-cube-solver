#include "ai_local_search.h"

namespace ai
{
  namespace LocalSearch
  {
    Algorithm::Algorithm(Problem *problem_in)
      : problem(problem_in),
        solution(0),
        restarts(0),
        states_generated(0),
        generation_limit(0)
    {
    }
    Algorithm::~Algorithm()
    {
      if(this->problem)
        {
          delete this->problem;
          this->problem = 0;
        }
      if(this->solution)
        {
          delete this->solution;
          this->solution = 0;
        }
    }

    Algorithm *Algorithm::MakeAlgorithm(AlgorithmType algorithm_type, Problem *the_problem)
    {
      Algorithm *the_algorithm = 0;
      switch(algorithm_type)
        {
        case T_HillClimbing:
          the_algorithm = new HillClimbing(the_problem);
          break;
        case T_FirstChoice:
          the_algorithm = new FirstChoice(the_problem);
          break;
        case T_SimulatedAnnealing:
          the_algorithm = new SimulatedAnnealing(the_problem);
          break;
        default:
          /* Error */
          break;
        }
      return the_algorithm;
    }

    bool Algorithm::RandomRestartSearch(int max_restarts)
    {
      bool done = false;
      this->restarts = 0;
      while(!done && 
            (max_restarts <= 0 || this->restarts < max_restarts)  &&
            (generation_limit == 0 || generation_limit > this->states_generated))
        {
          this->Search();
          done = this->problem->GoodEnough(this->solution);
          this->restarts ++;
        }
      return true;
    }
    const State * const Algorithm::GetSolution() const
    {
      return this->solution;
    }
    int Algorithm::GetRestartCount() const
    {
      return this->restarts;
    }
    int Algorithm::GetStatesGenerated() const
    {
      return this->states_generated;
    }
    bool Algorithm::SetGenerationLimit(int limit)
    {
      generation_limit = limit;
      return true;
    }

  }
}
