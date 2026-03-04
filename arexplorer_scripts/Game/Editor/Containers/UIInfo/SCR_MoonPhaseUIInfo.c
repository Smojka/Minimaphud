 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_MoonPhaseUIInfo : SCR_UIInfo
 {
     [Attribute()]
     protected LocalizedString m_sSimplifiedName;

     protected float m_fMoonphaseImageRotation = 0;

     //------------------------------------------------------------------------------------------------
  string GetSimplifiedName()
     {
         return m_sSimplifiedName;
     }

     //------------------------------------------------------------------------------------------------
  void SetMoonphaseImageRotation(bool northernHemisphere)
     {
         if (northernHemisphere)
             m_fMoonphaseImageRotation = 0;
         else
             m_fMoonphaseImageRotation = 180;
     }

     //------------------------------------------------------------------------------------------------
  float GetMoonphaseImageRotation()
     {
         return m_fMoonphaseImageRotation;
     }
 }