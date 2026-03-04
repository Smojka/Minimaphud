 // Base for 2D map UI components
 [BaseContainerProps()]
 class SCR_MapUIBaseComponent : ScriptedWidgetComponent
 {
     [Attribute("0", UIWidgets.Auto, "Disable this component, useful for example when we want to inherit config without a specific component" )]
     protected bool m_bDisableComponent;

     protected bool m_bHookToRoot = false;                           // determine whether this component is hooked to the root widget for use of ScriptedWidgetEventHandler events
     protected Widget m_RootWidget;                                  // map layout root widget
     protected SCR_MapEntity m_MapEntity;

     //------------------------------------------------------------------------------------------------
     // BASE METHODS
     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
  protected void OnMapOpen(MapConfiguration config)
     {
         if (m_bHookToRoot)
             m_RootWidget.AddHandler(this);
     }

     //------------------------------------------------------------------------------------------------
  protected void OnMapClose(MapConfiguration config)
     {
         if (m_bHookToRoot)
             m_RootWidget.RemoveHandler(this);
     }

     //------------------------------------------------------------------------------------------------
  bool IsConfigDisabled()
     {
         return m_bDisableComponent;
     }

     //------------------------------------------------------------------------------------------------
  void SetActive(bool active, bool isCleanup = false)
     {
         if (active)
         {
             m_RootWidget = m_MapEntity.GetMapConfig().RootWidgetRef; // Needs to be refreshed here

             m_MapEntity.GetOnMapOpen().Insert(OnMapOpen);
             m_MapEntity.GetOnMapClose().Insert(OnMapClose);
         }
         else
         {
             m_MapEntity.GetOnMapOpen().Remove(OnMapOpen);
             m_MapEntity.GetOnMapClose().Remove(OnMapClose);

             if (!isCleanup)
                 m_MapEntity.DeactivateComponent(this);
         }
     }

     //------------------------------------------------------------------------------------------------
  void Init();

     //------------------------------------------------------------------------------------------------
  void Update(float timeSlice);

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_MapUIBaseComponent()
     {
         m_MapEntity = SCR_MapEntity.GetMapInstance();
     }
 }