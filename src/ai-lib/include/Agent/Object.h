#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <string>

namespace ai
{
  namespace Agent
  {
    class Environment;
    class Object
    {
    public:
      Object();
      Object(const std::string &name_in);
      Object(const std::string &name_in, Location *location_in);
      Object(Location *location_in);
      virtual ~Object();
      virtual bool IsAlive() const;
      void SetName(const std::string &name_in);
      const std::string & GetName() const;
      void SetLocation(Location *location_in);
      Location * GetLocation();
      virtual Percept *GetPercept(const Location *location_in) = 0;

      // Fetch Object.m_object_id
      unsigned int GetIdNum() const;
      // Fetch Object.m_object_id_str
      const std::string &GetIdString() const;
      // Extract the id number from the string
      static unsigned int GetIdNum(const std::string &id_string);
      // Assign Object.m_object_id and Object.m_object_id_str using Object.m_largest_object_id
      bool AutoSetId();
      // Assign Object.m_object_id and Object.m_object_id_str update Object.m_largest_object_id if necessary
      bool SetId(unsigned int id);
      // Add Object's data to omsg, if values are different than stored in old_msg
      virtual bool AddToMessageIfChanged(ai::Agent::Message &omsg, ai::Agent::Message &old_msg);
      // Set Object's data from imsg, if values associated with id are present
      virtual bool SetFromMessageIfExists(unsigned int id, ai::Agent::Message &imsg);

      void SetEnvironment(Environment *env_in);
      Environment * GetEnvironment() const;
    protected:
      std::string name;
      Location *location;

      /**
       * Object.m_object_id
       *
       * This code will not be thread safe unless something is done
       * to protect the class variable.
       *
       * m_largest_object_id:  Class variable to maintain the uniqueness of m_object_id
       * m_object_id:          Each Object has a unique id number
       * m_object_id_str:      A string representation of the number, preceded by "OBJ"
       */

      static unsigned int m_largest_object_id;
      unsigned int m_object_id;
      std::string m_object_id_str;

      Environment  *environment;
    private:
    };
  }
}

#endif /* _OBJECT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
