#include "d2_hack_common.h"

namespace file_io
{
    Reader::Reader(std::istream &input)
        : m_offset(0)
        , m_begin(input)
        , m_end()
    {
    }

    void Reader::ThrowError(const Ogre::String & msg, const Ogre::String & where)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    msg + ", offset: " + std::to_string(m_offset),
                    where);
    }

    namespace helpers
    {

        ReadCount::ReadCount(size_t count)
                : m_count(count)
        {
        }

        bool ReadCount::IsSeparator(char /* symbol */)
        {
            return (m_count--) == 0;
        }
    }
}
