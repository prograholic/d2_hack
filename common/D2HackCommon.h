#ifndef D2_HACK_COMMON_D2_HACK_COMMON_H
#define D2_HACK_COMMON_D2_HACK_COMMON_H


#include <OgreException.h>
#include <OgreString.h>
#include <OgreLogManager.h>


#ifndef OGRE_LOG
#define OGRE_LOG(x) Ogre::LogManager::getSingleton().stream() << #x << ": "
#endif /* OGRE_LOG */


#endif /* D2_HACK_COMMON_D2_HACK_COMMON_H */
