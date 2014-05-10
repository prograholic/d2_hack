#include "D2ResFileInfo.h"

#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>

#include "D2HackCommon.h"

namespace
{
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

  typedef std::map<std::string, file_io::blob_t> DataDictionary;
  typedef std::map<Header, DataDictionary> FileSystem;
  typedef boost::function<bool (const std::string & sectionName, std::string & name, D2ResEntry & entry)> resource_watcher_t;
  typedef std::map<std::string, resource_watcher_t> ParserDispatcher;



  class D2ResInfoWatcher : public file_io::Reader
  {
  public:

    explicit D2ResInfoWatcher(std::istream & stream)
      : file_io::Reader(stream)
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



    size_t readHeader(Header & header)
    {
      size_t count = 0;

      count += readUntil(std::back_inserter(header.name), boost::bind(file_io::helpers::isSameSymbol, ' ', _1), true);

      std::string entitiesCount;
      count += readUntil(std::back_inserter(entitiesCount), boost::bind(file_io::helpers::isSameSymbol, '\0', _1), true);

      header.count = boost::lexical_cast<size_t>(entitiesCount);
      return count;
    }


    size_t skipFilesData(const std::string & /* sectionName */, std::string & name, D2ResEntry & entry)
    {
      size_t count = 0;
      count += readUntil(std::back_inserter(name), boost::bind(file_io::helpers::isSameSymbol, '\0', _1), true);

      boost::uint8_t countBuff[sizeof(boost::uint32_t)];
      count += readUntil(countBuff, file_io::helpers::ReadCount(sizeof(boost::uint32_t)), false);

      /// @warning little endian issue
      boost::uint32_t size = *reinterpret_cast<boost::uint32_t *>(countBuff);

      file_io::helpers::empty_container<boost::uint8_t> skipData;

      entry.offset = mOffset;
      entry.size = readUntil(std::back_inserter(skipData), file_io::helpers::ReadCount(size), false);

      count += entry.size;
      return count;
    }


    size_t skipNonFilesData(const std::string & sectionName, std::string & name, D2ResEntry & entry)
    {
      file_io::helpers::empty_container<char> skipData;

      entry.offset = mOffset;
      entry.size = readUntil(std::back_inserter(skipData), boost::bind(file_io::helpers::isSameSymbol, '\0', _1), true);

      /**
       * When we read non-files data, section name is actual filename
       */
      name = sectionName;

      return entry.size;
    }

    ParserDispatcher mDispatcher;
  };
}



void readFileInfo(std::istream& stream, D2ResFileInfo& fileInfo)
{
  D2ResInfoWatcher watcher(stream);

  watcher.readResFile(fileInfo);
}



