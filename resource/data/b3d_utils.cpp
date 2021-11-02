#include <d2_hack/resource/data/b3d_utils.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{

template <typename VertexListType>
VertexListType ExtractVertexFromTriangleFan(const VertexListType& data, const common::IndexList& indices)
{
    assert(indices.size() >= 3);

    VertexListType res;

    for (size_t i = 2; i != indices.size(); ++i)
    {
        res.push_back(data[indices[0]]);
        res.push_back(data[indices[i - 1]]);
        res.push_back(data[indices[i]]);
    }

    return res;
}

template <typename VertexListType>
VertexListType PrepareVertexFromTriangleFan(const VertexListType& parent, const VertexListType& current, const common::IndexList& indices)
{
    assert(indices.size() >= 3);
    const size_t extractedSize = (indices.size() - 2) * 3;

    if (!current.empty())
    {
        if (current.size() == extractedSize)
        {
            return current;
        }
        else if (current.size() == indices.size())
        {
            common::IndexList newIndices;
            for (std::uint32_t i = 0; i != indices.size(); ++i)
            {
                newIndices.push_back(i);
            }
            return ExtractVertexFromTriangleFan(current, newIndices);
        }
        else
        {
            assert(0 && "not implemented");
            return {};
        }
    }

    VertexListType res;
    if (!parent.empty())
    {
        res = ExtractVertexFromTriangleFan(parent, indices);
    }

    return res;
}

static common::SimpleMeshInfo PrepareStandaloneMeshInfoFromTriangleFan(const common::SimpleMeshInfo& parentMeshInfo, const common::SimpleMeshInfo& meshInfo)
{
    common::SimpleMeshInfo res;

    res.positions = PrepareVertexFromTriangleFan(parentMeshInfo.positions, meshInfo.positions, meshInfo.indices);
    res.texCoords = PrepareVertexFromTriangleFan(parentMeshInfo.texCoords, meshInfo.texCoords, meshInfo.indices);
    res.normals = PrepareVertexFromTriangleFan(parentMeshInfo.normals, meshInfo.normals, meshInfo.indices);

    return res;
}

////////////////////////////////////////////////////////////////////////////////


template <typename VertexListType>
VertexListType ExtractVertexFromTriangleStrip(const VertexListType& data, const common::IndexList& indices)
{
    assert(indices.size() >= 3);

    VertexListType res;

    for (size_t i = 2; i != indices.size(); ++i)
    {
        if ((i % 2) == 0)
        {
            res.push_back(data[indices[i - 2]]);
            res.push_back(data[indices[i - 1]]);
            res.push_back(data[indices[i]]);
        }
        else
        {
            res.push_back(data[indices[i - 2]]);
            res.push_back(data[indices[i]]);
            res.push_back(data[indices[i - 1]]);
        }
    }

    return res;
}


template <typename VertexListType>
VertexListType PrepareVertexFromTriangleStrip(const VertexListType& parent, const VertexListType& current, const common::IndexList& indices)
{
    assert(indices.size() >= 3);
    const size_t extractedSize = (indices.size() - 2) * 3;

    if (!current.empty())
    {
        if (current.size() == extractedSize)
        {
            return current;
        }
        else if (current.size() == indices.size())
        {
            common::IndexList newIndices;
            for (std::uint32_t i = 0; i != indices.size(); ++i)
            {
                newIndices.push_back(i);
            }
            return ExtractVertexFromTriangleStrip(current, newIndices);
        }
        else
        {
            assert(0 && "not implemented");
            return {};
        }
    }

    VertexListType res;
    if (!parent.empty())
    {
        res = ExtractVertexFromTriangleStrip(parent, indices);
    }

    return res;
}

static common::SimpleMeshInfo PrepareStandaloneMeshInfoFromTriangleStrip(const common::SimpleMeshInfo& parentMeshInfo, const common::SimpleMeshInfo& meshInfo)
{
    common::SimpleMeshInfo res;

    res.positions = PrepareVertexFromTriangleStrip(parentMeshInfo.positions, meshInfo.positions, meshInfo.indices);
    res.texCoords = PrepareVertexFromTriangleStrip(parentMeshInfo.texCoords, meshInfo.texCoords, meshInfo.indices);
    res.normals = PrepareVertexFromTriangleStrip(parentMeshInfo.normals, meshInfo.normals, meshInfo.indices);

    return res;
}

////////////////////////////////////////////////////////////////////////////////

template <typename VertexListType>
VertexListType PrepareVertexFromTriangleList(const VertexListType& parent, const VertexListType& current, const common::IndexList& indices, bool extractCurrent = false)
{
    if (!current.empty())
    {
        if (!extractCurrent)
        {
            assert(current.size() == indices.size());
            return current;
        }
        else
        {
            VertexListType res;

            for (auto index : indices)
            {
                res.push_back(current[index]);
            }

            return res;
        }
    }

    VertexListType res;
    if (!parent.empty())
    {
        for (auto index: indices)
        {
            res.push_back(parent[index]);
        }
    }

    return res;
}

common::SimpleMeshInfo PrepareStandaloneMeshInfoFromTriangleList(const common::SimpleMeshInfo& parentMeshInfo, const common::SimpleMeshInfo& meshInfo)
{
    common::SimpleMeshInfo res;

    res.positions = PrepareVertexFromTriangleList(parentMeshInfo.positions, meshInfo.positions, meshInfo.indices);
    res.texCoords = PrepareVertexFromTriangleList(parentMeshInfo.texCoords, meshInfo.texCoords, meshInfo.indices);
    res.normals = PrepareVertexFromTriangleList(parentMeshInfo.normals, meshInfo.normals, meshInfo.indices);

    return res;
}

////////////////////////////////////////////////////////////////////////////////

common::SimpleMeshInfo PrepareStandaloneMeshInfoForFace28(const common::SimpleMeshInfo& parentMeshInfo, const common::SimpleMeshInfo& meshInfo)
{
    common::IndexList indices;
    if (!meshInfo.texCoords.empty())
    {
        if (meshInfo.texCoords.size() == 4)
        {
            indices = { 0, 2, 1, 3, 2, 0 };
        }
        else if (meshInfo.texCoords.size() == 3)
        {
            indices = { 0, 2, 1 };
        }
    }
    assert(!indices.empty() && "not implemented");

    common::SimpleMeshInfo res;

    res.positions = PrepareVertexFromTriangleList(parentMeshInfo.positions, meshInfo.positions, indices, true);
    res.texCoords = PrepareVertexFromTriangleList(parentMeshInfo.texCoords, meshInfo.texCoords, indices, true);
    res.normals = PrepareVertexFromTriangleList(parentMeshInfo.normals, meshInfo.normals, indices, true);

    return res;
}

////////////////////////////////////////////////////////////////////////////////

#define B3D_INDEX_NOT_IMPLEMENTED() assert(0 && "not implemented");

common::SimpleMeshInfo PrepareStandaloneMeshInfo(const block_data::SimpleFaces8& /* block */, const block_data::Face8& face, const common::SimpleMeshInfo& parentMeshInfo)
{
    switch (face.type)
    {
    case block_data::Face8::UnknownType0:
        return PrepareStandaloneMeshInfoFromTriangleFan(parentMeshInfo, face.meshInfo);

    case block_data::Face8::UnknownType1:
        return PrepareStandaloneMeshInfoFromTriangleFan(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType2:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType3:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::UnknownType16:
        return PrepareStandaloneMeshInfoFromTriangleFan(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType17:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType48:
        return PrepareStandaloneMeshInfoFromTriangleFan(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType50:
        return PrepareStandaloneMeshInfoFromTriangleFan(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType51:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType128:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType129:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType131:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::UnknownType144:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::UnknownType145:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType176:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType177:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType178:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    case block_data::Face8::FaceIndexType179:
        return PrepareStandaloneMeshInfoFromTriangleStrip(parentMeshInfo, face.meshInfo);

    default:
        assert(0 && "not implemented");
        return {};
    };
}

common::SimpleMeshInfo PrepareStandaloneMeshInfo(const block_data::SimpleFaces28& /* block */, const block_data::Face28& face, const common::SimpleMeshInfo& parentMeshInfo)
{
    switch (face.type)
    {
    case block_data::Face28::Unknown2:
        return PrepareStandaloneMeshInfoForFace28(parentMeshInfo, face.meshInfo);

    case block_data::Face28::Unknown_minus256:
        return PrepareStandaloneMeshInfoForFace28(parentMeshInfo, face.meshInfo);

    default:
        assert(0 && "not implemented");
        return {};
    }
}

common::SimpleMeshInfo PrepareStandaloneMeshInfo(const block_data::SimpleFaces35& block, const block_data::Face35& face, const common::SimpleMeshInfo& parentMeshInfo)
{
    if (block.type == block_data::SimpleFaces35::IndicesOnly3)
    {
        switch (face.type)
        {
        case block_data::Face35::Indices0:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::Indices1:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::Indices3:
            //return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };
            B3D_INDEX_NOT_IMPLEMENTED();
            return {};

        case block_data::Face35::Indices16:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::Indices17:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

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
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::UnknownType3:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::UnknownType49:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::UnknownType51:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

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
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::Unknown2:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::UnknownType48:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

        case block_data::Face35::UnknownType50:
            return PrepareStandaloneMeshInfoFromTriangleList(parentMeshInfo, face.meshInfo);

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
