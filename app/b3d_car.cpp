#include <d2_hack/app/b3d_car.h>

#include <OgreException.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace app
{

Wheel::Wheel(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : MutableObject(name, std::move(rootNodes))
{
}

void Wheel::Rotate(Ogre::Degree angle)
{
    auto sceneNode = m_rootNodes[0]->GetSceneNode();
    sceneNode->pitch(angle);
}

void Wheel::Turn(Ogre::Degree angle)
{
    auto sceneNode = m_rootNodes[0]->GetSceneNode();
    sceneNode->roll(angle);
}

MoveableObject::MoveableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : BaseGameObject(name, std::move(rootNodes))
{
}

void MoveableObject::SetPosition(const Ogre::Vector3& /* position */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "MoveableObject::SetPosition not implemented");
}


MutableObject::MutableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(name, std::move(rootNodes))
{
}

void MutableObject::EnableShadow(bool enable)
{
    std::string shadowKey = std::string(GetName()) + "ShadowKey";

    ApplyState(shadowKey, enable ? 1 : 0);
}

void MutableObject::ApplyState(std::string_view stateName, size_t stateId)
{
    for (const auto& rootNode : m_rootNodes)
    {
        std::static_pointer_cast<scene_node::SceneNodeBase>(rootNode)->ApplyState(stateName, stateId);
    }
}

WheelBasedMoveableObject::WheelBasedMoveableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : MutableObject(name, std::move(rootNodes))
    , m_wheels(std::move(wheels))
{
}

void WheelBasedMoveableObject::SwitchStopLights(bool /* on */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "WheelBasedMoveableObject::SwitchStopLights not implemented");
}

void WheelBasedMoveableObject::Rotate(Ogre::Degree angle, std::string_view wheelId)
{
    auto wheelIter = m_wheels.find(wheelId);
    if (wheelIter != m_wheels.end())
    {
        wheelIter->second.Rotate(angle);
    }
}

void WheelBasedMoveableObject::Turn(Ogre::Degree angle, std::string_view wheelId)
{
    auto wheelIter = m_wheels.find(wheelId);
    if (wheelIter != m_wheels.end())
    {
        wheelIter->second.Turn(angle);
    }
}

CarBase::CarBase(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : WheelBasedMoveableObject(name, std::move(rootNodes), std::move(wheels))
{
}

B3dSemiTrailer::B3dSemiTrailer(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels rearWheels)
    : WheelBasedMoveableObject(name, std::move(rootNodes), std::move(rearWheels))
{
}

B3dHelicopter::B3dHelicopter(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(name, std::move(rootNodes))
{
}

B3dKatok::B3dKatok(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(name, std::move(rootNodes))
{
}


B3dTruck::B3dTruck(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : CarBase(name, std::move(rootNodes), std::move(wheels))
{
}

void B3dTruck::ConnectSemiTrailer(bool /* on */, B3dSemiTrailer* /* trailer */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "B3dTruck::ConnectSemiTrailer not implemented");
}



B3dCar::B3dCar(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : CarBase(name, std::move(rootNodes), std::move(wheels))
{
}

} // namespace app
} // namespace d2_hack
