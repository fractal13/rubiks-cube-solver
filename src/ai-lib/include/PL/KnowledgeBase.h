#ifndef _KNOWLEDGEBASE_H_
#define _KNOWLEDGEBASE_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <list>

namespace ai
{
  namespace PL
  {
    class KnowledgeBase
    {
    public:
      KnowledgeBase();
      virtual ~KnowledgeBase();

      virtual Symbol::State Evaluate() const;

      virtual void AddSentence(const std::string &s_in);
      virtual void SetModel(const std::vector<Symbol *> &model_in);

      virtual Symbol::State TT_Entails(KnowledgeBase &alpha);

      virtual Symbol::State TT_Entails_Pruned(KnowledgeBase &alpha);

      virtual Symbol::State DPLL_Entails(KnowledgeBase &alpha);

      virtual const std::map<std::string, Symbol *>   &GetSymbols() const;
      virtual const std::vector<Sentence *>           &GetSentences() const;

      int GetCount() const;
    protected:
      virtual Symbol *AddSymbol(const std::string &s_in);
      std::map<std::string, Symbol *>   symbols;
      std::vector<Sentence *>           sentences;
      int count;
    private:
      virtual Symbol::State TT_Check_All(KnowledgeBase &alpha,
                                         std::vector<Symbol *> &model_in,
                                         unsigned int pos);


      virtual Symbol::State TT_Check_All_Pruned(KnowledgeBase &alpha,
                                                std::vector<Symbol *> &model_in,
                                                unsigned int pos);


      virtual Symbol::State DPLL_Aux(KnowledgeBase &alpha,
                                     std::vector<Sentence *> &true_clauses,
                                     std::vector<Sentence *> &unknown_clauses,
                                     std::vector<Symbol *>   &assigned_symbols,
                                     std::vector<Symbol *>   &unassigned_symbols
                                     );
      virtual Symbol::State DPLL_ExtendOne(Symbol::State new_value,
                                           KnowledgeBase &alpha,
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

    std::ostream & operator<<(std::ostream &os, const KnowledgeBase &kb);
    std::ostream &operator<<(std::ostream &os, const std::list<Sentence *> &sentences);
    std::ostream &operator<<(std::ostream &os, const std::vector<Symbol *> &symbols);

  }
}

#endif /* _KNOWLEDGEBASE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
