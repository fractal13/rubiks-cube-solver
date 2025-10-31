#include "ai_csp.h"

namespace ai
{
  namespace ConstraintSatisfaction
  {
    /* empty constraint */
    Constraint::Constraint()
    {
      // empty
    }

    /* set initial constraint */
    Constraint::Constraint(const Scope &scope_in, const Relation &relation_in)
      : scope(scope_in),
        relation(relation_in)
    {
    }

    /* copy constructor */
    Constraint::Constraint(const Constraint &rhs)
      : scope(rhs.scope),
        relation(rhs.relation)
    {
    }

    /* assignment operator */
    Constraint &Constraint::operator=(const Constraint &rhs)
    {
      this->scope = rhs.scope;
      this->relation = rhs.relation;
      return *this;
    }

    /* virtual to allow destructor chaining */
    Constraint::~Constraint()
    {
      // empty
    }

    /* set the scope */
    void Constraint::setScope(const Scope &scope_in)
    {
      this->scope = scope_in;
    }

    /* get the scope */
    const Scope &Constraint::getScope() const
    {
      return this->scope;
    }

    /* set the relation */
    void Constraint::setRelation(const Relation &relation_in)
    {
      this->relation = relation_in;
    }

    /* get the relation */
    const Relation &Constraint::getRelation() const
    {
      return this->relation;
    }

  }
}
