#include <iostream>
#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    /*
     * NODE CLASS
     */
    Node::Node(State *state_in,
               Node *parent_in,
               Action *action_in,
               double path_cost_in,
               double heuristic_in,
               int depth_in)
    {
      state     = state_in;
      parent    = parent_in;
      action    = action_in;
      path_cost = path_cost_in;
      heuristic = heuristic_in;
      depth     = depth_in;
      in_destructor = false;
    }

    Node::~Node()
    {
      if(in_destructor) { return; }
      in_destructor = true;
#if 0
      /* This version will cause segmentation fault
       * through stack overflow with very deep trees.
       * Caused by the recursive calls to the destructor. */
      std::list<Node *>::iterator i;
      for(i = children.begin(); i != children.end(); i++)
        {
          delete (*i);
        }
      children.clear();
#else
      /* This version cleans up without deep recursive
       * calls to the destructor. */
      std::stack<Node *> nodes;
      nodes.push(this);
      std::list<Node *>::iterator i;

      while(nodes.size() > 0)
        {
          Node *current = nodes.top();
          if(current->children.size() == 0)
            {
              nodes.pop();
              if(current != this)
                {
                  delete current;
                }
            }
          else
            {
              for(i = current->children.begin(); i != current->children.end(); i++)
                {
                  nodes.push( (*i) );
                }
              current->children.clear();
            }
        }
#endif
      if(state)
        {
          delete state;
          state = 0;
        }
      if(action)
        {
          delete action;
          action = 0;
        }
      /* do not delete parent here */
      if(parent)
        { /* tell parent that I'm gone */
          if(parent->RemoveChild(this))
            {
              if(parent->children.size() == 0 &&
                 parent->parent != 0)
                {
                  delete parent;
                }
            }
        }
    }

    State  *Node::GetState() const
    {
      return state;
    }
    Node   *Node::GetParent() const
    {
      return parent;
    }
    Action *Node::GetAction() const
    {
      return action;
    }
    double  Node::GetPathCost() const
    {
      return path_cost;
    }
    double  Node::GetHeuristic() const
    {
      return heuristic;
    }
    int     Node::GetDepth() const
    {
      return depth;
    }

    bool    Node::AddChild(Node *node_in)
    {
      children.push_back(node_in);
      return true;
    }

    bool    Node::RemoveChild(Node *node_in)
    {
      std::list<Node *>::iterator i;
      for(i = children.begin(); i != children.end(); i++)
        {
          if((*i) == node_in)
            {
              children.erase(i);
              return true;
            }
        }
      return false;
    }

    bool    Node::RemoveParent( ) {
      parent = 0;
      return true;
    }

    size_t  Node::GetChildCount() const
    {
      return children.size();
    }


  }
}
