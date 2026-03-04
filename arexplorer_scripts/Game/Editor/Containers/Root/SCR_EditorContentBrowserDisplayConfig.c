
 [BaseContainerProps(configRoot: true)]
 class SCR_EditorContentBrowserDisplayConfig
 {
     [Attribute(desc: "Sets the header of the content browser. Leaving this empty keeps the default header.", UIWidgets.LocaleEditBox)]
     protected LocalizedString m_sContentBrowserHeader;

     [Attribute(desc: "List of Label groups that can be shown in the Content browser. If not empty then only these groups can be shown, If empty all groups can be shown.")]
     protected ref array<ref SCR_EditorContentBrowserDisplayGroupLabel> m_aWhiteListLabelGroups;

     [Attribute(desc: "List of labels that are active when the content browser is opened and can never be removed (These labels will never be shown and cannot be disabled).")]
     protected ref array<ref SCR_EditorContentBrowserActiveDisplayLabel> m_aAlwaysActiveLabels;

     [Attribute(desc: "List of Labels that can be shown in the Content browser. If not empty then only these lables can be shown, If empty all labels can be shown. (Unless always active label hides it)")]
     protected ref array<ref SCR_EditorContentBrowserDisplayLabel> m_aWhiteListLabels;

     [Attribute(desc: "Should the browser opened with this config save it's state", defvalue: "1")]
     protected bool m_bSaveContentBrowserState;

     LocalizedString GetHeader()
     {
         return m_sContentBrowserHeader;
     }

     bool CanShowLabelGroup(EEditableEntityLabelGroup groupLabel)
     {
         //~ Search label is white list
         if (!m_aWhiteListLabelGroups.IsEmpty())
         {
             bool groupLabelFound = false;

             foreach (SCR_EditorContentBrowserDisplayGroupLabel whiteGroupLabel: m_aWhiteListLabelGroups)
             {
                 //~ Found group label in white list
                 if (whiteGroupLabel.GetGroupLabel() == groupLabel)
                 {
                     //~ Check if white label was actually enabled
                     if (whiteGroupLabel.GetEnabled())
                         groupLabelFound = true;

                     break;
                 }
             }

             //White label not found so return false
             if (!groupLabelFound)
                 return false;
         }

         return true;
     }

     bool CanShowLabel(EEditableEntityLabel label)
     {
         //~ Search label is white list
         if (!m_aWhiteListLabels.IsEmpty())
         {
             bool labelFound = false;

             foreach (SCR_EditorContentBrowserDisplayLabel whiteLabel: m_aWhiteListLabels)
             {
                 //~ Found label in white list
                 if (whiteLabel.GetLabel() == label)
                 {
                     //~ Check if white label was actually enabled
                     if (whiteLabel.IsEnabled())
                         labelFound = true;

                     break;
                 }
             }
             //White label not found so return false
             if (!labelFound)
                 return false;
         }

         //~ Is always active label so check if can be shown
         if (!m_aAlwaysActiveLabels.IsEmpty())
         {
             foreach(SCR_EditorContentBrowserActiveDisplayLabel alwaysActiveLabel: m_aAlwaysActiveLabels)
             {
                 if (alwaysActiveLabel.GetLabel() == label)
                 {
                     if (alwaysActiveLabel.IsEnabled() && !alwaysActiveLabel.m_bShowLabel)
                         return false;

                     break;
                 }
             }
         }

         return true;
     }

     void GetAlwaysActiveLabels(out notnull array<EEditableEntityLabel> alwaysActiveLabels)
     {
         foreach (SCR_EditorContentBrowserDisplayLabel label: m_aAlwaysActiveLabels)
             alwaysActiveLabels.Insert(label.GetLabel());
     }

     void GetWhiteListeLabels(out notnull array<EEditableEntityLabel> whitelistLabels)
     {
         foreach (SCR_EditorContentBrowserDisplayLabel label: m_aWhiteListLabels)
             whitelistLabels.Insert(label.GetLabel());
     }

     void GetWhiteListeLabelGroups(out notnull array<EEditableEntityLabelGroup> whitelistLabelGroups)
     {
         foreach (SCR_EditorContentBrowserDisplayGroupLabel label: m_aWhiteListLabelGroups)
             whitelistLabelGroups.Insert(label.GetGroupLabel());
     }

     bool IsAlwaysActiveLabel(EEditableEntityLabel label)
     {
         //~ No always active labels
         if (m_aAlwaysActiveLabels.IsEmpty())
             return false;

         foreach (SCR_EditorContentBrowserDisplayLabel alwaysActiveLabel: m_aAlwaysActiveLabels)
         {
             //~ Found label in alwaysActive List
             if (alwaysActiveLabel.GetLabel() == label)
             {
                 //~ Always active label is enabled so return true
                 if (alwaysActiveLabel.IsEnabled())
                     return true;

                 break;
             }
         }

         //~ Always active label not found or was not enabled
         return false;
     }


     bool CanShowLabelInActiveFilters(EEditableEntityLabel label)
     {
         SCR_EditorContentBrowserActiveDisplayLabel alwaysActiveLabelClass;

         foreach(SCR_EditorContentBrowserDisplayLabel alwaysActiveLabel: m_aAlwaysActiveLabels)
         {
             if (alwaysActiveLabel.GetLabel() != label)
                 continue;

             if (!alwaysActiveLabel.IsEnabled())
                 return true;

             alwaysActiveLabelClass = SCR_EditorContentBrowserActiveDisplayLabel.Cast(alwaysActiveLabel);
             if (!alwaysActiveLabelClass)
                 return false;

             return alwaysActiveLabelClass.m_bShowLabel;
         }

         return true;
     }

     bool GetSaveContentBrowserState()
     {
         return m_bSaveContentBrowserState;
     }

     void SCR_EditorContentBrowserDisplayConfig(array<EEditableEntityLabelGroup> whiteListGroupLabels = null, array<EEditableEntityLabel> whiteListLabels = null, array<EEditableEntityLabel> alwaysActiveLabels = null, bool saveContentBrowserState = false, LocalizedString browserHeader = string.Empty)
     {
         // Return when called with empty arguments
         if (browserHeader.IsEmpty())
             return;

         m_sContentBrowserHeader = browserHeader;

         m_aWhiteListLabelGroups = {};
         //~ Create White list Group
         if (whiteListGroupLabels)
         {
             foreach (EEditableEntityLabelGroup groupLabel: whiteListGroupLabels)
             {
                 m_aWhiteListLabelGroups.Insert(new SCR_EditorContentBrowserDisplayGroupLabel(groupLabel));
             }
         }

         m_aWhiteListLabels = {};
         //~ Create white list labels
         if (whiteListLabels)
         {
             foreach (EEditableEntityLabel label: whiteListLabels)
             {
                 m_aWhiteListLabels.Insert(new SCR_EditorContentBrowserDisplayLabel(label));
             }
         }

         m_aAlwaysActiveLabels = {};
         //~ Create Always active labels
         if (alwaysActiveLabels)
         {
             foreach(EEditableEntityLabel label: alwaysActiveLabels)
             {
                 m_aAlwaysActiveLabels.Insert(new SCR_EditorContentBrowserActiveDisplayLabel(label));
             }
         }

         //~ If this config allows for saving of the Content Browser state. If this config is temporarly created then keep it false
         m_bSaveContentBrowserState = saveContentBrowserState;
     }
 };

 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityLabelGroup, "m_iGroupLabel")]
 class SCR_EditorContentBrowserDisplayGroupLabel
 {
     [Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabelGroup))]
     protected EEditableEntityLabelGroup m_iGroupLabel;

     [Attribute("1", desc: "If false then this label group will be ignored in the list")]
     protected bool m_bEnabled;

     EEditableEntityLabelGroup GetGroupLabel()
     {
         return m_iGroupLabel;
     }

     bool GetEnabled()
     {
         return m_bEnabled;
     }

     void SCR_EditorContentBrowserDisplayGroupLabel(EEditableEntityLabelGroup groupLabel = EEditableEntityLabelGroup.NONE)
     {
         if (groupLabel == EEditableEntityLabelGroup.NONE)
             return;

         m_iGroupLabel = groupLabel;
         m_bEnabled = true;
     }
 };

 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityLabel, "m_iLabel")]
 class SCR_EditorContentBrowserDisplayLabel
 {
     [Attribute(uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
     protected EEditableEntityLabel m_iLabel;

     [Attribute("1", desc: "If false then this label will be ignored in the list")]
     protected bool m_bEnabled;

     EEditableEntityLabel GetLabel()
     {
         return m_iLabel;
     }

     bool IsEnabled()
     {
         return m_bEnabled;
     }

     void SCR_EditorContentBrowserDisplayLabel(EEditableEntityLabel label = EEditableEntityLabel.NONE)
     {
         if (label == EEditableEntityLabel.NONE)
             return;

         m_iLabel = label;
         m_bEnabled = true;
     }
 };

 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityLabel, "m_iLabel")]
 class SCR_EditorContentBrowserActiveDisplayLabel: SCR_EditorContentBrowserDisplayLabel
 {
     [Attribute("0", desc: "If false then this label will hidden in filters and will always be forced to be active (if enabled)")]
     bool m_bShowLabel;
 };