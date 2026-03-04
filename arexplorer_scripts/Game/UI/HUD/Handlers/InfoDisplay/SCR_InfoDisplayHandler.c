 [BaseContainerProps()]
 class SCR_InfoDisplayHandler
 {
     protected SCR_InfoDisplay m_OwnerDisplay;

     [Attribute(desc: "Should this Handler be updated every frame?")]
     bool m_bCanUpdate;

     protected bool m_bEnabled;

     //------------------------------------------------------------------------------------------------
     void SetEnabled(bool enabled)
     {
         m_bEnabled = enabled;
     }

     //------------------------------------------------------------------------------------------------
     bool IsEnabled()
     {
         return m_bEnabled;
     }

     //------------------------------------------------------------------------------------------------
  void Initialize(notnull SCR_InfoDisplay display)
     {
         m_OwnerDisplay = display;
         m_bEnabled = true;
     }

     //------------------------------------------------------------------------------------------------
  void OnStart(notnull SCR_InfoDisplay display);

     //------------------------------------------------------------------------------------------------
  void OnStop(notnull SCR_InfoDisplay display);

     //------------------------------------------------------------------------------------------------
  void OnUpdate(float timeSlice);
 }