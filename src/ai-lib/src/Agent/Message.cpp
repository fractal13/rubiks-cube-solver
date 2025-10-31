#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    const std::string MESSAGE_IDENT_NAME   = "IDENT";
    const std::string MESSAGE_AGENT_NAME   = "AGENT";
    const std::string MESSAGE_DISPLAY_NAME = "DISPLAY";
    const std::string MESSAGE_DIE_NAME     = "DIE";
    const std::string MESSAGE_NAME_NAME    = "NAME";
    const std::string MESSAGE_PERCEPT_NAME = "PERCEPT";
    const std::string MESSAGE_UPDATE_NAME  = "UPDATE";
    const std::string MESSAGE_ACTION_NAME  = "ACTION";


    static const char * tsep = "::";
    static const char * vsep = "==";
    Message::Message()
      : mName(""), mMessageValid(false),
        mTokenSeparator(tsep), mValueSeparator(vsep)
    {
    }

    Message::Message(const std::string &name)
      : mName(name), mMessageValid(false),
        mTokenSeparator(tsep), mValueSeparator(vsep)
    {
    }

    Message::~Message()
    {
      Clear();
    }

    bool Message::SetName(const std::string &name)
    {
      if(name.find(mTokenSeparator) != std::string::npos)
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Bad data in name: " << name << std::endl);
          return false;
        }
      if(mName == "")
        {
          mMessageValid = false;
          mName = name;
          return true;
        }
      else
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Name already set: " << mName << "  trying to change to: " << name << std::endl);
          return false;
        }
    }
    const std::string &Message::GetName() const
    {
      return mName;
    }

    bool Message::AddKeyValue(const std::string &key, const std::string &value)
    {
      if(key.find(mTokenSeparator) != std::string::npos ||
         key.find(mValueSeparator) != std::string::npos ||
         value.find(mTokenSeparator) != std::string::npos)
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Bad data in key: " << key << "  or value: " << value << std::endl);
          return false;
        }
      std::map<std::string, std::string>::const_iterator it = mValues.find(key);
      if(it == mValues.end())
        {
          mMessageValid = false;
          mValues[key] = value;
          return true;
        }
      DBGP_E(LOG_STREAM(std::cerr) << "Key: " << key << " already set." << std::endl);
      return false;
    }

    bool Message::UpdateKeyValue(const std::string &key, const std::string &value)
    {
      if(key.find(mTokenSeparator) != std::string::npos ||
         key.find(mValueSeparator) != std::string::npos ||
         value.find(mTokenSeparator) != std::string::npos)
        {
          return false;
        }
      mMessageValid = false;
      mValues[key] = value;
      return true;
    }

    const std::string g_empty_string = "";
    const std::string &Message::GetValue(const std::string &key) const
    {
      std::map<std::string, std::string>::const_iterator it = mValues.find(key);
      if(it != mValues.end())
        {
          return it->second;
        }
      return g_empty_string;
    }

    const std::map<std::string, std::string> &Message::GetKeyValues() const
    {
      return mValues;
    }

    bool Message::SetFromString(const std::string &msg)
    {
      Clear();

      size_t pos = 0;
      std::string token;
      std::string key, value;
      if(!GetToken(msg, pos, mName, mTokenSeparator))
        {
          DBGP_E(LOG_STREAM(std::cerr)
                 << "!GetToken(msg="<<msg
                 <<", pos="<<pos
                 <<", mName="<<mName
                 <<", mTokenSeparator="<<mTokenSeparator
                 <<")" << std::endl);
          return false;
        }
      while(pos < msg.size())
        {
          if(GetToken(msg, pos, token, mTokenSeparator))
            {
              size_t tpos = 0;
              if(!(GetToken(token, tpos, key, mValueSeparator) &&
                   GetToken(token, tpos, value, mValueSeparator) &&
                   AddKeyValue(key, value)))
                {
                  DBGP_E(LOG_STREAM(std::cerr)
                         << "if(!(GetToken(token, tpos, key, mValueSeparator) && "
                         << "GetToken(token, tpos, value, mValueSeparator) && "
                         << "AddKeyValue(key, value)))" << std::endl);
                  return false;
                }
            }
          else
            {
              DBGP_E(LOG_STREAM(std::cerr)
                     << "GetToken(msg, pos, token, mTokenSeparator)"
                     << std::endl);
              return false;
            }
        }
      return true;
    }

    const std::string &Message::ToString()
    {
      if(!mMessageValid)
        {
          FormatMessage();
        }
      if(!mMessageValid)
        {
          mMessage = "";
        }
      return mMessage;
    }

    bool Message::RemoveKey(const std::string &key)
    {
      std::map<std::string, std::string>::const_iterator it = mValues.find(key);
      if(it == mValues.end())
        {
          return true;
        }
      mMessageValid = false;
      mValues.erase(it);
      it = mValues.find(key);
      if(it == mValues.end())
        {
          return true;
        }
      return false;
    }

    bool Message::Clear()
    {
      mName = "";
      mValues.clear();
      mMessageValid = false;
      mMessage.clear();
      return true;
    }

    bool Message::GetToken(const std::string &msg, size_t &pos, std::string &token, const std::string &delim)
    {
      // no more string to process
      if(pos == std::string::npos || pos >= msg.size())
        {
          DBGP_E(LOG_STREAM(std::cerr) << "pos = " << pos << " is out of range." << std::endl);
          DBGP_E(LOG_STREAM(std::cerr) << "npos = " << std::string::npos << std::endl);
          DBGP_E(LOG_STREAM(std::cerr) << "msg.size = " << msg.size() << std::endl);
          DBGP_E(LOG_STREAM(std::cerr) << "msg = " << msg << "  delim = " << delim << std::endl);
          return false;
        }

      size_t delim_pos = msg.find(delim, pos);

      if(delim_pos == std::string::npos)
        {
          // reached end of string
          token = msg.substr(pos);
          pos = delim_pos;
        }
      else
        {
          // found next deliminator
          token = msg.substr(pos, delim_pos-pos);
          pos = delim_pos + delim.size();
        }

      return true;
    }

    bool Message::FormatMessage()
    {
      mMessage.clear();
      mMessageValid = false;
      if(mName == "")
        {
          return false;
        }

      mMessage.clear();

      mMessage += mName;

      std::map<std::string, std::string>::const_iterator it;
      for(it = mValues.begin(); it != mValues.end(); it++)
        {
          DBGP_MSG(LOG_STREAM(std::cerr) << "key: " << it->first << " value: " << it->second << std::endl);
          mMessage += mTokenSeparator;
          mMessage += it->first;
          mMessage += mValueSeparator;
          mMessage += it->second;
        }
      DBGP_MSG(LOG_STREAM(std::cerr) << "mMessage: " << mMessage << std::endl);
      mMessageValid = true;
      return true;
    }

    std::ostream &operator<<(std::ostream &os, Message &msg)
    {
      os << msg.ToString();
      return os;
    }

    IdentMessage::IdentMessage()
      : Message(MESSAGE_IDENT_NAME)
    {
    }

    AgentMessage::AgentMessage()
      : Message(MESSAGE_AGENT_NAME)
    {
    }

    DisplayMessage::DisplayMessage()
      : Message(MESSAGE_DISPLAY_NAME)
    {
    }

    DieMessage::DieMessage()
      : Message(MESSAGE_DIE_NAME)
    {
    }

    AgentNameMessage::AgentNameMessage()
      : Message(MESSAGE_NAME_NAME)
    {
    }

    UpdateMessage::UpdateMessage()
      : Message(MESSAGE_UPDATE_NAME)
    {
    }

  }
}
