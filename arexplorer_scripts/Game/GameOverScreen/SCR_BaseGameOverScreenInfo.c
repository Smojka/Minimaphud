 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EGameOverTypes, "m_GameOverScreenId")]
 class SCR_BaseGameOverScreenInfo
 {
     [Attribute("0", UIWidgets.ComboBox, "Game Over Screen ID", "", ParamEnumArray.FromEnum(EGameOverTypes) )]
     protected EGameOverTypes m_GameOverScreenId;

     [Attribute("", desc: "This is the actual game over layout that is shown within the gameover screen", params: "layout")]
     protected ResourceName m_GameOverContentLayout;

     [Attribute(desc: "Optional parameters to be displayed in the game over screen. Note that many of these settings are set by the inherent class like faction flag and icon in FactionGameOverScreenInfo")]
     protected ref SCR_BaseGameOverScreenInfoOptional m_OptionalParams;

     [Attribute(desc: "Params for if the end screen can be displayed in editor. Can be left null if cannot be called by GM")]
     protected ref SCR_BaseGameOverScreenInfoEditor m_OptionalEditorParams;

     EGameOverTypes GetInfoId()
     {
         return m_GameOverScreenId;
     }

     ResourceName GetGameOverContentLayout()
     {
         return m_GameOverContentLayout;
     }

     bool HasOptionalParams()
     {
         return m_OptionalParams != null;
     }

     SCR_BaseGameOverScreenInfoOptional GetOptionalParams()
     {
         return m_OptionalParams;
     }

     SCR_BaseGameOverScreenInfoEditor GetEditorOptionalParams()
     {
         return m_OptionalEditorParams;
     }

     string GetTitle(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sTitle;
         else
             return string.Empty;
     }
     string GetTitle(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sTitle;
         else
             return string.Empty;
     }

     string GetSubtitle(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sSubtitle;
         else
             return string.Empty;
     }

     string GetSubtitle(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sSubtitle;
         else
             return string.Empty;
     }

     ResourceName GetImage(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sImageTexture;
         else
             return string.Empty;
     }

     ResourceName GetImage(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sImageTexture;
         else
             return string.Empty;
     }

     ResourceName GetIcon(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sIcon;
         else
             return string.Empty;
     }

     ResourceName GetIcon(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sIcon;
         else
             return string.Empty;
     }

     Color GetVignetteColor(Faction factionPlayer, array<Faction> factionsOther)
     {
         return null;
     }

     Color GetVignetteColor(int playerId, array<int> otherPlayerIds)
     {
         return null;
     }

     string GetDebriefing(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sDebriefing;
         else
             return string.Empty;
     }

     string GetDebriefing(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sDebriefing;
         else
             return string.Empty;
     }

     string GetOneShotAudio(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sAudioOneShot;
         else
             return string.Empty;
     }

     string GetOneShotAudio(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sAudioOneShot;
         else
             return string.Empty;
     }

     string GetTitleParam(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sTitleParam;
         else
             return string.Empty;
     }

     string GetTitleParam(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sTitleParam;
         else
             return string.Empty;
     }

     string GetSubtitleParam(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sSubtitleParam;
         else
             return string.Empty;
     }

     string GetSubtitleParam(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sSubtitleParam;
         else
             return string.Empty;
     }

     string GetDebriefingParam(Faction factionPlayer, array<Faction> factionsOther)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sDebriefingParam;
         else
             return string.Empty;
     }

     string GetDebriefingParam(int playerId, array<int> otherPlayerIds)
     {
         if (HasOptionalParams())
             return m_OptionalParams.m_sDebriefingParam;
         else
             return string.Empty;
     }

     void GameOverUICustomization(SCR_GameOverScreenUIComponent gameOverUI, Faction factionPlayer, array<Faction> factionsOther)
     {

     }

     void GameOverUICustomization(SCR_GameOverScreenUIComponent gameOverUI, int playerId, array<int> otherPlayerIds)
     {

     }
 };

 [BaseContainerProps(), BaseContainerCustomStringTitleField("Optional")]
 class SCR_BaseGameOverScreenInfoOptional
 {
     [Attribute()]
     LocalizedString m_sTitle;

     [Attribute()]
     LocalizedString m_sSubtitle;

     [Attribute()]
     ResourceName m_sImageTexture;

     [Attribute()]
     ResourceName m_sIcon;

     [Attribute()]
     LocalizedString m_sDebriefing;

     [Attribute()]
     string m_sAudioOneShot;

     [Attribute(desc: "%1 param in m_sTitle")]
     string m_sTitleParam;

     [Attribute(desc: "%1 param in m_sSubtitle")]
     string m_sSubtitleParam;

     [Attribute(desc: "%1 param in m_sDebriefing")]
     string m_sDebriefingParam;
 };

 [BaseContainerProps(), BaseContainerCustomStringTitleField("Optional Editor")]
 class SCR_BaseGameOverScreenInfoEditor
 {
     [Attribute("1", desc: "If this is true then the end game state can be set by GM if false but still has a display name it can still be used as a mirror state of another game state (Eg: Defeat cannot be set by GM but if playable faction is not selected when Victory condition is chosen then they will still be displayed defeat as this is a mirror condition for victory.")]
     bool m_bCanBeSetByGameMaster;

     [Attribute("0", desc: "This endscreen type can only show if at least one faction is playable")]
     bool m_bNeedsPlayableFactions;

     [Attribute(desc: "Editor only if m_bCanBeSetByGameMaster is true, This is not shown in the GameOver UI but instead the name shown in editor when deciding which faction shows what gameOver screen.")]
     LocalizedString m_sDisplayName;

     [Attribute(desc: "Editor only if m_bCanBeSetByGameMaster is true, This is not shown in the GameOver UI but instead show in the editor when this end game type has been chosen, explaining more about it to the player.")]
     LocalizedString m_sDescription;

     [Attribute(desc: "Optional param for description")]
     LocalizedString m_sDescriptionParam1;

     [Attribute(desc: "Optional param for description")]
     LocalizedString m_sDescriptionParam2;

     [Attribute(desc: "Editor only, if m_bCanBeSetByGameMaster is true and thus can be selected. If NOT set to NONE: This will indicate if the state has a mirror state. Any playable faction that is not selected will be given this end state instead eg: Not selected for Victory will get defeat", UIWidgets.ComboBox, "Mirrored state ID", "", ParamEnumArray.FromEnum(EGameOverTypes))]
     EGameOverTypes m_MirroredState;
 };