#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    Tree::Tree(Problem *problem_in, Frontier *frontier_in)
      : Algorithm(problem_in, frontier_in)
    {
    }

    Tree::~Tree()
    {
    }

    bool Tree::Search()
    {
      if(!SearchInit())
        {
          return false;
        }
      return SearchNext();
    }

    bool Tree::SearchInit()
    {
      if(root)
        {
          delete root;
          root   = 0;
        }
      // Count the root
      number_nodes_generated = 1;
      max_nodes_stored       = 1;
      
      root = new Node(problem->GetInitialState(), 0, 0, 0.0, problem->Heuristic(problem->GetInitialState()), 0);
      frontier->Insert(root);
      return true;
    }

    bool Tree::SearchNext()
    {
      while(!frontier->Empty() &&
            (generation_limit == 0 || generation_limit > number_nodes_generated) &&
            (store_limit == 0 || store_limit > max_nodes_stored)
            )
        {
          Node *node = frontier->Remove();

          State *s1 = node->GetState();
          if(problem->GoalTest(s1))
            {
              solution.SetFromNode(node);
              return true;
            }

          std::vector<Action *> actions = problem->Actions(s1);
          std::vector<Action *>::iterator aiter;

          for(aiter = actions.begin(); aiter != actions.end(); aiter++)
            {
              State *s2 = problem->Result(s1, *aiter);

              Node *new_node = new Node(s2,     // new state
                                        node,   // parent node
                                        *aiter, // action
                                        (node->GetPathCost() + problem->StepCost(s1, *aiter, s2)), // total path cost
                                        problem->Heuristic(s2), // heuristic
                                        (node->GetDepth() + 1)  // depth in tree
                                        );
              // Add to frontier
              if(!frontier->Insert(new_node))
                { // depth limited, is full, etc.
                  delete new_node;
                  continue;
                }
              
              // Forward links in tree
              node->AddChild(new_node);
              
              // Statistics
              number_nodes_generated++;
              max_nodes_stored = std::max(max_nodes_stored, frontier->Size());
            }
          deleteNodeIfNeeded( node );
        }
      return false;
    }

    void Tree::deleteNodeIfNeeded( Node *node ) {
      /* delete a Node if it has no children */
      if ( node ) {
        if ( node->GetChildCount( ) == 0 ) {
          
          Node *parent = node->GetParent( );
          /* disconnect from parent */
          if ( parent ) {
            parent->RemoveChild( node );
            node->RemoveParent( );
          } else {
            root = 0;
          }
          /* finally delete the node */
          delete node;
          /* delete parent if needed */
          deleteNodeIfNeeded( parent );
        }
      }
    }
    
    
  }
}
