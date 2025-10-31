#include "ai_search.h"

namespace ai
{
  namespace Search
  {

    DLFrontier::DLFrontier(int max_depth_in)
    {
      max_depth = max_depth_in;
    }

    DLFrontier::~DLFrontier()
    {
    }

    bool DLFrontier::Insert(Node *node_in)
    {
      if(node_in->GetDepth() <= max_depth)
        {
          frontier.push(node_in);
          return true;
        }
      return false;
    }

    Node *DLFrontier::Remove()
    {
      Node *tmp = frontier.top();
      frontier.pop();
      return tmp;
    }

    bool DLFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t DLFrontier::Size() const
    {
      return frontier.size();
    }
  }
}


