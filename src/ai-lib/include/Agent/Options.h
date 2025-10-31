#ifndef _OPTIONS_H_
#define _OPTIONS_H_
#include <map>
#include <string>

namespace ai
{
  namespace Agent
  {
    struct Option
    {
      std::string name;
      char code;
      std::string arg;
      std::string desc;
    };

    class Options
    {
    public:
      void AddOption(std::string name, char code, std::string arg, std::string desc);
      void AddOptions(struct Option * options);
      void Initialize(int argc, char **argv);
      std::string GetArg(std::string name);
      char GetArgChr(std::string name);
      double GetArgFlt(std::string name);
      int GetArgInt(std::string name);
      unsigned int GetArgUnsignedInt(std::string name);
    protected:
      std::map<std::string,Option> mOptions;
    private:
    };
  }
}

#endif /* _OPTIONS_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
