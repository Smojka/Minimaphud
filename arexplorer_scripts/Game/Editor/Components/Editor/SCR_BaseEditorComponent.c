 [ComponentEditorProps(category: "GameScripted/Editor", insertable: false)]
 class SCR_BaseEditorComponentClass: ScriptComponentClass
 {
     [Attribute(category: "Effects")]
     private ref array<ref SCR_BaseEditorEffect> m_EffectsActivate;

     [Attribute(category: "Effects")]
     private ref array<ref SCR_BaseEditorEffect> m_EffectsDeactivate;

     //------------------------------------------------------------------------------------------------
     array<ref SCR_BaseEditorEffect> GetEffectsActivate()
     {
         return m_EffectsActivate;
     }

     //------------------------------------------------------------------------------------------------
     array<ref SCR_BaseEditorEffect> GetEffectsDeactivate()
     {
         return m_EffectsDeactivate;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_BaseEditorComponentClass GetInstance(typename type, bool showError)
     {
         SCR_BaseEditorComponent component = SCR_BaseEditorComponent.Cast(SCR_BaseEditorComponent.GetInstance(type, showError));
         if (component)
             return SCR_BaseEditorComponentClass.Cast(component.GetEditorComponentData());
         else
             return null;
     }
 };


 class SCR_BaseEditorComponent : ScriptComponent
 {
     protected SCR_EditorBaseEntity m_Owner;
     protected SCR_EditorManagerEntity m_Manager;
     protected SCR_BaseEditorComponent m_Parent;
     protected RplComponent m_RplComponent;
     private bool m_bOpen;

     private ref ScriptInvoker m_OnEffect;


     //------------------------------------------------------------------------------------------------
  protected void EOnEditorInit();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorDelete();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorRequest(bool isOpen);

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorOpen();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorClose();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorPreActivate();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorActivate();

     //------------------------------------------------------------------------------------------------
  protected bool EOnEditorActivateAsync(int attempt)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorPostActivate();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorDeactivate();

     //------------------------------------------------------------------------------------------------
  protected bool EOnEditorDeactivateAsync(int attempt)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorPostDeactivate();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorDebug(array<string> debugTexts);

     //------------------------------------------------------------------------------------------------
  protected void EOnEffect(SCR_BaseEditorEffect effect);


     //------------------------------------------------------------------------------------------------
  protected void EOnEditorInitServer();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorDeleteServer();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorOpenServer();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorOpenServerCallback();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorCloseServer();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorCloseServerCallback();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorActivateServer();

     //------------------------------------------------------------------------------------------------
  protected void EOnEditorDeactivateServer();

     //--- Event handlers

     //------------------------------------------------------------------------------------------------
     sealed void OnInitBase()
     {
         EOnEditorInit();
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnDeleteBase()
     {
         EOnEditorDelete();
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnRequestBase(bool isOpen)
     {
         EOnEditorRequest(isOpen);
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnOpenedBase()
     {
         m_bOpen = true;
         EOnEditorOpen();
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnClosedBase()
     {
         m_bOpen = false;
         //OnDeactivateBase();
         EOnEditorClose();
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnPreActivateBase()
     {
         EOnEditorPreActivate();
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnActivateBase()
     {
         Activate(m_Owner);
         //SetEventMask(m_Owner, EntityEvent.FRAME);
         EOnEditorActivate();
     }

     //------------------------------------------------------------------------------------------------
     sealed bool OnActivateAsyncBase(int attempt)
     {
         return EOnEditorActivateAsync(attempt);
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnPostActivateBase()
     {
         EOnEditorPostActivate();

         SCR_BaseEditorComponentClass prefabData = SCR_BaseEditorComponentClass.Cast(GetEditorComponentData());
         if (prefabData)
             SCR_BaseEditorEffect.Activate(prefabData.GetEffectsActivate(), this);
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnDeactivateBase()
     {
         if (!IsActive())
             return;

         //--- Deactivate only when it's not being deleted, so that proper cleanup can be arranged in destructor
         if (!GetOwner().IsDeleted())
             Deactivate(m_Owner);

         EOnEditorDeactivate();

         SCR_BaseEditorComponentClass prefabData = SCR_BaseEditorComponentClass.Cast(GetEditorComponentData());
         if (prefabData)
             SCR_BaseEditorEffect.Activate(prefabData.GetEffectsDeactivate(), this);
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnPostDeactivateBase()
     {
         EOnEditorPostDeactivate();
     }

     //------------------------------------------------------------------------------------------------
     sealed bool OnDeactivateAsyncBase(int attempt)
     {
         return EOnEditorDeactivateAsync(attempt);
     }

     //------------------------------------------------------------------------------------------------
     sealed void OnEffectBase(SCR_BaseEditorEffect effect)
     {
         EOnEffect(effect);
     }

     //--- Custom functions

     //------------------------------------------------------------------------------------------------
  static Managed GetInstance(typename type, bool showError = false, bool modeFirst = false)
     {
         //--- Find the component in Editor Manager entity
         SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
         if (!editorManager)
         {
             if (showError)
                 Print(string.Format("Cannot find editor component '%1', local instance of editor manager not found!", type), LogLevel.ERROR);
             return null;
         }

         Managed component = editorManager.FindComponent(type);
         if (component && !modeFirst)
             return component;

         //--- Find the component in Editor Mode entity
         SCR_EditorModeEntity editorMode = SCR_EditorModeEntity.GetInstance();
         if (editorMode)
         {
             Managed componentMode = editorMode.FindComponent(type);
             if (componentMode)
             {
                 return componentMode;
             }
             else if (showError)
             {
                 Print(string.Format("Cannot find editor component '%1' on local instance of editor manager or on the curent editor mode!", type), LogLevel.ERROR);
             }
         }
         else if (showError)
         {
             Print(string.Format("Cannot find editor component '%1' on local instance of editor manager, and no current editor mode exists!", type), LogLevel.ERROR);
         }
         return component;
     }

     //------------------------------------------------------------------------------------------------
  static Managed GetInstance(SCR_EditorBaseEntity editorManager, typename type, bool showError = false)
     {
         Managed component = editorManager.FindComponent(type);
         if (component)
             return component;

         SCR_EditorModeEntity editorMode = SCR_EditorModeEntity.Cast(editorManager);
         if (!editorMode)
             return null;

         editorManager = editorMode.GetManager();
         if (!editorManager)
             return null;

         return editorManager.FindComponent(type);
     }

     //------------------------------------------------------------------------------------------------
  static int GetAllInstances(typename type, out notnull array<Managed> outComponents)
     {
         outComponents.Clear();

         SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
         if (!editorManager)
             return 0;

         Managed component = editorManager.FindComponent(type);
         if (component)
             outComponents.Insert(component);

         array<SCR_EditorModeEntity> modeEntities = {};
         int modesCount = editorManager.GetModeEntities(modeEntities);
         for (int i = 0; i < modesCount; i++)
         {
             component = modeEntities[i].FindComponent(type);
             if (component)
                 outComponents.Insert(component);
         }
         return outComponents.Count();
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditorManagerEntity GetManager()
     {
         if (!m_Owner)
             return null;

         SCR_EditorManagerEntity manager = SCR_EditorManagerEntity.Cast(m_Owner);
         if (!manager)
             manager = m_Owner.GetManager();

         return manager;
     }

     //------------------------------------------------------------------------------------------------
  SCR_BaseEditorComponent FindEditorComponent(typename type, bool showError = false, bool modeFirst = false)
     {
         //--- Find the component in Editor Manager entity
         SCR_EditorManagerEntity editorManager = GetManager();
         if (!editorManager)
         {
             if (showError)
                 Print(string.Format("Cannot find editor component '%1', local instance of editor manager not found!", type), LogLevel.ERROR);
             return null;
         }

         SCR_BaseEditorComponent component = SCR_BaseEditorComponent.Cast(editorManager.FindComponent(type));
         if (component && !modeFirst)
             return component;

         //--- Find the component in Editor Mode entity
         SCR_EditorModeEntity editorMode = editorManager.GetCurrentModeEntity();
         if (editorMode)
         {
             SCR_BaseEditorComponent componentMode = SCR_BaseEditorComponent.Cast(editorMode.FindComponent(type));
             if (componentMode)
             {
                 component = componentMode;
             }
             else if (showError && !component)
             {
                 Print(string.Format("Cannot find editor component '%1' on local instance of editor manager or on the curent editor mode!", type), LogLevel.ERROR);
             }
         }
         else if (showError && !component)
         {
             Print(string.Format("Cannot find editor component '%1' on local instance of editor manager, and no current editor mode exists!", type), LogLevel.ERROR);
         }
         return component;
     }

     //------------------------------------------------------------------------------------------------
     EntityComponentPrefabData GetEditorComponentData()
     {
         if (m_Owner)
             return GetComponentData(m_Owner);
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  bool IsRemoved()
     {
         return !m_Owner || m_Owner.IsRemoved();
     }

     //------------------------------------------------------------------------------------------------
  void ResetEditorComponent();

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnEffect()
     {
         if (!m_OnEffect)
             m_OnEffect = new ScriptInvoker();

         return m_OnEffect;
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsOnEditorManager()
     {
         return m_Owner && m_Owner.IsInherited(SCR_EditorManagerEntity);
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsOwner()
     {
         return m_RplComponent && m_RplComponent.IsOwner();
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsProxy()
     {
         return m_RplComponent && m_RplComponent.IsProxy();
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsMaster()
     {
         return m_RplComponent && m_RplComponent.IsMaster();
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsAdmin()
     {
         //--- ToDo: Proper admin detection once admin feature is implemented
         return SCR_Global.IsAdmin(GetManager().GetPlayerID()) && Replication.IsRunning();
     }

     //------------------------------------------------------------------------------------------------
     protected SCR_BaseEditorComponent GetParentComponent()
     {
         return m_Parent;
     }

     //------------------------------------------------------------------------------------------------
     protected void SetParentComponent(SCR_BaseEditorComponent parent)
     {
         if (m_Parent)
             return;

         m_Parent = parent;
     }

     //------------------------------------------------------------------------------------------------
     //If target Entity is given then location is used to target position unless: m_bSetLocationOnce is true in the data found in m_aNotificationDisplayInfos (SCR_NotificationManagerEditorComponent on the EditorManager)
     protected void SendNotification(ENotification notificationID, int selfID = 0, int targetID = 0, vector position = vector.Zero)
     {
         //Send notification
         SCR_NotificationsComponent.SendLocal(notificationID, position, selfID, targetID);
     }

     //------------------------------------------------------------------------------------------------
  void InitServer()
     {
         if (!m_Owner)
             return;

         if (m_Owner.GetOnOpenedServer())
             m_Owner.GetOnOpenedServer().Insert(EOnEditorOpenServer);

         if (m_Owner.GetOnOpenedServerCallback())
             m_Owner.GetOnOpenedServerCallback().Insert(EOnEditorOpenServerCallback);

         if (m_Owner.GetOnActivateServer())
             m_Owner.GetOnActivateServer().Insert(EOnEditorActivateServer);

         if (m_Owner.GetOnDeactivateServer())
             m_Owner.GetOnDeactivateServer().Insert(EOnEditorDeactivateServer);

         if (m_Owner.GetOnClosedServer())
             m_Owner.GetOnClosedServer().Insert(EOnEditorCloseServer);

         if (m_Owner.GetOnClosedServerCallback())
             m_Owner.GetOnClosedServerCallback().Insert(EOnEditorCloseServerCallback);

         EOnEditorInitServer();
     }

     //------------------------------------------------------------------------------------------------
  void InitOwner()
     {
         if (!m_Owner)
             return;

         //if (m_Owner.GetOnInit()) m_Owner.GetOnInit().Insert(OnInitBase);
         //if (m_Owner.GetOnRequest()) m_Owner.GetOnRequest().Insert(OnRequestBase);
         //if (m_Owner.GetOnOpened()) m_Owner.GetOnOpened().Insert(OnOpenedBase);
         //if (m_Owner.GetOnPreActivate()) m_Owner.GetOnPreActivate().Insert(OnPreActivateBase);
         //if (m_Owner.GetOnActivate()) m_Owner.GetOnActivate().Insert(OnActivateBase);
         //if (m_Owner.GetOnPostActivate()) m_Owner.GetOnPostActivate().Insert(OnPostActivateBase);
         //if (m_Owner.GetOnDeactivate()) m_Owner.GetOnDeactivate().Insert(OnDeactivateBase);
         //if (m_Owner.GetOnClosed()) m_Owner.GetOnClosed().Insert(OnClosedBase);
         if (m_Owner.GetOnDebug())
             m_Owner.GetOnDebug().Insert(EOnEditorDebug);

         m_Manager = SCR_EditorManagerEntity.Cast(m_Owner);
         if (!m_Manager)
             m_Manager = m_Owner.GetManager();

 //      if (m_Owner.IsOpened())
 //      {
 //          OnInitBase();
 //          OnOpenedBase();
 //      }
     }

     //--- Default functions

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         if (!m_Owner)
             return;

         m_RplComponent = RplComponent.Cast(m_Owner.FindComponent(RplComponent)); //--- Must be here, it's too early in the constructor
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_BaseEditorComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
         if (SCR_Global.IsEditMode(ent)) //--- Run-time only
             return;

         m_Owner = SCR_EditorBaseEntity.Cast(ent);
         if (!m_Owner)
         {
             Print("SCR_BaseEditorComponent must be attached to SCR_EditorBaseEntity!", LogLevel.ERROR);
             return;
         }

         Deactivate(null);
     }

     //------------------------------------------------------------------------------------------------
     // destructor
     void ~SCR_BaseEditorComponent()
     {
         if (Replication.IsServer())
             EOnEditorDeleteServer();
     }
 }