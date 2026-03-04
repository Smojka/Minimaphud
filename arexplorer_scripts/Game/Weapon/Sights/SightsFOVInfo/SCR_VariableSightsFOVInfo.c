 class SCR_VariableSightsFOVInfo: SCR_BaseVariableSightsFOVInfo
 {
     [Attribute("", UIWidgets.Auto, desc: "Individual FOV settings.")]
     protected ref array<float> m_aFOVs;

     [Attribute("6.0", UIWidgets.Slider, desc: "Interpolation speed.", params: "0 100 0.1")]
     protected float m_fInterpolationSpeed;

     protected float m_fCurrentFOV = 60.0;

     protected int m_iCurrentIndex = -1;

     //------------------------------------------------------------------------------------------------
  override int GetCount()
     {
         return m_aFOVs.Count();
     }

     //------------------------------------------------------------------------------------------------
  override int GetCurrentIndex()
     {
         return m_iCurrentIndex;
     }

     //------------------------------------------------------------------------------------------------
  protected override void SetIndex(int index)
     {
         m_iCurrentIndex = index;
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnInit(IEntity owner, BaseSightsComponent sights)
     {
         // Initialize to default (first available) value
         if (GetCount() > 0)
         {
             SetIndex(0);
             m_fCurrentFOV = m_aFOVs[0];
         }
         else
             m_fCurrentFOV = 60.0;
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnUpdate(IEntity owner, BaseSightsComponent sights, float timeSlice)
     {
         if (!m_aFOVs.IsIndexValid(m_iCurrentIndex))
             return;

         float target = m_aFOVs[m_iCurrentIndex];
         m_fCurrentFOV = Math.Lerp(m_fCurrentFOV, target, timeSlice * m_fInterpolationSpeed);
     }

     //------------------------------------------------------------------------------------------------
  override bool IsAdjusting()
     {
         if (!m_aFOVs.IsIndexValid(m_iCurrentIndex))
             return false;

         return !float.AlmostEqual(m_fCurrentFOV, m_aFOVs[m_iCurrentIndex]);
     }

     //------------------------------------------------------------------------------------------------
  protected override float GetCurrentFOV()
     {
         return m_fCurrentFOV;
     }

     //------------------------------------------------------------------------------------------------
  override float GetBaseFOV()
     {
         if (!m_aFOVs.IsEmpty())
             return m_aFOVs[0];

         return m_fCurrentFOV;
     }
 };