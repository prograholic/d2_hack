#include "d2_res_file_info.h"

#include <functional>

#include "d2_hack_common.h"

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

    typedef std::map<std::string, file_io::blob_t> DataDictionary;
    typedef std::map<Header, DataDictionary> FileSystem;
    typedef std::function<bool (const std::string & sectionName, std::string & name, D2ResEntry & entry)> ResourceWatcher;
    typedef std::map<std::string, ResourceWatcher> ParserDispatcher;



    class D2ResInfoWatcher : public file_io::Reader
    {
    public:

        explicit D2ResInfoWatcher(std::istream& stream)
            : file_io::Reader(stream)
            , m_dispatcher()
        {
            using namespace std::placeholders;
            
            m_dispatcher["COLORS"] = std::bind(&D2ResInfoWatcher::SkipNonFilesData, this, _1, _2, _3);
            m_dispatcher["MATERIALS"] = std::bind(&D2ResInfoWatcher::SkipNonFilesData, this, _1, _2, _3);

            m_dispatcher["TEXTUREFILES"] = std::bind(&D2ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
            m_dispatcher["PALETTEFILES"] = std::bind(&D2ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
            m_dispatcher["BACKFILES"] = std::bind(&D2ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
            m_dispatcher["MASKFILES"] = std::bind(&D2ResInfoWatcher::SkipFilesData, this, _1, _2, _3);
            m_dispatcher["SOUNDFILES"] = std::bind(&D2ResInfoWatcher::SkipFilesData, this, _1, _2, _3);

        }


        size_t ReadResFile(D2ResFileInfo& fileInfo)
        {
            size_t count = 0;
            while (Begin() != End())
            {
                Header header;
                count += ReadHeader(header);

                ParserDispatcher::const_iterator parser = m_dispatcher.find(header.name);
                if (parser != m_dispatcher.end())
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
                    ThrowError("cannot find readed for entry [" + header.name + "]", "D2ResInfoWatcher::readResFile");
                }
            }

            return count;
        }



        size_t ReadHeader(Header & header)
        {
            size_t count = 0;

            file_io::helpers::SymbolSeparatorBase<' ', true> spaceSep;

            count += ReadUntil(std::back_inserter(header.name), spaceSep);

            std::string entitiesCount;
            file_io::helpers::SymbolSeparatorBase<'\0', true> zeroSep;
            count += ReadUntil(std::back_inserter(entitiesCount), zeroSep);

            header.count = std::stoul(entitiesCount);
            return count;
        }


        size_t SkipFilesData(const std::string & /* sectionName */, std::string & name, D2ResEntry & entry)
        {
            size_t count = 0;
            file_io::helpers::SymbolSeparatorBase<'\0', true> zeroSep;
            count += ReadUntil(std::back_inserter(name), zeroSep);

            std::uint8_t countBuff[sizeof(std::uint32_t)];
            count += file_io::helpers::ReadUntil(*this, countBuff, sizeof(countBuff));

            /// @warning little endian issue
            std::uint32_t size = file_io::ToNumeric<std::uint32_t>(countBuff);

            file_io::helpers::empty_container<std::uint8_t> skipData;

            entry.offset = GetOffset();
            entry.size = file_io::helpers::ReadUntil(*this, std::back_inserter(skipData), size);

            count += entry.size;
            return count;
        }


        size_t SkipNonFilesData(const std::string& sectionName, std::string& name, D2ResEntry& entry)
        {
            file_io::helpers::empty_container<char> skipData;

            entry.offset = GetOffset();
            file_io::helpers::SymbolSeparatorBase<'\0', true> zeroSep;
            entry.size = ReadUntil(std::back_inserter(skipData), zeroSep);

            /**
             * When we read non-files data, section name is actual filename
             */
            name = sectionName;

            return entry.size;
        }

        ParserDispatcher m_dispatcher;
    };
}



void readFileInfo(std::istream& stream, D2ResFileInfo& fileInfo)
{
    D2ResInfoWatcher watcher(stream);

    watcher.ReadResFile(fileInfo);
}



