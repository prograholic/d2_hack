#include "d2_hack_common.h"

namespace file_io
{

void ThrowError(const Ogre::String & msg, const Ogre::String & where)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, msg, where);
}

Reader::Reader(std::istream &input)
    : m_offset(0)
    , m_begin(input)
    , m_end()
{
}

void Reader::ThrowError(const Ogre::String & msg, const Ogre::String & where)
{
    file_io::ThrowError(msg + ", offset: " + std::to_string(m_offset), where);
}

namespace helpers
{

ReadCountSeparator::ReadCountSeparator(size_t count)
    : m_count(count)
{
}

bool ReadCountSeparator::IsSeparator(char /* symbol */)
{
    return (m_count--) == 0;
}

} // namespace helpers
} // namespace file_io
