 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_MapModuleBase : Managed
 {
     [Attribute("0", UIWidgets.Auto, "Disable this module, useful for example when we want to inherit config without a specific module" )]
     protected bool m_bDisableModule;

     protected Widget m_wRootWidget;
     protected SCR_MapEntity m_MapEntity;

     //------------------------------------------------------------------------------------------------
  protected void OnMapOpen(MapConfiguration config)
     {}

     //------------------------------------------------------------------------------------------------
  protected void OnMapClose(MapConfiguration config)
     {}

     //------------------------------------------------------------------------------------------------
  bool IsConfigDisabled()
     {
         return m_bDisableModule;
     }

     //------------------------------------------------------------------------------------------------
  void SetActive(bool active, bool isCleanup = false)
     {
         if (active)
         {
             m_wRootWidget = m_MapEntity.GetMapConfig().RootWidgetRef; // Needs to be refreshed here

             m_MapEntity.GetOnMapOpen().Insert(OnMapOpen);
             m_MapEntity.GetOnMapClose().Insert(OnMapClose);
         }
         else
         {
             m_MapEntity.GetOnMapOpen().Remove(OnMapOpen);
             m_MapEntity.GetOnMapClose().Remove(OnMapClose);

             if (!isCleanup)
                 m_MapEntity.DeactivateModule(this);
         }
     }

     //------------------------------------------------------------------------------------------------
  void Init()
     {}

     //------------------------------------------------------------------------------------------------
  void Update(float timeSlice)
     {}

     //------------------------------------------------------------------------------------------------
  void SCR_MapModuleBase()
     {
         m_MapEntity = SCR_MapEntity.GetMapInstance();
     }
 };