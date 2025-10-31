#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iostream>
#include <string>
#include <map>
#include <set>


namespace ai
{
  namespace Agent
  {
    extern const std::string MESSAGE_IDENT_NAME;
    extern const std::string MESSAGE_AGENT_NAME;
    extern const std::string MESSAGE_DISPLAY_NAME;
    extern const std::string MESSAGE_DIE_NAME;
    extern const std::string MESSAGE_NAME_NAME;
    extern const std::string MESSAGE_PERCEPT_NAME;
    extern const std::string MESSAGE_UPDATE_NAME;
    extern const std::string MESSAGE_ACTION_NAME;
    /**
     * Represents a message to be sent or received
     * via communication channels
     */
    class Message
    {
    public:
      Message();
      Message(const std::string &name);
      virtual ~Message();

      bool SetName(const std::string &name);
      const std::string &GetName() const;

      bool AddKeyValue(const std::string &key, const std::string &value);
      bool UpdateKeyValue(const std::string &key, const std::string &value);
      const std::string &GetValue(const std::string &key) const;
      const std::map<std::string, std::string> &GetKeyValues() const;

      bool SetFromString(const std::string &msg);
      const std::string &ToString();

      bool RemoveKey(const std::string &key);
      bool Clear();

    protected:
      bool GetToken(const std::string &msg, size_t &pos, std::string &token, const std::string &delim);
      bool FormatMessage();

      // name
      std::string mName;

      // key/value pairs
      std::map<std::string, std::string> mValues;

      // data of the message body
      bool mMessageValid;
      std::string mMessage;

      // separators used in message formatting
      std::string mTokenSeparator;
      std::string mValueSeparator;
    private:
    };

    extern std::ostream &operator<<(std::ostream &os, Message &msg);

    class IdentMessage : public Message
    {
    public:
      IdentMessage();
    };
    class AgentMessage : public Message
    {
    public:
      AgentMessage();
    };
    class DisplayMessage : public Message
    {
    public:
      DisplayMessage();
    };
    class DieMessage : public Message
    {
    public:
      DieMessage();
    };
    class AgentNameMessage : public Message
    {
    public:
      AgentNameMessage();
    };
    class UpdateMessage : public Message
    {
    public:
      UpdateMessage();
    };

  }
}

#define STREAM_TO_STRING(kstr, kstream)         \
  do                                            \
    {                                           \
      std::stringstream l_ks;                   \
      l_ks << kstream;                          \
      kstr = l_ks.str();                        \
    } while (0)

#define ADD_TO_MSG_IF_CHANGED(keystream, valuestream, message, old_message) \
  do                                                                    \
    {                                                                   \
      std::string l_keystr, l_valuestr;                                 \
      STREAM_TO_STRING(l_keystr, keystream);                            \
      STREAM_TO_STRING(l_valuestr, valuestream);                        \
      if((old_message).GetValue(l_keystr) != l_valuestr)                \
        {                                                               \
          if(!(message).AddKeyValue(l_keystr, l_valuestr))              \
            {                                                           \
              DBGP_E(LOG_STREAM(std::cerr) << "Could not AddKeyValue: " \
                     << l_keystr << "==" << l_valuestr << std::endl);   \
            }                                                           \
          else                                                          \
            {                                                           \
              DBGP_MSG(LOG_STREAM(std::cerr) << "AddKeyValue: "         \
                       << l_keystr << "==" << l_valuestr << std::endl); \
            }                                                           \
          if(!(old_message).UpdateKeyValue(l_keystr, l_valuestr))       \
            {                                                           \
              DBGP_E(LOG_STREAM(std::cerr) << "Could not UpdateKeyValue: " \
                     << l_keystr << "==" << l_valuestr << std::endl);   \
            }                                                           \
          else                                                          \
            {                                                           \
              DBGP_MSG(LOG_STREAM(std::cerr) << "UpdateKeyValue: "      \
                       << l_keystr << "==" << l_valuestr << std::endl); \
            }                                                           \
        }                                                               \
    } while(0)

#define ADD_TO_MSG(keystream, valuestream, message)                   \
    do                                                                \
      {                                                               \
        std::string l_keystr, l_valuestr;                             \
        STREAM_TO_STRING(l_keystr, keystream);                        \
        STREAM_TO_STRING(l_valuestr, valuestream);                    \
        if(!(message).AddKeyValue(l_keystr, l_valuestr))              \
          {                                                           \
            DBGP_E(LOG_STREAM(std::cerr) << "Could not AddKeyValue: " \
                   << l_keystr << "==" << l_valuestr << std::endl);   \
          }                                                           \
        else                                                          \
          {                                                           \
            DBGP_MSG(LOG_STREAM(std::cerr) << "AddKeyValue: "         \
                     << l_keystr << "==" << l_valuestr << std::endl); \
          }                                                           \
      } while(0)

#define ADD_VECTOR_TO_MSG_IF_CHANGED(keystream, vec, message, old_message) \
    do                                                                  \
      {                                                                 \
        size_t M_i;                                                     \
        ADD_TO_MSG(keystream << "SIZE", vec.size(), message);           \
        for(M_i = 0; M_i < vec.size(); M_i++)                           \
          {                                                             \
            ADD_TO_MSG_IF_CHANGED(keystream << " " << M_i, vec[M_i], message, old_message); \
          }                                                             \
      } while(0)

#define ADD_ARRAY_TO_MSG_IF_CHANGED(keystream, array, counti, message, old_message) \
    do                                                                \
      {                                                                 \
        size_t M_i;                                                     \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            ADD_TO_MSG_IF_CHANGED(keystream << " " << M_i, array[M_i], message, old_message); \
          }                                                             \
      } while(0)


#define ADD_ARRAY_TO_MSG(keystream, array, counti, message)           \
    do                                                                \
      {                                                                 \
        size_t M_i;                                                     \
        ADD_TO_MSG(keystream << "SIZE", counti, message);               \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            ADD_TO_MSG(keystream << " " << M_i, array[M_i], message); \
          }                                                             \
      } while(0)


#define ADD_VECTOR_TO_MSG(keystream, vec, message)                  \
    do                                                              \
      {                                                             \
        size_t M_i;                                                 \
        ADD_TO_MSG(keystream << "SIZE", vec.size(), message);       \
        for(M_i = 0; M_i < vec.size(); M_i++)                       \
          {                                                         \
            ADD_TO_MSG(keystream << " " << M_i, vec[M_i], message); \
          }                                                         \
      } while(0)

#define ADD_VECTOR_OF_VECTOR_TO_MSG_IF_CHANGED(keystream, vec, message, old_message) \
    do                                                                \
      {                                                                 \
        size_t M_i,M_j;                                                 \
        ADD_TO_MSG(keystream << "SIZE", vec.size(), message);           \
        for(M_i = 0; M_i < vec.size(); M_i++)                           \
          {                                                             \
            ADD_TO_MSG(keystream << "SIZE " << M_i, vec[M_i].size(), message); \
            for(M_j = 0; M_j < vec[M_i].size(); M_j++)                  \
              {                                                         \
                ADD_TO_MSG_IF_CHANGED(keystream << " " << M_i << " " << M_j, vec[M_i][M_j], message, old_message); \
              }                                                         \
          }                                                             \
      } while(0)

#define ADD_VECTOR_OF_VECTOR_TO_MSG(keystream, vec, message)            \
    do                                                                \
      {                                                                 \
        size_t M_i,M_j;                                                 \
        ADD_TO_MSG(keystream << "SIZE", vec.size(), message);           \
        for(M_i = 0; M_i < vec.size(); M_i++)                           \
          {                                                             \
            ADD_TO_MSG(keystream << "SIZE " << M_i, vec[M_i].size(), message); \
            for(M_j = 0; M_j < vec[M_i].size(); M_j++)                  \
              {                                                         \
                ADD_TO_MSG(keystream << " " << M_i << " " << M_j, vec[M_i][M_j], message); \
              }                                                         \
          }                                                             \
      } while(0)

#define ADD_ARRAY_OF_ARRAY_TO_MSG_IF_CHANGED(keystream, array, counti, countj, message, old_message) \
    do                                                                \
      {                                                                 \
        size_t M_i,M_j;                                                 \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            for(M_j = 0; M_j < countj; M_j++)                           \
              {                                                         \
                ADD_TO_MSG_IF_CHANGED(keystream << " " << M_i << " " << M_j, array[M_i][M_j], message, old_message); \
              }                                                         \
          }                                                             \
      } while(0)

#define ADD_MAP_TO_POINTER_TO_MSG_IF_CHANGED(keystream, mp, message, old_message) \
    do                                                                \
      {                                                                 \
        size_t M_i = 0;                                                 \
        ADD_TO_MSG(keystream << "SIZE", (mp).size(), (message));        \
        for(auto M_it = (mp).begin(); M_it != (mp).end(); M_it++, M_i++) \
          {                                                             \
            if(M_it->second)                                            \
              {                                                         \
                ADD_TO_MSG(keystream << "ITEM " << M_i, M_it->first, (message)); \
                if(!M_it->second->AddToMessageIfChanged((message), (old_message))) \
                  {                                                     \
                    DBGP_E(LOG_STREAM(std::cerr) << "Failed to AddToMessageIfChange()" << std::endl); \
                  }                                                     \
              }                                                         \
            else                                                        \
              {                                                         \
                DBGP_E(LOG_STREAM(std::cerr) << #mp << " key: " << M_it->first << " has value = " << M_it->second << std::endl); \
              }                                                         \
          }                                                             \
      } while(0)

#define ADD_MAP_TO_MSG_IF_CHANGED(keystream, mp, message, old_message) \
    do                                                                  \
      {                                                                 \
        size_t M_i = 0;                                                 \
        ADD_TO_MSG(keystream << "SIZE", (mp).size(), (message));        \
        for(auto M_it = (mp).begin(); M_it != (mp).end(); M_it++, M_i++) \
          {                                                             \
            ADD_TO_MSG(keystream << "ITEM " << M_i, M_it->first, (message)); \
            ADD_TO_MSG_IF_CHANGED(keystream << "VALUE " << M_it->first, M_it->second, (message), (old_message)); \
          }                                                             \
      } while(0)


#define SET_FROM_MSG_IF_EXISTS(keystream, valuestream, message)         \
    do                                                                \
      {                                                                 \
        std::stringstream l_value;                                      \
        std::string l_keystr, l_valuestr;                               \
        STREAM_TO_STRING(l_keystr, keystream);                          \
        l_valuestr = (message).GetValue(l_keystr);                      \
        if(l_valuestr != "")                                            \
          {                                                             \
            l_value.str(l_valuestr);                                    \
            l_value >> valuestream;                                     \
            DBGP_MSG(LOG_STREAM(std::cerr) << "Changed: "               \
                     << l_keystr << "==" << l_valuestr << std::endl);   \
          }                                                             \
        else                                                            \
          {                                                             \
            DBGP_MSG(LOG_STREAM(std::cerr) << "No Change: "             \
                     << l_keystr << std::endl);                         \
          }                                                             \
      } while(0)

#define SET_FROM_MSG(keystream, valuestream, message)                   \
    do                                                                \
      {                                                                 \
        std::stringstream l_value;                                      \
        std::string l_keystr, l_valuestr;                               \
        STREAM_TO_STRING(l_keystr, keystream);                          \
        l_valuestr = (message).GetValue(l_keystr);                      \
        if(l_valuestr != "")                                            \
          {                                                             \
            l_value.str(l_valuestr);                                    \
            l_value >> valuestream;                                     \
            DBGP_MSG(LOG_STREAM(std::cerr) << "Found: "                 \
                     << l_keystr << "==" << l_valuestr << std::endl);   \
          }                                                             \
        else                                                            \
          {                                                             \
            DBGP_E(LOG_STREAM(std::cerr) << "Missing key: "             \
                   << l_keystr << std::endl);                           \
          }                                                             \
      } while(0)

#define SET_VECTOR_FROM_MSG_IF_EXISTS(keystream, vec, message)          \
    do                                                                \
      {                                                                 \
        size_t count = 0, M_i;                                              \
        SET_FROM_MSG(keystream << "SIZE", count, message);              \
        vec.resize(count);                                              \
        for(M_i = 0; M_i < count; M_i++)                                \
          {                                                             \
            SET_FROM_MSG_IF_EXISTS(keystream << " " << M_i, vec[M_i], message); \
          }                                                             \
      } while(0)

#define SET_ARRAY_FROM_MSG_IF_EXISTS(keystream, array, counti, message) \
    do                                                                \
      {                                                                 \
        size_t M_i;                                                     \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            SET_FROM_MSG_IF_EXISTS(keystream << " " << M_i, array[M_i], message); \
          }                                                             \
      } while(0)

#define SET_ARRAY_FROM_MSG(keystream, array, count, message)          \
    do                                                                \
      {                                                                 \
        size_t M_i;                                              \
        SET_FROM_MSG(keystream << "SIZE", count, message);              \
        for(M_i = 0; M_i < count; M_i++)                                \
          {                                                             \
            SET_FROM_MSG(keystream << " " << M_i, array[M_i], message); \
          }                                                             \
      } while(0)

#define SET_VECTOR_FROM_MSG(keystream, vec, message)                    \
    do                                                                \
      {                                                                 \
        size_t count = 0, M_i;                                              \
        SET_FROM_MSG(keystream << "SIZE", count, message);              \
        vec.resize(count);                                              \
        for(M_i = 0; M_i < count; M_i++)                                \
          {                                                             \
            SET_FROM_MSG(keystream << " " << M_i, vec[M_i], message);   \
          }                                                             \
      } while(0)

#define SET_VECTOR_OF_VECTOR_FROM_MSG(keystream, vec, message)          \
    do                                                                \
      {                                                                 \
        size_t counti = 0, M_i, countj = 0, M_j;                                \
        SET_FROM_MSG(keystream << "SIZE", counti, message);             \
        vec.resize(counti);                                             \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            SET_FROM_MSG(keystream << "SIZE " << M_i, countj, message); \
            vec[M_i].resize(M_j);                                       \
            for(M_j = 0; M_j < countj; M_j++)                           \
              {                                                         \
                SET_FROM_MSG(keystream << " " << M_i << " " << M_j, vec[M_i][M_j], message); \
              }                                                         \
          }                                                             \
      } while(0)

#define SET_VECTOR_OF_VECTOR_FROM_MSG_IF_EXISTS(keystream, vec, message) \
    do                                                                \
      {                                                                 \
        size_t counti = 0, M_i, countj = 0, M_j;                                \
        SET_FROM_MSG(keystream << "SIZE", counti, message);             \
        vec.resize(counti);                                             \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            SET_FROM_MSG(keystream << "SIZE " << M_i, countj, message); \
            vec[M_i].resize(M_j);                                       \
            for(M_j = 0; M_j < countj; M_j++)                           \
              {                                                         \
                SET_FROM_MSG_IF_EXISTS(keystream << " " << M_i << " " << M_j, vec[M_i][M_j], message); \
              }                                                         \
          }                                                             \
      } while(0)

#define SET_ARRAY_OF_ARRAY_FROM_MSG_IF_EXISTS(keystream, array, counti, countj, message) \
    do                                                                \
      {                                                                 \
        size_t M_i, M_j;                                                \
        for(M_i = 0; M_i < counti; M_i++)                               \
          {                                                             \
            for(M_j = 0; M_j < countj; M_j++)                           \
              {                                                         \
                SET_FROM_MSG_IF_EXISTS(keystream << " " << M_i << " " << M_j, array[M_i][M_j], message); \
              }                                                         \
          }                                                             \
      } while(0)

#define SET_MAP_TO_POINTER_FROM_MSG_IF_EXISTS(keystream, mp, keytype, ptrtype, message) \
    do                                                                \
      {                                                                 \
        size_t M_i, M_counti;                                           \
        unsigned int M_id;                                              \
        SET_FROM_MSG(keystream << "SIZE", M_counti, (message));         \
        std::set<keytype> not_here;                                     \
        for(auto M_it = (mp).begin(); M_it != (mp).end(); M_it++)       \
          {                                                             \
            not_here.insert(M_it->first);                               \
          }                                                             \
        for(M_i = 0; M_i < M_counti; M_i++)                             \
          {                                                             \
            M_id = 0x7FFFFFFF;                                          \
            SET_FROM_MSG(keystream << "ITEM " << M_i, M_id, (message)); \
            if(M_id == 0x7FFFFFFF)                                      \
              {                                                         \
                DBGP_E(LOG_STREAM(std::cerr) << "Object not here" << std::endl); \
                continue;                                               \
              }                                                         \
            auto M_iter = (mp).find(M_id);                              \
            if(M_iter == (mp).end())                                    \
              {                                                         \
                (mp)[M_id] = new ptrtype();                             \
              }                                                         \
            if((mp)[M_id])                                              \
              {                                                         \
                if(!(mp)[M_id]->SetFromMessageIfExists(M_id, imsg))     \
                  {                                                     \
                    DBGP_E(LOG_STREAM(std::cerr) << "Failed to SetFromMessageIfExists()" << std::endl); \
                  }                                                     \
                not_here.erase(M_id);                                   \
              }                                                         \
            else                                                        \
              {                                                         \
                DBGP_E(LOG_STREAM(std::cerr) << #mp << "[" << M_id << "] " << "doesn't exist." << std::endl); \
              }                                                         \
          }                                                             \
        for(auto M_it2 = not_here.begin(); M_it2 != not_here.end(); M_it2++) \
          {                                                             \
            (mp).erase(*M_it2);                                         \
          }                                                             \
      } while(0)

#define SET_MAP_FROM_MSG_IF_EXISTS(keystream, mp, keytype, message)     \
    do                                                                  \
      {                                                                 \
        size_t M_i, M_counti;                                           \
        SET_FROM_MSG(keystream << "SIZE", M_counti, (message));         \
        std::set<keytype> not_here;                                     \
        for(auto M_it = (mp).begin(); M_it != (mp).end(); M_it++)       \
          {                                                             \
            not_here.insert(M_it->first);                               \
          }                                                             \
        for(M_i = 0; M_i < M_counti; M_i++)                             \
          {                                                             \
            keytype M_id( 0 );                                          \
            SET_FROM_MSG(keystream << "ITEM " << M_i, M_id, (message)); \
            SET_FROM_MSG_IF_EXISTS(keystream << "VALUE " << M_id, (mp)[M_id], (message)); \
            not_here.erase(M_id);                                       \
          }                                                             \
        for(auto M_it2 = not_here.begin(); M_it2 != not_here.end(); M_it2++) \
          {                                                             \
            (mp).erase(*M_it2);                                         \
          }                                                             \
      } while(0)


#endif /* _MESSAGE_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
