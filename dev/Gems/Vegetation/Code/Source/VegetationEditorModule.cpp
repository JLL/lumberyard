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

#include "Vegetation_precompiled.h"

#include <VegetationEditorModule.h>

#include <Editor/EditorAreaBlenderComponent.h>
#include <Editor/EditorBlockerComponent.h>
#include <Editor/EditorDescriptorListCombinerComponent.h>
#include <Editor/EditorDescriptorListComponent.h>
#include <Editor/EditorDescriptorWeightSelectorComponent.h>
#include <Editor/EditorDistanceBetweenFilterComponent.h>
#include <Editor/EditorDistributionFilterComponent.h>
#include <Editor/EditorLevelSettingsComponent.h>
#include <Editor/EditorMeshBlockerComponent.h>
#include <Editor/EditorPositionModifierComponent.h>
#include <Editor/EditorReferenceShapeComponent.h>
#include <Editor/EditorRotationModifierComponent.h>
#include <Editor/EditorScaleModifierComponent.h>
#include <Editor/EditorShapeIntersectionFilterComponent.h>
#include <Editor/EditorSlopeAlignmentModifierComponent.h>
#include <Editor/EditorSpawnerComponent.h>
#include <Editor/EditorSurfaceAltitudeFilterComponent.h>
#include <Editor/EditorSurfaceMaskDepthFilterComponent.h>
#include <Editor/EditorSurfaceMaskFilterComponent.h>
#include <Editor/EditorSurfaceSlopeFilterComponent.h>
#include <Editor/EditorStaticVegetationBlockerComponent.h>

#include <Editor/EditorVegetationSystemComponent.h>

#include <Debugger/EditorDebugComponent.h>
#include <Debugger/EditorAreaDebugComponent.h>

namespace Vegetation
{
    VegetationEditorModule::VegetationEditorModule()
        : VegetationModule()
    {
        m_descriptors.insert(m_descriptors.end(), {
            EditorAreaBlenderComponent::CreateDescriptor(),
            EditorBlockerComponent::CreateDescriptor(),
            EditorDescriptorListCombinerComponent::CreateDescriptor(),
            EditorDescriptorListComponent::CreateDescriptor(),
            EditorDescriptorWeightSelectorComponent::CreateDescriptor(),
            EditorDistanceBetweenFilterComponent::CreateDescriptor(),
            EditorDistributionFilterComponent::CreateDescriptor(),
            EditorLevelSettingsComponent::CreateDescriptor(),
            EditorMeshBlockerComponent::CreateDescriptor(),
            EditorPositionModifierComponent::CreateDescriptor(),
            EditorReferenceShapeComponent::CreateDescriptor(),
            EditorRotationModifierComponent::CreateDescriptor(),
            EditorScaleModifierComponent::CreateDescriptor(),
            EditorShapeIntersectionFilterComponent::CreateDescriptor(),
            EditorSlopeAlignmentModifierComponent::CreateDescriptor(),
            EditorSpawnerComponent::CreateDescriptor(),
            EditorSurfaceAltitudeFilterComponent::CreateDescriptor(),
            EditorSurfaceMaskDepthFilterComponent::CreateDescriptor(),
            EditorSurfaceMaskFilterComponent::CreateDescriptor(),
            EditorSurfaceSlopeFilterComponent::CreateDescriptor(),
            EditorStaticVegetationBlockerComponent::CreateDescriptor(),
            EditorVegetationSystemComponent::CreateDescriptor(),
            EditorDebugComponent::CreateDescriptor(),
            EditorAreaDebugComponent::CreateDescriptor(),
        });
    }

    AZ::ComponentTypeList VegetationEditorModule::GetRequiredSystemComponents() const
    {
        AZ::ComponentTypeList requiredComponents = VegetationModule::GetRequiredSystemComponents();
        requiredComponents.push_back(azrtti_typeid<EditorVegetationSystemComponent>());
        return requiredComponents;
    }
}

AZ_DECLARE_MODULE_CLASS(VegetationEditor, Vegetation::VegetationEditorModule)
