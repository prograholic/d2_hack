#include "material_parser.h"

#include <d2_hack/common/platform.h>
#include <d2_hack/common/types.h>

#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

D2_HACK_DISABLE_WARNING_BEGIN(4100)

#include <OgreResourceGroupManager.h>

D2_HACK_DISABLE_WARNING_END() // 4100

#include <d2_hack/common/memory_mgmt.h>
#include <d2_hack/resource/manager/manager.h>

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
    "        pass\n"
    "        {\n"
    "            lighting on\n"
    "            ambient 0.1 0.3 0.1 1\n"
    "            diffuse 0.2 0.2 0.2 1\n"
    "            emissive %2% %3% %4% %5%\n"
    "            depth_check %6%\n"
    "        }\n"
    "    }\n"
    "}\n";


static const char TextureMaterialTemplate[] =
    "material %1%\n"
    "{\n"
    "    technique\n"
    "    {\n"
    "        pass\n"
    "        {\n"
    "            texture_unit\n"
    "            {\n"
    "                texture %2%\n"
    "            }\n"
    "            depth_check %3%\n"
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

Ogre::DataStreamPtr ParseColor(std::list<std::string>& tokens, const std::string& filename, const std::string& comment)
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

    palette::PalettePtr plm = manager::Manager::getSingleton().Load("COMMON\\common.plm", common::DefaultResourceGroup);

    Ogre::ColourValue cv = plm->GetColor(colorIndex);
    std::string materialContent = str(boost::format(ColorMaterialTemplate) % filename.substr(0, filename.size() - 9) % cv.r % cv.g % cv.b % alpha % depthCheck);
    //D2_HACK_LOG(MaterialParser) << "content: " << materialContent;

    materialContent += comment;

    const std::size_t dataSize = materialContent.size();

    std::unique_ptr<char, common::ArrayDeleter<Ogre::MEMCATEGORY_GENERAL>> buffer{ OGRE_ALLOC_T(char, dataSize, Ogre::MEMCATEGORY_GENERAL) };
    std::memcpy(buffer.get(), materialContent.c_str(), dataSize);

    Ogre::DataStreamPtr res = std::make_shared<Ogre::MemoryDataStream>(buffer.get(), dataSize, true, true);
    buffer.release();

    return res;
}

std::string LookupTextureByIndex(const ResFileInfo& fileInfo, std::uint32_t textureIndex)
{
    for (const auto& resEntry : fileInfo.info)
    {
        if (resEntry.name.find("txr\\") != std::string::npos)
        {
            if (textureIndex == resEntry.index)
            {
                return resEntry.name;
            }
        }
    }

    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Cannot find texture for index: " + std::to_string(textureIndex));
}

Ogre::DataStreamPtr ParseTexture(const ResFileInfo& fileInfo, std::list<std::string>& tokens, const std::string& filename, const std::string& comment)
{
    const std::uint32_t textureIndex = std::stoul(tokens.front()) - 1;
    tokens.pop_front();
    
    std::string depthCheck = "on";

    while (!tokens.empty())
    {
        if (tokens.front() == "noz")
        {
            tokens.pop_front();
            depthCheck = "off";
        }
        else if (tokens.front() == "notile")
        {
            tokens.pop_front();
            //assert(0 && "process `notile` option");
        }
        else if (tokens.front() == "move")
        {
            tokens.pop_front();
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `move` option");
        }
        else if (tokens.front() == "nof")
        {
            tokens.pop_front();
            //assert(0 && "process `nof` option");
        }
        else if (tokens.front() == "col")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `col` option");
        }
        else if (tokens.front() == "env")
        {
            tokens.pop_front();
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `env` option");
        }
        else if (tokens.front() == "coord")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `coord` option");
        }
        else if (tokens.front() == "att")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `att` option");
        }
        else if (tokens.front() == "\"wave\"")
        {
            tokens.pop_front();
            //assert(0 && "process `\"wave\"` option");
        }
        else if (tokens.front() == "usecol")
        {
            tokens.pop_front();
            //assert(0 && "process `usecol` option");
        }
        else if (tokens.front() == "RotPoint")
        {
            tokens.pop_front();
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `RotPoint` option");
        }
        else if (tokens.front() == "rot")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `rot` option");
        }
        else if (tokens.front() == "specular")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `specular` option");
        }
        else if (tokens.front() == "power")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `power` option");
        }
        else if (tokens.front() == "par")
        {
            tokens.pop_front();
            tokens.pop_front();
            //assert(0 && "process `par` option");
        }
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Got unprocessed token: " + tokens.front() + ", " + filename);
        }
    }

    
    const std::string textureFilename = LookupTextureByIndex(fileInfo, textureIndex);

    std::string materialContent = str(boost::format(TextureMaterialTemplate) % filename.substr(0, filename.size() - 9) % textureFilename % depthCheck);
    //D2_HACK_LOG(MaterialParser) << "content: \n" << materialContent;

    materialContent += comment;
    
    const std::size_t dataSize = materialContent.size();

    std::unique_ptr<char, common::ArrayDeleter<Ogre::MEMCATEGORY_GENERAL>> buffer{ OGRE_ALLOC_T(char, dataSize, Ogre::MEMCATEGORY_GENERAL) };
    std::memcpy(buffer.get(), materialContent.c_str(), dataSize);

    Ogre::DataStreamPtr res = std::make_shared<Ogre::MemoryDataStream>(buffer.get(), dataSize, true, true);
    buffer.release();

    return res;
}


Ogre::DataStreamPtr ParseMaterial(const ResFileInfo& fileInfo, const std::string& filename, const Ogre::DataStreamPtr& stream)
{
    std::list<std::string> tokens = ParseTokens(filename, *stream);

    std::string comment = "// " + boost::algorithm::join(tokens, " ") + "\n";

    const std::string materialType = tokens.front();
    tokens.pop_front();

    if (materialType == "col")
    {
        return ParseColor(tokens, filename, comment);
    }
    else if ((materialType == "ttx") || (materialType == "tex") || (materialType == "itx"))
    {
        return ParseTexture(fileInfo, tokens, filename, comment);
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
