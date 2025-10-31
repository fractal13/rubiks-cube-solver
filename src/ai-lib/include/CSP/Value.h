#ifndef _VALUE_H_
#define _VALUE_H_

#include <string>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Value
    {
    public:
      /* empty string value */
      Value();

      /* set domain from set */
      Value(const std::string &value_in);

      /* copy constructor */
      Value(const Value &rhs);

      /* assignment operator */
      Value &operator=(const Value &rhs);

      /* virtual to allow destructor chaining */
      virtual ~Value();

      /* allow comparison */
      virtual bool operator<(const Value &rhs) const;

      /* set the value */
      virtual void setValue(const std::string &value_in);

      /* get the value */
      virtual const std::string &getValue() const;

    protected:
      std::string value;
    private:
    };
  }
}

#endif /* _VALUE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
