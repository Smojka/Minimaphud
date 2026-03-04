 [ComponentEditorProps(category: "GameScripted/Editor", description: "Manager of Command actions in editor!", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_CommandActionsEditorComponentClass: SCR_BaseActionsEditorComponentClass
 {
 };
 class SCR_CommandActionsEditorComponent : SCR_BaseActionsEditorComponent
 {
     [Attribute("", uiwidget: UIWidgets.ComboBox, "When placing command instantly (e.g., Alt+RMB), choose the first action in one of these groups.\nWhen empty, all groups will be considered.", category: "Editor Command Actions", enums: ParamEnumArray.FromEnum(EEditorActionGroup))]
     protected ref array<EEditorActionGroup> m_DefaultActionGroups;

     protected SCR_BaseEditorAction m_CurrentAction;

     protected ref ScriptInvoker Event_OnCurrentActionChanged = new ScriptInvoker();

     ScriptInvoker GetOnCurrentActionChanged()
     {
         return Event_OnCurrentActionChanged;
     }
     bool IsActionCurrent(SCR_BaseEditorAction action)
     {
         return action == m_CurrentAction;
     }
     void SetCurrentAction(SCR_BaseEditorAction currentAction)
     {
         m_CurrentAction = currentAction;
         Event_OnCurrentActionChanged.Invoke();
     }

     void PerformDefaultAction(bool isQueue)
     {
         EEditorCommandActionFlags flags = 0;
         if (isQueue)
         {
             flags |= EEditorCommandActionFlags.IS_QUEUE;
         }

         vector position;
         array<ref SCR_EditorActionData> actions = {};
         for (int i = 0, count = GetAndEvaluateActions(position, actions); i < count; i++)
         {
             if (m_DefaultActionGroups.IsEmpty() || m_DefaultActionGroups.Contains(actions[i].GetAction().GetActionGroup()))
             {
                 SCR_BaseEditorAction action = actions[i].GetAction();
                 ActionPerformInstantly(action, flags);
                 break;
             }
         }
     }
     void StartPlacing(SCR_BaseEditorAction action)
     {
         SCR_BaseCommandAction commandAction = SCR_BaseCommandAction.Cast(action);
         if (commandAction && commandAction.StartPlacing(m_SelectedEntities))
         {
             SetCurrentAction(action);
         }
     }

     protected void OnPlacingSelectedPrefabChange(ResourceName prefab, ResourceName prefabPrev)
     {
         //--- Reset variables when placing ends
         if (prefab.IsEmpty())
         {
             SetCurrentAction(null);
         }
     }

     override int ValidateSelection(bool isInstant)
     {
         if (m_HoverManager)
             m_HoveredEntity = m_HoverManager.GetFirstEntity();
         else
             m_HoveredEntity = null;

         if (m_SelectedManager)
             m_SelectedManager.GetEntities(m_SelectedEntities);
         else
             m_SelectedEntities.Clear();

         EEditorCommandActionFlags flags;
         EEditableEntityType type;
         bool isTaskManager = GetTaskManager() != null;
         foreach (SCR_EditableEntityComponent entity: m_SelectedEntities)
         {
             type = entity.GetEntityType();

             //--- At least one group is selected
             if (entity.GetAIGroup() && !(flags & EEditorCommandActionFlags.OBJECTIVE))
             {
                 flags = flags | EEditorCommandActionFlags.WAYPOINT;
             }

             //--- A faction is selected (placing objectives in that case)
             if (isTaskManager && type == EEditableEntityType.FACTION)// || entity.HasEntityState(EEditableEntityState.PLAYER))
             {
                 flags = flags | EEditorCommandActionFlags.OBJECTIVE;
                 flags = flags & ~EEditorCommandActionFlags.WAYPOINT;
             }
         }

         return flags;
     }
     override protected TraceFlags GetInstantActionTraceFlags()
     {
         return TraceFlags.WORLD | TraceFlags.OCEAN;
     }

     override bool ActionCanBeShown(SCR_BaseEditorAction action, vector cursorWorldPosition, int flags)
     {
         return action.GetInfo() && action.CanBeShown(m_HoveredEntity, m_SelectedEntities, cursorWorldPosition, flags);
     }

     override bool ActionCanBePerformed(SCR_BaseEditorAction action, vector cursorWorldPosition, int flags)
     {
         return action.CanBePerformed(m_HoveredEntity, m_SelectedEntities, cursorWorldPosition, flags);
     }

     override void EOnEditorActivate()
     {
         super.EOnEditorActivate();

         SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent, true));
         if (placingManager)
             placingManager.GetOnSelectedPrefabChange().Insert(OnPlacingSelectedPrefabChange);
     }
     override void EOnEditorDeactivate()
     {
         super.EOnEditorDeactivate();

         SCR_PlacingEditorComponent placingManager = SCR_PlacingEditorComponent.Cast(SCR_PlacingEditorComponent.GetInstance(SCR_PlacingEditorComponent));
         if (placingManager)
             placingManager.GetOnSelectedPrefabChange().Remove(OnPlacingSelectedPrefabChange);
     }
 };