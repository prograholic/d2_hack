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
    MoveableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes);

    void SetPosition(const Ogre::Vector3& position);
};

typedef std::unique_ptr<MoveableObject> MoveableObjectPtr;

enum class DamageLevel : std::size_t
{
    No = 0,
    Level1,
    Level2,
    Level3
};

enum class DamageKey : std::size_t
{
    BR,
    One,
    R,
    FR,
    FL,
    L,
    FC,
    BC,
    BL,

    Wheel0,
    Wheel1,
    Wheel2,
    Wheel3,
    Wheel4,
    Wheel5,
    Wheel6,
    Wheel7
};

class MutableObject : public MoveableObject
{
public:
    MutableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes);

    void EnableShadow(bool enable);

    virtual void SetDamage(DamageKey key, DamageLevel damageLevel);

protected:
    void ApplyState(std::string_view stateName, size_t stateId);
};


class Wheel : public MutableObject
{
public:
    Wheel(std::string_view name, scene_node::SceneNodeBaseList rootNodes);

    void Rotate(Ogre::Degree angle);

    void Turn(Ogre::Degree angle);
};

enum class WheelId : std::size_t
{
    Wheel0,
    Wheel1,
    Wheel2,
    Wheel3,
    Wheel4,
    Wheel5,
    Wheel6,
    Wheel7,
};

WheelId MapObjectIdToWheelId(std::string_view objectId);

typedef std::map <WheelId, Wheel> Wheels;

class WheelBasedMoveableObject: public MutableObject
{
public:
    WheelBasedMoveableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels);

    void SwitchStopLights(bool on);
    void SwitchBackLights(bool on);
    void SwitchSizeLights(bool on);

    virtual void SetDamage(DamageKey key, DamageLevel damageLevel) override;

    void Rotate(Ogre::Degree angle, WheelId wheelId);
    void Turn(Ogre::Degree angle, WheelId wheelId);
private:
    Wheels m_wheels;

    void SetWheelDamage(DamageKey key, DamageLevel damageLevel);
};

class CarBase : public WheelBasedMoveableObject
{
public:
    CarBase(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels);

    void SwitchFrontLight(bool on);
};


class B3dSemiTrailer : public WheelBasedMoveableObject
{
public:
    B3dSemiTrailer(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels);
};

typedef std::unique_ptr<B3dSemiTrailer> B3dSemiTrailerPtr;

class B3dHelicopter : public MoveableObject
{
public:
    B3dHelicopter(std::string_view name, scene_node::SceneNodeBaseList rootNodes);
};

class B3dKatok : public MoveableObject
{
public:
    B3dKatok(std::string_view name, scene_node::SceneNodeBaseList rootNodes);
};

// Truck, should be connected with semitrailer
class B3dTruck : public CarBase
{
public:
    B3dTruck(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels);

    void ConnectSemiTrailer(bool on, B3dSemiTrailer* trailer);
};

typedef std::unique_ptr<B3dTruck> B3dTruckPtr;

// Simple car
class B3dCar : public CarBase
{
public:
    B3dCar(std::string_view name, scene_node::SceneNodeBaseList rootNodes, Wheels wheels);
};

typedef std::unique_ptr<B3dCar> B3dCarPtr;

} // namespace app
} // namespace d2_hack

#endif /* D2_HACK_APP_INCLUDE_D2_HACK_APP_B3D_CAR_H */
