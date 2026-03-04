 class SCR_CampaignBuildingObstructionExceptionComponentClass : ScriptComponentClass
 {
 }

 class SCR_CampaignBuildingObstructionExceptionComponent : ScriptComponent
 {
     [Attribute("", UIWidgets.ResourceNamePicker, desc: "List of prefab that can colide with this entity on Free Roam building obstruction test.", params: "et")]
     protected ref array<ResourceName> m_aWhiteListPrefabs;

     //------------------------------------------------------------------------------------------------
  bool IsWhitelistEmpty()
     {
         return m_aWhiteListPrefabs.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  bool IsOnWhitelist(ResourceName resName)
     {
         return m_aWhiteListPrefabs.Contains(resName);
     }
 }