#include "ai_csp.h"

namespace ai
{
  namespace ConstraintSatisfaction
  {
    /* empty string name */
    Variable::Variable()
      : name("")
    {
    }

    /* set the name */
    Variable::Variable(const std::string &name_in)
      : name(name_in)
    {
    }

    /* copy constructor */
    Variable::Variable(const Variable &rhs)
      : name(rhs.name)
    {
    }

    /* assignment operator */
    Variable &Variable::operator=(const Variable &rhs)
    {
      this->name = rhs.name;
      return *this;
    }

    /* virtual to allow destructor chaining */
    Variable::~Variable()
    {
      // empty
    }

    /* allow for comparison */
    bool Variable::operator<(const Variable &rhs) const
    {
      return this->name < rhs.name;
    }

    /* set the name */
    void Variable::setName(std::string &name_in)
    {
      this->name = name_in;
    }

    /* get the name */
    const std::string &Variable::getName() const
    {
      return this->name;
    }


  }
}
