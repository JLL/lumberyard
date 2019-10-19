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

#include <IGameRef.h>
#include <IEditorGame.h>

namespace LegacyGameInterface
{
    /*!
     * Handles the game instance while using Sandbox.
     */
    class EditorGame
        : public IEditorGame
    {
    public:
        EditorGame();
        ~EditorGame() override {}

        //////////////////////////////////////////////////////////////////////////
        //! IEditorGame
        bool Init(ISystem* system, IGameToEditorInterface* gameToEditorInterface) override;
        void Shutdown() override;
        bool SetGameMode(bool isInGame) override;
        IEntity* GetPlayer() override;
        void SetPlayerPosAng(Vec3 position, Vec3 viewDirection) override;
        void HidePlayer(bool hide) override;
        void OnBeforeLevelLoad() override;
        void OnAfterLevelLoad(const char* levelName, const char* levelFolder) override;
        void OnCloseLevel() override;
        IFlowSystem* GetIFlowSystem() override;
        IGameTokenSystem* GetIGameTokenSystem() override;
        //////////////////////////////////////////////////////////////////////////

    protected:
        enum NetContextConfiguration
        {
            eNetContextConfiguration_Enable,    //!< Enables the net context.
            eNetContextConfiguration_Disable,   //!< Disables the net context.
            eNetContextConfiguration_Reset      //!< Disables, then Enables the net context.
        };

        /*!
         * \param[in] configuration NetContextConfiguration enum value
         *
         * \return a bool indicating the method successfully configured
         */
        bool ConfigureNetContext(NetContextConfiguration configuration);

        IGameRef m_Game;                //!< Reference to the game instance.
        IGameStartup* m_GameStartup;    //!< A pointer to an IGameStartup used to initiailize a game instance.
        bool m_InGame;                  //!< True if in game, false if editing
        bool m_NetContextEnabled;       //!< True if net context is enabled currently, false if not
    };
} // namespace LegacyGameInterface
