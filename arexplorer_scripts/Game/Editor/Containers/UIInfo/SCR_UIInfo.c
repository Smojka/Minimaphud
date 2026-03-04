 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_UIInfo : SCR_UIDescription
 {
     [Attribute(params: "edds imageset", uiwidget: UIWidgets.ResourcePickerThumbnail)]
     protected ResourceName Icon;

     [Attribute(desc: "When 'Icon' is an image set, this defines name of the image in the set.")]
     protected string IconSetName;

     //------------------------------------------------------------------------------------------------
     ResourceName GetIconPath()
     {
         string ext;
         FilePath.StripExtension(Icon, ext);
         if (ext == "imageset")
         {
             Print(string.Format("Unable to return texture path, it's an image set '%1'! Use SCR_UIInfo.SetIconTo() instead.", Icon), LogLevel.WARNING);
             return ResourceName.Empty;
         }
         else
         {
             return Icon;
         }
     }

     //------------------------------------------------------------------------------------------------
  string GetIconSetName()
     {
         string ext;
         FilePath.StripExtension(Icon, ext);

         if (ext == "imageset")
             return IconSetName;
         else
             return "";
     }

     //------------------------------------------------------------------------------------------------
  bool HasIcon()
     {
         if (Icon.IsEmpty())
             return false;

         //Check if image set name is assigned
         string ext;
         FilePath.StripExtension(Icon, ext);
         if (ext == "imageset")
             return !GetIconSetName().IsEmpty();

         return true;
     }

     //------------------------------------------------------------------------------------------------
  bool SetIconTo(ImageWidget imageWidget)
     {
         if (!imageWidget || Icon.IsEmpty())
             return false;

         string ext;
         FilePath.StripExtension(Icon, ext);
         if (ext == "imageset")
             imageWidget.LoadImageFromSet(0, Icon, GetIconSetName());
         else
             imageWidget.LoadImageTexture(0, GetIconPath());

         return true;
     }

     //------------------------------------------------------------------------------------------------
  override void Log(string prefix = string.Empty, LogLevel logLevel = LogLevel.VERBOSE)
     {
         Print(string.Format(prefix + "%1: \"%2\", \"%3\", \"%4\"", Type(), Name, Description, Icon), logLevel);
     }

     //------------------------------------------------------------------------------------------------
  static SCR_UIInfo GetInfo(BaseContainer source, string varName)
     {
         BaseContainer infoSource = source.GetObject(varName);
         if (infoSource)
             return SCR_UIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(infoSource));
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_UIInfo CreateInfo(UIInfo source)
     {
         if (!source)
             return null;

         SCR_UIInfo info = new SCR_UIInfo();
         info.Name = source.GetName();
         info.Description = source.GetDescription();
         info.Icon = source.GetIconPath();
         return info;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_UIInfo CreateInfo(LocalizedString name, LocalizedString description = LocalizedString.Empty, ResourceName icon = ResourceName.Empty, string iconSetName = string.Empty)
     {
         SCR_UIInfo info = new SCR_UIInfo();
         info.Name = name;
         info.Description = description;
         info.Icon = icon;
         info.IconSetName = iconSetName;
         return info;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_UIInfo CreatePlaceholderInfo(typename type)
     {
         SCR_UIInfo info = SCR_UIInfo.Cast(type.Spawn());
         info.Name = string.Format("ERROR: Missing info for %1", type);
         return info;
     }

     //------------------------------------------------------------------------------------------------
     //--- Protected, to be overridden and/or made public by inherited classes
     protected override void CopyFrom(SCR_UIName source)
     {
         SCR_UIInfo sourceInfo = SCR_UIInfo.Cast(source);
         if (sourceInfo)
         {
             Icon = sourceInfo.Icon;
             IconSetName = sourceInfo.IconSetName;
         }

         super.CopyFrom(source);
     }
 }