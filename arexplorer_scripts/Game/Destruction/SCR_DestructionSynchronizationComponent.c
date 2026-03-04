 #define ENABLE_BASE_DESTRUCTION

 [EntityEditorProps(category: "GameScripted/Destruction", description: "")]
 class SCR_DestructionSynchronizationComponentClass : ScriptComponentClass
 {
 }

 class SCR_DestructionSynchronizationComponent : ScriptComponent
 {
 #ifdef ENABLE_BASE_DESTRUCTION
     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     void RPC_RequestDestructibleState(RplId rplId, int index)
     {
         SCR_MPDestructionManager destructionManager = SCR_MPDestructionManager.GetInstance();
         if (!destructionManager)
             return;

         SCR_DestructionDamageManagerComponent destructible = destructionManager.FindDynamicallySpawnedDestructibleByIndex(rplId, index);
         if (!destructible)
             return;

         destructible.ReplicateDestructibleState();
     }

     //------------------------------------------------------------------------------------------------
  void RequestDestructibleState(RplId rplId, int index)
     {
         Rpc(RPC_RequestDestructibleState, rplId, index);
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_DestructionSynchronizationComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
     }
 #endif
 }