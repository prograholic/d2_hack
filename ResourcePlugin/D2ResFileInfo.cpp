#include "D2ResFileInfo.h"

#include <OgreException.h>

#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>

namespace
{
  typedef std::istreambuf_iterator<char> data_iterator;
  typedef std::iterator_traits<data_iterator> data_iter_traits;

  typedef std::vector<boost::uint8_t> blob_t;


  struct Header
  {
    Header()
      : name()
      , count(0)
    {

    }

    std::string name;
    size_t count;
  };

  inline bool operator < (const Header & lhs, const Header & rhs)
  {
    return lhs.name < rhs.name;
  }

  typedef std::map<std::string, blob_t> DataDictionary;

  typedef std::map<Header, DataDictionary> FileSystem;



  bool isSameSymbol(const char symbol, const char testingSymbol)
  {
    return symbol == testingSymbol;
  }

  class ReadCount
  {
  public:
    explicit ReadCount(size_t count)
      : mCount(count)
    {

    }

    bool operator ()(char /* unused */)
    {
      return (mCount--) == 0;
    }
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



  typedef boost::function<bool (const std::string & sectionName, std::string & name, D2ResEntry & entry)> resource_watcher_t;

  typedef std::map<std::string, resource_watcher_t> ParserDispatcher;



  class D2ResInfoWatcher
  {
  public:

    explicit D2ResInfoWatcher(std::istream & stream)
      : mOffset(0)
      , mBegin(stream)
      , mEnd()
      , mDispatcher()
    {
      mDispatcher["COLORS"] = boost::bind(&D2ResInfoWatcher::skipNonFilesData, this, _1, _2, _3);
      mDispatcher["MATERIALS"] = boost::bind(&D2ResInfoWatcher::skipNonFilesData, this, _1, _2, _3);

      mDispatcher["TEXTUREFILES"] = boost::bind(&D2ResInfoWatcher::skipFilesData, this, _1, _2, _3);
      mDispatcher["PALETTEFILES"] = boost::bind(&D2ResInfoWatcher::skipFilesData, this, _1, _2, _3);
      mDispatcher["BACKFILES"] = boost::bind(&D2ResInfoWatcher::skipFilesData, this, _1, _2, _3);
      mDispatcher["MASKFILES"] = boost::bind(&D2ResInfoWatcher::skipFilesData, this, _1, _2, _3);
      mDispatcher["SOUNDFILES"] = boost::bind(&D2ResInfoWatcher::skipFilesData, this, _1, _2, _3);

    }


    size_t readResFile(D2ResFileInfo & fileInfo)
    {
      size_t count = 0;
      while (mBegin != mEnd)
      {
        Header header;
        count += readHeader(header);

        ParserDispatcher::const_iterator parser = mDispatcher.find(header.name);
        if (parser != mDispatcher.end())
        {
          for (size_t i = 0; i != header.count; ++i)
          {
            D2ResEntry entry;
            std::string dataName;

            count += parser->second(header.name, dataName, entry);
            fileInfo[dataName] = entry;
          }
        }
        else
        {
          throwError("cannot find readed for entry [" + header.name + "]", "D2ResInfoWatcher::readResFile");
        }
      }

      return count;
    }

  private:

    template <typename OutputIteratorT, typename SeparatorFunctionT>
    size_t readUntil(OutputIteratorT res, SeparatorFunctionT separator, bool skipSeparator)
    {
      size_t count = 0;
      for ( ; ; )
      {
        if (mBegin == mEnd)
        {
          throwError("unexpected end of file", "D2ResInfoWatcher::readUntil");
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

    size_t readHeader(Header & header)
    {
      size_t count = 0;

      count += readUntil(std::back_inserter(header.name), boost::bind(isSameSymbol, ' ', _1), true);

      std::string entitiesCount;
      count += readUntil(std::back_inserter(entitiesCount), boost::bind(isSameSymbol, '\0', _1), true);

      header.count = boost::lexical_cast<size_t>(entitiesCount);
      return count;
    }


    size_t skipFilesData(const std::string & /* sectionName */, std::string & name, D2ResEntry & entry)
    {
      size_t count = 0;
      count += readUntil(std::back_inserter(name), boost::bind(isSameSymbol, '\0', _1), true);

      boost::uint8_t countBuff[sizeof(boost::uint32_t)];
      count += readUntil(countBuff, ReadCount(sizeof(boost::uint32_t)), false);

      /// @warning little endian issue
      boost::uint32_t size = *reinterpret_cast<boost::uint32_t *>(countBuff);

      empty_container<boost::uint8_t> data;

      entry.offset = mOffset;
      entry.size = readUntil(std::back_inserter(data), ReadCount(size), false);

      count += entry.size;
      return count;
    }


    size_t skipNonFilesData(const std::string & sectionName, std::string & name, D2ResEntry & entry)
    {
      empty_container<char> data;

      entry.offset = mOffset;
      entry.size = readUntil(std::back_inserter(data), boost::bind(isSameSymbol, '\0', _1), true);

      /**
       * When we read non-files data, section name is actual filename
       */
      name = sectionName;

      return entry.size;
    }


    void throwError(const Ogre::String & msg, const Ogre::String & where)
    {
      OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                  msg + ", offset: " + boost::lexical_cast<std::string>(mOffset),
                  where);
    }


    size_t mOffset;
    data_iterator mBegin;
    data_iterator mEnd;

    ParserDispatcher mDispatcher;
  };
}



void readFileInfo(std::istream& stream, D2ResFileInfo& fileInfo)
{
  D2ResInfoWatcher watcher(stream);

  watcher.readResFile(fileInfo);
}


