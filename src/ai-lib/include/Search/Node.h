#ifndef _SEARCH_NODE_H_
#define _SEARCH_NODE_H_
#include <list>

namespace ai
{
  namespace Search
  {
    /**
     * Represents information for one state in a search tree.
     */

    class Node
    {
    public:
      Node(State *state_in,
           Node *parent_in,
           Action *action_in,
           double path_cost_in,
           double heuristic_in,
           int depth_in);
      virtual ~Node();

      State  *GetState() const;
      Node   *GetParent() const;
      Action *GetAction() const;
      double  GetPathCost() const;
      double  GetHeuristic() const;
      int     GetDepth() const;

      bool    AddChild(Node *node_in);
      bool    RemoveChild(Node *node_in);
      bool    RemoveParent( );
      size_t  GetChildCount() const;

    protected:

      State  *state;
      Node   *parent;
      Action *action;
      double  path_cost;
      double  heuristic;
      int     depth;
      bool    in_destructor;

      std::list<Node *> children;

    private:
    };

  }
}

#endif /* _NODE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
