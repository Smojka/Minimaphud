 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(configRoot: true)]
 class SCR_BaseSelectionMenuDisplay
 {
     protected bool m_bIsOpen;

     void SetDefault();

     void SetContent(array<BaseSelectionMenuEntry> allEntries, array<BaseSelectionMenuEntry> disabledEntries);

     void SetSelection(BaseSelectionMenuEntry selectedEntry, vector selectionInput, float selectionAngle, float minInputMagnitude);

     void SetSelection(BaseSelectionMenuEntry selectedEntry);

     void SetOpen(IEntity owner, bool open) { m_bIsOpen = open; }
 };

 //------------------------------------------------------------------------------------------------
 class SCR_RadialMenuIcons
 {
     static const string RADIALMENU_ICON_EMPTY = "{74DDB9BCF5A40173}UI/Textures/RadialMenu/RadialMenuEmpty.edds";
     static const string RADIALMENU_ICON_DEFAULT_WEAPONSWITCHING = "{74753A21981BB84D}UI/Textures/RadialMenu/RadialMenuDefaultWeapon.edds";
     static const string RADIALMENU_ICON_DEFAULT_AIMESSAGE = "{164112D4DD73C39F}UI/Textures/RadialMenu/RadialMenuDefaultAIMessage.edds";

     //------------------------------------------------------------------------------------------------
  protected static string GetSafeIconPath(string iconPath)
     {
         if (iconPath == string.Empty)
             return RADIALMENU_ICON_EMPTY;

         return iconPath;
     }

     //------------------------------------------------------------------------------------------------
  static string GetDefaultIconPath(Class instance)
     {
         auto baseEntry = BaseSelectionMenuEntry.Cast(instance);
         if (!baseEntry)
             return RADIALMENU_ICON_EMPTY;

         auto aiMessageEntry = AIMessageSelectionMenuEntry.Cast(instance);
         if (aiMessageEntry)
         {
             return GetSafeIconPath(RADIALMENU_ICON_DEFAULT_AIMESSAGE);
         }

         return RADIALMENU_ICON_EMPTY;
     }
 };