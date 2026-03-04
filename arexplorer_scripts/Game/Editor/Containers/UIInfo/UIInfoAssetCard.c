 [BaseContainerProps()]
 class UIInfoAssetCard : SCR_UIInfo
 {
     [Attribute("0.0 0.0 0.0 1.0", uiwidget: UIWidgets.ColorPicker)]
     private ref Color m_TextColor;

     [Attribute(params: "edds imageset")]
     private ResourceName m_TitleBackground;

     [Attribute(params: "edds imageset")]
     private ResourceName m_Image;

     [Attribute("1.0 1.0 1.0 1.0", uiwidget: UIWidgets.ColorPicker)]
     private ref Color m_FactionColor;

     [Attribute(params: "edds imageset")]
     private ResourceName m_IconBackground;

     [Attribute()]
     private bool m_bModded;

     [Attribute(params: "edds imageset")]
     private ResourceName m_ModsetIcon;

     [Attribute(params: "edds imageset")]
     private ResourceName m_ModsetFrame;

     [Attribute(params: "edds imageset")]
     private ResourceName m_ModsetLabel;

     [Attribute(params: "edds imageset")]
     private ResourceName m_CardBackground;

     [Attribute("1", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EAssetTrait))]
     private ref array<EAssetTrait> m_aTraits;

     //------------------------------------------------------------------------------------------------
     Color GetTextColor()
     {
         return Color.FromInt(m_TextColor.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetTitleBackgroundPath()
     {
         return m_TitleBackground;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetImagePath()
     {
         return m_Image;
     }

     //------------------------------------------------------------------------------------------------
     Color GetFactionColor()
     {
         return Color.FromInt(m_FactionColor.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetIconBackgroundPath()
     {
         return m_IconBackground;
     }

     //------------------------------------------------------------------------------------------------
  bool GetModded()
     {
         return m_bModded;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetModsetIcon()
     {
         return m_ModsetIcon;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetModsetFrame()
     {
         return m_ModsetFrame;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetModsetLabel()
     {
         return m_ModsetLabel;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetCardBackground()
     {
         return m_CardBackground;
     }

     //------------------------------------------------------------------------------------------------
  int GetTraits(out notnull array<EAssetTrait> outTraits)
     {
         if (!m_aTraits) return 0;
         return outTraits.Copy(m_aTraits);
     }
 }

 enum EAssetTrait
 {
     ANTI_TANK,
     ANTI_AIR,
     MEDICAL,
     REFUELING,
     TRANSPORT,
     FAST
 }