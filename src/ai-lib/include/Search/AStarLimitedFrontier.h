#ifndef _ASTARLIMITEDFRONTIER_H_
#define _ASTARLIMITEDFRONTIER_H_

#include <stack>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements a astar-cost-limited frontier using STL's stack
     */
    class AStarLimitedFrontier : public Frontier
    {
    public:
      AStarLimitedFrontier(double max_cost_in);
      virtual ~AStarLimitedFrontier();
      virtual bool Insert(Node *node_in);
      virtual Node *Remove();
      virtual bool Empty() const;
      virtual size_t Size() const;
    protected:
      std::stack<Node *> frontier;
    private:
      double max_cost;
    };
  }
}

#endif /* _ASTARLIMITEDFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
