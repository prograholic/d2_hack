#include <d2_hack/app/b3d_car.h>

#include <OgreException.h>

namespace d2_hack
{
namespace app
{

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
                                                   std::vector<Wheel> rearWheels,
                                                   Light leftStopLight,
                                                   Light rightStopLight,
                                                   Light leftBackLight,
                                                   Light rightBackLight)
    : DamageableObject(std::move(rootNodes))
    , m_rearWheels(std::move(rearWheels))
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

void WheelBasedMoveableObject::SwitchLight(bool /* on */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "WheelBasedMoveableObject::SwitchLight not implemented");
}


CarBase::CarBase(scene_node::SceneNodeBaseList rootNodes,
                 std::vector<Wheel> rearWheels,
                 Light leftStopLight,
                 Light rightStopLight,
                 Light leftBackLight,
                 Light rightBackLight,
                 Wheel leftFrontWheel,
                 Wheel rightFrontWheel,
                 Light leftFrontLight,
                 Light rightFrontLight)
    : WheelBasedMoveableObject(std::move(rootNodes), std::move(rearWheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight)
    , m_leftFrontWheel(leftFrontWheel)
    , m_rightFrontWheel(rightFrontWheel)
    , m_leftFrontLight(leftFrontLight)
    , m_rightFrontLight(rightFrontLight)
{
}


void CarBase::SwitchLight(bool /*on */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "CarBase::SwitchLight not implemented");
}


B3dTruck::B3dTruck(scene_node::SceneNodeBaseList rootNodes,
                   std::vector<Wheel> rearWheels,
                   Light leftStopLight,
                   Light rightStopLight,
                   Light leftBackLight,
                   Light rightBackLight,
                   Wheel leftFrontWheel,
                   Wheel rightFrontWheel,
                   Light leftFrontLight,
                   Light rightFrontLight)
    : CarBase(std::move(rootNodes), std::move(rearWheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight, leftFrontWheel, rightFrontWheel, leftFrontLight, rightFrontLight)
{
}

void B3dTruck::ConnectSemiTrailer(bool /* on */, B3dSemiTrailer* /* trailer */)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "B3dTruck::ConnectSemiTrailer not implemented");
}



B3dCar::B3dCar(scene_node::SceneNodeBaseList rootNodes,
               std::vector<Wheel> rearWheels,
               Light leftStopLight,
               Light rightStopLight,
               Light leftBackLight,
               Light rightBackLight,
               Wheel leftFrontWheel,
               Wheel rightFrontWheel,
               Light leftFrontLight,
               Light rightFrontLight)
    : CarBase(std::move(rootNodes), std::move(rearWheels), leftStopLight, rightStopLight, leftBackLight, rightBackLight, leftFrontWheel, rightFrontWheel, leftFrontLight, rightFrontLight)
{
}

} // namespace app
} // namespace d2_hack
