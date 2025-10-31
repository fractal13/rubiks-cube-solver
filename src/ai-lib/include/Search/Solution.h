#ifndef _SEARCH_SOLUTION_H_
#define _SEARCH_SOLUTION_H_
#include <list>
namespace ai
{
  namespace Search
  {
    /**
     * A list of nodes that represent a solution path.
     */
    class Solution
    {
    public:
      virtual ~Solution();
      virtual void Display() const;
      bool SetFromNode(Node *node_in);
      bool Clear();
      std::list<Node *> *GetList();
    protected:
      std::list<Node *> solution;
    private:
    };

  }
}

#endif /* _SOLUTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
