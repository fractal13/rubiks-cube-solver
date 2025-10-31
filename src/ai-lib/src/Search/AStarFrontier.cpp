#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    AStarFrontier::~AStarFrontier()
    {
    }

    bool AStarFrontier::Insert(Node *node_in)
    {
      NodePtr np(node_in);
      frontier.push(np);
      return true;
    }

    Node *AStarFrontier::Remove()
    {
      Node *tmp = frontier.top().ptr;
      frontier.pop();
      return tmp;
    }

    bool AStarFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t AStarFrontier::Size() const
    {
      return frontier.size();
    }

    AStarFrontier::NodePtr::NodePtr(Node *node_in)
    {
      ptr = node_in;
    }

    bool AStarFrontier::NodePtr::operator<(const NodePtr &rhs) const
    {
      return ((ptr->GetPathCost() + ptr->GetHeuristic()) > (rhs.ptr->GetPathCost() + rhs.ptr->GetHeuristic()));
    }

  }
}
