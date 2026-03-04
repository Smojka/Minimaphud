 [BaseContainerProps(configRoot: true)]
 class SCR_BasePreviewConfig
 {
     [Attribute()]
     ref array<ref SCR_BasePreviewEntry> m_aEntries;

     bool SaveContainer(ResourceName resourceName, string fileName = string.Empty)
     {
         Resource resource = BaseContainerTools.CreateContainerFromInstance(this);
         if (!resource || !resource.IsValid())
             return false;

         return BaseContainerTools.SaveContainer(resource.GetResource().ToBaseContainer(), resourceName, fileName);
     }
 };