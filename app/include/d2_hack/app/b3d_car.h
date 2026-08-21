#ifndef D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H
#define D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H

#include <d2_hack/app/base_game_object.h>

namespace d2_hack
{
namespace app
{




class MoveableObject : public BaseGameObject
{
public:
    explicit MoveableObject(scene_node::SceneNodeBaseList rootNodes);

    void SetPosition(const Ogre::Vector3& position);
};

typedef std::unique_ptr<MoveableObject> MoveableObjectPtr;

class MutableObject : public MoveableObject
{
public:
    explicit MutableObject(scene_node::SceneNodeBaseList rootNodes);

protected:
    void ApplyState(std::string_view stateName, size_t stateId);
};


class Wheel : public MutableObject
{
public:
    explicit Wheel(scene_node::SceneNodeBaseList rootNodes);

    enum class Damage : std::size_t
    {
        No = 0,
        Level1,
        Level2,
        Level3
    };

    void SetDamage(Damage damage);

    void Rotate(Ogre::Degree angle);

    void Turn(Ogre::Degree angle);
};


typedef std::map <std::string, Wheel, std::less<>> Wheels;

class WheelBasedMoveableObject: public MutableObject
{
public:
    WheelBasedMoveableObject(scene_node::SceneNodeBaseList rootNodes, Wheels wheels);

    void SwitchStopLights(bool on);
    void SwitchBackLights(bool on);
    void SwitchSizeLights(bool on);

    void Rotate(Ogre::Degree angle, std::string_view wheelId);
    void Turn(Ogre::Degree angle, std::string_view wheelId);
private:
    Wheels m_wheels;
};

class CarBase : public WheelBasedMoveableObject
{
public:
    CarBase(scene_node::SceneNodeBaseList rootNodes, Wheels wheels);

    void SwitchFrontLight(bool on);
};


class B3dSemiTrailer : public WheelBasedMoveableObject
{
public:
    B3dSemiTrailer(scene_node::SceneNodeBaseList rootNodes, Wheels wheels);
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
    B3dTruck(scene_node::SceneNodeBaseList rootNodes, Wheels wheels);

    void ConnectSemiTrailer(bool on, B3dSemiTrailer* trailer);
};

typedef std::unique_ptr<B3dTruck> B3dTruckPtr;

// Simple car
class B3dCar : public CarBase
{
public:
    B3dCar(scene_node::SceneNodeBaseList rootNodes, Wheels wheels);
};

typedef std::unique_ptr<B3dCar> B3dCarPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H */
