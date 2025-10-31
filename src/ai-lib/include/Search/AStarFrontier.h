#ifndef _ASTARFRONTIER_H_
#define _ASTARFRONTIER_H_

#include <queue>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements an A* frontier using STL's priority_queue.
     * Requires Problem::Heuristic() and Problem::StepCost() to work correctly.
     */
    class AStarFrontier : public Frontier
    {
    public:
      virtual ~AStarFrontier();
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

#endif /* _ASTARFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
