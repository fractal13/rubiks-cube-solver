#include "ai_search.h"
#include <iostream>

namespace ai
{
  namespace Search
  {
    /*
     * ACTION CLASS
     */
    Action::~Action()
    {
    }

    void   Action::Display() const
    {
      std::cout << "Action Display. Override me if you want better information." << std::endl;
    }

  }
}
