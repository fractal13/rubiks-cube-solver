#ifndef _SEARCH_ALGORITHM_H_
#define _SEARCH_ALGORITHM_H_

namespace ai
{
  namespace Search
  {
    /**
     * Abstract base class to represent the search algorithm.
     */
    class Algorithm
    {
    public:
      enum AlgorithmType
        {
          T_NULL,
          T_Tree,
          T_Graph,
          T_MAX
        };

      /* problem_in and frontier_in must be allocated with `new`,
       * and will be deleted by the Algorithm destructor.
       */
      Algorithm(Problem *problem_in, Frontier *frontier_in);

      /* `delete`s problem and frontier.
       */
      virtual ~Algorithm();

      /* The search algorithm implementation
       */
      virtual bool Search() = 0;

      /* Fetch the solution path if Search was successful.
       */
      Solution &GetSolution();

      /* Fetch internal statistics on storage and runtime usage.
       */
      size_t GetNumberNodesGenerated() const;
      size_t GetMaxNodesStored() const;

      /* Set limits to stop run-away searches.
       */
      bool SetGenerationLimit(size_t limit);
      bool SetStoreLimit(size_t limit);

    protected:
      /* Search instance specific data.
       */
      Problem  *problem;
      Frontier *frontier;
      Solution  solution;

      /* Root of the search tree.  (The node with the initial state.)
       */
      Node *root;

      /* Search stats.
       */
      size_t number_nodes_generated;
      size_t max_nodes_stored;

      /* Search limits.
       */
      size_t generation_limit;
      size_t store_limit;
    private:
    };
  }
}

#endif /* _SEARCH_ALGORITHM_H_ */

/* Local Variables: */
/* mode:c++         */
/* End:             */
