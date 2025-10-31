#ifndef _UCFRONTIER_H_
#define _UCFRONTIER_H_

#include <queue>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements a uniform-cost frontier using STL's vector and sort
     * Requires Problem::StepCost() to work correctly.
     */
    class UCFrontier : public Frontier
    {
    public:
      virtual ~UCFrontier();
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
#endif /* _UCFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
