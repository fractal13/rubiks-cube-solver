#include "ai_search.h"
#include <algorithm>

namespace ai
{
  namespace Search
  {
    Graph::Graph(Problem *problem_in, Frontier *frontier_in)
      : Algorithm(problem_in, frontier_in)
    {
    }

    Graph::~Graph()
    {
    }

    bool Graph::Search()
    {
      // Count the root
      number_nodes_generated = 1;
      max_nodes_stored       = 1;

      root = new Node(problem->GetInitialState(), 0, 0, 0.0, problem->Heuristic(problem->GetInitialState()), 0);
      frontier->Insert(root);
      closed.Clear();

      while(!frontier->Empty() &&
            (generation_limit == 0 || generation_limit > number_nodes_generated) &&
            (store_limit == 0 || store_limit > max_nodes_stored)
            )
        {
          Node *node = frontier->Remove();
          if(closed.Contains(node))
            { // already processed this state
              continue;
            }
          
          State *s1 = node->GetState();
          if(problem->GoalTest(s1))
            {
              solution.SetFromNode(node);
              return true;
            }
          closed.Insert(node);
          std::vector<Action *> actions = problem->Actions(s1);
          std::vector<Action *>::iterator aiter;

          for(aiter = actions.begin(); aiter != actions.end(); aiter++)
            {
              State *s2 = problem->Result(s1, *aiter);
              if(closed.Contains(s2))
                { // Already have this state
                  delete s2;
                  delete *aiter;
                  continue;
                }

              Node *new_node = new Node(s2,     // new state
                                        node,   // parent node
                                        *aiter, // action
                                        (node->GetPathCost() + problem->StepCost(s1, *aiter, s2)), // total path cost
                                        problem->Heuristic(s2), // heuristic
                                        (node->GetDepth() + 1)  // depth in tree
                                        );
              // Add to frontier
              if(!frontier->Insert(new_node))
                { // depth limited is full, etc.
                  delete new_node;
                  continue;
                }
              
              // Forward links in tree
              node->AddChild(new_node);
              
              // Statistics
              number_nodes_generated++;
              max_nodes_stored = std::max(max_nodes_stored, closed.Size() + frontier->Size());
            }
        }
      return false;
    }

  }
}
