 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_UIDescription : SCR_UIName
 {
     [Attribute(uiwidget: UIWidgets.EditBoxMultiline)]
     protected LocalizedString Description;

     //------------------------------------------------------------------------------------------------
  LocalizedString GetUnformattedDescription()
     {
         return Description;
     }

     //------------------------------------------------------------------------------------------------
  void SetDescription(LocalizedString description)
     {
         Description = description;
     }

     //------------------------------------------------------------------------------------------------
  LocalizedString GetDescription()
     {
         return Description;
     }

     //------------------------------------------------------------------------------------------------
  bool HasDescription()
     {
         return !GetDescription().IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  bool SetDescriptionTo(TextWidget textWidget)
     {
         if (!textWidget)
             return false;

         textWidget.SetText(GetDescription());
         return true;
     }

     //------------------------------------------------------------------------------------------------
  override void Log(string prefix = string.Empty, LogLevel logLevel = LogLevel.VERBOSE)
     {
         Print(string.Format(prefix + "%1: \"%2\", \"%3\"", Type(), Name, Description), logLevel);
     }

     //------------------------------------------------------------------------------------------------
     //--- Protected, to be overridden and/or made public by inherited classes
     override protected void CopyFrom(SCR_UIName source)
     {
         SCR_UIDescription sourceDescription = SCR_UIDescription.Cast(source);
         if (sourceDescription)
         {
             Description = sourceDescription.Description;
         }

         super.CopyFrom(source);
     }
 }