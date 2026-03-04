
 class SCR_BaseEditableEntityUIComponent : ScriptedWidgetComponent
 {
     [Attribute(desc: "When true, the icon will remain visible on edge of the screen when it's not in the view.")]
     private bool m_bShowOffScreen;

     private Widget m_Widget;
     protected SCR_EditableEntityComponent m_Entity;
     private WorkspaceWidget m_Workspace;
     private float m_fFade;

     //------------------------------------------------------------------------------------------------
  void OnRefresh(SCR_EditableEntityBaseSlotUIComponent slot);

     //------------------------------------------------------------------------------------------------
  void OnInit(SCR_EditableEntityComponent entity, SCR_UIInfo info, SCR_EditableEntityBaseSlotUIComponent slot);

     //------------------------------------------------------------------------------------------------
  void OnShownOffScreen(bool offScreen);

     //------------------------------------------------------------------------------------------------
  void Init(SCR_EditableEntityComponent entity, SCR_EditableEntityBaseSlotUIComponent slot)
     {
         if (entity)
         {
             m_Entity = entity;
             OnInit(entity, entity.GetInfo(), slot)
         }
     }

     //------------------------------------------------------------------------------------------------
     sealed void Init(SCR_UIInfo info, SCR_EditableEntityBaseSlotUIComponent slot)
     {
         if (info)
             OnInit(null, info, slot)
     }

     //------------------------------------------------------------------------------------------------
  void Exit(SCR_EditableEntityBaseSlotUIComponent slot)
     {
         if (m_Widget)
         {
             m_Widget.RemoveFromHierarchy();
             m_Widget = null;
         }
     }
     //------------------------------------------------------------------------------------------------
  SCR_EditableEntityComponent GetEntity()
     {
         return m_Entity;
     }

     //------------------------------------------------------------------------------------------------
     Widget GetWidget()
     {
         return m_Widget;
     }

     //------------------------------------------------------------------------------------------------
  bool IsVisible()
     {
         return m_Widget && m_Widget.IsVisible();
     }

     //------------------------------------------------------------------------------------------------
  void SetVisible(bool visible)
     {
         m_Widget.SetVisible(visible);
         m_Widget.SetEnabled(visible);
     }

 //  //------------------------------------------------------------------------------------------------
 //  //! Get faction of editable entity.
 //  //! \param[in] owner Entity which will be checked (by default the entity the widget represents)
 //  //! \return Faction
 //  Faction GetFaction(GenericEntity owner = null)
 //  {
 //      if (!owner && m_Entity)
 //          owner = m_Entity.GetOwner();
 //
 //      if (!owner)
 //          return null;
 //
 //      FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(owner.FindComponent(FactionAffiliationComponent));
 //      if (!factionComponent)
 //          return null;
 //
 //      return factionComponent.GetAffiliatedFaction();
 //  }

     //------------------------------------------------------------------------------------------------
  bool IsShownOffScreen()
     {
         return m_bShowOffScreen;// || m_Entity.HasEntityState(EEditableEntityState.CURRENT_LAYER_CHILDREN);
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         m_Widget = w;
         m_Workspace = GetGame().GetWorkspace();
         m_fFade = 0;
     }
 }