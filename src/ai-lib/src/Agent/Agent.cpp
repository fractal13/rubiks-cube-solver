#include <ai_agent.h>

namespace ai
{
  namespace Agent
  {
    Agent::Agent()
    {
      sock = NULL;
    }

    Agent::Agent(Socket * sock_in)
    {
      AgentNameMessage tmsg;
      std::string buffer;
      sock = sock_in;
      if(!sock->Send(tmsg.ToString()) || !sock->Recv(buffer) || !tmsg.SetFromString(buffer)) {
        DBGP_E( LOG_STREAM( std::cerr )
                << "Initial socket communication with agent failed.  Exiting."
                << std::endl );
        exit(1);
      }
      name = tmsg.GetValue(MESSAGE_NAME_NAME);
      alive = true;
      trace = false;
      performance = 0;
    }
    Agent::~Agent()
    {
      DieMessage tmsg;
      if(sock && !sock->Send(tmsg.ToString()))
        exit(1);
    }
    bool Agent::IsAlive() const
    {
      return alive;
    }
    void Agent::Kill()
    {
      alive = false;
    }
    Action * Agent::Program(const Percept *percept)
    {
      Message omsg;
      std::string buffer;

      if(!percept->ToMessage(omsg))
        {
          std::cout << this << " bad percept to agent.  Terminating agent."  << std::endl;
          Kill();
          return NULL;
        }

      if(!sock->Send(omsg.ToString()) || !sock->Recv(buffer))
        {
          std::cout << this << " socket communication failed.  Terminating agent."  << std::endl;
          DBGP_E( LOG_STREAM( std::cerr )
                  << "socket communication failed.  Terminating agent." 
                  << std::endl );
          Kill();
          return NULL;
        }

      Message imsg;
      if(!imsg.SetFromString(buffer) || imsg.GetName() != MESSAGE_ACTION_NAME)
        {
          DBGP_E( LOG_STREAM( std::cerr )
                  << "Bad message from agent. (" << buffer << ")" 
                  << std::endl );
          DBGP_E( LOG_STREAM( std::cerr )
                  << "Bad message from agent. SetFromString -> (" << imsg.SetFromString( buffer ) << ")" 
                  << std::endl );
          DBGP_E( LOG_STREAM( std::cerr )
                  << "Bad message from agent. GetName -> (" << imsg.GetName( ) << ") != " << MESSAGE_ACTION_NAME
                  << std::endl );
          std::cout << this << " bad message from agent.  Terminating agent."  << std::endl;
          Kill();
          return NULL;
        }
      ai::Agent::Action *action = MessageToAction(imsg);

      if(trace)
        {
          std::cout << this    << " perceives "
                    << percept << " and does "
                    << action  << std::endl;
        }
      return action;
    }
    bool Agent::TextDisplay(std::ostream & os) const
    {
      os << name;
      return true;
    }
    // Add Object's data to omsg, if values are different than stored in old_msg
    bool Agent::AddToMessageIfChanged(ai::Agent::Message &omsg, ai::Agent::Message &old_msg)
    {
      if(!Object::AddToMessageIfChanged(omsg, old_msg))
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Failed to add to message." << std::endl);
          return false;
        }
      // alive, performance
      ADD_TO_MSG_IF_CHANGED("OBJECT_AGENT_ALIVE " << m_object_id, alive, omsg, old_msg);
      ADD_TO_MSG_IF_CHANGED("OBJECT_AGENT_PERFORMANCE " << m_object_id, performance, omsg, old_msg);
      return true;
    }
    // Set Object's data from imsg, if values associated with id are present
    bool Agent::SetFromMessageIfExists(unsigned int id, ai::Agent::Message &imsg)
    {
      if(!Object::SetFromMessageIfExists(id, imsg))
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Failed to extract from message." << std::endl);
          return false;
        }
      // alive, performance
      SET_FROM_MSG_IF_EXISTS("OBJECT_AGENT_ALIVE " << m_object_id, alive, imsg);
      SET_FROM_MSG_IF_EXISTS("OBJECT_AGENT_PERFORMANCE " << m_object_id, performance, imsg);
      return true;
    }
    void Agent::Trace()
    {
      trace = true;
    }
    void Agent::NoTrace()
    {
      trace = false;
    }

    void Agent::SetPerformance(int value_in)
    {
      performance = value_in;
    }
    void Agent::AddPerformance(int value_in)
    {
      performance += value_in;
    }
    int Agent::GetPerformance() const
    {
      return performance;
    }
    Socket * Agent::GetSocket() const
    {
      return sock;
    }


    std::ostream & operator<<(std::ostream &os, const Agent * agent)
    {
      agent->TextDisplay(os);
      return os;
    }

  }

}
