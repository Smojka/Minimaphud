
 class SCR_EditableEntityBaseSlotUIComponent : ScriptedWidgetComponent
 {
     protected Widget m_Widget;
     protected SCR_EditableEntityComponent m_Entity;
     protected ref map<SCR_EntitiesEditorUIRule, SCR_BaseEditableEntityUIComponent> m_EntityWidgets = new map<SCR_EntitiesEditorUIRule, SCR_BaseEditableEntityUIComponent>();

     //------------------------------------------------------------------------------------------------
  bool IsEmpty()
     {
         return m_EntityWidgets.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditableEntityComponent GetEntity()
     {
         return m_Entity;
     }

     //------------------------------------------------------------------------------------------------
     Widget CreateWidget(SCR_EditableEntityUIInfo info, SCR_EntitiesEditorUIRule rule)
     {
         ResourceName layout = rule.GetLayout(info.GetEntityType());
         SCR_BaseEditableEntityUIComponent entityUI = CreateWidgetFromLayout(layout);
         entityUI.Init(info, this);
         return entityUI.GetWidget();
     }

     //------------------------------------------------------------------------------------------------
     Widget CreateWidget(SCR_EditableEntityComponent entity, SCR_EntitiesEditorUIRule rule)
     {
         ResourceName layout = rule.GetLayout(entity.GetEntityType());

         return CreateWidgetForEntity(entity, rule, layout);
     }

     //------------------------------------------------------------------------------------------------
     Widget CreateWidget(SCR_EditableEntityComponent entity, ResourceName layout)
     {
         return CreateWidgetForEntity(entity, null, layout);
     }

     //------------------------------------------------------------------------------------------------
     protected Widget CreateWidgetForEntity(SCR_EditableEntityComponent entity, SCR_EntitiesEditorUIRule rule, ResourceName layout)
     {
         //--- There is already a widget for this rule, ignore
         if (m_EntityWidgets.Contains(rule))
             return null;

         if (!layout)
             return null;

         SCR_BaseEditableEntityUIComponent entityUI = CreateWidgetFromLayout(layout);
         entityUI.Init(entity, this);
         m_EntityWidgets.Insert(rule, entityUI);

         return entityUI.GetWidget();
     }

     //------------------------------------------------------------------------------------------------
     protected SCR_BaseEditableEntityUIComponent CreateWidgetFromLayout(ResourceName layout)
     {
         Widget entityWidget = GetGame().GetWorkspace().CreateWidgets(layout, m_Widget);
         if (!entityWidget)
         {
             Debug.Error2(Type().ToString(), string.Format("Unable to create widget '%1'!\n", layout));
             return null;
         }

         //--- Get UI component
         SCR_BaseEditableEntityUIComponent entityUI = SCR_BaseEditableEntityUIComponent.Cast(entityWidget.FindHandler(SCR_BaseEditableEntityUIComponent));
         if (!entityUI)
         {
             Debug.Error2(Type().ToString(), string.Format("SCR_BaseEditableEntityUIComponent component is missing in the root widget of '%1'!\n", layout));
             if (entityWidget)
                 entityWidget.RemoveFromHierarchy();

             return null;
         }
         return entityUI;
     }

     //------------------------------------------------------------------------------------------------
  void DeleteWidget(SCR_EntitiesEditorUIRule rule)
     {
         SCR_BaseEditableEntityUIComponent entityUI;
         if (!m_EntityWidgets.Find(rule, entityUI))
             return;

         entityUI.Exit(this);
         m_EntityWidgets.Remove(rule);
     }

     //------------------------------------------------------------------------------------------------
     protected void SetFactionColor()
     {
         if (!m_Entity)
         {
             GetWidget().SetColor(UIColors.EDITOR_ICON_COLOR_NEUTRAL);
             return;
         }

         Faction faction = m_Entity.GetFaction();
         if (faction)
             GetWidget().SetColor(faction.GetFactionColor());
         else if (m_Entity.HasEntityState(EEditableEntityState.DESTROYED))
             GetWidget().SetColor(UIColors.EDITOR_ICON_COLOR_DESTROYED);
         else
             GetWidget().SetColor(UIColors.EDITOR_ICON_COLOR_NEUTRAL);
     }

     //------------------------------------------------------------------------------------------------
     protected void RefreshSlot()
     {
         foreach (SCR_EntitiesEditorUIRule rule, SCR_BaseEditableEntityUIComponent entityUI: m_EntityWidgets)
         {
             entityUI.OnRefresh(this);
         }

         GetGame().GetCallqueue().CallLater(SetFactionColor);
     }

     //------------------------------------------------------------------------------------------------
     protected void ResetSlot()
     {
         foreach (SCR_EntitiesEditorUIRule rule, SCR_BaseEditableEntityUIComponent entityUI: m_EntityWidgets)
         {
             DeleteWidget(rule);
             CreateWidget(m_Entity, rule);
         }

         GetGame().GetCallqueue().CallLater(SetFactionColor);
     }

     //------------------------------------------------------------------------------------------------
     vector UpdateSlot(int screenW, int screenH, vector posCenter, vector posCam)
     {
         return vector.Zero;
     }

     //------------------------------------------------------------------------------------------------
  void InitSlot(SCR_EditableEntityComponent entity)
     {
         if (!entity || !entity.GetOwner())
             return;

         m_Entity = entity;

         //--- Initialise faction color (set on the slot, child widgets inherit from it, so they don't need to set it individually)
         if (entity.HasEntityFlag(EEditableEntityFlag.HAS_FACTION))
             SetFactionColor();

         if (entity.GetOnUIRefresh())
             entity.GetOnUIRefresh().Insert(RefreshSlot);

         if (entity.GetOnUIReset())
             entity.GetOnUIReset().Insert(ResetSlot);
     }

     //------------------------------------------------------------------------------------------------
  void DeleteSlot()
     {
         if (m_Widget)
         {
             m_Widget.RemoveFromHierarchy();
             m_Widget = null;
         }

         if (m_Entity)
         {
             if (m_Entity.GetOnUIRefresh())
                 m_Entity.GetOnUIRefresh().Remove(RefreshSlot);

             if (m_Entity.GetOnUIReset())
                 m_Entity.GetOnUIReset().Remove(ResetSlot);
         }
     }

     //------------------------------------------------------------------------------------------------
  bool IsPreview()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
     Widget GetWidget()
     {
         return m_Widget;
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         m_Widget = w;
     }
 }