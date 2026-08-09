#include <d2_hack/app/b3d_car.h>

#include <OgreException.h>

#include <d2_hack/resource/data/b3d_types.h>

namespace d2_hack
{
namespace app
{

Wheel::Wheel(scene_node::SceneNodeBasePtr wheelSceneNode)
    : m_wheelSceneNode(std::move(wheelSceneNode))
{
}

void Wheel::Rotate(Ogre::Degree angle)
{
    GetRotationalSceneNode()->pitch(angle);
}

void Wheel::Turn(Ogre::Degree angle)
{
    GetRotationalSceneNode()->roll(angle);
}

Ogre::SceneNode* Wheel::GetRotationalSceneNode()
{
    auto typedSceneNode = m_wheelSceneNode->NodeCast<scene_node::OgreSceneNode<resource::data::b3d::block_data::GroupObjectsBlock5>>();
    /*
    PrintSceneNode:                 NODE: TRUCKS_SCENE_NODE-Zilwheel0, Vector3(0, 0, 0)
    PrintSceneNode:                     NODE: TRUCKS_SCENE_NODE-1299, Vector3(-0.85, -0.39, -2.1)     <------ Нужно брать дочернюю ноду.
     */

    return static_cast<Ogre::SceneNode*>(typedSceneNode->GetOgreSceneNode()->getChildren()[0]);
}

Light::Light(scene_node::SceneNodeBasePtr lightSceneNode)
    : m_lightSceneNode(std::move(lightSceneNode))
{
}

void Light::Switch(bool /* on */)
{
    //m_lightSceneNode
    //OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "Light::Switch not implemented");
}

MoveableObject::MoveableObject(scene_node::SceneNodeBaseList rootNodes)
    : BaseGameObject(std::move(rootNodes))
{
}

void MoveableObject::SetPosition(const Ogre::Vector3& /* position */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "MoveableObject::SetPosition not implemented");
}


DamageableObject::DamageableObject(scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(std::move(rootNodes))
{
}

void DamageableObject::SetDamage(bool /* on */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "DamageableObject::SetDamage not implemented");
}

WheelBasedMoveableObject::WheelBasedMoveableObject(scene_node::SceneNodeBaseList rootNodes,
                                                   Wheels wheels,
                                                   Light leftStopLight,
                                                   Light rightStopLight,
                                                   Light leftBackLight,
                                                   Light rightBackLight)
    : DamageableObject(std::move(rootNodes))
    , m_wheels(std::move(wheels))
    , m_leftStopLight(leftStopLight)
    , m_rightStopLight(rightStopLight)
    , m_leftBackLight(leftBackLight)
    , m_rightBackLight(rightBackLight)
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

void WheelBasedMoveableObject::SwitchLight(bool on)
{
    m_leftBackLight.Switch(on);
    m_rightBackLight.Switch(on);
}


CarBase::CarBase(scene_node::SceneNodeBaseList rootNodes,
                 Wheels wheels,
                 Light leftStopLight,
                 Light rightStopLight,
                 Light leftBackLight,
                 Light rightBackLight,
                 Light leftFrontLight,
                 Light rightFrontLight)
    : WheelBasedMoveableObject(std::move(rootNodes), std::move(wheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight)
    , m_leftFrontLight(leftFrontLight)
    , m_rightFrontLight(rightFrontLight)
{
}


void CarBase::SwitchLight(bool on)
{
    m_leftFrontLight.Switch(on);
    m_rightFrontLight.Switch(on);

    WheelBasedMoveableObject::SwitchLight(on);
}


B3dSemiTrailer::B3dSemiTrailer(scene_node::SceneNodeBaseList rootNodes,
                               Wheels rearWheels,
                               Light leftStopLight,
                               Light rightStopLight,
                               Light leftBackLight,
                               Light rightBackLight)
    : WheelBasedMoveableObject(std::move(rootNodes), std::move(rearWheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight)
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


B3dTruck::B3dTruck(scene_node::SceneNodeBaseList rootNodes,
                   Wheels wheels,
                   Light leftStopLight,
                   Light rightStopLight,
                   Light leftBackLight,
                   Light rightBackLight,
                   Light leftFrontLight,
                   Light rightFrontLight)
    : CarBase(std::move(rootNodes), std::move(wheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight, leftFrontLight, rightFrontLight)
{
}

void B3dTruck::ConnectSemiTrailer(bool /* on */, B3dSemiTrailer* /* trailer */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "B3dTruck::ConnectSemiTrailer not implemented");
}



B3dCar::B3dCar(scene_node::SceneNodeBaseList rootNodes,
               Wheels wheels,
               Light leftStopLight,
               Light rightStopLight,
               Light leftBackLight,
               Light rightBackLight,
               Light leftFrontLight,
               Light rightFrontLight)
    : CarBase(std::move(rootNodes), std::move(wheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight, leftFrontLight, rightFrontLight)
{
}

} // namespace app
} // namespace d2_hack
