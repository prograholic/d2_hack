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



template <typename ContainerType, typename FaceType>
common::IndexList GetSimpleIndexList(const FaceType& face)
{
    auto data = std::get<ContainerType>(face.data);

    common::IndexList res;
    for (const auto& item : data)
    {
        res.push_back(item.index);
    }

    return res;
}


template <typename IndexEntry>
std::uint32_t GetIndex(const IndexEntry& entry)
{
    return entry.index;
}

static std::uint32_t GetIndex(std::uint32_t entry)
{
    return entry;
}



template <typename ContainerType, typename FaceType>
common::IndexList GetIndexListFromTriangleFan(const FaceType& face)
{
    auto data = std::get<ContainerType>(face.data);
    assert(data.size() >= 3);

    common::IndexList res;
    for (size_t i = 2; i != data.size(); ++i)
    {
        res.push_back(GetIndex(data[0]));
        res.push_back(GetIndex(data[i - 1]));
        res.push_back(GetIndex(data[i]));
    }

    return res;
}


template <typename ContainerType, typename FaceType>
common::IndexList GetIndexListFromTriangleStrip(const FaceType& face)
{
    auto data = std::get<ContainerType>(face.data);
    assert(data.size() >= 3);

    common::IndexList res;
    for (size_t i = 2; i != data.size(); ++i)
    {
        if ((i % 2) == 0)
        {
            res.push_back(GetIndex(data[i - 2]));
            res.push_back(GetIndex(data[i - 1]));
            res.push_back(GetIndex(data[i]));
        }
        else
        {
            res.push_back(GetIndex(data[i - 2]));
            res.push_back(GetIndex(data[i]));
            res.push_back(GetIndex(data[i - 1]));
        }
    }

    return res;
}

common::IndexList GetIndexListForFace28(const block_data::Face28& face)
{
    const auto& data = std::get<std::vector<block_data::Face28::Unknown>>(face.data);
    if (data.size() == 4)
    {
        return { 0, 2, 1, 3, 2, 0 };
    }
    else if (data.size() == 3)
    {
        return { 0, 2, 1 };
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
        return GetIndexListFromTriangleFan<common::IndexList>(face);

    case block_data::Face8::UnknownType1:
        return GetIndexListFromTriangleFan<common::IndexList>(face);

    case block_data::Face8::FaceIndexType2:
        return GetIndexListFromTriangleStrip<common::IndexWithTexCoordList>(face);

    case block_data::Face8::FaceIndexType3:
        return GetSimpleIndexList<common::IndexWithTexCoordList>(face);

    case block_data::Face8::UnknownType16:
        return GetIndexListFromTriangleFan<common::IndexList>(face);

    case block_data::Face8::FaceIndexType17:
        return GetIndexListFromTriangleStrip<common::IndexList>(face);

    case block_data::Face8::FaceIndexType48:
        return GetIndexListFromTriangleFan<common::IndexWithNormalList>(face);

    case block_data::Face8::FaceIndexType50:
        return GetIndexListFromTriangleFan<common::IndexWithTexCoordNormalList>(face);

    case block_data::Face8::FaceIndexType51:
        return GetIndexListFromTriangleStrip<common::IndexWithNormalList>(face);

    case block_data::Face8::FaceIndexType128:
        return GetIndexListFromTriangleStrip<common::IndexList>(face);

    case block_data::Face8::FaceIndexType129:
        return GetIndexListFromTriangleStrip<common::IndexList>(face);

    case block_data::Face8::FaceIndexType131:
        return GetIndexListFromTriangleStrip<common::IndexWithTexCoordList>(face);

    case block_data::Face8::UnknownType144:
        return GetIndexListFromTriangleStrip<common::IndexList>(face);

    case block_data::Face8::UnknownType145:
        return GetIndexListFromTriangleStrip<common::IndexList>(face);

    case block_data::Face8::FaceIndexType176:
        return GetIndexListFromTriangleFan<common::IndexWithNormalList>(face);

    case block_data::Face8::FaceIndexType177:
        return GetIndexListFromTriangleStrip<std::vector<block_data::Face8::Unknown177>>(face);

    case block_data::Face8::FaceIndexType178:
        return GetIndexListFromTriangleStrip<common::IndexWithTexCoordNormalList>(face);

    case block_data::Face8::FaceIndexType179:
        return GetIndexListFromTriangleFan<common::IndexWithNormalList>(face);

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
            return std::get<common::IndexList>(face.data);

        case block_data::Face35::Indices1:
            return std::get<common::IndexList>(face.data);

        case block_data::Face35::Indices3:
            //return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };
            B3D_INDEX_NOT_IMPLEMENTED();
            return {};

        case block_data::Face35::Indices16:
            return std::get<common::IndexList>(face.data);

        case block_data::Face35::Indices17:
            return std::get<common::IndexList>(face.data);

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
            return std::get<common::IndexList>(face.data);

        case block_data::Face35::UnknownType3:
            return GetSimpleIndexList<common::IndexWithTexCoordList>(face);

        case block_data::Face35::UnknownType49:
            return GetSimpleIndexList<std::vector<block_data::Face35::Unknown49>>(face);

        case block_data::Face35::UnknownType51:
            return GetSimpleIndexList<common::IndexWithNormalList>(face);

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
            return std::get<common::IndexList>(face.data);

        case block_data::Face35::Unknown2:
            return GetSimpleIndexList<common::IndexWithTexCoordList>(face);

        case block_data::Face35::UnknownType48:
            return GetSimpleIndexList<common::IndexWithNormalList>(face);

        case block_data::Face35::UnknownType50:
            return GetSimpleIndexList<common::IndexWithTexCoordNormalList>(face);

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
