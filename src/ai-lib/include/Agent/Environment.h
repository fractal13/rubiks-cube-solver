#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_
#include <vector>
#include <map>

namespace ai
{
  namespace Agent
  {
    class EnvironmentDisplay;

    class Environment
    {
      friend class ai::Agent::EnvironmentDisplay;
    public:
      Environment();
      Environment(const RandomNumber &rand_in, std::vector<Socket *> *displays_in);
      virtual ~Environment();
      virtual bool ApplyAction(Agent *agent, Action *action) = 0;
      virtual void CalculatePerformance(Agent *agent) = 0;
      virtual Percept *GetPercept(Agent *agent);
      bool IsDone() const;
      virtual void Step();
      virtual void Run(const int n_steps);
      void FineSleep( double seconds ) const;
      virtual void AddAgent(Agent *agent);
      virtual void AddAgent(Agent *agent, Location *location);
      virtual void AddObject(Object *object, Location *location);
      virtual Location *DefaultLocation() const = 0;
      virtual bool GetUpdateMessage(Message &omsg);
      virtual bool UpdateFromMessage(Message &imsg);
      void UpdateDisplays();
      void SetDelay(double delay_in);
      int GetPerformance(Agent *agent);
      Agent * GetAgent(int agent_index);
      Object * GetObject(unsigned int object_id);
      const RandomNumber &GetRandomNumber() const;
      virtual void Reset();
    protected:
      std::map<unsigned int, Object *> objects;
      std::map<unsigned int, Agent *>  agents;
      std::vector<unsigned int> agents_order;
      std::vector<Socket *> *displays;
      //ai::Agent::EnvironmentDisplay   *display;
      double                delay;
      RandomNumber          random_number;
      Message mLastMessage;
    private:
    };
  }
}

/**
 * To be used by derived classes to make sure objects
 * exist before being unpacked from messages.
 * Needs to be done in derived class to pick up correct
 * derived concret object type.
 */
#define MSG_MAKE_OBJECTS_EXIST(imsg)                      \
  do                                                      \
    {                                                     \
      unsigned int id;                                    \
      size_t i, count;                                    \
      Object *obj;                                        \
      SET_FROM_MSG("OBJECT_SEQ_COUNT", count, imsg);      \
      for(i = 0; i < count; i++)                          \
        {                                                 \
          SET_FROM_MSG("OBJECT_SEQ_ID " << i, id, imsg);  \
          obj = dynamic_cast<Object*>(objects[id]);       \
          if(!obj)                                        \
            {                                             \
              objects[id] = new Object();                 \
            }                                             \
        }                                                 \
    } while(0)

/**
 * To be used by derived classes to make sure agents
 * exist before being unpacked from messages.
 * Needs to be done in derived class to pick up correct
 * derived concret agent type.
 */
#define MSG_MAKE_AGENTS_EXIST(imsg)                     \
  do                                                    \
    {                                                   \
      unsigned int id;                                  \
      size_t i, count = 0;                              \
      Agent *agent;                                     \
      SET_FROM_MSG("AGENT_SEQ_COUNT", count, imsg);     \
      for(i = 0; i < count; i++)                        \
        {                                               \
          SET_FROM_MSG("AGENT_SEQ_ID " << i, id, imsg); \
          agent = dynamic_cast<Agent*>(agents[id]);     \
          if(!agent)                                    \
            {                                           \
              agents[id] = new Agent();                 \
            }                                           \
        }                                               \
    } while(0)

#define MSG_ENVIRONMENT_PREP(imsg)              \
  do                                            \
    {                                           \
      MSG_MAKE_OBJECTS_EXIST(imsg);             \
      MSG_MAKE_AGENTS_EXIST(imsg);              \
    } while(0)

#endif /* _ENVIRONMENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
