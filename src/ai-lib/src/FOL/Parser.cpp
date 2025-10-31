/*
 * This is a recursive decent parser for a modified first order logic grammer 
 * that was originally based on the grammar specified in AIMA 2nd Ed. by
 * Russell and Norvig on page 247.  The modified grammar is shown below.
 *
 * The parser will give lots of std::cerr messages when it encounters
 * a malformed sentence, but will still give back a parsed sentence.
 * This could be viewed as a bug, or feature.  Take your pick.
 *
 * June 1, 2009 - CGL
 *
 * Modifications:
 * 1- Added operator precedence through levels of the grammar.
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
 * 3- Renamed the connectives using easily typed symbols.
 * 4- Allows extra parenthesis pairs around sentences without
 *    any harm.
 *
 *
 // *
 // * <Sentence> -> <Sentence0>
 // *
 // * <Sentence0> -> <Sentence1> <Sentence0Tail>
 // * <Sentence0Tail> -> TOKEN_IFF <Sentence1> <Sentence0Tail>
 // * <Sentence0Tail> -> {empty}
 // *
 // * <Sentence1> -> <Sentence2> <Sentence1Tail>
 // * <Sentence1Tail> -> TOKEN_IMPLIES <Sentence2> <Sentence1Tail>
 // * <Sentence1Tail> -> {empty}
 // *
 // * <Sentence2> -> <Sentence3> <Sentence2Tail>
 // * <Sentence2Tail> -> TOKEN_OR <Sentence3> <Sentence2Tail>
 // * <Sentence2Tail> -> {empty}
 // *           
 // * <Sentence3> -> <Sentence4> <Sentence3Tail>
 // * <Sentence3Tail> -> TOKEN_AND <Sentence4> <Sentence3Tail>
 // * <Sentence3Tail> -> {empty}
 // *           
 // * <Sentence4> -> TOKEN_NOT <Sentence4>
 // * <Sentence4> -> <Sentence5>
 // *           
 // * <Sentence5> -> <AtomicSentence>
 // * <Sentence5> -> TOKEN_LEFT_PAREN <Sentence0> TOKEN_RIGHT_PAREN
 // * <Sentence5> -> TOKEN_FORALL <VariableList> <Sentence0>
 // * <Sentence5> -> TOKEN_EXISTS <VariableList> <Sentence0>
 // *
 // * <AtomicSentence> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
 // * <AtomicSentence> -> <Term> TOKEN_EQUAL <Term>
 // *
 // * <VariableList> -> <Identifier> <VariableListTail>
 // * <VariableListTail> -> TOKEN_COMMA <Identifier> <VariableListTail>
 // * <VariableListTail> -> {empty}
 // *
 // * <TermList> -> <Term> <TermListTail>
 // * <TermListTail> -> TOKEN_COMMA <Term> <TermListTail>
 // * <TermListTail> -> <Term>
 // *
 // * <Term> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
 // * <Term> -> <Identifier>
 // *
 // * <Identifier> -> TOKEN_SYMBOL          // ([A-Za-z][A-Za-z0-9_]*)
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
      Tokenize(str_in);
#if DEBUG_PARSE
      std::cout << str_in << std::endl;
      std::cout << tokens << std::endl;
#endif
      curr_token = tokens.begin();
      Sentence *s = pSentence();
#if DEBUG_PARSE
      if(s)
	{
	  std::cout << "Parse: " << str_in << std::endl;
	  std::cout << "     : " << (*s) << std::endl;
	}
#endif
      return s;
    }

    bool Parser::Match(const TokenType &type_in)
    {
      if(type_in == curr_token->type)
	{
	  return true;
	}
      Token t(type_in);
      std::cerr << "Parse error.  Expected token " << t << ".  Got token " << *curr_token << "." << std::endl;
      return false;
    }
    
    Sentence       *Parser::pSentence()
    { // * <Sentence> -> <Sentence0>
      Sentence0 *s0 = pSentence0();
      if(s0 && Match(TOKEN_EOF))
	{
	  curr_token ++;
	  return new Sentence(s0);
	}
      return 0;
    }
    
    Sentence0      *Parser::pSentence0()
    { // * <Sentence0> -> <Sentence1> <Sentence0Tail>
      Sentence1 *s1 = pSentence1();
      if(s1)
	{
	  Sentence0 *lhs = new Sentence0(s1);
	  return pSentence0Tail(lhs);
	}
      return 0;
    }
    Sentence0      *Parser::pSentence0Tail(Sentence0 *lhs_in)
    { // * <Sentence0Tail> -> TOKEN_IFF <Sentence1> <Sentence0Tail>
      // * <Sentence0Tail> -> {empty}
      if(curr_token->type == TOKEN_IFF)
	{
	  curr_token ++;
	  Sentence1 *rhs = pSentence1();
	  Sentence0 *s0  = new Sentence0(lhs_in, rhs);
	  return pSentence0Tail(s0);
	}
      return lhs_in;
    }
    Sentence1      *Parser::pSentence1()
    { // * <Sentence1> -> <Sentence2> <Sentence1Tail>
      Sentence2 *s2 = pSentence2();
      if(s2)
	{
	  Sentence1 *lhs = new Sentence1(s2);
	  return pSentence1Tail(lhs);
	}
      return 0;
    }
    Sentence1      *Parser::pSentence1Tail(Sentence1 *lhs_in)
    { // * <Sentence1Tail> -> TOKEN_IMPLIES <Sentence2> <Sentence1Tail>
      // * <Sentence1Tail> -> {empty}
      if(curr_token->type == TOKEN_IMPLIES)
	{
	  curr_token ++;
	  Sentence2 *rhs = pSentence2();
	  Sentence1 *s1 = new Sentence1(lhs_in, rhs);
	  return pSentence1Tail(s1);
	}
      return lhs_in;
    }
    Sentence2      *Parser::pSentence2()
    { // * <Sentence2> -> <Sentence3> <Sentence2Tail>
      Sentence3 *s3 = pSentence3();
      if(s3)
	{
	  Sentence2 *lhs = new Sentence2(s3);
	  return pSentence2Tail(lhs);
	}
      return 0;
    }
    Sentence2      *Parser::pSentence2Tail(Sentence2 *lhs_in)
    { // * <Sentence2Tail> -> TOKEN_OR <Sentence3> <Sentence2Tail>
      // * <Sentence2Tail> -> {empty}
      if(curr_token->type == TOKEN_OR)
	{
	  curr_token ++;
	  Sentence3 *rhs = pSentence3();
	  Sentence2 *s2 = new Sentence2(lhs_in, rhs);
	  return pSentence2Tail(s2);
	}
      return lhs_in;
    }
    Sentence3      *Parser::pSentence3()
    { // * <Sentence3> -> <Sentence4> <Sentence3Tail>
      Sentence4 *s4 = pSentence4();
      if(s4)
	{
	  Sentence3 *lhs = new Sentence3(s4);
	  return pSentence3Tail(lhs);
	}
      return 0;
    }
    Sentence3      *Parser::pSentence3Tail(Sentence3 *lhs_in)
    { // * <Sentence3Tail> -> TOKEN_AND <Sentence4> <Sentence3Tail>
      // * <Sentence3Tail> -> {empty}
      if(curr_token->type == TOKEN_AND)
	{
	  curr_token ++;
	  Sentence4 *rhs = pSentence4();
	  Sentence3 *s3 = new Sentence3(lhs_in, rhs);
	  return pSentence3Tail(s3);
	}
      return lhs_in;
    }
    Sentence4      *Parser::pSentence4()
    { // * <Sentence4> -> TOKEN_NOT <Sentence4>
      // * <Sentence4> -> <Sentence5>
      if(curr_token->type == TOKEN_NOT)
	{
	  curr_token ++;
	  Sentence4 *rhs = pSentence4();
	  if(rhs)
	    {
	      return new Sentence4(rhs);
	    }
	}
      else
	{
	  Sentence5 *rhs = pSentence5();
	  if(rhs)
	    {
	      return new Sentence4(rhs);
	    }
	}
      return 0;
      
    }
    Sentence5      *Parser::pSentence5()
    { // * <Sentence5> -> <AtomicSentence>
      // * <Sentence5> -> TOKEN_LEFT_PAREN <Sentence0> TOKEN_RIGHT_PAREN
      // * <Sentence5> -> TOKEN_FORALL <VariableList> <Sentence0>
      // * <Sentence5> -> TOKEN_EXISTS <VariableList> <Sentence0>
      if(curr_token->type == TOKEN_LEFT_PAREN)
	{
	  curr_token ++;
	  Sentence0 *s0 = pSentence0();
	  if(s0 && Match(TOKEN_RIGHT_PAREN))
	    {
	      curr_token ++;
	      return new Sentence5(s0);
	    }
	  return 0;
	}
      else if((curr_token->type == TOKEN_FORALL) || (curr_token->type == TOKEN_EXISTS))
	{
	  SentenceType type;
	  if(curr_token->type == TOKEN_FORALL)
	    {
	      type = T_FORALL;
	    }
	  else
	    {
	      type = T_EXISTS;
	    }
	  curr_token ++;
	  VariableList *vl = pVariableList();
	  Sentence0 *s0 = pSentence0();
	  if(vl && s0)
	    {
	      std::list<Identifier *> variables;
	      vl->GetAllVariables(variables);
	      std::list<Identifier *> constants;
	      s0->GetAllConstants(constants);
	      
	      std::list<Identifier *>::iterator vit;
	      std::list<Identifier *>::iterator cit;
	      for(vit = variables.begin(); vit != variables.end(); vit++)
		{
		  std::string vname = (*vit)->GetSymbol()->GetName();
		  for(cit = constants.begin(); cit != constants.end(); cit++)
		    {
		      Symbol *csym = (*cit)->GetSymbol();
		      std::string cname = csym->GetName();
		      if(vname == cname)
			{
			  csym->SetType(Symbol::TYPE_VARIABLE);
			}
		    }
		}
	      return new Sentence5(type, vl, s0);
	    }
	  return 0;
	}
      else
	{
	  AtomicSentence *as = pAtomicSentence();
	  if(as)
	    {
	      return new Sentence5(as);
	    }
	  return 0;
	}
      return 0;
    }
    AtomicSentence *Parser::pAtomicSentence()
    { // * <AtomicSentence> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
      // * <AtomicSentence> -> <Term> TOKEN_EQUAL <Term>
      
      /* FIXME, this is a hack because the grammar is ambiguous!
       * Need to figure out grammar correction.
       */
      Term *lhs = pTerm();
      if(curr_token->type == TOKEN_EQUAL)
	{
	  curr_token ++;
	  Term *rhs = pTerm();
	  if(rhs)
	    {
	      return new AtomicSentence(lhs, rhs);
	    }
	  return 0;
	}
      else if(lhs->GetIdentifier() && lhs->GetTermList())
	{ // The Term returned should be a Function that needs to be changed into a Predicate.
	  // Check and change Term.
	  Identifier *identifier = lhs->GetIdentifier();
	  TermList   *term_list  = lhs->GetTermList();
	  lhs->ClearFunction();
	  delete lhs;
	  if(!identifier || identifier->GetSymbol()->GetType() != Symbol::TYPE_FUNCTION)
	    {
	      return 0;
	    }
	  
	  Symbol *symbol = identifier->GetSymbol();
	  symbol->SetType(Symbol::TYPE_PREDICATE);

	  return new AtomicSentence(identifier, term_list);
	}
      return 0;
    }
    VariableList   *Parser::pVariableList()
    { // * <VariableList> -> <Identifier> <VariableListTail>
      Identifier *variable = pIdentifier();
      if(variable)
	{
	  variable->GetSymbol()->SetType(Symbol::TYPE_VARIABLE);
	  VariableList *lhs = new VariableList(variable);
	  return pVariableListTail(lhs);
	}
      return 0;
    }
    VariableList   *Parser::pVariableListTail(VariableList *lhs_in)
    { // * <VariableListTail> -> TOKEN_COMMA <Identifier> <VariableListTail>
      // * <VariableListTail> -> {empty}
      if(curr_token->type == TOKEN_COMMA)
	{
	  curr_token ++;
	  Identifier *rhs = pIdentifier();
	  rhs->GetSymbol()->SetType(Symbol::TYPE_VARIABLE);
	  VariableList *variable_list = new VariableList(lhs_in, rhs);
	  return pVariableListTail(variable_list);
	}
      return lhs_in;
    }
    TermList       *Parser::pTermList()
    { // * <TermList> -> <Term> <TermListTail>
      Term *term = pTerm();
      if(term)
	{
	  TermList *lhs = new TermList(term);
	  return pTermListTail(lhs);
	}
      return 0;
    }
    TermList       *Parser::pTermListTail(TermList *lhs_in)
    { // * <TnermListTail> -> TOKEN_COMMA <Term> <TermListTail>
      // * <TermListTail> -> <Term>
      if(curr_token->type == TOKEN_COMMA)
	{
	  curr_token ++;
	  Term *rhs = pTerm();
	  TermList *term_list = new TermList(lhs_in, rhs);
	  return pTermListTail(term_list);
	}
      return lhs_in;
    }
    Term           *Parser::pTerm()
    { // * <Term> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
      // * <Term> -> <Identifier>

      // Varibles are created from Constants inside of FORALL and EXISTS parsing.
      Identifier *identifier = pIdentifier();
      if(identifier)
	{
	  if(curr_token->type == TOKEN_LEFT_PAREN)
	    { 
	      curr_token ++;
	      identifier->GetSymbol()->SetType(Symbol::TYPE_FUNCTION);
	      TermList *term_list = pTermList();
	      if(term_list && Match(TOKEN_RIGHT_PAREN))
		{
		  identifier->GetSymbol()->SetArity(term_list->CountArity());
		  curr_token ++;
		  return new Term(identifier, term_list);
		}
	      return 0;
	    }
	  else
	    {
	      identifier->GetSymbol()->SetType(Symbol::TYPE_CONSTANT);
	      return new Term(identifier);
	    }
	}
      return 0;
    }
    Identifier     *Parser::pIdentifier()
    { // * <Identifier> -> TOKEN_SYMBOL          // ([A-Za-z][A-Za-z0-9_]*)
      if(Match(TOKEN_SYMBOL))
	{
	  Symbol *symbol = new Symbol(curr_token->symbol);
	  symbol->SetType(Symbol::TYPE_CONSTANT);
	  curr_token ++;
	  return new Identifier(symbol);
	}
      return 0;
    }
    
    void Parser::Tokenize(const std::string &s_in)
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
      Token t(TOKEN_EOF);
      tokens.push_back(t);
#if DEBUG_PARSE && 0
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
	case Parser::TOKEN_EOF:
	  os << "TOKEN_EOF";
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
