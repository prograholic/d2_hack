#include <iostream>
#include <stdexcept>

#include <d2_hack/codec/archive/res.h>


void ReadPalleteFromCommon()
{
    d2_hack::codec::archive::ResArchive archive{D2_ROOT_DIR "/COMMON/common.res", "test"};

    archive.load();
    Ogre::DataStreamPtr stream = archive.open("common.plm");
    if (!stream)
    {
        throw std::runtime_error("aa: res is NULL");
    }
}

int main()
{
    try
    {
        ReadPalleteFromCommon();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
