#include "ai_search.h"

namespace ai
{
  namespace Search
  {

    BFFrontier::~BFFrontier()
    {
    }

    bool BFFrontier::Insert(Node *node_in)
    {
      frontier.push(node_in);
      return true;
    }

    Node *BFFrontier::Remove()
    {
      Node *tmp = frontier.front();
      frontier.pop();
      return tmp;
    }

    bool BFFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t BFFrontier::Size() const
    {
      return frontier.size();
    }

  }
}
