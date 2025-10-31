#ifndef _KNOWLEDGEBASEFAST_H_
#define _KNOWLEDGEBASEFAST_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <list>

namespace ai
{
  namespace PL
  {
    class KnowledgeBaseFast
    {
    public:
      KnowledgeBaseFast();
      virtual ~KnowledgeBaseFast();

      virtual Symbol::State Evaluate() const;

      virtual void AddSentence(const std::string &s_in);
      virtual void SetModel(const std::vector<Symbol *> &model_in);

      virtual Symbol::State TT_Entails(KnowledgeBaseFast &alpha);

      virtual Symbol::State TT_Entails_Pruned(KnowledgeBaseFast &alpha);

      virtual Symbol::State DPLL_Entails(KnowledgeBaseFast &alpha);

      virtual const std::map<std::string, Symbol *>   &GetSymbols() const;
      virtual const std::vector<Sentence *>           &GetSentences() const;

      int GetCount() const;
    protected:
      virtual Symbol *AddSymbol(const std::string &s_in);
      std::map<std::string, Symbol *>   symbols;
      std::vector<Sentence *>           sentences;
      int count;
    private:
      virtual Symbol::State TT_Check_All(KnowledgeBaseFast &alpha,
                                         std::vector<Symbol *> &model_in,
                                         unsigned int pos);


      virtual Symbol::State TT_Check_All_Pruned(KnowledgeBaseFast &alpha,
                                                std::vector<Symbol *> &model_in,
                                                unsigned int pos);


      virtual Symbol::State DPLL_Aux(KnowledgeBaseFast &alpha,
                                     std::vector<Sentence *> &true_clauses,
                                     std::vector<Sentence *> &unknown_clauses,
                                     std::vector<Symbol *>   &assigned_symbols,
                                     std::vector<Symbol *>   &unassigned_symbols
                                     );
      virtual Symbol::State DPLL_ExtendOne(Symbol::State new_value,
                                           KnowledgeBaseFast &alpha,
                                           std::vector<Sentence *> &true_clauses,
                                           std::vector<Sentence *> &unknown_clauses,
                                           std::vector<Symbol *>   &assigned_symbols,
                                           std::vector<Symbol *>   &unassigned_symbols
                                           );
      virtual void DPLL_Unmove(std::vector<Sentence *> &true_clauses,
                               std::vector<Sentence *> &unknown_clauses,
                               int moved_clauses);

      virtual Symbol::State DPLL_Evaluate(std::vector<Sentence *> &true_clauses,
                                          std::vector<Sentence *> &unknown_clauses,
                                          int &moved_clauses);

      virtual void DPLL_PureSymbols(std::vector<Sentence *> &unknown_clauses,
                                    std::vector<Symbol *>   &unassigned_symbols,
                                    std::vector<Symbol *>   &pure_symbols);

    };

    std::ostream & operator<<(std::ostream &os, const KnowledgeBaseFast &kb);
    /* Defined in KnowledgeBase.(h|cpp)
       std::ostream &operator<<(std::ostream &os, const std::list<Sentence *> &sentences);
       std::ostream &operator<<(std::ostream &os, const std::vector<Symbol *> &symbols);
    */

  }
}

#endif /* _KNOWLEDGEBASEFAST_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
