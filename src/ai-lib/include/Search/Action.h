#ifndef _SEARCH_ACTION_H_
#define _SEARCH_ACTION_H_
namespace ai
{
  namespace Search
  {
    /**
     * Base class for actions to evolve states during search
     */
    class Action
    {
    public:
      /* Virtual to insure destructor chaining
       */
      virtual ~Action();

      /* Textual display of action for "print debugging" usage.
       */
      virtual void   Display() const;

    protected:
      /* Data members to support your search problem.
       */
    private:
    };
  }
}

#endif /* _ACTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
