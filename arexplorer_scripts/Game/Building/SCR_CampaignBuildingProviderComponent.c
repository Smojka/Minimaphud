 [EntityEditorProps(category: "GameScripted/Building", description: "Component attached to a provider, responsible for basic provider behaviour.")]
 class SCR_CampaignBuildingProviderComponentClass : SCR_MilitaryBaseLogicComponentClass
 {
 }

 class SCR_CampaignBuildingProviderComponent : SCR_MilitaryBaseLogicComponent
 {
     [Attribute("", UIWidgets.EditBox, "Name of provider shown in provider interface", "")]
     protected string m_sProviderDisplayName;

     [Attribute("0", UIWidgets.CheckBox, "Can the building mode at this provider executed only via user action?")]
     protected bool m_bUserActionActivationOnly;

     [Attribute("0", UIWidgets.CheckBox, "Master provider is for example HQ with other service (providers) in vicinity.")]
     protected bool m_bIsMasterProvider;

     [Attribute("0", UIWidgets.CheckBox, "When opening a Free Roam Building mode, try to use master provider. Master provider is HQ where the service is registered in.")]
     protected bool m_bUseMasterProvider;

     [Attribute("0", UIWidgets.CheckBox, "Register at nearby base, if available.")]
     protected bool m_bRegisterAtBase;

     [Attribute("0", UIWidgets.CheckBox, "If set, player can command friendly AI in building radius.")]
     protected bool m_bCanCommandAI;

     [Attribute("1", UIWidgets.CheckBox, "Obstruct view when an enemy is present within the radius of the building.")]
     protected bool m_bObstructViewWhenEnemyInRange;

     [Attribute("50", UIWidgets.EditBox, "Building radius")]
     protected float m_fBuildingRadius;

     [Attribute(defvalue: "1", uiwidget: UIWidgets.ComboBox, desc: "Minimal rank that allows player to use the provider to build structures.", enums: ParamEnumArray.FromEnum(SCR_ECharacterRank))]
     protected SCR_ECharacterRank m_iRank;

     [Attribute(desc: "Fill in the budgets to be used with this provider")]
     protected ref array<ref SCR_CampaignBuildingBudgetToEvaluateData> m_aBudgetsToEvaluate;

     [Attribute(desc: "Traits this provider will provide. Each trait represents a tab in building interface. The tabs have to be defined in building mode's SCR_ContentBrowserEditorComponent.", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel))]
     protected ref array<EEditableEntityLabel> m_aAvailableTraits;

     [RplProp()]
     protected int m_iCurrentPropValue;

     [RplProp()]
     protected int m_iCurrentAIValue;

     protected Physics m_ProviderPhysics;

     protected SCR_ResourceComponent m_ResourceComponent;

     protected ref array<int> m_aActiveUsersIDs = {};
     protected ref array<int> m_aAvailableUsersIDs = {};
     protected ref array<SCR_CampaignBuildingBudgetToEvaluateData> m_aShownBudget = {};

     protected static ref ScriptInvokerVoid s_OnProviderCreated = new ScriptInvokerVoid();
     protected ref ScriptInvokerVoid m_OnCooldownLockUpdated;

     protected const int MOVING_CHECK_PERIOD = 1000;
     protected const int PROVIDER_SPEED_TO_REMOVE_BUILDING_SQ = 1;

     protected ref array<ref Tuple2<int, WorldTimestamp>> m_aPlacingCooldown = {};
     protected bool m_bCooldownClientLock;

     SCR_CampaignBuildingProviderComponent m_MasterProviderComponent;

     //------------------------------------------------------------------------------------------------
  string GetProviderDisplayName()
     {
         return m_sProviderDisplayName;
     }

     //------------------------------------------------------------------------------------------------
  bool UseMasterProvider()
     {
         return m_bUseMasterProvider;
     }

     //------------------------------------------------------------------------------------------------
  bool IsMasterProvider()
     {
         return m_bIsMasterProvider;
     }

     //------------------------------------------------------------------------------------------------
  bool CanCommandAI()
     {
         return m_bCanCommandAI;
     }

     //------------------------------------------------------------------------------------------------
  bool ObstrucViewWhenEnemyInRange()
     {
         return m_bObstructViewWhenEnemyInRange;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetMasterProviderEntity()
     {
         // Check bases this service is registered to. If non, it's standalone, return itself.
         array<SCR_MilitaryBaseComponent> bases = {};
         array<SCR_CampaignBuildingProviderComponent> providers = {};

         GetBases(bases);

         foreach (SCR_MilitaryBaseComponent base : bases)
         {
             base.GetBuildingProviders(providers);

             foreach (SCR_CampaignBuildingProviderComponent provider : providers)
             {
                 if (provider.IsMasterProvider())
                     return provider.GetOwner();
             }
         }

         return GetOwner();
     }

     //------------------------------------------------------------------------------------------------
  bool UseMasterProviderBudget(EEditableEntityBudget budget, out SCR_CampaignBuildingProviderComponent masterProviderComponent)
     {
         if (!m_aBudgetsToEvaluate || m_aBudgetsToEvaluate.IsEmpty())
             return false;

         IEntity masterProvider;

         foreach (SCR_CampaignBuildingBudgetToEvaluateData budgetData : m_aBudgetsToEvaluate)
         {
             if (budgetData.GetBudget() == budget && budgetData.UseMasterProviderBudget())
             {
                 masterProvider = GetMasterProviderEntity();
                 if (masterProvider == GetOwner())
                     return false;

                 masterProviderComponent = SCR_CampaignBuildingProviderComponent.Cast(masterProvider.FindComponent(SCR_CampaignBuildingProviderComponent));
                 return true;
             }
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
     void SetClientLock(bool lock, IEntity provider, int playerId)
     {
         PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
         if (!playerController)
             return;

         SCR_CampaignBuildingNetworkComponent buildingNetworkComponent = SCR_CampaignBuildingNetworkComponent.Cast(playerController.FindComponent(SCR_CampaignBuildingNetworkComponent));
         if (!buildingNetworkComponent)
             return;

         buildingNetworkComponent.SetClientLock(lock, provider);
     }

     //------------------------------------------------------------------------------------------------
     void SetCooldownClientLock(bool val)
     {
         m_bCooldownClientLock = val;

         if (m_OnCooldownLockUpdated)
             m_OnCooldownLockUpdated.Invoke();
     }

     //------------------------------------------------------------------------------------------------
  void SetPlayerCooldown(int playerId, int cooldownTime)
     {
         ChimeraWorld world = GetOwner().GetWorld();
         if (!world)
             return;

         m_aPlacingCooldown.Insert(new Tuple2<int, WorldTimestamp>(playerId,  world.GetServerTimestamp().PlusMilliseconds(CalculateCooldownTime(playerId, cooldownTime) * 1000)));

         SetClientLock(true, GetOwner(), playerId);
         GetGame().GetCallqueue().CallLater(UpdateCooldownTimer, 250, true, null);
     }

     //------------------------------------------------------------------------------------------------
  void UpdateCooldownTimer()
     {
         ChimeraWorld world = GetOwner().GetWorld();
         if (!world)
             return;

         bool cooldownChange;

         for (int i = m_aPlacingCooldown.Count() - 1; i >= 0; i--)
         {
             if (!m_aPlacingCooldown[i].param2.Greater(world.GetServerTimestamp()))
             {
                 SetClientLock(false, GetOwner(), m_aPlacingCooldown[i].param1);
                 m_aPlacingCooldown.Remove(i);
                 cooldownChange = true;
             }
         }

         if (m_OnCooldownLockUpdated && cooldownChange)
                 m_OnCooldownLockUpdated.Invoke();

         if (m_aPlacingCooldown.IsEmpty())
             GetGame().GetCallqueue().Remove(UpdateCooldownTimer);
     }

     //------------------------------------------------------------------------------------------------
  bool HasCooldownSet(int playerId)
     {
         return m_bCooldownClientLock || !m_aPlacingCooldown.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  float GetCooldownValue(int playerId)
     {
         float cooldown = 0.0;

         ChimeraWorld world = GetOwner().GetWorld();
         if (!world)
             return cooldown;

         foreach (Tuple2<int, WorldTimestamp> cooldownEntry : m_aPlacingCooldown)
         {
             if (cooldownEntry.param1 == playerId)
             {
                 cooldown = cooldownEntry.param2.DiffMilliseconds(world.GetServerTimestamp()) * 0.001;
                 return cooldown;
             }
         }

         return cooldown;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvokerVoid GetOnCooldownLockUpdated()
     {
         if (!m_OnCooldownLockUpdated)
             m_OnCooldownLockUpdated = new ScriptInvokerVoid();

         return m_OnCooldownLockUpdated;
     }

     //------------------------------------------------------------------------------------------------
     override void RegisterBase(notnull SCR_MilitaryBaseComponent base)
     {
         if (!m_bRegisterAtBase)
             return;

         super.RegisterBase(base);
     }

     //------------------------------------------------------------------------------------------------
  bool CanRegisterAtMilitaryBase()
     {
         return m_bRegisterAtBase;
     }

     //------------------------------------------------------------------------------------------------
     SCR_MilitaryBaseComponent GetMilitaryBaseComponent()
     {
         array<SCR_MilitaryBaseComponent> bases = {};
         GetBases(bases);

         if (bases.IsEmpty())
             return null;

         return bases[0];
     }

     //------------------------------------------------------------------------------------------------
  SCR_ECharacterRank GetAccessRank()
     {
         return m_iRank;
     }

     //------------------------------------------------------------------------------------------------
  float GetBuildingRadius()
     {
         return m_fBuildingRadius;
     }

     //------------------------------------------------------------------------------------------------
  int GetMaxBudgetValue(EEditableEntityBudget budget)
     {
         SCR_CampaignBuildingBudgetToEvaluateData budgetData = GetBudgetData(budget);
         if (!budgetData)
             return -1;

         SCR_CampaignBuildingMaxValueBudgetToEvaluateData maxValueBudgetData = SCR_CampaignBuildingMaxValueBudgetToEvaluateData.Cast(budgetData);
         if (maxValueBudgetData)
             return maxValueBudgetData.GetMaxValue();

         return -1;
     }

     //------------------------------------------------------------------------------------------------
  int GetCurrentPropValue()
     {
         return m_iCurrentPropValue;
     }

     //------------------------------------------------------------------------------------------------
  void SetPropValue(int value)
     {
         m_iCurrentPropValue = value;

         SCR_CampaignBuildingProviderComponent masterProviderComponent;

         if (UseMasterProviderBudget(EEditableEntityBudget.PROPS, masterProviderComponent))
             masterProviderComponent.SetPropValue(value);

         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
  void AddPropValue(int value)
     {
         m_iCurrentPropValue += value;

         SCR_CampaignBuildingProviderComponent masterProviderComponent;

         if (UseMasterProviderBudget(EEditableEntityBudget.PROPS, masterProviderComponent))
             masterProviderComponent.AddPropValue(value);

         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
     int GetCurrentAIValue()
     {
         SCR_CampaignBuildingProviderComponent masterProviderComponent;

         if (UseMasterProviderBudget(EEditableEntityBudget.AI, masterProviderComponent))
             return masterProviderComponent.GetCurrentAIValue();

         return m_iCurrentAIValue;
     }

     //------------------------------------------------------------------------------------------------
     void SetAIValue(int value)
     {
         m_iCurrentAIValue = value;

         SCR_CampaignBuildingProviderComponent masterProviderComponent;

         if (UseMasterProviderBudget(EEditableEntityBudget.AI, masterProviderComponent))
             masterProviderComponent.SetAIValue(value);

         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
     void AddAIValue(int value)
     {
         m_iCurrentAIValue += value;

         SCR_CampaignBuildingProviderComponent masterProviderComponent;

         if (UseMasterProviderBudget(EEditableEntityBudget.AI, masterProviderComponent))
             masterProviderComponent.AddAIValue(value);

         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
  protected void OnAIRemoved(IEntity ent)
     {
         RemoveEvents(ent);
         AddAIValue(-1);
     }

     //------------------------------------------------------------------------------------------------
  void SetOnEntityKilled(IEntity ent)
     {
         SCR_EditableCharacterComponent editableCharacter = SCR_EditableCharacterComponent.Cast(ent.FindComponent(SCR_EditableCharacterComponent));
         if (!editableCharacter)
             return;

         editableCharacter.GetOnDestroyed().Insert(OnAIRemoved);
         editableCharacter.GetOnDeleted().Insert(OnAIRemoved);
     }

     //------------------------------------------------------------------------------------------------
  protected void RemoveEvents(IEntity ent)
     {
         SCR_EditableCharacterComponent editableCharacter = SCR_EditableCharacterComponent.Cast(ent.FindComponent(SCR_EditableCharacterComponent));
         if (!editableCharacter)
             return;

         editableCharacter.GetOnDestroyed().Remove(OnAIRemoved);
         editableCharacter.GetOnDeleted().Remove(OnAIRemoved);
     }

     //------------------------------------------------------------------------------------------------
     array<EEditableEntityLabel> GetAvailableTraits()
     {
         array<EEditableEntityLabel> availableTraits = {};
         availableTraits.Copy(m_aAvailableTraits);
         return availableTraits;
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         SetEventMask(owner, EntityEvent.INIT);

         super.OnPostInit(owner);
     }

     //------------------------------------------------------------------------------------------------
     override void EOnInit(IEntity owner)
     {
         super.EOnInit(owner);

         if (System.IsConsoleApp() || !GetGame().GetPlayerController())
             SetOnProviderFactionChangedEvent();
     }

     //------------------------------------------------------------------------------------------------
  void AddNewActiveUser(int userID)
     {
         m_aActiveUsersIDs.Insert(userID);
         SetOnPlayerDeathActiveUserEvent(userID);
     }

     //------------------------------------------------------------------------------------------------
  void RemoveActiveUser(int userID)
     {
         m_aActiveUsersIDs.RemoveItem(userID);
     }

     //------------------------------------------------------------------------------------------------
  int GetActiveUsers(out notnull array<int> users)
     {
         return users.Copy(m_aActiveUsersIDs);
     }

     //------------------------------------------------------------------------------------------------
  bool ContainActiveUsers(int playerId)
     {
         return m_aActiveUsersIDs.Contains(playerId);
     }

     //------------------------------------------------------------------------------------------------
  void AddNewAvailableUser(int userID)
     {
         m_aAvailableUsersIDs.Insert(userID);
         SetOnPlayerDeathAvailableUserEvent(userID);
     }

     //------------------------------------------------------------------------------------------------
  void RemoveAvailableUser(int userID)
     {
         m_aAvailableUsersIDs.RemoveItem(userID);
     }

     //------------------------------------------------------------------------------------------------
  int GetAvailableUsers(out array<int> users)
     {
         return users.Copy(m_aAvailableUsersIDs);
     }

     //------------------------------------------------------------------------------------------------
  bool ContainAvailableUsers(int playerId)
     {
         return m_aAvailableUsersIDs.Contains(playerId);
     }

     //------------------------------------------------------------------------------------------------
  void RequestBuildingMode(int playerID, bool userActionUsed)
     {
         RequestEnterBuildingMode(playerID, userActionUsed);
     }

     //------------------------------------------------------------------------------------------------
  void RequestEnterBuildingMode(int playerID, bool userActionUsed)
     {
         SCR_CampaignBuildingNetworkComponent networkComponent = GetNetworkManager();
         if (!networkComponent)
             return;

         SCR_EditorManagerEntity editorManager = GetEditorManager();
         if (!editorManager)
             return;

         SetOnPlayerConsciousnessChanged();
         SetOnPlayerTeleported(playerID);
         editorManager.GetOnOpenedServer().Insert(BuildingModeCreated);
         editorManager.GetOnClosed().Insert(OnModeClosed);
         networkComponent.RequestEnterBuildingMode(GetOwner(), playerID, m_bUserActionActivationOnly, userActionUsed);
     }

     //------------------------------------------------------------------------------------------------
  void SetOnPlayerTeleported(int playerID)
     {
         PlayerController playerController = GetGame().GetPlayerController();
         if (!playerController)
             return;

         SCR_PlayerTeleportedFeedbackComponent playerTeleportComponent = SCR_PlayerTeleportedFeedbackComponent.Cast(playerController.FindComponent(SCR_PlayerTeleportedFeedbackComponent));
         if (!playerTeleportComponent)
             return;

         playerTeleportComponent.GetOnPlayerTeleported().Insert(PlayerTeleported);
     }

     //------------------------------------------------------------------------------------------------
  void SetOnPlayerConsciousnessChanged()
     {
         ChimeraCharacter player = ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
         if (!player)
             return;

         SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(player.GetCharacterController());
         if (controller)
             controller.m_OnLifeStateChanged.Insert(OnLifeStateChanged);
     }

     //------------------------------------------------------------------------------------------------
  void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState)
     {
         if (newLifeState == ECharacterLifeState.ALIVE)
             return;

         SCR_CampaignBuildingNetworkComponent networkComponent = GetNetworkManager();
         if (!networkComponent)
             return;

         networkComponent.RemoveEditorMode(SCR_PlayerController.GetLocalPlayerId(), GetOwner());
     }

     //------------------------------------------------------------------------------------------------
  void PlayerTeleported(SCR_EditableCharacterComponent character, bool isLongFade, SCR_EPlayerTeleportedReason teleportReason)
     {
         SCR_CampaignBuildingNetworkComponent networkComponent = GetNetworkManager();
         if (!networkComponent)
             return;

         networkComponent.RemoveEditorMode(SCR_PlayerController.GetLocalPlayerId(), GetOwner());

     }

     //------------------------------------------------------------------------------------------------
  void RemoveOnLifeStateChanged()
     {
         ChimeraCharacter player = ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
         if (!player)
             return;

         SCR_CharacterControllerComponent controller = SCR_CharacterControllerComponent.Cast(player.GetCharacterController());
         if (controller)
             controller.m_OnLifeStateChanged.Remove(OnLifeStateChanged);
     }

     //------------------------------------------------------------------------------------------------
  void SetOnProviderFactionChangedEvent()
     {
         SCR_FactionAffiliationComponent factionComponent = SCR_FactionAffiliationComponent.Cast(GetOwner().FindComponent(SCR_FactionAffiliationComponent));
         if (!factionComponent)
             return;

         factionComponent.GetOnFactionChanged().Insert(OnBaseOwnerChanged);
     }

     //------------------------------------------------------------------------------------------------
  void SetOnPlayerDeathActiveUserEvent(int userID)
     {
         SCR_CharacterControllerComponent comp = GetCharacterControllerComponent(userID);
         if (!comp)
             return;

         comp.GetOnPlayerDeathWithParam().Insert(OnActiveUserDeath);
     }

     //------------------------------------------------------------------------------------------------
  void SetOnPlayerDeathAvailableUserEvent(int userID)
     {
         SCR_CharacterControllerComponent comp = GetCharacterControllerComponent(userID);
         if (!comp)
             return;

         comp.GetOnPlayerDeathWithParam().Insert(OnAvailableUserDeath);
     }

     //------------------------------------------------------------------------------------------------
  void OnActiveUserDeath(SCR_CharacterControllerComponent characterControllerComponent, IEntity instigatorEntity, notnull Instigator instigator)
     {
         RemoveActiveUser(GetPlayerIdFromCharacterController(characterControllerComponent));
     }

     //------------------------------------------------------------------------------------------------
  void OnAvailableUserDeath(SCR_CharacterControllerComponent characterControllerComponent, IEntity instigatorEntity, notnull Instigator instigator)
     {
         RemoveAvailableUser(GetPlayerIdFromCharacterController(characterControllerComponent));
     }

     //------------------------------------------------------------------------------------------------
  void BuildingModeCreated()
     {
         SCR_EditorManagerEntity editorManager = GetEditorManager();
         if (!editorManager)
             return;

         editorManager.GetOnOpenedServer().Remove(BuildingModeCreated);

         SCR_EditorModeEntity editorMode = editorManager.GetCurrentModeEntity();
         if (!editorMode)
             return;

         SCR_PlacingEditorComponent placingComponent = SCR_PlacingEditorComponent.Cast(editorMode.FindComponent(SCR_PlacingEditorComponent));
         if (!placingComponent)
             return;

         placingComponent.GetOnPlaceEntityServer().Insert(EntitySpawnedByProvider);
     }

     //------------------------------------------------------------------------------------------------
  void EntitySpawnedByProvider(int prefabID, SCR_EditableEntityComponent editableEntity)
     {
         SCR_GameModeCampaign campaign = SCR_GameModeCampaign.GetInstance();
         if (!campaign)
             return;

         if (m_aActiveUsersIDs.IsEmpty())
             return;

         IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(m_aActiveUsersIDs[0]);
         campaign.OnEntityRequested(editableEntity.GetOwner(), player, SCR_Faction.Cast(GetEntityFaction(GetOwner())), this);
     }

     //------------------------------------------------------------------------------------------------
  void OnModeClosed()
     {
         RemoveOnModeClosed();
         RemoveOnLifeStateChanged();
         RemoveOnPlayerTeleported();

         m_aShownBudget.Clear();

         SCR_CampaignBuildingNetworkComponent networkComponent = GetNetworkManager();
         if (!networkComponent)
             return;

         networkComponent.RemoveEditorMode(SCR_PlayerController.GetLocalPlayerId(), GetOwner());
     }

     //------------------------------------------------------------------------------------------------
  static ScriptInvokerVoid GetOnProviderCreated()
     {
         return s_OnProviderCreated;
     }

     //------------------------------------------------------------------------------------------------
  void RemoveOnModeClosed()
     {
         SCR_EditorManagerEntity editorManager = GetEditorManager();
         if (!editorManager)
             return;

         editorManager.GetOnClosed().Remove(OnModeClosed);
     }

     //------------------------------------------------------------------------------------------------
  void RemoveOnPlayerTeleported()
     {
         PlayerController playerController = GetGame().GetPlayerController();
         if (!playerController)
             return;

         SCR_PlayerTeleportedFeedbackComponent playerTeleportComponent = SCR_PlayerTeleportedFeedbackComponent.Cast(playerController.FindComponent(SCR_PlayerTeleportedFeedbackComponent));
         if (!playerTeleportComponent)
             return;

         playerTeleportComponent.GetOnPlayerTeleported().Remove(PlayerTeleported);
     }

     //------------------------------------------------------------------------------------------------
  bool GetUserActionInitOnly()
     {
         return m_bUserActionActivationOnly;
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_CampaignBuildingNetworkComponent GetNetworkManager()
     {
         PlayerController playerController = GetGame().GetPlayerController();
         if (!playerController)
             return null;

         return SCR_CampaignBuildingNetworkComponent.Cast(playerController.FindComponent(SCR_CampaignBuildingNetworkComponent));
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_EditorManagerEntity GetEditorManager()
     {
         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (!core)
             return null;

         return core.GetEditorManager();
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_EditorManagerEntity GetEditorManagerByID(int playerId)
     {
         SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
         if (!core)
             return null;

         return core.GetEditorManager(playerId);
     }

     //------------------------------------------------------------------------------------------------
     protected SCR_CharacterControllerComponent GetCharacterControllerComponent(int playerID)
     {
         IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
         if (!player)
             return null;

         return SCR_CharacterControllerComponent.Cast(player.FindComponent(SCR_CharacterControllerComponent));
     }

     //------------------------------------------------------------------------------------------------
     protected int GetPlayerIdFromCharacterController(SCR_CharacterControllerComponent characterControllerComponent)
     {
         if (!characterControllerComponent)
             return -1;

         IEntity ent = characterControllerComponent.GetOwner();
         if (!ent)
             return -1;

         return GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(ent);
     }

     //------------------------------------------------------------------------------------------------
  // Method called by a user action to make sure it can only be shown to the players of the same faction the base belongs to.
  bool IsCharacterFactionSame(notnull IEntity character)
     {
         Faction playerFaction = GetEntityFaction(character);
         if (!playerFaction)
             return false;

         IEntity provider = GetOwner();
         if (!provider)
             return false;

         Faction owningFaction = GetEntityFaction(provider);
         if (!owningFaction)
             return false;

         return playerFaction == owningFaction;
     }

     //------------------------------------------------------------------------------------------------
     protected Faction GetEntityFaction(notnull IEntity ent)
     {
         FactionAffiliationComponent factionComp = FactionAffiliationComponent.Cast(ent.FindComponent(FactionAffiliationComponent));

         // Seacrch for the faction component on parent entities as not always is it on the same component as this one (vehicle for an example)
         while (!factionComp && ent)
         {
             ent = ent.GetParent();
             if (ent)
                 factionComp = FactionAffiliationComponent.Cast(ent.FindComponent(FactionAffiliationComponent));
         }

         if (!factionComp)
             return null;

         Faction faction = factionComp.GetAffiliatedFaction();
         if (!faction)
             faction = factionComp.GetDefaultAffiliatedFaction();

         return faction;
     }

     //------------------------------------------------------------------------------------------------
  protected void OnBaseOwnerChanged(FactionAffiliationComponent owner, Faction previousFaction, Faction newFaction)
     {
         // ToDo: In the future if the trigger activation will be enabled again, here I have to use a trigger range to find all entities which should be added and remove those on the list now.
         RemoveActiveUsers();
     }

     //------------------------------------------------------------------------------------------------
     SCR_ResourceComponent GetResourceComponent()
     {
         if (!m_ResourceComponent)
             m_ResourceComponent = SCR_ResourceComponent.FindResourceComponent(GetOwner());

         return m_ResourceComponent;
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("SCR_CampaignBuildingProviderComponent.GetResourceComponent() should be used instead.")]
     SCR_CampaignSuppliesComponent GetSuppliesComponent()
     {
         return SCR_CampaignSuppliesComponent.Cast(GetOwner().FindComponent(SCR_CampaignSuppliesComponent));
     }

     //------------------------------------------------------------------------------------------------
  SCR_CampaignBuildingBudgetToEvaluateData GetBudgetData(EEditableEntityBudget budget)
     {
         foreach (SCR_CampaignBuildingBudgetToEvaluateData budgetData : m_aBudgetsToEvaluate)
         {
             if (budgetData.GetBudget() == budget)
                 return budgetData;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  int GetBudgetTypesToEvaluate(notnull out array<ref EEditableEntityBudget> budgets)
     {
         foreach (SCR_CampaignBuildingBudgetToEvaluateData budgetData : m_aBudgetsToEvaluate)
         {
             budgets.Insert(budgetData.GetBudget());
         }

         return budgets.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool IsBudgetToEvaluate(EEditableEntityBudget blockingBudget)
     {
         if (!m_aBudgetsToEvaluate)
             return false;

         foreach (SCR_CampaignBuildingBudgetToEvaluateData budgetData : m_aBudgetsToEvaluate)
         {
             if (budgetData.GetBudget() == blockingBudget && budgetData.CanBeUsed())
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  EEditableEntityBudget GetShownBudget()
     {
         foreach (SCR_CampaignBuildingBudgetToEvaluateData budgetData : m_aBudgetsToEvaluate)
         {
             if (budgetData.CanShowBudgetInUI() && !m_aShownBudget.Contains(budgetData))
             {
                 m_aShownBudget.Insert(budgetData);
                 return budgetData.GetBudget();
             }
         }

         return -1;
     }

     //------------------------------------------------------------------------------------------------
  int CalculateCooldownTime(int playerId, int cooldownTime)
     {
         SCR_CampaignBuildingCooldownWithRankBudgetToEvaluateData cooldownBudgetData;

         foreach (SCR_CampaignBuildingBudgetToEvaluateData budgetData : m_aBudgetsToEvaluate)
         {
             if (budgetData.GetBudget() == EEditableEntityBudget.COOLDOWN)
             {
                 cooldownBudgetData = SCR_CampaignBuildingCooldownWithRankBudgetToEvaluateData.Cast(budgetData);
                 if (!cooldownBudgetData)
                     continue;

                 return cooldownBudgetData.CooldownTimeModifier(playerId, cooldownTime);
             }
         }

         return cooldownTime;
     }

     //------------------------------------------------------------------------------------------------
  void RemoveActiveUsers()
     {
         BaseGameMode gameMode = GetGame().GetGameMode();
         if (!gameMode)
             return;

         SCR_CampaignBuildingManagerComponent buildingManagerComponent = SCR_CampaignBuildingManagerComponent.Cast(gameMode.FindComponent(SCR_CampaignBuildingManagerComponent));
         if (!buildingManagerComponent)
             return;

         bool isActiveUser;
         foreach (int playerId : m_aAvailableUsersIDs)
         {
             isActiveUser = false;

             SCR_EditorManagerEntity editorManager = GetEditorManagerByID(playerId);
             if (!editorManager)
                 return;

             SCR_EditorModeEntity modeEntity = SCR_EditorModeEntity.Cast(editorManager.FindModeEntity(EEditorMode.BUILDING));
             if (!modeEntity)
                 return;

             if (m_aActiveUsersIDs.Contains(playerId))
             {
                 RemoveActiveUser(playerId);
                 isActiveUser = true;
             }

             buildingManagerComponent.RemoveProvider(playerId, this, isActiveUser);
             RemoveAvailableUser(playerId);
         }

         RemoveCheckProviderMove();
     }

     //------------------------------------------------------------------------------------------------
  void RemoveCheckProviderMove()
     {
         if (IsProviderDynamic())
             GetGame().GetCallqueue().Remove(CheckProviderMove);
     }

     //------------------------------------------------------------------------------------------------
  void SetCheckProviderMove()
     {
         if (IsProviderDynamic())
             GetGame().GetCallqueue().CallLater(CheckProviderMove, MOVING_CHECK_PERIOD, true);
     }

     //------------------------------------------------------------------------------------------------
  private void CheckProviderMove()
     {
         vector velocity = m_ProviderPhysics.GetVelocity();

         if ((velocity.LengthSq()) > PROVIDER_SPEED_TO_REMOVE_BUILDING_SQ)
             RemoveActiveUsers();
     }

     //------------------------------------------------------------------------------------------------
  private bool IsProviderDynamic()
     {
         IEntity mainParent = SCR_EntityHelper.GetMainParent(GetOwner(), true);
         if (!mainParent)
             return false;

         m_ProviderPhysics = mainParent.GetPhysics();

         return m_ProviderPhysics && m_ProviderPhysics.IsDynamic();
     }

     //------------------------------------------------------------------------------------------------
  bool IsEnemyFaction(notnull ChimeraCharacter char)
     {
         FactionAffiliationComponent factionComponent = FactionAffiliationComponent.Cast(char.FindComponent(FactionAffiliationComponent));
         if (!factionComponent)
             return false;

         Faction faction = factionComponent.GetAffiliatedFaction();
         if (!faction )
             return false;

         SCR_CampaignFactionManager factionManager = SCR_CampaignFactionManager.Cast(GetGame().GetFactionManager());
         if (!factionManager)
             return false;

         Faction playerFaction = factionManager.GetLocalPlayerFaction();
         if (!playerFaction)
             return false;

         return playerFaction.IsFactionEnemy(faction);
     }

     //------------------------------------------------------------------------------------------------
     override bool RplLoad(ScriptBitReader reader)
     {
         s_OnProviderCreated.Invoke();
         return true;
     }

     //------------------------------------------------------------------------------------------------
     // destructor
     void ~SCR_CampaignBuildingProviderComponent()
     {
         if (!m_aAvailableUsersIDs.IsEmpty())
             RemoveActiveUsers();
     }
 }