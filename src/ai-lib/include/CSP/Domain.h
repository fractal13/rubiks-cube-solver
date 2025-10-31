#ifndef _DOMAIN_H_
#define _DOMAIN_H_

#include <set>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Domain
    {
    public:
      /* empty domain */
      Domain();

      /* set domain from set */
      Domain(const std::set<Value> &values_in);

      /* copy constructor */
      Domain(const Domain &rhs);

      /* assignment operator */
      Domain &operator=(const Domain &rhs);

      /* virtual to allow destructor chaining */
      virtual ~Domain();

      /* set the values */
      void setValues(const std::set<Value> &values_in);

      /* get the values */
      const std::set<Value> &getValues() const;

      /* remove a value */
      bool removeValue(const Value &value_in);

    protected:
      std::set<Value> values;
    private:
    };
  }
}

#endif /* _DOMAIN_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
