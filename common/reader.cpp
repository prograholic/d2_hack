#include <d2_hack/common/reader.h>


namespace d2_hack
{
namespace common
{

Reader::Reader(std::istream &input)
    : m_offset(0)
    , m_begin(input)
    , m_end()
{
}

void Reader::ThrowError(const Ogre::String & msg, const Ogre::String & where)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, msg + ", offset: " + std::to_string(m_offset), where);
}

Reader::Iterator Reader::Begin() const
{
    return m_begin;
}

Reader::Iterator Reader::End() const
{
    return m_end;
}

size_t Reader::GetOffset() const
{
    return m_offset;
}

namespace reader_helper
{

ReadCountSeparator::ReadCountSeparator(size_t count)
    : m_count(count)
{
}

bool ReadCountSeparator::IsSeparator(char /* symbol */)
{
    return (m_count--) == 0;
}

bool ReadCountSeparator::MoreDataNeeded()
{
    return m_count > 0;
}

} // namespace reader_helper

} // namespace common
} // namespace d2_hack
