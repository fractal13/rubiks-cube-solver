#ifndef _DISPLAYDRIVER_H_
#define _DISPLAYDRIVER_H_
#include <vector>

namespace ai
{
  namespace Agent
  {
    class DisplayDriver
    {
    public:
      DisplayDriver(int argc, char **argv);
      virtual ~DisplayDriver();
      virtual void SetEnvironment(Environment * env);
      // Must set mEnvironmentDisplay to be a correct
      // EnvironmentDisplay, passing env.  Must also
      // set mEnvironmentDisplaySet.
      virtual void SetEnvironmentDisplay(Environment * env) = 0;
      virtual void Run();
      virtual void DrawFrame();
      virtual void GetEnvironment();
    protected:
      Environment        * mEnvironment;
      volatile bool mEnvironmentSet;
      EnvironmentDisplay * mEnvironmentDisplay;
      volatile bool mEnvironmentDisplaySet;
      Options mOptions;
      Socket mSocket;
      bool   mClose;
    private:

    };
  }
}

#endif /* _DISPLAYDRIVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
