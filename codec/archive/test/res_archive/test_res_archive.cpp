#include <iostream>
#include <stdexcept>

#include <d2_hack/codec/archive/res.h>


void ReadTxrFromAa()
{
    d2_hack::codec::archive::ResArchive archive{D2_ROOT_DIR "/ENV/aa.res", "test"};

    archive.load();
    Ogre::StringVectorPtr res = archive.find("*.txr");
    if (!res)
    {
        throw std::runtime_error("aa: res is NULL");
    }

    if (res->empty())
    {
        throw std::runtime_error("aa: res is empty");
    }
}

void ReadPalleteFromCommon()
{
    d2_hack::codec::archive::ResArchive archive{D2_ROOT_DIR "/COMMON/common.res", "test"};

    archive.load();
    Ogre::StringVectorPtr res = archive.find("*.plm");
    if (!res)
    {
        throw std::runtime_error("common: res is NULL");
    }

    if (res->empty())
    {
        throw std::runtime_error("common: res is empty");
    }
}

int main()
{
    try
    {
        ReadTxrFromAa();
        ReadPalleteFromCommon();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
