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

#include <GridMate/GridMate.h>
#include <AzCore/EBus/EBus.h>

namespace GridMate
{
    class Driver;
    class DriverAddress;

    class DriverEvents
    {
    public:
        // Called when a datagram is actually sent.
        virtual void OnDatagramSent(size_t payloadBytesSent, const AZStd::intrusive_ptr<DriverAddress>& to) = 0;
        // Called when a datagram is received, before any filtering or processing.
        virtual void OnDatagramReceived(size_t payloadBytesReceived, const AZStd::intrusive_ptr<DriverAddress>& from) = 0;
    };

    class DriverEBusTraits : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        typedef AZStd::recursive_mutex MutexType;
        typedef Driver* BusIdType;
    };
    typedef AZ::EBus<DriverEvents, DriverEBusTraits> DriverEventBus;
}
