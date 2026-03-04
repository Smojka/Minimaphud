 class SCR_ComponentHelper
 {
     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource GetWeaponComponentSource(IEntitySource entitySource)
     {
         if (!entitySource)
             return null;

         int componentsCount = entitySource.GetComponentCount();
         for (int i = 0; i < componentsCount; i++)
         {
             IEntityComponentSource componentSource = entitySource.GetComponent(i);
             if (componentSource.GetClassName().ToType().IsInherited(WeaponComponent))
                 return componentSource;
         }
         return null;
     }

     //------------------------------------------------------------------------------------------------
  static WeaponUIInfo GetWeaponComponentInfo(IEntityComponentSource componentSource)
     {
         BaseContainer infoSource = componentSource.GetObject("UIInfo");
         if (!infoSource)
             return null;

         WeaponUIInfo info = WeaponUIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(infoSource));
         return info;
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource GetInventoryItemComponentSource(IEntitySource entitySource)
     {
         if (!entitySource)
             return null;

         int componentsCount = entitySource.GetComponentCount();
         for (int i = 0; i < componentsCount; i++)
         {
             IEntityComponentSource componentSource = entitySource.GetComponent(i);
             if (componentSource.GetClassName().ToType().IsInherited(InventoryItemComponent))
                 return componentSource;
         }
         return null;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ItemAttributeCollection GetInventoryItemInfo(IEntityComponentSource componentSource)
     {
         BaseContainer infoSource = componentSource.GetObject("Attributes");
         if (!infoSource)
             return null;

         SCR_ItemAttributeCollection attributeCollection = SCR_ItemAttributeCollection.Cast(BaseContainerTools.CreateInstanceFromContainer(infoSource));
         return attributeCollection;
     }
 };