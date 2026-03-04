 class SCR_Ray
 {
     vector m_vPosition;
     vector m_vDirection;

     //------------------------------------------------------------------------------------------------
  static SCR_Ray Lerp(notnull SCR_Ray valueA, notnull SCR_Ray valueB, float t = 0.5)
     {
         SCR_Ray result = new SCR_Ray();
         result.m_vPosition = vector.Lerp(valueA.m_vPosition, valueB.m_vPosition, t);
         result.m_vDirection = vector.Lerp(valueA.m_vDirection, valueB.m_vDirection, t).Normalized();
         return result;
     }
 }