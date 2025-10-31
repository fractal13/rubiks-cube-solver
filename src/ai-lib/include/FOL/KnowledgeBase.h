#ifndef _KNOWLEDGEBASE_H_
#define _KNOWLEDGEBASE_H_

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <map>

namespace ai
{
  namespace FOL
  {
    class KnowledgeBase
    {
    public:
      KnowledgeBase();
      ~KnowledgeBase();

      void AddSentence(const std::string &s_in);

      const std::map<std::string, Symbol *>   &GetSymbols() const;
      const std::vector<Sentence *>           &GetSentences() const;

    protected:
      void AddSymbol(Symbol *symbol_in);
      std::string FindUniqueName(const std::string &base_name);
      std::map<std::string, Symbol *>   symbols;    /* These are pointers into the symbols in sentences. */
      std::vector<Sentence *>           sentences;
      int                               unique_count;
    private:
    };

    std::ostream & operator<<(std::ostream &os, const KnowledgeBase &kb);

  }
}

#endif /* _KNOWLEDGEBASE_H_ */
