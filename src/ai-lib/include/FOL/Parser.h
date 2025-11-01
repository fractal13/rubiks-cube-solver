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
                       TOKEN_COMMA, TOKEN_FORALL, TOKEN_EXISTS, TOKEN_EOF, TOKEN_MAX };
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
      bool Match(const TokenType &type_in);
      Sentence       *pSentence();
      Sentence0      *pSentence0();
      Sentence0      *pSentence0Tail(Sentence0 *lhs_in);
      Sentence1      *pSentence1();
      Sentence1      *pSentence1Tail(Sentence1 *lhs_in);
      Sentence2      *pSentence2();
      Sentence2      *pSentence2Tail(Sentence2 *lhs_in);
      Sentence3      *pSentence3();
      Sentence3      *pSentence3Tail(Sentence3 *lhs_in);
      Sentence4      *pSentence4();
      Sentence5      *pSentence5();
      AtomicSentence *pAtomicSentence();
      VariableList   *pVariableList();
      VariableList   *pVariableListTail(VariableList *lhs_in);
      TermList       *pTermList();
      TermList       *pTermListTail(TermList *lhs_in);
      Term           *pTerm();
      Identifier     *pIdentifier();
      void            Tokenize(const std::string &s_in);

    private:
      std::list<Token> tokens;
      std::list<Token>::const_iterator curr_token;

    };
    std::ostream &operator<<(std::ostream &os, const Parser::Token &t);
    std::ostream &operator<<(std::ostream &os, const std::list<Parser::Token> &tokens);

  }
}


#endif /* _PARSER_H_ */
