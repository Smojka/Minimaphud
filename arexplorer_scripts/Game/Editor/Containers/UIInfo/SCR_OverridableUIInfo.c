 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_OverridableUIInfo : SCR_UIInfo
 {
     //------------------------------------------------------------------------------------------------
  override void SetName(LocalizedString name)
     {
         Name = name;
     }

     //------------------------------------------------------------------------------------------------
  override void SetDescription(LocalizedString description)
     {
         Description = description;
     }

     //------------------------------------------------------------------------------------------------
  void SetIcon(string icon)
     {
         Icon = icon;
     }

     //------------------------------------------------------------------------------------------------
  void SetIconSetName(string iconSetName)
     {
         IconSetName = iconSetName;
     }
 }