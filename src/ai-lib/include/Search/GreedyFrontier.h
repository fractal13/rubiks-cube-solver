#ifndef _GREEDYFRONTIER_H_
#define _GREEDYFRONTIER_H_

#include <queue>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements an Greedy-Best-First frontier using STL's priority_queue
     * Requires Problem::Heuristic() to work correctly.
     */
    class GreedyFrontier : public Frontier
    {
    public:
      virtual ~GreedyFrontier();
      virtual bool Insert(Node *node_in);
      virtual Node *Remove();
      virtual bool Empty() const;
      virtual size_t Size() const;

    protected:

      struct NodePtr
      {
        NodePtr(Node *node_in);
        bool operator<(const NodePtr &rhs) const;
        Node *ptr;
      };

      std::priority_queue<NodePtr> frontier;
    private:
    };

  }
}

#endif /* _GREEDYFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
