/*
 * This is a recursive decent parser for a modified first order logic grammer 
 * that is based on the grammar specified in AIMA 2nd Ed. by Russell and 
 * Norvig on page 247.  The modified grammar is reproduced below.
 *
 * The parser will give lots of std::cerr messages when it encounters
 * a malformed sentence, but will still give back a parsed sentence.
 * This could be viewed as a bug.  (FIXME:  A solution may be to
 * no-whitespace comparison between the input string and the ToString()
 * of the output sentence.)
 *
 * May 25, 2009 - CGL
 *
 *
 *
 *
 * Beware that parentheses are important and must be used to enforce
 * order of operations.
 *
 * Modifications:
 * 1- Added ( Sentence ) as a valid sentence syntax.
 * 2- Predicates, Functions, Variables and Constants all have the
 *    same allowable syntax.  Predicates and functions are found
 *    by context because of the following parenthesis.  Predicates
 *    are distinguished by context.  They are only found at the
 *    base of an atomic sentence.
 *   
 *    Variables are distinguished from constants by being nested
 *    inside of a quantifier that uses the variable.  This forces
 *    the sentences to have unique variable names for nested 
 *    quantifiers.
 * 3- Added the TermList specification to match the implementation
 *    of the parser.
 * 4- Renamed the connectives using easily typed symbols.
 *
 * Sentence: AtomicSentence
 *           ( Sentence )
 *           ( Sentence Connective Sentence )
 *           Quantifier Variable,... Sentence
 *           !Sentence
 *
 * AtomicSentence: Predicate(TermList)
 *                 Term = Term
 *
 * TermList:   Term,...
 *
 * Term: Function(TermList)
 *       Constant
 *       Variable
 *
 * Connective: =>
 *             &
 *             |
 *             <=>
 *
 * Quantifier: forall
 *             exists
 *
 * Constant: [A-Za-z][A-Za-z0-9_]*
 *
 * Variable: [A-Za-z][A-Za-z0-9_]*
 *
 * Predicate: [A-Za-z][A-Za-z0-9_]*
 *
 * Function: [A-Za-z][A-Za-z0-9_]*
 *
 */


#include <ai_fol.h>

#define DEBUG_PARSE 0

namespace ai
{
  namespace FOL
  {
    Parser::Parser()
    {
    }
    Parser::~Parser()
    {
    }

    Sentence *Parser::Parse(const std::string &str_in)
    {
      std::list<Token> tokens;
      Tokenize(str_in, tokens);
#if DEBUG_PARSE
      std::cout << tokens << std::endl;
#endif
      std::list<Token>::const_iterator curr_token = tokens.begin();
      Sentence *s = pSentence(curr_token);
#if DEBUG_PARSE
      if(s)
	{
	  std::cout << "Parse: " << str_in << std::endl;
	  std::cout << "     : " << (*s) << std::endl;
	}
#endif
      return s;
    }
    
    Sentence *Parser::pSentence(std::list<Token>::const_iterator &curr_token)
    {
      /* Sentence: AtomicSentence
       *           ( Sentence )
       *           ( Sentence Connective Sentence )
       *           Quantifier Variable,... Sentence
       *           !Sentence
       */

#if DEBUG_PARSE
      std::cout << "pSentence: " << (*curr_token) << std::endl;
#endif
      /* ( Sentence Connective Sentence ) is detected by a '(' */
      if(curr_token->type == TOKEN_LEFT_PAREN)
	{
	  curr_token++; // pass over the '('
	  Sentence *lhs = pSentence(curr_token);
	  if(!lhs)
	    {
	      std::cerr << "Parser::pSentence(): Error parsing connective, missing LHS sentence." << std::endl;
	    }
	  Sentence *op  = pConnective(curr_token);
	  if(!op)
	    { /* OK.  This represents ( Sentence ) */
	      
	      if(curr_token->type != TOKEN_RIGHT_PAREN)
		{
		  std::cerr << "Parser::pSentence(): 1 Error parsing connective, missing right paren." << std::endl;
		  std::cerr << "\t" << (*curr_token) << std::endl;
		}
	      else
		{ // pass over the ')'
		  curr_token++;
		}
	      return lhs;
	    }
	  Sentence *rhs = pSentence(curr_token);
	  if(!rhs)
	    {
	      std::cerr << "Parser::pSentence(): Error parsing connective, missing RHS sentence." << std::endl;
	    }
	  if(curr_token->type != TOKEN_RIGHT_PAREN)
	    {
	      std::cerr << "Parser::pSentence(): 2 Error parsing connective, missing right paren." << std::endl;
	      std::cerr << "\t" << (*curr_token) << std::endl;
	      std::cerr << "\t" << (*lhs) << ":" << (*op) << ":" << (*rhs) << std::endl;
	    }
	  else
	    { // pass over the ')'
	      curr_token++;
	    }
	  
	  Sentence *s = new Sentence(lhs, op->GetOperator(), rhs);
	  delete op;
	  
#if DEBUG_PARSE
	  std::cout << "pSentence::Sentence::s " << (*s) << std::endl;
#endif
	  return s;
	}
      /* AtomicSentence is detected by a Predicate/Function symbol token
       */
      else if(curr_token->type == TOKEN_SYMBOL)
	{
	  Sentence *lhs = pAtomicSentence(curr_token);
	  if(!lhs)
	    {
	      std::cerr << "Parser::pSentence(): Error parsing atomic sentence, missing LHS sentence." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
#if DEBUG_PARSE
	  else
	    {
	      std::cout << "pSentence::AtomicSentence::LHS " << (*lhs) << std::endl;
	    }
#endif
	  return lhs;

	}
      /* Quantifier Variable,... Sentence is detected by a FORALL or EXISTS token */
      else if(curr_token->type == TOKEN_FORALL || curr_token->type == TOKEN_EXISTS)
	{
	  /*
	   * forall x sentence(x)
	   * exists x sentence(x)
	   */
	  Sentence::SentenceType type;
	  if(curr_token->type == TOKEN_FORALL)
	    {
	      type = Sentence::T_FORALL;
	    }
	  else
	    {
	      type = Sentence::T_EXISTS;
	    }
	  std::list<Token>::const_iterator next_token = curr_token; next_token++;
	  if(next_token->type != TOKEN_SYMBOL)
	    {
	      std::cerr << "Parser::pSentence(): Error parsing quantifier, expected variable symbol." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	      return 0;
	    }
	  Symbol *variable = new Symbol(next_token->symbol);
	  variable->SetType(Symbol::TYPE_VARIABLE);
	  curr_token ++; // skip quantifier
	  curr_token ++; // skip variable
	  Sentence *rhs = pSentence(curr_token);
	  if(!rhs)
	    {
	      std::cerr << "Parser::pSentence(): Error parsing quantifier, missing RHS sentence." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
	  else
	    {
	      rhs->SetVariable(variable);
	    }
	  Sentence *s = new Sentence(type, variable, rhs);
#if DEBUG_PARSE
	  std::cout << "pSentence::Quantifier::s " << (*s) << std::endl;
#endif
	  return s;
	}
      /* !Sentence is detected by the NOT token */
      else if(curr_token->type == TOKEN_NOT)
	{
	  curr_token++; // pass over the '!'
	  Sentence *rhs = pSentence(curr_token);
	  if(!rhs)
	    {
	      std::cerr << "Parser::pSentence(): Error parsing not sentence, missing RHS sentence." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
	  Sentence *s = new Sentence(Sentence::O_NOT, rhs);
#if DEBUG_PARSE
	  std::cout << "pSentence::Not::s " << (*s) << std::endl;
#endif
	  return s;
	}
      else
	{ 
	  std::cerr << "Parser::pSentence(): Error parsing sentence, unexpected condition." << std::endl;
	  std::cerr << "\t:" << (*curr_token) << std::endl;
	}
      
      return 0;
    }
    
    Sentence *Parser::pAtomicSentence(std::list<Token>::const_iterator &curr_token)
    {
      /* AtomicSentence: Predicate(TermList)
       *                 Term = Term
       */

#if DEBUG_PARSE
      std::cout << "pAtomicSentence: " << (*curr_token) << std::endl;
#endif

      /* Predicate(TermList) is detected by SYMBOL followed by '(' */
      std::list<Token>::const_iterator next_token = curr_token; next_token++;
      if(curr_token->type == TOKEN_SYMBOL &&
	 next_token->type == TOKEN_LEFT_PAREN)
	{
	  Symbol *symbol = new Symbol(curr_token->symbol);
	  symbol->SetType(Symbol::TYPE_PREDICATE);
	  curr_token++; // skip symbol
	  curr_token++; // skip '('
	  Sentence *args = pTermList(curr_token);
	  if(curr_token->type != TOKEN_RIGHT_PAREN)
	    {
	      std::cerr << "Parser::pAtomicSentence(): Error parsing term list, missing right paren." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
	  else
	    { // pass over the ')'
	      curr_token++;
	    }
	  symbol->SetArity(CountArity(args));

	  if(curr_token->type == TOKEN_EQUAL)
	    { /* this is actually a function_term = term */
	      symbol->SetType(Symbol::TYPE_FUNCTION);
	      Sentence *lhs = new Sentence(symbol, args);
	      
	      curr_token++; // skip the '='
	      Sentence *rhs = pTerm(curr_token);
	      if(!rhs)
		{
		  std::cerr << "Parser::pAtomicSentence(): Error parsing term = term, missing RHS sentence." << std::endl;
		  std::cerr << "\t:" << (*curr_token) << std::endl;
		}
	      
	      Sentence *s = new Sentence(lhs, Sentence::O_EQUAL, rhs);
	      return s;
	    }
	  else
	    {
	      Sentence *s = new Sentence(symbol, args);
#if DEBUG_PARSE
	      std::cout << "pAtomicSentence::Predicate::s " << (*s) << std::endl;
#endif
	      return s;
	    }
	}
      /* Term = Term is detected by a leading symbol */
      else if(curr_token->type == TOKEN_SYMBOL)
	{
	  Sentence *lhs = pTerm(curr_token);
	  if(!lhs)
	    {
	      std::cerr << "Parser::pAtomicSentence(): Error parsing term = term, missing LHS sentence." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
	  if(curr_token->type == TOKEN_EQUAL)
	    {
	      curr_token++;
	    }
	  else
	    {
	      std::cerr << "Parser::pAtomicSentence(): Error parsing term = term, missing '='." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
	  Sentence *rhs = pTerm(curr_token);
	  if(!rhs)
	    {
	      std::cerr << "Parser::pAtomicSentence(): Error parsing term = term, missing RHS sentence." << std::endl;
	      std::cerr << "\t:" << (*curr_token) << std::endl;
	    }
	  
	  Sentence *s = new Sentence(lhs, Sentence::O_EQUAL, rhs);
	  return s;
	}
      else
	{ 
	  std::cerr << "Parser::pAtomicSentence(): Error parsing atomic sentence, unexpected condition." << std::endl;
	  std::cerr << "\t:" << (*curr_token) << std::endl;
	}
      return 0;
    }
    
    Sentence *Parser::pConnective(std::list<Token>::const_iterator &curr_token)
    {
      /* Connective: =>
       *             &
       *             |
       *             <=>
       */

#if DEBUG_PARSE
      std::cout << "pConnective: " << (*curr_token) << std::endl;
#endif
      Sentence *s = 0;
      if(curr_token->type == TOKEN_IMPLIES)
	{
	  curr_token++;
	  s = new Sentence(0, Sentence::O_IMPLIES, 0);
	}
      else if(curr_token->type == TOKEN_AND)
	{
	  curr_token++;
	  s = new Sentence(0, Sentence::O_AND, 0);
	}
      else if(curr_token->type == TOKEN_OR)
	{
	  curr_token++;
	  s = new Sentence(0, Sentence::O_OR, 0);
	}
      else if(curr_token->type == TOKEN_IFF)
	{
	  curr_token++;
	  s = new Sentence(0, Sentence::O_IFF, 0);
	}
      /* We do not show this because this condition regularly occurs when
       * we have a sentence wrapped in parens.
      else
	{ 
	  std::cerr << "Parser::pConnective(): Error parsing connective, unexpected condition." << std::endl;
	  std::cerr << "\t:" << (*curr_token) << std::endl;
	}
      */
      
#if DEBUG_PARSE
      if(s)
	{
	  std::cout << "pConnective::s " << (*s) << std::endl;
	}
#endif
      return s;
    }

    Sentence *Parser::pTermList(std::list<Token>::const_iterator &curr_token)
    {
      /* TermList: Term,...
       */
      
#if DEBUG_PARSE
      std::cout << "pTermList: " << (*curr_token) << std::endl;
#endif

      if(curr_token->type == TOKEN_SYMBOL)
	{
	  Sentence *lhs = pTerm(curr_token);
	  if(!lhs)
	    {
	      std::cerr << "Parser::pTermList(): Error parsing term, missing LHS term." << std::endl;
	    }
	  if(curr_token->type == TOKEN_COMMA)
	    {
	      curr_token++;
	      Sentence *rhs = pTermList(curr_token);
	      if(!rhs)
		{
		  std::cerr << "Parser::pTermList(): Error parsing term, missing RHS termlist." << std::endl;
		  std::cerr << "\t:" << (*curr_token) << std::endl;
		}
	      Sentence *s = new Sentence(lhs, Sentence::O_COMMA, rhs);
	      return s;
	    }
	  else /* single term */
	    {
	      return lhs;
	    }
	}
      else
	{
	  std::cerr << "Parser::pTermList(): Error parsing term list, unexpected condition." << std::endl;
	  std::cerr << "\t:" << (*curr_token) << std::endl;
	}
      
      return 0;
    }
    
    Sentence *Parser::pTerm(std::list<Token>::const_iterator &curr_token)
    {
      /* Term: Function(TermList)
       *       Constant
       *       Variable
       */
      
#if DEBUG_PARSE
      std::cout << "pTerm: " << (*curr_token) << std::endl;
#endif

      if(curr_token->type == TOKEN_SYMBOL)
	{
	  std::list<Token>::const_iterator next_token = curr_token; next_token++;
	  if(next_token->type == TOKEN_LEFT_PAREN)
	    {
	      Symbol *symbol = new Symbol(curr_token->symbol);
	      symbol->SetType(Symbol::TYPE_FUNCTION);
	      curr_token++; // skip symbol
	      curr_token++; // skip '('
	      Sentence *args = pTermList(curr_token);
	      if(curr_token->type != TOKEN_RIGHT_PAREN)
		{
		  std::cerr << "Parser::pTerm(): Error parsing term list, missing right paren." << std::endl;
		  std::cerr << "\t:" << (*curr_token) << std::endl;
		}
	      else
		{ // pass over the ')'
		  curr_token++;
		}
	      symbol->SetArity(CountArity(args));
	      Sentence *s = new Sentence(symbol, args);
	      return s;
	    }
	  else
	    {
	      Symbol *symbol = new Symbol(curr_token->symbol);
	      symbol->SetType(Symbol::TYPE_CONSTANT);
	      curr_token++; // skip symbol
	      Sentence *s = new Sentence(symbol);
	      return s;
	    }
	}
      else
	{
	  std::cerr << "Parser::pTerm(): Error parsing term list, unexpected condition." << std::endl;
	  std::cerr << "\t:" << (*curr_token) << std::endl;
	}

      return 0;
    }
    
    int Parser::CountArity(const Sentence *s_in) const
    {
      if((s_in) &&
	 (s_in->GetType() == Sentence::T_BINARY) &&
	 (s_in->GetOperator() == Sentence::O_COMMA))
	{
	  return 1 + CountArity(s_in->GetRHS());
	}
      return 1;
    }
    
    void Parser::Tokenize(const std::string &s_in, std::list<Token> &tokens)
    {
      std::string::const_iterator it     = s_in.begin();
      std::string::const_iterator it_end = s_in.end();
      
      std::string::const_iterator token_start;
      std::string::const_iterator token_end;
      
      while(it != it_end)
	{
	  if(isspace(*it))
	    { // remove space
	      it ++;
	    }
	  else if(isalpha(*it))
	    { // must be a symbol
	      token_start = it;
	      token_end   = it;
	      while((it != it_end) && (isalnum(*it) || ((*it) == '_')))
		{
		  it ++;
		  token_end = it; // token_end is really 1 past the end.
		}
	      std::string sym(token_start, token_end);
	      /* keywords */
	      if(sym == "forall")
		{
		  Token t(TOKEN_FORALL);
		  tokens.push_back(t);
		}
	      else if(sym == "exists")
		{
		  Token t(TOKEN_EXISTS);
		  tokens.push_back(t);
		}
	      else
		{
		  Token t(TOKEN_SYMBOL, sym);
		  tokens.push_back(t);
		}
	    }
	  else if(*it == '(')
	    { // 
	      Token t(TOKEN_LEFT_PAREN);
	      tokens.push_back(t);
	      it++;
	    }
	  else if(*it == ')')
	    { //
	      Token t(TOKEN_RIGHT_PAREN);
	      tokens.push_back(t);
	      it++;
	    }
	  else if(*it == '!')
	    { // 
	      Token t(TOKEN_NOT);
	      tokens.push_back(t);
	      it++;
	    }
	  else if(*it == '&')
	    { // 
	      Token t(TOKEN_AND);
	      tokens.push_back(t);
	      it++;
	    }
	  else if(*it == '|')
	    { // 
	      Token t(TOKEN_OR);
	      tokens.push_back(t);
	      it++;
	    }
	  else if((*it == '=') && ((it+1) != it_end) && (*(it+1) == '>'))
	    { // 
	      Token t(TOKEN_IMPLIES);
	      tokens.push_back(t);
	      it+=2;
	    }
	  else if((*it == '<') && ((it+1) != it_end) && (*(it+1) == '=') && ((it+2) != it_end) && (*(it+2) == '>'))
	    { // 
	      Token t(TOKEN_IFF);
	      tokens.push_back(t);
	      it+=3;
	    }
	  else if(*it == ',')
	    { // 
	      Token t(TOKEN_COMMA);
	      tokens.push_back(t);
	      it++;
	    }
	  else if(*it == '=')
	    { // 
	      Token t(TOKEN_EQUAL);
	      tokens.push_back(t);
	      it++;
	    }
	  else
	    { // skipping for now
	      std::cout << "ERROR! Skipping unknown character in Tokenize(): " << *it << std::endl;
	      it++;
	    }
	}
#if DEBUG_PARSE
      std::cout << tokens << std::endl;
#endif
    }

    std::ostream &operator<<(std::ostream &os, const Parser::Token &t)
    {
      switch(t.type)
	{
	case Parser::TOKEN_MIN:
	  os << "TOKEN_MIN";
	  break;
	case Parser::TOKEN_SYMBOL:
	  os << "TOKEN_SYMBOL " << t.symbol;
	  break;
	case Parser::TOKEN_LEFT_PAREN:
	  os << "TOKEN_LEFT_PAREN";
	  break;
	case Parser::TOKEN_RIGHT_PAREN:
	  os << "TOKEN_RIGHT_PAREN";
	  break;
	case Parser::TOKEN_NOT:
	  os << "TOKEN_NOT";
	  break;
	case Parser::TOKEN_AND:
	  os << "TOKEN_AND";
	  break;
	case Parser::TOKEN_OR:
	  os << "TOKEN_OR";
	  break;
	case Parser::TOKEN_IMPLIES:
	  os << "TOKEN_IMPLIES";
	  break;
	case Parser::TOKEN_IFF:
	  os << "TOKEN_IFF";
	  break;
	case Parser::TOKEN_COMMA:
	  os << "TOKEN_COMMA";
	  break;
	case Parser::TOKEN_EQUAL:
	  os << "TOKEN_EQUAL";
	  break;
	case Parser::TOKEN_FORALL:
	  os << "TOKEN_FORALL";
	  break;
	case Parser::TOKEN_EXISTS:
	  os << "TOKEN_EXISTS";
	  break;
	case Parser::TOKEN_MAX:
	  os << "TOKEN_MAX";
	  break;
	default:
	  os << "TOKEN_ERROR(" << t.type << ")";
	  break;
	}
      return os;
    }

    // Display a list of Tokens
    std::ostream &operator<<(std::ostream &os,
			     const std::list<Parser::Token> &tokens)
    {
      std::list<Parser::Token>::const_iterator op         = tokens.begin();
      std::list<Parser::Token>::const_iterator op_end     = tokens.end();
      os << "Tokens:" << std::endl;
      while(op != op_end)
	{
	  os << (*op)<< "  ";
	  op++;
	}
      os << std::endl;
      return os;
    }



  }
}
