 [BaseContainerProps()]
 class WallWeightPair
 {
     [Attribute("", UIWidgets.ResourcePickerThumbnail, "Prefab", "et")]
     ResourceName m_sWallAsset;

     [Attribute("1", UIWidgets.EditBox)]
     float m_fWeight;

     [Attribute("0", UIWidgets.Slider, params: "-10 10 0.01")]
     float m_fPrePadding;

     [Attribute("0", UIWidgets.Slider, params: "-10 10 0.01")]
     float m_fPostPadding;
 }