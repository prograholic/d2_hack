#include "material_parser.h"

#include <d2_hack/common/platform.h>

D2_HACK_DISABLE_WARNING_BEGIN(4100)

#include <OgreResourceGroupManager.h>

D2_HACK_DISABLE_WARNING_END() // 4100

#include "res_file_info.h"

namespace d2_hack
{
namespace codec
{
namespace archive
{
namespace res
{

static std::vector<std::string> ParseTokens(const std::string& filename, Ogre::DataStream& stream)
{
    std::string content = stream.getAsString();
    std::vector<std::string> tokens = Ogre::StringUtil::split(content, " ");
    if (tokens.size() < 2)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid content of " + filename + ": " + content);
    }
    tokens.erase(tokens.begin()); // skip name of resource

    return tokens;
}

Ogre::DataStreamPtr ParseMaterial(const std::string& /* resId */, const std::string& filename, const Ogre::DataStreamPtr& stream)
{
    std::vector<std::string> tokens = ParseTokens(filename, *stream);

    if (tokens[0] == "col")
    {
#if 0
        {
            const std::string colorFile = GetColorFileName(resId, tokens[1]);
            ColorResourcePtr colorContent = D2ResourceManager::getSingleton().getByName(colorFile);

            float xxx = colorContent->GetXxx();
            if (xxx >= 0)
            {
                __debugbreak();
            }

            Ogre::ColourValue cv = colorContent->GetColor();
            cv.getAsRGBA();
        }
#endif //0
    }
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unknown tokens[0] of " + filename + ": " + tokens[0]);
    }

    return Ogre::DataStreamPtr();
}

} // namespace res
} // namespace archive
} // namespace codec
} // namespace d2_hack  
