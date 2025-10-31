#ifndef _RELATION_H_
#define _RELATION_H_

#include <set>
#include <vector>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Relation
    {
    public:
      /* empty relation */
      Relation();

      /* set relation from set */
      Relation(const std::set<std::vector<Value> > &values_in);

      /* copy constructor */
      Relation(const Relation &rhs);

      /* assignment operator */
      Relation &operator=(const Relation &rhs);

      /* virtual to allow destructor chaining */
      virtual ~Relation();

      /* set the values */
      void setValues(const std::set<std::vector<Value> > &values_in);

      /* get the values */
      const std::set<std::vector<Value> > &getValues() const;

    protected:
      std::set<std::vector<Value> > values;
    private:
    };
  }
}

#endif /* _RELATION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
