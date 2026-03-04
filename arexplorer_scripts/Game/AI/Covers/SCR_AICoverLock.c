
 class SCR_AICoverLock
 {
     vector m_vCoverPos;
     vector m_vCoverTallestPos;

     vector m_vCoverDirXZ; // Cover direction vector in XZ plane

     int m_iTileX;
     int m_iTileY;
     int m_iCoverId;

     //----------------------------------------------------------------
     vector GetPosition()
     {
         return m_vCoverPos;
     }

     //----------------------------------------------------------------
  bool IsValid()
     {
         return m_vCoverPos != vector.Zero;
     }

     //----------------------------------------------------------------
  void Release()
     {
         CoverManagerComponent coverMgr = GetCoverManager();
         if (!coverMgr)
             return;

         // Release cover
         if (m_vCoverPos != vector.Zero)
             coverMgr.SetOccupiedCover(m_iTileX, m_iTileY, m_iCoverId, false);

         // This object is invalid from now on
         m_vCoverPos = vector.Zero;
     }

     //----------------------------------------------------------------
     void SCR_AICoverLock(int tilex, int tiley, int coverId, vector coverPos, vector coverTallestPos)
     {
         CoverManagerComponent coverMgr = GetCoverManager();
         if (!coverMgr)
             return;

         m_iTileX = tilex;
         m_iTileY = tiley;
         m_iCoverId = coverId;
         m_vCoverPos = coverPos;
         m_vCoverTallestPos = coverTallestPos;

         // Occupy cover on construction
         if (m_vCoverPos != vector.Zero)
             coverMgr.SetOccupiedCover(m_iTileX, m_iTileY, m_iCoverId, true);

         // Calculate derived values
         vector coverDirXZ = m_vCoverTallestPos - m_vCoverPos;
         coverDirXZ[1] = 0;
         coverDirXZ.Normalize();
         m_vCoverDirXZ = coverDirXZ;
     }

     //----------------------------------------------------------------
     void ~SCR_AICoverLock()
     {
         Release();
     }

     //----------------------------------------------------------------
  float CosAngleToThreat(vector threatPos)
     {
         vector dirCoverToThreatXZ = threatPos - m_vCoverPos;
         dirCoverToThreatXZ[1] = 0;
         dirCoverToThreatXZ.Normalize();
         float cosAngleCoverDirThreatDir = vector.Dot(m_vCoverDirXZ, dirCoverToThreatXZ);
         return cosAngleCoverDirThreatDir;
     }

     //----------------------------------------------------------------
     protected static CoverManagerComponent GetCoverManager()
     {
         AIWorld aiWorld = GetGame().GetAIWorld();
         if (!aiWorld)
             return null;
         CoverManagerComponent coverMgr = CoverManagerComponent.Cast(aiWorld.FindComponent(CoverManagerComponent));
         return coverMgr;
     }
 }