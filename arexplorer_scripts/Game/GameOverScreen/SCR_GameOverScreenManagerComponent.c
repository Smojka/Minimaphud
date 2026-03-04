 [ComponentEditorProps(category: "GameScripted/GameOver", description: "")]
 class SCR_GameOverScreenManagerComponentClass : SCR_BaseGameModeComponentClass
 {
 }

 class SCR_GameOverScreenManagerComponent : SCR_BaseGameModeComponent
 {
     [Attribute()]
     protected ref SCR_GameOverScreenConfig m_GameOverScreenConfig;

     [Attribute("1", desc: "Delay (in seconds) to lock player input and display the actual end screen after end screen has faded in")]
     protected float m_fShowEndscreenDelay;

     [Attribute("{144919871D65F121}UI/layouts/HUD/GameOver/EndScreen/EndScreen.layout", params: "layout")]
     protected ResourceName m_sGameOverScreenBasePrefab;

     //Editor only
     protected bool m_bListeningToEditorCalledEndGame = false;
     protected EGameOverTypes m_iEditorSetGameOverType;
     protected ref array<int> m_aEditorSetFactions = {};

     //End game vars saved so it can be usesed in multiple function
     protected EGameOverTypes m_iEndGameType = EGameOverTypes.UNKNOWN;
     protected bool m_bIsFactionVictory = false;
     protected Faction m_FactionPlayer = null;
     protected int m_iPlayerId = -1;
     protected int m_iEndReason = 0;
     protected ref array<Faction> m_aWinningFactions = {};
     protected ref array<int> m_aWinningPlayers = {};
     protected bool m_bIsPlayingGameOverAudio = false;

     static const ref ScriptInvoker s_OnEndGame = new ScriptInvoker();

     protected Widget m_EndScreenFade;

     //------------------------------------------------------------------------------------------------
  SCR_GameOverScreenConfig GetGameOverConfig()
     {
         return m_GameOverScreenConfig;
     }

     //------------------------------------------------------------------------------------------------
  protected void StartEndGameFade()
     {
         SetGameOverVarriables(GetGameMode().GetEndGameData());
         SCR_HUDManagerComponent hudManager = GetGame().GetHUDManager();
         if (!hudManager)
             return;

         m_EndScreenFade = hudManager.CreateLayout(m_sGameOverScreenBasePrefab, EHudLayers.ALWAYS_TOP, 0);
         SCR_FadeUIComponent fadeComponent = SCR_FadeUIComponent.Cast(m_EndScreenFade.FindHandler(SCR_FadeUIComponent));

         if (!fadeComponent)
         {
             Debug.Error2("SCR_GameOverScreenManagerComponent", "StartEndGameFade, could not find FadeUIComponent!");
             return;
         }

         fadeComponent.GetOnFadeDone().Insert(OnEndScreenFadeDone);
         fadeComponent.FadeIn();

         // Hide other HUD elements
         hudManager.SetVisibleLayers(hudManager.GetVisibleLayers() & ~(EHudLayers.HIGH));

         //Play game over audio if has any and non are playing
         if (m_bIsPlayingGameOverAudio)
             return;

         SCR_BaseGameOverScreenInfo gameOverScreenInfo;
         if (!m_GameOverScreenConfig.GetGameOverScreenInfo(m_iEndGameType, gameOverScreenInfo))
         {
             Print(string.Format("SCR_GameOverScreenManagerComponent could not find end screen: %1, to play audio", typename.EnumToString(EGameOverTypes, m_iEndGameType)), LogLevel.WARNING);
             return;
         }

         if (!gameOverScreenInfo.HasOptionalParams())
             return;

         if (m_bIsFactionVictory)
             s_OnEndGame.Invoke(gameOverScreenInfo.GetOneShotAudio(m_FactionPlayer, m_aWinningFactions));
         else
             s_OnEndGame.Invoke(gameOverScreenInfo.GetOneShotAudio(m_iPlayerId, m_aWinningPlayers));

         m_bIsPlayingGameOverAudio = true;
     }

     //------------------------------------------------------------------------------------------------
  protected void OnEndScreenFadeDone(SCR_FadeUIComponent fadeComponent, bool isFadeIn)
     {
         //Close all dialogs
         GetGame().GetMenuManager().CloseAllMenus();
         GetGame().GetCallqueue().CallLater(ShowGameOverScreen, m_fShowEndscreenDelay * 1000, false, null);
     }

     //------------------------------------------------------------------------------------------------
  SCR_GameOverScreenUIComponent ShowGameOverScreen(SCR_GameModeEndData endData = null)
     {
         //Remove the fade UI
         if (m_EndScreenFade)
         {
             m_EndScreenFade.RemoveFromHierarchy();

             // Unhide other HUD elements which we hid previously when we started the fade
             SCR_HUDManagerComponent hudManager = GetGame().GetHUDManager();
             if (hudManager)
                 hudManager.SetVisibleLayers(hudManager.GetVisibleLayers() | EHudLayers.HIGH);
         }

         if (!endData)
             endData = GetGameMode().GetEndGameData();

         if (m_iEndGameType == EGameOverTypes.UNKNOWN)
             SetGameOverVarriables(endData);

         SCR_BaseGameOverScreenInfo gameOverScreenInfo;

         //Get game over type
         if (!m_GameOverScreenConfig.GetGameOverScreenInfo(m_iEndGameType, gameOverScreenInfo))
         {
             Print(string.Format("'SCR_GameOverScreenConfig': Could not find '%1' endscreen in array!", typename.EnumToString(EGameOverTypes, m_iEndGameType)), LogLevel.NORMAL);
             return null;
         }

         if (gameOverScreenInfo.HasOptionalParams())
         {
             //Show end screen
             if (m_bIsFactionVictory)
             {
                 if (!m_bIsPlayingGameOverAudio)
                     s_OnEndGame.Invoke(gameOverScreenInfo.GetOneShotAudio(m_FactionPlayer, m_aWinningFactions));

                 SCR_GameOverScreenUIComponent gameOverScreen = OpenGameOverScreen(gameOverScreenInfo.GetGameOverContentLayout(), endData, gameOverScreenInfo.GetTitle(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetSubtitle(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetDebriefing(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetImage(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetIcon(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetVignetteColor(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetTitleParam(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetSubtitleParam(m_FactionPlayer, m_aWinningFactions), gameOverScreenInfo.GetDebriefingParam(m_FactionPlayer, m_aWinningFactions));
                 gameOverScreenInfo.GameOverUICustomization(gameOverScreen, m_FactionPlayer, m_aWinningFactions);
                 return gameOverScreen;
             }
             else
             {
                 if (!m_bIsPlayingGameOverAudio)
                     s_OnEndGame.Invoke(gameOverScreenInfo.GetOneShotAudio(m_iPlayerId, m_aWinningPlayers));

                 SCR_GameOverScreenUIComponent gameOverScreen = OpenGameOverScreen(gameOverScreenInfo.GetGameOverContentLayout(), endData, gameOverScreenInfo.GetTitle(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetSubtitle(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetDebriefing(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetImage(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetIcon(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetVignetteColor(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetTitleParam(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetSubtitleParam(m_iPlayerId, m_aWinningPlayers), gameOverScreenInfo.GetDebriefingParam(m_iPlayerId, m_aWinningPlayers));
                 gameOverScreenInfo.GameOverUICustomization(gameOverScreen, m_iPlayerId, m_aWinningPlayers);
                 return gameOverScreen;
             }
         }
         else
         {
             if (m_bIsFactionVictory)
             {
                 SCR_GameOverScreenUIComponent gameOverScreen = OpenGameOverScreen(gameOverScreenInfo.GetGameOverContentLayout(), endData, gameOverScreenInfo.GetTitle(m_FactionPlayer, m_aWinningFactions));
                 gameOverScreenInfo.GameOverUICustomization(gameOverScreen, m_FactionPlayer, m_aWinningFactions);
                 return gameOverScreen;
             }
             else
             {
                 SCR_GameOverScreenUIComponent gameOverScreen = OpenGameOverScreen(gameOverScreenInfo.GetGameOverContentLayout(), endData, gameOverScreenInfo.GetTitle(m_iPlayerId, m_aWinningPlayers));
                 gameOverScreenInfo.GameOverUICustomization(gameOverScreen, m_iPlayerId, m_aWinningPlayers);
                 return gameOverScreen;
             }
         }

         //Makes sure set game over varriables are set again next time end game is called
         m_iEndGameType = EGameOverTypes.UNKNOWN;
     }

     //------------------------------------------------------------------------------------------------
  protected void SetGameOverVarriables(SCR_GameModeEndData endData = null)
     {
         m_iEndGameType = EGameOverTypes.UNKNOWN;
         m_bIsPlayingGameOverAudio = false;
         m_FactionPlayer = null;
         m_iPlayerId = -1;
         m_iEndReason = 0;
         m_aWinningFactions.Clear();
         m_aWinningPlayers.Clear();
         m_bIsFactionVictory = false;

         FactionManager factionManager = GetGame().GetFactionManager();
         PlayerManager playerManager = GetGame().GetPlayerManager();

         if (!endData)
             endData = GetGameMode().GetEndGameData();

         if (endData != null)
         {
             m_iEndReason = endData.GetEndReason();

             if (factionManager)
             {
                 array<int> winningFactionIds = {};
                 endData.GetFactionWinnerIds(winningFactionIds);

                 if (winningFactionIds)
                 {
                     Faction factionVictor;

                     foreach (int id: winningFactionIds)
                     {
                         factionVictor = factionManager.GetFactionByIndex(id);
                         if (factionVictor)
                             m_aWinningFactions.Insert(factionVictor);
                     }
                 }
             }

             endData.GetWinnerIds(m_aWinningPlayers);

             m_bIsFactionVictory = (m_aWinningFactions != null && !m_aWinningFactions.IsEmpty());
         }

         if (playerManager)
         {
             SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
             if (pc)
             {
                 m_iPlayerId = pc.GetPlayerId();

                 SCR_FactionManager scrFactionManager = SCR_FactionManager.Cast(factionManager);
                 if (scrFactionManager)
                     m_FactionPlayer = scrFactionManager.GetLocalPlayerFaction();
                 else
                     m_FactionPlayer = null;
             }
         }

         //Get the type of game over using given data
         m_iEndGameType = GetGameOverType(m_iEndReason, m_iPlayerId, m_aWinningPlayers, m_FactionPlayer, m_aWinningFactions, m_bIsFactionVictory);
     }

     //------------------------------------------------------------------------------------------------
  protected EGameOverTypes GetGameOverType(int endReason, int playerId, array<int> winningPlayers, Faction factionPlayer, array<Faction> factionsVictor, bool isFactionVictory)
     {

         //-1: ENDREASON_UNDEFINED: Undefined
         if (endReason == EGameOverTypes.ENDREASON_UNDEFINED)
         {
             return EGameOverTypes.NEUTRAL;
         }
         //-2: ENDREASON_TIMELIMIT: Time limit reached
         else if (endReason == EGameOverTypes.ENDREASON_TIMELIMIT)
         {
             if (isFactionVictory)
             {
                 if (!factionPlayer)
                     return EGameOverTypes.FACTION_NEUTRAL;
                 else if (factionsVictor.Contains(factionPlayer))
                     return EGameOverTypes.FACTION_VICTORY_TIME;
                 else
                     return EGameOverTypes.FACTION_DEFEAT_TIME;
             }
             else
             {
                 if (winningPlayers.Contains(playerId))
                     return EGameOverTypes.DEATHMATCH_VICTORY_TIME;
                 else
                     return EGameOverTypes.DEATHMATCH_DEFEAT_TIME;
             }

         }
         //-3: ENDREASON_SCORELIMIT: Score limit reached
         else if (endReason == EGameOverTypes.ENDREASON_SCORELIMIT)
         {
             if (isFactionVictory)
             {
                 if (!factionPlayer)
                     return EGameOverTypes.FACTION_NEUTRAL;
                 else if (factionsVictor.Contains(factionPlayer))
                     return EGameOverTypes.FACTION_VICTORY_SCORE;
                 else
                     return EGameOverTypes.FACTION_DEFEAT_SCORE;
             }
             else
             {
                 if (winningPlayers.Contains(playerId))
                     return EGameOverTypes.DEATHMATCH_VICTORY_SCORE;
                 else
                     return EGameOverTypes.DEATHMATCH_DEFEAT_SCORE;
             }
         }
         //-4: ENDREASON_DRAW
         else if (endReason == EGameOverTypes.ENDREASON_DRAW)
         {
             if (isFactionVictory)
             {
                 if (!factionPlayer)
                     return EGameOverTypes.FACTION_NEUTRAL;
                 else
                     return EGameOverTypes.FACTION_DRAW;
             }
             else
             {
                 return EGameOverTypes.DEATHMATCH_DRAW;
             }
         }
         else if (endReason == EGameOverTypes.SERVER_RESTART)
         {
             return EGameOverTypes.SERVER_RESTART;
         }
         //Editor Victory
         else if (endReason == EGameOverTypes.EDITOR_FACTION_VICTORY)
         {
             if (!factionPlayer)
                 return EGameOverTypes.EDITOR_FACTION_NEUTRAL;
             else if (factionsVictor.Contains(factionPlayer))
                 return EGameOverTypes.EDITOR_FACTION_VICTORY;
             else
                 return EGameOverTypes.EDITOR_FACTION_DEFEAT;
         }
         //Editor Draw
         else if (endReason == EGameOverTypes.EDITOR_FACTION_DRAW)
         {
             if (!factionPlayer)
                 return EGameOverTypes.EDITOR_FACTION_NEUTRAL;
             else
                 return EGameOverTypes.EDITOR_FACTION_DRAW;
         }
         else
         {
             return endReason;
         }
     }

     //------------------------------------------------------------------------------------------------
  EGameOverTypes GetCurrentGameOverType()
     {
         return m_iEndGameType;
     }

     //------------------------------------------------------------------------------------------------
  SCR_GameOverScreenUIComponent OpenGameOverScreen(ResourceName contentLayout, SCR_GameModeEndData endData, LocalizedString title = string.Empty, LocalizedString subtitle = string.Empty, LocalizedString debriefing = string.Empty, ResourceName imageTexture = ResourceName.Empty, ResourceName icon = ResourceName.Empty, Color vignetteColor = null, string titleParam = string.Empty, string subtitleParam = string.Empty, string debriefingParam = string.Empty)
     {
         SCR_HUDManagerComponent hudManager = GetGame().GetHUDManager();
         if (!hudManager)
             return null;

         //This is just temporary hopefully. This bypasses a lot of the code here and in other files. If one wants to change the layout, do it in Menu Config.
         MenuBase menu = GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.EndgameScreen, 0, true);

         Widget screen = Widget.Cast(menu.GetRootWidget());

         SCR_GameOverScreenUIComponent screenUIComponent = SCR_GameOverScreenUIComponent.Cast(screen.FindHandler(SCR_GameOverScreenUIComponent));

         if (screenUIComponent)
         {
             SCR_GameOverScreenUIContentData content = new SCR_GameOverScreenUIContentData(contentLayout, endData, title, subtitle, debriefing, imageTexture, icon, vignetteColor, titleParam, subtitleParam, debriefingParam);
             screenUIComponent.InitGameOverScreen(content);
         }

         return screenUIComponent;
     }


     //------------------------------------------------------------------------------------------------
     protected void OnEditorEndGameApplyDelayDone()
     {
         m_bListeningToEditorCalledEndGame = false;

         SCR_BaseGameMode gamemode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if (!gamemode)
             return;

         SCR_BaseGameOverScreenInfo gameOverInfo = m_GameOverScreenConfig.GetGameOverInfo(m_iEditorSetGameOverType);
         if (!gameOverInfo)
         {
             gameOverInfo = m_GameOverScreenConfig.GetGameOverInfo(EGameOverTypes.EDITOR_NEUTRAL);
             //~ Could not find game over so used EDITOR_NEUTRAL
             if (gameOverInfo)
             {
                 m_iEditorSetGameOverType = EGameOverTypes.EDITOR_NEUTRAL;
                 Print(string.Format("SCR_GameOverScreenManagerComponent could not find end screen: %1, so EDITOR_NEUTRAL is used instead", typename.EnumToString(EGameOverTypes, m_iEditorSetGameOverType)), LogLevel.ERROR);
             }
             //~ Could not find EDITOR_NEUTRAL either
             else
             {
                 //~ Try to find NEUTRAL
                 gameOverInfo = m_GameOverScreenConfig.GetGameOverInfo(EGameOverTypes.NEUTRAL);
                 if (gameOverInfo)
                 {
                     m_iEditorSetGameOverType = EGameOverTypes.NEUTRAL;
                     Print(string.Format("SCR_GameOverScreenManagerComponent could not find end screen: %1 and EDITOR_NEUTRAL could not be found so NEUTRAL is used instead", typename.EnumToString(EGameOverTypes, m_iEditorSetGameOverType)), LogLevel.ERROR);
                 }
                 //~ Even NEUTRAL could not be found for some reason
                 else
                 {
                     Print(string.Format("SCR_GameOverScreenManagerComponent could not find end screen: %1 nor EDITOR_NEUTRAL or NEUTRAL. Check if the config is set up correctly!", typename.EnumToString(EGameOverTypes, m_iEditorSetGameOverType)), LogLevel.ERROR);
                     return;
                 }
             }
         }

         SCR_BaseGameOverScreenInfoEditor optionalEditorParam = gameOverInfo.GetEditorOptionalParams();

         if (!optionalEditorParam || !optionalEditorParam.m_bNeedsPlayableFactions)
             gamemode.EndGameMode(SCR_GameModeEndData.CreateSimple(m_iEditorSetGameOverType));
         else
             gamemode.EndGameMode(SCR_GameModeEndData.Create(m_iEditorSetGameOverType, null, m_aEditorSetFactions));

         //Clear vars
         m_aEditorSetFactions.Clear();
         m_iEditorSetGameOverType = EGameOverTypes.EDITOR_NEUTRAL;
     }

     //------------------------------------------------------------------------------------------------
  void SetEditorGameOverType(EGameOverTypes gameOverType)
     {
         m_iEditorSetGameOverType = gameOverType;

         if (!m_bListeningToEditorCalledEndGame)
         {
             m_bListeningToEditorCalledEndGame = true;
             GetGame().GetCallqueue().CallLater(OnEditorEndGameApplyDelayDone, 1);
         }
     }

     //------------------------------------------------------------------------------------------------
  void SetEditorGameOverFactions(notnull array<int> factions)
     {
         m_aEditorSetFactions = factions;

         if (!m_bListeningToEditorCalledEndGame)
         {
             m_bListeningToEditorCalledEndGame = true;
             GetGame().GetCallqueue().CallLater(OnEditorEndGameApplyDelayDone, 1);
         }
     }

     //------------------------------------------------------------------------------------------------
     // When game ends
     override void OnGameModeEnd(SCR_GameModeEndData data)
     {
         //If called on init HUD Manager might not exist so call it with a delay
         if (!GetGame().GetHUDManager())
         {
             GetGame().GetCallqueue().CallLater(OnInitDelayedEndGame, 1000);
             return;
         }
         StartEndGameFade();
     }

     //------------------------------------------------------------------------------------------------
     protected void OnInitDelayedEndGame()
     {
         StartEndGameFade();
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
     }
 }

 class SCR_GameOverScreenUIContentData
 {
     ResourceName m_sGameOverLayout;
     ref SCR_GameModeEndData m_EndGameData;
     LocalizedString m_sTitle;
     LocalizedString m_sSubtitle;
     LocalizedString m_sDebriefing;
     ResourceName m_sImageTexture;
     ResourceName m_sIcon;
     Color m_cVignetteColor;
     string m_sTitleParam;
     string m_sSubtitleParam;
     string m_sDebriefingParam;

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_GameOverScreenUIContentData(ResourceName gameOverLayout, SCR_GameModeEndData endGameData, LocalizedString title = string.Empty, LocalizedString subtitle = string.Empty, LocalizedString debriefing = string.Empty, ResourceName imageTexture = ResourceName.Empty, ResourceName icon = ResourceName.Empty, Color vignetteColor = null, string titleParam = string.Empty, string subtitleParam = string.Empty, string debriefingParam = string.Empty)
     {
         m_sGameOverLayout = gameOverLayout;
         m_EndGameData = endGameData;
         m_sTitle = title;
         m_sSubtitle = subtitle;
         m_sDebriefing = debriefing;
         m_sImageTexture = imageTexture;
         m_sIcon = icon;
         m_cVignetteColor = vignetteColor;
         m_sTitleParam = titleParam;
         m_sSubtitleParam = subtitleParam;
         m_sDebriefingParam = debriefingParam;
     }
 }