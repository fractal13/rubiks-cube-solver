#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include <set>

namespace ai
{
  namespace ConstraintSatisfaction
  {
    class Constraint
    {
    public:
      /* empty constraint */
      Constraint();

      /* set initial constraint */
      Constraint(const Scope &scope_in, const Relation &relation_in);

      /* copy constructor */
      Constraint(const Constraint &rhs);

      /* assignment operator */
      Constraint &operator=(const Constraint &rhs);

      /* virtual to allow destructor chaining */
      virtual ~Constraint();

      /* set the scope */
      void setScope(const Scope &scope_in);

      /* get the scope */
      const Scope &getScope() const;

      /* set the relation */
      void setRelation(const Relation &relation_in);

      /* get the relation */
      const Relation &getRelation() const;

    protected:
      Scope    scope;
      Relation relation;
    private:
    };
  }
}

#endif /* _CONSTRAINT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
