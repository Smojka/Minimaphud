 [ComponentEditorProps(category: "GameScripted/Editor", description: "Attribute for managing attributes. Works only with SCR_EditorBaseEntity!", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_AttributesManagerEditorComponentClass : SCR_BaseEditorComponentClass
 {
     [Attribute(category: "Attributes")]
     protected ref array<ref SCR_EditorAttributeList> m_AttributeLists;

     protected ref array<SCR_BaseEditorAttribute> m_aAttributes = {};

     //------------------------------------------------------------------------------------------------
  SCR_BaseEditorAttribute GetAttribute(int index)
     {
         return m_aAttributes[index];
     }

     //------------------------------------------------------------------------------------------------
  int GetAttributesCount()
     {
         return m_aAttributes.Count();
     }

     //------------------------------------------------------------------------------------------------
  int FindAttribute(SCR_BaseEditorAttribute attribute)
     {
         return m_aAttributes.Find(attribute);
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_AttributesManagerEditorComponentClass(BaseContainer prefab)
     {
         foreach (SCR_EditorAttributeList list: m_AttributeLists)
         {
             list.InsertAllAttributes(m_aAttributes);
         }
     }
 }


 class SCR_AttributesManagerEditorComponent : SCR_BaseEditorComponent
 {
     const int SNAPSHOT_SIZE = 96;

     [Attribute(desc: "Dialog created when attributes are edited.", defvalue: "-1", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ChimeraMenuPreset))]
     private ChimeraMenuPreset m_MenuPreset;

     [Attribute(desc: "Displayed description and icon when hovering over an locked attribute. All attributes need access to this.", category: "Attributes")]
     protected ref SCR_EditorAttributeUIInfo m_ConflictingAttributeUIInfo;

     private SCR_AttributesManagerEditorComponentClass m_PrefabData;
     private SCR_StatesEditorComponent m_StatesManager;
     private bool m_bHasServerAttributes;
     private ref array<Managed> m_aEditedItems;
     private ref array<SCR_BaseEditorAttribute> m_aEditedAttributes; //~~! Class, BaseEditorAttributeVar. Get this by SetInstance
     private ref ScriptInvoker Event_OnAttributesRequest = new ScriptInvoker();
     private ref ScriptInvoker Event_OnAttributesStart = new ScriptInvoker();
     private ref ScriptInvoker Event_OnAttributesConfirm = new ScriptInvoker();
     private ref ScriptInvoker Event_OnAttributesCancel = new ScriptInvoker();
     private ref ScriptInvoker Event_OnResetAttributes = new ScriptInvoker();
     private ref ScriptInvoker Event_OnAttributeChangesApplied = new ScriptInvoker();
     private ref ScriptInvoker Event_OnAttributeDescriptionChanged = new ScriptInvoker();
     private ref ScriptInvoker Event_OnAttributeCategoryChanged = new ScriptInvoker();

     protected bool m_bHasChangedAttributesOnce = false;
     //protected bool m_CanOpenAttributes;

     //protected ref map<EEditorMode, int> m_mSavedGlobalAttributeTabs = new map<EEditorMode, int>;
     protected ResourceName m_CurrentCategory;

     //--- StartEditing

     //------------------------------------------------------------------------------------------------
  void StartEditing(Managed item)
     {
         if (!item || item.IsInherited(array) || item.IsInherited(set) || item.IsInherited(map)) //--- Lists themselves are Managed
         {
             if (!item)
             {
                 array<Managed> items = {};
                 StartEditing(items);
                 Print("Opening attributes with NULL entity!", LogLevel.WARNING);
                 return;
             }

             if (item)
                 Print(string.Format("Cannot edit attributes of %1, must be explicitly array<Managed> (even when the array element itself inherits from Managed)!", item.Type()), LogLevel.ERROR);
             return;
         }

         array<Managed> items = {item};
         StartEditing(items);
     }

     //------------------------------------------------------------------------------------------------
  void StartEditing(notnull array<Managed> items, bool onlyServer = true)
     {
         //--- Not an owner, ignore
         if (!m_PrefabData || !IsOwner())
             return;

         //--- Already editing, ignore
         if (m_aEditedItems)
             return;

         if (m_StatesManager)
             m_StatesManager.SetIsWaiting(true);

         Event_OnAttributesRequest.Invoke(items);

         if (onlyServer && m_bHasServerAttributes)
         {
             //--- Editing server attributes, ask for values
             array<int> itemIds = {};
             m_aEditedItems = {};
             foreach (Managed item: items)
             {
                 int id = Replication.FindId(item);
                 if (id != -1 && m_aEditedItems.Find(item) == -1)
                 {
                     m_aEditedItems.Insert(item);
                     itemIds.Insert(id);
                 }
             }
             //StartEditingServer(itemIds);
             Rpc(StartEditingServer, itemIds);
         }
         else
         {
             //--- Editing only local attributes, skip server communication
             m_aEditedItems = {};
             foreach (Managed item: items)
             {
                 if (m_aEditedItems.Find(item) == -1)
                     m_aEditedItems.Insert(item);
             }

             StartEditingOwner({}, {}, {});
         }
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void StartEditingServer(array<int> itemIds)
     {
         m_aEditedItems = {};
         foreach (int id: itemIds)
         {
             Managed item = Replication.FindItem(id);
             if (item)
                 m_aEditedItems.Insert(item);
         }

         //--- Get attribute values from server
         array<int> attributesIds = {};
         array<ref SCR_BaseEditorAttributeVar> attributesVars = {};
         array<ref EEditorAttributeMultiSelect> attributesMultiSelect = {};
         GetVariables(true, m_aEditedItems, attributesIds, attributesVars, attributesMultiSelect);

         //--- Remember edited attributes
         m_aEditedAttributes = {};
         foreach (int i, int attributeId: attributesIds)
         {
             SCR_BaseEditorAttribute attribute = m_PrefabData.GetAttribute(attributeId);

             if (!attribute.BoolAllowDuplicate() && GetIsAttributeDuplicate(attribute.Type()))
             {
                 Print(string.Format("Trying to add editor attribute of type '%1' but there is already an attribute of the same type", attribute.Type()), LogLevel.ERROR);
                 continue;
             }

             m_aEditedAttributes.Insert(attribute);
         }

         //Print("Sending attributes:");
         //attributesVars.Debug();

         //StartEditingOwner(attributesIds, attributesVars);
         int simulatedDelay = DiagMenu.GetValue(SCR_DebugMenuID.DEBUGUI_EDITOR_NETWORK_DELAY) * 100;
         if (simulatedDelay > 0 && !Replication.IsRunning())
             GetGame().GetCallqueue().CallLater(StartEditingOwner, simulatedDelay, false, attributesIds, attributesVars, attributesMultiSelect);
         else
             Rpc(StartEditingOwner, attributesIds, attributesVars, attributesMultiSelect);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     protected void StartEditingOwner(notnull array<int> attributesIds, notnull array<ref SCR_BaseEditorAttributeVar> attributesVars, notnull array<ref EEditorAttributeMultiSelect> attributesMultiSelect)
     {
         //--- Current menu changed while waiting, ignore
         if (m_StatesManager && !m_StatesManager.SetIsWaiting(false))
         {
             Clean();
             return;
         }

         //Print("Receiving attributes:");
         //attributesVars.Debug();

         //--- Get attribute values from client (add them to those received from the server)
         GetVariables(false, m_aEditedItems, attributesIds, attributesVars, attributesMultiSelect);

         //--- Process all edited attributes
         m_aEditedAttributes = {};
         foreach (int i, int attributeId: attributesIds)
         {
             //-- Register to an array that is sent to the invoker
             SCR_BaseEditorAttribute attribute = m_PrefabData.GetAttribute(attributeId);
             SCR_BaseEditorAttributeVar var = attributesVars[i];
             EEditorAttributeMultiSelect attributeMultiSelect = attributesMultiSelect[i];

             if (!attribute.BoolAllowDuplicate() && GetIsAttributeDuplicate(attribute.Type()))
             {
                 Print(string.Format("Trying to add editor attribute of type '%1' but there is already an attribute of the same type", attribute.Type()), LogLevel.ERROR);
                 continue;
             }

             attribute.SetIsMultiSelect(attributeMultiSelect & EEditorAttributeMultiSelect.MULTI_SELECT);
             attribute.SetHasConflictingValues(attributeMultiSelect & EEditorAttributeMultiSelect.COLLIDING_ATTRIBUTE);

             m_aEditedAttributes.Insert(attribute);

             //--- Create snapshot for later comparison
             SSnapshot snapshot = null;
             if (var)
             {
                 snapshot = new SSnapshot(SNAPSHOT_SIZE);
                 SSnapSerializer snapWriter = SSnapSerializer.MakeWriter(snapshot);
                 SCR_BaseEditorAttributeVar.Extract(var, null, snapWriter);
             }
             attribute.StartEditing(var, snapshot);
         }

         //--- Open menu
         GetGame().GetMenuManager().OpenDialog(m_MenuPreset);

         //--- Invoke an event for other systems
         Event_OnAttributesStart.Invoke(m_aEditedAttributes);

         foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
         {
             attribute.UpdateInterlinkedVariables(attribute.GetVariable(), this, true);
         }
     }

     //------------------------------------------------------------------------------------------------
     // Checks if the given attribute is a dupplicant
     protected bool GetIsAttributeDuplicate(typename type)
     {
         //Ignore Attributes that can be duplicated
         if (type.IsInherited(SCR_BaseDuplicatableEditorAttribute))
             return false;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
                 return true;
         }

         return false;
     }

     //--- ConfirmEditing

     //------------------------------------------------------------------------------------------------
  void ConfirmEditing()
     {
         //--- Not an owner, ignore
         if (!IsOwner())
             return;

         //--- Not editing, ignore
         if (!m_aEditedItems)
             return;

         SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
         int PlayerID = -1;

         if (playerController)
             PlayerID = playerController.GetPlayerId();

         bool attributeChangesApplied = false;

         //--- Apply changed values
         array<int> attributesIds = {};
         array<ref SCR_BaseEditorAttributeVar> attributesVars = {};
         foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
         {
             SCR_BaseEditorAttributeVar var = attribute.GetVariable();
             SSnapshot snapshot = attribute.GetSnapshot();

             //--- Non-shared value wasn't set, skip
             if (!var)
                 continue;

             //~ Attribute is not enabled so skip
             if (!attribute.IsEnabled())
                 continue;

             //~ If attribute edits multiple entities and the value on the entities differ from eachother and that value is being changed overridden (even if it might be the same as some entities) then consider the value changed
             //~ Else check if attribute was changed using snapshot
             if (!attribute.GetIsMultiSelect() || !attribute.GetHasConflictingValues() || !attribute.GetIsOverridingValues())
             {
                 //--- Value didn't change compared to snapshot, skip
                 if (snapshot)
                 {
                     SSnapSerializer snapReader = SSnapSerializer.MakeReader(snapshot);
                     if (SCR_BaseEditorAttributeVar.PropCompare(var, snapReader, null))
                         continue;
                 }
             }

             if (attribute.IsServer())
             {
                 //--- Add to the list to be sent to server
                 attributesIds.Insert(m_PrefabData.FindAttribute(attribute));
                 attributesVars.Insert(var);
             }
             else
             {
                 //--- Apply locally
                 foreach (Managed item: m_aEditedItems)
                 {
                     if (attribute.ReadVariable(item, this))
                         attribute.WriteVariable(item, var, this, PlayerID);
                 }
             }

             attributeChangesApplied = true;
         }

         //--- Send to server
         if (!attributesIds.IsEmpty())
         {
             //ConfirmEditingServer(attributesIds, attributesVars);
             Rpc(ConfirmEditingServer, attributesIds, attributesVars, PlayerID);
         }

         Event_OnAttributesConfirm.Invoke(m_aEditedAttributes);
         Clean();

         if (attributeChangesApplied)
         {
             Event_OnAttributeChangesApplied.Invoke();

             if (!m_bHasChangedAttributesOnce)
                 m_bHasChangedAttributesOnce = true;
         }
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void ConfirmEditingServer(notnull array<int> attributesIds, notnull array<ref SCR_BaseEditorAttributeVar> attributesVars, int PlayerID)
     {
         if (!m_PrefabData)
             return;

         foreach (int i, int attributeId: attributesIds)
         {
             SCR_BaseEditorAttribute attribute = m_PrefabData.GetAttribute(attributeId);
             SCR_BaseEditorAttributeVar var = attributesVars[i];
             foreach (Managed item: m_aEditedItems)
             {
                 if (attribute.ReadVariable(item, this))
                 {
                     attribute.WriteVariable(item, var, this, PlayerID);

                     //--- Mark editable entities for serialization if their attributes were modified
                     SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
                     if (editableEntity)
                         editableEntity.SetHierarchyAsDirtyInParents();
                 }
             }
         }
         Event_OnAttributesConfirm.Invoke(m_aEditedAttributes);
         Clean();
     }

     //--- Cancel

     //------------------------------------------------------------------------------------------------
  void CancelEditing()
     {
         //--- Not an owner, ignore (not used, would kill the process when the world is shutting down and the owner is already null)
         //if (!IsOwner()) return;

         //--- Not editing, ignore
         if (!m_aEditedItems)
             return;

         //--- Restore original values
         Reset(false);

         if (!IsRemoved())
             Rpc(CancelEditingServer); //--- Send Rpc only when the entity is not being deleted, e.g., when closing the game

         Event_OnAttributesCancel.Invoke(m_aEditedAttributes);
         Clean();
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void CancelEditingServer()
     {
         Event_OnAttributesCancel.Invoke(m_aEditedAttributes);
         Clean();
     }

     //--- Reset

     //------------------------------------------------------------------------------------------------
  void Reset(bool telegraphChange)
     {
         //--- Not editing, ignore
         if (!m_aEditedItems)
             return;

         //--- Restore original values
         foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
         {
             SCR_BaseEditorAttributeVar var = attribute.GetVariable();
             SSnapshot snapshot = attribute.GetSnapshot();

             if (var && snapshot)
             {
                 SSnapSerializer snapReader = SSnapSerializer.MakeReader(snapshot);
                 if (!SCR_BaseEditorAttributeVar.PropCompare(var, snapReader, null))
                 {
                     //--- Create new reader with index back at 0
                     snapReader = SSnapSerializer.MakeReader(snapshot);
                     SCR_BaseEditorAttributeVar.Inject(snapReader, null, var);

                     //--- Update GUI
                     if (telegraphChange)
                         attribute.TelegraphChange(true);

                     foreach (Managed item: m_aEditedItems)
                     {
                         if (attribute.ReadVariable(item, this))
                         {
                             attribute.PreviewVariable(false, this);
                             attribute.UpdateInterlinkedVariables(var, this);
                         }

                     }
                 }
                 //Still reset if value is conflicting or if is override var and value did not change
                 else if (attribute.GetHasConflictingValues())
                 {
                     if (telegraphChange)
                         attribute.TelegraphChange(true);
                 }
             }
             //If conflicting attribute then it will still set UI as default value and update copyVar
             else if (!var)
             {
                 if (telegraphChange)
                     attribute.TelegraphChange(true);
             }
         }

         Event_OnResetAttributes.Invoke();
     }

     //--- Support Funcions

     //------------------------------------------------------------------------------------------------
     protected void Clean()
     {
         if (m_aEditedAttributes)
         {
             foreach (SCR_BaseEditorAttribute attribute: m_aEditedAttributes)
             {
                 attribute.StopEditing();
             }
         }

         m_aEditedItems = null;
         m_aEditedAttributes = null;
     }

     //------------------------------------------------------------------------------------------------
     protected int GetVariables(bool onlyServer, notnull array<Managed> items, notnull out array<int> outIds, notnull out array<ref SCR_BaseEditorAttributeVar> outVars,  notnull out array<ref EEditorAttributeMultiSelect> outAttributesMultiSelect)
     {
         SCR_BaseEditorAttribute attribute;
         for (int i = 0, count = m_PrefabData.GetAttributesCount(); i < count; i++)
         {
             attribute = m_PrefabData.GetAttribute(i);
             if (!attribute)
                 continue;

             if (attribute.IsServer() != onlyServer)
                 continue;

             SCR_BaseEditorAttributeVar var = null;
             bool isCompatible = false;

             SSnapshot snapshot = new SSnapshot(SNAPSHOT_SIZE);
             SSnapSerializer snapWriter;
             SSnapSerializer snapReader;

             EEditorAttributeMultiSelect multiSelectState;

             foreach (Managed item: items)
             {
                 if (!item)
                     continue;

                 SCR_BaseEditorAttributeVar checkVar = attribute.ReadVariable(item, this);
                 if (!checkVar)
                     continue;

                 var = checkVar;
                 isCompatible = true;

                 if (!snapReader)
                 {
                     //--- First item, write snapshot for comparison
                     snapWriter = SSnapSerializer.MakeWriter(snapshot);
                     snapReader = SSnapSerializer.MakeReader(snapshot);
                     SCR_BaseEditorAttributeVar.Extract(var, null, snapWriter);
                 }
                 else
                 {
                     //Set is multi select
                     multiSelectState |= EEditorAttributeMultiSelect.MULTI_SELECT;

                     //--- Reset the reader so the comparison starts from the beginning
                     snapReader.Seek(0);

                     if (!SCR_BaseEditorAttributeVar.PropCompare(var, snapReader, null))
                     {
                         //--- When item don't share the same variable, don't check other items
                         multiSelectState |= EEditorAttributeMultiSelect.COLLIDING_ATTRIBUTE;
                         break;
                     }
                 }
             }

             //--- Use the attribute when at least one item has it
             if (isCompatible)
             {
                 outIds.Insert(i);
                 outVars.Insert(var);
                 outAttributesMultiSelect.Insert(multiSelectState);
             }
             multiSelectState = 0;
         }
         return outVars.Count();
     }

     //~Todo: Make sure server and non-server actions are correctly checked. Currently if only has attributes that can only be checked on server the attributes cannot be opened!
     //------------------------------------------------------------------------------------------------
  bool CanOpenAttributeDialog(notnull array<Managed> items)
     {
         SCR_BaseEditorAttribute attribute;
         for (int i = 0, count = m_PrefabData.GetAttributesCount(); i < count; i++)
         {
             attribute = m_PrefabData.GetAttribute(i);
             if (!attribute)
                 continue;
             //if (attribute.IsServer() != onlyServer)
             //  continue;

             foreach (Managed item: items)
             {
                 if (!item)
                     continue;

                 SCR_BaseEditorAttributeVar var = attribute.ReadVariable(item, this);
                 if (!var)
                     continue;

                 //As attribute so show context action
                 return true;
             }
         }

         return false;

     }

     //------------------------------------------------------------------------------------------------
  bool SetAttributeVariable(typename type, SCR_BaseEditorAttributeVar var)
     {
         SCR_BaseEditorAttribute attribute;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
             {
                 attribute = attributeEntry;
                 break;
             }
         }

         if (attribute)
         {
             return attribute.SetVariable(var);
         }
         else
         {
             Debug.Error2(type.ToString(), "Attribute type not found to set variable!");
             return false;
         }
     }

     //------------------------------------------------------------------------------------------------
  bool GetAttributeVariable(typename type, out SCR_BaseEditorAttributeVar var)
     {
         if (!m_aEditedAttributes || m_aEditedAttributes.IsEmpty())
             return false;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
             {
                 var = attributeEntry.GetVariableOrCopy();
                 if (var == null)
                     Print("SCR_AttributesManagerEditorComponent could not find the attribute var which will cause certain attributes to break", LogLevel.ERROR);

                 return var != null;
             }
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  int GetActiveAttributesOfType(typename type, notnull out array<SCR_BaseEditorAttribute> attributes, bool includedInherit = true, SCR_BaseEditorAttribute ignoreAttribute = null)
     {
         attributes.Clear();

         if (!m_aEditedAttributes || m_aEditedAttributes.IsEmpty())
             return 0;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (((includedInherit && attributeEntry.Type().IsInherited(type)) || (!includedInherit && attributeEntry.Type() == type)) && attributeEntry != ignoreAttribute)
                 attributes.Insert(attributeEntry);
         }

         return attributes.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool GetActiveAttribute(typename type, out SCR_BaseEditorAttribute attribute)
     {
         if (!m_aEditedAttributes || m_aEditedAttributes.IsEmpty())
             return false;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
             {
                 attribute = attributeEntry;

                 //Attribute can only be obtained from non server attributes as changes are local
                 return !attributeEntry.IsServer();
             }
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool SetAttributeEnabled(typename type, bool enabled)
     {
         SCR_BaseEditorAttribute attribute;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
             {
                 attribute = attributeEntry;
                 break;
             }
         }

         if (attribute)
         {
             attribute.Enable(enabled);
             return true;
         }
         else
         {
             //--- Don't show error, missing link may be legit when one attribute is used in different situations
             //Debug.Error2(type.ToString(), "Attribute type not found to set attribute enabled!");
             return false;
         }
     }

     //------------------------------------------------------------------------------------------------
  bool SetAttributeSelected(typename type, bool selected, int index = -1)
     {
         SCR_BaseEditorAttribute attribute;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
             {
                 attribute = attributeEntry;
                 break;
             }
         }

         if (attribute)
         {
             attribute.ToggleSelected(selected, index);
             return true;
         }
         else
         {
             Debug.Error2(type.ToString(), "Attribute type not found to set attriabute selected!");
             return false;
         }
     }

     //------------------------------------------------------------------------------------------------
  bool SetAttributeAsSubAttribute(typename type)
     {
         SCR_BaseEditorAttribute attribute;

         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
             {
                 attribute = attributeEntry;
                 break;
             }
         }

         if (attribute)
         {
             attribute.SetAsSubAttribute();
             return true;
         }
         else
         {
             return false;
         }
     }

     //------------------------------------------------------------------------------------------------
  SCR_BaseEditorAttribute GetAttributeRef(typename type)
     {
         foreach (SCR_BaseEditorAttribute attributeEntry: m_aEditedAttributes)
         {
             if (attributeEntry.Type() == type)
                 return attributeEntry;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  int GetEditedAttributes(out notnull array<SCR_BaseEditorAttribute> outAttributes)
     {
         return outAttributes.Copy(m_aEditedAttributes);
     }

     //------------------------------------------------------------------------------------------------
  int GetEditedItems(out notnull array<Managed> outitems)
     {
         return outitems.Copy(m_aEditedItems);
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributesRequest()
     {
         return Event_OnAttributesRequest;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributesStart()
     {
         return Event_OnAttributesStart;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributesConfirm()
     {
         return Event_OnAttributesConfirm;
     }
     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributesCancel()
     {
         return Event_OnAttributesCancel;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnResetAttributes()
     {
         return Event_OnResetAttributes;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributeChangesApplied()
     {
         return Event_OnAttributeChangesApplied;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributeDescriptionChanged()
     {
         return Event_OnAttributeDescriptionChanged;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnAttributeCategoryChanged()
     {
         return Event_OnAttributeCategoryChanged;
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditorAttributeUIInfo GetConflictingAttributeUIInfo()
     {
         return m_ConflictingAttributeUIInfo;
     }

     //------------------------------------------------------------------------------------------------
  void SetCurrentCategory(ResourceName category)
     {
         m_CurrentCategory = category;
         Event_OnAttributeCategoryChanged.Invoke(m_CurrentCategory);
     }
     //------------------------------------------------------------------------------------------------
     ResourceName GetCurrentCategory()
     {
         return m_CurrentCategory;
     }

     //------------------------------------------------------------------------------------------------
  bool GetChangedAttributesOnce()
     {
         return m_bHasChangedAttributesOnce;
     }

     //------------------------------------------------------------------------------------------------
  void SetAttributeDescription(SCR_EditorAttributeUIInfo uiInfo, string customDescription = string.Empty, string param1 = string.Empty, string param2 = string.Empty, string param3 = string.Empty)
     {
         Event_OnAttributeDescriptionChanged.Invoke(uiInfo, customDescription, param1, param2, param3);
     }

     //--- Default Functions

     //------------------------------------------------------------------------------------------------
     override void EOnEditorActivate()
     {
         m_StatesManager = SCR_StatesEditorComponent.Cast(SCR_StatesEditorComponent.GetInstance(SCR_StatesEditorComponent));
     }

     //------------------------------------------------------------------------------------------------
     override void EOnEditorDeactivate()
     {
         CancelEditing();
     }

     //------------------------------------------------------------------------------------------------
     override void EOnEditorInit()
     {
         m_PrefabData = SCR_AttributesManagerEditorComponentClass.Cast(GetEditorComponentData());

         //--- Check if any server attributes are defined
         m_bHasServerAttributes = false;
         for (int i = 0, count = m_PrefabData.GetAttributesCount(); i < count; i++)
         {
             if (m_PrefabData.GetAttribute(i).IsServer())
             {
                 m_bHasServerAttributes = true;
                 break;
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     override void EOnEditorInitServer()
     {
         m_PrefabData = SCR_AttributesManagerEditorComponentClass.Cast(GetEditorComponentData());
     }
 }