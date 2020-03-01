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

static const char ColorMaterialTemplate [] = 
    "material %1%\n"
    "{\n"
    "    technique\n"
    "    {\n"
    "        pass\n""        {\n"
    "            lighting on\n"
    "            ambient 0.1 0.3 0.1 1\n"
    "            diffuse 0.2 0.2 0.2 1\n"
    "            emissive %2% %3% %4% %5%\n"
    "            depth_check %6%"
    "        }\n"
    "    }\n"
    "}\n";


static std::list<std::string> ParseTokens(const std::string& filename, Ogre::DataStream& stream)
{
    std::string content = stream.getAsString();
    std::vector<std::string> tokens = Ogre::StringUtil::split(content, " ");
    if (tokens.size() < 2)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Invalid content of " + filename + ": " + content);
    }
    // skip name of resource
    return std::list<std::string>{tokens.begin() + 1, tokens.end()};
}

Ogre::DataStreamPtr ParseColor(std::list<std::string>& tokens, const std::string& filename)
{
    if (tokens.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Tokens are empty for color resource: " + filename);
    }

    const size_t colorIndex = std::stoi(tokens.front());
    tokens.pop_front();

    float alpha = 1.0f;
    std::string depthCheck = "on";

    while (!tokens.empty())
    {
        if (tokens.front() == "transp")
        {
            tokens.pop_front();
            alpha = std::stof(tokens.front());
            tokens.pop_front();
        }
        else if (tokens.front() == "noz")
        {
            tokens.pop_front();
            depthCheck = "off";
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Got unprocessed token: " + tokens.front() + ", " + filename);
        }        
    }

    palette::PalettePtr plm = manager::Manager::getSingleton().Load("common.plm", "D2");

    Ogre::ColourValue cv = plm->GetColor(colorIndex);
    std::string materialContent = str(boost::format(ColorMaterialTemplate) % filename % cv.r % cv.g % cv.b % alpha % depthCheck);
    const std::size_t dataSize = materialContent.size();

    std::unique_ptr<char, common::ArrayDeleter<Ogre::MEMCATEGORY_GENERAL>> buffer{ OGRE_ALLOC_T(char, dataSize, Ogre::MEMCATEGORY_GENERAL) };
    std::memcpy(buffer.get(), materialContent.c_str(), dataSize);

    Ogre::DataStreamPtr res = std::make_shared<Ogre::MemoryDataStream>(buffer.get(), dataSize, true, true);
    buffer.release();

    return res;
}

Ogre::DataStreamPtr ParseTexture(std::list<std::string>& /* tokens */, const std::string& /* filename */)
{
    return Ogre::DataStreamPtr();
}


Ogre::DataStreamPtr ParseMaterial(const std::string& /* resId */, const std::string& filename, const Ogre::DataStreamPtr& stream)
{
    std::list<std::string> tokens = ParseTokens(filename, *stream);

    const std::string materialType = tokens.front();
    tokens.pop_front();

    if (materialType == "col")
    {
        return ParseColor(tokens, filename);
    }
    else if ((materialType == "ttx") || (materialType == "tex") || (materialType == "itx"))
    {
        return ParseTexture(tokens, filename);
    }
    else
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unknown material type of " + filename + ": " + materialType);
    }
}

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  
