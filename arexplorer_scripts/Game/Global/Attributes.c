
 class SCR_BaseContainerStaticTitleField : BaseContainerCustomTitle
 {
     protected string m_sCustomTitle;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerStaticTitleField(string customTitle = "")
     {
         customTitle.Trim();
         m_sCustomTitle = customTitle;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         // if (m_sCustomTitle.IsEmpty())
         //  return false;

         title = m_sCustomTitle;
         return true;
     }
 };

 class SCR_BaseContainerCustomTitleField : BaseContainerCustomTitle
 {
     protected string m_sPropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleField(string propertyName, string format = "%1")
     {
         m_sPropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         if (!source.Get(m_sPropertyName, title))
             return false;

         title = string.Format(m_sFormat, title);
         return true;
     }
 };

 class SCR_BaseContainerCustomTitleFields : BaseContainerCustomTitle
 {
     protected ref array<string> m_aPropertyNames;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleFields(array<string> propertyNames, string format = "%1")
     {
         m_aPropertyNames = propertyNames;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         if (!m_aPropertyNames)
         {
             title = m_sFormat;
             return false;
         }

         int count = m_aPropertyNames.Count();
         array<string> arguments = {};
         arguments.Resize(count); // needed here

         for (int i = 0; i < count; i++)
         {
             title = "<Â°))))-<";
             if (!source.Get(m_aPropertyNames[i], title) || title == "<Â°))))-<")
             {
                 ResourceName tempResourceName;
                 if (source.Get(m_aPropertyNames[i], tempResourceName))
                     title = FilePath.StripPath(tempResourceName);
                 else
                     title = "x";
             }

             arguments[i] = title;
         }

         title = SCR_StringHelper.Format(m_sFormat, arguments);

         return true;
     }
 };

 class SCR_BaseContainerLocalizedTitleField : BaseContainerCustomTitle
 {
     protected string m_sPropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerLocalizedTitleField(string propertyName, string format = "%1")
     {
         m_sPropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         if (!source.Get(m_sPropertyName, title))
             return false;

         title = string.Format(m_sFormat, WidgetManager.Translate(title));
         return true;
     }
 };

 class SCR_BaseContainerLocalizedTitleFields : BaseContainerCustomTitle
 {
     protected ref array<string> m_aPropertyNames;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerMultipleParametersTitleField(array<string> propertyNames, string format = "%1")
     {
         m_aPropertyNames = propertyNames;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         if (!m_aPropertyNames)
         {
             title = m_sFormat;
             return false;
         }

         int count = m_aPropertyNames.Count();
         array<string> arguments = {};
         arguments.Resize(count); // needed here

         for (int i = 0; i < count; i++)
         {
             title = "<Â°))))-<"; // looks fishy hey?
             if (!source.Get(m_aPropertyNames[i], title) || title == "<Â°))))-<")
             {
                 ResourceName tempResourceName;
                 if (source.Get(m_aPropertyNames[i], tempResourceName) && title != "<Â°))))-<")
                     title = FilePath.StripPath(tempResourceName);
                 else
                     title = "x";
             }

             arguments[i] = title;
         }

         title = SCR_StringHelper.Translate(m_sFormat, arguments);

         return true;
     }
 };
 class SCR_BaseContainerResourceTitleField : BaseContainerCustomTitle
 {
     protected string m_sPropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerResourceTitleField(string propertyName, string format = "%1")
     {
         m_sPropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         ResourceName resourceName;
         if (!source.Get(m_sPropertyName, resourceName))
             return false;

         title = string.Format(m_sFormat, FilePath.StripPath(resourceName));
         return true;
     }
 };

 class SCR_BaseContainerCustomTitleEnum : BaseContainerCustomTitle
 {
     protected typename m_EnumType;
     protected string m_PropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleEnum(typename enumType, string propertyName, string format = "%1")
     {
         m_EnumType = enumType;
         m_PropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         int enumValue;
         if (!source.Get(m_PropertyName, enumValue))
         {
             return false;
         }

         title = string.Format(m_sFormat, typename.EnumToString(m_EnumType, enumValue));
         return true;
     }
 };

 class BaseContainerCustomEnumWithValue : BaseContainerCustomTitle
 {
     protected typename m_EnumType;
     protected string m_sEnumName;
     protected string m_sValueName;
     protected string m_sDefaultValue;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomEnumWithValue(typename enumType, string enumName, string valueName, string defaultValue, string format = "%1: %2")
     {
         m_EnumType = enumType;
         m_sEnumName = enumName;
         m_sValueName = valueName;
         m_sDefaultValue = defaultValue;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         int enumValue;
         if (!source.Get(m_sEnumName, enumValue))
             return false;

         string value;
         if (!source.Get(m_sValueName, value))
             return false;

         //~ No value found so use default
         if (value.IsEmpty())
             value = m_sDefaultValue;

         title = string.Format(m_sFormat, typename.EnumToString(m_EnumType, enumValue), value);
         return true;
     }
 };

 class SCR_BaseContainerCustomTitleFlags : BaseContainerCustomTitle
 {
     protected typename m_EnumType;
     protected string m_PropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleFlags(typename enumType, string propertyName, string format = "%1")
     {
         m_EnumType = enumType;
         m_PropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         int enumValue;
         if (!source.Get(m_PropertyName, enumValue))
             return false;

         array<int> values = {};
         string enumName;
         for (int i = 0, count = SCR_Enum.BitToIntArray(enumValue, values); i < count; i++)
         {
             if (i > 0)
                 enumName += " | ";

             enumName += typename.EnumToString(m_EnumType, values[i]);
         }

         title = string.Format(m_sFormat, enumName);
         return true;
     }
 };

 class SCR_BaseContainerCustomTitleResourceName : BaseContainerCustomTitle
 {
     protected string m_sPropertyName;
     protected bool m_bFileNameOnly;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleResourceName(string propertyName, bool fileNameOnly = false, string format = "%1")
     {
         m_sPropertyName = propertyName;
         m_bFileNameOnly = fileNameOnly;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         ResourceName path;
         if (!source.Get(m_sPropertyName, path))
             return false;

         title = path.GetPath();
         if (m_bFileNameOnly)
             title = string.Format(m_sFormat, FilePath.StripPath(title));

         return true;
     }
 };

 class SCR_BaseContainerCustomTitleObject : BaseContainerCustomTitle
 {
     protected string m_sPropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleObject(string propertyName, string format = "%1")
     {
         m_sPropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         BaseContainer object = source.GetObject(m_sPropertyName);
         if (!object)
             return false;

         title = string.Format(m_sFormat, object.GetClassName());
         return true;
     }
 };

 class BaseContainerCustomStringTitleField : BaseContainerCustomTitle
 {
     string m_Title;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomStringTitleField(string title)
     {
         m_Title = title;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         title = m_Title;
         return !title.IsEmpty();
     }
 };

 class BaseContainerCustomDoubleTitleField : BaseContainerCustomTitle
 {
     protected string m_sPropertyName1;
     protected string m_sPropertyName2;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomDoubleTitleField(string propertyName1, string propertyName2, string format = "%1: %2")
     {
         m_sPropertyName1 = propertyName1;
         m_sPropertyName2 = propertyName2;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         string title1, title2;

         if (!source.Get(m_sPropertyName1, title1))
             return false;

         if (!source.Get(m_sPropertyName2, title2))
             return false;

         title = string.Format(m_sFormat, title1, title2);
         return true;
     }
 };

 class BaseContainerCustomDoubleCheckTitleField : BaseContainerCustomTitle
 {
     protected string m_sCheckVar;
     protected string m_sPropertyName;
     protected string m_sFormatTrue;
     protected string m_sFormatFalse;
     protected string m_sCheckVarEqual;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomDoubleCheckTitleField(string checkVar, string propertyName, string checkVarEqual = "1", string formatTrue = "%1", string formatFalse = "EXAMPLE FORMAT - %1")
     {
         m_sCheckVar = checkVar;
         m_sPropertyName = propertyName;
         m_sFormatTrue = formatTrue;
         m_sFormatFalse = formatFalse;
         m_sCheckVarEqual = checkVarEqual;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         string checkVar, titleName;

         if (!source.Get(m_sCheckVar, checkVar))
             return false;

         if (!source.Get(m_sPropertyName, titleName))
             return false;

         if (checkVar == m_sCheckVarEqual)
             title = string.Format(m_sFormatTrue, titleName);
         else
             title = string.Format(m_sFormatFalse, titleName);

         return true;
     }
 };

 class BaseContainerCustomDoubleCheckIntResourceNameTitleField : BaseContainerCustomTitle
 {
     protected string m_sCheckVar;
     protected string m_sPropertyName;
     protected string m_sFormatTrue;
     protected string m_sFormatFalse;
     protected float m_iCheckVarEqual;
     protected bool m_bFileNameOnly;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomDoubleCheckIntResourceNameTitleField(string checkVar, string propertyName, int checkVarEqual, string formatTrue = "%1", string formatFalse = "EXAMPLE FORMAT - %1", bool fileNameOnly = true)
     {
         m_sCheckVar = checkVar;
         m_sPropertyName = propertyName;
         m_sFormatTrue = formatTrue;
         m_sFormatFalse = formatFalse;
         m_iCheckVarEqual = checkVarEqual;
         m_bFileNameOnly = fileNameOnly;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         int checkVar;
         string pathString;

         if (!source.Get(m_sCheckVar, checkVar))
             return false;

         ResourceName path;
         if (!source.Get(m_sPropertyName, path))
             return false;

         if (path.IsEmpty())
             pathString = "NO PREFAB";
         else
             pathString = path.GetPath();

         if (checkVar == m_iCheckVarEqual)
         {
             if (!m_bFileNameOnly || path.IsEmpty())
                 title = string.Format(m_sFormatTrue, pathString);
             else
                 title = string.Format(m_sFormatTrue, FilePath.StripPath(pathString));
         }
         else
         {
             if (!m_bFileNameOnly || path.IsEmpty())
                 title = string.Format(m_sFormatFalse, pathString);
             else
                 title = string.Format(m_sFormatFalse, FilePath.StripPath(pathString));
         }

         return true;
     }
 };

 class BaseContainerCustomCheckIntTitleField : BaseContainerCustomTitle
 {
     protected string m_sCheckVar;
     protected string m_sConditionTrueText;
     protected string m_sConditionFalseText;
     protected int m_iCheckVarEqual;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomCheckIntTitleField(string checkVar, string conditionTrueText, string conditionFalseText, int checkVarEqual)
     {
         m_sCheckVar = checkVar;
         m_sConditionTrueText = conditionTrueText;
         m_sConditionFalseText = conditionFalseText;
         m_iCheckVarEqual = checkVarEqual;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         int checkVar;

         if (!source.Get(m_sCheckVar, checkVar))
             return false;

         if (checkVar == m_iCheckVarEqual)
             title = m_sConditionTrueText;
         else
             title = m_sConditionFalseText;

         return true;
     }
 };

 class BaseContainerCustomCheckIntWithFlagTitleField : BaseContainerCustomTitle
 {
     protected typename m_FlagEnumType;
     protected string m_sFlagName;
     protected string m_sFlagDivider;
     protected string m_sCheckVar;
     protected string m_sConditionTrueText;
     protected string m_sConditionFalseText;
     protected int m_iCheckVarEqual;

     //------------------------------------------------------------------------------------------------
     void BaseContainerCustomCheckIntWithFlagTitleField(typename flagEnumType, string flagName, string checkVar, int checkVarEqual, string conditionTrueText = "DEFAULT TRUE - %1", string conditionFalseText = "DEFAULT FALSE - %2", string flagDivider = " & ")
     {
         m_FlagEnumType = flagEnumType;
         m_sFlagName = flagName;
         m_sFlagDivider = flagDivider;
         m_sCheckVar = checkVar;
         m_sConditionTrueText = conditionTrueText;
         m_sConditionFalseText = conditionFalseText;
         m_iCheckVarEqual = checkVarEqual;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         int checkVar;

         if (!source.Get(m_sCheckVar, checkVar))
             return false;

         if (checkVar == m_iCheckVarEqual)
         {
             title = m_sConditionTrueText;
         }
         else
         {
             title = m_sConditionFalseText;
             return true;
         }

         string enumName = "NONE";

         int enumValue;
         if (source.Get(m_sFlagName, enumValue))
         {
             array<int> values = {};
             for (int i = 0, count = SCR_Enum.BitToIntArray(enumValue, values); i < count; i++)
             {
                 if (i == 0)
                     enumName = string.Empty;

                 if (i > 0)
                     enumName += m_sFlagDivider;

                 enumName += typename.EnumToString(m_FlagEnumType, values[i]);
             }
         }

         //~ Add flags
         title = title.Format(title, enumName);

         return true;
     }
 }

 class SCR_BaseContainerCustomTitleUIInfo : BaseContainerCustomTitle
 {
     protected string m_sPropertyName;
     protected string m_sFormat;

     //------------------------------------------------------------------------------------------------
     void SCR_BaseContainerCustomTitleUIInfo(string propertyName, string format = "%1")
     {
         m_sPropertyName = propertyName;
         m_sFormat = format;
     }

     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         BaseContainer info = source.GetObject(m_sPropertyName);
         if (!info || !info.Get("Name", title))
             return false;

         title = string.Format(m_sFormat, WidgetManager.Translate(title));
         return true;
     }
 };