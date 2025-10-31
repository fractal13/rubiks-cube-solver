#ifndef _PERCEPTATOM_H_
#define _PERCEPTATOM_H_
#include <string>

namespace ai
{
  namespace Agent
  {
    class PerceptAtom
    {
    public:
      PerceptAtom();
      PerceptAtom(const std::string &name_in, const std::string &value_in);
      const std::string &GetName() const;
      const std::string &GetValue() const;
      void SetValue(const std::string &value_in);
    protected:
      std::string name;
      std::string value;
    private:
    };
  }
}

#endif /* _PERCEPTATOM_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
