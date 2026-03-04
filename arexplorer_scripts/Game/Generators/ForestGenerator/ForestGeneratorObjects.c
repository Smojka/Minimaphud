 //------------------------------------------------------------------------------------------------
 // Bindings of C++ helpers for ForestGeneratorEntity
 //------------------------------------------------------------------------------------------------

 //------------------------------------------------------------------------------------------------
 class ForestGeneratorDistaceAttribute
 {
     DistanceType m_iDistanceType;
     void ForestGeneratorDistaceAttribute(DistanceType distanceType)
     {
         m_iDistanceType = distanceType;
     }
 }

 //------------------------------------------------------------------------------------------------
 class ForestGeneratorGroupIndexAttribute
 {
 }

 //------------------------------------------------------------------------------------------------
 class ForestGeneratorCapsuleStartAttribute
 {
 }

 //------------------------------------------------------------------------------------------------
 class ForestGeneratorCapsuleEndAttribute
 {
 }

 //------------------------------------------------------------------------------------------------
 class SCR_ForestGeneratorTreeBase : ForestGeneratorTreeBase
 {
     [Attribute("0.8", UIWidgets.SpinBox, "Min scale of this object", params: "0 1000 0.01")]
     float m_fMinScale;

     [Attribute("1.2", UIWidgets.SpinBox, "Max scale of this object", params: "0 1000 0.01")]
     float m_fMaxScale;

     [Attribute("", UIWidgets.ResourcePickerThumbnail, "Prefab of this object", "et")]
     ResourceName m_Prefab;

     [Attribute(defvalue: "1", uiwidget: UIWidgets.SpinBox, "Minimum required radius in the bottom layer for this object to spawn"), ForestGeneratorDistaceAttribute(DistanceType.BOTTOM)];
     float m_fBotDistance;

     [Attribute("0", uiwidget: UIWidgets.SpinBox, "Maximum random pitch angle", "0 180 1")]
     float m_fRandomPitchAngle;

     [Attribute("0", uiwidget: UIWidgets.SpinBox, "Maximum random roll angle", "0 180 1")]
     float m_fRandomRollAngle;

     [Attribute("0")]
     float m_fVerticalOffset;

     [ForestGeneratorGroupIndexAttribute()]
     int m_iGroupIndex;

     float m_fScale = 2;
     SCR_ETreeType m_eType;

     void AdjustScale();
 }

 //------------------------------------------------------------------------------------------------
 class ForestGeneratorShapeImportData
 {
     IEntitySource source;
     IEntity entity;
     int id;
     ref SCR_AABB bbox;
     ref array<vector> points = new array<vector>();

     void GenerateAAB()
     {
         bbox = new SCR_AABB(points);
     }
 }