#ifndef _FILEDESCRIPTOR_H_
#define _FILEDESCRIPTOR_H_
#include <iosfwd>

template <typename charT, typename traits>
int fileno(const std::basic_ios<charT, traits>& stream);

#endif /* _FILEDESCRIPTOR_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
