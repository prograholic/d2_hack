#include <d2_hack/resource/data/b3d_utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

std::uint32_t Hash(const SubMeshInfo& subMeshInfo)
{
    std::uint32_t res = subMeshInfo.useSharedVertices ? 1 : 0;

    res += (subMeshInfo.materialIndex * 2);

    res += (subMeshInfo.operationType * 1024);

    return res;
}

bool operator < (const SubMeshInfo& left, const SubMeshInfo& right)
{
    return Hash(left) < Hash(right);
}


static const SubMeshInfo InvalidSubMeshInfo{ false, std::numeric_limits<std::uint32_t>::max(), Ogre::RenderOperation::OT_POINT_LIST };

SubMeshInfo GetFaceMapping(const block_data::SimpleFaces8& /* block */, const block_data::Face8& face)
{
    switch (face.type)
    {
    case block_data::Face8::UnknownType0:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::UnknownType1:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::FaceIndexType2:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType3:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

    case block_data::Face8::UnknownType16:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::FaceIndexType17:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType48:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::FaceIndexType50:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::FaceIndexType51:
        return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType128:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType129:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::FaceIndexType131:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::UnknownType144:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::UnknownType145:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType176:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::FaceIndexType177:
        return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType178:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType179:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    default:
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    };
}

SubMeshInfo GetFaceMapping(const block_data::SimpleFaces28& /* block */, const block_data::Face28& face)
{
    switch (face.type)
    {
    case block_data::Face28::Unknown2:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    default:
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    }
}

SubMeshInfo GetFaceMapping(const block_data::SimpleFaces35& block, const block_data::Face35& face)
{
    if (block.type == block_data::SimpleFaces35::IndicesOnly3)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::Indices1:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::Indices3:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::Indices16:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::Indices17:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        default:
            assert(0 && "not implemented");
            return InvalidSubMeshInfo;
        };
    }
    else if (block.type == block_data::SimpleFaces35::Unknown2)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices1:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::UnknownType3:
            return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::UnknownType49:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::UnknownType51:
            return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        default:
            assert(0 && "not implemented");
            return InvalidSubMeshInfo;
        };
    }
    else if (block.type == block_data::SimpleFaces35::Unknown1)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::Unknown2:
            return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::UnknownType48:
            return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        case block_data::Face35::UnknownType50:
            return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

        default:
            assert(0 && "not implemented");
            return InvalidSubMeshInfo;
        };
    }
    else
    {
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    }
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
