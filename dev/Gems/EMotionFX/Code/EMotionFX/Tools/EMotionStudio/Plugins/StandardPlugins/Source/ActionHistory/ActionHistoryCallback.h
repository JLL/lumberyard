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

#include "../StandardPluginsConfig.h"
#include <AzCore/std/containers/vector.h>
#include <MCore/Source/StandardHeaders.h>
#include <MCore/Source/CommandManagerCallback.h>
#include <MCore/Source/Command.h>
#include <MCore/Source/CommandManager.h>
#include <MCore/Source/CommandLine.h>
#include <EMotionFX/CommandSystem/Source/CommandManager.h>

#include <QBrush>
#include <QListWidget>
#include <QDialog>
#include <QTextEdit>


namespace EMStudio
{
    class ErrorWindow
        : public QDialog
    {
        Q_OBJECT // AUTOMOC
        MCORE_MEMORYOBJECTCATEGORY(ErrorWindow, MCore::MCORE_DEFAULT_ALIGNMENT, MEMCATEGORY_STANDARDPLUGINS);

    public:
        ErrorWindow(QWidget* parent);
        void Init(const AZStd::vector<AZStd::string>& errors);
        virtual ~ErrorWindow();

    private:
        QTextEdit* mTextEdit;
    };


    class ActionHistoryCallback
        : public MCore::CommandManagerCallback
    {
        MCORE_MEMORYOBJECTCATEGORY(ActionHistoryCallback, MCore::MCORE_DEFAULT_ALIGNMENT, MEMCATEGORY_STANDARDPLUGINS);

    public:
        ActionHistoryCallback(QListWidget* list);
        ~ActionHistoryCallback();

        void OnPreExecuteCommand(MCore::CommandGroup* group, MCore::Command* command, const MCore::CommandLine& commandLine) override;
        void OnPostExecuteCommand(MCore::CommandGroup* group, MCore::Command* command, const MCore::CommandLine& commandLine, bool wasSuccess, const AZStd::string& outResult) override;
        void OnAddCommandToHistory(uint32 historyIndex, MCore::CommandGroup* group, MCore::Command* command, const MCore::CommandLine& commandLine) override;
        void OnPreExecuteCommandGroup(MCore::CommandGroup* group, bool undo) override;
        void OnPostExecuteCommandGroup(MCore::CommandGroup* group, bool wasSuccess) override;
        void OnRemoveCommand(uint32 historyIndex) override;
        void OnSetCurrentCommand(uint32 index) override;
        void OnShowErrorReport(const AZStd::vector<AZStd::string>& errors) override;

    private:
        QListWidget* mList;
        AZStd::string mTempString;
        uint32 mIndex;
        bool mIsRemoving;
        ErrorWindow* mErrorWindow;

        bool mGroupExecuting;
        MCore::CommandGroup* mExecutedGroup;
        size_t mNumGroupCommands;
        uint32 mCurrentCommandIndex;
        QBrush m_brush;
        QBrush m_darkenedBrush;
    };
} // namespace EMStudio