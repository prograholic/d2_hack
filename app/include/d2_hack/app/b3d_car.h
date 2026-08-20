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
    Wheel() = default;

    explicit Wheel(scene_node::SceneNodeBasePtr wheelSceneNode);

    void Rotate(Ogre::Degree angle);

    void Turn(Ogre::Degree angle);

private:
    scene_node::SceneNodeBasePtr m_wheelSceneNode;

    Ogre::SceneNode* GetRotationalSceneNode();
};

class Light
{
public:
    Light() = default;

    explicit Light(scene_node::SceneNodeBasePtr lightSceneNode);

    void Switch(bool on);
private:
    scene_node::SceneNodeBasePtr m_lightSceneNode;
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

typedef std::map <std::string, Wheel, std::less<>> Wheels;

class WheelBasedMoveableObject: public DamageableObject
{
public:
    WheelBasedMoveableObject(scene_node::SceneNodeBaseList rootNodes,
                             Wheels wheels,
                             Light leftStopLight,
                             Light rightStopLight,
                             Light leftBackLight,
                             Light rightBackLight);

    void SwitchStopLights(bool on);

    virtual void SwitchLight(bool on);

    void Rotate(Ogre::Degree angle, std::string_view wheelId);
    void Turn(Ogre::Degree angle, std::string_view wheelId);
private:
    Wheels m_wheels;
    Light m_leftStopLight;
    Light m_rightStopLight;
    Light m_leftBackLight;
    Light m_rightBackLight;
};

class CarBase : public WheelBasedMoveableObject
{
public:
    CarBase(scene_node::SceneNodeBaseList rootNodes,
            Wheels wheels,
            Light leftStopLight,
            Light rightStopLight,
            Light leftBackLight,
            Light rightBackLight,
            Light leftFrontLight,
            Light rightFrontLight);

    virtual void SwitchLight(bool on) override;
private:
    Light m_leftFrontLight;
    Light m_rightFrontLight;
};


class B3dSemiTrailer : public WheelBasedMoveableObject
{
public:
    B3dSemiTrailer(scene_node::SceneNodeBaseList rootNodes,
                   Wheels wheels,
                   Light leftStopLight,
                   Light rightStopLight,
                   Light leftBackLight,
                   Light rightBackLight);
};

typedef std::unique_ptr<B3dSemiTrailer> B3dSemiTrailerPtr;

class B3dHelicopter : public MoveableObject
{
public:
    explicit B3dHelicopter(scene_node::SceneNodeBaseList rootNodes);
};

class B3dKatok : public MoveableObject
{
public:
    explicit B3dKatok(scene_node::SceneNodeBaseList rootNodes);
};

// Truck, should be connected with semitrailer
class B3dTruck : public CarBase
{
public:
    B3dTruck(scene_node::SceneNodeBaseList rootNodes,
             Wheels wheels,
             Light leftStopLight,
             Light rightStopLight,
             Light leftBackLight,
             Light rightBackLight,
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
           Wheels wheels,
           Light leftStopLight,
           Light rightStopLight,
           Light leftBackLight,
           Light rightBackLight,
           Light leftFrontLight,
           Light rightFrontLight);
};

typedef std::unique_ptr<B3dCar> B3dCarPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H */
