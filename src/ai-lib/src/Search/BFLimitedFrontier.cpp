#include "ai_search.h"

namespace ai 
{
  namespace Search 
  {
    
    BFLimitedFrontier::BFLimitedFrontier(int max_depth_in)
    {
      max_depth = max_depth_in;
    }

    BFLimitedFrontier::~BFLimitedFrontier() 
    {
    }

    bool BFLimitedFrontier::Insert(Node *node_in)
    {
      if(node_in->GetDepth() <= max_depth)
        {
          frontier.push(node_in);
          return true;
        }
      return false;
    }

    Node *BFLimitedFrontier::Remove()
    {
      Node *tmp = frontier.front();
      frontier.pop();
      return tmp;
    }

    bool BFLimitedFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t BFLimitedFrontier::Size() const
    {
      return frontier.size();
    }

  }
}
