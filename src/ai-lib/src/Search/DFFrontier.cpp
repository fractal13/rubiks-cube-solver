#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    DFFrontier::~DFFrontier()
    {
    }

    bool DFFrontier::Insert(Node *node_in)
    {
      frontier.push(node_in);
      return true;
    }

    Node *DFFrontier::Remove()
    {
      Node *tmp = frontier.top();
      frontier.pop();
      return tmp;
    }

    bool DFFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t DFFrontier::Size() const
    {
      return frontier.size();
    }
  }
}
