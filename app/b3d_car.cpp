#include <d2_hack/app/b3d_car.h>

#include <format>

#include <OgreException.h>

#include <d2_hack/resource/data/b3d_types.h>

#include <d2_hack/common/log.h>

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

WheelId MapObjectIdToWheelId(std::string_view objectId)
{
    if (objectId.ends_with("wheel0"))
    {
        return WheelId::Wheel0;
    }
    else if (objectId.ends_with("wheel1"))
    {
        return WheelId::Wheel1;
    }
    else if (objectId.ends_with("wheel2"))
    {
        return WheelId::Wheel2;
    }
    else if (objectId.ends_with("wheel3"))
    {
        return WheelId::Wheel3;
    }
    else if (objectId.ends_with("wheel4"))
    {
        return WheelId::Wheel4;
    }
    else if (objectId.ends_with("wheel5"))
    {
        return WheelId::Wheel5;
    }
    else if (objectId.ends_with("wheel6"))
    {
        return WheelId::Wheel6;
    }
    else if (objectId.ends_with("wheel7"))
    {
        return WheelId::Wheel7;
    }
    
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Cannot map `{}` to WheelId", objectId));
}

MutableObject::MutableObject(std::string_view name, scene_node::SceneNodeBaseList rootNodes)
    : MoveableObject(name, std::move(rootNodes))
{
}

void MutableObject::EnableShadow(bool enable)
{
    // TODO: add Shadow to root nodes!!!!
    std::string shadowKey = std::string(GetName()) + "ShadowKey";

    ApplyState(shadowKey, enable ? 1 : 0);
}


static std::string_view MapDamageKeyToString(DamageKey key)
{
    static constexpr std::string_view DamageKeyMapping[] =
    {
        "DamageBRKey",
        "Damage1Key",
        "DamageRKey",
        "DamageFRKey",
        "DamageFLKey",
        "DamageLKey",
        "DamageFCKey",
        "DamageBCKey",
        "DamageBLKey",
        "DamageWheel0Key",
        "DamageWheel1Key",
        "DamageWheel2Key",
        "DamageWheel3Key",
        "DamageWheel4Key",
        "DamageWheel5Key",
        "DamageWheel6Key",
        "DamageWheel7Key",
        "DamageWheel7Key",
    };

    return DamageKeyMapping[static_cast<size_t>(key)];
}

void MutableObject::SetDamage(DamageKey key, DamageLevel damageLevel)
{
    ApplyState(MapDamageKeyToString(key), static_cast<size_t>(damageLevel));
}

void MutableObject::ApplyState(std::string_view stateName, size_t stateId)
{
    D2_HACK_LOG(MutableObject::ApplyState) << "firing event `" << stateName << "` with state " << stateId;
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

void WheelBasedMoveableObject::SetDamage(DamageKey key, DamageLevel damageLevel)
{
    switch (key)
    {
    case DamageKey::Wheel0:
    case DamageKey::Wheel1:
    case DamageKey::Wheel2:
    case DamageKey::Wheel3:
    case DamageKey::Wheel4:
    case DamageKey::Wheel5:
    case DamageKey::Wheel6:
    case DamageKey::Wheel7:
        SetWheelDamage(key, damageLevel);
        break;

    default:
        MutableObject::SetDamage(key, damageLevel);
    }

}

void WheelBasedMoveableObject::Rotate(Ogre::Degree angle, WheelId wheelId)
{
    auto wheelIter = m_wheels.find(wheelId);
    if (wheelIter != m_wheels.end())
    {
        wheelIter->second.Rotate(angle);
    }
}

void WheelBasedMoveableObject::Turn(Ogre::Degree angle, WheelId wheelId)
{
    auto wheelIter = m_wheels.find(wheelId);
    if (wheelIter != m_wheels.end())
    {
        wheelIter->second.Turn(angle);
    }
}

void WheelBasedMoveableObject::SetWheelDamage(DamageKey key, DamageLevel damageLevel)
{
    static constexpr WheelId DamageKeyToWheelIdMapping[] =
    {
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,
        WheelId::Wheel0,

        WheelId::Wheel0,
        WheelId::Wheel1,
        WheelId::Wheel2,
        WheelId::Wheel3,
        WheelId::Wheel4,
        WheelId::Wheel5,
        WheelId::Wheel6,
        WheelId::Wheel7
    };

    WheelId wheelId = DamageKeyToWheelIdMapping[static_cast<size_t>(key)];

    auto wheelIter = m_wheels.find(wheelId);
    if (wheelIter != m_wheels.end())
    {
        wheelIter->second.SetDamage(key, damageLevel);
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
