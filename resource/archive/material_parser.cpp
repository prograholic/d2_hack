#include "material_parser.h"

#include <d2_hack/common/platform.h>

#include <boost/format.hpp>

D2_HACK_DISABLE_WARNING_BEGIN(4100)

#include <OgreResourceGroupManager.h>

D2_HACK_DISABLE_WARNING_END() // 4100

#include <d2_hack/common/memory_mgmt.h>
#include <d2_hack/resource/manager/manager.h>

#include "res_file_info.h"

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

static const char MaterialTemplate [] = 
    "material %1%\n"
    "{\n"
    "    technique\n"
    "    {\n"
    "        pass\n""        {\n"
    "            lighting on\n"
    "            ambient 0.1 0.3 0.1 1\n"
    "            diffuse 0.2 0.2 0.2 1\n"
    "            emissive %2% %3% %4% %5%\n"
    "        }\n"
    "    }\n"
    "}\n";


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

Ogre::DataStreamPtr ParseMaterial(const std::string& resId, const std::string& filename, const Ogre::DataStreamPtr& stream)
{
    std::vector<std::string> tokens = ParseTokens(filename, *stream);

    if (tokens[0] == "col")
    {
        const std::string colorFile = GetColorFileName(resId, tokens[1]);
        color::ColorResourcePtr colorContent = manager::Manager::getSingleton().Create(colorFile, "D2");
        
        Ogre::ColourValue cv = colorContent->GetColor();
        cv.getAsRGBA();

        std::string materialContent = str(boost::format(MaterialTemplate) % filename % cv.r % cv.g % cv.b % cv.a);
        const std::size_t dataSize = materialContent.size();

        std::unique_ptr<char, common::ArrayDeleter<Ogre::MEMCATEGORY_GENERAL>> buffer{OGRE_ALLOC_T(char, dataSize, Ogre::MEMCATEGORY_GENERAL)};
        std::memcpy(buffer.get(), materialContent.c_str(), dataSize);

        Ogre::DataStreamPtr res = std::make_shared<Ogre::MemoryDataStream>(buffer.get(), dataSize, true, true);
        buffer.release();

        return res;
    }
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unknown tokens[0] of " + filename + ": " + tokens[0]);
    }
}

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  
