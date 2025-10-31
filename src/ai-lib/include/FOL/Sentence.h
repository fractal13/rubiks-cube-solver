#ifndef _SENTENCE_H_
#define _SENTENCE_H_
#include <iostream>
#include <vector>
#include <list>

namespace ai
{
  namespace FOL
  {
    enum SentenceType { T_TRUE, T_FALSE, T_SYMBOL, T_UNARY, T_BINARY,
                        T_LEFT_PAREN, T_RIGHT_PAREN, T_FORALL, T_EXISTS,
                        T_IFF, T_IMPLIES, T_OR, T_AND, T_NOT, T_ATOMIC,

                        T_ERROR };

    class Sentence0;
    class Sentence1;
    class Sentence2;
    class Sentence3;
    class Sentence4;
    class Sentence5;
    class AtomicSentence;
    class VariableList;
    class TermList;
    class Term;
    class Identifier;

    class Sentence0
    {
    public:
      friend class Sentence1; friend class Sentence2; friend class Sentence3; friend class Sentence4; friend class Sentence5;
      //  * <Sentence0> -> <Sentence0> TOKEN_IFF <Sentence1>
      Sentence0(Sentence0 *lhs_in, Sentence1 *rhs_in);
      //  * <Sentence0> -> <Sentence1>
      Sentence0(Sentence1 *rhs_in);
      Sentence0(const Sentence0 &original_in);          // Copy constructor
      Sentence0(Sentence2 *s2_in);                      // Create nested sentence structure
      Sentence0(Sentence3 *s3_in);                      // Create nested sentence structure
      virtual ~Sentence0();

      Sentence0 *GetLHS();
      Sentence1 *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      SentenceType GetSentenceType() const;
      Sentence4 * GetNextNot();
      Sentence3 * GetNextAnd();
      Sentence2 * GetNextOr();

      std::string ToString(bool verbose) const;
    protected:
      Sentence0 *lhs;
      Sentence1 *rhs;
    private:
    };

    class Sentence1
    {
    public:
      friend class Sentence0; friend class Sentence2; friend class Sentence3; friend class Sentence4; friend class Sentence5;
      //  * <Sentence1> -> <Sentence1> TOKEN_IMPLIES <Sentence2>
      Sentence1(Sentence1 *lhs_in, Sentence2 *rhs_in);
      //  * <Sentence1> -> <Sentence2>
      Sentence1(Sentence2 *rhs_in);
      Sentence1(const Sentence1 &original_in);          // Copy constructor
      Sentence1(Sentence3 *s3_in);                      // Create nested sentence structure
      virtual ~Sentence1();

      Sentence1 *GetLHS();
      Sentence2 *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      SentenceType GetSentenceType() const;
      Sentence4 * GetNextNot();
      Sentence3 * GetNextAnd();
      Sentence2 * GetNextOr();
      std::string ToString(bool verbose) const;
    protected:
      Sentence1 *lhs;
      Sentence2 *rhs;
    private:
    };


    class Sentence2
    {
    public:
      friend class Sentence0; friend class Sentence1; friend class Sentence3; friend class Sentence4; friend class Sentence5;
      //  * <Sentence2> -> <Sentence2> TOKEN_OR <Sentence3>
      Sentence2(Sentence2 *lhs_in, Sentence3 *rhs_in);
      //  * <Sentence2> -> <Sentence3>
      Sentence2(Sentence3 *rhs_in);
      Sentence2(const Sentence2 &original_in);          // Copy constructor
      Sentence2(Sentence1 *s1_in);                      // Create nested sentence structure
      Sentence2(Sentence4 *s4_in);                      // Create nested sentence structure
      virtual ~Sentence2();

      Sentence2 *GetLHS();
      Sentence3 *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      SentenceType GetSentenceType() const;
      Sentence4 * GetNextNot();
      Sentence3 * GetNextAnd();
      Sentence2 * GetNextOr();
      std::string ToString(bool verbose) const;
    protected:
      Sentence2 *lhs;
      Sentence3 *rhs;
    private:
    };


    class Sentence3
    {
    public:
      friend class Sentence0; friend class Sentence1; friend class Sentence2; friend class Sentence4; friend class Sentence5;
      //  * <Sentence3> -> <Sentence3> TOKEN_AND <Sentence4>
      Sentence3(Sentence3 *lhs_in, Sentence4 *rhs_in);
      //  * <Sentence3> -> <Sentence4>
      Sentence3(Sentence4 *rhs_in);
      Sentence3(const Sentence3 &original_in);          // Copy constructor
      Sentence3(Sentence1 *s1_in);                      // Create nested sentence structure
      Sentence3(Sentence2 *s2_in);                      // Create nested sentence structure
      virtual ~Sentence3();

      Sentence3 *GetLHS();
      Sentence4 *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      SentenceType GetSentenceType() const;
      Sentence4 * GetNextNot();
      Sentence3 * GetNextAnd();
      Sentence2 * GetNextOr();
      std::string ToString(bool verbose) const;
    protected:
      Sentence3 *lhs;
      Sentence4 *rhs;
    private:
    };


    class Sentence4
    {
    public:
      friend class Sentence0; friend class Sentence1; friend class Sentence2; friend class Sentence3; friend class Sentence5;
      //  * <Sentence4> -> TOKEN_NOT <Sentence4>
      Sentence4(Sentence4 *lhs_in);
      //  * <Sentence4> -> <Sentence5>
      Sentence4(Sentence5 *rhs_in);
      Sentence4(const Sentence4 &original_in);          // Copy constructor
      Sentence4(Sentence1 *s1_in);                      // Create nested sentence structure
      Sentence4(Sentence2 *s2_in);                      // Create nested sentence structure
      Sentence4(Sentence3 *s3_in);                      // Create nested sentence structure
      virtual ~Sentence4();

      Sentence4 *GetLHS();
      Sentence5 *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      SentenceType GetSentenceType() const;
      Sentence4 * GetNextNot();
      Sentence3 * GetNextAnd();
      Sentence2 * GetNextOr();
      std::string ToString(bool verbose) const;
    protected:
      Sentence4 *lhs;
      Sentence5 *rhs;

      void MoveNotIn_NotNot();
      void MoveNotIn_NotAnd();
      void MoveNotIn_NotOr();
      void MoveNotIn_NotForall();
      void MoveNotIn_NotExists();

    private:
    };


    class Sentence5
    {
    public:
      friend class Sentence0; friend class Sentence1; friend class Sentence2; friend class Sentence3; friend class Sentence4;
      //  * <Sentence5> -> <AtomicSentence>
      Sentence5(AtomicSentence *atomic_sentence_in);
      //  * <Sentence5> -> TOKEN_LEFT_PAREN <Sentence0> TOKEN_RIGHT_PAREN
      Sentence5(Sentence0 *sentence_in);
      //  * <Sentence5> -> TOKEN_FORALL <VariableList> <Sentence0>
      //  * <Sentence5> -> TOKEN_EXISTS <VariableList> <Sentence0>
      Sentence5(SentenceType type_in, VariableList *variable_list_in, Sentence0 *sentence_in);
      Sentence5(const Sentence5 &original_in);          // Copy constructor
      Sentence5(Sentence1 *s1_in);                      // Create nested sentence structure
      Sentence5(Sentence2 *s2_in);                      // Create nested sentence structure
      Sentence5(Sentence3 *s3_in);                      // Create nested sentence structure
      virtual ~Sentence5();

      AtomicSentence *GetAtomicSentence();
      VariableList   *GetVariableList();
      Sentence0      *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      SentenceType GetSentenceType() const;
      Sentence4 * GetNextNot();
      Sentence3 * GetNextAnd();
      Sentence2 * GetNextOr();
      std::string ToString(bool verbose) const;
    protected:
      AtomicSentence *atomic_sentence;
      VariableList *variable_list;
      Sentence0 *rhs;
      SentenceType type;
    private:
    };


    class AtomicSentence
    {
    public:
      //  * <AtomicSentence> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
      AtomicSentence(Identifier *predicate_in, TermList *term_list_in);
      //  * <AtomicSentence> -> <Term> TOKEN_EQUAL <Term>
      AtomicSentence(Term *lhs_in, Term *rhs_in);
      AtomicSentence(const AtomicSentence &original_in);          // Copy constructor

      virtual ~AtomicSentence();

      Identifier *GetPredicate();
      TermList  *GetTermList();
      Term      *GetLHS();
      Term      *GetRHS();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;

      std::string ToString(bool verbose) const;
    protected:
      Identifier *predicate;
      TermList  *term_list;
      Term      *lhs;
      Term      *rhs;
    private:
    };


    class VariableList
    {
    public:
      // * <VariableList> -> <VariableList> TOKEN_COMMA <Identifier>
      VariableList(VariableList *variable_list_in, Identifier *variable_in);
      // * <VariableList> -> <Identifier>
      VariableList(Identifier *variable_in);
      VariableList(const VariableList &original_in);          // Copy constructor

      virtual ~VariableList();

      VariableList *GetVariableList();
      Identifier   *GetVariable();

      void GetAllVariables(std::list<Identifier *> &variables_out) const;

      std::string ToString(bool verbose) const;
    protected:
      VariableList *variable_list;
      Identifier   *variable;
    private:
    };


    class TermList
    {
    public:
      // * <TermList> -> <TermList> TOKEN_COMMA <Term>
      TermList(TermList *term_list_in, Term *term_in);
      // * <TermList> -> <Term>
      TermList(Term *term_in);
      TermList(const TermList &original_in);          // Copy constructor

      virtual ~TermList();

      TermList *GetTermList();
      Term     *GetTerm();

      int       CountArity() const;
      void GetAllConstants(std::list<Identifier *> &constants_out) const;

      std::string ToString(bool verbose) const;
    protected:
      TermList *term_list;
      Term     *term;
    private:
    };


    class Term
    {
    public:
      // * <Term> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
      Term(Identifier *function_in, TermList *term_list_in);
      // * <Term> -> <Identifier>
      Term(Identifier *variable_in);
      Term(const Term &original_in);          // Copy constructor

      virtual ~Term();

      // sets the function and term_list pointers to 0
      // used for Function to Predicate conversion.
      void ClearFunction();

      Identifier *GetIdentifier();
      TermList *GetTermList();

      void GetAllConstants(std::list<Identifier *> &constants_out) const;

      std::string ToString(bool verbose) const;
    protected:
      Identifier *identifier;
      TermList *term_list;
    private:
    };


    class Identifier
    {
    public:
      // * <Identifier> -> TOKEN_SYMBOL          // ([A-Za-z][A-Za-z0-9_]*)
      Identifier(Symbol *symbol_in);
      Identifier(const Identifier &original_in);          // Copy constructor

      virtual ~Identifier();

      Symbol *GetSymbol();

      std::string ToString(bool verbose) const;
    protected:
      Symbol *symbol;

    private:
    };



    class Sentence
    {
    public:
      Sentence(Sentence0 *sentence0_in);
      Sentence(const Sentence &sentence0_in);

      Sentence &operator=(const Sentence &rhs_in);

      ~Sentence();
      void Delete();

      void GetCNF(std::vector<Sentence *> &clauses) const;
      void EliminateBiconditional();
      void EliminateImplication();
      void MoveNotIn();
      void EliminateExtraParens();

      Sentence0 *GetSentence();

      std::string ToString() const;
      std::string ToString(bool verbose) const;

    protected:
      Sentence0 *sentence0;

    private:

    };

    std::ostream & operator<<(std::ostream &os, const Sentence &s);
    std::ostream & operator<<(std::ostream &os, const std::list<Sentence *> &sentences);

  }
}
#endif /* _SENTENCE_H_ */
