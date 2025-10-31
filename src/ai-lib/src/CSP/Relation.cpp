#include "ai_csp.h"

namespace ai
{
  namespace ConstraintSatisfaction
  {

    /* empty relation */
    Relation::Relation()
    {
      // empty
    }

    /* set relation from set */
    Relation::Relation(const std::set<std::vector<Value> > &values_in)
      : values(values_in)
    {
    }

    /* copy constructor */
    Relation::Relation(const Relation &rhs)
      : values(rhs.values)
    {
    }

    /* assignment operator */
    Relation &Relation::operator=(const Relation &rhs)
    {
      this->values = rhs.values;
      return *this;
    }

    /* virtual to allow destructor chaining */
    Relation::~Relation()
    {
      // empty
    }

    /* set the values */
    void Relation::setValues(const std::set<std::vector<Value> > &values_in)
    {
      this->values = values_in;
    }

    /* get the values */
    const std::set<std::vector<Value> > &Relation::getValues() const
    {
      return this->values;
    }

  }
}
