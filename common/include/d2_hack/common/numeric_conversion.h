#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_NUMERIC_CONVERSION_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_NUMERIC_CONVERSION_H

#include <d2_hack/common/platform.h>

#include <cstdint>
#include <iterator>

#include <boost/numeric/conversion/cast.hpp>

#include <OgreException.h>

namespace d2_hack
{
namespace common
{

namespace details
{
template <typename ResultT>
ResultT ToNumericUnsafe(const std::uint8_t* data)
{
    return *reinterpret_cast<const ResultT*>(data);
}

} //namespace details


template <typename ResultT, size_t ArraySize>
ResultT ToNumeric(const std::uint8_t(&data)[ArraySize])
{
    static_assert(ArraySize >= sizeof(ResultT), "Incorrect range for converting range to numeric value");
    return details::ToNumericUnsafe<ResultT>(data);
}

template <typename ResultT>
ResultT ToNumeric(const std::uint8_t* first, const std::uint8_t* last)
{
    if (std::distance(first, last) < sizeof(ResultT))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Incorrect range for converting range to numeric value");
    }
    return details::ToNumericUnsafe<ResultT>(first);
}


template <typename OutputResult, typename InputResult>
auto NumericCast(InputResult value)
{
    try
    {
        return boost::numeric_cast<OutputResult>(value);
    }
    catch (const boost::numeric::bad_numeric_cast& e)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, Ogre::String("Cannot convert numeric value: ") + e.what());
    }
}


} // namespace common
} // namespace d2_hack


#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_NUMERIC_CONVERSION_H */
