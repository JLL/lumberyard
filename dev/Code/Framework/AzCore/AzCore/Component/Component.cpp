/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Math/Sfmt.h>
#include <AzCore/Math/Crc.h>

namespace AZ
{
    //=========================================================================
    // Component
    // [6/15/2012]
    //=========================================================================
    Component::Component()
        : m_entity(nullptr)
        , m_id(InvalidComponentId)
    {
    }

    //=========================================================================
    // ~Component
    // [6/15/2012]
    //=========================================================================
    Component::~Component()
    {
        if (m_entity)
        {
            m_entity->RemoveComponent(this);
        }
    }

    //=========================================================================
    // GetEntityId
    // [6/15/2012]
    //=========================================================================
    EntityId Component::GetEntityId() const
    {
        if (m_entity)
        {
            return m_entity->GetId();
        }

        AZ_Warning("System", false, "Can't get component %p entity ID as it is not attached to an entity yet!", this);
        return EntityId();
    }

    NamedEntityId Component::GetNamedEntityId() const
    {
        if (m_entity)
        {
            return NamedEntityId(m_entity->GetId(), m_entity->GetName());
        }

        AZ_Warning("System", false, "Can't get component %p entity ID as it is not attached to an entity yet!", this);
        return NamedEntityId();
    }

    //=========================================================================
    // SetConfiguration
    //=========================================================================
    bool Component::SetConfiguration(const ComponentConfig& config)
    {
        // Components cannot be configured while activated.
        if (!m_entity || (m_entity->GetState() <= Entity::State::ES_INIT))
        {
            if (ReadInConfig(&config))
            {
                return true;
            }

            AZ_Warning("System", false, "Configuration type '%s' %s incompatible with component type '%s' %s.",
                config.RTTI_GetTypeName(), config.RTTI_GetType().ToString<AZStd::string>().c_str(),
                RTTI_GetTypeName(), RTTI_GetType().ToString<AZStd::string>().c_str());
        }
        else
        {
            AZ_Warning("System", false, "Component cannot be configured while activated!");
        }
        return false;
    }

    //=========================================================================
    // GetConfiguration
    //=========================================================================
    bool Component::GetConfiguration(ComponentConfig& outConfig) const
    {
        if (WriteOutConfig(&outConfig))
        {
            return true;
        }

        AZ_Warning("System", false, "Configuration type '%s' %s incompatible with component type '%s' %s.",
            outConfig.RTTI_GetTypeName(), outConfig.RTTI_GetType().ToString<AZStd::string>().c_str(),
            RTTI_GetTypeName(), RTTI_GetType().ToString<AZStd::string>().c_str());

        return false;
    }

    //=========================================================================
    // ReadInConfig
    //=========================================================================
    bool Component::ReadInConfig(const ComponentConfig*)
    {
        AZ_Warning("System", false, "ReadInConfig() is not implemented for component type '%s' %s",
            RTTI_GetTypeName(), RTTI_GetType().ToString<AZStd::string>().c_str());
        return false;
    }

    //=========================================================================
    // WriteOutConfig
    //=========================================================================
    bool Component::WriteOutConfig(ComponentConfig*) const
    {
        AZ_Warning("System", false, "WriteOutConfig() is not implemented for component type '%s' %s",
            RTTI_GetTypeName(), RTTI_GetType().ToString<AZStd::string>().c_str());
        return false;
    }

    //=========================================================================
    // Reflect
    //=========================================================================
    void Component::SetEntity(Entity* entity)
    {
        // This can called only from the entity, we assume the input is valid
        if (m_entity != entity)
        {
            m_entity = entity;
            if (entity)
            {
                // We don't have many components on an entity and we guarantee uniques only for this component
                // Random should be find
                if (m_id == InvalidComponentId)
                {
                    // only if this component was removed the entity of it's a new component
                    m_id = Sfmt::GetInstance().Rand64();
                }
            }
            else
            {
                m_id = InvalidComponentId;
            }
        }
    }

    //=========================================================================
    // ReflectInternal
    //=========================================================================
    void Component::ReflectInternal(ReflectContext* reflection)
    {
        SerializeContext* serializeContext = azrtti_cast<SerializeContext*>(reflection);
        if (serializeContext)
        {
            serializeContext->Class<Component>()->
                PersistentId([](const void* instance) -> u64 { return reinterpret_cast<const Component*>(instance)->GetId(); })->
                Field("Id", &Component::m_id);
        }
    }

    //=========================================================================
    // ~ReleaseDescriptor
    //=========================================================================
    void ComponentDescriptor::ReleaseDescriptor()
    {
        EBUS_EVENT(ComponentApplicationBus, UnregisterComponentDescriptor, this);
        delete this;
    }
} // namespace AZ
