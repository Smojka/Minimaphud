 //------------------------------------------------------------------------------------------------
 class SCR_PossessSpawnData : SCR_SpawnData
 {
     protected RplId m_RplId;

     RplId GetRplId()
     {
         return m_RplId;
     }

     static SCR_PossessSpawnData FromRplId(RplId id)
     {
         SCR_PossessSpawnData data = new SCR_PossessSpawnData();
         data.m_RplId = id;
         return data;
     }

     static SCR_PossessSpawnData FromEntity(notnull IEntity entity)
     {
         RplId id = RplId.Invalid();
         RplComponent rplComponent = RplComponent.Cast(entity.FindComponent(RplComponent));
         if (rplComponent)
             id = rplComponent.Id();

         return FromRplId(id);
     }

     protected void SCR_PossessSpawnData()
     {
     }

     override bool IsValid()
     {
         return m_RplId.IsValid();
     }
 };