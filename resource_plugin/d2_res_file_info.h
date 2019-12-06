#ifndef D2_HACK_RESOURCE_PLUGIN_D2_RES_FILE_INFO_H
#define D2_HACK_RESOURCE_PLUGIN_D2_RES_FILE_INFO_H

#include <istream>

#include <OgreString.h>

struct D2ResEntry
{
    size_t offset;
    size_t size;
};

typedef std::map<Ogre::String, D2ResEntry> D2ResFileInfo;

void readFileInfo(std::istream& stream, D2ResFileInfo& fileInfo);




#endif // D2_HACK_RESOURCE_PLUGIN_D2_RES_FILE_INFO_H
