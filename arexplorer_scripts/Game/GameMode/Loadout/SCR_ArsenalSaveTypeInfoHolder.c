 [BaseContainerProps(configRoot: true)]
 class SCR_ArsenalSaveTypeInfoHolder
 {
     [Attribute(desc: "List of UI info for each arsenal save types UI Info")]
     protected ref array<ref SCR_ArsenalSaveTypeInfo> m_aArsenalSaveTypeUIInfoList;

     //------------------------------------------------------------------------------------------------
  int GetArsenalSaveTypeInfoList(out notnull array<SCR_ArsenalSaveTypeInfo> arsenalSaveTypeUIInfoList)
     {
         arsenalSaveTypeUIInfoList.Clear();

         foreach (SCR_ArsenalSaveTypeInfo saveType : m_aArsenalSaveTypeUIInfoList)
         {
             if (!saveType)
                 continue;

             arsenalSaveTypeUIInfoList.Insert(saveType);
         }

         return arsenalSaveTypeUIInfoList.Count();
     }

     //------------------------------------------------------------------------------------------------
  SCR_ArsenalSaveTypeUIInfo GetUIInfoOfType(SCR_EArsenalSaveType saveTypeToCheck)
     {
         foreach (SCR_ArsenalSaveTypeInfo saveType : m_aArsenalSaveTypeUIInfoList)
         {
             if (!saveType)
                 continue;

             if (saveType.GetSaveType() == saveTypeToCheck)
                 return saveType.GetUIInfo();
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  bool HasSaveType(SCR_EArsenalSaveType saveTypeToCheck)
     {
         foreach (SCR_ArsenalSaveTypeInfo saveType : m_aArsenalSaveTypeUIInfoList)
         {
             if (!saveType)
                 continue;

             if (saveType.GetSaveType() == saveTypeToCheck)
                 return true;
         }

         return false;
     }
 }

 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(SCR_EArsenalSaveType, "m_eArsenalSaveType")]
 class SCR_ArsenalSaveTypeInfo
 {
     [Attribute("Save type associated with the with the UIInfo", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(SCR_EArsenalSaveType))]
     protected SCR_EArsenalSaveType m_eArsenalSaveType;

     [Attribute("UI info of arsenal save type. To display the display name and description")]
     protected ref SCR_ArsenalSaveTypeUIInfo m_UIInfo;

     //------------------------------------------------------------------------------------------------
     SCR_EArsenalSaveType GetSaveType()
     {
         return m_eArsenalSaveType;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ArsenalSaveTypeUIInfo GetUIInfo()
     {
         return m_UIInfo;
     }
 }