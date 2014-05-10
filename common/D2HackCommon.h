#ifndef D2_HACK_COMMON_D2_HACK_COMMON_H
#define D2_HACK_COMMON_D2_HACK_COMMON_H

#include <iterator>
#include <vector>

#include <boost/cstdint.hpp>

#include <OgreException.h>
#include <OgreString.h>
#include <OgreLogManager.h>


#ifndef OGRE_LOG
#define OGRE_LOG(x) Ogre::LogManager::getSingleton().stream() << #x << ": "
#endif /* OGRE_LOG */




namespace file_io
{
  typedef std::istreambuf_iterator<char> data_iterator;
  typedef std::iterator_traits<data_iterator> data_iter_traits;

  typedef std::vector<boost::uint8_t> blob_t;

  class Reader
  {
  public:
    explicit Reader(std::istream& input);

  protected:

    template <typename OutputIteratorT, typename SeparatorFunctionT>
    size_t readUntil(OutputIteratorT res, SeparatorFunctionT separator, bool skipSeparator)
    {
      size_t count = 0;
      for ( ; ; )
      {
        if (mBegin == mEnd)
        {
          throwError("unexpected end of file", "Reader::readUntil");
        }
        const char data = *mBegin;
        if (separator(data))
        {
          if (skipSeparator)
          {
            ++mBegin;
            ++mOffset;
            ++count;
          }
          break;
        }

        *res = data;
        ++res;
        ++mBegin;
        ++mOffset;
        ++count;
      }

      return count;
    }

    void throwError(const Ogre::String & msg, const Ogre::String & where);

    size_t mOffset;
    data_iterator mBegin;
    data_iterator mEnd;
  };

  namespace helpers
  {
    bool isSameSymbol(const char symbol, const char testingSymbol);

    class ReadCount
    {
    public:
      explicit ReadCount(size_t count);

      bool operator ()(char symbol);
    private:
      size_t mCount;
    };


    template <typename T>
    struct empty_container
    {
      typedef const T& const_reference;

      void push_back(const T& /* unused */)
      {

      }
    };
  }
}


#endif /* D2_HACK_COMMON_D2_HACK_COMMON_H */
