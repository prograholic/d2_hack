#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_LOG_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_LOG_H

#include <d2_hack/common/platform.h>

#include <OgreLogManager.h>

#define D2_HACK_LOG(x) Ogre::LogManager::getSingleton().stream() << #x << ": "

#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_LOG_H */
