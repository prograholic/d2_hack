#include "d2_res_archive.h"

int main()
{
    D2ResArchive archive{D2_AA_RES_ARCHIVE_NAME, "test"};
    
    archive.load();
    Ogre::StringVectorPtr res = archive.find("*.txr");
    if (!res)
    {
        throw "res is NULL";
    }

    if (res->empty())
    {
        throw "res is empty";
    }
}
