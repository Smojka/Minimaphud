 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Component for nametag config selection")]
 class SCR_NametagConfigComponentClass : ScriptComponentClass
 {}

 //------------------------------------------------------------------------------------------------
 class SCR_NametagConfigComponent : ScriptComponent
 {
     [Attribute("", UIWidgets.ResourceNamePicker, desc: "Nametag config", "conf class=SCR_NameTagConfig")]
     protected ResourceName m_sConfigPath;

     //------------------------------------------------------------------------------------------------
     ResourceName GetConfigPath()
     {
         return m_sConfigPath;
     }

 };