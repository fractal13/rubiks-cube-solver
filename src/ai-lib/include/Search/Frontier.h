#ifndef _FRONTIER_H_
#define _FRONTIER_H_

namespace ai
{
  namespace Search
  {
    /**
     * Abstract base class to enforce support for
     * frontier operations.
     */
    class Frontier
    {
    public:
      enum FrontierType
        {
          T_NULL,
          T_DepthFirst,
          T_BreadthFirst,
          T_MAX
        };

      virtual ~Frontier();
      virtual bool Insert(Node *node_in) = 0;
      virtual Node *Remove() = 0;
      virtual bool Empty() const = 0;
      virtual size_t Size() const = 0;
    protected:
    private:
    };
  }
}

#endif /* _FRONTIER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
