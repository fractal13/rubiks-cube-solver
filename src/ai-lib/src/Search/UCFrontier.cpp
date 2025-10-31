#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    UCFrontier::~UCFrontier()
    {
    }

    bool UCFrontier::Insert(Node *node_in)
    {
      NodePtr np(node_in);
      frontier.push(np);
      return true;
    }

    Node *UCFrontier::Remove()
    {
      Node *tmp = frontier.top().ptr;
      frontier.pop();
      return tmp;
    }

    bool UCFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t UCFrontier::Size() const
    {
      return frontier.size();
    }

    UCFrontier::NodePtr::NodePtr(Node *node_in)
    {
      ptr = node_in;
    }

    bool UCFrontier::NodePtr::operator<(const NodePtr &rhs) const
    {
      return (ptr->GetPathCost() > rhs.ptr->GetPathCost());
    }

  }
}
