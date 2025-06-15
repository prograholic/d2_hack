#ifndef D2_HACK_RESOURCE_ARCHIVE_RES_MATERIAL_PARSER_H
#define D2_HACK_RESOURCE_ARCHIVE_RES_MATERIAL_PARSER_H

#include <optional>

#include <OgreVector.h>
#include <OgreDataStream.h>

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

enum class MaterialType
{
    col,
    tex,
    ttx,
    itx
};

struct MaterialDescriptor
{
    MaterialType type;
    std::uint32_t index = 0;

    bool noz = false;
    std::optional<Ogre::Real> transparency;
    
    bool notile = false;
    std::optional<Ogre::Vector2> move;
    bool nof = false;
    std::optional<Ogre::Vector2> env;
    std::optional<std::uint32_t> coord;
    std::optional<std::uint32_t> att;
    bool wave = false;
    bool usecol = false;
    std::optional<Ogre::Vector2> rotPoint;
    std::optional<Ogre::Real> rot;
    std::optional<Ogre::Real> specular;
    std::optional<std::uint32_t> power;
    std::optional<std::uint32_t> par;
    std::optional<std::uint32_t> col;
    bool env3 = false;
};

typedef std::vector<MaterialDescriptor> MaterialDescriptorList;

MaterialDescriptor ParseMaterialDescriptor(const std::string_view& content);

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  

#endif // D2_HACK_RESOURCE_ARCHIVE_RES_MATERIAL_PARSER_H
