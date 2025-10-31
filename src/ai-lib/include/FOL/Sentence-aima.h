#ifndef _SENTENCE_H_
#define _SENTENCE_H_
#include <iostream>
#include <vector>
#include <list>

namespace ai
{
  namespace FOL
  {
    class Sentence
    {
    public:
      enum SentenceType { T_TRUE, T_FALSE, T_SYMBOL, T_UNARY, T_BINARY,
                          T_LEFT_PAREN, T_RIGHT_PAREN, T_FORALL, T_EXISTS };
      /* Order of the operator types implies precedence.  Lowest to highest */
      enum OperatorType { O_EQUAL, O_COMMA, O_IFF, O_IMPLIES, O_OR, O_AND, O_NOT };

      Sentence(bool value_in);                        // create T_TRUE or T_FALSE
      Sentence(SentenceType type_in);                 // for parsing
      Sentence(SentenceType type_in, OperatorType op_in); // for parsing
      Sentence(SentenceType type_in, Sentence *rhs_in);   // for T_ARGS
      Sentence(Symbol *symbol_in);                    // create T_SYMBOL
      Sentence(Symbol *symbol_in, Sentence *rhs_in);  // create T_SYMBOL for PREDICATE and FUNCTION
      Sentence(SentenceType type_in, Symbol *symbol_in, Sentence *rhs_in); // for T_FORALL/T_EXISTS
      Sentence(OperatorType op_in, Sentence *rhs_in); // create T_UNARY, O_NOT
      Sentence(Sentence *lhs_in, OperatorType op_in,
               Sentence *rhs_in);                     // create T_BINARY
      Sentence(const Sentence &rhs_in);               // copy constructor

      Sentence &operator=(const Sentence &rhs_in);

      ~Sentence();
      void Delete();

      void SetLHS(Sentence *lhs_in);
      void SetRHS(Sentence *rhs_in);
      void SetSymbol(Symbol *symbol_in);
      void SetLHSNoDelete(Sentence *lhs_in);
      void SetRHSNoDelete(Sentence *rhs_in);
      void SetSymbolNoDelete(Symbol *symbol_in);
      void SetVariable(Symbol *variable_in);

      SentenceType    GetType() const;
      const Symbol   *GetSymbol() const;
      OperatorType    GetOperator() const;
      const Sentence *GetLHS() const;
      const Sentence *GetRHS() const;

      void GetAllSymbols(std::list<Symbol *> &symbols_out) const;

      std::string ToString() const;
      std::string ToString(bool verbose) const;

      bool ContainsConnective() const;

    protected:
      SentenceType  type;

      /* Non-null only if type == T_SYMBOL */
      Symbol       *symbol;

      /* Non-null only if type == T_BINARY */
      Sentence     *lhs;

      /* Valid if type == T_UNARY or T_BINARY */
      OperatorType  op;

      /* Non-null if type == T_BINARY or T_UNARY */
      Sentence     *rhs;


    private:

    };

    std::ostream & operator<<(std::ostream &os, const Sentence &s);
    std::ostream & operator<<(std::ostream &os, const std::list<Sentence *> &sentences);

  }
}
#endif /* _SENTENCE_H_ */
