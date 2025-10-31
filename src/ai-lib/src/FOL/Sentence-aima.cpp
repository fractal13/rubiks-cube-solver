#include <ai_fol.h>
#include <cstdio>

namespace ai
{
  namespace FOL
  {
    Sentence::Sentence(bool value_in)
    {
      if(value_in)
	{
	  type = T_TRUE;
	}
      else
	{
	  type = T_FALSE;
	}
      symbol = 0;
      lhs    = 0;
      op     = O_NOT;
      rhs    = 0;
    }
    Sentence::Sentence(SentenceType type_in)
    {
      type   = type_in; // should be T_LEFT_PAREN or T_RIGHT_PAREN
      symbol = 0;
      lhs    = 0;
      op     = O_NOT;
      rhs    = 0;
    }
    Sentence::Sentence(SentenceType type_in, OperatorType op_in)
    {
      type   = type_in;
      symbol = 0;
      lhs    = 0;
      op     = op_in;
      rhs    = 0;
    }
    Sentence::Sentence(SentenceType type_in, Sentence *rhs_in)
    {
      type   = type_in;
      symbol = 0;
      lhs    = 0;
      op     = O_NOT;
      rhs    = rhs_in;
    }
    Sentence::Sentence(Symbol *symbol_in)
    {
      type   = T_SYMBOL;
      symbol = symbol_in;
      lhs    = 0;
      op     = O_NOT;
      rhs    = 0;
    }
    Sentence::Sentence(Symbol *symbol_in, Sentence *rhs_in)
    {
      type   = T_SYMBOL;
      symbol = symbol_in;
      lhs    = 0;
      op     = O_NOT;
      rhs    = rhs_in;
    }
    Sentence::Sentence(SentenceType type_in, Symbol *symbol_in, Sentence *rhs_in)
    {
      type   = type_in;
      symbol = symbol_in; // quantifier variable
      lhs    = 0;
      op     = O_NOT;
      rhs    = rhs_in;
    }
    Sentence::Sentence(OperatorType op_in, Sentence *rhs_in)
    {
      type   = T_UNARY;
      symbol = 0;
      lhs    = 0;
      op     = op_in;
      rhs    = rhs_in;
    }
    Sentence::Sentence(Sentence *lhs_in, OperatorType op_in, Sentence *rhs_in)
    {
      type   = T_BINARY;
      symbol = 0;
      lhs    = lhs_in;
      op     = op_in;
      rhs    = rhs_in;
    }
    
    Sentence::Sentence(const Sentence &rhs_in)
    {
      type   = T_TRUE;
      symbol = 0;
      lhs    = 0;
      op     = O_NOT;
      rhs    = 0;
      
      *this = rhs_in;
    }

    Sentence &Sentence::operator=(const Sentence &rhs_in)
    {
      Delete();

      type   = rhs_in.type;
      symbol = rhs_in.symbol;
      op     = rhs_in.op;
      if(rhs_in.lhs)
	{
	  lhs = new Sentence(*(rhs_in.lhs));
	}
      if(rhs_in.rhs)
	{
	  rhs = new Sentence(*(rhs_in.rhs));
	}
      
      return *this;
    }
    
    Sentence::~Sentence()
    {
      Delete();
    }

    void Sentence::Delete()
    {
      if(symbol != 0)
	{
	  symbol = 0;
	  // do not delete, symbols are deleted elsewhere.
	}
      if(lhs != 0)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs != 0)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
  
    void Sentence::SetLHS(Sentence *lhs_in)
    {
      if(lhs != 0)
	{
	  delete lhs;
	}
      lhs = lhs_in;
    }
    void Sentence::SetRHS(Sentence *rhs_in)
    {
      if(rhs != 0)
	{
	  delete rhs;
	}
      rhs = rhs_in;
    }
    void Sentence::SetSymbol(Symbol *symbol_in)
    {
      if(symbol != 0)
	{
	  delete symbol;
	}
      symbol = symbol_in;
    }
    void Sentence::SetLHSNoDelete(Sentence *lhs_in)
    {
      lhs = lhs_in;
    }
    void Sentence::SetRHSNoDelete(Sentence *rhs_in)
    {
      rhs = rhs_in;
    }
    void Sentence::SetSymbolNoDelete(Symbol *symbol_in)
    {
      symbol = symbol_in;
    }
    
    void Sentence::SetVariable(Symbol *variable_in)
    {
      if((symbol) &&
	 (symbol->GetName() == variable_in->GetName()) &&
	 (symbol->GetType() == Symbol::TYPE_CONSTANT))
	{
	  symbol->SetType(Symbol::TYPE_VARIABLE);
	}
      if(lhs)
	{
	  lhs->SetVariable(variable_in);
	}
      if(rhs)
	{
	  rhs->SetVariable(variable_in);
	}
    }
  
    Sentence::SentenceType    Sentence::GetType() const
    {
      return type;
    }
    const Symbol   *Sentence::GetSymbol() const
    {
      return symbol;
    }
    Sentence::OperatorType    Sentence::GetOperator() const
    {
      return op;
    }
    const Sentence *Sentence::GetLHS() const
    {
      return lhs;
    }
    const Sentence *Sentence::GetRHS() const
    {
      return rhs;
    }
    
    void Sentence::GetAllSymbols(std::list<Symbol *> &symbols_out) const
    {
      if(symbol)
	{
	  symbols_out.push_back(symbol);
	}
      if(lhs)
	{
	  lhs->GetAllSymbols(symbols_out);
	}
      if(rhs)
	{
	  rhs->GetAllSymbols(symbols_out);
	}
    }


    std::string Sentence::ToString() const
    {
      return ToString(true);
    }
    
    std::string Sentence::ToString(bool verbose) const
    {
      Sentence::SentenceType type   = this->GetType();
      Sentence::OperatorType op     = this->GetOperator();
      std::string  str    = "";
      std::string  op_str = "";
      switch(op)
	{
	case Sentence::O_NOT:
	  op_str = "!";
	  break;
	case Sentence::O_AND:
	  op_str = "&";
	  break;
	case Sentence::O_OR:
	  op_str = "|";
	  break;
	case Sentence::O_IMPLIES:
	  op_str = "=>";
	  break;
	case Sentence::O_IFF:
	  op_str = "<=>";
	  break;
	case Sentence::O_COMMA:
	  op_str = ",";
	  break;
	case Sentence::O_EQUAL:
	  op_str = "=";
	  break;
	default:
	  op_str = "OPERATOR_ERROR";
	  break;
	}
    
      switch(type)
	{
	case Sentence::T_TRUE:
	  str += "True";
	  break;
	case Sentence::T_FALSE:
	  str += "False";
	  break;
	case Sentence::T_SYMBOL:
	  str += this->GetSymbol()->GetName();
	  if(verbose)
	    {
	      if(this->GetSymbol()->GetType() == Symbol::TYPE_VARIABLE)
		{
		  str += "/v";
		}
	      if(this->GetSymbol()->GetType() == Symbol::TYPE_CONSTANT)
		{
		  str += "/c";
		}
	      if((this->GetSymbol()->GetType() == Symbol::TYPE_PREDICATE) ||
		 (this->GetSymbol()->GetType() == Symbol::TYPE_FUNCTION))
		{
		  char buf[16];
		  std::sprintf(buf, "%d", this->GetSymbol()->GetArity());
		  str += "/";
		  str += buf;
		}
	    }
	  if(this->GetRHS() != 0)
	    {
	      str += "(";
	      str += this->GetRHS()->ToString();
	      str += ")";
	    }
	  break;
	case Sentence::T_UNARY:
	  str +=  "( " + op_str;
	  if(this->GetRHS() != 0)
	    {
	      str += this->GetRHS()->ToString();
	    }
	  else
	    {
	      str += "(null)";
	    }
	  str += " )";
	  break;
	case Sentence::T_BINARY:
	  if(op == O_COMMA)
	    {
	      if(this->GetLHS() != 0)
		{
		  str += this->GetLHS()->ToString();
		}
	      else
		{
		  str += "(null)";
		}
	      str += op_str + " ";
	      if(this->GetRHS() != 0)
		{
		  str += this->GetRHS()->ToString();
		}
	      else
		{
		  str += "(null)";
		}
	    }
	  else if(op == O_EQUAL)
	    {
	      if(this->GetLHS() != 0)
		{
		  str += this->GetLHS()->ToString();
		}
	      else
		{
		  str += "(null)";
		}
	      str += " " + op_str + " ";
	      if(this->GetRHS() != 0)
		{
		  str += this->GetRHS()->ToString();
		}
	      else
		{
		  str += "(null)";
		}
	    }
	  else
	    { /* O_OR, O_AND, O_IMPLIES, O_IFF */
	      str += "( ";
	      if(this->GetLHS() != 0)
		{
		  str += this->GetLHS()->ToString();
		}
	      else
		{
		  str += "(null)";
		}
	      str += " " + op_str + " ";
	      if(this->GetRHS() != 0)
		{
		  str += this->GetRHS()->ToString();
		}
	      else
		{
		  str += "(null)";
		}
	      str += " )";
	    }
	  break;
	case Sentence::T_LEFT_PAREN:
	  str += "( ";
	  break;
	case Sentence::T_RIGHT_PAREN:
	  str += " )";
	  break;
	case Sentence::T_FORALL:
	  str += "forall ";
	  if(this->GetSymbol() != 0)
	    {
	      str += this->GetSymbol()->GetName();
	      str += " ";
	    }
	  else
	    {
	      str += "(null) ";
	    }
	  if(this->GetRHS() != 0)
	    {
	      str += "( ";
	      str += this->GetRHS()->ToString();
	      str += " )";
	    }
	  else
	    {
	      str += "(null)";
	    }
	  break;
	case Sentence::T_EXISTS:
	  str += "exists ";
	  if(this->GetSymbol() != 0)
	    {
	      str += this->GetSymbol()->GetName();
	      str += " ";
	    }
	  else
	    {
	      str += "(null) ";
	    }
	  if(this->GetRHS() != 0)
	    {
	      str += "( ";
	      str += this->GetRHS()->ToString();
	      str += " )";
	    }
	  else
	    {
	      str += "(null)";
	    }
	  break;
	default:
	  str += "ERROR";
	  std::cerr << "Bad type in Sentence::ToString(): " << type << std::endl;
	  //{ int *i = 0; (*i) = 0; }
	  break;
	}
      return str;
    }

    bool Sentence::ContainsConnective() const
    {
      switch(type)
	{
	case T_TRUE:
	case T_FALSE:
	case T_SYMBOL:
	  return false;
	  break;
	case T_UNARY:
	  if(op == O_NOT)
	    {
	      return true;
	    }
	  else
	    {
	      return rhs->ContainsConnective();
	    }
	  break;
	case T_BINARY:
	  if(op == O_AND || op == O_OR || op == O_IMPLIES || op == O_IFF)
	    {
	      return true;
	    }
	  else
	    {
	      return lhs->ContainsConnective() || rhs->ContainsConnective();
	    }
	  break;
	default:
	  std::cerr << "Unexptected type in ContainsConnective(): " << type << std::endl;
	  { int *i = 0; (*i) = 0; }
	}
      return false;
    }
    
    std::ostream & operator<<(std::ostream &os, const Sentence &s)
    {
      os << s.ToString();
      return os;
    }

    // Display a list of Sentences
    std::ostream &operator<<(std::ostream &os, const std::list<Sentence *> &sentences)
    {
      std::list<Sentence *>::const_iterator op         = sentences.begin();
      std::list<Sentence *>::const_iterator op_end     = sentences.end();
      os << "Sentences:" << std::endl;
      while(op != op_end)
	{
	  os << *(*op) << "  ";
	  op++;
	}
      os << std::endl;
      return os;
    }

  }
}
