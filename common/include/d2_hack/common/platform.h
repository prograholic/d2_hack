#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_PLATFORM_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_PLATFORM_H

#include <OgrePlatform.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC

#   define D2_HACK_DISABLE_WARNING_BEGIN(warning_id) __pragma(warning(push)) __pragma(warning(disable : warning_id))
#   define D2_HACK_DISABLE_WARNING_END() __pragma(warning(pop))
#   define D2_HACK_NORETURN __declspec(noreturn)

#else /* OGRE_COMPILER */
#error Unsupported compiler
#endif /* OGRE_COMPILER */






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_PLATFORM_H */
