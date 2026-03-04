 class SCR_WallGroup
 {
     ref array<ref SCR_WallPair> m_aWallPairs = {};
     ref array<float> m_aWeights = {};

     float m_fWallLength;

     //------------------------------------------------------------------------------------------------
  SCR_WallPair GetRandomWall()
     {
         int index = SCR_ArrayHelper.GetWeightedIndex(m_aWeights, Math.RandomFloat01());
         return m_aWallPairs.Get(index);
     }
 }