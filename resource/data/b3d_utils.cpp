#include <d2_hack/resource/data/b3d_utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{


Ogre::RenderOperation::OperationType GetRenderOperation(const block_data::SimpleFaces8& /* block */, const block_data::Face8& face)
{
    switch (face.type)
    {
    case block_data::Face8::UnknownType0:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::UnknownType1:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::FaceIndexType2:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType3:
        return Ogre::RenderOperation::OT_TRIANGLE_LIST;

    case block_data::Face8::UnknownType16:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::FaceIndexType17:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType48:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::FaceIndexType50:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::FaceIndexType51:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType128:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType129:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::FaceIndexType131:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::UnknownType144:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::UnknownType145:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType176:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    case block_data::Face8::FaceIndexType177:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType178:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    case block_data::Face8::FaceIndexType179:
        return Ogre::RenderOperation::OT_TRIANGLE_FAN;

    default:
        assert(0 && "not implemented");
        return Ogre::RenderOperation::OT_POINT_LIST;
    };
}

Ogre::RenderOperation::OperationType GetRenderOperation(const block_data::SimpleFaces28& /* block */, const block_data::Face28& face)
{
    switch (face.type)
    {
    case block_data::Face28::Unknown2:
        return Ogre::RenderOperation::OT_TRIANGLE_STRIP;

    default:
        assert(0 && "not implemented");
        return Ogre::RenderOperation::OT_POINT_LIST;
    }
}

Ogre::RenderOperation::OperationType GetRenderOperation(const block_data::SimpleFaces35& block, const block_data::Face35& face)
{
    if (block.type == block_data::SimpleFaces35::IndicesOnly3)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::Indices1:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::Indices3:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::Indices16:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::Indices17:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        default:
            assert(0 && "not implemented");
            return Ogre::RenderOperation::OT_POINT_LIST;
        };
    }
    else if (block.type == block_data::SimpleFaces35::Unknown2)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices1:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::UnknownType3:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::UnknownType49:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::UnknownType51:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        default:
            assert(0 && "not implemented");
            return Ogre::RenderOperation::OT_POINT_LIST;
        };
    }
    else if (block.type == block_data::SimpleFaces35::Unknown1)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::Unknown2:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::UnknownType48:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        case block_data::Face35::UnknownType50:
            return Ogre::RenderOperation::OT_TRIANGLE_LIST;

        default:
            assert(0 && "not implemented");
            return Ogre::RenderOperation::OT_POINT_LIST;
        };
    }
    else
    {
        assert(0 && "not implemented");
        return Ogre::RenderOperation::OT_POINT_LIST;
    }
}


static common::IndexList GetIndexListFromTriangleFan(const common::IndexList& data)
{
    assert(data.size() >= 3);

    common::IndexList res;
    for (size_t i = 2; i != data.size(); ++i)
    {
        res.push_back(data[0]);
        res.push_back(data[i - 1]);
        res.push_back(data[i]);
    }

    return res;
}



static common::IndexList GetIndexListFromTriangleStrip(const common::IndexList& data)
{
    assert(data.size() >= 3);

    common::IndexList res;
    for (size_t i = 2; i != data.size(); ++i)
    {
        if ((i % 2) == 0)
        {
            res.push_back(data[i - 2]);
            res.push_back(data[i - 1]);
            res.push_back(data[i]);
        }
        else
        {
            res.push_back(data[i - 2]);
            res.push_back(data[i]);
            res.push_back(data[i - 1]);
        }
    }

    return res;
}

common::IndexList GetIndexListForFace28(const block_data::Face28& face)
{
    if (face.meshInfo.texCoords.has_value())
    {
        const auto& data = *(face.meshInfo.texCoords);

        if (data.size() == 4)
        {
            return { 0, 2, 1, 3, 2, 0 };
        }
        else if (data.size() == 3)
        {
            return { 0, 2, 1 };
        }
    }

    assert(0 && "not implemented");
    return {};
}


#define B3D_INDEX_NOT_IMPLEMENTED() assert(0 && "not implemented");

common::IndexList PrepareIndices(const block_data::SimpleFaces8& /* block */, const block_data::Face8& face)
{
    switch (face.type)
    {
    case block_data::Face8::UnknownType0:
        return GetIndexListFromTriangleFan(*face.meshInfo.indices);

    case block_data::Face8::UnknownType1:
        return GetIndexListFromTriangleFan(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType2:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType3:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::UnknownType16:
        return GetIndexListFromTriangleFan(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType17:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType48:
        return GetIndexListFromTriangleFan(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType50:
        return GetIndexListFromTriangleFan(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType51:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType128:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType129:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType131:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::UnknownType144:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::UnknownType145:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType176:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType177:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType178:
        return GetIndexListFromTriangleStrip(*face.meshInfo.indices);

    case block_data::Face8::FaceIndexType179:
        return GetIndexListFromTriangleFan(*face.meshInfo.indices);

    default:
        assert(0 && "not implemented");
        return {};
    };
}

common::IndexList PrepareIndices(const block_data::SimpleFaces28& /* block */, const block_data::Face28& face)
{
    switch (face.type)
    {
    case block_data::Face28::Unknown2:
        return GetIndexListForFace28(face);

    default:
        assert(0 && "not implemented");
        return {};
    }
}

common::IndexList PrepareIndices(const block_data::SimpleFaces35& block, const block_data::Face35& face)
{
    if (block.type == block_data::SimpleFaces35::IndicesOnly3)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return *face.meshInfo.indices;

        case block_data::Face35::Indices1:
            return *face.meshInfo.indices;

        case block_data::Face35::Indices3:
            //return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };
            B3D_INDEX_NOT_IMPLEMENTED();
            return {};

        case block_data::Face35::Indices16:
            return *face.meshInfo.indices;

        case block_data::Face35::Indices17:
            return *face.meshInfo.indices;

        default:
            assert(0 && "not implemented");
            return {};
        };
    }
    else if (block.type == block_data::SimpleFaces35::Unknown2)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices1:
            return *face.meshInfo.indices;

        case block_data::Face35::UnknownType3:
            return *face.meshInfo.indices;

        case block_data::Face35::UnknownType49:
            return *face.meshInfo.indices;

        case block_data::Face35::UnknownType51:
            return *face.meshInfo.indices;

        default:
            assert(0 && "not implemented");
            return {};
        };
    }
    else if (block.type == block_data::SimpleFaces35::Unknown1)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return *face.meshInfo.indices;

        case block_data::Face35::Unknown2:
            return *face.meshInfo.indices;

        case block_data::Face35::UnknownType48:
            return *face.meshInfo.indices;

        case block_data::Face35::UnknownType50:
            return *face.meshInfo.indices;

        default:
            assert(0 && "not implemented");
            return {};
        };
    }
    else
    {
        assert(0 && "not implemented");
        return {};
    }
}

} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
