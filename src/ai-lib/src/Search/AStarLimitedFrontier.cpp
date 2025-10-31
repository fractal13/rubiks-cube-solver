#include "ai_search.h"

namespace ai
{
  namespace Search
  {

    AStarLimitedFrontier::AStarLimitedFrontier(double max_cost_in)
    {
      max_cost = max_cost_in;
    }

    AStarLimitedFrontier::~AStarLimitedFrontier()
    {
    }

    bool AStarLimitedFrontier::Insert(Node *node_in)
    {
      if(node_in->GetPathCost() + node_in->GetHeuristic() <= max_cost)
        {
          frontier.push(node_in);
          return true;
        }
      return false;
    }

    Node *AStarLimitedFrontier::Remove()
    {
      Node *tmp = frontier.top();
      frontier.pop();
      return tmp;
    }

    bool AStarLimitedFrontier::Empty() const
    {
      return frontier.empty();
    }

    size_t AStarLimitedFrontier::Size() const
    {
      return frontier.size();
    }
  }
}


