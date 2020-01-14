#include "res_file_info.h"

#include <functional>

#include <d2_hack/common/types.h>
#include <d2_hack/common/reader.h>
#include <d2_hack/common/numeric_conversion.h>
#include <d2_hack/common/resource_mgmt.h>

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{
namespace entries
{

const char Colors[] = "COLORS";
const char Materials[] = "MATERIALS";
const char Sounds[] = "SOUNDS";
const char TextureFiles[] = "TEXTUREFILES";
const char PaletteFiles[] = "PALETTEFILES";
const char BackFiles[] = "BACKFILES";
const char MaskFiles[] = "MASKFILES";
const char SoundFiles[] = "SOUNDFILES";

} // namespace entries

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
typedef std::function<void(const std::string & sectionName, ResEntry& entry, size_t index)> ResourceWatcher;
typedef std::map<std::string, ResourceWatcher> ParserDispatcher;



class ResInfoWatcher : public common::Reader
{
public:

    ResInfoWatcher(const std::string& resId, Ogre::DataStream& stream)
        : common::Reader(stream)
        , m_dispatcher()
        , m_resId(resId)
    {
        using namespace std::placeholders;

        m_dispatcher[entries::Colors] = std::bind(&ResInfoWatcher::ParseColorsData, this, _1, _2, _3);
        m_dispatcher[entries::Materials] = std::bind(&ResInfoWatcher::ParseMaterialsData, this, _1, _2, _3);
        m_dispatcher[entries::Sounds] = std::bind(&ResInfoWatcher::SkipNonFilesData, this, _1, _2, _3);

        m_dispatcher[entries::TextureFiles] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher[entries::PaletteFiles] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher[entries::BackFiles] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher[entries::MaskFiles] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
        m_dispatcher[entries::SoundFiles] = std::bind(&ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
    }


    void ReadResFile(ResFileInfo& fileInfo)
    {
        while (GetStream().tell() < GetStream().size())
        {
            Header header;
            ReadHeader(header);

            ParserDispatcher::const_iterator parser = m_dispatcher.find(header.name);
            if (parser != m_dispatcher.end())
            {
                for (size_t i = 0; i != header.count; ++i)
                {
                    ResEntry entry;
                    parser->second(header.name, entry, i);
                    fileInfo.info.push_back(entry);
                }
            }
            else
            {
                ThrowError("cannot find reader for entry [" + header.name + "]", "D2ResInfoWatcher::readResFile");
            }
        }
    }



    void ReadHeader(Header& header)
    {
        header.name = ReadString(' ');

        std::string entitiesCount = ReadString('\0');
        header.count = std::stoul(entitiesCount);
    }


    void SkipFilesData(const std::string & /* sectionName */, ResEntry & entry, size_t /* index */)
    {
        entry.type = EntryType::File;
        entry.name = ReadString('\0');

        auto sep = entry.name.find(' ');
        if (sep != entry.name.npos)
        {
            entry.name = entry.name.substr(0, sep);
        }

        entry.size = ReadUint32();
        entry.offset = GetStream().tell();

        GetStream().seek(entry.offset + entry.size);
    }


    void SkipNonFilesData(const std::string& sectionName, ResEntry& entry, size_t index)
    {
        entry.offset = GetStream().tell();
        entry.size = SkipLine('\0');

        entry.name = sectionName + "_" + std::to_string(index) + ".d2resinfo";
    }

    void ParseColorsData(const std::string& /* sectionName */, ResEntry& entry, size_t index)
    {
        entry.type = EntryType::Color;
        entry.offset = GetStream().tell();
        entry.size = SkipLine('\0');

        entry.name = GetColorFileName(m_resId, std::to_string(index));
    }

    void ParseMaterialsData(const std::string& /* sectionName */, ResEntry& entry, size_t /* index */)
    {
        entry.type = EntryType::Material;
        entry.offset = GetStream().tell();
        entry.name = ReadString('\0');
        entry.size = entry.name.size();

        auto sep = entry.name.find(' ');
        if (sep != entry.name.npos)
        {
            entry.name = entry.name.substr(0, sep);
        }

        entry.name = common::GetResourceName(m_resId, entry.name) + ".material";
    }

    ParserDispatcher m_dispatcher;
    const std::string m_resId;
};


} // namespace



void ReadFileInfo(const std::string& resId, Ogre::DataStream& stream, ResFileInfo& fileInfo)
{
    ResInfoWatcher watcher(resId, stream);

    watcher.ReadResFile(fileInfo);
}

std::string GetColorFileName(const std::string& resId, const std::string& colorId)
{
    return common::GetResourceName(resId, entries::Colors + colorId) + ".d2colorinfo";
}

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  
