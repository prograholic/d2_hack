#include "b3d_reader.h"

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cctype>

using namespace file_io::helpers;


#if defined B3D_TRACING
#    define TRACE_ARG(impl) impl, m_output
#    define TRACE_DATA(data, op, dumpType) \
    do \
    { \
        size_t offset = m_offset; \
        op; \
        Dump(outStream, #data, data, offset, dumpType);\
    } \
    while(0)

#else /* B3D_TRACING */
#    define TRACE_ARG(impl) impl
#    define TRACE_DATA(data, op, dumpType) op
#endif /* B3D_TRACING */


B3dReader::B3dReader(std::istream &input, std::ostream &output)
    : file_io::Reader(input)
    , m_output(output)
{
}

void B3dReader::Read()
{
    detail::b3d_file impl;
    Read(TRACE_ARG(impl));
}

struct String32Finder : public file_io::helpers::SkipAdapter<String32Finder, false>
{
    String32Finder(bool& s, size_t minLen = 0)
        : succeeded(s)
        , remainingBytes(32)
        , remainingStringLen(minLen)
    {
        succeeded = true;
    }

    bool isSeparator(char symbol)
    {
        if (remainingBytes == 0)
        {
            return true;
        }

        assert(remainingBytes > 0);
        --remainingBytes;

        if (symbol == 0)
        {
            /// string must start from printable character, or must have at least minLen length
            if ((remainingBytes == 31) || (remainingStringLen > 0))
            {
                succeeded = false;
            }
            return false;
        }

        if (std::isalnum(symbol) || (symbol == '_'))
        {
            if (succeeded)
            {
                if (remainingStringLen > 0)
                {
                    --remainingStringLen;
                }
            }
            /**
             * if state is failed, then we must stop at first printable symbol
             * If state is succeeded, then we must consume symbol
             */
            return !succeeded;
        }
        else
        {
            succeeded = false;
        }
        return false;
    }

    bool& succeeded;
    size_t remainingBytes;
    size_t remainingStringLen;
};

void B3dReader::Read(detail::b3d_file& b3d
#if defined(B3D_TRACING)
                     , std::ostream& outStream
#endif /* B3D_TRACING */
                    )
{
    file_io::helpers::ReadCount signatureSep(4);
    TRACE_DATA(b3d.signature, ReadUntil(b3d.signature, signatureSep), dump_type::String);

    file_io::helpers::ReadCount unknown_1Sep(20);
    TRACE_DATA(b3d.unknown_1, ReadUntil(b3d.unknown_1, unknown_1Sep), dump_type::Data);

    std::uint8_t countBuff[sizeof(std::uint32_t)];
    file_io::helpers::ReadCount sizeSep(4);
    TRACE_DATA(countBuff, ReadUntil(countBuff, sizeSep), dump_type::Data);

    b3d.res_name_count = *reinterpret_cast<std::uint32_t *>(countBuff);
    b3d.res_names.resize(b3d.res_name_count);

    for (std::uint32_t resNo = 0; resNo != b3d.res_name_count; ++resNo)
    {
        file_io::helpers::ReadCount string32Sep(32);
        TRACE_DATA(b3d.res_names[resNo], ReadUntil(b3d.res_names[resNo].begin(), string32Sep), dump_type::String);
    }


    size_t pos = m_offset;

    std::vector<std::uint8_t> betweenStrings;
    size_t lastValidString32Pos = m_offset;

    try
    {
        for ( ; ; )
        {
            std::uint8_t string32Buff[32];
            bool stringFounded = true;
            String32Finder finder(stringFounded);

            size_t offset = m_offset;


            const size_t dmpOffset = 1024 * 1024;
            if ((offset - pos) > dmpOffset)
            {
                pos = offset;
                std::cerr << "processing offset " << offset << " ..." << std::endl;
            }

            size_t readed = ReadUntil(string32Buff, finder);

            if (stringFounded)
            {
                if (!betweenStrings.empty())
                {
                    Dump(outStream, "data     ", betweenStrings, lastValidString32Pos, dump_type::Data);
                    betweenStrings.clear();
                }
                lastValidString32Pos = m_offset;
                Dump(outStream, "string_32", string32Buff, offset, dump_type::String);
            }
            else
            {
                if (lastValidString32Pos)
                {
                    betweenStrings.insert(betweenStrings.end(), string32Buff, string32Buff + readed);
                }
            }
        }
    }
    catch (const Ogre::Exception& e)
    {
        std::cerr << "ignoring error: " << e.what() << std::endl;
    }



}

template <typename TypeT>
void B3dReader::Dump(std::ostream& outStream,
                     const std::string& field,
                     const TypeT& data,
                     size_t offset,
                     file_io::helpers::dump_type::Value dumpType)
{
    size_t size = 0;
    outStream << "offset: " << std::hex << std::setfill('0') << std::setw(sizeof(offset) * 2) << offset << " ";
    outStream << field << "[";
    file_io::helpers::Dump(outStream, data, dumpType, &size);
    outStream << "]" << " size: " << std::dec << size << std::endl;
}
