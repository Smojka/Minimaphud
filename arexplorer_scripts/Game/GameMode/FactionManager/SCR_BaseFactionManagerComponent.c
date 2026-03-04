 [ComponentEditorProps(category: "GameScripted/FactionManager/Components", description: "Base for FactionManager scripted component.")]
 class SCR_BaseFactionManagerComponentClass : ScriptComponentClass
 {
 }

 class SCR_BaseFactionManagerComponent : ScriptComponent
 {
     //------------------------------------------------------------------------------------------------
  void OnFactionsInit(array<Faction> factions);

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_BaseFactionManagerComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
         if (!ent.Type().IsInherited(SCR_FactionManager))
             Print(string.Format("SCR_BaseFactionManagerComponent must be on SCR_FactionManager, it's on %1!", ent.Type()), LogLevel.WARNING);
     }
 }