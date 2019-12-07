#ifndef D2_HACK_COMMON_D2_HACK_PLATFORM_SUPPORT_H
#define D2_HACK_COMMON_D2_HACK_PLATFORM_SUPPORT_H

#include <OgrePlatform.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
#   define D2_HACK_DISABLE_WARNING_BEGIN(warning_id) __pragma(warning(push)) __pragma(warning(disable : warning_id))
#   define D2_HACK_DISABLE_WARNING_END() __pragma(warning(pop))
#else /* OGRE_COMPILER */
#error Unsupported compiler
#endif /* OGRE_COMPILER */



#endif /* D2_HACK_COMMON_D2_HACK_PLATFORM_SUPPORT_H */
