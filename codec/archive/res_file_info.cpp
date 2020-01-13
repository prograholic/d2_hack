#include "res_file_info.h"

#include <functional>

#include <d2_hack/common/types.h>
#include <d2_hack/common/reader.h>
#include <d2_hack/common/numeric_conversion.h>

namespace d2_hack
{
namespace codec
{
namespace archive
{

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

inline bool operator < (const Header& lhs, const Header& rhs)
{
    return lhs.name < rhs.name;
}

typedef std::map<std::string, common::Blob> DataDictionary;
typedef std::map<Header, DataDictionary> FileSystem;
typedef std::function<void (const std::string & sectionName, std::string & name, ResEntry & entry)> ResourceWatcher;
typedef std::map<std::string, ResourceWatcher> ParserDispatcher;



class ResInfoWatcher : public common::Reader
{
public:

    explicit ResInfoWatcher(std::istream& stream)
        : common::Reader(stream)
        , m_dispatcher()
    {
        using namespace std::placeholders;
        
        m_dispatcher["COLORS"] = std::bind(&ResInfoWatcher::SkipNonFilesData, this, _1, _2, _3);
        m_dispatcher["MATERIALS"] = std::bind(&ResInfoWatcher::SkipNonFilesData, this, _1, _2, _3);

        m_dispatcher["TEXTUREFILES"] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher["PALETTEFILES"] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher["BACKFILES"] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher["MASKFILES"] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher["SOUNDFILES"] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
    }


    void ReadResFile(ResFileInfo& fileInfo)
    {
        while (Begin() != End())
        {
            Header header;
            ReadHeader(header);

            ParserDispatcher::const_iterator parser = m_dispatcher.find(header.name);
            if (parser != m_dispatcher.end())
            {
                for (size_t i = 0; i != header.count; ++i)
                {
                    ResEntry entry;
                    std::string dataName;

                    parser->second(header.name, dataName, entry);
                    fileInfo.info[dataName] = entry;
                }
            }
            else
            {
                ThrowError("cannot find readed for entry [" + header.name + "]", "D2ResInfoWatcher::readResFile");
            }
        }
    }



    void ReadHeader(Header & header)
    {
        common::reader_helper::SymbolSeparatorBase<' ', true> spaceSep;

        ReadUntil(std::back_inserter(header.name), spaceSep);

        std::string entitiesCount;
        common::reader_helper::SymbolSeparatorBase<'\0', true> zeroSep;
        ReadUntil(std::back_inserter(entitiesCount), zeroSep);

        header.count = std::stoul(entitiesCount);
    }


    void SkipFilesData(const std::string & /* sectionName */, std::string & name, ResEntry & entry)
    {
        common::reader_helper::SymbolSeparatorBase<'\0', true> zeroSep;
        ReadUntil(std::back_inserter(name), zeroSep);

        auto sep = name.find(' ');
        if (sep != name.npos)
        {
            name = name.substr(0, sep);
        }

        const std::uint32_t size = ReadUint32();

        common::reader_helper::EmptyContainer<std::uint8_t> skipData;

        entry.offset = GetOffset();
        ReadUntil(std::back_inserter(skipData), size);
        entry.size = size;
    }


    void SkipNonFilesData(const std::string& sectionName, std::string& name, ResEntry& entry)
    {
        common::reader_helper::EmptyContainer<char> skipData;

        entry.offset = GetOffset();
        common::reader_helper::SymbolSeparatorBase<'\0', true> zeroSep;
        ReadUntil(std::back_inserter(skipData), zeroSep);
        entry.size = GetOffset() - entry.offset;

        /**
         * When we read non-files data, section name is actual filename
         */
        name = sectionName;
    }

    ParserDispatcher m_dispatcher;
};


} // namespace



void ReadFileInfo(std::istream& stream, ResFileInfo& fileInfo)
{
    ResInfoWatcher watcher(stream);

    watcher.ReadResFile(fileInfo);
}

} //namespace archive
} // namespace codec
} // namespace d2_hack  
