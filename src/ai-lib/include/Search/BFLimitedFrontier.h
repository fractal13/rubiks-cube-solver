#ifndef _BFLIMITEDFRONTIER_H_
#define _BFLIMITEDFRONTIER_H_

#include <queue>

namespace ai {
  namespace Search {
    /** FRONTIER
     * Implements a breadth-first frontier using STL's queue
     */
    class BFLimitedFrontier : public Frontier {
    public:
      BFLimitedFrontier(int max_depth_in);
      virtual ~BFLimitedFrontier();
      virtual bool Insert(Node *node_in);
      virtual Node *Remove();
      virtual bool Empty() const;
      virtual size_t Size() const;
    protected:
      std::queue<Node *> frontier;
    private:
      int max_depth;
    };
  }
}

#endif /* _BFLIMITEDFRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
