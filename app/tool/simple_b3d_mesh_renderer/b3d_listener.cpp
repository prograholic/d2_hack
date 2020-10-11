#include "b3d_listener.h"

D2_HACK_DISABLE_WARNING_BEGIN(4100)
D2_HACK_DISABLE_WARNING_BEGIN(4275)
D2_HACK_DISABLE_WARNING_BEGIN(4251)

#include <OgreHardwareBufferManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreEntity.h>

D2_HACK_DISABLE_WARNING_END() // 4251
D2_HACK_DISABLE_WARNING_END() // 4275
D2_HACK_DISABLE_WARNING_END() // 4100


#include <d2_hack/common/resource_mgmt.h>
#include <d2_hack/common/utils.h>
#include <d2_hack/common/log.h>

//#define B3D_NOT_IMPLEMENTED() D2_HACK_LOG("") << __FUNCSIG__ << ": NOT IMPLEMENTED"
#define B3D_NOT_IMPLEMENTED()


namespace d2_hack
{
namespace app
{

using namespace resource::data::b3d;



B3dTreeVisitor::B3dTreeVisitor(const char* b3dId,
                               const std::string& b3dName,
                               Ogre::SceneManager* sceneManager,
                               Ogre::SceneNode* rootNode,
                               Ogre::MeshManager* meshManager,
                               const common::Materials& materials)
    : m_b3dId(b3dId)
    , m_b3dName(b3dName)
    , m_sceneManager(sceneManager)
    , m_rootNode(rootNode)
    , m_meshManager(meshManager)
    , m_materials(materials)
{
}


B3dTreeVisitor::~B3dTreeVisitor()
{
    assert(m_meshStack.empty());
    assert(m_transformQueue.empty());
    assert(m_currentLods.empty());
    assert(m_sceneNodes.empty());
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::Empty0& /* block */, VisitMode /* visitMode */)
{
    // no need to implement
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupRoadInfraObjects4& /* block */, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        BeginSceneNode(name);
    }
    else
    {
        EndSceneNode();
    }
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupObjects5& /* block */, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        BeginSceneNode(name);
    }
    else
    {
        EndSceneNode();
    }
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupVertex7& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        Ogre::MeshPtr mesh = BeginMesh(name);
        AddVertexData(mesh, block.vertices);
    }
    else
    {
        EndMesh();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFaces8& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& data : block.faces)
        {
            Ogre::SubMesh* subMesh = nullptr;
            if (data.type == block_data::Face8::FaceIndexType129)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::FaceIndexType50)
            {
                subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
            }
            else if (data.type == block_data::Face8::FaceIndexType178)
            {
                subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::UnknownType144)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::UnknownType16)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::UnknownType1)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::FaceIndexType48)
            {
                subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::UnknownType0)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_FAN);
            }
            else if (data.type == block_data::Face8::FaceIndexType3)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::FaceIndexType176)
            {
                subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::FaceIndexType2)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else if (data.type == block_data::Face8::FaceIndexType128)
            {
                subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
            }
            else
            {
                assert(0 && "not implemented");
            }

            auto visitor = [this, subMesh](auto&& items)
            {
                SetSubMeshData(subMesh, items);
            };
            std::visit(visitor, data.data);
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupTrigger9& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupLodParameters10& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        m_currentLods.push(block);
    }
    else
    {
        m_currentLods.pop();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupUnknown12& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleTrigger13& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleUnknown14& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleObjectConnector18& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        std::string sceneName = GetNameImpl(common::ResourceNameToString(block.object), "scene_node", false);

        Ogre::SceneNode* sceneNode = m_sceneManager->getSceneNode(sceneName, false);
        if (sceneNode)
        {
            //auto transformList = m_transformMap[GetB3dResourceId(block.space)];
            //for (const auto& transform : transformList)
            //{
            //    sceneNode->translate(transform.matrix, transform.position);
            //}
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupObjects19& /* block */, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        BeginSceneNode(name);
    }
    else
    {
        EndSceneNode();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFlatCollision20& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupObjects21& /* block */, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        BeginSceneNode(name);
    }
    else
    {
        EndSceneNode();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleVolumeCollision23& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupTransformMatrix24& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        m_transformQueue.push_back(block);
    }
    else
    {
        TransformList transformList;
        for (const auto& item : m_transformQueue)
        {
            Transform transform;

            transform.matrix.FromAxes(item.x, item.y, item.z);
            transform.position = item.position;

            transformList.push_back(transform);
        }

        m_transformMap[name] = transformList;
        m_transformQueue.pop_back();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFaces28& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        for (const auto& data : block.facesEntries)
        {
            if (data.type == block_data::Face28Entry::Unknown2)
            {
                Ogre::SubMesh* subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_STRIP);
                SetSubMeshData(subMesh, data);
            }
            else
            {
                assert(0 && "not implemented");
            }
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::GroupUnknown29& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimplePortal30& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupLightingObjects33& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        BeginSceneNode(name);

        std::string full_name = GetNameImpl(name, "light", false);

        Ogre::Light* light = m_sceneManager->createLight(full_name);
        Ogre::SceneNode* lightNode = m_sceneManager->createSceneNode(full_name + "_scene_node");
        lightNode->attachObject(light);
        lightNode->setPosition(block.position);
        m_sceneNodes.top()->addChild(lightNode);

        // TODO: setup light
        B3D_NOT_IMPLEMENTED();
    }
    else
    {
        EndSceneNode();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleFaceData35& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        if (block.type == block_data::SimpleFaceData35::IndicesOnly3)
        {
            for (const auto& data : block.meshList)
            {
                Ogre::SubMesh* subMesh = nullptr;
                if (data.type == block_data::Mesh35::Indices16)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::Indices1)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::Indices3)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::Indices0)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::Indices17)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else
                {
                    assert(0 && "not implemented");
                }

                auto visitor = [this, subMesh](auto&& items)
                {
                    SetSubMeshData(subMesh, items);
                };
                std::visit(visitor, data.data);
            }
        }
        else if (block.type == block_data::SimpleFaceData35::Unknown2)
        {
            for (const auto& data : block.meshList)
            {
                Ogre::SubMesh* subMesh = nullptr;
                if (data.type == block_data::Mesh35::Indices1)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::UnknownType3)
                {
                    subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::UnknownType51)
                {
                    subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::UnknownType49)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else
                {
                    assert(0 && "not implemented");
                }

                auto visitor = [this, subMesh](auto&& items)
                {
                    SetSubMeshData(subMesh, items);
                };
                std::visit(visitor, data.data);
            }
        }
        else if (block.type == block_data::SimpleFaceData35::Unknown1)
        {
            for (const auto& data : block.meshList)
            {
                Ogre::SubMesh* subMesh = nullptr;
                if (data.type == block_data::Mesh35::UnknownType48)
                {
                    subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::UnknownType50)
                {
                    subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::Indices0)
                {
                    subMesh = CreateSubMesh(true, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else if (data.type == block_data::Mesh35::Unknown2)
                {
                    subMesh = CreateSubMesh(false, data.materialIndex, Ogre::RenderOperation::OT_TRIANGLE_LIST);
                }
                else
                {
                    assert(0 && "not implemented");
                }

                auto visitor = [this, subMesh](auto&& items)
                {
                    SetSubMeshData(subMesh, items);
                };
                std::visit(visitor, data.data);
            }
        }
        else
        {
            assert(0 && "not implemented");
        }
    }
}

void B3dTreeVisitor::Visit(const std::string& name, const block_data::GroupVertexData37& block, VisitMode visitMode)
{
    if (visitMode == VisitMode::PreOrder)
    {
        Ogre::MeshPtr mesh = BeginMesh(name);

        auto visitor = [this, mesh](auto&& vertices)
        {
            AddVertexData(mesh, vertices);
        };

        std::visit(visitor, block.data);
    }
    else
    {
        EndMesh();
    }
}

void B3dTreeVisitor::Visit(const std::string& /* name */, const block_data::SimpleGeneratedObjects40& /* block */, VisitMode /* visitMode */)
{
    B3D_NOT_IMPLEMENTED();
}

std::string B3dTreeVisitor::GetB3dResourceId(const std::string& name) const
{
    return common::GetResourceName(m_b3dId, name);
}

std::string B3dTreeVisitor::GetB3dResourceId(const common::ResourceName& name) const
{
    return GetB3dResourceId(common::ResourceNameToString(name));
}

std::string B3dTreeVisitor::GetMaterialName(const std::uint32_t materialIndex) const
{
    return GetB3dResourceId(common::ResourceNameToString(m_materials[materialIndex]));
}

std::string B3dTreeVisitor::GetNameImpl(const std::string& blockName, const std::string& subName, bool forceUnique) const
{
    std::string name = GetB3dResourceId(blockName) + "_" + subName;

    if (forceUnique)
    {
        static int cnt = 0;
        name += ("_" + std::to_string(cnt));
        ++cnt;
    }

    return name;
}

void B3dTreeVisitor::BeginSceneNode(const std::string& name)
{
    std::string full_name = GetNameImpl(name, "scene_node", false);
    if (m_sceneManager->hasSceneNode(full_name))
    {
        full_name = GetNameImpl(name, "scene_node", true);
    }

    Ogre::SceneNode* sceneNode = m_sceneManager->createSceneNode(full_name);
    Ogre::SceneNode* parent = m_sceneNodes.empty() ? m_rootNode : m_sceneNodes.top();
    parent->addChild(sceneNode);

    m_sceneNodes.push(sceneNode);

    D2_HACK_LOG(B3dTreeVisitor::CreateSceneNode) << "name: " << name << ", parent: " << parent->getName() << ", scene nodes count: " << m_sceneNodes.size();
}

void B3dTreeVisitor::EndSceneNode()
{
    m_sceneNodes.pop();
}

Ogre::MeshPtr B3dTreeVisitor::BeginMesh(const std::string& blockName)
{
    std::string name = GetNameImpl(blockName, "mesh", false);
    std::string group = "D2";
    if (m_meshManager->resourceExists(name, group))
    {
        name = GetNameImpl(blockName, "mesh", true);
    }

    D2_HACK_LOG(BeginMesh) << "Starting processing mesh with name: " << name;

    Ogre::MeshPtr newMesh = m_meshManager->createManual(name, group);
    m_meshStack.push(newMesh);

    return newMesh;
}

void B3dTreeVisitor::EndMesh()
{
    Ogre::MeshPtr currentMesh = m_meshStack.top();
    m_meshStack.pop();

    const std::string entityName = currentMesh->getName() + ".entity";
    Ogre::Entity* entity = m_sceneManager->createEntity(entityName, currentMesh);

    m_sceneNodes.top()->attachObject(entity);

    D2_HACK_LOG(EndMesh) << "Finished processing mesh with name: " << currentMesh->getName();
}

void B3dTreeVisitor::AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordList& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    bind->setBinding(0, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}

void B3dTreeVisitor::AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithNormalList& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    bind->setBinding(0, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}

void B3dTreeVisitor::AddVertexData(const Ogre::MeshPtr& mesh, const common::PositionWithTexCoordNormalList& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    bind->setBinding(0, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}

void B3dTreeVisitor::AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<block_data::GroupVertexData37::Unknown514>& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);


    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    bind->setBinding(0, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}


void B3dTreeVisitor::AddVertexData(const Ogre::MeshPtr& mesh, const std::vector<block_data::GroupVertexData37::Unknown258>& data)
{
    std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

    vertexData->vertexCount = data.size();
    Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

    size_t offset = 0;

    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1);


    Ogre::AxisAlignedBox bbox = mesh->getBounds();

    for (const auto& vertex : data)
    {
        bbox.merge(vertex.position);
    }

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        offset,
        vertexData->vertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    vbuf->writeData(0, vbuf->getSizeInBytes(), data.data(), true);
    bind->setBinding(0, vbuf);

    mesh->sharedVertexData = vertexData.release();

    mesh->_setBounds(bbox, true);
}





Ogre::SubMesh* B3dTreeVisitor::CreateSubMesh(bool useSharedVertices, std::uint32_t materialIndex, Ogre::RenderOperation::OperationType operationType)
{
    const std::string materialName = GetMaterialName(materialIndex);

    auto mesh = m_meshStack.top();

    const auto& subMeshes = mesh->getSubMeshes();

    bool shouldCreateNewSubMesh = false;
    if (subMeshes.empty())
    {
        shouldCreateNewSubMesh = true;
    }
    else
    {
        if (!useSharedVertices || (operationType != Ogre::RenderOperation::OT_TRIANGLE_LIST))
        {
            shouldCreateNewSubMesh = true;
        }
        else
        {
            Ogre::SubMesh* subMesh = subMeshes.back();
            if ((subMesh->useSharedVertices != useSharedVertices) ||
                (subMesh->getMaterialName() != materialName) ||
                (subMesh->operationType != operationType))
            {
                shouldCreateNewSubMesh = true;
            }
        }
    }

    if (shouldCreateNewSubMesh)
    {
        Ogre::SubMesh* subMesh = mesh->createSubMesh();
        subMesh->useSharedVertices = useSharedVertices;
        subMesh->operationType = operationType;

        subMesh->setMaterialName(materialName);

        D2_HACK_LOG(CreateSubmesh) << "New submesh for mesh " << mesh->getName() << ", material name: " << materialName;

        return subMesh;
    }

    return subMeshes.back();
}


void B3dTreeVisitor::SetSubMeshData(Ogre::SubMesh* subMesh, const block_data::Face28Entry& face)
{
    common::TexCoordList texCoords;
    for (const auto& item : face.data)
    {
        texCoords.push_back(item.texCoord);
    }

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = texCoords.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), texCoords.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dTreeVisitor::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexList& data)
{
    ManageSubMeshIndexBuffer(subMesh, data);
}

void B3dTreeVisitor::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithTexCoordList& data)
{
    common::IndexList indices;
    common::TexCoordList texCoords;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        texCoords.push_back(item.texCoord);
    }

    ManageSubMeshIndexBuffer(subMesh, indices);

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = texCoords.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), texCoords.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dTreeVisitor::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionList& data)
{
    common::IndexList indices;
    common::PositionList positions;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        positions.push_back(item.position);
    }

    ManageSubMeshIndexBuffer(subMesh, indices);

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = positions.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), positions.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dTreeVisitor::SetSubMeshData(Ogre::SubMesh* subMesh, const common::IndexWithPositionTexCoordList& data)
{
    common::IndexList indices;
    common::PositionWithTexCoordList positions;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
        positions.push_back({ item.position, item.texCoord });
    }

    ManageSubMeshIndexBuffer(subMesh, indices);

    {
        std::unique_ptr<Ogre::VertexData> vertexData{ OGRE_NEW Ogre::VertexData };

        vertexData->vertexCount = positions.size();
        Ogre::VertexDeclaration* decl = vertexData->vertexDeclaration;
        Ogre::VertexBufferBinding* bind = vertexData->vertexBufferBinding;

        size_t offset = 0;

        decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

        Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            offset,
            vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        vbuf->writeData(0, vbuf->getSizeInBytes(), positions.data(), true);
        bind->setBinding(0, vbuf);

        subMesh->vertexData = vertexData.release();
    }
}

void B3dTreeVisitor::SetSubMeshData(Ogre::SubMesh* subMesh, const std::vector<block_data::Mesh35::Unknown49>& data)
{
    common::IndexList indices;
    for (const auto& item : data)
    {
        indices.push_back(item.index);
    }

    ManageSubMeshIndexBuffer(subMesh, indices);
}

void B3dTreeVisitor::ManageSubMeshIndexBuffer(Ogre::SubMesh* subMesh, const common::IndexList& indices)
{
    size_t prevSize = subMesh->indexData->indexBuffer ? subMesh->indexData->indexBuffer->getNumIndexes() : 0;
    size_t prevSizeInBytes = subMesh->indexData->indexBuffer ? subMesh->indexData->indexBuffer->getSizeInBytes() : 0;

    auto ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_32BIT,
        prevSize + indices.size(),
        Ogre::HardwareBuffer::HBU_STATIC);

    std::vector<std::uint8_t> buff;
    buff.resize(ibuf->getSizeInBytes());

    if (prevSize && prevSizeInBytes)
    {
        subMesh->indexData->indexBuffer->readData(0, prevSizeInBytes, buff.data());
    }

    size_t restDataSize = ibuf->getSizeInBytes() - prevSizeInBytes;
    std::memcpy(buff.data() + prevSizeInBytes, indices.data(), restDataSize);

    ibuf->writeData(0, ibuf->getSizeInBytes(), buff.data(), true);

    subMesh->indexData->indexBuffer = ibuf;
    subMesh->indexData->indexCount = ibuf->getNumIndexes();
    subMesh->indexData->indexStart = 0;
}

} // namespace app
} // namespace d2_hack
