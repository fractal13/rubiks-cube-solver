#ifndef _BFFRONTIER_H_
#define _BFFRONTIER_H_

#include <queue>

namespace ai
{
  namespace Search
  {
    /** FRONTIER
     * Implements a breadth-first frontier using STL's queue
     */
    class BFFrontier : public Frontier
    {
    public:
      virtual ~BFFrontier();
      virtual bool Insert(Node *node_in);
      virtual Node *Remove();
      virtual bool Empty() const;
      virtual size_t Size() const;
    protected:
      std::queue<Node *> frontier;
    private:
    };
  }
}

#endif /* _BFFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
