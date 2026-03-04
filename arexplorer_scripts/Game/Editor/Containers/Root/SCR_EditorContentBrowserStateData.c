
 class SCR_EditorContentBrowserSaveStateData
 {
     [Attribute(desc: "List of labels that are saved", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
     protected ref array<EEditableEntityLabel> m_iSavedLabels;

     [Attribute(desc: "Index that is set when loaded.")]
     protected int m_iPageIndex = 0;

     [Attribute(desc: "Search string that is set when state is loaded")]
     protected string m_sSearchString;

     int GetSavedLabelCount()
     {
         return m_iSavedLabels.Count();
     }

     void SetLabels(notnull array<EEditableEntityLabel> savedLabels, bool checkForInvalid = false)
     {
         //~ Create list
         if (!m_iSavedLabels)
             m_iSavedLabels = new array<EEditableEntityLabel>;
         else
             m_iSavedLabels.Clear();

         array<EEditableEntityLabel> validLabels = {};

         if (checkForInvalid)
             SCR_Enum.GetEnumValues(EEditableEntityLabel, validLabels);

         foreach (EEditableEntityLabel label: savedLabels)
         {
             //~ Check if invalid in cause the enums changed or a mod was changed
             if (checkForInvalid && !validLabels.Contains(label))
                 continue;

             m_iSavedLabels.Insert(label);
         }
     }

     void AddRemoveLabel(EEditableEntityLabel label, bool addLabel)
     {
         if (addLabel)
         {
             if (m_iSavedLabels.Contains(label))
                 return;

             m_iSavedLabels.Insert(label);
         }
         else
         {
             int count = m_iSavedLabels.Count();

             for(int i = 0; i < count; i++)
             {
                 if (m_iSavedLabels[i] == label)
                 {
                     m_iSavedLabels.RemoveOrdered(i);
                     return;
                 }
             }
         }

     }

     int GetLabels(out notnull array<EEditableEntityLabel> labels)
     {
         if (m_iSavedLabels)
             labels.Copy(m_iSavedLabels);
         else
             return 0;

         return labels.Count();
     }

     bool ContainsLabel(EEditableEntityLabel label)
     {
         return m_iSavedLabels.Contains(label);
     }

     void SetPageIndex(int pageIndex)
     {
         m_iPageIndex = pageIndex;
     }

     int GetPageIndex()
     {
         return m_iPageIndex;
     }

     void SetSearchString(string searchString)
     {
         m_sSearchString = searchString;
     }


     string GetSearchString()
     {
         return m_sSearchString;
     }

     bool HasSearchSaved()
     {
         return !m_sSearchString.IsEmpty();
     }
 }