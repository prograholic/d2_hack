#include "b3d_tree_visitor.h"

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>

#include <d2_hack/common/utils.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;

static const SubMeshInfo InvalidSubMeshInfo{ false, 0, Ogre::RenderOperation::OT_POINT_LIST };

static SubMeshInfo GetFaceMapping(const block_data::SimpleFaces8& /* block */, const block_data::Face8& face)
{
    switch (face.type)
    {
    case block_data::Face8::UnknownType0:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN };

    case block_data::Face8::UnknownType1:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType2:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType3:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::UnknownType16:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType48:
        return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType50:
        return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST };

    case block_data::Face8::FaceIndexType128:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType129:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::UnknownType144:
        return { true, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType176:
        return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    case block_data::Face8::FaceIndexType178:
        return { false, face.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP };

    default:
        assert(0 && "not implemented");
        return InvalidSubMeshInfo;
    };
}

static SubMeshInfo GetFaceMapping(const block_data::SimpleFaces28& /* block */, const block_data::Face28& face)
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

static SubMeshInfo GetFaceMapping(const block_data::SimpleFaces35& block, const block_data::Face35& face)
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


B3dTreeVisitor::B3dTreeVisitor(const char* b3dId,
                               const std::string& b3dName,
                               Ogre::SceneManager* sceneManager,
                               Ogre::SceneNode* rootNode,
                               Ogre::MeshManager* meshManager,
                               const common::Materials& materials)
    : B3dSceneBuilder(b3dId, b3dName, sceneManager, rootNode, meshManager, materials)
{
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::Empty0& /* block */, VisitMode /* visitMode */)
{
    // no need to implement
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupRoadInfraObjects4& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupObjects5& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupVertex7& block, VisitMode visitMode)
{
    Ogre::MeshPtr mesh = ProcessMesh(name, visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        AddVertexData(mesh, block.vertices);
    }
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::SimpleFaces8& block, VisitMode visitMode)
{
    VisitFaces(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupTrigger9& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupLodParameters10& block, VisitMode visitMode)
{
    ProcessLod(block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupUnknown12& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleTrigger13& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleUnknown14& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleObjectConnector18& block, VisitMode visitMode)
{
    ProcessObjectConnector(block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupObjects19& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleFlatCollision20& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupObjects21& /* block */, VisitMode visitMode)
{
    ProcessSceneNode(name, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleVolumeCollision23& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupTransformMatrix24& block, VisitMode visitMode)
{
    ProcessTransformQueue(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::SimpleFaces28& block, VisitMode visitMode)
{
    VisitFaces(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::GroupUnknown29& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimplePortal30& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupLightingObjects33& block, VisitMode visitMode)
{
    ProcessLight(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::SimpleFaces35& block, VisitMode visitMode)
{
    VisitFaces(name, block, visitMode);
}

void B3dTreeVisitor::Visit(const std::string& name, block_data::GroupVertexData37& block, VisitMode visitMode)
{
    Ogre::MeshPtr mesh = ProcessMesh(name, visitMode);
    if (visitMode == VisitMode::PreOrder)
    {
        auto visitor = [this, mesh](auto&& vertices)
        {
            AddVertexData(mesh, vertices);
        };

        std::visit(visitor, block.data);
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, block_data::SimpleGeneratedObjects40& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

template <typename Faces>
void B3dTreeVisitor::VisitFaces(const std::string& /* name */, Faces& block, resource::data::b3d::VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& face : block.faces)
        {
            Ogre::SubMesh* subMesh = CreateSubMesh(GetFaceMapping(block, face));

            auto visitor = [this, subMesh](auto&& items)
            {
                SetSubMeshData(subMesh, items);
            };
            std::visit(visitor, face.data);
        }
    }
}


namespace optimizations
{

class FacesVisitor : public NoOpNodeVisitor
{
public:
    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces8& /* block */, VisitMode /* visitMode */) override
    {

    }

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces28& /* block */, VisitMode /* visitMode */) override
    {

    }

    virtual void Visit(const std::string& /* name */, block_data::SimpleFaces35& block, VisitMode /* visitMode */) override
    {
        block_data::Face35List origFaces = std::move(block.faces);

        if (origFaces.empty())
        {
            return;
        }

        std::map<SubMeshInfo, block_data::Face35List> mapping;

        for (const auto& data : origFaces)
        {
            SubMeshInfo subMeshInfo = GetFaceMapping(block, data);
            mapping[subMeshInfo].push_back(data);
        }

        for (const auto& item : mapping)
        {
            if (CanMerge(item.first))
            {
                block_data::Face35List merged = Merge(item.second);
                block.faces.insert(block.faces.end(), merged.begin(), merged.end());
            }
            else
            {
                block.faces.insert(block.faces.end(), item.second.begin(), item.second.end());
            }
        }
    }


private:

    static bool CanMerge(const SubMeshInfo& subMeshInfo)
    {
        if (subMeshInfo.useSharedVertices && (subMeshInfo.operationType == Ogre::RenderOperation::OT_TRIANGLE_LIST))
        {
            return true;
        }

        return false;
    }

    template <typename MeshType>
    struct MeshHolder
    {
        typename MeshType::Types::tuple_t meshData;


        template <typename DataType>
        void Add(const DataType& data)
        {
            auto& holder = std::get<DataType>(meshData);
            holder.insert(holder.end(), data.begin(), data.end());
        }
    };

    template <typename MeshList>
    static MeshList Merge(const MeshList& meshList)
    {
        MeshHolder<MeshList::value_type> holder;

        auto visitor = [&holder](auto&& data)
        {
            holder.Add(data);
        };

        for (const auto& mesh: meshList)
        {
            std::visit(visitor, mesh.data);
        }

        MeshList res;

        auto visitor2 = [&meshList, &res](auto&& arg)
        {
            if (!arg.empty())
            {
                MeshList::value_type mesh = meshList[0];
                mesh.data = arg;
                res.push_back(mesh);
            }
        };

        boost::fusion::for_each(holder.meshData, visitor2);

        return res;
    }
};

void RemoveEmptyNodes(resource::data::b3d::B3dTree& /* tree */)
{
    // not implemented
}


void VisitNode(const NodePtr& node, FacesVisitor& visitor)
{
    node->Visit(visitor, VisitMode::PreOrder);

    const auto& children = node->GetChildNodeList();
    for (auto child : children)
    {
        VisitNode(child, visitor);
    }
}

void MergeFaces(resource::data::b3d::B3dTree& tree)
{
    FacesVisitor facesVisitor;

    for (const auto& node : tree.rootNodes)
    {
        VisitNode(node, facesVisitor);
    }
}


} // namespace optimizations
} // namespace app
} // namespace d2_hack
