 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_UIName
 {
     [Attribute(uiwidget: UIWidgets.LocaleEditBox)]
     protected LocalizedString Name;

     //------------------------------------------------------------------------------------------------
  LocalizedString GetUnformattedName()
     {
         return Name;
     }

     //------------------------------------------------------------------------------------------------
  void SetName(LocalizedString name)
     {
         Name = name;
     }

     //------------------------------------------------------------------------------------------------
  LocalizedString GetName()
     {
         return Name;
     }

     //------------------------------------------------------------------------------------------------
  bool HasName()
     {
         return !GetName().IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  bool SetNameTo(TextWidget textWidget)
     {
         if (!textWidget)
             return false;

         textWidget.SetText(GetName());
         return true;
     }

     //------------------------------------------------------------------------------------------------
  void Log(string prefix = string.Empty, LogLevel logLevel = LogLevel.VERBOSE)
     {
         Print(string.Format(prefix + "%1: \"%2\"", Type(), Name), logLevel);
     }

     //--- Protected, to be overridden and/or made public by inherited classes
     protected void CopyFrom(SCR_UIName source)
     {
         if (source)
             Name = source.Name;
     }
 }