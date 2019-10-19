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

class QString;
class QStringList;

#include <QLibrary>
#include <QVector>
#include <AzCore/std/containers/set.h>
#include <AzCore/Math/Uuid.h>

namespace AZ
{
    class ComponentDescriptor;
    class Entity;

    namespace Internal
    {
        class EnvironmentInterface;
    }
    typedef Internal::EnvironmentInterface* EnvironmentInstance;
}

namespace AssetBuilder
{
    /**
    * Class to manage external module builders for AssetBuilder.  Note that this is similar
    * to a class in Asset Processor, because both AssetProcessor.exe and AssetBuilder.exe both load builders in a similar manner.
    * The implementation details differ.
    */
    class ExternalModuleAssetBuilderInfo
    {
    public:
        ExternalModuleAssetBuilderInfo(const QString& modulePath);
        virtual ~ExternalModuleAssetBuilderInfo();

        const QString& GetName() const;

        //! Sanity check for the module's status
        bool IsLoaded() const;

        //! Perform the module initialization for the external builder
        void Initialize();

        //! Perform the necessary process of uninitializing an external builder
        void UnInitialize();

        //! Register a builder descriptor ID to track as part of this builders lifecycle management
        void RegisterBuilderDesc(const AZ::Uuid& builderDesc);

        //! Register a component descriptor to track as part of this builders lifecycle management
        void RegisterComponentDesc(AZ::ComponentDescriptor* descriptor);

        //! Check to see if the builder has the required functions defined.
        bool IsAssetBuilder();

    protected:
        bool Load();
        void Unload();

        AZStd::set<AZ::Uuid>    m_registeredBuilderDescriptorIDs;

        typedef void(* InitializeModuleFunction)(AZ::EnvironmentInstance sharedEnvironment);
        typedef void(* ModuleRegisterDescriptorsFunction)(void);
        typedef void(* ModuleAddComponentsFunction)(AZ::Entity* entity);
        typedef void(* UninitializeModuleFunction)(void);

        template<typename T>
        T ResolveModuleFunction(const char* functionName, QStringList& missingFunctionsList);

        InitializeModuleFunction m_initializeModuleFunction;
        ModuleRegisterDescriptorsFunction m_moduleRegisterDescriptorsFunction;
        ModuleAddComponentsFunction m_moduleAddComponentsFunction;
        UninitializeModuleFunction m_uninitializeModuleFunction;
        QVector<AZ::ComponentDescriptor*> m_componentDescriptorList;
        AZ::Entity* m_entity = nullptr;

        QString m_builderName;
        QString m_modulePath;
        QLibrary m_library;
    };
} // AssetBuilder
