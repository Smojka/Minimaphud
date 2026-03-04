 //------------------------------------------------------------------------------------------------
 class SCR_VONEntry : SCR_SelectionMenuEntry
 {
     protected bool m_bIsUsable = true;  // whether this entry is allowed to be used (f.e. radio turned off)
     protected bool m_bIsActive;     // this entry will be used in case of outgoing transmission
     protected bool m_bIsSelected;   // this entry is currently selected/hovered in a menu
     protected string m_sText;       // current display text

     //------------------------------------------------------------------------------------------------
     // Init entry data
     void InitEntry()
     {}

     //------------------------------------------------------------------------------------------------
  void SetUsable(bool state)
     {
         m_bIsUsable = state;
     }

     //------------------------------------------------------------------------------------------------
  bool IsUsable()
     {
         return m_bIsUsable;
     }

     //------------------------------------------------------------------------------------------------
  void SetActive(bool state)
     {
         m_bIsActive = state;
     }

     //------------------------------------------------------------------------------------------------
  bool IsActive()
     {
         return m_bIsActive;
     }

     //------------------------------------------------------------------------------------------------
  void SetSelected(bool state)
     {
         m_bIsSelected = state;
     }

     //------------------------------------------------------------------------------------------------
  void AdjustEntry(int modifier)
     {}

     //------------------------------------------------------------------------------------------------
  void AdjustEntryModif(int modifier)
     {}

     //------------------------------------------------------------------------------------------------
  void ToggleEntry()
     {}

     //------------------------------------------------------------------------------------------------
  string GetDisplayText()
     {
         return m_sText;
     }

     //------------------------------------------------------------------------------------------------
  string GetIconResource()
     {
         return string.Empty;
     }

     //------------------------------------------------------------------------------------------------
  ECommMethod GetVONMethod()
     {
         return ECommMethod.DIRECT;
     }

     //------------------------------------------------------------------------------------------------
  override void Update()
     {
     }
 };