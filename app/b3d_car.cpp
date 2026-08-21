#include <d2_hack/app/b3d_car.h>

#include <OgreException.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace app
{

Wheel::Wheel(scene_node::SceneNodeBaseList rootNodes)
    : MutableObject(std::move(rootNodes))
{
}

void Wheel::Rotate(Ogre::Degree /* angle */)
{
}

void Wheel::Turn(Ogre::Degree /* angle */)
{
}

MoveableObject::MoveableObject(scene_node::SceneNodeBaseList rootNodes)
    : BaseGameObject(std::move(rootNodes))
{
}

void MoveableObject::SetPosition(const Ogre::Vector3& /* position */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "MoveableObject::SetPosition not implemented");
}


MutableObject::MutableObject(scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(std::move(rootNodes))
{
}

void MutableObject::ApplyState(std::string_view stateName, size_t stateId)
{
    for (const auto& rootNode : m_rootNodes)
    {
        std::static_pointer_cast<scene_node::SceneNodeBase>(rootNode)->ApplyState(stateName, stateId);
    }
}

WheelBasedMoveableObject::WheelBasedMoveableObject(scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : MutableObject(std::move(rootNodes))
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

CarBase::CarBase(scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : WheelBasedMoveableObject(std::move(rootNodes), std::move(wheels))
{
}

B3dSemiTrailer::B3dSemiTrailer(scene_node::SceneNodeBaseList rootNodes, Wheels rearWheels)
    : WheelBasedMoveableObject(std::move(rootNodes), std::move(rearWheels))
{
}

B3dHelicopter::B3dHelicopter(scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(std::move(rootNodes))
{
}

B3dKatok::B3dKatok(scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(std::move(rootNodes))
{
}


B3dTruck::B3dTruck(scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : CarBase(std::move(rootNodes), std::move(wheels))
{
}

void B3dTruck::ConnectSemiTrailer(bool /* on */, B3dSemiTrailer* /* trailer */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "B3dTruck::ConnectSemiTrailer not implemented");
}



B3dCar::B3dCar(scene_node::SceneNodeBaseList rootNodes, Wheels wheels)
    : CarBase(std::move(rootNodes), std::move(wheels))
{
}

} // namespace app
} // namespace d2_hack
