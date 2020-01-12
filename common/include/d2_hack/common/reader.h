#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_READER_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_READER_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <istream>
#include <iterator>

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
    
class SeparatorBase
{
public:
    SeparatorBase() = default;
    virtual ~SeparatorBase() = default;

    virtual bool IsSeparator(char symbol) = 0;

    virtual bool Skip() = 0;

    virtual bool MoreDataNeeded()
    {
        return true;
    }
    
private:
    SeparatorBase(const SeparatorBase&) = delete;
    SeparatorBase& operator=(const SeparatorBase& ) = delete;
};


namespace reader_helper
{

template <typename T>
struct EmptyContainer
{
    typedef typename T value_type;
    void push_back(const T&)
    {
    }
};

template <typename InnerSeparatorT, bool skipSeparator>
class SkipAdapter : public SeparatorBase
{
public:
    virtual bool Skip()
    {
        return skipSeparator;
    }

    virtual bool IsSeparator(char symbol)
    {
        return static_cast<InnerSeparatorT *>(this)->IsSeparator(symbol);
    }
};

template <char sepSymbol, bool skip>
class SymbolSeparatorBase : public SkipAdapter<SymbolSeparatorBase<sepSymbol, skip>, skip>
{
public:
    bool IsSeparator(char symbol)
    {
        return symbol == sepSymbol;
    }
};

class ReadCountSeparator : public SkipAdapter<ReadCountSeparator, false>
{
public:
    explicit ReadCountSeparator(size_t count);

    virtual bool IsSeparator(char symbol) override;

    virtual bool MoreDataNeeded() override;

private:
    size_t m_count;
};

} // namespace reader_helper




class Reader
{
public:

    typedef std::istreambuf_iterator<char> Iterator;

    explicit Reader(std::istream& input);

    D2_HACK_NORETURN void ThrowError(const Ogre::String& msg, const Ogre::String& where);

    template <typename OutputIteratorT>
    void ReadUntil(OutputIteratorT res, SeparatorBase& separator)
    {
        for ( ; ; )
        {
            if (!separator.MoreDataNeeded())
            {
                break;
            }
            if (m_begin == m_end)
            {
                ThrowError("unexpected end of file", "Reader::readUntil");
            }
            const char data = *m_begin;
            if (separator.IsSeparator(data))
            {
                if (separator.Skip())
                {
                    ++m_begin;
                    ++m_offset;
                }
                break;
            }

            *res = data;
            ++res;
            ++m_begin;
            ++m_offset;
        }
    }

    template <typename OutputIteratorT>
    void ReadUntil(OutputIteratorT res, size_t count)
    {
        reader_helper::ReadCountSeparator separator{ count };
        ReadUntil(res, separator);
    }

    template <typename IntegralT>
    IntegralT ReadInt()
    {
        static_assert(std::is_integral<IntegralT>::value, "Incorrect type");

        std::uint8_t buffer[sizeof(IntegralT)];

        ReadUntil(buffer, sizeof(IntegralT));
        return ToNumeric<IntegralT>(buffer);
    }

    inline std::uint32_t ReadUint32()
    {
        return ReadInt<std::uint32_t>();
    }

    inline float ReadFloat()
    {
        static_assert(sizeof(float) == 4, "Incorrect float type, shoult be 4 for D2");

        std::uint8_t buffer[sizeof(float)];

        ReadUntil(buffer, sizeof(float));
        return ToNumeric<float>(buffer);
    }

    Ogre::Vector3 ReadVector3()
    {
        Ogre::Vector3 res;

        res.x = ReadFloat();
        res.y = ReadFloat();
        res.z = ReadFloat();

        return res;
    }

    Ogre::Vector2 ReadVector2()
    {
        Ogre::Vector2 res;

        res.x = ReadFloat();
        res.y = ReadFloat();

        return res;
    }

    BoundingSphere ReadBoundingSphere()
    {
        BoundingSphere res;

        res.center = ReadVector3();
        res.radius = ReadFloat();

        return res;
    }

    Index ReadIndex()
    {
        return ReadUint32();
    }

    Position ReadPosition()
    {
        return ReadVector3();
    }

    TexCoord ReadTexCoord()
    {
        return ReadVector2();
    }

    Normal ReadNormal()
    {
        return ReadVector3();
    }

    PositionWithTexCoord ReadPositionWithTexCoord()
    {
        PositionWithTexCoord res;
        
        res.position = ReadPosition();
        res.texCoord = ReadTexCoord();

        return res;
    }

    PositionWithTexCoordNormal ReadPositionWithTexCoordNormal()
    {
        PositionWithTexCoordNormal res;

        res.position = ReadPosition();
        res.texCoord = ReadTexCoord();
        res.normal = ReadNormal();

        return res;
    }

    PositionWithNormal ReadPositionWithNormal()
    {
        PositionWithNormal res;

        res.position = ReadPosition();
        res.normal = ReadNormal();

        return res;
    }

    IndexWithTexCoord ReadIndexWithTexCoord()
    {
        IndexWithTexCoord res;

        res.index = ReadIndex();
        res.texCoord = ReadTexCoord();

        return res;
    }

    IndexWithPosition ReadIndexWithPosition()
    {
        IndexWithPosition res;

        res.index = ReadIndex();
        res.position = ReadPosition();

        return res;
    }

    IndexWithPositionTexCoord ReadIndexWithPositionTexCoord()
    {
        IndexWithPositionTexCoord res;

        res.index = ReadIndex();
        res.position = ReadPosition();
        res.texCoord = ReadTexCoord();

        return res;
    }

    Iterator Begin() const;

    Iterator End() const;

    size_t GetOffset() const;

private:
    Reader(const Reader& ) = delete;
    Reader& operator=(const Reader&) = delete;

    size_t m_offset;
    Iterator m_begin;
    Iterator m_end;
};



} // namespace common
} // namespace d2_hack


#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_READER_H */
