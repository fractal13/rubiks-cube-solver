#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    GreedyFrontier::~GreedyFrontier()
    {
    }

    bool GreedyFrontier::Insert(Node *node_in)
    {
      NodePtr np(node_in);
      frontier.push(np);
      return true;
    }

    Node *GreedyFrontier::Remove()
    {
      Node *tmp = frontier.top().ptr;
      frontier.pop();
      return tmp;
    }

    bool GreedyFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t GreedyFrontier::Size() const
    {
      return frontier.size();
    }

    GreedyFrontier::NodePtr::NodePtr(Node *node_in)
    {
      ptr = node_in;
    }

    bool GreedyFrontier::NodePtr::operator<(const NodePtr &rhs) const
    {
      return ((ptr->GetHeuristic()) > (rhs.ptr->GetHeuristic()));
    }

  }
}
