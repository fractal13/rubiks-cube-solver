#include <ai_pl.h>
#include <sstream>

namespace ai
{
  namespace PL
  {
    Symbol::State operator++(Symbol::State &lhs, int x)
    {
      Symbol::State copy = lhs;
      switch(lhs)
        {
        case Symbol::STATE_MIN:
          lhs = Symbol::KNOWN_TRUE;
          break;
        case Symbol::KNOWN_TRUE:
          lhs = Symbol::KNOWN_FALSE;
          break;
        case Symbol::KNOWN_FALSE:
          lhs = Symbol::UNKNOWN;
          break;
        case Symbol::UNKNOWN:
          lhs = Symbol::STATE_MAX;
          break;
        case Symbol::STATE_MAX:
          // do nothing
          break;
        default:
          {
            std::stringstream ss;
            ss << "Symbol::State is invalid: " << (int) lhs;
            throw std::domain_error( ss.str( ) );
          }
          break;
        }
      return copy;
    }

    std::ostream & operator<<(std::ostream &os, const Symbol::State &rhs)
    {
      switch(rhs)
        {
        case Symbol::KNOWN_TRUE:
          os << "True";
          break;
        case Symbol::KNOWN_FALSE:
          os << "False";
          break;
        case Symbol::UNKNOWN:
          os << "Unknown";
          break;
        default:
          {
            std::stringstream ss;
            ss << "Symbol::State is invalid: " << (int) rhs;
            throw std::domain_error( ss.str( ) );
          }
          break;
        }
      return os;
    }


    Symbol::Symbol(const std::string &name_in)
    {
      ValidateName( name_in );
      name = name_in;
      value = Symbol::UNKNOWN;
    }

    void Symbol::SetName(const std::string &name_in)
    {
      ValidateName( name_in );
      name = name_in;
    }
    
    void Symbol::SetValue(State value_in)
    {
      if( value_in > State::STATE_MIN && value_in < State::STATE_MAX ) {
        value = value_in;
      } else {
        std::stringstream ss;
        ss << "Symbol::State is invalid: " << (int) value_in;
        throw std::domain_error( ss.str( ) );
      }
    }

    std::string Symbol::GetName() const
    {
      return name;
    }

    Symbol::State       Symbol::GetValue() const
    {
      return value;
    }

    int         Symbol::GetNumber() const
    {
      return this->number;
    }
    void        Symbol::SetNumber(int number_in)
    {
      this->number = number_in;
    }
    
    void Symbol::ValidateName( const std::string &potential_name )
    {
      if( potential_name.size( ) == 0 ) {
        throw std::domain_error( "Empty name." );
      }
      if( !( ( potential_name[ 0 ] >= 'A' && potential_name[ 0 ] <= 'Z' ) ||
             ( potential_name[ 0 ] >= 'a' && potential_name[ 0 ] <= 'z' ) ) ) {
        throw std::domain_error( "Bad Symbol Name: First character must be [a-zA-Z], found '" + std::string( 1, potential_name[ 0 ] ) + "'" );
      }
      size_t i;
      for( i = 1 ; i < potential_name.size( ) ; i++ ) {
        
        if( !( ( potential_name[ i ] >= 'A' && potential_name[ i ] <= 'Z' ) ||
               ( potential_name[ i ] >= 'a' && potential_name[ i ] <= 'z' ) ||
               ( potential_name[ i ] >= '0' && potential_name[ i ] <= '9' ) ||
               ( potential_name[ i ] == '_' )
               ) ) {
          throw std::domain_error( "Bad Symbol Name: non-initial characters must be [a-zA-Z0-9_], found '" + std::string( 1, potential_name[ i ] ) + "'" );
        }
      }
    }

    bool operator==(const Symbol &lhs, const Symbol &rhs)
    {
      return lhs.GetName() == rhs.GetName();
    }
    bool operator==(const Symbol &lhs, const std::string &rhs)
    {
      return lhs.GetName() == rhs;
    }
    bool operator==(const std::string &lhs, const Symbol &rhs)
    {
      return rhs.GetName() == lhs;
    }


    std::ostream & operator<<(std::ostream &os, const Symbol &s)
    {
      os << s.GetName() << " = " << s.GetValue() << "[" << s.GetNumber() << "]";
      return os;
    }
  }
}
