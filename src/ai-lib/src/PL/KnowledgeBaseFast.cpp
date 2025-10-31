#include <ai_pl.h>
#include <list>
//#include <stack>

#define DEBUG_PRINT 0
#define DEBUG_DPLL 0
#define USE_PURE_SYMBOL 1
#define USE_UNIT_CLAUSE 1

namespace ai
{
  namespace PL
  {
    /*
     * Initially know nothing
     */
    KnowledgeBaseFast::KnowledgeBaseFast()
      : sentences(0)
    {
    }

    /*
     * Clean up the dynamic memory
     */
    KnowledgeBaseFast::~KnowledgeBaseFast()
    {
      unsigned int i;
      for(i = 0; i < sentences.size(); i++)
        {
          delete sentences[i];
        }

      std::map<std::string, Symbol *>::const_iterator it;
      for(it = symbols.begin(); it != symbols.end(); it++)
        {
          delete it->second;
        }
    }

    /* evaluates all sentences for the currently set model
     * returns KNOWN_TRUE if all sentences are true
     * returns KNOWN_FALSE if any one sentence is false
     * returns UNKNOWN if there are no false sentences but
     *         one or more are UNKNOWN
     */
    Symbol::State KnowledgeBaseFast::Evaluate() const
    {
      Symbol::State rval;
      bool have_unknown  = false;
      std::vector<Sentence *>::const_iterator sen_it = sentences.begin();

      while(sen_it != sentences.end())
        {
          rval = (*sen_it)->Evaluate();
          if(rval == Symbol::KNOWN_FALSE)
            {
              return rval;
            }
          else if(rval == Symbol::UNKNOWN)
            {
              have_unknown = true;
            }
          sen_it ++;
        }

      if(have_unknown)
        {
          return Symbol::UNKNOWN;
        }

      return Symbol::KNOWN_TRUE;
    }

    /* Assign values to all symbols using model_in
     * Destroys any previous assignments to symbols
     */
    void KnowledgeBaseFast::SetModel(const std::vector<Symbol *> &model_in)
    {
      std::vector<Symbol *>::const_iterator m_it;
      std::map<std::string, Symbol *>::const_iterator s_it;

      // set all symbols to UNKNOWN
      for(s_it = symbols.begin(); s_it != symbols.end(); s_it++)
        {
          s_it->second->SetValue(Symbol::UNKNOWN);
        }

      // set symbols that are in the model
      for(m_it = model_in.begin(); m_it != model_in.end(); m_it ++)
        {
          s_it = symbols.find((*m_it)->GetName());
          if(s_it != symbols.end())
            {
              s_it->second->SetValue((*m_it)->GetValue());
            }
        }
    }

    /* Defined in KnowledgeBase.(h|cpp)
    // Display a set of symbols
    std::ostream &operator<<(std::ostream &os,
    const std::vector<Symbol *> &symbols)
    {
    unsigned int i;
    os << "Model" << std::endl;
    for(i = 0; i < symbols.size(); i++)
    {
    os << *(symbols[i]) << "  ";
    }
    os << std::endl;
    return os;
    }
    */

    /* The Truth Table Entailment method
     * Enumerate all models of KB.
     * Returns KNOWN_TRUE if alpha is true in all models of KB.
     * Returns KNOWN_FALSE if alpha is false in any model of KB.
     * Returns UNKNOWN otherwise.
     */
    Symbol::State KnowledgeBaseFast::TT_Entails(KnowledgeBaseFast &alpha)
    {
      Symbol::State rval;
      std::vector<PL::Symbol *> model;

      // Create a model with all symbols
      std::map<std::string, Symbol *>::const_iterator it;
      std::map<std::string, Symbol *>::const_iterator temp_it;
      for(it = symbols.begin(); it != symbols.end(); it++)
        {
          model.push_back(new PL::Symbol(it->second->GetName()));
        }
      for(it = alpha.symbols.begin(); it != alpha.symbols.end(); it++)
        {
          temp_it = symbols.find(it->second->GetName());
          if(temp_it == symbols.end())
            { // don't add duplicates
              model.push_back(new PL::Symbol(it->second->GetName()));
            }
        }

      // Set all symbols in model to UNKNOWN
      unsigned int i;
      for(i = 0; i < model.size(); i++)
        {
          model[i]->SetValue(Symbol::UNKNOWN);
        }
      count = 0;
      // Call recursive method
      rval = TT_Check_All(alpha, model, 0);

      // Clean up dynamic memory
      for(i = 0; i < model.size(); i++)
        {
          delete model[i];
          model[i] = 0;
        }
      return rval;
    }

    /* The Truth Table Entailment worker method
     * Enumerate all models of KB.
     * Returns KNOWN_TRUE if alpha is true in all models of KB.
     * Returns KNOWN_FALSE if alpha is false in any model of KB.
     * Returns UNKNOWN otherwise.
     */
    Symbol::State KnowledgeBaseFast::TT_Check_All(KnowledgeBaseFast &alpha,
                                                  std::vector<Symbol *> &model,
                                                  unsigned int pos)
    {
      Symbol::State rval;

      // if the model is completely assigned (base case)
      if(pos >= model.size())
        {
          count ++;
          // Evaluate the KB to see if this is a model of KB
          SetModel(model);
          rval = Evaluate();

          // if it is a model of KB
          if(rval == Symbol::KNOWN_TRUE)
            {
#if DEBUG_PRINT
              std::cout << std::endl
                        << "*** KB is TRUE" << std::endl
                        << model << std::endl << std::endl;
#endif

              // Check if it is also a model of alpha
              alpha.SetModel(model);
              rval = alpha.Evaluate();
              return rval;
            }
          // If this is not a model of KB, then no need to check alpha
          return Symbol::KNOWN_TRUE;
        }

      // For the next symbol, try all possible values in the model
      Symbol::State p;
      for(p = Symbol::KNOWN_TRUE; p <= Symbol::KNOWN_FALSE; p++)
        {
          model[pos]->SetValue(p);
          rval = TT_Check_All(alpha, model, pos + 1);
          if(rval != Symbol::KNOWN_TRUE)
            { /* Symbol::KNOWN_FALSE or Symbol::UNKNOWN */
              return rval;
            }
        }
      return Symbol::KNOWN_TRUE;
    }


    /* Truth Table Entailment method, with tree pruning.
     * Same as Truth Table Entailment, except it has the
     * ability to stop exploring branches where the outcome
     * is known in advance.
     */
    Symbol::State KnowledgeBaseFast::TT_Entails_Pruned(KnowledgeBaseFast &alpha)
    {
      Symbol::State rval;
      std::vector<PL::Symbol *> model;

      std::map<std::string, Symbol *>::const_iterator it;
      std::map<std::string, Symbol *>::const_iterator temp_it;
      for(it = symbols.begin(); it != symbols.end(); it++)
        {
          model.push_back(new PL::Symbol(it->second->GetName()));
        }
      for(it = alpha.symbols.begin(); it != alpha.symbols.end(); it++)
        {
          temp_it = symbols.find(it->second->GetName());
          if(temp_it == symbols.end())
            { // don't add duplicates
              model.push_back(new PL::Symbol(it->second->GetName()));
            }
        }

      unsigned int i;
      for(i = 0; i < model.size(); i++)
        {
          model[i]->SetValue(Symbol::UNKNOWN);
        }
      count = 0;
      rval = TT_Check_All_Pruned(alpha, model, 0);

      for(i = 0; i < model.size(); i++)
        {
          delete model[i];
          model[i] = 0;
        }
      return rval;
    }

    Symbol::State KnowledgeBaseFast::TT_Check_All_Pruned(KnowledgeBaseFast &alpha,
                                                         std::vector<Symbol *> &model,
                                                         unsigned int pos)
    {
      /*
       * If the prune is successful, be sure to set the
       * model information back to UNKNOWN so that future
       * tests down other branches will not mistakenly
       * use the information.
       *
       * This is the reason for the SetValue(Symbol::UNKNOWN)
       * calls before the returns.
       */
      Symbol::State rval;


      /* Check if the possibly incomplete model is known to be a model or not
       * a model of KB.
       * If so, check if it is known for alpha.
       * If it is known for both, then prune the search tree.
       */
      count ++;
      SetModel(model);
      rval = Evaluate();

      if(rval == Symbol::KNOWN_TRUE)
        {
          alpha.SetModel(model);
          rval = alpha.Evaluate();


          if(rval == Symbol::KNOWN_TRUE)
            {
              if(pos > 0) { model[pos-1]->SetValue(Symbol::UNKNOWN); }
              return rval;
            }
          else if(rval == Symbol::KNOWN_FALSE)
            {
              if(pos > 0) { model[pos-1]->SetValue(Symbol::UNKNOWN); }
              return rval;
            }
          else
            { // do nothing, need to expand the model further
            }
        }
      else if(rval == Symbol::KNOWN_FALSE)
        { // This model doesn't satisfy the knowledge base
          if(pos > 0) { model[pos-1]->SetValue(Symbol::UNKNOWN); }
          return Symbol::KNOWN_TRUE;
        }
      else
        {// do nothing, need to expand the model further
        }

      // Try all possible values for the next symbol
      Symbol::State p;
      for(p = Symbol::KNOWN_TRUE; p <= Symbol::KNOWN_FALSE; p++)
        {
          model[pos]->SetValue(p);
          rval = TT_Check_All_Pruned(alpha, model, pos + 1);
          if(rval != Symbol::KNOWN_TRUE)
            { /* Symbol::KNOWN_FALSE */
              return rval;
            }
        }
      // if we reached here both recursive calls returned KNOWN_TRUE
      if(pos > 0) { model[pos-1]->SetValue(Symbol::UNKNOWN); }
      return Symbol::KNOWN_TRUE;
    }


    /* Defined in KnowledgeBase.(h|cpp)
    // Display a list of Sentences
    std::ostream &operator<<(std::ostream &os,
    const std::list<Sentence *> &sentences)
    {
    std::list<Sentence *>::const_iterator op         = sentences.begin();
    std::list<Sentence *>::const_iterator op_end     = sentences.end();
    os << "Sentences:" << std::endl;
    while(op != op_end)
    {
    os << *(*op) << "  ";
    op++;
    }
    os << std::endl;
    return os;
    }
    */

    static void ApplyOperatorPrecedence(std::list<Sentence *> &op_stack,
                                        std::list<Sentence *> &postfix,
                                        std::list<Sentence *>::iterator op_first,
                                        std::list<Sentence *>::iterator op_end
                                        )
    {
#define PRECEDENCE_DEBUG 0
#if PRECEDENCE_DEBUG
      std::cout << "START " << std::endl;
      std::cout << op_stack;
      std::cout << postfix << std::endl;
#endif
      /* find highest precedence operator */
      //std::list<Sentence *>::iterator op         = op_stack.begin();
      //std::list<Sentence *>::iterator op_end     = op_stack.end();

      std::list<Sentence *>::iterator op = op_first;
      std::list<Sentence *>::iterator op_highest = op;
      op++;
      while(op != op_end)
        {
          if((*op)->GetOperator() < (*op_highest)->GetOperator())
            { // lower number => higher precedence
              op_highest = op;
            }
          op++;
        }
#if PRECEDENCE_DEBUG
      std::cout << "highest: " << *(*op_highest) << std::endl;
#endif
      /* now find operand(s) that go with the operator */
      std::list<Sentence *>::iterator rop        = op_stack.end();
      std::list<Sentence *>::iterator rpf        = postfix.end();
      rop--;
      rpf--;
      while(rop != op_highest)
        {
          if((*rop)->GetType() == Sentence::T_UNARY)
            {
#if PRECEDENCE_DEBUG
              std::cout << "skipping: " << *(*rop) << " " << *(*rpf) << std::endl;
#endif

              rpf--;
              rop--;
            }
          else if((*rop)->GetType() == Sentence::T_BINARY)
            {
#if PRECEDENCE_DEBUG
              std::cout << "skipping: " << *(*rop) << " " << *(*rpf) << std::endl;
              //rpf--;
              //std::cout << " " << *(*rpf) << std::endl;
#endif
              rpf--;
              rop--;
            }
        }

      /* now rpf should hold the rhs to rop, which is op_highest
       * consolidate the operator and operand(s), insert into correct spot
       */
      Sentence *s = (*rop);
      op_stack.erase(rop);

      if(s->GetType() == Sentence::T_UNARY)
        {
          Sentence *rhs = (*rpf);
          s->SetRHS(rhs);

          postfix.insert(rpf, s);
          postfix.erase(rpf);
        }
      else if(s->GetType() == Sentence::T_BINARY)
        {
          std::list<Sentence *>::iterator rpf1 = rpf;
          rpf1--;

          Sentence *rhs = (*rpf);
          Sentence *lhs = *(rpf1);
          s->SetLHS(lhs);
          s->SetRHS(rhs);

          postfix.insert(rpf1, s);
          postfix.erase(rpf1);
          postfix.erase(rpf);
        }
      else
        {
          std::cout << "error in op_stack contents" << std::endl;
        }

#if PRECEDENCE_DEBUG
      std::cout << op_stack;
      std::cout << postfix;
      std::cout << "END " << std::endl << std::endl;
#endif
    }




    /* Parse an input string, add as sentence to the KB
     *
     * FIXME: This parser requires parens to inforce precedence!
     */
    void KnowledgeBaseFast::AddSentence(const std::string &s_in)
    {
      /* Symbol     := "[a-zA-Z][a-zA-Z0-9_]*"
       * Operators  := !   -- NOT
       *               &   -- AND
       *               |   -- OR
       *               =>  -- IMPLICATION
       *               <=> -- BICONDITIONAL
       *
       * Parens can be used override precedence, or left-to-right processing
       * Whitespace is ignored
       */

      std::string::const_iterator it     = s_in.begin();
      std::string::const_iterator it_end = s_in.end();
      std::string::const_iterator token_start;
      std::string::const_iterator token_end;

      std::list<Sentence *>      postfix;
      std::list<Sentence *>      op_stack;

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
              while((it != it_end) && (isalnum(*it) || *it == '_'))
                {
                  it ++;
                  token_end = it; // token_end is really 1 past the end.
                }
              std::string sym(token_start, token_end);
              Symbol   *symbol   = AddSymbol(sym);
              Sentence *sentence = new Sentence(symbol);
              postfix.push_back(sentence);
            }
          else if(*it == '(')
            { //
              Sentence *sentence = new Sentence(Sentence::T_LEFT_PAREN);
              op_stack.push_back(sentence);
              it++;
            }
          else if(*it == ')')
            { //
              while(op_stack.back()->GetType() != Sentence::T_LEFT_PAREN)
                {
                  std::list<Sentence *>::iterator rop = op_stack.end();
                  rop--;
                  while((*rop)->GetType() != Sentence::T_LEFT_PAREN)
                    {
                      rop--;
                    }
                  rop++; // Do not allow the left-paren into consideration
                  ApplyOperatorPrecedence(op_stack, postfix, rop, op_stack.end());
                }
              // delete the left-paren from the operator stack
              delete op_stack.back();
              op_stack.pop_back();
              it++;
            }
          else if(*it == '!')
            { //
              Sentence *sentence = new Sentence(Sentence::T_UNARY,
                                                Sentence::O_NOT);
              op_stack.push_back(sentence);
              it++;
            }
          else if(*it == '&')
            { //
              Sentence *sentence = new Sentence(Sentence::T_BINARY,
                                                Sentence::O_AND);
              op_stack.push_back(sentence);
              it++;
            }
          else if(*it == '|')
            { //
              Sentence *sentence = new Sentence(Sentence::T_BINARY,
                                                Sentence::O_OR);
              op_stack.push_back(sentence);
              it++;
            }
          else if((*it == '=') && (*(it+1) == '>'))
            { //
              Sentence *sentence = new Sentence(Sentence::T_BINARY,
                                                Sentence::O_IMPLIES);
              op_stack.push_back(sentence);
              it+=2;
            }
          else if((*it == '<') && (*(it+1) == '=') && (*(it+2) == '>'))
            { //
              Sentence *sentence = new Sentence(Sentence::T_BINARY,
                                                Sentence::O_IFF);
              op_stack.push_back(sentence);
              it+=3;
            }
          else
            { // skipping for now
              std::cout << "ERROR! Skipping unknown character: " << *it << std::endl;
              it++;
            }
        }

      while(!op_stack.empty())
        {
          ApplyOperatorPrecedence(op_stack, postfix, op_stack.begin(), op_stack.end());
        }

      // SHOULD ONLY BE ONE SENTENCE IN postfix HERE
      while(!postfix.empty())
        {
          Sentence *s = postfix.back();
          postfix.pop_back();
          sentences.push_back(s);
        }
    }

    /* Add the string as a new symbol in the KB, unless already exists.
     */
    Symbol *KnowledgeBaseFast::AddSymbol(const std::string &s_in)
    {
      std::map<std::string, Symbol *>::const_iterator it;
      it = symbols.find(s_in);
      if(it != symbols.end())
        {
          return it->second;
        }

      // doesn't already exist;
      Symbol *s = new Symbol(s_in);
      symbols[s_in] = s;
      return s;
    }

    /* DPLL Entailment method.
     *
     *
     */
    Symbol::State KnowledgeBaseFast::DPLL_Entails(KnowledgeBaseFast &alpha)
    {
      unsigned int i;
      Symbol::State rval = Symbol::UNKNOWN;
      std::vector<Sentence *> unknown_clauses;
      std::vector<Sentence *> true_clauses;
      std::vector<Symbol *>   assigned_symbols;
      std::vector<Symbol *>   unassigned_symbols;

      /* Copy my sentences */
      for(i = 0; i < sentences.size(); i++)
        {
          Sentence *s_tmp = new Sentence(*(sentences[i]));
          s_tmp->CNF();
          Sentence::SplitClauses(s_tmp, unknown_clauses);
        }

      /* Copy question sentence */
      if(alpha.sentences.size() != 1)
        {
          std::cerr << "Not designed for questions with more than one sentence." << std::endl;
          return Symbol::UNKNOWN;
        }
      { /* Negate question to go for satisfiability */
        Sentence *s_tmp = new Sentence(*(alpha.sentences[0]));
        Sentence *s_tmp_not = new Sentence(Sentence::O_NOT, s_tmp);
        s_tmp_not->CNF();
        Sentence::SplitClauses(s_tmp_not, unknown_clauses);
      }
#if DEBUG_DPLL
      std::cout << "CLAUSES" << std::endl;
      for(i = 0; i < unknown_clauses.size(); i++)
        {
          std::cout << *(unknown_clauses[i]) << std::endl;
        }
#endif

      /* Make array of my symbols */
      std::map<std::string, Symbol *>::const_iterator sym_it = symbols.begin();
      while(sym_it != symbols.end())
        {
          unassigned_symbols.push_back(new PL::Symbol(sym_it->second->GetName()));
          sym_it ++;
        }

      /* Copy question symbols */
      sym_it = alpha.symbols.begin();
      std::map<std::string, Symbol *>::const_iterator s_it;
      while(sym_it != alpha.symbols.end())
        {
          s_it = symbols.find((sym_it->second)->GetName());
          if(s_it == symbols.end())
            {
              unassigned_symbols.push_back(new PL::Symbol(sym_it->second->GetName()));
            }
          sym_it ++;
        }

      /* Set initial model to unknown */
      for(i = 0; i < unassigned_symbols.size(); i++)
        {
          unassigned_symbols[i]->SetValue(Symbol::UNKNOWN);
        }

      // OK because unassigned should be set to UNKNOWN anyway.
      SetModel(assigned_symbols);
      alpha.SetModel(assigned_symbols);

      count = 0;
      //
      rval = DPLL_Aux(alpha,
                      true_clauses,
                      unknown_clauses,
                      assigned_symbols,
                      unassigned_symbols);
      //

#if DEBUG_DPLL
      std::cout << "rval: " << rval << std::endl;
#endif
      for(i = 0; i < unknown_clauses.size(); i++)
        {
          delete unknown_clauses[i];
        }
      for(i = 0; i < true_clauses.size(); i++)
        {
          delete true_clauses[i];
        }

      if(rval == Symbol::KNOWN_TRUE)
        { // negation was satisfiable, question is not provable
          return Symbol::UNKNOWN;
        }
      else
        { // negation was unsatisfiable, question is proved.
          return Symbol::KNOWN_TRUE;
        }
    }

    Symbol::State KnowledgeBaseFast::DPLL_ExtendOne(Symbol::State new_value,
                                                    KnowledgeBaseFast &alpha,
                                                    std::vector<Sentence *> &true_clauses,
                                                    std::vector<Sentence *> &unknown_clauses,
                                                    std::vector<Symbol *>   &assigned_symbols,
                                                    std::vector<Symbol *>   &unassigned_symbols
                                                    )
    {
      Symbol::State r;
      Symbol *t_sym;

      t_sym = unassigned_symbols[unassigned_symbols.size()-1];
      t_sym->SetValue(new_value);

      unassigned_symbols.pop_back();
      assigned_symbols.push_back(t_sym);

      r = DPLL_Aux(alpha,
                   true_clauses, unknown_clauses,
                   assigned_symbols, unassigned_symbols);

      t_sym->SetValue(Symbol::UNKNOWN);

      unassigned_symbols.push_back(t_sym);
      assigned_symbols.pop_back();

      return r;
    }

    void KnowledgeBaseFast::DPLL_Unmove(std::vector<Sentence *> &true_clauses,
                                        std::vector<Sentence *> &unknown_clauses,
                                        int moved_clauses)
    {
      int i;
      for(i = 0; i < moved_clauses; i++)
        {
#if DEBUG_PRINT
          std::cout << "UNMOVE: " << *(true_clauses[true_clauses.size() - 1]) << std::endl;
#endif
          unknown_clauses.push_back(true_clauses[true_clauses.size() - 1]);
          true_clauses.pop_back();
        }
    }

    Symbol::State KnowledgeBaseFast::DPLL_Evaluate(std::vector<Sentence *> &true_clauses,
                                                   std::vector<Sentence *> &unknown_clauses,
                                                   int &moved_clauses)
    {
      Symbol::State rval;
      moved_clauses = 0;

      count ++;

      unsigned int i;
      for(i = 0; i < unknown_clauses.size(); i++)
        {
          rval = unknown_clauses[i]->Evaluate();

          if(rval == Symbol::KNOWN_FALSE)
            { // no point in continuing
              return Symbol::KNOWN_FALSE;
            }
          else if(rval == Symbol::KNOWN_TRUE)
            { // add to true_clauses
#if DEBUG_PRINT
              std::cout << "MOVE: " << *(unknown_clauses[i]) << std::endl;
#endif
              true_clauses.push_back(unknown_clauses[i]);
              unknown_clauses[i] = unknown_clauses[unknown_clauses.size() - 1];
              unknown_clauses.pop_back();
              moved_clauses ++;
              i --;
            }
          else
            { // go to next clause
            }
        }
      if(unknown_clauses.size() == 0)
        {
          return Symbol::KNOWN_TRUE;
        }
      return Symbol::UNKNOWN;
    }

    void KnowledgeBaseFast::DPLL_PureSymbols(std::vector<Sentence *> &unknown_clauses,
                                             std::vector<Symbol *>   &unassigned_symbols,
                                             std::vector<Symbol *>   &pure_symbols)
    {
      std::map<std::string, Symbol *> my_symbol_map;
      std::vector<Symbol *>::iterator sym_it;
      unsigned int i,j;

      // iterate over unassigned, set all to UNKNOWN
      for(sym_it = unassigned_symbols.begin(); sym_it != unassigned_symbols.end(); sym_it++)
        {
          (*sym_it)->SetValue(Symbol::UNKNOWN);
          my_symbol_map[(*sym_it)->GetName()] = (*sym_it);
        }

      /* iterate over unknown_clauses
       * for each symbol in the clause
       *   if the symbol is negated, and symbol is set to UNKNOWN, or FALSE, set to FALSE
       *   if the symbol is negated, and symbol is set to TRUE, set to STATE_MIN --> unpure
       *   if the symbol is not negated, and symbol is set to UNKNOWN, or TRUE, set to TRUE
       *   if the symbol is not negated, and symbol is set to FALSE, set to STATE_MIN --> unpure
       */
      std::vector<Symbol *> asserted;
      std::vector<Symbol *> negated;
      Symbol::State v;
      for(i = 0; i < unknown_clauses.size(); i++)
        {
          asserted.clear();
          negated.clear();
          unknown_clauses[i]->FindPure(asserted, negated);
#if DEBUG_PRINT
          if(asserted.size() > 0 || negated.size() > 0)
            {
              std::cout << *(unknown_clauses[i]) << std::endl;
            }
#endif

          for(j = 0; j < negated.size(); j++)
            {
#if DEBUG_PRINT
              std::cout << "N: " << negated[j]->GetName() << std::endl;
#endif

              v = my_symbol_map[negated[j]->GetName()]->GetValue();
              switch(v)
                {
                case Symbol::STATE_MIN:
                  /* do nothing, already has both states */
                  break;
                case Symbol::KNOWN_FALSE:
                  /* do nothing, already is false*/
                  break;
                case Symbol::UNKNOWN:
                  my_symbol_map[negated[j]->GetName()]->SetValue(Symbol::KNOWN_FALSE);
                  break;
                case Symbol::KNOWN_TRUE:
                  my_symbol_map[negated[j]->GetName()]->SetValue(Symbol::STATE_MIN);
                  break;
                default:
                  std::cerr << "unexpected value"  << std::endl;
                  { int *i = 0; (*i) = 0; }
                  break;
                }
            }
          for(j = 0; j < asserted.size(); j++)
            {
#if DEBUG_PRINT
              std::cout << "P: " << asserted[j]->GetName() << std::endl;
#endif
              v = my_symbol_map[asserted[j]->GetName()]->GetValue();
              switch(v)
                {
                case Symbol::STATE_MIN:
                  /* do nothing, already has both states */
                  break;
                case Symbol::KNOWN_TRUE:
                  /* do nothing, already is true */
                  break;
                case Symbol::UNKNOWN:
                  my_symbol_map[asserted[j]->GetName()]->SetValue(Symbol::KNOWN_TRUE);
                  break;
                case Symbol::KNOWN_FALSE:
                  my_symbol_map[asserted[j]->GetName()]->SetValue(Symbol::STATE_MIN);
                  break;
                default:
                  std::cerr << "unexpected value"  << std::endl;
                  { int *i = 0; (*i) = 0; }
                  break;
                }
            }

        }

      // iterate over unassigned, any that are TRUE or FALSE are pure, move them there
      for(i = 0; i < unassigned_symbols.size(); i++)
        {
          if((unassigned_symbols[i]->GetValue() == Symbol::KNOWN_TRUE) ||
             (unassigned_symbols[i]->GetValue() == Symbol::KNOWN_FALSE))
            {
              pure_symbols.push_back(unassigned_symbols[i]);
              unassigned_symbols[i] = unassigned_symbols[unassigned_symbols.size()-1];
              unassigned_symbols.pop_back();
              i--;
            }
        }

      for(sym_it = unassigned_symbols.begin(); sym_it != unassigned_symbols.end(); sym_it++)
        {
          (*sym_it)->SetValue(Symbol::UNKNOWN);
        }
    }



    Symbol::State KnowledgeBaseFast::DPLL_Aux(KnowledgeBaseFast &alpha,
                                              std::vector<Sentence *> &true_clauses,
                                              std::vector<Sentence *> &unknown_clauses,
                                              std::vector<Symbol *>   &assigned_symbols,
                                              std::vector<Symbol *>   &unassigned_symbols)
    {
      // OK because unassigned should be set to UNKNOWN anyway.
      SetModel(assigned_symbols);
      alpha.SetModel(assigned_symbols);

#if DEBUG_PRINT
      {
        unsigned int i;
        std::cout << "Clauses: " << std::endl;
        for(i = 0; i < true_clauses.size(); i++)
          {
            std::cout << "T: " << *(true_clauses[i]) << " : " << true_clauses[i]->Evaluate() << std::endl;
          }
        std::cout << std::endl;
        for(i = 0; i < unknown_clauses.size(); i++)
          {
            std::cout << "U: " << *(unknown_clauses[i])<< " : " << unknown_clauses[i]->Evaluate()  << std::endl;
          }
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "Model: " << std::endl;

        for(i = 0; i < assigned_symbols.size(); i++)
          {
            std::cout << *(assigned_symbols[i]) << " ";
          }
        std::cout << std::endl;
        for(i = 0; i < unassigned_symbols.size(); i++)
          {
            std::cout << *(unassigned_symbols[i]) << " ";
          }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
      }
#endif

      int moved_clauses;
      Symbol::State rval;
      rval = DPLL_Evaluate(true_clauses, unknown_clauses, moved_clauses);
      if(rval == Symbol::KNOWN_TRUE)
        {
#if DEBUG_DPLL
          {
            unsigned int i;
            std::cout << "TRUE MODEL: " << std::endl;

            for(i = 0; i < assigned_symbols.size(); i++)
              {
                std::cout << *(assigned_symbols[i]) << " ";
              }
            std::cout << std::endl;
            for(i = 0; i < unassigned_symbols.size(); i++)
              {
                std::cout << *(unassigned_symbols[i]) << " ";
              }
            std::cout << std::endl;
            std::cout << std::endl;
          }
#endif
          DPLL_Unmove(true_clauses, unknown_clauses, moved_clauses);
          return Symbol::KNOWN_TRUE;
        }
      else if(rval == Symbol::KNOWN_FALSE)
        {
#if DEBUG_DPLL
          {
            unsigned int i;
            std::cout << "FALSE MODEL: " << std::endl;

            for(i = 0; i < assigned_symbols.size(); i++)
              {
                std::cout << *(assigned_symbols[i]) << " ";
              }
            std::cout << std::endl;
            for(i = 0; i < unassigned_symbols.size(); i++)
              {
                std::cout << *(unassigned_symbols[i]) << " ";
              }
            std::cout << std::endl;
            std::cout << std::endl;
          }
#endif
          DPLL_Unmove(true_clauses, unknown_clauses, moved_clauses);
          return Symbol::KNOWN_FALSE;
        }

#if USE_PURE_SYMBOL
      std::vector<Symbol *>    pure_symbols;
      size_t                   pure_count;
#if DEBUG_PRINT
      std::cout << std::endl;
#endif
      DPLL_PureSymbols(unknown_clauses, unassigned_symbols, pure_symbols);
      pure_count = pure_symbols.size();
      if(pure_count > 0)
        {
          size_t i;
          for(i = 0; i < pure_count; i++)
            {
#if DEBUG_PRINT
              std::cout << "PURE: " << pure_symbols[i]->GetName() << ":" << pure_symbols[i]->GetValue() << std::endl;
#endif
              assigned_symbols.push_back(pure_symbols[i]);
            }


          rval = DPLL_Aux(alpha, true_clauses, unknown_clauses,
                          assigned_symbols, unassigned_symbols);
          for(i = 0; i < pure_count; i++)
            {
              assigned_symbols.pop_back();
              unassigned_symbols.push_back(pure_symbols[i]);
              pure_symbols[i]->SetValue(Symbol::UNKNOWN);
            }
          DPLL_Unmove(true_clauses, unknown_clauses, moved_clauses);
          return rval;
        }
#endif
#if USE_UNIT_CLAUSE
      {
        bool found;
        unsigned int i;
        Symbol *unit_symbol = 0;
        for(i = 0; i < unknown_clauses.size(); i++)
          {
            if(unknown_clauses[i]->IsUnitClause(unit_symbol))
              {
                break;
              }
          }
        if(unit_symbol != 0)
          {
            found = false;
            for(i = 0; i < unassigned_symbols.size(); i++)
              {
                if(unassigned_symbols[i]->GetName() == unit_symbol->GetName())
                  {
                    unassigned_symbols[i]->SetValue(unit_symbol->GetValue());
                    assigned_symbols.push_back(unassigned_symbols[i]);
                    unassigned_symbols[i] = unassigned_symbols[unassigned_symbols.size() - 1];
                    unassigned_symbols.pop_back();
                    found = true;
                    break;
                  }
              }
            if(found)
              {

                rval = DPLL_Aux(alpha, true_clauses, unknown_clauses,
                                assigned_symbols, unassigned_symbols);

                unassigned_symbols.push_back(assigned_symbols[assigned_symbols.size()-1]);
                assigned_symbols.pop_back();
                unassigned_symbols[unassigned_symbols.size()-1]->SetValue(Symbol::UNKNOWN);

                DPLL_Unmove(true_clauses, unknown_clauses, moved_clauses);
                return rval;
              }
            else
              {
                std::cerr << "unexpected results" << std::endl;
              }
          }

      }
#endif


      if((DPLL_ExtendOne(Symbol::KNOWN_TRUE,
                         alpha,
                         true_clauses, unknown_clauses,
                         assigned_symbols, unassigned_symbols) == Symbol::KNOWN_TRUE) ||
         (DPLL_ExtendOne(Symbol::KNOWN_FALSE,
                         alpha,
                         true_clauses, unknown_clauses,
                         assigned_symbols, unassigned_symbols) == Symbol::KNOWN_TRUE))
        {
          DPLL_Unmove(true_clauses, unknown_clauses, moved_clauses);
          return Symbol::KNOWN_TRUE;
        }
      else
        {
          DPLL_Unmove(true_clauses, unknown_clauses, moved_clauses);
          return Symbol::UNKNOWN;
        }
    }



    const std::map<std::string, Symbol *> &KnowledgeBaseFast::GetSymbols() const
    {
      return symbols;
    }
    const std::vector<Sentence *> &KnowledgeBaseFast::GetSentences() const
    {
      return sentences;
    }
    int KnowledgeBaseFast::GetCount() const
    {
      return count;
    }

    std::ostream & operator<<(std::ostream &os, const KnowledgeBaseFast &kb)
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
