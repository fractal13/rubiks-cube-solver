#include "ai_search.h"
#include <iostream>
namespace ai
{
  namespace Search
  {
    /*
     * STATE CLASS
     */
    State::~State()
    {
    }
    void   State::Display() const
    {
      std::cout << "State Display.  Override me to get better information." << std::endl;
    }

  }
}
