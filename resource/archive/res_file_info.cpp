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
    std::uint32_t count;
};

inline bool operator < (const Header& lhs, const Header& rhs)
{
    return lhs.name < rhs.name;
}

typedef std::map<std::string, common::Blob> DataDictionary;
typedef std::map<Header, DataDictionary> FileSystem;
typedef std::function<void(ResEntry& entry, std::uint32_t index)> ResourceWatcher;
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

        m_dispatcher[entries::Colors] = std::bind(&ResInfoWatcher::ParseColorsData, this, _1, _2);
        m_dispatcher[entries::Materials] = std::bind(&ResInfoWatcher::ParseMaterialsData, this, _1, _2);
        m_dispatcher[entries::Sounds] = std::bind(&ResInfoWatcher::SkipSoundsData, this, _1, _2);

        m_dispatcher[entries::TextureFiles] = std::bind(&ResInfoWatcher::SkipTextureFilesData, this, _1, _2);
        m_dispatcher[entries::PaletteFiles] = std::bind(&ResInfoWatcher::SkipPaletteFilesData, this, _1, _2);
        m_dispatcher[entries::BackFiles] = std::bind(&ResInfoWatcher::SkipBackFilesData, this, _1, _2);
        m_dispatcher[entries::MaskFiles] = std::bind(&ResInfoWatcher::SkipMaskFilesData, this, _1, _2);
        m_dispatcher[entries::SoundFiles] = std::bind(&ResInfoWatcher::SkipSoundFilesData, this, _1, _2);
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
                for (std::uint32_t i = 0; i != header.count; ++i)
                {
                    ResEntry entry;
                    parser->second(entry, i);
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


    void SkipTextureFilesData(ResEntry & entry, std::uint32_t index)
    {
        entry.name = common::GetTextureFileName(m_resId, index + 1);
        SkipFilesData(entry);
    }

    void SkipPaletteFilesData(ResEntry& entry, std::uint32_t /* index */)
    {
        SkipFilesData(entry);
        entry.name = common::GetPaletteFileName(m_resId, entry.resString.substr(0, entry.resString.find('.')));
    }

    void SkipBackFilesData(ResEntry& entry, std::uint32_t index)
    {
        entry.name = common::GetBackFileName(m_resId, index + 1);
        SkipFilesData(entry);
    }

    void SkipMaskFilesData(ResEntry& entry, std::uint32_t index)
    {
        entry.name = common::GetMaskFileName(m_resId, index + 1);
        SkipFilesData(entry);
    }

    void SkipSoundFilesData(ResEntry& entry, std::uint32_t index)
    {
        entry.name = common::GetSoundFileName(m_resId, index + 1);
        SkipFilesData(entry);
    }

    void SkipSoundsData(ResEntry& entry, std::uint32_t index)
    {
        entry.offset = GetStream().tell();
        entry.size = SkipLine('\0');

        entry.name = common::GetSoundFileName(m_resId, index + 1);
    }

    void ParseColorsData(ResEntry& entry, std::uint32_t index)
    {
        entry.offset = GetStream().tell();
        entry.size = SkipLine('\0');

        entry.name = common::GetColorName(m_resId, index + 1);
    }

    void ParseMaterialsData(ResEntry& entry, std::uint32_t /* index */)
    {
        entry.offset = GetStream().tell();
        auto materialId = ReadString('\0');
        entry.size = materialId.size();

        auto sep = materialId.find(' ');
        if (sep != materialId.npos)
        {
            materialId = materialId.substr(0, sep);
        }

        entry.name = common::GetMaterialFileName(m_resId, materialId);
    }


    void SkipFilesData(ResEntry& entry)
    {
        entry.resString = ReadString('\0');

        entry.size = ReadUint32();
        entry.offset = GetStream().tell();

        GetStream().seek(entry.offset + entry.size);
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

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  
