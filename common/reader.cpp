#include <d2_hack/common/reader.h>


namespace d2_hack
{
namespace common
{

Reader::Reader(Ogre::DataStream& input)
    : m_input(input)
{
}

void Reader::ThrowError(const std::string_view& msg, const char* where)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, std::format("{}, offset: {}", msg, m_input.tell()), where);
}

void Reader::ReadBytes(void* buffer, size_t size)
{
    size_t actualSize = m_input.read(buffer, size);
    if (actualSize != size)
    {
        ThrowError(std::format("Failed to read, expected {}, got {}", size, actualSize), "B3dReaderImpl::ReadCount");
    }
}

Ogre::DataStream& Reader::GetStream()
{
    return m_input;
}

std::string Reader::ReadString(char sep, bool skipSeparator)
{
    std::string res;

    char s;
    while (m_input.read(&s, 1) > 0)
    {
        if (s == sep)
        {
            if (!skipSeparator)
            {
                m_input.seek(m_input.tell() - 1);
            }
            break;
        }
        res.append(1, s);
    }

    return res;
}

size_t Reader::SkipLine(char sep, bool skipSeparator)
{
    size_t res = 0;

    char s;
    while (m_input.read(&s, 1) > 0)
    {
        if (s == sep)
        {
            if (!skipSeparator)
            {
                m_input.seek(m_input.tell() - 1);
            }
            break;
        }
        res += 1;
    }

    return res;
}

std::uint32_t Reader::ReadUint32()
{
    return ReadInt<std::uint32_t>();
}

float Reader::ReadFloat()
{
    static_assert(sizeof(float) == 4, "Incorrect float type, shoult be 4 for D2");

    std::uint8_t buffer[sizeof(float)];

    ReadBytes(buffer, sizeof(float));
    return common::ToNumeric<float>(buffer);
}

Ogre::Vector4 Reader::ReadVector4()
{
    Ogre::Vector4 res;

    res.x = ReadFloat();
    res.y = ReadFloat();
    res.z = ReadFloat();
    res.w = ReadFloat();

    return res;
}

Ogre::Vector3 Reader::ReadVector3()
{
    Ogre::Vector3 res;

    res.x = ReadFloat();
    res.y = ReadFloat();
    res.z = ReadFloat();

    return res;
}

Ogre::Vector2 Reader::ReadVector2()
{
    Ogre::Vector2 res;

    res.x = ReadFloat();
    res.y = ReadFloat();

    return res;
}

common::BoundingSphere Reader::ReadBoundingSphere()
{
    common::BoundingSphere res;

    res.center = ReadVector3();
    res.radius = ReadFloat();

    return res;
}
} // namespace common
} // namespace d2_hack
