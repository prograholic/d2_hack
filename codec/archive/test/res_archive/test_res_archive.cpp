#include <d2_hack/codec/archive/res.h>

int main()
{
    d2_hack::codec::archive::ResArchive archive{D2_AA_RES_ARCHIVE_NAME, "test"};
    
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
