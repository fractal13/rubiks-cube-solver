#include "ai_csp.h"

namespace ai
{
  namespace ConstraintSatisfaction
  {
    Value::Value()
      : value("")
    {
    }

    /* set domain from set */
    Value::Value(const std::string &value_in)
      : value(value_in)
    {
    }

    /* copy constructor */
    Value::Value(const Value &rhs)
      : value(rhs.value)
    {
    }

    /* assignment operator */
    Value &Value::operator=(const Value &rhs)
    {
      this->value = rhs.value;
      return *this;
    }

    /* virtual to allow destructor chaining */
    Value::~Value()
    {
      // empty
    }

    /* allow comparison */
    bool Value::operator<(const Value &rhs) const
    {
      return this->value < rhs.value;
    }

    /* set the value */
    void Value::setValue(const std::string &value_in)
    {
      this->value = value_in;
    }

    /* get the value */
    const std::string &Value::getValue() const
    {
      return this->value;
    }

  }
}
