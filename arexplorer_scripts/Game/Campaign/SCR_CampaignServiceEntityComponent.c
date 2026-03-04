 #define ENABLE_BASE_DESTRUCTION
 [EntityEditorProps(category: "GameScripted/Campaign", description: "This component handles a destruction of entity in composition and how it affects composition operability.")]
 class SCR_CampaignServiceEntityComponentClass : ScriptComponentClass
 {
 }

 class SCR_CampaignServiceEntityComponent : ScriptComponent
 {
     [Attribute("50", UIWidgets.Slider, "How much of service operability this entity represents", "0 100 1")]
     protected int m_iEntityOperabilityPart;

     [Attribute("50", UIWidgets.Slider, "Cost of repair", "0 1000 1")]
     protected int m_iRepairCost;

     protected SCR_CampaignServiceCompositionComponent m_ServiceCompositionComp;
     protected SCR_DestructionMultiPhaseComponent m_DestMultiComp;
     protected bool m_bCanEvaluateHit = true;
     protected RplComponent m_RplComponent;
     private static const int INITIAL_STATE = 0;

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         SetEventMask(owner, EntityEvent.INIT);
     }

     //------------------------------------------------------------------------------------------------
     override void EOnInit(IEntity owner)
     {
         m_DestMultiComp = SCR_DestructionMultiPhaseComponent.Cast(owner.FindComponent(SCR_DestructionMultiPhaseComponent));
         if (!m_DestMultiComp)
             return;
 #ifdef ENABLE_BASE_DESTRUCTION
         m_DestMultiComp.GetOnDamageInvoker().Insert(EvaluateHit);
 #endif

         IEntity parentComp = SCR_EntityHelper.GetMainParent(owner);
         if (!parentComp)
             return;

         m_ServiceCompositionComp = SCR_CampaignServiceCompositionComponent.Cast(parentComp.FindComponent(SCR_CampaignServiceCompositionComponent));
         m_RplComponent = RplComponent.Cast(parentComp.FindComponent(RplComponent));
         ClearEventMask(owner, EntityEvent.INIT);
     }

     //------------------------------------------------------------------------------------------------
  void EvaluateHit()
     {
         if (IsProxy())
             return;

         if (!m_bCanEvaluateHit || !m_DestMultiComp || !m_ServiceCompositionComp)
             return;

 #ifdef ENABLE_BASE_DESTRUCTION
         if (m_DestMultiComp.GetDestroyed())
         {
             SetHitEvaluation(false);
             m_ServiceCompositionComp.ChangeOperability(m_iEntityOperabilityPart);
             m_DestMultiComp.ReplicateDestructibleState();
         }
 #endif
     }

 #ifdef ENABLE_BASE_DESTRUCTION
     //------------------------------------------------------------------------------------------------
  void RepairEntity()
     {
         if (!m_DestMultiComp || !m_ServiceCompositionComp)
             return;

         m_DestMultiComp.GoToDamagePhase(INITIAL_STATE, false);
         m_DestMultiComp.ReplicateDestructibleState(true);
         m_ServiceCompositionComp.ChangeOperability(-m_iEntityOperabilityPart);
         SetHitEvaluation(true);
     }
 #endif

     //------------------------------------------------------------------------------------------------
  bool IsProxy()
     {
         return (m_RplComponent && m_RplComponent.IsProxy());
     }

     //------------------------------------------------------------------------------------------------
  int GetRepairCost()
     {
         return m_iRepairCost;
     }

     //------------------------------------------------------------------------------------------------
  void SetHitEvaluation(bool canEvaluate)
     {
         m_bCanEvaluateHit = canEvaluate;
     }
 }