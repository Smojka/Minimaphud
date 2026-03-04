 [ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", insertable: false)]
 class SCR_EditableEntityBaseChildComponentClass : ScriptComponentClass
 {
 }


 class SCR_EditableEntityBaseChildComponent : ScriptComponent
 {
     [Attribute("0", UIWidgets.Flags, "", enums: ParamEnumArray.FromEnum(EEditableEntityState))]
     protected EEditableEntityState m_State;

     protected GenericEntity m_Owner;

     //------------------------------------------------------------------------------------------------
  void EOnStateChanged(EEditableEntityState states, EEditableEntityState changedState, bool toSet);

     //------------------------------------------------------------------------------------------------
  void UpdateFromCurrentState()
     {
         //--- Call event on existing states
         IEntity parentTemp = m_Owner;
         SCR_EditableEntityComponent entity;
         while (parentTemp)
         {
             GenericEntity parentGeneric = GenericEntity.Cast(parentTemp);
             if (parentGeneric)
             {
                 entity = SCR_EditableEntityComponent.Cast(parentGeneric.FindComponent(SCR_EditableEntityComponent));
                 if (entity)
                 {
                     //--- Mark the entity as virtual
                     if (!entity.HasEntityFlag(EEditableEntityFlag.VIRTUAL))
                         entity.SetEntityFlag(EEditableEntityFlag.VIRTUAL, true);

                     //--- Get all active states
                     EEditableEntityState states = entity.GetEntityStates();
                     EOnStateChanged(states, states, true);
                     break;
                 }
             }
             parentTemp = parentTemp.GetParent();
         }
     }

     //------------------------------------------------------------------------------------------------
  bool CanApply(EEditableEntityState changedState)
     {
         return (m_State & changedState) && m_Owner;
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_EditableEntityBaseChildComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
         m_Owner = GenericEntity.Cast(ent);
     }
 }