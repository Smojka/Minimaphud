

 [BaseContainerProps(), SCR_BaseManualCameraComponentTitle()]
 class SCR_CampaignBuildingLimitCylinderManualCameraComponent : SCR_LimitCylinderManualCameraComponent
 {
     [Attribute(defvalue: "1", desc: "Multiplicator of camera height")]
     protected float m_fHeightMultiplicator;

     [Attribute(defvalue: "1", desc: "Multiplicator of camera range")]
     protected float m_fRangeMultiplicator;

     //------------------------------------------------------------------------------------------------
     override bool EOnCameraInit()
     {
         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (!core)
             return false;

         SCR_EditorManagerEntity editorManager = core.GetEditorManager();
         if (!editorManager)
             return false;

         SCR_EditorModeEntity modeEntity = editorManager.FindModeEntity(EEditorMode.BUILDING);
         if (!modeEntity)
             return false;

         SCR_CampaignBuildingEditorComponent buildingComponent = SCR_CampaignBuildingEditorComponent.Cast(modeEntity.FindComponent(SCR_CampaignBuildingEditorComponent));
         if (!buildingComponent)
             return false;

         SCR_FreeRoamBuildingClientTriggerEntity areaTrigger = SCR_FreeRoamBuildingClientTriggerEntity.Cast(buildingComponent.GetTrigger());
         if (!areaTrigger)
             return false;

         m_fRadius = areaTrigger.GetSphereRadius() * m_fRangeMultiplicator;
         m_fHeightMinor = areaTrigger.GetSphereRadius() * m_fHeightMultiplicator;
         super.EOnCameraInit();
         return true;
     }
 }