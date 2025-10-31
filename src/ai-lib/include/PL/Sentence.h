#ifndef _SENTENCE_H_
#define _SENTENCE_H_
#include <iostream>
#include <vector>

namespace ai
{
  namespace PL
  {
    class Sentence
    {
    public:
      enum SentenceType { T_TRUE, T_FALSE, T_SYMBOL, T_UNARY, T_BINARY,
                          T_LEFT_PAREN, T_RIGHT_PAREN };
      enum OperatorType { O_NOT, O_AND, O_OR, O_IMPLIES, O_IFF, O_NONE };

      Sentence(bool value_in);                        // create T_TRUE or T_FALSE
      Sentence(SentenceType type_in);                 // for parsing: (,) only
      Sentence(SentenceType type_in, OperatorType op_in); // for parsing: UNARY/NOT, BINARY/AND,OR,IMPLIES,IFF
      Sentence(Symbol *symbol_in);                    // create T_SYMBOL
      Sentence(OperatorType op_in, Sentence *rhs_in); // create T_UNARY, O_NOT
      Sentence(Sentence *lhs_in, OperatorType op_in,
               Sentence *rhs_in);                     // create T_BINARY
      Sentence(const Sentence &rhs_in);               // copy constructor

      Sentence &operator=(const Sentence &rhs_in);

      ~Sentence();
      void Delete();

      void SetLHS(Sentence *lhs_in);
      void SetRHS(Sentence *rhs_in);

      Symbol::State Evaluate() const;

      SentenceType    GetType() const;
      const Symbol   *GetSymbol() const;
      OperatorType    GetOperator() const;
      const Sentence *GetLHS() const;
      const Sentence *GetRHS() const;

      std::string ToString() const;

      void CNF(); // convert to conjuntive normal form
      static void SplitClauses(Sentence *s_in, std::vector<Sentence *> &clauses);

      void FindPure(std::vector<Symbol *> &asserted,
                    std::vector<Symbol *> &negated);

      bool IsUnitClause(Symbol *&sym_out);

      bool operator==( const Sentence& rhs ) const;

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
      Symbol::State EvaluateBinary() const;
      static Symbol::State EvaluateConjunct(Symbol::State t1, Symbol::State t2);
      static Symbol::State EvaluateDisjunct(Symbol::State t1, Symbol::State t2);
      static Symbol::State EvaluateImplication(Symbol::State t1, Symbol::State t2);
      static Symbol::State EvaluateBiconditional(Symbol::State t1, Symbol::State t2);

      static bool EliminateBiconditional(Sentence *s_in);
      static bool EliminateImplication(Sentence *s_in);
      static void MoveNotIn(Sentence *s_in, bool have_not);
      static bool DistributeOr(Sentence *s_in);
      static void SplitClausesAux(Sentence *s_in, std::vector<Sentence *> &clauses);

      static void FindPureAux(Sentence *s_in,
                              std::vector<Symbol *> &asserted,
                              std::vector<Symbol *> &negated);

    };

    std::ostream & operator<<(std::ostream &os, const Sentence &s);

  }
}
#endif /* _SENTENCE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
