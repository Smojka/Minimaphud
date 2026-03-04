 [EntityEditorProps(category: "GameScripted/Editor", description: "World settings for the editor", color: "251 91 0 255", icon: "WBData/EntityEditorProps/entityEditor.png")]
 class SCR_EditorSettingsEntityClass: SCR_EditorBaseEntityClass
 {
 };

 class SCR_EditorSettingsEntity : SCR_EditorBaseEntity
 {
     [Attribute(uiwidget: UIWidgets.ResourceNamePicker, desc: "Individual editor manager", params: "et", category: "Editor Settings")]
     protected ResourceName m_EditorManagerPrefab;

     [Attribute(desc: "True to disable the editor completely.\nFOR DEVELOPMENT PURPOSES ONLY!", category: "Editor Settings")]
     protected bool m_bDisableEditor;

     [Attribute(desc: "When enabled, using the editor in this scenario is a legal operation, part of game rules.\nFor example this is enabled in Game Master scenarios which are about GM building the gameplay,\nbut it's disabled in Conflict where only admin can access the editor.", category: "Editor Settings")]
     protected bool m_bIsUnlimitedEditorLegal;

     [Attribute(desc: "When enabled, each player will receive editor modes defined by 'Base Modes' attribute instead of the modes defined in core config.", category: "Editor Settings")]
     protected bool m_bOverrideBaseModes;

     [Attribute("0", UIWidgets.Flags, "Editor modes added to every pllayer when they connect.", enums: ParamEnumArray.FromEnum(EEditorMode), category: "Editor Settings")]
     protected EEditorMode m_BaseModes;

     static SCR_EditorSettingsEntity GetInstance()
     {
         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (core)
             return core.GetSettingsEntity();
         else
             return null;
     }

     ResourceName GetPrefab(ResourceName basePrefab)
     {
         if (m_EditorManagerPrefab.IsEmpty()) return basePrefab;
         return m_EditorManagerPrefab;
     }

     bool IsEditorDisabled()
     {
         return m_bDisableEditor;
     }

     bool IsUnlimitedEditorLegal()
     {
         return m_bIsUnlimitedEditorLegal;
     }

     void EnableBaseOverride(bool value)
     {
         m_bOverrideBaseModes = value;
     }
     bool IsBaseOverrideEnabled()
     {
         return m_bOverrideBaseModes;
     }
     bool GetBaseModes(out EEditorMode modes = 0)
     {
         modes = m_BaseModes;
         return m_bOverrideBaseModes;
     }
     void SetBaseModes(EEditorMode modes, bool validate = true)
     {
         m_BaseModes = modes;

         //--- Validate all existing editor modes
         if (validate)
         {
             SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
             if (core)
             {
                 array<SCR_EditorManagerEntity> editorManagers = {};
                 core.GetEditorEntities(editorManagers);
                 foreach (SCR_EditorManagerEntity editorManager: editorManagers)
                 {
                     editorManager.SetEditorModes(EEditorModeAccess.BASE, m_BaseModes, false);
                 }
             }
         }
     }

     void SCR_EditorSettingsEntity(IEntitySource src, IEntity parent)
     {
         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (core) core.SetSettingsEntity(this);
     }
 };