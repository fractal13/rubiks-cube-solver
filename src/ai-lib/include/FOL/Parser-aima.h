#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>
#include <iostream>
#include <ai_fol.h>

namespace ai
{
  namespace FOL
  {
    class Parser
    {
    public:
      enum TokenType { TOKEN_MIN, TOKEN_SYMBOL, TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
                       TOKEN_NOT, TOKEN_AND, TOKEN_OR, TOKEN_IMPLIES, TOKEN_IFF, TOKEN_EQUAL,
                       TOKEN_COMMA, TOKEN_FORALL, TOKEN_EXISTS, TOKEN_MAX };
      struct Token
      {
      public:
      Token(TokenType type_in) : type(type_in) { /* empty */ }
      Token(TokenType type_in, const std::string &symbol_in) : type(type_in), symbol(symbol_in) { /* empty */ }
        TokenType type;
        std::string symbol;
      };


      Parser();
      ~Parser();
      Sentence *Parse(const std::string &str_in);
    protected:
      Sentence *pSentence(std::list<Token>::const_iterator &curr_token);
      Sentence *pAtomicSentence(std::list<Token>::const_iterator &curr_token);
      Sentence *pConnective(std::list<Token>::const_iterator &curr_token);
      Sentence *pTermList(std::list<Token>::const_iterator &curr_token);
      Sentence *pTerm(std::list<Token>::const_iterator &curr_token);
      int       CountArity(const Sentence *s_in) const;
      void Tokenize(const std::string &s_in, std::list<Token> &tokens);

    private:

    };
    std::ostream &operator<<(std::ostream &os, const Parser::Token &t);
    std::ostream &operator<<(std::ostream &os, const std::list<Parser::Token> &tokens);

  }
}


#endif /* _PARSER_H_ */
