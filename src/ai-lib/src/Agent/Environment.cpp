#include <ai_agent.h>
// for finer precision sleep command
#include <thread>
#include <chrono>

namespace ai
{
  namespace Agent
  {
    Environment::Environment()
    {
      displays = NULL;
    }

    Environment::Environment(const RandomNumber &rand_in, std::vector<Socket *> *displays_in)
      : displays(displays_in), delay(0.0), random_number(rand_in)
    {
      // empty
    }
    Environment::~Environment()
    {
      Reset();
    }
    Percept *Environment::GetPercept(Agent *agent)
    {
      Percept *percept = new Percept();

      Percept *p = agent->GetPercept(agent->GetLocation());
      percept->Join(p);
      delete p;

      std::map<unsigned int, Object *>::iterator oit;
      for(oit = objects.begin(); oit != objects.end(); oit++)
        {
          p = oit->second->GetPercept(agent->GetLocation());
          percept->Join(p);
          delete p;
        }
      return percept;
    }
    bool Environment::IsDone() const
    {
      std::map<unsigned int, Agent *>::const_iterator ait;
      for(ait = agents.begin(); ait != agents.end(); ait++)
        {
          if(ait->second->IsAlive()) { return false; }
        }
      return true;
    }
    void Environment::Step()
    {
      std::vector<Action *> actions;
      size_t i;
      for(i = 0; i < agents_order.size(); i++)
        {
          Agent *agent = agents[agents_order[i]];
          Percept *percept = GetPercept(agent);
          Action  *action  = agent->Program(percept);
          if(action)
            {
              actions.push_back(action);
            }
          delete percept;
        }
      for(i = 0; i < agents_order.size(); i++)
        {
          Agent *agent = agents[agents_order[i]];
          ApplyAction(agent, actions[i]);
        }
      for(i = 0; i < actions.size(); i++)
        {
          delete actions[i];
        }
      for(i = 0; i < agents_order.size(); i++)
        {
          Agent *agent = agents[agents_order[i]];
          CalculatePerformance(agent);
        }
      UpdateDisplays();
    }
    void Environment::Run(const int n_steps)
    {
      int t;
      size_t i;
      for(i = 0; i < agents_order.size(); i++)
        {
          Agent *agent = agents[agents_order[i]];
          agent->SetPerformance(0);
        }
      UpdateDisplays();
      if(delay > 0.0)
        {
          FineSleep(delay);
        }
      for(t = 0; t < n_steps; t++)
        {
          if(IsDone())
            {
              return;
            }
          Step();
          if(delay > 0.0)
            {
              FineSleep(delay);
            }
        }
    }
    void Environment::FineSleep( double seconds ) const
    {
      if( seconds > 0.0 ) 
        {
          std::this_thread::sleep_for(std::chrono::microseconds((int)(1000000*seconds)));
        }
    }
    void Environment::AddAgent(Agent *agent)
    {
      AddAgent(agent, 0);
    }
    void Environment::AddAgent(Agent *agent, Location *location)
    {
      if(location)
        {
          agent->SetLocation(location);
        }
      else
        {
          agent->SetLocation(DefaultLocation());
        }
      agent->SetEnvironment(this);
      agents[agent->GetIdNum()] = agent;
      agents_order.push_back(agent->GetIdNum());
    }
    void Environment::AddObject(Object *object, Location *location)
    {
      if(location)
        {
          object->SetLocation(location);
        }
      else
        {
          object->SetLocation(DefaultLocation());
        }
      object->SetEnvironment(this);
      objects[object->GetIdNum()] = object;
    }
    bool Environment::GetUpdateMessage(Message &omsg)
    {
      if(!omsg.SetName(ai::Agent::MESSAGE_UPDATE_NAME))
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Failed to set message name." << std::endl);
          return false;
        }

      bool rval = true;
      int i;
      /**
       * Add objects, make sure we know how to unpack them later
       * Use OBJECT_SEQ_COUNT to identify how many there are
       * Use OBJECT_SEQ_ID # to identify the object_id for each
       */
      ADD_TO_MSG("OBJECT_SEQ_COUNT", objects.size(), omsg);
      i = 0;
      for(auto oit = objects.begin(); oit != objects.end(); oit++, i++)
        {
          ADD_TO_MSG("OBJECT_SEQ_ID " << i, oit->second->GetIdNum(), omsg);
          if(!oit->second->AddToMessageIfChanged(omsg, mLastMessage))
            {
              DBGP_E(LOG_STREAM(std::cerr) << "Failed to add object to message." << std::endl);
              rval = false;
            }
        }

      /**
       * Add agents, make sure we know how to unpack them later
       * Use AGENT_SEQ_COUNT to identify how many there are
       * Use AGENT_SEQ_ID # to identify the object_id for each
       */
      ADD_TO_MSG("AGENT_SEQ_COUNT", agents.size(), omsg);
      i = 0;
      for(auto ait = agents.begin(); ait != agents.end(); ait++, i++)
        {
          ADD_TO_MSG("AGENT_SEQ_ID " << i, ait->second->GetIdNum(), omsg);
          if(!ait->second->AddToMessageIfChanged(omsg, mLastMessage))
            {
              DBGP_E(LOG_STREAM(std::cerr) << "Failed to add agent to message." << std::endl);
              rval = false;
            }
        }

      /**
       * Add agents_order
       */
      i = 0;
      for(auto ait = agents_order.begin(); ait != agents_order.end(); ait++, i++)
        {
          ADD_TO_MSG_IF_CHANGED("AGENT_ORDER_ID " << i, *ait, omsg, mLastMessage);
        }

      return rval;
    }
    bool Environment::UpdateFromMessage(Message &imsg)
    {
      if(imsg.GetName() != ai::Agent::MESSAGE_UPDATE_NAME)
        {
          DBGP_E(LOG_STREAM(std::cerr) << "Bad message name: " << imsg.GetName() << "." << std::endl);
          return false;
        }
      bool rval = true;
      size_t i, count = 0;
      unsigned int id;
      /**
       * Extract objects
       * Use OBJECT_SEQ_COUNT to identify how many there are
       * Use OBJECT_SEQ_ID # to identify the object_id for each
       */
      Object *obj;

      SET_FROM_MSG_IF_EXISTS("OBJECT_SEQ_COUNT", count, imsg);
      for(i = 0; i < count; i++)
        {
          SET_FROM_MSG_IF_EXISTS("OBJECT_SEQ_ID " << i, id, imsg);
          obj = objects[id];
          if(obj)
            {
              if(!obj->SetFromMessageIfExists(id, imsg))
                {
                  DBGP_E(LOG_STREAM(std::cerr) << "Failed to extract object from message." << std::endl);
                  rval = false;
                }
            }
          else
            {
              DBGP_E(LOG_STREAM(std::cerr) << "Object["<<id<<"] is NULL." << std::endl);
            }
        }

      /**
       * Extract agents
       * Use AGENT_SEQ_COUNT to identify how many there are
       * Use AGENT_SEQ_ID # to identify the object_id for each
       */
      Agent *agent;

      SET_FROM_MSG_IF_EXISTS("AGENT_SEQ_COUNT", count, imsg);
      for(i = 0; i < count; i++)
        {
          SET_FROM_MSG_IF_EXISTS("AGENT_SEQ_ID " << i, id, imsg);
          agent = agents[id];
          if(agent)
            {
              if(!agent->SetFromMessageIfExists(id, imsg))
                {
                  DBGP_E(LOG_STREAM(std::cerr) << "Failed to extract agent from message." << std::endl);
                  rval = false;
                }
            }
          else
            {
              DBGP_E(LOG_STREAM(std::cerr) << "Agent["<<id<<"] is NULL." << std::endl);
            }
        }

      /**
       * Extract agents_order
       */
      agents_order.resize(count);
      for(i = 0; i < count; i++)
        {
          SET_FROM_MSG_IF_EXISTS("AGENT_ORDER_ID " << i, agents_order[i], imsg);
        }

      return rval;
    }
    void Environment::UpdateDisplays()
    {
      if(displays->size() <= 0)
        {
          return;
        }
      Message omsg;
      GetUpdateMessage(omsg);
      std::vector<Socket *>::iterator i;
      for(i = displays->begin(); i != displays->end(); i++)
        {
          if(!(*i)->Send(omsg.ToString()))
            {
              delete (*i);
              displays->erase(i--);
            }
        }
    }
    void Environment::SetDelay(double delay_in)
    {
      delay = delay_in;
    }
    int Environment::GetPerformance(Agent *agent)
    {
      return agent->GetPerformance();
    }
    Agent * Environment::GetAgent(int agent_index)
    {
      return agents[agents_order[agent_index]];
    }
    Object * Environment::GetObject(unsigned int object_id)
    {
      return objects[object_id];
    }
    const RandomNumber &Environment::GetRandomNumber() const
    {
      return random_number;
    }
    void Environment::Reset()
    {
      std::map<unsigned int, Object *>::iterator oit;
      for(oit = objects.begin(); oit != objects.end(); oit++)
        {
          delete oit->second;
          oit->second = 0;
        }
      objects.clear();
      std::map<unsigned int, Agent *>::iterator ait;
      for(ait = agents.begin(); ait != agents.end(); ait++)
        {
          delete ait->second;
          ait->second = 0;
        }
      agents.clear();
      agents_order.clear();
      if(displays)
        {
          std::vector<Socket *>::iterator i;
          for(i = displays->begin(); i != displays->end(); i++)
            {
              DieMessage tmsg;
              if(!(*i)->Send(tmsg.ToString()))
                {
                  delete (*i);
                  displays->erase(i--);
                }
            }
        }
      // Don't delete displays, it's not ours
    }

  }
}
