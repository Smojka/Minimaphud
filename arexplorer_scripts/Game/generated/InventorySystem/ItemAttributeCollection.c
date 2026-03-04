 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ItemAttributeCollection: ScriptAndConfig
 {
     proto external UIInfo GetUIInfo();
     proto external BaseItemAttributeData FindAttribute(typename typeName);
     proto external ECommonItemType GetCommonType();

     // callbacks

     event protected void OnInitCollection(IEntityComponentSource src);
 }
