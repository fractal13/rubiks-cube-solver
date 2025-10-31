#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    unsigned int Object::m_largest_object_id = 1;

    Object::Object()
      : name(""), location(0), environment(0)
    {
      AutoSetId();
    }
    Object::Object(const std::string &name_in)
      : name(name_in), location(0), environment(0)
    {
      AutoSetId();
    }
    Object::Object(const std::string &name_in, Location *location_in)
      : name(name_in), location(location_in), environment(0)
    {
      AutoSetId();
    }
    Object::Object(Location *location_in)
      : name(""), location(location_in), environment(0)
    {
      AutoSetId();
    }
    Object::~Object()
    {
      if(location)
        {
          delete location;
          location = 0;
        }
    }
    bool Object::IsAlive() const
    {
      return false;
    }
    void Object::SetName(const std::string &name_in)
    {
      name = name_in;
    }
    const std::string & Object::GetName() const
    {
      return name;
    }
    void Object::SetLocation(Location *location_in)
    {
      location = location_in;
    }
    Location * Object::GetLocation()
    {
      return location;
    }
    // Fetch Object.m_object_id
    unsigned int Object::GetIdNum() const
    {
      return m_object_id;
    }
    // Fetch Object.m_object_id_str
    const std::string &Object::GetIdString() const
    {
      return m_object_id_str;
    }
    // Extract the id number from the string
    unsigned int Object::GetIdNum(const std::string &id_string)
    {
      if(id_string.size() > 3)
        {
          std::stringstream ss;
          unsigned int id_num;
          ss.str(id_string.substr(3));
          ss >> id_num;
          return id_num;
        }
      return 0;
    }
    // Assign Object.m_object_id and Object.m_object_id_str using Object.m_largest_object_id
    bool Object::AutoSetId()
    {
      // This is NOT-THREAD-SAFE.  Others may be working on this variable at the same time.
      m_object_id = ++m_largest_object_id;
      std::stringstream ss;
      ss << "OBJ" << m_object_id;
      m_object_id_str = ss.str();
      return true;
    }
    // Assign Object.m_object_id and Object.m_object_id_str update Object.m_largest_object_id if necessary
    bool Object::SetId(unsigned int id)
    {
      // Assume we know what we are doing with manually set ids.  Just make sure that
      // new Objects get a unique id.
      if(id > m_largest_object_id)
        {
          m_largest_object_id = id;
        }
      m_object_id = id;
      std::stringstream ss;
      ss << "OBJ" << m_object_id;
      m_object_id_str = ss.str();
      return true;
    }
    // Add Object's data to omsg, if values are different than stored in old_msg
    bool Object::AddToMessageIfChanged(ai::Agent::Message &omsg, ai::Agent::Message &old_msg)
    {
      /**
       * name, location, m_object_id
       */
      ADD_TO_MSG_IF_CHANGED("OBJECT_NAME " << m_object_id, name, omsg, old_msg);
      // location is not saved here.  It is expected that derived Object classes
      // will do the correct work to save/restore specific location data.
      ADD_TO_MSG_IF_CHANGED("OBJECT_ID " << m_object_id, m_object_id, omsg, old_msg);
      return true;
    }
    // Set Object's data from imsg, if values associated with id are present
    bool Object::SetFromMessageIfExists(unsigned int id, ai::Agent::Message &imsg)
    {
      /**
       * name, location, m_object_id
       */
      SET_FROM_MSG_IF_EXISTS("OBJECT_NAME " << id, name, imsg);
      // location is not saved here.  It is expected that derived Object classes
      // will do the correct work to save/restore specific location data.
      SET_FROM_MSG_IF_EXISTS("OBJECT_ID " << id, m_object_id, imsg);
      return true;
    }
    void Object::SetEnvironment(Environment *env_in)
    {
      environment = env_in;
    }
    Environment * Object::GetEnvironment() const
    {
      return environment;
    }

  }
}
