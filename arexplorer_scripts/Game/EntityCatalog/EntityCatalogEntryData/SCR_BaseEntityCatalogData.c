
 [BaseContainerProps(), BaseContainerCustomStringTitleField("DO NOT USE BASE CLASS!")]
 class SCR_BaseEntityCatalogData
 {
     [Attribute("1", desc: "Allows to disable the Entity info. SCR_EntityCatalog will ignore the info as if it is null. Used for specific gamemodes and modding.")]
     protected bool m_bEnabled;

     //--------------------------------- Is Enabled ---------------------------------\\

  bool IsEnabled()
     {
         return m_bEnabled;
     }

     //--------------------------------- Init Data ---------------------------------\\

  void InitData(notnull SCR_EntityCatalogEntry entry)
     {

     }
 };