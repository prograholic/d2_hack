#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_READER_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_READER_H

#include <d2_hack/common/platform.h>

#include <vector>

#include <OgreDataStream.h>

D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreVector2.h>

D2_HACK_DISABLE_WARNING_END() //4251

#include <d2_hack/common/numeric_conversion.h>
#include <d2_hack/common/types.h>

namespace d2_hack
{

namespace common
{

class Reader
{
public:
    explicit Reader(Ogre::DataStream& input);

    void ThrowError(const Ogre::String & msg, const Ogre::String & where);

    void ReadBytes(void* buffer, size_t size);

    template <typename T, typename A>
    void ReadCount(std::vector<T, A>& data, size_t count)
    {
        data.resize(count);
        ReadBytes(data.data(), count * sizeof(T));
    }

    Ogre::DataStream& GetStream();

    template <typename IntegralT>
    IntegralT ReadInt()
    {
        static_assert(std::is_integral<IntegralT>::value, "Incorrect type");

        std::uint8_t buffer[sizeof(IntegralT)];

        ReadBytes(buffer, sizeof(IntegralT));
        return common::ToNumeric<IntegralT>(buffer);
    }

    std::string ReadString(char sep, bool skipSeparator = true);

    size_t SkipLine(char sep, bool skipSeparator = true);

    std::uint32_t ReadUint32();

    float ReadFloat();

    Ogre::Vector3 ReadVector3();

    Ogre::Vector2 ReadVector2();

    common::BoundingSphere ReadBoundingSphere();

private:
    Ogre::DataStream& m_input;
};



} // namespace common
} // namespace d2_hack


#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_READER_H */
