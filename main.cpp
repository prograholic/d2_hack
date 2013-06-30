#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>


typedef std::istreambuf_iterator<char> data_iterator;

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



template <typename InputIteratorT, typename OutputIteratorT, typename SeparatorFunctionT>
bool readUntil(InputIteratorT & begin, InputIteratorT end, OutputIteratorT res, SeparatorFunctionT separator, bool skipSeparator)
{
  for ( ; ; )
  {
    if (begin == end)
    {
      /// @todo emit some error (unexpected end of file)
      return false;
    }
    const char data = *begin;
    if (separator(data))
    {
      if (skipSeparator)
      {
        ++begin;
      }
      break;
    }

    *res = data;
    ++res;
    ++begin;
  }

  return true;
}

bool readHeader(data_iterator & begin, data_iterator end, Header & header)
{
  if (!readUntil(begin, end, std::back_inserter(header.name), boost::bind(isSameSymbol, ' ', _1), true))
  {
    return false;
  }

  std::string entitiesCount;
  if (!readUntil(begin, end, std::back_inserter(entitiesCount), boost::bind(isSameSymbol, '\0', _1), true))
  {
    return false;
  }

  header.count = boost::lexical_cast<size_t>(entitiesCount);
  return true;
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

bool readFilesData(data_iterator & begin, data_iterator end, std::string & name, blob_t & data)
{
  if (!readUntil(begin, end, std::back_inserter(name), boost::bind(isSameSymbol, '\0', _1), true))
  {
    return false;
  }

  boost::uint8_t countBuff[sizeof(boost::uint32_t)];

  if (!readUntil(begin, end, countBuff, ReadCount(sizeof(boost::uint32_t)), false))
  {
    return false;
  }

  /// @warning little endian issue
  boost::uint32_t size = *reinterpret_cast<boost::uint32_t *>(countBuff);

  data.reserve(size);

  return readUntil(begin, end, std::back_inserter(data), ReadCount(size), false);
}


bool readNonFilesData(data_iterator & begin, data_iterator end, std::string & name, blob_t & data)
{
  return readUntil(begin, end, std::back_inserter(name), boost::bind(isSameSymbol, '\0', _1), true);
}



typedef boost::function<bool (data_iterator & begin, data_iterator end, std::string & name, blob_t & data)> resource_parser_t;

typedef std::map<std::string, resource_parser_t> ParserDispatcher;


bool readResFile(const std::string & resFile, const ParserDispatcher & disp, FileSystem & fs)
{
  std::ifstream iFile(resFile.c_str(), std::ios_base::binary);

  data_iterator begin(iFile);
  data_iterator end;

  while (begin != end)
  {
    Header header;
    if (!readHeader(begin, end, header))
    {
      return false;
    }

    DataDictionary & dict = fs[header];

    ParserDispatcher::const_iterator parser = disp.find(header.name);
    if (parser != disp.end())
    {
      for (size_t i = 0; i != header.count; ++i)
      {
        blob_t data;
        std::string dataName;
        if (!parser->second(begin, end, dataName, data))
        {
          return false;
        }

        dict[dataName] = data;
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}








int main()
{
  FileSystem fs;

  ParserDispatcher disp;
  disp["COLORS"] = readNonFilesData;
  disp["MATERIALS"] = readNonFilesData;

  disp["TEXTUREFILES"] = readFilesData;
  disp["PALETTEFILES"] = readFilesData;
  disp["BACKFILES"] = readFilesData;
  disp["MASKFILES"] = readFilesData;
  disp["SOUNDFILES"] = readFilesData;

  std::string name = "/home/pixel/.wine/drive_c/Program Files/King/ENV/af.res";

  bool status = readResFile(name, disp, fs);
  std::cout << "read file: " << status << std::endl;

  if (status)
  {
    for (FileSystem::const_iterator it = fs.begin(); it != fs.end(); ++it)
    {
      std::cout << "name : " << it->first.name << std::endl;
      for (DataDictionary::const_iterator dataIt = it->second.begin(); dataIt != it->second.end(); ++dataIt)
      {
        std::cout << "  name: " << dataIt->first << ", size: " << dataIt->second.size() << std::endl;
      }
    }
  }



  return 0;
}
