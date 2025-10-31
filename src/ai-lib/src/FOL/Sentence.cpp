#include <ai_fol.h>
#include <cstdio>

namespace ai
{
  namespace FOL
  {
    /* Sentence 0 */
    Sentence0::Sentence0(Sentence0 *lhs_in, Sentence1 *rhs_in)
      : lhs(lhs_in), rhs(rhs_in)
    {
    }
    Sentence0::Sentence0(Sentence1 *rhs_in)
      : lhs(0), rhs(rhs_in)
    {
    }
    Sentence0::Sentence0(const Sentence0 &original_in)
      : lhs(0), rhs(0)
    {
      if(original_in.lhs)
	{
	  lhs = new Sentence0(*original_in.lhs);
	}
      if(original_in.rhs)
	{
	  rhs = new Sentence1(*original_in.rhs);
	}
    }
    Sentence0::Sentence0(Sentence2 *s2_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence1(s2_in);
    }
    Sentence0::Sentence0(Sentence3 *s3_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence1(s3_in);
    }
    Sentence0::~Sentence0()
    {
      if(lhs)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    Sentence0 *Sentence0::GetLHS()
    {
      return lhs;
    }
    Sentence1 *Sentence0::GetRHS()
    {
      return rhs;
    }
    
    void Sentence0::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(lhs)
	{
	  lhs->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }
    
    void Sentence0::EliminateBiconditional()
    {
      if(lhs)
	{
	  lhs->EliminateBiconditional();
	}
      if(rhs)
	{
	  rhs->EliminateBiconditional();
	}
      if(lhs && rhs)
	{ /* At this point, lhs should be a Sentence0 with only rhs left,
	   * rhs should be a Sentence1
	   * Need to follow this rule: (A <=> B) goes to (A => B) & (B => A)
	   * Need to construct this structure with A == lhs->rhs and B == rhs
	   * Build the trees from the bottom up.
	   *
	   */

	  Sentence3 *LHS = 0;
	  Sentence4 *RHS = 0;
	  // (A => B)
	  {
	    Sentence1 *A = new Sentence1(*(lhs->rhs));
	    Sentence1 *B = new Sentence1(*rhs);
	    
	    Sentence1 *s1 = new Sentence1(A, new Sentence2(B)) ;
	    Sentence0 *s0 = new Sentence0(s1);
	    Sentence5 *s5 = new Sentence5(s0);
	    Sentence4 *s4 = new Sentence4(s5);
	    Sentence3 *s3 = new Sentence3(s4);
	    LHS = s3;
	  }

	  // (B => A)
	  {
	    Sentence1 *A = new Sentence1(*(lhs->rhs));
	    Sentence1 *B = new Sentence1(*rhs);
	    
	    Sentence1 *s1 = new Sentence1(B, new Sentence2(A)) ;
	    Sentence0 *s0 = new Sentence0(s1);
	    Sentence5 *s5 = new Sentence5(s0);
	    Sentence4 *s4 = new Sentence4(s5);
	    RHS = s4;
	  }

	  Sentence3 *s3 = new Sentence3(LHS, RHS);
	  Sentence2 *s2 = new Sentence2(s3);
	  Sentence1 *s1 = new Sentence1(s2);

	  delete lhs;
	  lhs = 0;
	  delete rhs;
	  rhs = s1;
	}
    }
    
    void Sentence0::EliminateImplication()
    {
      if(lhs)
	{
	  lhs->EliminateImplication();
	}
      if(rhs)
	{
	  rhs->EliminateImplication();
	}
    }

    void Sentence0::MoveNotIn()
    {
      if(lhs)
	{
	  lhs->MoveNotIn();
	}
      if(rhs)
	{
	  rhs->MoveNotIn();
	}
    }

    void Sentence0::EliminateExtraParens()
    {
      if(lhs)
	{
	  lhs->EliminateExtraParens();
	}
      if((rhs) && !(lhs) && /* Sentence1 */
	 (rhs->rhs) && !(rhs->lhs) && /* Sentence2 */
	 (rhs->rhs->rhs) && !(rhs->rhs->lhs) && /* Sentence3 */
	 (rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->lhs) && /* Sentence4 */
	 (rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->lhs) && /* Sentence5 */
	 (rhs->rhs->rhs->rhs->rhs->rhs) && /* Sentence0 */
	 !(rhs->rhs->rhs->rhs->rhs->variable_list) /* VariableList */
	 )
	{ /* our rhs is just a loop */
	  Sentence0 *child = rhs->rhs->rhs->rhs->rhs->rhs;
	  Sentence0 *new_lhs = 0;
	  Sentence1 *new_rhs = 0;
	  if(child->lhs)
	    {
	      new_lhs = new Sentence0(*(child->lhs));
	    }
	  if(child->rhs)
	    {
	      new_rhs = new Sentence1(*(child->rhs));
	    }
	  if (lhs) { delete lhs; }
	  delete rhs;
	  lhs = new_lhs;
	  rhs = new_rhs;
	  EliminateExtraParens(); // recursive call now that we have changed.
	}
      if(rhs)
	{
	  rhs->EliminateExtraParens();
	}
    }
    
    SentenceType Sentence0::GetSentenceType() const
    {
      if(lhs)
	{
	  return T_IFF;
	}
      else if(rhs)
	{
	  return rhs->GetSentenceType();
	}
      return T_ERROR;
    }

    Sentence4 * Sentence0::GetNextNot()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextNot();
	}
      return 0;
    }
    
    Sentence3 * Sentence0::GetNextAnd()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextAnd();
	}
      return 0;
    }
    
    Sentence2 * Sentence0::GetNextOr()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextOr();
	}
      return 0;
    }
    
    std::string Sentence0::ToString(bool verbose) const
    {
      std::string str = "";
      if(lhs && rhs)
	{
	  str = lhs->ToString(verbose) + " <=> " + rhs->ToString(verbose);
	}
      else if(rhs)
	{
	  str = rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Sentence0";
	}
      return str;
    }

    /* Sentence 1 */
    Sentence1::Sentence1(Sentence1 *lhs_in, Sentence2 *rhs_in)
      : lhs(lhs_in), rhs(rhs_in)
    {
    }
    Sentence1::Sentence1(Sentence2 *rhs_in)
      : lhs(0), rhs(rhs_in)
    {
    }
    Sentence1::Sentence1(const Sentence1 &original_in)
      : lhs(0), rhs(0)
    {
      if(original_in.lhs)
	{
	  lhs = new Sentence1(*original_in.lhs);
	}
      if(original_in.rhs)
	{
	  rhs = new Sentence2(*original_in.rhs);
	}
    }
    Sentence1::Sentence1(Sentence3 *s3_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence2(s3_in);
    }
    Sentence1::~Sentence1()
    {
      if(lhs)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    Sentence1 *Sentence1::GetLHS()
    {
      return lhs;
    }
    Sentence2 *Sentence1::GetRHS()
    {
      return rhs;
    }
    
    void Sentence1::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(lhs)
	{
	  lhs->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }

    void Sentence1::EliminateBiconditional()
    {
      if(lhs)
	{
	  lhs->EliminateBiconditional();
	}
      if(rhs)
	{
	  rhs->EliminateBiconditional();
	}
    }
    
    void Sentence1::EliminateImplication()
    {
      if(lhs)
	{
	  lhs->EliminateImplication();
	}
      if(rhs)
	{
	  rhs->EliminateImplication();
	}
      if(lhs && rhs)
	{ /* At this point, the lhs->lhs should be 0 and lhs->rhs and rhs need to be joined as a Sentence2.
	   * Need to follow the rule: (A => B) goes to (!A | B)
	   * with A == lhs->rhs and B == rhs
	   * Build the trees from the bottom up.
	   */

	  Sentence4 *NOT_A = 0;
	  // !A
	  {
	    Sentence2 *A = new Sentence2(*(lhs->rhs));
	    Sentence4 *s4 = new Sentence4(A);
	    NOT_A = new Sentence4(s4);
	  }
	  Sentence2 *LHS = new Sentence2(NOT_A);
	  Sentence3 *RHS = new Sentence3(rhs);
	  rhs = new Sentence2(LHS, RHS);
	  delete lhs;
	  lhs = 0;
	}
    }

    void Sentence1::MoveNotIn()
    {
      if(lhs)
	{
	  lhs->MoveNotIn();
	}
      if(rhs)
	{
	  rhs->MoveNotIn();
	}
    }
    
    void Sentence1::EliminateExtraParens()
    {
      if(lhs)
	{
	  lhs->EliminateExtraParens();
	}
      if((rhs) && !(lhs) && /* Sentence2 */
	 (rhs->rhs) && !(rhs->lhs) && /* Sentence3 */
	 (rhs->rhs->rhs) && !(rhs->rhs->lhs) && /* Sentence4 */
	 (rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->lhs) && /* Sentence5 */
	 (rhs->rhs->rhs->rhs->rhs) && /* Sentence0 */
	 !(rhs->rhs->rhs->rhs->variable_list) && /* VariableList */
	 (rhs->rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->rhs->lhs) /* Sentence1 */
	 )
	{ /* our rhs is just a loop */
	  Sentence1 *child = rhs->rhs->rhs->rhs->rhs->rhs;
	  Sentence1 *new_lhs = 0;
	  Sentence2 *new_rhs = 0;
	  if(child->lhs)
	    {
	      new_lhs = new Sentence1(*(child->lhs));
	    }
	  if(child->rhs)
	    {
	      new_rhs = new Sentence2(*(child->rhs));
	    }
	  if (lhs) { delete lhs; }
	  delete rhs;
	  lhs = new_lhs;
	  rhs = new_rhs;
	  EliminateExtraParens(); // recursive call now that we have changed.
	}
      if(rhs)
	{
	  rhs->EliminateExtraParens();
	}
    }
    
    SentenceType Sentence1::GetSentenceType() const
    {
      if(lhs)
	{
	  return T_IMPLIES;
	}
      else if(rhs)
	{
	  return rhs->GetSentenceType();
	}
      return T_ERROR;
    }
    
    Sentence4 * Sentence1::GetNextNot()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextNot();
	}
      return 0;
    }
    
    Sentence3 * Sentence1::GetNextAnd()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextAnd();
	}
      return 0;
    }
    
    Sentence2 * Sentence1::GetNextOr()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextOr();
	}
      return 0;
    }
    
    std::string Sentence1::ToString(bool verbose) const
    {
      std::string str = "";
      if(lhs && rhs)
	{
	  str = lhs->ToString(verbose) + " => " + rhs->ToString(verbose);
	}
      else if(rhs)
	{
	  str = rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Sentence1";
	}
      return str;
    }


    /* Sentence 2 */
    Sentence2::Sentence2(Sentence2 *lhs_in, Sentence3 *rhs_in)
      : lhs(lhs_in), rhs(rhs_in)
    {
    }
    Sentence2::Sentence2(Sentence3 *rhs_in)
      : lhs(0), rhs(rhs_in)
    {
    }
    Sentence2::Sentence2(const Sentence2 &original_in)
      : lhs(0), rhs(0)
    {
      if(original_in.lhs)
	{
	  lhs = new Sentence2(*original_in.lhs);
	}
      if(original_in.rhs)
	{
	  rhs = new Sentence3(*original_in.rhs);
	}
    }
    Sentence2::Sentence2(Sentence1 *s1_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence3(s1_in);
    }
    Sentence2::Sentence2(Sentence4 *s4_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence3(s4_in);
    }
    Sentence2::~Sentence2()
    {
      if(lhs)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    Sentence2 *Sentence2::GetLHS()
    {
      return lhs;
    }
    Sentence3 *Sentence2::GetRHS()
    {
      return rhs;
    }
    
    void Sentence2::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(lhs)
	{
	  lhs->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }
    
    void Sentence2::EliminateBiconditional()
    {
      if(lhs)
	{
	  lhs->EliminateBiconditional();
	}
      if(rhs)
	{
	  rhs->EliminateBiconditional();
	}
    }
    
    void Sentence2::EliminateImplication()
    {
      if(lhs)
	{
	  lhs->EliminateImplication();
	}
      if(rhs)
	{
	  rhs->EliminateImplication();
	}
    }

    void Sentence2::MoveNotIn()
    {
      if(lhs)
	{
	  lhs->MoveNotIn();
	}
      if(rhs)
	{
	  rhs->MoveNotIn();
	}
    }
    
    void Sentence2::EliminateExtraParens()
    {
      if(lhs)
	{
	  lhs->EliminateExtraParens();
	}
      if((rhs) && !(lhs) && /* Sentence3 */
	 (rhs->rhs) && !(rhs->lhs) && /* Sentence4 */
	 (rhs->rhs->rhs) && !(rhs->rhs->lhs) && /* Sentence5 */
	 (rhs->rhs->rhs->rhs) && /* Sentence0 */
	 !(rhs->rhs->rhs->variable_list) && /* VariableList */
	 (rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->lhs) && /* Sentence1 */
	 (rhs->rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->rhs->lhs) /* Sentence2 */
	 )
	{ /* our rhs is just a loop */
	  Sentence2 *child = rhs->rhs->rhs->rhs->rhs->rhs;
	  Sentence2 *new_lhs = 0;
	  Sentence3 *new_rhs = 0;
	  if(child->lhs)
	    {
	      new_lhs = new Sentence2(*(child->lhs));
	    }
	  if(child->rhs)
	    {
	      new_rhs = new Sentence3(*(child->rhs));
	    }
	  if (lhs) { delete lhs; }
	  delete rhs;
	  lhs = new_lhs;
	  rhs = new_rhs;
	  EliminateExtraParens(); // recursive call now that we have changed.
	}
      if(rhs)
	{
	  rhs->EliminateExtraParens();
	}
    }
    
    SentenceType Sentence2::GetSentenceType() const
    {
      if(lhs)
	{
	  return T_OR;
	}
      else if(rhs)
	{
	  return rhs->GetSentenceType();
	}
      return T_ERROR;
    }

    Sentence4 * Sentence2::GetNextNot()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextNot();
	}
      return 0;
    }
    
    Sentence3 * Sentence2::GetNextAnd()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextAnd();
	}
      return 0;
    }
    
    Sentence2 * Sentence2::GetNextOr()
    {
      if(lhs)
	{
	  return this;
	}
      else if(rhs)
	{
	  return rhs->GetNextOr();
	}
      return 0;
    }

    
    std::string Sentence2::ToString(bool verbose) const
    {
      std::string str = "";
      if(lhs && rhs)
	{
	  str = lhs->ToString(verbose) + " | " + rhs->ToString(verbose);
	}
      else if(rhs)
	{
	  str = rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Sentence2";
	}
      return str;
    }


    /* Sentence 3 */
    Sentence3::Sentence3(Sentence3 *lhs_in, Sentence4 *rhs_in)
      : lhs(lhs_in), rhs(rhs_in)
    {
    }
    Sentence3::Sentence3(Sentence4 *rhs_in)
      : lhs(0), rhs(rhs_in)
    {
    }
    Sentence3::Sentence3(const Sentence3 &original_in)
      : lhs(0), rhs(0)
    {
      if(original_in.lhs)
	{
	  lhs = new Sentence3(*original_in.lhs);
	}
      if(original_in.rhs)
	{
	  rhs = new Sentence4(*original_in.rhs);
	}
    }
    Sentence3::Sentence3(Sentence1 *s1_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence4(s1_in);
    }
    Sentence3::Sentence3(Sentence2 *s2_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence4(s2_in);
    }
    Sentence3::~Sentence3()
    {
      if(lhs)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    Sentence3 *Sentence3::GetLHS()
    {
      return lhs;
    }
    Sentence4 *Sentence3::GetRHS()
    {
      return rhs;
    }
    
    void Sentence3::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(lhs)
	{
	  lhs->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }

    void Sentence3::EliminateBiconditional()
    {
      if(lhs)
	{
	  lhs->EliminateBiconditional();
	}
      if(rhs)
	{
	  rhs->EliminateBiconditional();
	}
    }

    void Sentence3::EliminateImplication()
    {
      if(lhs)
	{
	  lhs->EliminateImplication();
	}
      if(rhs)
	{
	  rhs->EliminateImplication();
	}
    }

    void Sentence3::MoveNotIn()
    {
      if(lhs)
	{
	  lhs->MoveNotIn();
	}
      if(rhs)
	{
	  rhs->MoveNotIn();
	}
    }
    
    void Sentence3::EliminateExtraParens()
    {
      if(lhs)
	{
	  lhs->EliminateExtraParens();
	}
      if((rhs) && !(lhs) && /* Sentence4 */
	 (rhs->rhs) && !(rhs->lhs) && /* Sentence5 */
	 (rhs->rhs->rhs) && /* Sentence0 */
	 !(rhs->rhs->variable_list) && /* VariableList */
	 (rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->lhs) && /* Sentence1 */
	 (rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->lhs) && /* Sentence2 */
	 (rhs->rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->rhs->lhs) /* Sentence3 */
	 )
	{ /* our rhs is just a loop */
	  Sentence3 *child = rhs->rhs->rhs->rhs->rhs->rhs;
	  Sentence3 *new_lhs = 0;
	  Sentence4 *new_rhs = 0;
	  if(child->lhs)
	    {
	      new_lhs = new Sentence3(*(child->lhs));
	    }
	  if(child->rhs)
	    {
	      new_rhs = new Sentence4(*(child->rhs));
	    }
	  if (lhs) { delete lhs; }
	  delete rhs;
	  lhs = new_lhs;
	  rhs = new_rhs;
	  EliminateExtraParens(); // recursive call now that we have changed.
	}
      if(rhs)
	{
	  rhs->EliminateExtraParens();
	}
    }

    SentenceType Sentence3::GetSentenceType() const
    {
      if(lhs)
	{
	  return T_AND;
	}
      else if(rhs)
	{
	  return rhs->GetSentenceType();
	}
      return T_ERROR;
    }

    Sentence4 * Sentence3::GetNextNot()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextNot();
	}
      return 0;
    }
   
    Sentence3 * Sentence3::GetNextAnd()
    {
      if(lhs)
	{
	  return this;
	}
      else if(rhs)
	{
	  return rhs->GetNextAnd();
	}
      return 0;
    }
    
    Sentence2 * Sentence3::GetNextOr()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextOr();
	}
      return 0;
    }
    
    std::string Sentence3::ToString(bool verbose) const
    {
      std::string str = "";
      if(lhs && rhs)
	{
	  str = lhs->ToString(verbose) + " & " + rhs->ToString(verbose);
	}
      else if(rhs)
	{
	  str = rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Sentence3";
	}
      return str;
    }


    /* Sentence 4 */
    Sentence4::Sentence4(Sentence4 *lhs_in)
      : lhs(lhs_in), rhs(0)
    {
    }
    Sentence4::Sentence4(Sentence5 *rhs_in)
      : lhs(0), rhs(rhs_in)
    {
    }
    Sentence4::Sentence4(const Sentence4 &original_in)
      : lhs(0), rhs(0)
    {
      if(original_in.lhs)
	{
	  lhs = new Sentence4(*original_in.lhs);
	}
      if(original_in.rhs)
	{
	  rhs = new Sentence5(*original_in.rhs);
	}
    }
    Sentence4::Sentence4(Sentence1 *s1_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence5(s1_in);
    }
    Sentence4::Sentence4(Sentence2 *s2_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence5(s2_in);
    }
    Sentence4::Sentence4(Sentence3 *s3_in)
      : lhs(0), rhs(0)
    {
      rhs = new Sentence5(s3_in);
    }
    Sentence4::~Sentence4()
    {
      if(lhs)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    Sentence4 *Sentence4::GetLHS()
    {
      return lhs;
    }
    Sentence5 *Sentence4::GetRHS()
    {
      return rhs;
    }
    
    void Sentence4::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(lhs)
	{
	  lhs->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }
    
    void Sentence4::EliminateBiconditional()
    {
      if(lhs)
	{
	  lhs->EliminateBiconditional();
	}
      if(rhs)
	{
	  rhs->EliminateBiconditional();
	}
    }
    
    void Sentence4::EliminateImplication()
    {
      if(lhs)
	{
	  lhs->EliminateImplication();
	}
      if(rhs)
	{
	  rhs->EliminateImplication();
	}
    }

    void Sentence4::MoveNotIn()
    {
      if(lhs)
	{ /* This is a not, figure out what to do.  We should not have any implications at this point.
	   * !A stays
	   * !(!A) goes to A
	   * !(A & B) goes to (!A | !B)
	   * !(A | B) goes to (!A & !B)
	   * !(forall x A) goes to exists x (!A)
	   * !(exists x A) goes to forall x (!A)
	   */
	  SentenceType type = lhs->GetSentenceType();
	  switch(type)
	    {
	    case T_ATOMIC: // do nothing, this is where we want the NOTs
	      break;
	    case T_NOT:
	      MoveNotIn_NotNot();
	      break;
	    case T_AND:
	      MoveNotIn_NotAnd();
	      break;
	    case T_OR:
	      MoveNotIn_NotOr();
	      break;
	    case T_FORALL:
	      MoveNotIn_NotForall();
	      break;
	    case T_EXISTS:
	      MoveNotIn_NotExists();
	      break;
	    default:
	      std::cerr << "UNEXPECTED SENTENCE TYPE IN Sentence4::MoveNotIn(): " << type << std::endl;
	      break;
	    }
	}
      
      /* This handles the recursive push-down as well as un-NOTTED sentences */
      if(rhs)
	{
	  rhs->MoveNotIn();
	}
    }
    
    void Sentence4::EliminateExtraParens()
    {
      if(lhs)
	{
	  lhs->EliminateExtraParens();
	}
      if((rhs) && !(lhs) && /* Sentence5 */
	 (rhs->rhs) && /* Sentence0 */
	 !(rhs->variable_list) && /* VariableList */
	 (rhs->rhs->rhs) && !(rhs->rhs->lhs) && /* Sentence1 */
	 (rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->lhs) && /* Sentence2 */
	 (rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->lhs) && /* Sentence3 */
	 (rhs->rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->rhs->lhs) /* Sentence4 */
	 )
	{ /* our rhs is just a loop */
	  Sentence4 *child = rhs->rhs->rhs->rhs->rhs->rhs;
	  Sentence4 *new_lhs = 0;
	  Sentence5 *new_rhs = 0;
	  if(child->lhs)
	    {
	      new_lhs = new Sentence4(*(child->lhs));
	    }
	  if(child->rhs)
	    {
	      new_rhs = new Sentence5(*(child->rhs));
	    }
	  if (lhs) { delete lhs; }
	  delete rhs;
	  lhs = new_lhs;
	  rhs = new_rhs;
	  EliminateExtraParens(); // recursive call now that we have changed.
	}
      if(rhs)
	{
	  rhs->EliminateExtraParens();
	}
    }

    SentenceType Sentence4::GetSentenceType() const
    {
      if(lhs)
	{
	  return T_NOT;
	}
      else if(rhs)
	{
	  return rhs->GetSentenceType();
	}
      return T_ERROR;
    }

    Sentence4 * Sentence4::GetNextNot()
    {
      if(lhs)
	{
	  return this;
	}
      else if(rhs)
	{
	  return rhs->GetNextNot();
	}
      return 0;
    }

    Sentence3 * Sentence4::GetNextAnd()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextAnd();
	}
      return 0;
    }
    
    Sentence2 * Sentence4::GetNextOr()
    {
      if(lhs)
	{
	  return 0;
	}
      else if(rhs)
	{
	  return rhs->GetNextOr();
	}
      return 0;
    }
    
    std::string Sentence4::ToString(bool verbose) const
    {
      std::string str = "";
      if(lhs)
	{
	  str = "!" + lhs->ToString(verbose);
	}
      else if(rhs)
	{
	  str = rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Sentence4";
	}
      return str;
    }

    void Sentence4::MoveNotIn_NotNot()
    { /* !(!A) goes to A */
      Sentence4 *next_not = lhs->GetNextNot();
      if(next_not->lhs->lhs)
	{ /* copy "A", which is negated ... */
	  Sentence4 *new_lhs = new Sentence4(*(next_not->lhs->lhs));
	  delete lhs;
	  lhs = new_lhs;
	  MoveNotIn(); // need to recursively call MoveNotIn(), since we changed the lhs
	}
      else
	{
	  /* copy "A" */
	  rhs = new Sentence5(*(next_not->lhs->rhs));
	  delete lhs;
	  lhs = 0;
	}
    }
    void Sentence4::MoveNotIn_NotAnd()
    { /* !(A & B) goes to (!A | !B) */
      Sentence3 * next_and = lhs->GetNextAnd();
      Sentence3 * A = new Sentence3(*(next_and->lhs));
      Sentence4 * B = new Sentence4(*(next_and->rhs));
      Sentence4 * not_A = new Sentence4(new Sentence4(A));
      Sentence4 * not_B = new Sentence4(B);
      
      Sentence2 * not_A2 = new Sentence2(not_A);
      Sentence3 * not_B2 = new Sentence3(not_B);

      Sentence2 * new_or = new Sentence2(not_A2, not_B2);

      Sentence5 * new_rhs = new Sentence5(new_or);
      
      delete lhs;
      lhs = 0;
      rhs = new_rhs;
      /* No need for recursive call here */
    }
    void Sentence4::MoveNotIn_NotOr()
    { /* !(A | B) goes to (!A & !B) */
      Sentence2 * next_or = lhs->GetNextOr();
      Sentence2 * A = new Sentence2(*(next_or->lhs));
      Sentence3 * B = new Sentence3(*(next_or->rhs));
      Sentence4 * not_A = new Sentence4(new Sentence4(A));
      Sentence4 * not_B = new Sentence4(B);
      
      Sentence3 * not_A2 = new Sentence3(not_A);

      Sentence3 * new_and = new Sentence3(not_A2, not_B);

      Sentence5 * new_rhs = new Sentence5(new_and);
      
      delete lhs;
      lhs = 0;
      rhs = new_rhs;
      /* No need for recursive call here */
    }
    void Sentence4::MoveNotIn_NotForall()
    { /* !(forall x A) goes to exists x (!A) */
      std::cerr << "FIXME !(forall x A) " << std::endl;
    }
    void Sentence4::MoveNotIn_NotExists()
    { /* !(exists x A) goes to forall x (!A) */
      std::cerr << "FIXME !(exists x A) " << std::endl;
    }

    

    
    /* Sentence 5 */
    Sentence5::Sentence5(AtomicSentence *atomic_sentence_in)
      : atomic_sentence(atomic_sentence_in), variable_list(0), rhs(0), type(T_ATOMIC)
    {
    }
    
    Sentence5::Sentence5(Sentence0 *sentence_in)
      : atomic_sentence(0), variable_list(0), rhs(sentence_in), type(T_LEFT_PAREN)
    {
    }
    
    Sentence5::Sentence5(SentenceType type_in, VariableList *variable_list_in, Sentence0 *sentence_in)
      : atomic_sentence(0), variable_list(variable_list_in), rhs(sentence_in), type(type_in)
    {
    }
    Sentence5::Sentence5(const Sentence5 &original_in)
      : atomic_sentence(0), variable_list(0), rhs(0), type(original_in.type)
    {
      if(original_in.atomic_sentence)
	{
	  atomic_sentence = new AtomicSentence(*original_in.atomic_sentence);
	}
      if(original_in.variable_list)
	{
	  variable_list = new VariableList(*original_in.variable_list);
	}
      if(original_in.rhs)
	{
	  rhs = new Sentence0(*original_in.rhs);
	}
    }
    Sentence5::Sentence5(Sentence1 *s1_in)
      : atomic_sentence(0), variable_list(0), rhs(0), type(T_LEFT_PAREN)
    {
      rhs = new Sentence0(s1_in);
    }
    Sentence5::Sentence5(Sentence2 *s2_in)
      : atomic_sentence(0), variable_list(0), rhs(0), type(T_LEFT_PAREN)
    {
      rhs = new Sentence0(s2_in);
    }
    Sentence5::Sentence5(Sentence3 *s3_in)
      : atomic_sentence(0), variable_list(0), rhs(0), type(T_LEFT_PAREN)
    {
      rhs = new Sentence0(s3_in);
    }

    Sentence5::~Sentence5()
    {
      if(atomic_sentence)
	{
	  delete atomic_sentence;
	  atomic_sentence = 0;
	}
      if(variable_list)
	{
	  delete variable_list;
	  variable_list = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    AtomicSentence *Sentence5::GetAtomicSentence()
    {
      return atomic_sentence;
    }
    
    VariableList   *Sentence5::GetVariableList()
    {
      return variable_list;
    }
    
    Sentence0      *Sentence5::GetRHS()
    {
      return rhs;
    }
    
    void Sentence5::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(atomic_sentence)
	{
	  atomic_sentence->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }
    
    void Sentence5::EliminateBiconditional()
    {
      if(rhs)
	{
	  rhs->EliminateBiconditional();
	}
    }
    
    void Sentence5::EliminateImplication()
    {
      if(rhs)
	{
	  rhs->EliminateImplication();
	}
    }

    void Sentence5::MoveNotIn()
    {
      if(rhs)
	{
	  rhs->MoveNotIn();
	}
    }
    
    void Sentence5::EliminateExtraParens()
    {
      if((rhs) && /* Sentence0 */
	 !(variable_list) && /* VariableList */
	 (rhs->rhs) && !(rhs->lhs) && /* Sentence1 */
	 (rhs->rhs->rhs) && !(rhs->rhs->lhs) && /* Sentence2 */
	 (rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->lhs) && /* Sentence3 */
	 (rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->lhs) && /* Sentence4 */
	 (rhs->rhs->rhs->rhs->rhs->rhs) && !(rhs->rhs->rhs->rhs->rhs->lhs) /* Sentence5 */
	 )
	{ /* our rhs is just a loop */
	  Sentence5 *child = rhs->rhs->rhs->rhs->rhs->rhs;
	  AtomicSentence *new_atomic_sentence = 0;
	  VariableList *new_variable_list = 0;
	  Sentence0 *new_rhs = 0;
	  SentenceType new_sentence_type = child->type;
	  if(child->atomic_sentence)
	    {
	      new_atomic_sentence = new AtomicSentence(*(child->atomic_sentence));
	    }
	  if(child->variable_list)
	    {
	      new_variable_list = new VariableList(*(child->variable_list));
	    }
	  if(child->rhs)
	    {
	      new_rhs = new Sentence0(*(child->rhs));
	    }
	  if (atomic_sentence) { delete atomic_sentence; }
	  if (variable_list) { delete variable_list; }
	  delete rhs;
	  atomic_sentence = new_atomic_sentence;
	  variable_list = new_variable_list;
	  rhs = new_rhs;
	  type = new_sentence_type;

	  EliminateExtraParens(); // recursive call now that we have changed.
	}
      if(rhs)
	{
	  rhs->EliminateExtraParens();
	}
    }
    
    SentenceType Sentence5::GetSentenceType() const
    {
      if(atomic_sentence)
	{
	  return T_ATOMIC;
	}
      else if(!variable_list && rhs)
	{
	  return rhs->GetSentenceType();
	}
      else if(variable_list && rhs && type == T_FORALL)
	{
	  return T_FORALL;
	}
      else if(variable_list && rhs && type == T_EXISTS)
	{
	  return T_EXISTS;
	}
      return T_ERROR;
    }
    
    Sentence4 * Sentence5::GetNextNot()
    {
      if(atomic_sentence)
	{
	  return 0;
	}
      else if(!variable_list && rhs)
	{
	  return rhs->GetNextNot();
	}
      else if(variable_list && rhs && type == T_FORALL)
	{
	  return 0;
	}
      else if(variable_list && rhs && type == T_EXISTS)
	{
	  return 0;
	}
      return 0;
    }

    Sentence3 * Sentence5::GetNextAnd()
    {
      if(atomic_sentence)
	{
	  return 0;
	}
      else if(!variable_list && rhs)
	{
	  return rhs->GetNextAnd();
	}
      else if(variable_list && rhs && type == T_FORALL)
	{
	  return 0;
	}
      else if(variable_list && rhs && type == T_EXISTS)
	{
	  return 0;
	}
      return 0;
    }
    
    Sentence2 * Sentence5::GetNextOr()
    {
      if(atomic_sentence)
	{
	  return 0;
	}
      else if(!variable_list && rhs)
	{
	  return rhs->GetNextOr();
	}
      else if(variable_list && rhs && type == T_FORALL)
	{
	  return 0;
	}
      else if(variable_list && rhs && type == T_EXISTS)
	{
	  return 0;
	}
      return 0;
    }

    std::string Sentence5::ToString(bool verbose) const
    {
      std::string str = "";
      if(atomic_sentence)
	{
	  str = atomic_sentence->ToString(verbose);
	}
      else if(!variable_list && rhs)
	{
	  str = "(" + rhs->ToString(verbose) + ")";
	}
      else if(variable_list && rhs && type == T_FORALL)
	{
	  str = "forall " + variable_list->ToString(verbose) + " " + rhs->ToString(verbose);
	}
      else if(variable_list && rhs && type == T_EXISTS)
	{
	  str = "exists " + variable_list->ToString(verbose) + " " + rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Sentence5";
	}
      return str;
    }


    /* AtomicSentence */
    AtomicSentence::AtomicSentence(Identifier *predicate_in, TermList *term_list_in)
      : predicate(predicate_in), term_list(term_list_in), lhs(0), rhs(0)
    {
    }

    AtomicSentence::AtomicSentence(Term *lhs_in, Term *rhs_in)
      : predicate(0), term_list(0), lhs(lhs_in), rhs(rhs_in)
    {
    }
    
    AtomicSentence::AtomicSentence(const AtomicSentence &original_in)
      : predicate(0), term_list(0), lhs(0), rhs(0)
    {
      if(original_in.predicate)
	{
	  predicate = new Identifier(*original_in.predicate);
	}
      if(original_in.term_list)
	{
	  term_list = new TermList(*original_in.term_list);
	}
      if(original_in.lhs)
	{
	  lhs = new Term(*original_in.lhs);
	}
      if(original_in.rhs)
	{
	  rhs = new Term(*original_in.rhs);
	}
    }

    AtomicSentence::~AtomicSentence()
    {
      if(predicate)
	{
	  delete predicate;
	  predicate = 0;
	}
      if(term_list)
	{
	  delete term_list;
	  term_list = 0;
	}
      if(lhs)
	{
	  delete lhs;
	  lhs = 0;
	}
      if(rhs)
	{
	  delete rhs;
	  rhs = 0;
	}
    }
    
    Identifier *AtomicSentence::GetPredicate()
    {
      return predicate;
    }
    TermList  *AtomicSentence::GetTermList()
    {
      return term_list;
    }
    Term      *AtomicSentence::GetLHS()
    {
      return lhs;
    }
    Term      *AtomicSentence::GetRHS()
    {
      return rhs;
    }
    
    void AtomicSentence::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(term_list)
	{
	  term_list->GetAllConstants(constants_out);
	}
      if(lhs)
	{
	  lhs->GetAllConstants(constants_out);
	}
      if(rhs)
	{
	  rhs->GetAllConstants(constants_out);
	}
    }

    std::string AtomicSentence::ToString(bool verbose) const
    {
      std::string str = "";
      if(predicate && term_list)
	{
	  str = predicate->ToString(verbose) + "(" + term_list->ToString(verbose) + ")";
	}
      else if(lhs && rhs)
	{
	  str = lhs->ToString(verbose) + " = " + rhs->ToString(verbose);
	}
      else
	{
	  str = "ERROR in AtomicSentence";
	}
      return str;
    }

    /* VariableList */
    VariableList::VariableList(VariableList *variable_list_in, Identifier *variable_in)
      : variable_list(variable_list_in), variable(variable_in)
    {
    }
    VariableList::VariableList(Identifier *variable_in)
      : variable_list(0), variable(variable_in)
    {
    }
    VariableList::VariableList(const VariableList &original_in)
      : variable_list(0), variable(0)
    {
      if(original_in.variable_list)
	{
	  variable_list = new VariableList(*original_in.variable_list);
	}
      if(original_in.variable)
	{
	  variable = new Identifier(*original_in.variable);
	}
    }

    VariableList::~VariableList()
    {
      if(variable_list)
	{
	  delete variable_list;
	  variable_list = 0;
	}
      if(variable)
	{
	  delete variable;
	  variable = 0;
	}
    }

    VariableList *VariableList::GetVariableList()
    {
      return variable_list;
    }
    Identifier *VariableList::GetVariable()
    {
      return variable;
    }
    
    void VariableList::GetAllVariables(std::list<Identifier *> &variables_out) const
    {
      if(variable)
	{
	  variables_out.push_back(variable);
	}
      if(variable_list)
	{
	  GetAllVariables(variables_out);
	}
    }
    
    std::string VariableList::ToString(bool verbose) const
    {
      std::string str = "";
      if(variable_list && variable)
	{
	  str = variable_list->ToString(verbose) + ", " + variable->ToString(verbose);
	}
      else if(variable)
	{
	  str = variable->ToString(verbose);
	}
      else
	{
	  str = "ERROR in VariableList";
	}
      return str;
    }



    /* TermList */
    TermList::TermList(TermList *term_list_in, Term *term_in)
      : term_list(term_list_in), term(term_in)
    {
    }
    TermList::TermList(Term *term_in)
      : term_list(0), term(term_in)
    {
    }
    TermList::TermList(const TermList &original_in)
      : term_list(0), term(0)
    {
      if(original_in.term_list)
	{
	  term_list = new TermList(*original_in.term_list);
	}
      if(original_in.term)
	{
	  term = new Term(*original_in.term);
	}
    }

    TermList::~TermList()
    {
      if(term_list)
	{
	  delete term_list;
	  term_list = 0;
	}
      if(term)
	{
	  delete term;
	  term = 0;
	}
    }

    TermList *TermList::GetTermList()
    {
      return term_list;
    }
    Term *TermList::GetTerm()
    {
      return term;
    }

    int TermList::CountArity() const
    {
      int count = 0;
      if(term)
	{
	  count ++;
	}
      if(term_list)
	{
	  count += term_list->CountArity();
	}
      return count;
    }
    
    void TermList::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(term_list)
	{
	  term_list->GetAllConstants(constants_out);
	}
      if(term)
	{
	  term->GetAllConstants(constants_out);
	}
    }

    std::string TermList::ToString(bool verbose) const
    {
      std::string str = "";
      if(term_list && term)
	{
	  str = term_list->ToString(verbose) + ", " + term->ToString(verbose);
	}
      else if(term)
	{
	  str = term->ToString(verbose);
	}
      else
	{
	  str = "ERROR in TermList";
	}
      return str;
    }


    /* Term */
    Term::Term(Identifier *function_in, TermList *term_list_in)
      : identifier(function_in), term_list(term_list_in)
    {
    }
    Term::Term(Identifier *identifier_in)
      : identifier(identifier_in), term_list(0)
    {
    }
    Term::Term(const Term &original_in)
      : identifier(0), term_list(0)
    {
      if(original_in.identifier)
	{
	  identifier = new Identifier(*original_in.identifier);
	}
      if(original_in.term_list)
	{
	  term_list = new TermList(*original_in.term_list);
	}
    }
    
    Term::~Term()
    {
      if(identifier)
	{
	  delete identifier;
	  identifier = 0;
	}
      if(term_list)
	{
	  delete term_list;
	  term_list = 0;
	}
    }

    void Term::ClearFunction()
    { /* do not delete, these pointers are to be reused else-where */
      identifier = 0;
      term_list = 0;
    }
    
    Identifier *Term::GetIdentifier()
    {
      return identifier;
    }
    TermList *Term::GetTermList()
    {
      return term_list;
    }
    
    void Term::GetAllConstants(std::list<Identifier *> &constants_out) const
    {
      if(term_list)
	{
	  term_list->GetAllConstants(constants_out);
	}
      if(identifier && identifier->GetSymbol()->GetType() == Symbol::TYPE_CONSTANT)
	{
	  constants_out.push_back(identifier);
	}
    }

    std::string Term::ToString(bool verbose) const
    {
      std::string str = "";
      if(identifier && term_list)
	{
	  str = identifier->ToString(verbose) + "(" + term_list->ToString(verbose) + ")";
	}
      else if(identifier)
	{
	  str = identifier->ToString(verbose);
	}
      else
	{
	  str = "ERROR in Term";
	}
      return str;
    }



    /* Identifier */
    Identifier::Identifier(Symbol *symbol_in)
      : symbol(symbol_in)
    {
    }
    Identifier::Identifier(const Identifier &original_in)
      : symbol(original_in.symbol)
    {
    }
    
    Identifier::~Identifier()
    { // do not delete symbol, that is handled elsewhere
      symbol = 0;
    }

    Symbol *Identifier::GetSymbol()
    {
      return symbol;
    }
    std::string Identifier::ToString(bool verbose) const
    {
      std::string str = "";
      str += symbol->GetName();
      if(verbose)
	{
	  if(symbol->GetType() == Symbol::TYPE_CONSTANT)
	    {
	      str += "/c";
	    }
	  else if(symbol->GetType() == Symbol::TYPE_VARIABLE)
	    {
	      str += "/v";
	    }
	  else if(symbol->GetType() == Symbol::TYPE_FUNCTION)
	    {
	      str += "/f";
	      
	      char buf[16];
	      std::sprintf(buf, "%d", symbol->GetArity());
	      str += "/";
	      str += buf;
	    }
	  else if(symbol->GetType() == Symbol::TYPE_PREDICATE)
	    {
	      str += "/p";
	      
	      char buf[16];
	      std::sprintf(buf, "%d", symbol->GetArity());
	      str += "/";
	      str += buf;
	    }
	  else
	    {
	      str += "/ERROR";
	    }
	}
      return str;
    }


    /* Sentence */
    Sentence::Sentence(Sentence0 *sentence0_in)
      : sentence0(sentence0_in)
    {
    }

    Sentence::Sentence(const Sentence &sentence0_in)
      : sentence0(0)
    {
      *this = sentence0_in;
    }

    Sentence &Sentence::operator=(const Sentence &rhs_in)
    {
      Delete();

      sentence0 = new Sentence0(*rhs_in.sentence0);

      return *this;
    }
    
    Sentence::~Sentence()
    {
      Delete();
    }

    void Sentence::Delete()
    {
      if(sentence0 != 0)
	{
	  delete sentence0;
	  sentence0 = 0;
	}
    }

    void Sentence::GetCNF(std::vector<Sentence *> &clauses) const
    {
      Sentence *s1 = new Sentence(*this);
      s1->EliminateBiconditional();
      s1->EliminateImplication();
      s1->MoveNotIn();
      s1->EliminateExtraParens();
      clauses.push_back(s1);
    }
    void Sentence::EliminateBiconditional()
    {
      if(sentence0)
	{
	  sentence0->EliminateBiconditional();
	}
    }
    void Sentence::EliminateImplication()
    {
      if(sentence0)
	{
	  sentence0->EliminateImplication();
	}
    }
    void Sentence::MoveNotIn()
    {
      if(sentence0)
	{
	  sentence0->MoveNotIn();
	}
    }
    
    void Sentence::EliminateExtraParens()
    {
      if(sentence0)
	{
	  sentence0->EliminateExtraParens();
	}
    }
  
    Sentence0 *Sentence::GetSentence()
    {
      return sentence0;
    }

    std::string Sentence::ToString() const
    {
      return ToString(true);
    }
    
    std::string Sentence::ToString(bool verbose) const
    {
      return sentence0->ToString(verbose);
    }

    std::ostream & operator<<(std::ostream &os, const Sentence &s)
    {
      os << s.ToString();
      return os;
    }

    // Display a list of Sentences
    std::ostream &operator<<(std::ostream &os, const std::list<Sentence *> &sentences)
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

  }
}
