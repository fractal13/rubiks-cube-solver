#include <ai_pl.h>
#include <sstream>

namespace ai
{
  namespace PL
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
      op     = O_NONE;
      rhs    = 0;
    }
    Sentence::Sentence(SentenceType type_in)
    {
      if( type_in == T_LEFT_PAREN || type_in == T_RIGHT_PAREN ) {
        type   = type_in; // should be T_LEFT_PAREN or T_RIGHT_PAREN
      } else {
        std::stringstream ss;
        ss << "Invalid SentenceType for this constructor: " << type_in;
        throw std::domain_error( ss.str( ) );
      }
      symbol = 0;
      lhs    = 0;
      op     = O_NONE;
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
    Sentence::Sentence(Symbol *symbol_in)
    {
      type   = T_SYMBOL;
      symbol = symbol_in;
      lhs    = 0;
      op     = O_NONE;
      rhs    = 0;
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
      op     = O_NONE;
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

    Symbol::State Sentence::Evaluate() const
    {
      switch(type)
        {
        case T_TRUE:
          return Symbol::KNOWN_TRUE;
          break;
        case T_FALSE:
          return Symbol::KNOWN_FALSE;
          break;
        case T_SYMBOL:
          return symbol->GetValue();
          break;
        case T_UNARY:
          {
            Symbol::State t = rhs->Evaluate();
            if(t == Symbol::UNKNOWN)
              {
                return t;
              }
            switch(op)
              {
              case O_NOT:
                if(t == Symbol::KNOWN_TRUE)
                  {
                    return Symbol::KNOWN_FALSE;
                  }
                else if(t == Symbol::KNOWN_FALSE)
                  {
                    return Symbol::KNOWN_TRUE;
                  }
                else
                  {
                    std::cerr << "Unknown Symbol Value." << std::endl;
                    { int *i = 0; (*i) = 0; }
                    return Symbol::UNKNOWN;
                  }
                break;
              default:
                std::cerr << "Unknown Sentence Operator." << std::endl;
                { int *i = 0; (*i) = 0; }
                return Symbol::UNKNOWN;
                break;
              }
          }
          break;
        case T_BINARY:
          return EvaluateBinary();
          break;
        default:
          std::cerr << "Unknown Sentence Type." << std::endl;
          { int *i = 0; (*i) = 0; }
          return Symbol::UNKNOWN;
          break;
        }
    }

    Symbol::State Sentence::EvaluateBinary() const
    {
      Symbol::State t1 = lhs->Evaluate();
      Symbol::State t2 = rhs->Evaluate();
      switch(op)
        {
        case O_AND:
          return EvaluateConjunct(t1, t2);
          break;
        case O_OR:
          return EvaluateDisjunct(t1, t2);
          break;
        case O_IMPLIES:
          return EvaluateImplication(t1, t2);
          break;
        case O_IFF:
          return EvaluateBiconditional(t1, t2);
          break;
        default:
          std::cerr << "Unknown Sentence Operator." << std::endl;
          { int *i = 0; (*i) = 0; }
          return Symbol::UNKNOWN;
          break;
        }
    }

    Symbol::State Sentence::EvaluateConjunct(Symbol::State t1, Symbol::State t2)
    {
      if((t1 == Symbol::KNOWN_TRUE) && (t2 == Symbol::KNOWN_TRUE))
        {
          return Symbol::KNOWN_TRUE;
        }
      else if((t1 == Symbol::KNOWN_FALSE) || (t2 == Symbol::KNOWN_FALSE))
        {
          return Symbol::KNOWN_FALSE;
        }
      else
        {
          return Symbol::UNKNOWN;
        }
    }

    Symbol::State Sentence::EvaluateDisjunct(Symbol::State t1, Symbol::State t2)
    {
      if((t1 == Symbol::KNOWN_TRUE) || (t2 == Symbol::KNOWN_TRUE))
        {
          return Symbol::KNOWN_TRUE;
        }
      else if((t1 == Symbol::KNOWN_FALSE) && (t2 == Symbol::KNOWN_FALSE))
        {
          return Symbol::KNOWN_FALSE;
        }
      else
        {
          return Symbol::UNKNOWN;
        }
    }

    Symbol::State Sentence::EvaluateImplication(Symbol::State t1, Symbol::State t2)
    {
      if(t1 == Symbol::KNOWN_FALSE)
        {
          return Symbol::KNOWN_TRUE;
        }
      else if((t1 == Symbol::KNOWN_TRUE) && (t2 == Symbol::KNOWN_FALSE))
        {
          return Symbol::KNOWN_FALSE;
        }
      else if((t1 == Symbol::KNOWN_TRUE) && (t2 == Symbol::KNOWN_TRUE))
        {
          return Symbol::KNOWN_TRUE;
        }
      else
        {
          return Symbol::UNKNOWN;
        }
    }

    Symbol::State Sentence::EvaluateBiconditional(Symbol::State t1, Symbol::State t2)
    {
      if((t1 == Symbol::KNOWN_FALSE) && (t2 == Symbol::KNOWN_FALSE))
        {
          return Symbol::KNOWN_TRUE;
        }
      else if((t1 == Symbol::KNOWN_FALSE) && (t2 == Symbol::KNOWN_TRUE))
        {
          return Symbol::KNOWN_FALSE;
        }
      else if((t1 == Symbol::KNOWN_TRUE) && (t2 == Symbol::KNOWN_FALSE))
        {
          return Symbol::KNOWN_FALSE;
        }
      else if((t1 == Symbol::KNOWN_TRUE) && (t2 == Symbol::KNOWN_TRUE))
        {
          return Symbol::KNOWN_TRUE;
        }
      else
        {
          return Symbol::UNKNOWN;
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


    std::string Sentence::ToString() const
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
        case Sentence::O_NONE:
          op_str = "";
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
          break;
        case Sentence::T_LEFT_PAREN:
          str += "( ";
          break;
        case Sentence::T_RIGHT_PAREN:
          str += " )";
          break;
        default:
          str += "ERROR";
          std::cerr << "Bad type in Sentence::ToString(): " << type << std::endl;
          { int *i = 0; (*i) = 0; }
          break;
        }
      return str;
    }


    std::ostream & operator<<(std::ostream &os, const Sentence &s)
    {
      os << s.ToString();
      return os;
    }

    /*
     * Convert sentence to Conjunctive Normal Form
     */
    void Sentence::CNF()
    {
      while(EliminateBiconditional(this)) {/*empty*/}
      while(EliminateImplication(this)) {/*empty*/}
      MoveNotIn(this, false);
      while(DistributeOr(this)) {/*empty*/}
    }

    bool Sentence::EliminateBiconditional(Sentence *s_in)
    {
      bool rval = false;
      if((s_in->type == T_BINARY) && (s_in->op == O_IFF))
        {
          Sentence *lhs_temp = s_in->lhs;
          Sentence *rhs_temp = s_in->rhs;

          s_in->lhs = new Sentence(new Sentence(*lhs_temp), O_IMPLIES, new Sentence(*rhs_temp));
          s_in->rhs = new Sentence(rhs_temp, O_IMPLIES, lhs_temp);
          s_in->op = O_AND;
          rval = true;
        }
      if(s_in->type == T_BINARY)
        {
          if(EliminateBiconditional(s_in->lhs))
            {
              rval = true;
            }
        }
      if(s_in->type == T_BINARY || s_in->type == T_UNARY)
        {
          if(EliminateBiconditional(s_in->rhs))
            {
              rval = true;
            }
        }
      return rval;
    }

    bool Sentence::EliminateImplication(Sentence *s_in)
    {
      bool rval = false;
      if((s_in->type == T_BINARY) && (s_in->op == O_IMPLIES))
        {
          Sentence *lhs_temp = s_in->lhs;
          s_in->lhs = new Sentence(O_NOT, lhs_temp);
          s_in->op = O_OR;
          rval = true;
        }
      if(s_in->type == T_BINARY)
        {
          if(EliminateImplication(s_in->lhs))
            {
              rval = true;
            }
        }
      if(s_in->type == T_BINARY || s_in->type == T_UNARY)
        {
          if(EliminateImplication(s_in->rhs))
            {
              rval = true;
            }
        }
      return rval;
    }

    void Sentence::MoveNotIn(Sentence *s_in, bool have_not)
    {
      switch(s_in->type)
        {
        case T_TRUE:
          if(have_not)
            {
              s_in->type = T_FALSE;
            }
          break;
        case T_FALSE:
          if(have_not)
            {
              s_in->type = T_TRUE;
            }
          break;
        case T_SYMBOL:
          if(have_not)
            {
              s_in->rhs  = new Sentence(*s_in);
              s_in->type = T_UNARY;
              s_in->op   = O_NOT;
            }
          break;
        case T_UNARY:
          {
            switch(s_in->op)
              {
              case O_NOT:
                {
                  if(have_not)
                    {
                      MoveNotIn(s_in->rhs, false);
                    }
                  else
                    {
                      MoveNotIn(s_in->rhs, true);
                    }
                  Sentence *rhs_temp = s_in->rhs;
                  s_in->type   = rhs_temp->type;
                  s_in->symbol = rhs_temp->symbol;
                  s_in->lhs    = rhs_temp->lhs;
                  s_in->op     = rhs_temp->op;
                  s_in->rhs    = rhs_temp->rhs;

                  rhs_temp->symbol = 0;
                  rhs_temp->lhs    = 0;
                  rhs_temp->rhs    = 0;
                  delete rhs_temp;
                }
                break;
              default:
                std::cerr << "Unknown Sentence Operator." << std::endl;
                { int *i = 0; (*i) = 0; }
                break;
              }
          }
          break;
        case T_BINARY:
          {
            switch(s_in->op)
              {
              case O_OR:
                if(have_not)
                  {
                    s_in->op = O_AND;
                    MoveNotIn(s_in->lhs, true);
                    MoveNotIn(s_in->rhs, true);
                  }
                else
                  {
                    MoveNotIn(s_in->lhs, false);
                    MoveNotIn(s_in->rhs, false);
                  }
                break;
              case O_AND:
                if(have_not)
                  {
                    s_in->op = O_OR;
                    MoveNotIn(s_in->lhs, true);
                    MoveNotIn(s_in->rhs, true);
                  }
                else
                  {
                    MoveNotIn(s_in->lhs, false);
                    MoveNotIn(s_in->rhs, false);
                  }
                break;
              default:
                std::cerr << "Unknown Sentence Operator." << std::endl;
                { int *i = 0; (*i) = 0; }
                break;
              }
          }
          break;
        default:
          std::cerr << "Unknown Sentence Type." << std::endl;
          { int *i = 0; (*i) = 0; }
          break;
        }
    }
    bool Sentence::DistributeOr(Sentence *s_in)
    {
      bool rval = false;
      if(s_in->type == T_UNARY)
        {
          if(DistributeOr(s_in->rhs))
            {
              rval = true;
            }
        }
      else if(s_in->type == T_BINARY)
        {
          if(s_in->op == O_OR)
            {
              if(DistributeOr(s_in->lhs))
                {
                  rval = true;
                }
              if(DistributeOr(s_in->rhs))
                {
                  rval = true;
                }
              if((s_in->rhs->type == T_BINARY) && (s_in->rhs->op == O_AND))
                { // a | (b & c)  --> (a | b) & (a' | c)
                  Sentence *new_lhs  = new Sentence(s_in->lhs, O_OR, s_in->rhs->lhs);
                  Sentence *lhs_temp = new Sentence(*(s_in->lhs));

                  s_in->rhs->op  = O_OR;
                  s_in->rhs->lhs = lhs_temp;

                  s_in->lhs      = new_lhs;
                  s_in->op       = O_AND;
                  rval = true;
                }
              else if((s_in->lhs->type == T_BINARY) && (s_in->lhs->op == O_AND))
                { // (a & b) | c --> (a | c') & (b | c)
                  Sentence *new_rhs  = new Sentence(s_in->lhs->rhs, O_OR, s_in->rhs);
                  Sentence *rhs_temp = new Sentence(*(s_in->rhs));

                  s_in->lhs->op  = O_OR;
                  s_in->lhs->rhs = rhs_temp;

                  s_in->rhs      = new_rhs;
                  s_in->op       = O_AND;
                  rval = true;
                }
            }
          else if(s_in->op == O_AND)
            {
              if(DistributeOr(s_in->lhs))
                {
                  rval = true;
                }
              if(DistributeOr(s_in->rhs))
                {
                  rval = true;
                }
            }
          else
            {
              std::cerr << "Unknown Sentence Operator." << std::endl;
              { int *i = 0; (*i) = 0; }
            }
        }
      return rval;
    }

    void Sentence::SplitClausesAux(Sentence *s_in, std::vector<Sentence *> &clauses)
    {
      if(s_in->type == T_BINARY)
        {
          if(s_in->op == O_OR)
            {
              SplitClausesAux(s_in->lhs, clauses);
              SplitClausesAux(s_in->rhs, clauses);
            }
          else if(s_in->op == O_AND)
            {
              SplitClauses(s_in->lhs, clauses);
              SplitClauses(s_in->rhs, clauses);
              s_in->symbol = 0;
              s_in->lhs = 0;
              s_in->rhs = 0;
              delete s_in;
            }
          else
            {
              std::cerr << "Unknown Sentence Operator." << std::endl;
              { int *i = 0; (*i) = 0; }
            }
        }
      else if(s_in->type == T_UNARY)
        {
          SplitClausesAux(s_in->rhs, clauses);
        }
      else if(s_in->type == T_SYMBOL)
        {
          // Do Nothing, is OK
        }
      else
        { // Other ?
          std::cerr << "Unexpected Sentenc Type: " << (*s_in) << std::endl;
          { int *i = 0; (*i) = 0; }
        }

    }

    void Sentence::SplitClauses(Sentence *s_in, std::vector<Sentence *> &clauses)
    {
      if(s_in->type == T_BINARY)
        {
          if(s_in->op == O_OR)
            {
              SplitClausesAux(s_in->lhs, clauses);
              SplitClausesAux(s_in->rhs, clauses);
              clauses.push_back(s_in);
            }
          else if(s_in->op == O_AND)
            {
              SplitClauses(s_in->lhs, clauses);
              SplitClauses(s_in->rhs, clauses);
              s_in->symbol = 0;
              s_in->lhs = 0;
              s_in->rhs = 0;
              delete s_in;
            }
          else
            {
              std::cerr << "Unknown Sentence Operator." << std::endl;
              { int *i = 0; (*i) = 0; }
            }
        }
      else if(s_in->type == T_UNARY)
        {
          SplitClausesAux(s_in->rhs, clauses);
          clauses.push_back(s_in);
        }
      else
        { // Other ?
          clauses.push_back(s_in);
        }
    }

    void Sentence::FindPureAux(Sentence *s_in,
                               std::vector<Symbol *> &asserted,
                               std::vector<Symbol *> &negated)
    {
      if((s_in->type == T_BINARY) && (s_in->op == O_OR))
        {
          FindPureAux(s_in->lhs, asserted, negated);
          FindPureAux(s_in->rhs, asserted, negated);
        }
      else if((s_in->type == T_UNARY) && (s_in->op == O_NOT))
        {
          if(s_in->rhs->symbol->GetValue() == Symbol::UNKNOWN)
            {
              negated.push_back(s_in->rhs->symbol);
            }
        }
      else if(s_in->type == T_SYMBOL)
        {
          if(s_in->symbol->GetValue() == Symbol::UNKNOWN)
            {
              asserted.push_back(s_in->symbol);
            }
        }
      else
        {
          std::cerr << "Unexpected Sentence Type." << std::endl;
          { int *i = 0; (*i) = 0; }
        }
    }

    void Sentence::FindPure(std::vector<Symbol *> &asserted,
                            std::vector<Symbol *> &negated)
    {
      FindPureAux(this, asserted, negated);
    }

    bool Sentence::IsUnitClause(Symbol *&sym_out)
    {
      std::vector<Symbol *> asserted;
      std::vector<Symbol *> negated;

      FindPureAux(this, asserted, negated);
      if((asserted.size() + negated.size()) == 1)
        {
          if(asserted.size() == 1)
            {
              sym_out = asserted[0];
              sym_out->SetValue(Symbol::KNOWN_TRUE);
            }
          else if(negated.size() == 1)
            {
              sym_out = negated[0];
              sym_out->SetValue(Symbol::KNOWN_FALSE);
            }
          else
            {
              std::cerr << "Unexpected Chain of Events." << std::endl;
              { int *i = 0; (*i) = 0; }
            }
          return true;
        }
      return false;
    }
    
    bool Sentence::operator==( const Sentence& rhs ) const {
      if( type != rhs.type ) {
        return false;
      }
      
      if( op != rhs.op ) {
        return false;
      }
      
      if( symbol == 0 && rhs.symbol == 0 ) {
        // both 0, OK
      } else if( symbol != 0 && rhs.symbol != 0 ) {
        if( !( *(symbol) == *(rhs.symbol) ) ) {
          return false;
        }
      } else {
        // exactly 1 is 0, not OK
        return false;
      }
      
      if( lhs == 0 && rhs.lhs == 0 ) {
        // both 0, OK
      } else if( lhs != 0 && rhs.lhs != 0 ) {
        if( !( *(lhs) == *(rhs.lhs) ) ) {
          return false;
        }
      } else {
        // exactly 1 is 0, not OK
        return false;
      }

      if( this->rhs == 0 && rhs.rhs == 0 ) {
        // both 0, OK
      } else if( this->rhs != 0 && rhs.rhs != 0 ) {
        if( !( *(this->rhs) == *(rhs.rhs) ) ) {
          return false;
        }
      } else {
        // exactly 1 is 0, not OK
        return false;
      }

      return true;
    }
    

  }
}
