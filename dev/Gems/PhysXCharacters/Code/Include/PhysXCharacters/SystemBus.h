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

#pragma once

#include <AzCore/EBus/EBus.h>
#include <AzFramework/Physics/Ragdoll.h>

namespace physx
{
    class PxControllerManager;
} // namespace physx

namespace PhysXCharacters
{
    class SystemRequests
        : public AZ::EBusTraits
    {
    public:
        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        /// Gets a pointer to the per-scene singleton responsible for character controller creation, destruction etc.
        virtual physx::PxControllerManager* GetControllerManager(const Physics::World& world) = 0;
    };
    using SystemRequestBus = AZ::EBus<SystemRequests>;
} // namespace PhysXCharacters
