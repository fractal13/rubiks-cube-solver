#include <ai_fol.h>
#include <list>
#include <cstdio>

#define DEBUG_PARSE 0
#define DEBUG_UNIQUE_VARIABLE 0

namespace ai
{
  namespace FOL
  {
    /*
     * Initially know nothing
     */
    KnowledgeBase::KnowledgeBase()
      : sentences(0),
	unique_count(0)
    {
    }

    /*
     * Clean up the dynamic memory
     */
    KnowledgeBase::~KnowledgeBase()
    {
      unsigned int i;
      for(i = 0; i < sentences.size(); i++)
	{
	  delete sentences[i];
	}

      /*  Do not delete these.  We didn't allocate them.
      std::map<std::string, Symbol *>::const_iterator it;
      for(it = symbols.begin(); it != symbols.end(); it++)
	{
	  delete it->second;
	}
      */
    }

    /* Parse an input string, add as sentence to the KB
     *
     */
    void KnowledgeBase::AddSentence(const std::string &s_in)
    {
      Parser p;
#if DEBUG_PARSE
      std::cout << "AddSentence: " << s_in << std::endl;
#endif
      Sentence *s = p.Parse(s_in);
      if(s)
	{
#if DEBUG_PARSE
	  std::cout << "ParsedSentence:" << (*s) << std::endl;
#endif
#if 0
	  std::list<Symbol *> symbol_list;
	  std::list<Symbol *>::iterator it;
	  s->GetAllSymbols(symbol_list);
	  for(it = symbol_list.begin(); it != symbol_list.end(); it++)
	    {
	      if((*it)->GetType() == Symbol::TYPE_VARIABLE)
		{
#if DEBUG_UNIQUE_VARIABLE
		  std::cout << "UQ1: " << (*(*it)) << std::endl;
#endif
		  if((*it)->GetIsUnique())
		    { // already unique
		      continue;
		    }
#if DEBUG_UNIQUE_VARIABLE
		  std::cout << "UQ2: " << (*(*it)) << std::endl;
#endif
		  std::string unique_name = FindUniqueName((*it)->GetName());
		  std::string original_name = (*it)->GetName();
		  //if(unique_name != (*it)->GetName())
		    {
		      std::list<Symbol *>::iterator temp_it;
		      for(temp_it = symbol_list.begin(); temp_it != symbol_list.end(); temp_it++)
			{
			  if((*temp_it)->GetType() == Symbol::TYPE_VARIABLE)
			    {

#if DEBUG_UNIQUE_VARIABLE
			      std::cout << "UQ3: " << (*(*temp_it)) << std::endl;
#endif
			      
			      if((*temp_it)->GetName() == original_name)
				{
				  (*temp_it)->MakeUnique(unique_name);
#if DEBUG_UNIQUE_VARIABLE
				  std::cout << "UNIQUE: " << (*temp_it) << " " << (*s) << std::endl;
#endif
				}
			    }
			}
		    }
		}
	      AddSymbol((*it));
	    }
#endif /* 0 */
	  sentences.push_back(s);
	}

    }

    /* Add the string as a new symbol in the KB, unless already exists.
     */
    void KnowledgeBase::AddSymbol(Symbol *symbol_in)
    {
      std::map<std::string, Symbol *>::const_iterator it;
      it = symbols.find(symbol_in->GetName());
      if(it != symbols.end())
	{
	  return; // it->second holds the symbol
	}

      // doesn't already exist;
      symbols[symbol_in->GetName()] = symbol_in;
    }
    
    std::string KnowledgeBase::FindUniqueName(const std::string &base_name)
    {
      std::string unique_name = base_name;
      std::map<std::string, Symbol *>::const_iterator symbol_it;
      symbol_it = symbols.find(unique_name);
      while(symbol_it != symbols.end())
	{ // This variable name is already used.  Make a new one.
	  char buf[16];
	  unique_count ++;
	  std::sprintf(buf, "%d", unique_count);
	  unique_name = base_name + "_" + buf;
	  symbol_it = symbols.find(unique_name);
	}
      return unique_name;
    }
    
    const std::map<std::string, Symbol *> &KnowledgeBase::GetSymbols() const
    {
      return symbols;
    }
    const std::vector<Sentence *> &KnowledgeBase::GetSentences() const
    {
      return sentences;
    }
  
    std::ostream & operator<<(std::ostream &os, const KnowledgeBase &kb)
    {
      const std::map<std::string, Symbol *> &symbols   = kb.GetSymbols();
      const std::vector<Sentence *>         &sentences = kb.GetSentences();

      std::map<std::string, Symbol *>::const_iterator sym_it = symbols.begin();
      std::vector<Sentence *>::const_iterator         sen_it = sentences.begin();

      while(sym_it != symbols.end())
	{
	  os << *(sym_it->second) << std::endl;
	  sym_it ++;
	}
      while(sen_it != sentences.end())
	{
	  os << *(*sen_it) << std::endl;
	  sen_it ++;
	}
    
      return os;
    }

  }
}
