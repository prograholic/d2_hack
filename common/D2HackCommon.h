#ifndef D2_HACK_COMMON_D2_HACK_COMMON_H
#define D2_HACK_COMMON_D2_HACK_COMMON_H

#include <iterator>
#include <vector>
#include <iomanip>

#include <boost/cstdint.hpp>
#include <boost/range.hpp>
#include <boost/range/iterator.hpp>

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


  class SeparatorBase : private boost::noncopyable
  {
  public:

    virtual bool isSeparator(char symbol) = 0;

    virtual bool skip() = 0;
  };

  class Reader
  {
  public:
    explicit Reader(std::istream& input);

  protected:

    template <typename OutputIteratorT>
    size_t readUntil(OutputIteratorT res, SeparatorBase& separator)
    {
      size_t count = 0;
      for ( ; ; )
      {
        if (mBegin == mEnd)
        {
          throwError("unexpected end of file", "Reader::readUntil");
        }
        const char data = *mBegin;
        if (separator.isSeparator(data))
        {
          if (separator.skip())
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
    template <typename InnerSeparatorT, bool skipSeparator>
    class SkipAdapter : public SeparatorBase
    {
    public:
      virtual bool skip()
      {
        return skipSeparator;
      }

      virtual bool isSeparator(char symbol)
      {
        return static_cast<InnerSeparatorT *>(this)->isSeparator(symbol);
      }
    };

    template <char sepSymbol, bool skip>
    class SymbolSeparatorBase : public SkipAdapter<SymbolSeparatorBase<sepSymbol, skip>, skip>
    {
    public:
      bool isSeparator(char symbol)
      {
        return symbol == sepSymbol;
      }
    };

    class ReadCount : public SkipAdapter<ReadCount, false>
    {
    public:
      explicit ReadCount(size_t count);

      bool isSeparator(char symbol);

    private:
      size_t mCount;
    };


    template <typename T>
    struct empty_container
    {
      typedef const T& const_reference;
      typedef T value_type;

      void push_back(const T& /* unused */)
      {

      }
    };


    namespace dump_type
    {
      enum Value
      {
        String,
        Data
      };
    }

    template <typename RangeT>
    std::ostream& dump(std::ostream &outStream, const RangeT& range, dump_type::Value dumpType, size_t* size = 0)
    {
      typedef typename boost::range_iterator<const RangeT>::type iterator_t;

      iterator_t begin = boost::begin(range);
      iterator_t end = boost::end(range);

      size_t tmpSize = 0;

      while (begin != end)
      {
        unsigned char symbol = *begin;
        if (dumpType == dump_type::String)
        {
          if (std::isprint(symbol))
          {
            outStream << symbol;
          }
          else
          {
            if (symbol != 0)
            {
              outStream << "\\x";
              outStream << std::hex << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(symbol) & 0xFF);
            }
          }
        }
        else
        {
          outStream << std::hex << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(symbol) & 0xFF);
        }

        ++begin;
        ++tmpSize;
      }

      if (size)
      {
        *size = tmpSize;
      }

      return outStream;
    }
  }
}


#endif /* D2_HACK_COMMON_D2_HACK_COMMON_H */
