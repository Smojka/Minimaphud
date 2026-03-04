 void ScriptInvoker_SaveManagerCoreMethod(ESaveType type, string fileName);
 typedef func ScriptInvoker_SaveManagerCoreMethod;
 typedef ScriptInvokerBase<ScriptInvoker_SaveManagerCoreMethod> ScriptInvoker_SaveManagerCore;

 [BaseContainerProps(configRoot: true)]
 class SCR_SaveManagerCore: SCR_GameCoreBase
 {
     protected const string GAME_SESSION_STORAGE_FILE_NAME_TO_LOAD = "SCR_SaveFileManager_FileNameToLoad";
     protected const string GAME_SESSION_STORAGE_USED_CLI = "SCR_SaveFileManager_UsedCLI";
     protected const string CLI_PARAM = "loadSessionSave";

     [Attribute()]
     protected ref array<ref SCR_DSSessionCallback> m_aCallbacks;

     [Attribute()]
     protected ref SCR_LatestSaveDSSessionCallback m_LatestSaveCallback;

     protected ref SCR_DSSessionCallbackSessionStorage m_SessionStorageCallback;

     protected string m_sMissionSaveFileName;
     protected ref SCR_MissionHeader m_WorkbenchMissionHeader;

     protected ref ScriptInvoker_SaveManagerCore m_OnSaved = new ScriptInvoker_SaveManagerCore();
     protected ref ScriptInvoker_SaveManagerCore m_OnLoaded = new ScriptInvoker_SaveManagerCore();
     protected ref ScriptInvoker_SaveManagerCore m_OnDeleted = new ScriptInvoker_SaveManagerCore();
     protected ref ScriptInvokerString m_OnLatestSave = new ScriptInvokerString();

     protected bool m_bLoadedOnInit;
     protected bool m_bDebugDelete;
     protected ref SCR_ServerSaveRequestCallback m_UploadCallback;
     protected ref SCR_SaveManager_BackendCallback m_DownloadCallback;
     protected ref SCR_SaveManager_PageParams m_DownloadPageParams;


     bool Save(ESaveType type, string customName = string.Empty)
     {
         SCR_DSSessionCallback callback = FindCallback(type);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot save, no rules found for save type %1! Check configuration of SCR_SaveLoadComponent on game mode.", typename.EnumToString(ESaveType, type)), LogLevel.WARNING);
             return false;
         }

         return callback.SaveSession(m_sMissionSaveFileName, customName);
     }

     //----------------------------------------------------------------------------------------
  bool OverrideCurrentSave(ESaveType type)
     {
         SCR_DSSessionCallback callback = FindCallback(type);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot override current save, no rules found for save type %1! Check configuration of SCR_SaveLoadComponent on game mode.", typename.EnumToString(ESaveType, type)), LogLevel.WARNING);
             return false;
         }

         string customName = callback.GetCurrentCustomName();
         return customName && callback.SaveSession(m_sMissionSaveFileName, customName);
     }

     //----------------------------------------------------------------------------------------
  bool Load(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot load save file '%1', no rules found for it!", fileName), LogLevel.WARNING);
             return false;
         }

         return callback.LoadSession(fileName);
     }

     //----------------------------------------------------------------------------------------
  bool RestartAndLoad()
     {
         string latestSaveFileName;
         return FindLatestSave(m_sMissionSaveFileName, latestSaveFileName) && RestartAndLoad(latestSaveFileName);
     }

     //----------------------------------------------------------------------------------------
  bool RestartAndLoad(ESaveType type, string customName = string.Empty)
     {
         SCR_DSSessionCallback callback = FindCallback(type);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot restart and load, no rules found for save type %1! Check configuration of SCR_SaveLoadComponent on game mode.", typename.EnumToString(ESaveType, type)), LogLevel.WARNING);
             return false;
         }

         string fileName = callback.GetFileName(m_sMissionSaveFileName, customName);
         return RestartAndLoad(fileName);
     }

     //----------------------------------------------------------------------------------------
  bool RestartAndLoad(string fileName)
     {
         if (Replication.IsClient())
         {
             UploadToWorkshop(fileName);
             return true;
         }
         else
         {
             //--- Server / SP
             if (!SetFileNameToLoad(fileName))
                 return false;

             GameStateTransitions.RequestScenarioRestart();
             return true;
         }
     }

     //----------------------------------------------------------------------------------------
  bool Delete(ESaveType type, string customName = string.Empty)
     {
         SCR_DSSessionCallback callback = FindCallback(type);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot delete, no rules found for save type %1! Check configuration of SCR_SaveLoadComponent on game mode.", typename.EnumToString(ESaveType, type)), LogLevel.WARNING);
             return false;
         }

         return callback.Delete(m_sMissionSaveFileName, customName);
     }

     //----------------------------------------------------------------------------------------
  bool Delete(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot delete save file '%1', no rules found for it!", fileName), LogLevel.WARNING);
             return false;
         }

         return callback.Delete(fileName);
     }

     //----------------------------------------------------------------------------------------
  bool FileExists(ESaveType type, string customName = string.Empty)
     {
         SCR_DSSessionCallback callback = FindCallback(type);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot check if file exists, no rules found for save type %1! Check configuration of SCR_SaveLoadComponent on game mode.", typename.EnumToString(ESaveType, type)), LogLevel.WARNING);
             return false;
         }

         return callback.FileExists(m_sMissionSaveFileName, customName);
     }

     //----------------------------------------------------------------------------------------
  bool FileExists(string fileName)
     {
         return GetGame().GetBackendApi().GetStorage().CheckFileID(fileName);
     }

     //----------------------------------------------------------------------------------------
  bool CanSaveToCloud()
     {
         return RplSession.Mode() == RplMode.Dedicated && GetGame().GetBackendApi().GetStorage().GetOnlineWritePrivilege();
     }

     //----------------------------------------------------------------------------------------
  SCR_MetaStruct GetMeta(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (!callback)
         {
             Print(string.Format("SCR_SaveManagerCore: Cannot load meta of save file '%1', no rules found for it!", fileName), LogLevel.WARNING);
             return null;
         }

         return callback.GetMeta(fileName);
     }


     int GetLocalSaveFiles(out notnull array<string> outLocalSaves, ESaveType saveTypes, bool currentMissionOnly)
     {
         string missionFileName;
         if (currentMissionOnly)
             missionFileName = m_sMissionSaveFileName;

         return GetLocalSaveFiles(outLocalSaves, saveTypes, missionFileName);
     }
     int GetLocalSaveFiles(out notnull array<string> outLocalSaves, ESaveType saveTypes, string missionFileName = string.Empty)
     {
         for (int i = GetGame().GetBackendApi().GetStorage().AvailableSaves(outLocalSaves) - 1; i >= 0; i--)
         {
             //--- Filter out downloaded files
             if (IsDownloaded(outLocalSaves[i]))
             {
                 outLocalSaves.Remove(i);
                 continue;
             }

             SCR_DSSessionCallback callback = FindCallback(outLocalSaves[i]);
             if (!callback || !(saveTypes & callback.GetSaveType()) || (missionFileName && callback.GetMissionFileName(outLocalSaves[i]) != missionFileName))
                 outLocalSaves.Remove(i);
         }
         return outLocalSaves.Count();
     }

     protected bool IsDownloaded(string fileName)
     {
         string ext;
         FilePath.StripExtension(fileName, ext);
         return ext.Contains("_"); //--- Downloaded files have GUID added at the end, e.g., "MissionName-CustomName.save_5D82C234B9132BBC"
     }


     bool CanSave(ESaveType type)
     {
         if (!m_sMissionSaveFileName || !FindCallback(type))
             return false;

         SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         return !gameMode || gameMode.GetState() == SCR_EGameModeState.GAME;
     }

     //----------------------------------------------------------------------------------------
  void SetStruct(ESaveType type, SCR_MissionStruct struct)
     {
         foreach (SCR_DSSessionCallback callback: m_aCallbacks)
         {
             if (callback.GetSaveType() == type)
             {
                 callback.SetStruct(struct);
                 return;
             }
         }
     }

     //----------------------------------------------------------------------------------------
  void Log(ESaveType type)
     {
         SCR_DSSessionCallback callback = FindCallback(type);
         if (callback)
             callback.Log();
     }

     //----------------------------------------------------------------------------------------
  string GetMissionFileName(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (callback)
             return callback.GetMissionFileName(fileName);
         else
             return string.Empty;
     }

     //----------------------------------------------------------------------------------------
  string GetCustomName(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (callback)
             return callback.GetCustomName(fileName);
         else
             return string.Empty;
     }

     //----------------------------------------------------------------------------------------
  SCR_UIInfo GetSaveTypeInfo(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (callback)
             return callback.GetInfo();
         else
             return null;
     }

     //----------------------------------------------------------------------------------------
     protected SCR_DSSessionCallback FindCallback(ESaveType type)
     {
         foreach (SCR_DSSessionCallback callback: m_aCallbacks)
         {
             if (callback.GetSaveType() == type && callback.IsConfigured())
                 return callback;
         }
         return null;
     }

     //----------------------------------------------------------------------------------------
     protected SCR_DSSessionCallback FindCallback(string fileName)
     {
         foreach (SCR_DSSessionCallback callback: m_aCallbacks)
         {
             if (callback.IsCompatible(fileName))
                 return callback;
         }
         return null;
     }

     // Workshop WIP
  void UploadToWorkshop(string fileName)
     {
         SCR_DSSessionCallback callback = FindCallback(fileName);
         if (!callback)
             return;

         m_UploadCallback = new SCR_ServerSaveRequestCallback(fileName);//, callback.GetStruct());
     }

     //----------------------------------------------------------------------------------------
     void DownloadFromWorkshop()
     {
         m_DownloadCallback = new SCR_SaveManager_BackendCallback();

         m_DownloadPageParams = new SCR_SaveManager_PageParams();
         m_DownloadPageParams.limit = 50;

         GetGame().GetBackendApi().GetWorldSaveApi().RequestPage(m_DownloadCallback, m_DownloadPageParams, true);
     }

     //----------------------------------------------------------------------------------------
     void OnDownloadFromWorkshop()
     {
         m_DownloadCallback = null;
         m_DownloadPageParams = null;

         Print("GetPageCount() = " + GetGame().GetBackendApi().GetWorldSaveApi().GetPageCount());
         array<WorldSaveItem> items = {};
         int count = GetGame().GetBackendApi().GetWorldSaveApi().GetPageItems(items);
         foreach (int i, WorldSaveItem item: items)
         {
             PrintFormat("%1: Id=%2, Name='%3', Description='%4'", i, item.Id(), item.Name(), item.Description());

             if (m_bDebugDelete)
                 item.DeleteOnline(null);
         }
     }


     bool SetFileNameToLoad(SCR_MissionHeader missionHeader)
     {
         if (!missionHeader)
             return false;

         //--- Find latest save for the mission
         string latestSaveFileName;
         if (!FindLatestSave(missionHeader.GetSaveFileName(), latestSaveFileName))
             return false;

         return SetFileNameToLoad(latestSaveFileName);
     }

     //----------------------------------------------------------------------------------------
  bool SetFileNameToLoad(string fileName)
     {
         if (!GetGame().GetBackendApi().GetStorage().CheckFileID(fileName))
             return false;

         GameSessionStorage.s_Data.Insert(GAME_SESSION_STORAGE_FILE_NAME_TO_LOAD, fileName);
         Print(string.Format("'%1' set as a save file name to load after world start.", fileName), LogLevel.VERBOSE);
         return true;
     }

     //----------------------------------------------------------------------------------------
  void ResetFileNameToLoad()
     {
         if (GameSessionStorage.s_Data.Contains(GAME_SESSION_STORAGE_FILE_NAME_TO_LOAD))
         {
             GameSessionStorage.s_Data.Remove(GAME_SESSION_STORAGE_FILE_NAME_TO_LOAD);
             Print("Save file name to load after world start removed.", LogLevel.VERBOSE);
         }
     }

     //----------------------------------------------------------------------------------------
  bool FindFileNameToLoad(out string fileNameToLoad)
     {
         return GameSessionStorage.s_Data.Find(GAME_SESSION_STORAGE_FILE_NAME_TO_LOAD, fileNameToLoad);
     }


     void SetCurrentMissionLatestSave(string saveFileName)
     {
         SetLatestSave(m_sMissionSaveFileName, saveFileName);
     }

     //----------------------------------------------------------------------------------------
  void RemoveCurrentMissionLatestSave()
     {
         RemoveLatestSave(m_sMissionSaveFileName);
     }

     //----------------------------------------------------------------------------------------
  bool FindCurrentMissionLatestSave(out string outSaveFileName)
     {
         return FindLatestSave(m_sMissionSaveFileName, outSaveFileName);
     }

     //----------------------------------------------------------------------------------------
  void SetLatestSave(string missionFileName, string saveFileName)
     {
         m_LatestSaveCallback.SetFileName(missionFileName, saveFileName);
     }

     //----------------------------------------------------------------------------------------
  void RemoveLatestSave(string missionFileName)
     {
         m_LatestSaveCallback.RemoveFileName(missionFileName);
     }

     //----------------------------------------------------------------------------------------
  bool FindLatestSave(string missionFileName, out string outSaveFileName)
     {
         return m_LatestSaveCallback.FindFileName(missionFileName, outSaveFileName);
     }

     //----------------------------------------------------------------------------------------
  bool FindLatestSave(SCR_MissionHeader missionHeader, out string outSaveFileName)
     {
         return missionHeader && FindLatestSave(missionHeader.GetSaveFileName(), outSaveFileName);
     }

     //----------------------------------------------------------------------------------------
  bool HasLatestSave(string missionFileName)
     {
         string saveFileName;
         return FindLatestSave(missionFileName, saveFileName);
     }

     //----------------------------------------------------------------------------------------
  bool HasLatestSave(SCR_MissionHeader missionHeader)
     {
         string saveFileName;
         return missionHeader && FindLatestSave(missionHeader, saveFileName);
     }


     //----------------------------------------------------------------------------------------
  ScriptInvoker_SaveManagerCore GetOnSaved()
     {
         return m_OnSaved;
     }

     //----------------------------------------------------------------------------------------
  ScriptInvoker_SaveManagerCore GetOnLoaded()
     {
         return m_OnLoaded;
     }

     //----------------------------------------------------------------------------------------
  ScriptInvoker_SaveManagerCore GetOnDeleted()
     {
         return m_OnDeleted;
     }

     //----------------------------------------------------------------------------------------
  ScriptInvokerString GetOnLatestSave()
     {
         return m_OnLatestSave;
     }

     //----------------------------------------------------------------------------------------
     SCR_ServerSaveRequestCallback GetUploadCallback()
     {
         return m_UploadCallback;
     }


     // Init

     void LoadOnInit()
     {
         //--- Stop if mission save file is not defined (e.g., when mission header is missing) or when on client
         if (m_bLoadedOnInit || !m_sMissionSaveFileName || !Replication.IsServer())
             return;

         //--- Mark as initialized, so nothing will happen in case somebody is crazy/brave/naive enough to call LoadOnInit() again
         m_bLoadedOnInit = true;

         //--- Find save file to be loaded on start
         string fileNameToLoad;
         if (System.IsConsoleApp())
         {
             //--- DEDICATED SERVER

             //--- Check if some file was marked for load, e.g, using client's editor interface. If not, continue with evaluating CLI param.
             if (!FindFileNameToLoad(fileNameToLoad))
             {
                 //--- Terminate when the file marked by the CLI param was loaded before (happens e.g., after calling the #restart command)
                 if (GameSessionStorage.s_Data.Contains(GAME_SESSION_STORAGE_USED_CLI))
                     return;

                 //--- Terminate when the CLI param is not used
                 if (!System.GetCLIParam(CLI_PARAM, fileNameToLoad))
                 {
                     Print(string.Format("SCR_SaveManagerCore: -%1 CLI param not used, no save file is loaded.", CLI_PARAM), LogLevel.VERBOSE);
                     return;
                 }

                 //--- Terminate when the CLI param is marked to load the latest save, but there is none defined
                 if (fileNameToLoad.IsEmpty() && !FindLatestSave(m_sMissionSaveFileName, fileNameToLoad))
                 {
                     Print(string.Format("SCR_SaveManagerCore: -%1 CLI param set to load the latest save, but none was defined for the mission '%2'!", CLI_PARAM, m_sMissionSaveFileName), LogLevel.WARNING);
                     return;
                 }

                 GameSessionStorage.s_Data.Insert(GAME_SESSION_STORAGE_USED_CLI, fileNameToLoad);
             }
         }
         else
         {
             //--- STANDARD GAME

             //--- Show a warning when the CLI param is used incorrectly (otherwise harmless)
             if (System.IsCLIParam(CLI_PARAM))
                 Print(string.Format("SCR_SaveManagerCore: -%1 CLI is intended for dedicated server only!", CLI_PARAM), LogLevel.WARNING);

             //--- Terminate if requested save file does not exist
             if (!FindFileNameToLoad(fileNameToLoad))
                 return;
         }

         //--- Load the file and unmark it, so restarting from pause menu won't load it again
         Load(fileNameToLoad);
         ResetFileNameToLoad();
     }

     //----------------------------------------------------------------------------------------
     protected void InitDebugMissionHeader(out SCR_MissionHeader missionHeader)
     {
         //--- Mission header not found, create a debug one (play mode in World Editor never has a mission header, even when one for the world exists)
         if (!missionHeader && SCR_SaveLoadComponent.GetInstance())
         {
             m_WorkbenchMissionHeader = new SCR_MissionHeader();
             m_WorkbenchMissionHeader.m_sSaveFileName = FilePath.StripPath(FilePath.StripExtension(GetGame().GetWorldFile()));
             m_WorkbenchMissionHeader.m_bIsSavingEnabled = true;
             missionHeader = m_WorkbenchMissionHeader;
         }
     }

     // Default functions
  override void OnUpdate(float timeSlice)
     {
         if (m_sMissionSaveFileName && !System.IsConsoleApp() && GetGame().IsDev())
         {
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_SAVE))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_SAVE, false);

                 ESaveType saveType = 1 << DiagMenu.GetValue(SCR_DebugMenuID.DEBUGUI_SAVING_TYPE);

                 array<string> customNames = {"Alpha", "Kilo", "Zulu"};
                 string customName = customNames.GetRandomElement();
                 Save(saveType, customName);
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_LOG))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_LOG, false);

                 ESaveType saveType = 1 << DiagMenu.GetValue(SCR_DebugMenuID.DEBUGUI_SAVING_TYPE);
                 Log(saveType);
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_LOAD_LATEST))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_LOAD_LATEST, false);

                 string fileName;
                 if (FindLatestSave(m_sMissionSaveFileName, fileName))
                     Load(fileName);
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_RESTART_AND_LOAD_LATEST))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_RESTART_AND_LOAD_LATEST, false);
                 RestartAndLoad();
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_LOG_LATEST))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_LOG_LATEST, false);

                 string latestSaveFileName;
                 if (FindLatestSave(m_sMissionSaveFileName, latestSaveFileName))
                     PrintFormat("The latest save file name for mission '%1' is '%2'", m_sMissionSaveFileName, latestSaveFileName);
                 else
                     PrintFormat("There is no latest save file name for mission '%1'", latestSaveFileName);
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_UPLOAD_LATEST))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_UPLOAD_LATEST, false);

                 string fileName;
                 if (FindLatestSave(m_sMissionSaveFileName, fileName))
                     UploadToWorkshop(fileName);
                 else
                     Print("SCR_SaveManagerCore: Cannot upload, latest save not found!", LogLevel.WARNING);
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_DOWNLOAD))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_DOWNLOAD, false);

                 m_bDebugDelete = false;
                 DownloadFromWorkshop();
             }
             if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_SAVING_DELETE))
             {
                 DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_SAVING_DELETE, false);

                 m_bDebugDelete = true;
                 DownloadFromWorkshop();
             }
         }
     }

     //----------------------------------------------------------------------------------------
     override void OnGameStart()
     {
         //--- Saving is not configured for the current world
         if (!SCR_SaveLoadComponent.GetInstance())
         {
             // Load save immidiatelly - works only when getting back to main menu, not when starting game
             OnGameStorageInitialize();

             if (m_LatestSaveCallback.IsLoaded())
                 return;

             // Wait for init - happens on game start
             m_SessionStorageCallback = new SCR_DSSessionCallbackSessionStorage();
             m_SessionStorageCallback.GetOnInitialize().Insert(OnGameStorageInitialize);
             GetGame().GetBackendApi().SetSessionCallback(m_SessionStorageCallback);

             return;
         }

         //--- Init mission header (use debug one in special conditions)
         SCR_MissionHeader missionHeader = SCR_MissionHeader.Cast(GetGame().GetMissionHeader());

 #ifdef WORKBENCH
         InitDebugMissionHeader(missionHeader);
 #endif

 #ifdef SAVE_MANAGER_DEBUG_HEADER
         InitDebugMissionHeader(missionHeader);
 #endif

         //--- Set mission save file name, but only if saving is enabled
         if (missionHeader && missionHeader.m_bIsSavingEnabled)
             m_sMissionSaveFileName = missionHeader.GetSaveFileName();

         //--- Initialize callbacks
         foreach (SCR_DSSessionCallback callback: m_aCallbacks)
         {
             callback.OnGameStart(m_sMissionSaveFileName);
         }

         //--- Initialize save manager and load marked save file
         m_LatestSaveCallback.ReadFromFile();

         //--- Diag menu init
         if (GetGame().IsDev() && Replication.IsServer() && !System.IsConsoleApp())
         {
             typename enumType = ESaveType;
             string categoryName = "Save Manager";
             DiagMenu.RegisterMenu(SCR_DebugMenuID.DEBUGUI_SAVING, categoryName, "Game");
             DiagMenu.RegisterRange(SCR_DebugMenuID.DEBUGUI_SAVING_TYPE, "", "Type", categoryName, string.Format("0,%1,0,1", enumType.GetVariableCount() - 1));
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_LOG, "", "Log Struct By Type", categoryName);
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_SAVE, "", "Save By Type", categoryName);

             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_LOAD_LATEST, "", "Load Latest Save", categoryName);
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_RESTART_AND_LOAD_LATEST, "", "Restart and Load Latest Save", categoryName);
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_LOG_LATEST, "", "Log Latest Save File Name", categoryName);
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_UPLOAD_LATEST, "", "Upload Latest Save", categoryName);

             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_DOWNLOAD, "", "Download Workshop Saves", categoryName);
             DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_SAVING_DELETE, "", "Delete Workshop Saves", categoryName);
         }
     }

     //----------------------------------------------------------------------------------------
     protected void OnGameStorageInitialize()
     {
         if (SCR_MainMenuEntity.GetInstance())
             m_LatestSaveCallback.ReadFromFile();

         if (m_SessionStorageCallback)
             m_SessionStorageCallback.GetOnInitialize().Remove(OnGameStorageInitialize);
     }

     //----------------------------------------------------------------------------------------
     override void OnGameEnd()
     {
         foreach (SCR_DSSessionCallback callback: m_aCallbacks)
         {
             callback.OnGameEnd(m_sMissionSaveFileName);
         }

         m_bLoadedOnInit = false;
         m_sMissionSaveFileName = string.Empty;
         m_WorkbenchMissionHeader = null;

         //--- Clear invokers are the world end
         m_OnSaved.Clear();
         m_OnLoaded.Clear();
         m_OnDeleted.Clear();

         if (GetGame().IsDev() && Replication.IsServer() && !System.IsConsoleApp())
         {
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_TYPE);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_SAVE);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_LOG);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_LOAD_LATEST);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_RESTART_AND_LOAD_LATEST);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_LOG_LATEST);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_UPLOAD_LATEST);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_DOWNLOAD);
             DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_SAVING_DELETE);
         }
     }
 };

 class SCR_SaveManager_BackendCallback: BackendCallback
 {
     protected ref SCR_SaveManager_PageParams m_DownloadPageParams;

     override void OnError( int code, int restCode, int apiCode )
     {
         PrintFormat("[BackendCallback] OnError: code=%1 ('%4'), restCode=%2, apiCode=%3", code, restCode, apiCode, GetGame().GetBackendApi().GetErrorCode(code));
     }
     override void OnSuccess( int code )
     {
         PrintFormat("[BackendCallback] OnSuccess(): code=%1", code);
         GetGame().GetSaveManager().OnDownloadFromWorkshop();
     }
     override void OnTimeout()
     {
         Print("[BackendCallback] OnTimeout");
     }
 };
 class SCR_SaveManager_PageParams: PageParams
 {
     override void OnPack()
     {
         StoreBoolean("owned", true);
     }
 };

 //----------------------------------------------------------------------------------------
 class SCR_DSSessionCallbackSessionStorage: DSSessionCallback
 {
     protected ref ScriptInvokerVoid m_OnInitialize;

     //----------------------------------------------------------------------------------------
     ScriptInvokerVoid GetOnInitialize()
     {
         if (!m_OnInitialize)
             m_OnInitialize = new ScriptInvokerVoid();

         return m_OnInitialize;
     }

     //----------------------------------------------------------------------------------------
     override void OnInitialize()
     {
         super.OnInitialize();
         m_OnInitialize.Invoke();
     }
 };