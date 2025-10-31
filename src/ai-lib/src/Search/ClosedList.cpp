#include "ai_search.h"

namespace ai
{
  namespace Search
  {
    ClosedListStatePointer::ClosedListStatePointer(const State * const state_in)
      : state(state_in)
    {
    }
    
    bool ClosedListStatePointer::operator<(const ClosedListStatePointer &rhs) const
    {
      return state->IsLessThan(rhs.state);
    }

    
    ClosedList::ClosedList()
      : use_set(US_YES)
    {
    }

    void ClosedList::Clear()
    {
      closed_list.clear();
      closed_set.clear();
    }

    bool ClosedList::Contains(const State * const state_in) const
    {
      unsigned int i;

      State *s1 = 0;
      const State * const s2 = state_in;

      if(use_set == US_NO)
        {
          for(i = 0; i < closed_list.size(); i++)
            {
              s1 = closed_list[i]->GetState();
              if(s1->IsEqual(s2))
                {
                  return true;
                }
            }
          return false;
        }
      else if(use_set == US_YES)
        {
          ClosedListStatePointer p(state_in);
          return closed_set.find(p) != closed_set.end();
        }
      return false;
    }
    
    bool ClosedList::Contains(const Node * const node_in) const
    {
      return Contains(node_in->GetState());
    }

    bool ClosedList::Insert(Node * node_in)
    {
      SetStyle(node_in);
      if(use_set == US_NO)
        {
          if(Contains(node_in))
            {
              return false;
            }
          closed_list.push_back(node_in);
          return true;
        }
      else if(use_set == US_YES)
        {
          std::pair<std::set<ClosedListStatePointer>::iterator,bool> ret;
          ret = closed_set.insert(ClosedListStatePointer(node_in->GetState()));
          return ret.second;
        }
      return false;
    }

    size_t ClosedList::Size() const
    {
      if(use_set == US_NO)
        {
          return closed_list.size();
        }
      else if(use_set == US_YES)
        {
          return closed_set.size();
        }
      return 0;
    }

    void ClosedList::SetStyle(const Node * const node_in)
    {
      if(use_set == US_UNKNOWN)
        {
          use_set = US_YES;
        }
    }
  }
}

