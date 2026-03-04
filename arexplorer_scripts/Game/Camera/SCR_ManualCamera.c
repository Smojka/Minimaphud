 [EntityEditorProps(category: "GameScripted/Camera", description: "Manual camera", color: "0 255 255 255")]
 class SCR_ManualCameraClass: SCR_CameraBaseClass
 {
 };



 class SCR_ManualCamera : SCR_CameraBase
 {
     [Attribute("1", UIWidgets.Auto, "True to enable manual control.", category: "Manual Camera")]
     private bool m_bInputEnabled;

     [Attribute("0", UIWidgets.Auto, "True to create the camera at the position of this entity.", category: "Manual Camera")]
     private bool m_bStartAtEntity;

     [Attribute("ManualCameraContext", UIWidgets.Auto, "", category: "Manual Camera")]
     private string m_sActionContext;

     [Attribute("", UIWidgets.Flags, enums: ParamEnumArray.FromEnum(EManualCameraFlag), category: "Manual Camera")]
     private EManualCameraFlag m_DefaultFlags;

     [Attribute(category: "Manual Camera Components")]
     private ref array<ref SCR_BaseManualCameraComponent> m_aComponents;

     //--- Variables
     private BaseWorld m_World;
     private CameraManager m_CameraManager;
     private InputManager m_InputManager;
     private MenuManager m_MenuManager;
     private IEntity m_Owner;
     private ref SCR_SortedArray<SCR_BaseManualCameraComponent> m_aComponentsSorted = new SCR_SortedArray<SCR_BaseManualCameraComponent>;
     private ref array<SCR_BaseManualCameraComponent> m_aComponentsOnFrame = new array<SCR_BaseManualCameraComponent>;
     private int m_iComponentsCount;
     private int m_iComponentsOnFrameCount;
     private float m_fFOVDefault = 1;
     private ref SCR_ManualCameraParam m_Param;
     private ref ScriptInvoker Event_OnCursorPos;
     private vector m_vTransformPrev[4];
     private bool m_bIsDirtyExternal;
     private Widget m_Widget;

     //--- Custom methods

     //------------------------------------------------------------------------------------------------
     protected void RegisterComponents()
     {
         if (!m_aComponents)
             return;

         //--- Get camera components
         foreach (SCR_BaseManualCameraComponent cameraComponent: m_aComponents)
         {
             if (cameraComponent.IsEnabled())
                 m_aComponentsSorted.Insert(cameraComponent.GetPriority(), cameraComponent);
         }
         m_iComponentsCount = m_aComponentsSorted.Count();
     }

     //------------------------------------------------------------------------------------------------
     protected void InitComponents()
     {
         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             if (m_aComponentsSorted[i].InitBase(this))
                 m_aComponentsOnFrame.Insert(m_aComponentsSorted[i]);
         }
         m_iComponentsOnFrameCount = m_aComponentsOnFrame.Count();
     }

     //------------------------------------------------------------------------------------------------
     protected void ProcessComponents(float timeSlice = 0)
     {
         vector transform[4];
         //GetWorldCameraTransform(transform);
         GetLocalTransform(transform);
         bool inputEnabled = m_bInputEnabled && !IsDisabledByMenu();
         vector velocity = vector.Zero;
         if (m_Param) velocity = m_Param.velocityOriginal;
         m_Param = new SCR_ManualCameraParam(transform, timeSlice, m_World, GetVerticalFOV(), inputEnabled, velocity, m_DefaultFlags);

         //--- External influence
         m_Param.isDirtyExternal = m_bIsDirtyExternal;
         m_bIsDirtyExternal = false;

         //--- Mark first call as dirty to make sure that all features refresh
         if (timeSlice == 0)
             m_Param.isDirty = true;

         //--- Activate context
         if (inputEnabled)
         {
             m_InputManager.ActivateContext(m_sActionContext);
             //m_InputManager.SetContextDebug(m_sActionContext, true);
         }

         //--- Iterate through all components (ToDo: Don't check for IsEnabled here, hold array with just enabled components)
         SCR_BaseManualCameraComponent component;
         for (int c = 0; c < m_iComponentsOnFrameCount; c++)
         {
             component = m_aComponentsOnFrame[c];
             if (component.IsEnabled() && component.HasFlag(m_Param.flag))
                 component.EOnCameraFrame(m_Param);
         }

         //-- Show on-screen debug
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_MANUAL_CAMERA_DEBUG))
             m_Param.ShowDebug(this);

         //--- Apply changes
         if (m_Param.isDirty)
         {
             m_Param.ProcessTransform();

             SetLocalTransform(m_Param.transform);
             SetVerticalFOV(m_Param.fov);

             m_vTransformPrev = m_Param.transform;
         }
         m_Param.velocityOriginal = m_Param.transform[3] - m_Param.transformOriginal[3];
     }

     //------------------------------------------------------------------------------------------------
  void SaveComponents(notnull array<ref SCR_ManualCameraComponentSave> outData)
     {
         SCR_ManualCameraComponentSave save;
         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             if (!m_aComponentsSorted[i].IsEnabled())
                 continue;

             save = new SCR_ManualCameraComponentSave();
             m_aComponentsSorted[i].EOnCameraSave(save);

             if (save.m_aValues && !save.m_aValues.IsEmpty())
             {
                 save.m_sTypeName = m_aComponentsSorted[i].Type().ToString();

                 int d = 0;
                 int dataCount = outData.Count();
                 while (d < dataCount)
                 {
                     //~ Check if data already exists. If it does, replace it. If not insert it at the end
                     if (outData[d].m_sTypeName == save.m_sTypeName)
                     {
                         outData.RemoveOrdered(d);
                         break;
                     }
                     d++;
                 }

                 outData.InsertAt(save, d);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  void LoadComponents(array<ref SCR_ManualCameraComponentSave> data)
     {
         typename type;
         foreach (SCR_ManualCameraComponentSave dataEntry: data)
         {
             type = dataEntry.m_sTypeName.ToType();
             for (int i = m_iComponentsCount - 1; i >= 0; i--)
             {
                 if (m_aComponentsSorted[i].Type() == type && m_aComponentsSorted[i].IsEnabled())
                     m_aComponentsSorted[i].EOnCameraLoad(dataEntry);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  void ResetComponents()
     {
         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             if (m_aComponentsSorted[i].IsEnabled())
                 m_aComponentsSorted[i].EOnCameraReset();
         }
     }
     //------------------------------------------------------------------------------------------------
  SCR_BaseManualCameraComponent FindCameraComponent(typename type)
     {
         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             if (m_aComponentsSorted[i].Type() == type)
                 return m_aComponentsSorted[i];
         }
         return null;
     }

     //------------------------------------------------------------------------------------------------
     protected void MoveToCamera(CameraBase camera = null)
     {
         if (!camera) camera = GetPreviousCamera();
         if (!camera) return;
         vector matrix[4];
         camera.GetTransform(matrix);
         SetTransform(matrix);
     }

     //------------------------------------------------------------------------------------------------
     protected CameraBase GetPreviousCamera()
     {
         if (!m_CameraManager) return null;

         //--- Get all registered cameras
         array<CameraBase> cameras = new array<CameraBase>();
         m_CameraManager.GetCamerasList(cameras);

         //--- Get the most recently registered camera
         for (int i = cameras.Count() - 1; i >= 0; i--)
         {
             if (cameras[i] != this) return cameras[i];
         }
         return null;
     }

     //------------------------------------------------------------------------------------------------
  bool TrySwitchToControlledEntityCamera()
     {
         //--- Don't switch when this camera is not active
         if (!m_CameraManager || m_CameraManager.CurrentCamera() != this)
             return false;

         // We primarily want the player to return to his controlled entity camera
         CameraBase camera = GetGame().GetPlayerController().GetPlayerCamera();

         // Only if that fails we just use the next best previously created camera
         if (!camera)
             camera = GetPreviousCamera();

         if (camera)
         {
             m_CameraManager.SetCamera(camera);
             return true;
         }

         return false;

     }

     //------------------------------------------------------------------------------------------------
  void SwitchToPreviousCamera()
     {
         //--- Don't switch when this camera is not current
         if (!m_CameraManager || m_CameraManager.CurrentCamera() != this)
             return;

         CameraBase camera = GetPreviousCamera();
         if (camera)
             m_CameraManager.SetCamera(camera);
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsFirstCamera()
     {
         if (!m_CameraManager)
             return false;

         array<CameraBase> cameras = new array<CameraBase>();
         m_CameraManager.GetCamerasList(cameras);

         int count = cameras.Count();
         if (count == 0)
             return true;

         if (count == 1)
             return cameras[0] == this; //--- When this camera is already registered, return true when it's the only one

         return false;
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsDisabledByMenu()
     {
         if (!m_MenuManager) return false;

         if (m_MenuManager.IsAnyDialogOpen()) return true;

         MenuBase topMenu = m_MenuManager.GetTopMenu();
         return topMenu && !topMenu.IsInherited(EditorMenuUI);
     }

     //------------------------------------------------------------------------------------------------
  void AttachTo(IEntity parent)
     {
         if (parent == GetParent()) return;

         vector transform[4];
         GetWorldTransform(transform);
         parent.AddChild(this, 0);
         SetWorldTransform(transform);

         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             m_aComponentsSorted[i].EOnCameraParentChange(true, parent);
         }
     }

     //------------------------------------------------------------------------------------------------
  void Detach()
     {
         IEntity parent = GetParent();
         if (!parent) return;

         vector transform[4];
         GetWorldTransform(transform);
         parent.RemoveChild(this);
         SetWorldTransform(transform);

         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             m_aComponentsSorted[i].EOnCameraParentChange(false, parent);
         }
     }

     //------------------------------------------------------------------------------------------------
  void SetInputEnabled(bool enable)
     {
         m_bInputEnabled = enable;
     }

     //------------------------------------------------------------------------------------------------
  bool IsInputEnabled()
     {
         return m_bInputEnabled;
     }

     //------------------------------------------------------------------------------------------------
  float GetDefaultFOV()
     {
         return m_fFOVDefault;
     }

     //------------------------------------------------------------------------------------------------
  bool IsManualInput()
     {
         return m_Param.isManualInput;
     }

     //------------------------------------------------------------------------------------------------
  void SetDirty(bool isDirty)
     {
         m_bIsDirtyExternal = isDirty;
     }

     //------------------------------------------------------------------------------------------------
     Widget GetWidget()
     {
         return m_Widget;
     }

     //------------------------------------------------------------------------------------------------
     Widget CreateCameraWidget(ResourceName layout, bool isVisible = true)
     {
         if (!m_Widget || layout.IsEmpty()) return null;

         Widget widget = GetGame().GetWorkspace().CreateWidgets(layout, m_Widget);
         if (!widget) return null;

         widget.SetVisible(isVisible);
         return widget;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ManualCameraParam GetCameraParam()
     {
         return m_Param;
     }


     //------------------------------------------------------------------------------------------------
  void Terminate()
     {
         //--- Already marked as destroyed
         if (m_iComponentsCount == -1) return;

         //--- Terminate components
         for (int i = m_iComponentsCount - 1; i >= 0; i--)
         {
             m_aComponentsSorted[i].EOnCameraExit();
         }
         m_iComponentsCount = -1;
         //DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_MANUAL_CAMERA);
         //DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_MANUAL_CAMERA_DEBUG);
     }

     //--- Default functions

     //------------------------------------------------------------------------------------------------
     override protected void EOnPostFrame(IEntity owner, float timeSlice)
     {
         if (m_CameraManager && m_CameraManager.CurrentCamera() != this)
             return;

         timeSlice = timeSlice / m_World.GetTimeScale();
         ProcessComponents(timeSlice);
     }

     //------------------------------------------------------------------------------------------------
     override protected void EOnInit(IEntity owner)
     {
         if (SCR_Global.IsEditMode(this)) return;

         if (owner.GetFlags() & EntityFlags.EDITOR_ONLY)
         {
             owner.ClearEventMask(EntityEvent.POSTFRAME);
             return;
         }

         ChimeraWorld world = GetGame().GetWorld();
         if (world)
         {
             world.RegisterEntityToBeUpdatedWhileGameIsPaused(this);
         }

         m_Owner = owner;
         if (m_CameraManager) m_CameraManager.SetCamera(this);

         //--- Move to starting position
         if (!m_bStartAtEntity && !IsFirstCamera())
         {
             MoveToCamera()
         }

         //--- Create parent widget
         m_Widget = GetGame().GetWorkspace().CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.IGNORE_CURSOR | WidgetFlags.NOFOCUS, Color.FromInt(Color.WHITE), 0);
         m_Widget.SetName(Type().ToString());
         FrameSlot.SetAnchorMin(m_Widget, 0, 0);
         FrameSlot.SetAnchorMax(m_Widget, 1, 1);
         FrameSlot.SetOffsets(m_Widget, 0, 0, 0, 0);

         InitComponents();

         DiagMenu.RegisterMenu(SCR_DebugMenuID.DEBUGUI_MANUAL_CAMERA, "Manual Camera", "");
         DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_MANUAL_CAMERA_DEBUG, "", "Show debug menu", "Manual Camera", false);

         //--- Initial activation (timeSlice=0 will mark it as dirty to process all components)
         ProcessComponents(0);
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_ManualCamera(IEntitySource src, IEntity parent)
     {
         if (SCR_Global.IsEditMode(this)) return;

         m_MenuManager = GetGame().GetMenuManager();
         m_InputManager = GetGame().GetInputManager();
         m_World = GetWorld();
         m_fFOVDefault = GetVerticalFOV();

         if (m_Widget) m_Widget.RemoveFromHierarchy();

         ArmaReforgerScripted game = GetGame();
         if (!game) return;

         m_CameraManager = game.GetCameraManager();
         //if (!m_CameraManager) return; //--- Camera manager is not required, so the camera can be used for quick debugging in test worlds

         RegisterComponents();

         SetFlags(EntityFlags.ACTIVE);
         SetEventMask(EntityEvent.INIT | EntityEvent.POSTFRAME);

         SetName("ManualCamera");
     }

     //------------------------------------------------------------------------------------------------
     // destructor
     void ~SCR_ManualCamera()
     {
         ChimeraWorld world = GetGame().GetWorld();
         if (world)
         {
             world.UnregisterEntityToBeUpdatedWhileGameIsPaused(this);
         }

         Terminate();
         SwitchToPreviousCamera();
     }
 }