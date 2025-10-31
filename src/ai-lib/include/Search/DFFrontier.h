#ifndef _DFFRONTIER_H_
#define _DFFRONTIER_H_

#include <stack>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements a depth-first frontier using STL's stack
     */
    class DFFrontier : public Frontier
    {
    public:
      virtual ~DFFrontier();
      virtual bool Insert(Node *node_in);
      virtual Node *Remove();
      virtual bool Empty() const;
      virtual size_t Size() const;
    protected:
      std::stack<Node *> frontier;
    private:
    };
  }
}



#endif /* _DFFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
