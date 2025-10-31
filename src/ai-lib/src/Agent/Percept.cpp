#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {

    Percept::Percept()
    {
    }
    Percept::~Percept()
    {
    }
    bool Percept::TextDisplay(std::ostream &os) const
    {
      os << "[ ";
      std::map<std::string, PerceptAtom>::const_iterator it;
      for(it = atoms.begin(); it != atoms.end(); it++)
        {
          os << it->second.GetName()
             << ": "
             << it->second.GetValue()
             << ", ";
        }
      os << " ]";
      return true;
    }
    void Percept::Join(Percept *percept_in)
    {
      std::map<std::string, PerceptAtom>::const_iterator it;
      for(it = percept_in->atoms.begin(); it != percept_in->atoms.end(); it++)
        {
          JoinAtom(it->second);
        }
    }
    void Percept::JoinAtom(const PerceptAtom &atom_in)
    {
      std::map<std::string, PerceptAtom>::iterator it;
      it = atoms.find(atom_in.GetName());
      if(it != atoms.end())
        {
          it->second.SetValue(atom_in.GetValue());
        }
      else
        {
          atoms[atom_in.GetName()] = atom_in;
          atom_keys.push_back(atom_in.GetName());
        }
    }
    unsigned int Percept::NumAtom() const
    {
      return (unsigned int)atoms.size();
    }
    PerceptAtom Percept::GetAtom(const int i) const
    {
      std::map<std::string, PerceptAtom>::const_iterator it;
      if(i >= 0 && i < (int)atom_keys.size())
        {
          it = atoms.find(atom_keys[i]);
          if(it != atoms.end())
            {
              return it->second;
            }
        }
      PerceptAtom empty;
      return empty;
    }
    PerceptAtom Percept::GetAtom(const std::string &name) const
    {
      std::map<std::string, PerceptAtom>::const_iterator it;
      it = atoms.find(name);
      if(it != atoms.end())
        {
          return it->second;
        }
      else
        {
          PerceptAtom empty;
          return empty;
        }
    }
    bool Percept::ToMessage(Message &omsg) const
    {
      if(!omsg.Clear())
        {
          return false;
        }
      if(!omsg.SetName(MESSAGE_PERCEPT_NAME))
        {
          return false;
        }
      std::map<std::string, PerceptAtom>::const_iterator it;
      for(it = atoms.begin(); it != atoms.end(); it++)
        {
          if(!omsg.AddKeyValue(it->second.GetName(), it->second.GetValue()))
            {
              return false;
            }
        }
      return true;
    }
    bool Percept::FromMessage(const Message &imsg)
    {
      if(imsg.GetName() != MESSAGE_PERCEPT_NAME)
        {
          return false;
        }
      const std::map<std::string, std::string> &values = imsg.GetKeyValues();
      std::map<std::string, std::string>::const_iterator it;
      for(it = values.begin(); it != values.end(); it++)
        {
          JoinAtom(PerceptAtom(it->first, it->second));
        }
      return true;
    }

    std::ostream & operator<<(std::ostream &os, const Percept * percept)
    {
      percept->TextDisplay(os);
      return os;
    }

  }
}
