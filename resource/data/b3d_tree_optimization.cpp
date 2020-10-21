#include <d2_hack/resource/data/b3d_tree_optimization.h>

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>


#include <d2_hack/resource/data/b3d_utils.h>
#include <d2_hack/common/log.h>

namespace d2_hack
{
namespace resource
{
namespace data
{
namespace b3d
{
namespace optimization
{

class FacesVisitor : public NoOpNodeVisitor
{
public:
    virtual void Visit(const std::string& name, block_data::SimpleFaces8& block, VisitMode visitMode) override
    {
        VisitFaces(name, block, visitMode);
    }

    virtual void Visit(const std::string& name, block_data::SimpleFaces28& block, VisitMode visitMode) override
    {
        VisitFaces(name, block, visitMode);
    }

    virtual void Visit(const std::string& name, block_data::SimpleFaces35& block, VisitMode visitMode) override
    {
        VisitFaces(name, block, visitMode);
    }


private:

    template <typename Faces>
    void VisitFaces(const std::string& /* name */, Faces& block, VisitMode /* visitMode */)
    {
        auto origFaces = std::move(block.faces);

        if (origFaces.empty())
        {
            return;
        }

        std::map<Ogre::RenderOperation::OperationType, decltype(origFaces)> mapping;

        for (const auto& data : origFaces)
        {
            Ogre::RenderOperation::OperationType renderOperation = GetRenderOperation(block, data);
            mapping[renderOperation].push_back(data);
        }

        for (const auto& item : mapping)
        {
            if (CanMerge(item.first))
            {
                auto merged = Merge(item.second);
                block.faces.insert(block.faces.end(), merged.begin(), merged.end());
            }
            else
            {
                block.faces.insert(block.faces.end(), item.second.begin(), item.second.end());
            }
        }
    }

    static bool CanMerge(const Ogre::RenderOperation::OperationType& /* operationType */)
    {
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

        for (const auto& mesh : meshList)
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


static void VisitNode(const NodePtr& node, FacesVisitor& visitor)
{
    node->Visit(visitor, VisitMode::PreOrder);

    const auto& children = node->GetChildNodeList();
    for (auto child : children)
    {
        VisitNode(child, visitor);
    }
}

void MergeFaces(B3dTree& tree)
{
    FacesVisitor facesVisitor;

    for (const auto& node : tree.rootNodes)
    {
        VisitNode(node, facesVisitor);
    }
}


static bool NeedToRemove(const NodePtr& node)
{
    const auto& children = node->GetChildNodeList();

    NodeList newChildList;
    for (auto child : children)
    {
        bool needToRemove = NeedToRemove(child);
        if (!needToRemove)
        {
            newChildList.push_back(child);
        }
        else
        {
            D2_HACK_LOG(NeedToRemove) << "removed node: " << child->GetName() << " with type: " << child->GetType();
        }
    }

    node->SetChildNodes(std::move(newChildList));

    if (node->GetChildNodeList().empty())
    {
        switch (node->GetType())
        {
        case block_data::GroupRoadInfraObjectsBlock4:
        case block_data::GroupObjectsBlock5:
        case block_data::GroupObjectsBlock19:
        case block_data::GroupObjectsBlock21:
            return true;
        };
    }

    return false;
}

void RemoveEmptyNodes(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        NeedToRemove(node);
    }
}


static bool ShouldRemoveLods(const NodePtr& node, bool insideLod)
{
    bool isLodNode = node->GetType() == block_data::GroupLodParametersBlock10;
    if (insideLod && isLodNode)
    {
        return true;
    }

    const auto& children = node->GetChildNodeList();

    NodeList newChildList;
    for (auto child : children)
    {
        bool shouldRemove = ShouldRemoveLods(child, isLodNode || insideLod);
        if (!shouldRemove)
        {
            newChildList.push_back(child);
        }
        else
        {
            D2_HACK_LOG(ShouldRemoveLods) << "removed LOD node: " << child->GetName();
        }
    }

    node->SetChildNodes(std::move(newChildList));
    return false;
}

static void RemoveLods(B3dTree& tree)
{
    for (const auto& node : tree.rootNodes)
    {
        ShouldRemoveLods(node, false);
    }
}


void Optimize(B3dTree& tree)
{
    RemoveEmptyNodes(tree);
    if (0)
    {
        RemoveLods(tree);
        MergeFaces(tree);
    }
}

} // namespace optimization
} // namespace b3d
} // namespace data
} // namespace resource
} // namespace d2_hack
