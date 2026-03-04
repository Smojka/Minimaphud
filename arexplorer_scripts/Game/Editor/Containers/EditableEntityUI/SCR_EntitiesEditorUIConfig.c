
 [BaseContainerProps(configRoot: true)]
 class SCR_EditableEntityUIConfig
 {
     [Attribute()]
     protected ref array<ref SCR_EntitiesEditorUIRule> m_aRules;

     array<ref SCR_EntitiesEditorUIRule> GetRules()
     {
         return m_aRules;
     }

     static SCR_EditableEntityUIConfig GetConfig(SCR_EditableEntityUIConfig config = null)
     {
         if (config)
             return config;

         SCR_MenuLayoutEditorComponent layoutManager = SCR_MenuLayoutEditorComponent.Cast(SCR_MenuLayoutEditorComponent.GetInstance(SCR_MenuLayoutEditorComponent, true));
         if (layoutManager)
             return layoutManager.GetEditableEntityUI();
         else
             return null;
     }
 };