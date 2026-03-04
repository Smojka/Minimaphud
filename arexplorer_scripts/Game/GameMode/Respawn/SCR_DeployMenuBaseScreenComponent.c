 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Base for the deploy menu screen components")]
 class SCR_DeployMenuBaseScreenComponentClass : SCR_BaseGameModeComponentClass
 {
 }

 class SCR_DeployMenuBaseScreenComponent : SCR_BaseGameModeComponent
 {
     [Attribute()]
     protected string m_sHeaderTitle;

     [Attribute()]
     protected string m_sHeaderSubtitle;

     [Attribute()]
     protected ref SCR_DeployMenuBaseScreenLayout m_BaseLayout;

     //------------------------------------------------------------------------------------------------
  string GetHeaderTitle()
     {
         return m_sHeaderTitle;
     }

     //------------------------------------------------------------------------------------------------
  string GetHeaderSubtitle()
     {
         return m_sHeaderSubtitle;
     }

     //------------------------------------------------------------------------------------------------
  SCR_DeployMenuBaseScreenLayout GetBaseLayout()
     {
         return m_BaseLayout;
     }
 }

 [BaseContainerProps(), SCR_ContainerActionTitle()]
 class SCR_DeployMenuBaseScreenLayout : ScriptAndConfig
 {
     protected ref array<ref SCR_WelcomeScreenBaseContent> m_aScreenBaseContents = {};

     //------------------------------------------------------------------------------------------------
  void InitContent(SCR_WelcomeScreenMenu menu);

     //------------------------------------------------------------------------------------------------
  int GetScreenBaseContents(out array<ref SCR_WelcomeScreenBaseContent> screenBaseContents)
     {
         screenBaseContents = m_aScreenBaseContents;

         return m_aScreenBaseContents.Count();
     }
 }