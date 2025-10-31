#include <ai_fol.h>

namespace ai
{
  namespace FOL
  {
  
    std::ostream & operator<<(std::ostream &os, const Symbol::Type &rhs)
    {
      switch(rhs)
	{
	case Symbol::TYPE_UNKNOWN:
	  os << "Unknown";
	  break;
	case Symbol::TYPE_CONSTANT:
	  os << "Constant";
	  break;
	case Symbol::TYPE_VARIABLE:
	  os << "Variable";
	  break;
	case Symbol::TYPE_PREDICATE:
	  os << "Predicate";
	  break;
	case Symbol::TYPE_FUNCTION:
	  os << "Function";
	  break;
	default:
	  os << "Type Error";
	  break;
	}
      return os;
    }

  
    Symbol::Symbol(const std::string &name_in)
    {
      name = name_in;
      type = Symbol::TYPE_UNKNOWN;
      arity = 0;
      is_unique = false;
      original_name = "";
    }
  
    void Symbol::SetType(Type type_in)
    {
      type = type_in;
    }
  
    void Symbol::SetArity(int arity_in)
    {
      arity = arity_in;
    }
    void Symbol::MakeUnique(const std::string &new_name_in)
    {
      name = new_name_in;
      is_unique = true;
      original_name = name;
    }
  
    std::string Symbol::GetName() const
    {
      return name;
    }
  
    Symbol::Type Symbol::GetType() const
    {
      return type;
    }
  
    int Symbol::GetArity() const
    {
      return arity;
    }
    
    bool Symbol::GetIsUnique() const
    {
      return is_unique;
    }
    
    std::string Symbol::GetOriginalName() const
    {
      return original_name;
    }

    /* Both are symbols, the arity needs to match */
    bool operator==(const Symbol &lhs, const Symbol &rhs)
    {
      return ( (lhs.GetName() == rhs.GetName()) &&
	       (lhs.GetArity() == rhs.GetArity()) );
    }
    /* Only one is symbol, ignore arity */
    bool operator==(const Symbol &lhs, const std::string &rhs)
    {
      return lhs.GetName() == rhs;
    }
    /* Only one is symbol, ignore arity */
    bool operator==(const std::string &lhs, const Symbol &rhs)
    {
      return rhs.GetName() == lhs;
    }
  
  
    std::ostream & operator<<(std::ostream &os, const Symbol &s)
    {
      os << s.GetName() << "/" << s.GetArity() << " " << s.GetType();
      return os;
    }

    // Display a set of symbols
    std::ostream &operator<<(std::ostream &os,
			     const std::vector<Symbol *> &symbols)
    {
      unsigned int i;
      os << "Model" << std::endl;
      for(i = 0; i < symbols.size(); i++)
	{
	  os << *(symbols[i]) << "  ";
	}
      os << std::endl;
      return os;
    }

  }
}
