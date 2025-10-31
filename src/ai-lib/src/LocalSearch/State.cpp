#include "ai_local_search.h"

namespace ai
{
  namespace LocalSearch
  {
    State::~State()
    {
      /* empty */
    }
    std::ostream &State::Display(std::ostream &os) const
    {
      os << "Override ai::LocalSearch::State::Display()";
      return os;
    }
  }
}
