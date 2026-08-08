#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H

#include <d2_hack/app/base_game_object.h>

namespace d2_hack
{
namespace app
{


class Wheel
{
public:
    void Rotate(float angle);

private:
    scene_node::SceneNodeBasePtr m_wheelSceneNode;
};

class Light
{
public:
    void Switch(bool on);
};




class MoveableObject : public BaseGameObject
{
public:
    explicit MoveableObject(scene_node::SceneNodeBaseList rootNodes);

    void SetPosition(const Ogre::Vector3& position);
};

typedef std::unique_ptr<MoveableObject> MoveableObjectPtr;

class DamageableObject : public MoveableObject
{
public:
    explicit DamageableObject(scene_node::SceneNodeBaseList rootNodes);

    void SetDamage(bool on);
};

class WheelBasedMoveableObject: public DamageableObject
{
public:
    WheelBasedMoveableObject(scene_node::SceneNodeBaseList rootNodes,
                             std::vector<Wheel> rearWheels,
                             Light leftStopLight,
                             Light rightStopLight,
                             Light leftBackLight,
                             Light rightBackLight);

    void SwitchStopLights(bool on);

    virtual void SwitchLight(bool on);
private:
    std::vector<Wheel> m_rearWheels;
    Light m_leftStopLight;
    Light m_rightStopLight;
    Light m_leftBackLight;
    Light m_rightBackLight;
};

class CarBase : public WheelBasedMoveableObject
{
public:
    CarBase(scene_node::SceneNodeBaseList rootNodes,
            std::vector<Wheel> rearWheels,
            Light leftStopLight,
            Light rightStopLight,
            Light leftBackLight,
            Light rightBackLight,
            Wheel leftFrontWheel,
            Wheel rightFrontWheel,
            Light leftFrontLight,
            Light rightFrontLight);

    virtual void SwitchLight(bool on) override;
private:
    Wheel m_leftFrontWheel;
    Wheel m_rightFrontWheel;

    Light m_leftFrontLight;
    Light m_rightFrontLight;
};


class B3dSemiTrailer : public WheelBasedMoveableObject
{
};

class B3dHelicopter : public MoveableObject
{
};

class B3dKatok : public MoveableObject
{
};

// Truck, should be connected with semitrailer
class B3dTruck : public CarBase
{
public:
    B3dTruck(scene_node::SceneNodeBaseList rootNodes,
             std::vector<Wheel> rearWheels,
             Light leftStopLight,
             Light rightStopLight,
             Light leftBackLight,
             Light rightBackLight,
             Wheel leftFrontWheel,
             Wheel rightFrontWheel,
             Light leftFrontLight,
             Light rightFrontLight);

    void ConnectSemiTrailer(bool on, B3dSemiTrailer* trailer);
};

typedef std::unique_ptr<B3dTruck> B3dTruckPtr;

// Simple car
class B3dCar : public CarBase
{
public:
    B3dCar(scene_node::SceneNodeBaseList rootNodes,
           std::vector<Wheel> rearWheels,
           Light leftStopLight,
           Light rightStopLight,
           Light leftBackLight,
           Light rightBackLight,
           Wheel leftFrontWheel,
           Wheel rightFrontWheel,
           Light leftFrontLight,
           Light rightFrontLight);
};

typedef std::unique_ptr<B3dCar> B3dCarPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H */
