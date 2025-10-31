#include <ai_agent.h>
#include <getopt.h>

namespace ai
{
  namespace Agent
  {
    void Options::AddOption(std::string name, char code, std::string arg, std::string desc)
    {
      Option o;
      o.name = name;
      o.code = code;
      o.arg = arg;
      o.desc = desc;
      mOptions[name] = o;
    }

    void Options::AddOptions(struct Option * options)
    {
      for(int i = 0; true; i++)
        {
          if(options[i].name == "")
            break;
          mOptions[options[i].name] = options[i];
        }
    }

    void Options::Initialize(int argc, char **argv)
    {
      std::string short_options;
      struct option * long_options = new option[mOptions.size()+1];

      unsigned int i;
      std::map<std::string, Option>::iterator it;

      for(it = mOptions.begin(), i = 0; it != mOptions.end(); it++, i++)
        {
          Option *o = &((*it).second);
          if(o->code)
            {
              short_options.push_back(o->code);
              short_options.push_back(':');
            }
          long_options[i].name = o->name.c_str();
          long_options[i].has_arg = required_argument;
          long_options[i].flag = 0;
          long_options[i].val = o->code;
          DBGP_INFO(LOG_STREAM(std::cout) << "long_options["<<i<<"] = " << long_options[i].name << " "
                    << long_options[i].has_arg << " "
                    << long_options[i].flag << " "
                    << long_options[i].val
                    <<  std::endl);
        }
      DBGP_INFO(LOG_STREAM(std::cout) << "short_options = " << short_options << std::endl);
      long_options[mOptions.size()].name = 0;
      long_options[mOptions.size()].has_arg = 0;
      long_options[mOptions.size()].flag = 0;
      long_options[mOptions.size()].val = 0;

      while(1)
        {
          int option_index = -1;
          int c = getopt_long(argc, argv, short_options.c_str(), long_options, &option_index);

          DBGP_INFO(LOG_STREAM(std::cout) << "c =            " << c << " " << ((char)c) << std::endl);
          DBGP_INFO(LOG_STREAM(std::cout) << "option_index = " << option_index << std::endl);
          DBGP_INFO(LOG_STREAM(std::cout) << "opt_arg =      " << optarg << std::endl);

          if(option_index == -1)
            for(i = 0; i < mOptions.size(); i++)
              if(long_options[i].val == c)
                option_index = i;

          if(c == '?')
            {
              for(it = mOptions.begin(); it != mOptions.end(); it++)
                {
                  Option o = (*it).second;
                  std::cerr << "--" << o.name;
                  if(o.code)
                    std::cerr << " , -" << o.code;
                  std::cerr << " : " << o.desc << " [" << o.arg << "]" << std::endl;
                }
              exit(1);
            }
          else if(c == -1)
            break;
          else
            mOptions[long_options[option_index].name].arg = optarg;
        }
      if( long_options ) {
        delete [ ] long_options;
        long_options = 0;
      }
    }

    std::string Options::GetArg(std::string name)
    {
      return mOptions[name].arg;
    }

    char Options::GetArgChr(std::string name)
    {
      return mOptions[name].arg[0];
    }

    double Options::GetArgFlt(std::string name)
    {
      return atof(mOptions[name].arg.c_str());
    }

    int Options::GetArgInt(std::string name)
    {
      return atoi(mOptions[name].arg.c_str());
    }

    unsigned int Options::GetArgUnsignedInt(std::string name)
    {
      return static_cast<unsigned int>(strtoul(mOptions[name].arg.c_str(), 0, 10));
    }
  }
}
