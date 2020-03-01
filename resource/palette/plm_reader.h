#ifndef D2_HACK_RESOURCE_PALETTE_PALETTE_PARSER_H
#define D2_HACK_RESOURCE_PALETTE_PALETTE_PARSER_H

#include <d2_hack/common/reader.h>

D2_HACK_DISABLE_WARNING_BEGIN(4100)
#include <OgreColourValue.h>
D2_HACK_DISABLE_WARNING_END() //4100

namespace d2_hack
{
namespace resource
{
namespace palette
{




struct PlmFile
{
    std::vector<Ogre::ColourValue> palette;
};

class PlmReader: private common::Reader
{
public:
    explicit PlmReader(Ogre::DataStream& input);

    void Read(PlmFile& plmFile);

private:
    void ReadHeader();

    void ReadPalette(PlmFile& plmFile);
};


} // namespace palette
} // namespace resource
} // namespace d2_hack


#endif /* D2_HACK_RESOURCE_PALETTE_PALETTE_PARSER_H */
