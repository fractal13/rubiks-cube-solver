#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <string>
#include <iostream>

namespace ai
{
  namespace PL
  {
    class Symbol
    {
    public:
      enum State { STATE_MIN, KNOWN_TRUE, KNOWN_FALSE, UNKNOWN, STATE_MAX };

      Symbol(const std::string &name_in);
      void SetName(const std::string &name_in);
      void SetValue(State value_in);
      std::string GetName() const;
      State       GetValue() const;
      int         GetNumber() const;
      void        SetNumber(int number_in);
      
      static void ValidateName( const std::string &potential_name );
    protected:
      std::string name;
      State       value;
      int         number;
    private:
    };

    Symbol::State operator++(Symbol::State &lhs, int x);
    std::ostream & operator<<(std::ostream &os, const Symbol::State &rhs);

    bool operator==(const Symbol &lhs, const Symbol &rhs);
    bool operator==(const Symbol &lhs, const std::string &rhs);
    bool operator==(const std::string &lhs, const Symbol &rhs);

    std::ostream & operator<<(std::ostream &os, const Symbol &s);
  }
}
#endif /* _SYMBOL_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
