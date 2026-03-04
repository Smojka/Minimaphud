 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_EditorAttributeUIInfo : SCR_UIInfo
 {
     [Attribute("1 1 1 1", UIWidgets.ColorPicker, desc: "Description Icon Color")]
     protected ref Color m_cDescriptionIconColor;

     //------------------------------------------------------------------------------------------------
  void CopyFromEditorAttributeUIInfo(notnull SCR_EditorAttributeUIInfo source, LocalizedString overwriteDescription = string.Empty, Color overwriteColor = null)
     {
         CopyFrom(source);

         if (!overwriteDescription.IsEmpty())
             Description = overwriteDescription;

         if (!overwriteColor)
             m_cDescriptionIconColor = overwriteColor;
     }

     //------------------------------------------------------------------------------------------------
     override void CopyFrom(SCR_UIName source)
     {
         SCR_EditorAttributeUIInfo sourceInfo = SCR_EditorAttributeUIInfo.Cast(source);
         if (sourceInfo)
         {
             Icon = sourceInfo.Icon;
             IconSetName = sourceInfo.IconSetName;
             Description = sourceInfo.Description;
             m_cDescriptionIconColor = sourceInfo.m_cDescriptionIconColor;
         }

         super.CopyFrom(source);
     }

     //------------------------------------------------------------------------------------------------
     Color GetDescriptionIconColor()
     {
         return Color.FromInt(m_cDescriptionIconColor.PackToInt());
     }
 }