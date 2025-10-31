#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    Algorithm::Algorithm(Problem *problem_in, Frontier *frontier_in)
    {
      problem = problem_in;
      frontier  = frontier_in;
      root    = 0;

      number_nodes_generated = 0;
      max_nodes_stored       = 0;
      generation_limit       = 0;
      store_limit            = 0;
    }

    Algorithm::~Algorithm()
    {
      solution.Clear();
      if(root)
        {
          delete root;
          root = 0;
        }
      if(problem)
        {
          delete problem;
          problem = 0;
        }
      if(frontier)
        {
          delete frontier;
          frontier = 0;
        }
    }

    Solution &Algorithm::GetSolution()
    {
      return solution;
    }

    size_t Algorithm::GetNumberNodesGenerated() const
    {
      return number_nodes_generated;
    }

    size_t Algorithm::GetMaxNodesStored() const
    {
      return max_nodes_stored;
    }

    bool Algorithm::SetGenerationLimit(size_t limit)
    {
      generation_limit = limit;
      return true;
    }

    bool Algorithm::SetStoreLimit(size_t limit)
    {
      store_limit = limit;
      return true;
    }

  }
}
