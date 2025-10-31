#include "ai_csp.h"

namespace ai
{
  namespace ConstraintSatisfaction
  {
    /* empty scope */
    Scope::Scope()
    {
    }

    /* set scope from vector */
    Scope::Scope(const std::vector<Variable> &variables_in)
      : variables(variables_in)
    {
    }

    /* copy constructor */
    Scope::Scope(const Scope &rhs)
      : variables(rhs.variables)
    {
    }

    /* assignment operator */
    Scope &Scope::operator=(const Scope &rhs)
    {
      this->variables = rhs.variables;
      return *this;
    }

    /* virtual to allow destructor chaining */
    Scope::~Scope()
    {
      // empty
    }

    /* set the variables */
    void Scope::setVariables(const std::vector<Variable> &variables_in)
    {
      this->variables = variables_in;
    }

    /* get the variables */
    const std::vector<Variable> &Scope::getVariables() const
    {
      return this->variables;
    }

  }
}
