 [EntityEditorProps(category: "GameScripted/DeployableItems", description: "")]
 class SCR_BaseDeployableInventoryItemComponentClass : ScriptComponentClass
 {
 }

 class SCR_BaseDeployableInventoryItemComponent : ScriptComponent
 {
     [RplProp(onRplName: "OnRplDeployed")]
     protected bool m_bIsDeployed;

     protected int m_iItemOwnerID = -1;

     protected RplComponent m_RplComponent;

     //------------------------------------------------------------------------------------------------
  void Deploy(IEntity userEntity = null)
     {
         if (!m_RplComponent || m_RplComponent.IsProxy())
             return;

         PlayerManager playerManager = GetGame().GetPlayerManager();
         if (!playerManager)
             return;

         if (userEntity)
             m_iItemOwnerID = playerManager.GetPlayerIdFromControlledEntity(userEntity);

         // Put deploy logic here

         m_bIsDeployed = true;
         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
  void Dismantle(IEntity userEntity = null)
     {
         if (!m_RplComponent || m_RplComponent.IsProxy())
             return;

         m_iItemOwnerID = -1; // Reset owner ID

         // Put dismantle logic here

         m_bIsDeployed = false;
         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
  protected void OnRplDeployed();

     //------------------------------------------------------------------------------------------------
  bool CanDeployBeShown(notnull IEntity userEntity)
     {
         return !m_bIsDeployed;
     }

     //------------------------------------------------------------------------------------------------
  bool CanDismantleBeShown(notnull IEntity userEntity)
     {
         return m_bIsDeployed;
     }

     //------------------------------------------------------------------------------------------------
  bool IsDeployed()
     {
         return m_bIsDeployed;
     }

     //------------------------------------------------------------------------------------------------
  int GetItemOwnerID()
     {
         return m_iItemOwnerID;
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         super.OnPostInit(owner);
         SetEventMask(GetOwner(), EntityEvent.INIT);
         m_RplComponent = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
     }
 }