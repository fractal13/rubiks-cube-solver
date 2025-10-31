#include "ai_search.h"
#include <iostream>

namespace ai
{
  namespace Search
  {
    /*
     * SOLUTION CLASS
     */
    Solution::~Solution()
    {
    }

    void Solution::Display() const
    {
      std::list<Node *>::const_iterator i;
      for(i = solution.begin(); i != solution.end(); i++)
        {
          if((*i)->GetAction())
            {
              (*i)->GetAction()->Display();
            }
          if((*i)->GetState())
            {
              (*i)->GetState()->Display();
            }
        }
      std::cout << "Cost: " << solution.back()->GetPathCost() << std::endl;
    }

    bool Solution::SetFromNode(Node *node_in)
    {
      Node *n;
      for(n = node_in; n != 0; n = n->GetParent())
        {
          solution.push_front(n);
        }
      return true;
    }

    bool Solution::Clear()
    {
      solution.clear();
      return true;
    }
    std::list<Node *> *Solution::GetList()
    {
      return &solution;
    }

  }
}

