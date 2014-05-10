#include "D2HackCommon.h"


#include <boost/lexical_cast.hpp>

namespace file_io
{

  Reader::Reader(std::istream &input)
    : mOffset(0), mBegin(input), mEnd()
  {
  }

  void Reader::throwError(const Ogre::String & msg, const Ogre::String & where)
  {
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                msg + ", offset: " + boost::lexical_cast<std::string>(mOffset),
                where);
  }

  namespace helpers
  {
    bool isSameSymbol(const char symbol, const char testingSymbol)
    {
      return symbol == testingSymbol;
    }

    ReadCount::ReadCount(size_t count)
        : mCount(count)
    {
    }

    bool ReadCount::operator ()(char /* symbol */)
    {
      return (mCount--) == 0;
    }
  }
}
