#ifndef _CGLRUBIKSCOMMAND_H_
#define _CGLRUBIKSCOMMAND_H_

#include <string>

namespace cgl {
  namespace rubiks {

    // forward declaration of AppData
    struct AppData;
    typedef int (*ActionFunctionType)( AppData& data );
    struct ActionRecord {
    public:
      ActionFunctionType action;  // the function
      std::string        name;    // the keyword
      std::string        usage;   // usage message
      std::string        help;    // help message
    };

    enum ExitStatus { EXIT_OK, EXIT_USAGE, EXIT_HELP, EXIT_USAGE_HELP, EXIT_EXIT, EXIT_THROW };

    /* cglRubiksSearch.cpp */
    int search( AppData& data );
    int plan( AppData& data );

  }
}

#endif /* _CGLRUBIKSCOMMAND_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
