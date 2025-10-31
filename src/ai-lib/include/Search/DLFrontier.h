#ifndef _DLFRONTIER_H_
#define _DLFRONTIER_H_

#include <stack>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements a depth-limited frontier using STL's stack
     */
    class DLFrontier : public Frontier
    {
    public:
      DLFrontier(int max_depth_in);
      virtual ~DLFrontier();
      virtual bool Insert(Node *node_in);
      virtual Node *Remove();
      virtual bool Empty() const;
      virtual size_t Size() const;
    protected:
      std::stack<Node *> frontier;
    private:
      int max_depth;
    };
  }
}

#endif /* _DLFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
