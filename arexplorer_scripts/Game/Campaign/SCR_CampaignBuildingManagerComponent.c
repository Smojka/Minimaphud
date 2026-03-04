 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Base for gamemode scripted component.")]
 class SCR_CampaignBuildingManagerComponentClass : SCR_BaseGameModeComponentClass
 {
 }

 class SCR_CampaignBuildingManagerComponent : SCR_BaseGameModeComponent
 {
     [Attribute("", UIWidgets.ResourcePickerThumbnail, "Prefab of trigger spawned on server to activate a building mode when player enters its range.", "et")]
     protected ResourceName m_sFreeRoamBuildingServerTrigger;

     [Attribute("", UIWidgets.ResourcePickerThumbnail, "Prefab of trigger spawned only on clients, to visualize the building area and allow player build only within its radius.", "et")]
     protected ResourceName m_sFreeRoamBuildingClientTrigger;

     [Attribute("{58F07022C12D0CF5}Assets/Editor/PlacingPreview/Preview.emat", UIWidgets.ResourcePickerThumbnail, "Material used when for the preview of the compositions outside of the building mode.", category: "Preview", params: "emat")]
     protected ResourceName m_sPreviewMaterial;

     [Attribute("0", UIWidgets.ComboBox, "", enums: ParamEnumArray.FromEnum(EEditableEntityBudget))]
     protected EEditableEntityBudget m_BudgetType;

     [Attribute("25", UIWidgets.EditBox, "Refund percentage", "")]
     protected int m_iCompositionRefundPercentage;

     [Attribute("10", UIWidgets.EditBox, "How many times player has to perform build step to gain a XP reward", "")]
     protected int m_iXpRewardTreshold;

     [Attribute("0", UIWidgets.CheckBox, "If checked, only players of faction that match the owning faction of provider can disassemble composition")]
     protected bool m_bSameFactionDisassembleOnly;

     [Attribute()]
     protected ref SCR_CampaignBuildingCompositionOutlineManager m_OutlineManager;

     [Attribute("", UIWidgets.ResourceNamePicker, desc: "Config with prefabs available to build. The config has to be the same as on Editor Mode - placing editor component..", params: "conf")]
     protected ResourceName m_sPrefabsToBuildResource;

     protected ref array<ResourceName> m_aPlaceablePrefabs = {};

     protected SCR_EditableEntityCore m_EntityCore;
     protected IEntity m_TemporaryProvider;
     protected RplComponent m_RplComponent;
     protected int m_iBuildingCycle

     //------------------------------------------------------------------------------------------------
  SCR_CampaignBuildingCompositionOutlineManager GetOutlineManager()
     {
         return m_OutlineManager;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetResourceComponent(IEntity owner, out SCR_ResourceComponent component)
     {
         if (!owner)
             return false;

         IEntity providerEntity;
         SCR_CampaignBuildingCompositionComponent campaignCompositionComponent = SCR_CampaignBuildingCompositionComponent.Cast(owner.FindComponent(SCR_CampaignBuildingCompositionComponent));
         if (campaignCompositionComponent)
         providerEntity = campaignCompositionComponent.GetProviderEntity();

         if (!providerEntity)
         {
             providerEntity = GetTemporaryProvider();
             SetTemporaryProvider(null);
         }

         if (!providerEntity)
             return false;

         SetTemporaryProvider(null);

         component = SCR_ResourceComponent.FindResourceComponent(providerEntity);

         return component;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetCompositionPreviewMaterial()
     {
         return m_sPreviewMaterial;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetServerTriggerResourceName()
     {
         return m_sFreeRoamBuildingServerTrigger;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetClientTriggerResourceName()
     {
         return m_sFreeRoamBuildingClientTrigger;
     }

     //------------------------------------------------------------------------------------------------
  void SetTemporaryProvider(IEntity ent)
     {
         m_TemporaryProvider = ent;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetTemporaryProvider()
     {
         return m_TemporaryProvider;
     }

     //------------------------------------------------------------------------------------------------
  bool CanDisassembleSameFactionOnly()
     {
         return m_bSameFactionDisassembleOnly;
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("SCR_CampaignBuildingManagerComponent.GetResourceComponent() should be used instead.")]
     protected bool GetSupplyComponent(IEntity ownerEntity, out SCR_CampaignSuppliesComponent suppliesComponent)
     {
         SCR_CampaignBuildingCompositionComponent campaignCompositionComponent = SCR_CampaignBuildingCompositionComponent.Cast(ownerEntity.FindComponent(SCR_CampaignBuildingCompositionComponent));
         if (!campaignCompositionComponent)
             return false;

         IEntity providerEntity;
         providerEntity = campaignCompositionComponent.GetProviderEntity();
         if (!providerEntity)
         {
             providerEntity = GetTemporaryProvider();
             SetTemporaryProvider(null);
         }
         else
             SetTemporaryProvider(null);

         if (!providerEntity)
             return false;

         suppliesComponent = SCR_CampaignSuppliesComponent.Cast(providerEntity.FindComponent(SCR_CampaignSuppliesComponent));
         return suppliesComponent != null;
     }

     //------------------------------------------------------------------------------------------------
  void ProcesXPreward()
     {
         m_iBuildingCycle++;
         if (m_iBuildingCycle < m_iXpRewardTreshold)
             return;

         m_iBuildingCycle = 0;

         PlayerController playerController = GetGame().GetPlayerController();
         if (!playerController)
             return;

         SCR_CampaignBuildingNetworkComponent networkComponent = SCR_CampaignBuildingNetworkComponent.Cast(playerController.FindComponent(SCR_CampaignBuildingNetworkComponent));
         if (!networkComponent)
             return;

         networkComponent.AddXPReward(playerController.GetPlayerId());
     }

     //------------------------------------------------------------------------------------------------
  int GetCompositionRefundPercentage()
     {
         return m_iCompositionRefundPercentage;
     }

     //------------------------------------------------------------------------------------------------
     protected void OnEntityCoreBudgetUpdated(EEditableEntityBudget entityBudget, int originalBudgetValue, int budgetChange, int updatedBudgetValue, SCR_EditableEntityComponent entity)
     {
         if (IsProxy())
             return;

         if (entityBudget != m_BudgetType)
             return;

         if (entity.GetOwner().IsLoaded())
             return;

         SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();

         if (campaign && campaign.IsSessionLoadInProgress())
             return;

         int propBudgetValue;
         array<ref SCR_EntityBudgetValue> budgets = {};
         entity.GetEntityAndChildrenBudgetCost(budgets);

         foreach (SCR_EntityBudgetValue budget : budgets)
         {
             if (budget.GetBudgetType() != EEditableEntityBudget.PROPS)
                 continue;

             propBudgetValue = budget.GetBudgetValue();
             break;
         }

         IEntity entityOwner = entity.GetOwnerScripted();
         SCR_ResourceComponent resourceComponent;

         if (!GetResourceComponent(entityOwner, resourceComponent))
             return;

         //~ Supplies not enabled so no need to remove any
         if (!resourceComponent.IsResourceTypeEnabled())
             return;

         IEntity providerEntity = resourceComponent.GetOwner();

         if (!providerEntity)
             return;

         SCR_CampaignBuildingProviderComponent providerComponent = SCR_CampaignBuildingProviderComponent.Cast(providerEntity.FindComponent(SCR_CampaignBuildingProviderComponent));

         if (!providerComponent)
             return;

         if (budgetChange < 0)
         {
             budgetChange = Math.Round(budgetChange * m_iCompositionRefundPercentage * 0.01);

             providerComponent.AddPropValue(-propBudgetValue);

             SCR_ResourceGenerator generator = resourceComponent.GetGenerator(EResourceGeneratorID.DEFAULT, EResourceType.SUPPLIES);

             if (generator)
                 generator.RequestGeneration(-budgetChange);
         }
         else
         {
             providerComponent.AddPropValue(propBudgetValue);

             SCR_ResourceConsumer consumer = resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT, EResourceType.SUPPLIES);

             if (consumer)
                 consumer.RequestConsumtion(budgetChange);
         }
     }

     //------------------------------------------------------------------------------------------------
     override void EOnInit(IEntity owner)
     {
         m_EntityCore = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
         m_RplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));

         GetPrefabListFromConfig();

         if (!GetGameMode().IsMaster())
             return;

         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (!core)
             return;

         m_EntityCore.Event_OnEntityBudgetUpdated.Insert(OnEntityCoreBudgetUpdated);
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         if (SCR_Global.IsEditMode())
             return;

         SetEventMask(owner, EntityEvent.INIT);
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsProxy()
     {
         return (m_RplComponent && m_RplComponent.IsProxy());
     }

     //------------------------------------------------------------------------------------------------
  void GetPrefabListFromConfig()
     {
         Resource configContainer = BaseContainerTools.LoadContainer(m_sPrefabsToBuildResource);
         if (!configContainer || !configContainer.IsValid())
             return;

         SCR_PlaceableEntitiesRegistry registry = SCR_PlaceableEntitiesRegistry.Cast(BaseContainerTools.CreateInstanceFromContainer(configContainer.GetResource().ToBaseContainer()));
         if (!registry)
             return;

         m_aPlaceablePrefabs = registry.GetPrefabs();
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetCompositionResourceName(int prefabID)
     {
         // the array doesn't exist or the index I'm searching for is out of the bounds, terminate.
         if (!m_aPlaceablePrefabs || !m_aPlaceablePrefabs.IsIndexValid(prefabID))
             return string.Empty;

         return m_aPlaceablePrefabs[prefabID];
     }

     //------------------------------------------------------------------------------------------------
  int GetCompositionId(ResourceName resName)
     {
         return m_aPlaceablePrefabs.Find(resName);
     }

     //------------------------------------------------------------------------------------------------
  void GetEditorMode(int playerID, notnull IEntity provider, bool userActionActivationOnly = false, bool userActionUsed = false)
     {
         SCR_EditorManagerEntity editorManager = GetEditorManagerEntity(playerID);
         if (!editorManager)
             return;

         SCR_EditorModeEntity modeEntity = SCR_EditorModeEntity.Cast(editorManager.FindModeEntity(EEditorMode.BUILDING));
         if (!modeEntity)
             modeEntity = editorManager.CreateEditorMode(EEditorMode.BUILDING, false);

         if (!modeEntity)
             return;

         SetEditorMode(editorManager, modeEntity, playerID, provider, userActionActivationOnly, userActionUsed);
     }

     //------------------------------------------------------------------------------------------------
     protected void SetEditorMode(notnull SCR_EditorManagerEntity editorManager, notnull SCR_EditorModeEntity modeEntity, int playerID, notnull IEntity provider, bool userActionActivationOnly = false, bool userActionUsed = false)
     {
         SCR_CampaignBuildingEditorComponent buildingComponent = SCR_CampaignBuildingEditorComponent.Cast(modeEntity.FindComponent(SCR_CampaignBuildingEditorComponent));
         if (!buildingComponent)
             return;

         SCR_CampaignBuildingProviderComponent providerComponent = SCR_CampaignBuildingProviderComponent.Cast(provider.FindComponent(SCR_CampaignBuildingProviderComponent));
         if (!providerComponent)
             return;

         providerComponent.AddNewAvailableUser(playerID);
         if (userActionUsed)
             providerComponent.AddNewActiveUser(playerID);

         buildingComponent.AddProviderEntityEditorComponent(providerComponent);

         if (!editorManager.IsOpened())
             editorManager.SetCurrentMode(EEditorMode.BUILDING);

         if (userActionActivationOnly || userActionUsed)
             ToggleEditorMode(editorManager);

         // events
         SCR_PlacingEditorComponent placingComponent = SCR_PlacingEditorComponent.Cast(modeEntity.FindComponent(SCR_PlacingEditorComponent));
         placingComponent.GetOnPlaceEntityServer().Insert(EntitySpawnedByProvider);

         SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if (gameMode)
             gameMode.GetOnPlayerDisconnected().Insert(PlayerDisconnected);

         SetOnPlayerDeathEvent(playerID);
         SetOnProviderDestroyedEvent(provider);
         providerComponent.SetCheckProviderMove();
     }

     //------------------------------------------------------------------------------------------------
  void PlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
     {
         SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if (!gameMode)
             return;

         gameMode.GetOnPlayerDisconnected().Remove(PlayerDisconnected);
     }

     //------------------------------------------------------------------------------------------------
  void EntitySpawnedByProvider(int prefabID, SCR_EditableEntityComponent editableEntity, int playerId)
     {
         SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
         if (!campaign)
             return;

         SCR_EditorManagerEntity editorManager = GetEditorManagerEntity(playerId);
         if (!editorManager)
             return;

         SCR_EditorModeEntity modeEntity = SCR_EditorModeEntity.Cast(editorManager.FindModeEntity(EEditorMode.BUILDING));
         if (!modeEntity)
             return;

         SCR_CampaignBuildingEditorComponent buildingComponent = SCR_CampaignBuildingEditorComponent.Cast(modeEntity.FindComponent(SCR_CampaignBuildingEditorComponent));
         if (!buildingComponent)
             return;

         SCR_CampaignBuildingProviderComponent provider = SCR_CampaignBuildingProviderComponent.Cast(buildingComponent.GetProviderComponent());
         if (!provider)
             return;

         IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
         if (!player)
             return;

         campaign.OnEntityRequested(editableEntity.GetOwner(), player, SCR_Faction.Cast(SCR_Faction.GetEntityFaction(editableEntity.GetOwner())), provider);
     }

     //------------------------------------------------------------------------------------------------
     protected void SetOnPlayerDeathEvent(int playerID)
     {
         IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
         if (!player)
             return;

         SCR_CharacterControllerComponent comp = SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
         if (!comp)
             return;

         comp.GetOnPlayerDeathWithParam().Insert(OnPlayerDeath);
     }

     //------------------------------------------------------------------------------------------------
     protected void RemoveOnPlayerDeathEvent(int playerID)
     {
         IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
         if (!player)
             return;

         SCR_CharacterControllerComponent comp = SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
         if (!comp)
             return;

         comp.GetOnPlayerDeathWithParam().Remove(OnPlayerDeath);
     }

     //------------------------------------------------------------------------------------------------
     protected void SetOnProviderDestroyedEvent(IEntity provider)
     {
         SCR_DamageManagerComponent hitZoneContainerComponent = SCR_DamageManagerComponent.Cast(provider.FindComponent(SCR_DamageManagerComponent));
         if (!hitZoneContainerComponent)
             return;

         SCR_HitZone zone = SCR_HitZone.Cast(hitZoneContainerComponent.GetDefaultHitZone());
         if (zone)
             zone.GetOnDamageStateChanged().Insert(OnProviderDestroyed);
     }

     //------------------------------------------------------------------------------------------------
     protected void RemoveOnProviderDestroyedEvent(IEntity provider)
     {
         SCR_DamageManagerComponent hitZoneContainerComponent = SCR_DamageManagerComponent.Cast(provider.FindComponent(SCR_DamageManagerComponent));
         if (!hitZoneContainerComponent)
             return;

         SCR_HitZone zone = SCR_HitZone.Cast(hitZoneContainerComponent.GetDefaultHitZone());
         if (zone)
             zone.GetOnDamageStateChanged().Remove(OnProviderDestroyed);
     }

     //------------------------------------------------------------------------------------------------
     protected void OnPlayerDeath(SCR_CharacterControllerComponent characterController, IEntity instigatorEntity, notnull Instigator killer)
     {
         if (!characterController)
             return;

         int playerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(characterController.GetOwner());

         SCR_EditorManagerEntity editorManager = GetEditorManagerEntity(playerID);
         if (!editorManager)
             return;

         if (editorManager.IsOpened())
             ToggleEditorMode(editorManager);

         RemoveEditorMode(playerID);
     }

     //------------------------------------------------------------------------------------------------
  protected void OnProviderDestroyed(SCR_HitZone hitZone)
     {
         if (!hitZone)
             return;

         if (hitZone.GetDamageState() != EDamageState.DESTROYED)
             return;

         IEntity provider = hitZone.GetOwner();
         if (!provider)
             return;

         SCR_CampaignBuildingProviderComponent providerComponent = SCR_CampaignBuildingProviderComponent.Cast(provider.FindComponent(SCR_CampaignBuildingProviderComponent));
         if (!providerComponent)
             return;

         array<int> playersIDs = {};
         providerComponent.GetAvailableUsers(playersIDs);
         foreach (int playerId : playersIDs)
         {
             if (providerComponent.ContainActiveUsers(playerId))
             {
                 RemoveProvider(playerId, providerComponent, true);
                 providerComponent.RemoveActiveUser(playerId);
             }

             providerComponent.RemoveAvailableUser(playerId);
         }
     }

     //------------------------------------------------------------------------------------------------
  bool RemovePlayerIdFromProvider(int playerID, SCR_CampaignBuildingProviderComponent providerComponent)
     {
         bool isActiveUser = providerComponent.ContainActiveUsers(playerID);

         providerComponent.RemoveActiveUser(playerID);
         providerComponent.RemoveAvailableUser(playerID);

         return isActiveUser;
     }

     //------------------------------------------------------------------------------------------------
  void RemoveProvider(int playerID, SCR_CampaignBuildingProviderComponent providerComponent, bool isActiveUser)
     {
         SCR_EditorManagerEntity editorManager = GetEditorManagerEntity(playerID);
         if (!editorManager)
             return;

         SCR_EditorModeEntity modeEntity = SCR_EditorModeEntity.Cast(editorManager.FindModeEntity(EEditorMode.BUILDING));
         if (!modeEntity)
             return;

         SCR_CampaignBuildingEditorComponent editorComponent = SCR_CampaignBuildingEditorComponent.Cast(modeEntity.FindComponent(SCR_CampaignBuildingEditorComponent));
         if (!editorComponent)
             return;

         if (isActiveUser && editorManager.IsOpened())
             editorManager.Close();

         editorComponent.RemoveProviderEntityEditorComponent(providerComponent);

         IEntity provider = providerComponent.GetOwner();
         if (provider)
             RemoveOnProviderDestroyedEvent(providerComponent.GetOwner());

         providerComponent.RemoveCheckProviderMove();
         RemoveOnPlayerDeathEvent(playerID);

         SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if (gameMode)
             gameMode.GetOnPlayerDisconnected().Remove(PlayerDisconnected);

         // if it was a last provider and forced provider doesn't exist, remove the mode completely.
         if (!editorComponent.GetProviderComponent())
             RemoveEditorMode(playerID);
     }

     //------------------------------------------------------------------------------------------------
     protected void ToggleEditorMode(notnull SCR_EditorManagerEntity editorManager)
     {
         EEditorMode mode = editorManager.GetCurrentMode();

         editorManager.Toggle();
     }

     //------------------------------------------------------------------------------------------------
     protected void RemoveEditorMode(int playerID)
     {
         SCR_EditorManagerEntity editorManager = GetEditorManagerEntity(playerID);
         if (!editorManager)
             return;

         SCR_EditorModeEntity editorModeEntity = editorManager.FindModeEntity(EEditorMode.BUILDING);
         if (!editorModeEntity)
             return;

         SCR_EntityHelper.DeleteEntityAndChildren(editorModeEntity);
     }

     //------------------------------------------------------------------------------------------------
     protected SCR_EditorManagerEntity GetEditorManagerEntity(int playerID)
     {
         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (!core)
             return null;

         return core.GetEditorManager(playerID);
     }
 }