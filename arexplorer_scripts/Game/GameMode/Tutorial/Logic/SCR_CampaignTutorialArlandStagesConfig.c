 [BaseContainerProps(configRoot: true)]
 class SCR_CampaignTutorialArlandStagesConfig
 {
     [Attribute()]
     private ref array<ref SCR_CampaignTutorialArlandStages> m_TutorialArlandStagesConfigs;

     //------------------------------------------------------------------------------------------------
  void GetConfigs(out notnull array<ref SCR_CampaignTutorialArlandStages> TutorialArlandStagesConfigs)
     {
         TutorialArlandStagesConfigs = m_TutorialArlandStagesConfigs;
     }
 }