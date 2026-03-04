 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_DamageStateUIInfo : SCR_UIInfo
 {
     [Attribute("1.0 1.0 1.0 1.0", desc: "Main color of icon")]
     protected ref Color m_Color;

     [Attribute("1.0 1.0 1.0 1.0", desc: "Main color of background")]
     protected ref Color m_BackgroundColor;

     [Attribute("1.0 1.0 1.0 1.0", desc: "Main color of outline")]
     protected ref Color m_OutlineColor;

     [Attribute("1.0 1.0 1.0 1.0", desc: "Regeneration color of background")]
     protected ref Color m_BackgroundColorRegen;

     [Attribute("1.0 1.0 1.0 1.0", desc: "Regeneration color of outline")]
     protected ref Color m_OutlineColorRegen;

     [Attribute("{B9199157B90D6216}UI/Textures/InventoryIcons/Medical/Medical-icons.imageset", params: "edds, imageset", uiwidget: UIWidgets.ResourcePickerThumbnail)]
     protected ResourceName m_sOutlineImage;

     [Attribute("Hitzone-outline_UI", desc: "Quad name of the outline image (if using an imageset")]
     protected string m_sOutlineQuadName;

     [Attribute("{B9199157B90D6216}UI/Textures/InventoryIcons/Medical/Medical-icons.imageset", params: "edds, imageset", uiwidget: UIWidgets.ResourcePickerThumbnail)]
     protected ResourceName m_sBackgroundImage;

     [Attribute("Hitzone-BG_UI", desc: "Quad name of the background image (if using an imageset")]
     protected string m_sBackgroundQuadName;

     [Attribute("", desc: "Used for access of multiple icons from a single imageset")]
     protected ref array<string> m_aIconNames;

     //------------------------------------------------------------------------------------------------
     Color GetColor()
     {
         return Color.FromInt(m_Color.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     Color GetBackgroundColor()
     {
         return Color.FromInt(m_BackgroundColor.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     Color GetOutlineColor()
     {
         return Color.FromInt(m_OutlineColor.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     Color GetBackgroundColorRegen()
     {
         return Color.FromInt(m_BackgroundColorRegen.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     Color GetOutlineColorRegen()
     {
         return Color.FromInt(m_OutlineColorRegen.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetOutlineImage()
     {
         return m_sOutlineImage;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetBackgroundImage()
     {
         return m_sBackgroundImage;
     }

     //------------------------------------------------------------------------------------------------
  string GetOutlineQuadName()
     {
         return m_sOutlineQuadName;
     }

     //------------------------------------------------------------------------------------------------
  string GetBackgroundQuadName()
     {
         return m_sBackgroundQuadName;
     }

     //------------------------------------------------------------------------------------------------
  bool SetImageTo(ImageWidget imageWidget, ResourceName imageRes, string quadName = string.Empty)
     {
         if (!imageWidget)
             return false;

         string ext;
         FilePath.StripExtension(imageRes, ext);
         if (ext == "imageset")
             return imageWidget.LoadImageFromSet(0, imageRes, quadName);
         else
             return imageWidget.LoadImageTexture(0, imageRes);

         return true;
     }

     //------------------------------------------------------------------------------------------------
  bool SetIconTo(ImageWidget imageWidget, int iconIndex)
     {
         if (!imageWidget || Icon.IsEmpty())
             return false;

         string ext;
         FilePath.StripExtension(Icon, ext);
         if (ext == "imageset")
             imageWidget.LoadImageFromSet(0, Icon, GetIconSetName(iconIndex));
         else
             imageWidget.LoadImageTexture(0, GetIconPath());

         return true;
     }

     //------------------------------------------------------------------------------------------------
  string GetIconSetName(int iconIndex)
     {
         if (!m_aIconNames || !m_aIconNames.IsIndexValid(iconIndex))
             return GetIconSetName();

         return m_aIconNames[iconIndex];
     }
 };