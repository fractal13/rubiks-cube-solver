#include "ai_csp.h"

namespace ai
{
  namespace ConstraintSatisfaction
  {
    /* empty domain */
    Domain::Domain()
    {
      // empty
    }

    /* set domain from set */
    Domain::Domain(const std::set<Value> &values_in)
      : values(values_in)
    {
    }

    /* copy constructor */
    Domain::Domain(const Domain &rhs)
      : values(rhs.values)
    {
    }

    /* assignment operator */
    Domain &Domain::operator=(const Domain &rhs)
    {
      this->values = rhs.values;
      return *this;
    }

    /* virtual to allow destructor chaining */
    Domain::~Domain()
    {
      // empty
    }

    /* set the values */
    void Domain::setValues(const std::set<Value> &values_in)
    {
      this->values = values_in;
    }

    /* get the values */
    const std::set<Value> &Domain::getValues() const
    {
      return this->values;
    }

    /* remove a value */
    bool Domain::removeValue(const Value &value_in)
    {
      std::set<Value>::iterator it = this->values.find(value_in);
      if(it != this->values.end())
        {
          this->values.erase(it);
          return true;
        }
      return false;
    }

  }
}
