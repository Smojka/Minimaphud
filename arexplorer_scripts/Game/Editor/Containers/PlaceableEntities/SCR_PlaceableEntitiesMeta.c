 [BaseContainerProps()]
 class SCR_PlaceableEntitiesMeta
 {
     [Attribute()]
     private LocalizedString m_sDisplayName;

     string GetDisplayName()
     {
         return m_sDisplayName;
     }
 };