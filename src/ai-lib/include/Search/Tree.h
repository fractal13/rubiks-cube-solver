#ifndef _SEARCH_TREE_H_
#define _SEARCH_TREE_H_

namespace ai
{
  namespace Search
  {
    /**
     * Search tree algorithm.
     * See Algorithm.h
     */
    class Tree : public Algorithm
    {
    public:
      Tree(Problem *problem_in, Frontier *frontier_in);
      virtual ~Tree();

      /* used to search for one solution */
      virtual bool Search();

      /* used to search for more than one solution
       *
       * tree->SearchInit();
       * while(tree->SearchNext())
       * {
       *   // get solution
       * }
       */
      virtual bool SearchInit();
      virtual bool SearchNext();

    protected:
      
      virtual void deleteNodeIfNeeded( Node *node );
      
    private:
    };
  }
}

#endif /* _TREE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
