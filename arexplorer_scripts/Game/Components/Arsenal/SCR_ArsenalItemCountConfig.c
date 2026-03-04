 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(SCR_EArsenalItemType, "m_ItemType")]
 class SCR_ArsenalItemCountConfig
 {
     [Attribute("0", UIWidgets.ComboBox, "", enums: ParamEnumArray.FromEnum(SCR_EArsenalItemType))]
     protected SCR_EArsenalItemType m_ItemType;

     [Attribute()]
     protected int m_MaxItemCount;

     //------------------------------------------------------------------------------------------------
  SCR_EArsenalItemType GetItemType()
     {
         return m_ItemType;
     }

     //------------------------------------------------------------------------------------------------
  int GetMaxItemCount()
     {
         return m_MaxItemCount;
     }

     //------------------------------------------------------------------------------------------------
  static bool CheckMaxCount(array<ref SCR_ArsenalItemCountConfig> maxItemCounts, SCR_EArsenalItemType itemType, int currentCountTaken)
     {
         if (!maxItemCounts)
             return false;

         for (int i = 0, c = maxItemCounts.Count(); i < c; i++)
         {
             SCR_ArsenalItemCountConfig item = maxItemCounts[i];
             if (item.GetItemType() == itemType
                 && currentCountTaken >= item.GetMaxItemCount())
             {
                 return false;
             }
         }
         return true;
     }
 }