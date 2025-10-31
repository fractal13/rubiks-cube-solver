#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <string>
#include <iostream>
#include <vector>

namespace ai
{
  namespace FOL
  {
    class Symbol
    {
    public:
      enum Type { TYPE_MIN, TYPE_UNKNOWN, TYPE_CONSTANT, TYPE_VARIABLE,
                  TYPE_PREDICATE, TYPE_FUNCTION, TYPE_MAX };

      Symbol(const std::string &name_in);
      void SetType(Type type_in);
      void SetArity(int arity_in);
      void MakeUnique(const std::string &new_name_in);
      std::string GetName() const;
      Type        GetType() const;
      int         GetArity() const;
      bool        GetIsUnique() const;
      std::string GetOriginalName() const;
    protected:
      std::string name;
      Type        type;
      int         arity;
      bool        is_unique;
      std::string original_name;
    private:
    };

    std::ostream & operator<<(std::ostream &os, const Symbol::Type &rhs);

    bool operator==(const Symbol &lhs, const Symbol &rhs);
    bool operator==(const Symbol &lhs, const std::string &rhs);
    bool operator==(const std::string &lhs, const Symbol &rhs);

    std::ostream & operator<<(std::ostream &os, const Symbol &s);
    std::ostream & operator<<(std::ostream &os,  const std::vector<Symbol *> &symbols);

  }
}
#endif /* _SYMBOL_H_ */
