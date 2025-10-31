#ifndef _GRAPH_H_
#define _GRAPH_H_

namespace ai
{
  namespace Search
  {
    /**
     * Search algorithm to remove repeated states from search space.
     * See Algorithm.h
     */
    class Graph : public Algorithm
    {
    public:
      Graph(Problem *problem_in, Frontier *frontier_in);
      virtual ~Graph();
      virtual bool Search();

    protected:
      ClosedList closed;
    private:
    };
  }
}

#endif /* _GRAPH_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
