 enum EResourceDebugVisualizationFlags
 {
     CONSUMER        = 1 << 0,
     CONTAINER       = 1 << 1,
     GENERATOR       = 1 << 2,
     ENCAPSULATOR    = 1 << 3,
 }

 //~ ScriptInvokers
 void SCR_Resources_OnResourceEnabledChanged(SCR_ResourceComponent resourceComponent, array<EResourceType> disabledResourceTypes);
 typedef func SCR_Resources_OnResourceEnabledChanged;

 [ComponentEditorProps(category: "GameScripted/Resources", description: "")]
 class SCR_ResourceComponentClass : ScriptComponentClass
 {
     [Attribute(uiwidget: UIWidgets.Object, category: "Containers")]
     protected ref array<ref SCR_ResourceContainer> m_aContainers;

     //------------------------------------------------------------------------------------------------
     array<ref SCR_ResourceContainer> GetContainers()
     {
         return m_aContainers;
     }

     //------------------------------------------------------------------------------------------------
     override static array<typename> Requires(IEntityComponentSource src)
     {
         return { RplComponent };
     }
 }

 class SCR_ResourceComponent : ScriptComponent
 {
     const float UPDATE_DISTANCE_TRESHOLD = 2.5;
     const float UPDATE_DISTANCE_TRESHOLD_SQUARE = UPDATE_DISTANCE_TRESHOLD * UPDATE_DISTANCE_TRESHOLD;
     protected const float UPDATE_PERIOD = 10.0 / 60.0;
     protected bool m_bIsDynamic;
     protected bool m_bIsNetDirty;
     protected int m_iGridUpdateId = int.MIN;

     protected int m_iGridContainersBoundsMins = 0xFFFFFFFF;
     protected int m_iGridContainersBoundsMaxs = 0xFFFFFFFF;
     protected vector m_vGridContainersBoundingVolumeMins;
     protected vector m_vGridContainersBoundingVolumeMaxs;

     protected bool m_bIsFlaggedForProcessing;

     [Attribute(uiwidget: UIWidgets.Object, category: "Encapsulators")]
     protected ref array<ref SCR_ResourceEncapsulator> m_aEncapsulators;

     [Attribute(uiwidget: UIWidgets.Object, category: "Consumers"), RplProp(onRplName: "TEMP_OnInteractorReplicated")]
     protected ref array<ref SCR_ResourceConsumer> m_aConsumers;

     [Attribute(uiwidget: UIWidgets.Object, category: "Generators"), RplProp(onRplName: "TEMP_OnInteractorReplicated")]
     protected ref array<ref SCR_ResourceGenerator> m_aGenerators;

     //~ Any Resource Types that is set here is a disabled resource type
     [Attribute(desc: "List of all disabled resource types", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EResourceType)), RplProp(onRplName: "OnResourceTypeEnabledChanged")]
     protected ref array<EResourceType> m_aDisabledResourceTypes;

     //~ Any ResourceType that is set here cannot be enabled or disabled in runtime for this entity by editor
     [Attribute(desc: "Any ResourceType that is set here cannot be enabled or disabled in runtime for this entity by editor", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EResourceType))]
     protected ref array<EResourceType> m_aDisallowChangingEnableResource;

     protected ref ScriptInvoker m_TEMP_OnInteractorReplicated;

     protected ref ScriptInvokerBase<SCR_Resources_OnResourceEnabledChanged> m_OnResourceTypeEnabledChanged;

     //------------------------------------------------------------------------------------------------
     ScriptInvoker TEMP_GetOnInteractorReplicated()
     {
         if (!m_TEMP_OnInteractorReplicated)
             m_TEMP_OnInteractorReplicated = new ScriptInvoker();

         return m_TEMP_OnInteractorReplicated;
     }

     //------------------------------------------------------------------------------------------------
  void TEMP_OnInteractorReplicated()
     {
         if (m_TEMP_OnInteractorReplicated)
             m_TEMP_OnInteractorReplicated.Invoke();
     }

     [Attribute(uiwidget: UIWidgets.CheckBox, category: "Debugging")]
     protected bool m_bEnableDebugVisualization;

     [Attribute(defvalue: "-1", uiwidget: UIWidgets.Flags, enums: ParamEnumArray.FromEnum(EResourceDebugVisualizationFlags), category: "Debugging")]
     protected EResourceDebugVisualizationFlags m_eDebugVisualizationFlags;

     [Attribute(defvalue: "0.4 0.0 0.467 0.267 ", uiwidget: UIWidgets.ColorPicker, category: "Debugging")]
     protected ref Color m_DebugColor;

     [RplProp(onRplName: "OnVisibilityChanged")]
     protected bool m_bIsVisible = true;

     protected ref array<ref SCR_ResourceContainer> m_aContainerInstances = {};

     protected RplComponent m_ReplicationComponent;
     protected FactionAffiliationComponent m_FactionAffiliationComponent;

     protected vector m_LastPosition = vector.Zero;
     protected bool m_bHasParent;
     protected bool m_bIsOwnerActive;

     // TODO: Remove after fixing initialisation and hierarchy issues.
     protected bool m_bIsInitialized;

     // TODO: Remove after fixing initialisation and hierarchy issues.
     protected bool m_bIsAddedToParentBuffered;

     //------------------------------------------------------------------------------------------------
  bool IsResourceTypeEnabled(EResourceType resourceType = EResourceType.SUPPLIES)
     {
         if (!SCR_ResourceSystemHelper.IsGlobalResourceTypeEnabled(resourceType))
             return false;

         return !m_aDisabledResourceTypes.Contains(resourceType);
     }

     //------------------------------------------------------------------------------------------------
  int GetDisabledResourceTypes(inout notnull array<EResourceType> disabledResourceTypes)
     {
         disabledResourceTypes.Copy(m_aDisabledResourceTypes);
         return disabledResourceTypes.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool CanResourceTypeEnabledBeChanged(EResourceType resourceType = EResourceType.SUPPLIES)
     {
         return !m_aDisallowChangingEnableResource.Contains(resourceType);
     }

     //------------------------------------------------------------------------------------------------
  void SetResourceTypeEnabled(bool enable, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         if (!CanResourceTypeEnabledBeChanged(resourceType))
             return;

         int index = m_aDisabledResourceTypes.Find(resourceType);

         //~ Already Enabled/Disabled
         if ((index < 0) == enable)
             return;

         if (!enable)
             m_aDisabledResourceTypes.Insert(resourceType);
         else
             m_aDisabledResourceTypes.Remove(index);

         Replication.BumpMe();

         //~ Enable/Disable resource type for all encapsulated resourceComponents
         /*array<SCR_ResourceEncapsulator> encapsulators = GetEncapsulators();
         SCR_ResourceComponent resourceComponent;
         foreach (SCR_ResourceEncapsulator encapsulator : encapsulators)
         {
             resourceComponent = encapsulator.GetComponent();
             if (!resourceComponent || !resourceComponent.CanResourceTypeEnabledBeChanged(resourceType))
                 continue;

             resourceComponent.SetResourceTypeEnabled(enable, resourceType);
         }*/

         SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if ((gameMode && gameMode.IsMaster()) ||  (!gameMode && Replication.IsServer()))
             OnResourceTypeEnabledChanged();
     }

     //------------------------------------------------------------------------------------------------
     protected void OnResourceTypeEnabledChanged()
     {
         if (m_OnResourceTypeEnabledChanged)
             m_OnResourceTypeEnabledChanged.Invoke(this, m_aDisabledResourceTypes);
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvokerBase<SCR_Resources_OnResourceEnabledChanged> GetOnResourceTypeEnabledChanged()
     {
         if (!m_OnResourceTypeEnabledChanged)
             m_OnResourceTypeEnabledChanged = new ScriptInvokerBase<SCR_Resources_OnResourceEnabledChanged>();

         return m_OnResourceTypeEnabledChanged;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ResourceComponent FindResourceComponent(IEntity entity, bool ignoreChildIfHasStorage = false)
     {
         //~ Function is used in many places. Not all can guarantee that entity is not null
         if (!entity)
             return null;

         //~ Get resource component
         SCR_ResourceComponent resourceComponent = SCR_ResourceComponent.Cast(entity.FindComponent(SCR_ResourceComponent));
         if (resourceComponent)
             return resourceComponent;

         //~ Loop over slotted entities if entity is a vehicle. Note that slotted entities are not yet in hierarchy the moment they are created for clients
         if (Vehicle.Cast(entity))
         {
             SlotManagerComponent slotManager = SlotManagerComponent.Cast(entity.FindComponent(SlotManagerComponent));
             if (!slotManager)
                 return null;

             array<EntitySlotInfo> outSlotInfos = {};
             slotManager.GetSlotInfos(outSlotInfos);
             IEntity slot;

             foreach (EntitySlotInfo slotInfo : outSlotInfos)
             {
                 slot = slotInfo.GetAttachedEntity();

                 if (!slot)
                     continue;

                 resourceComponent = SCR_ResourceComponent.Cast(slot.FindComponent(SCR_ResourceComponent));
                 if (resourceComponent)
                 {
                     //~ It does not care if the slotted entity has a storage or not
                     if (!ignoreChildIfHasStorage)
                         return resourceComponent;

                     //~ Has no storage so this is a valid resource component
                     if (!slot.FindComponent(ScriptedBaseInventoryStorageComponent))
                         return resourceComponent;
                 }
             }
         }
         //~ Check if parent is vehicle than get resource component from parent or any other slot
         else if (Vehicle.Cast(entity.GetParent()))
         {
             InventoryItemComponent inventoryItem = InventoryItemComponent.Cast(entity.FindComponent(InventoryItemComponent));
             if (inventoryItem)
             {
                 //~ Is item in inventory thus ignore getting vehicle parent
                 if (inventoryItem.GetParentSlot())
                     return null;
             }

             return SCR_ResourceComponent.FindResourceComponent(entity.GetParent(), ignoreChildIfHasStorage);
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  int GetGridContainersBoundsMins()
     {
         return m_iGridContainersBoundsMins;
     }

     //------------------------------------------------------------------------------------------------
  int GetGridContainersBoundsMaxs()
     {
         return m_iGridContainersBoundsMaxs;
     }

     //------------------------------------------------------------------------------------------------
  void GetGridContainersBounds(out int mins, out int maxs)
     {
         mins = m_iGridContainersBoundsMins;
         maxs = m_iGridContainersBoundsMaxs;
     }

     //------------------------------------------------------------------------------------------------
  void GetGridContainersBoundingBox(out vector mins, out vector maxs)
     {
         mins = m_vGridContainersBoundingVolumeMins;
         maxs = m_vGridContainersBoundingVolumeMaxs;
     }

     //------------------------------------------------------------------------------------------------
  void GetGridContainersWorldBoundingBox(out vector mins, out vector maxs)
     {
         vector ownerOrigin = GetOwner().GetOrigin();
         mins = ownerOrigin + m_vGridContainersBoundingVolumeMins;
         maxs = ownerOrigin + m_vGridContainersBoundingVolumeMaxs;
     }

     //------------------------------------------------------------------------------------------------
  bool IsDynamic()
     {
         return m_bIsDynamic;
     }

     //------------------------------------------------------------------------------------------------
     RplComponent GetReplicationComponent()
     {
         return m_ReplicationComponent;
     }

     //------------------------------------------------------------------------------------------------
     FactionAffiliationComponent GetFactionAffiliationComponent()
     {
         return m_FactionAffiliationComponent;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceEncapsulator GetEncapsulator(EResourceType resourceType)
     {
         if (resourceType == EResourceType.INVALID || !m_aEncapsulators)
             return null;

         int higherLimitPosition = m_aEncapsulators.Count();

         if (higherLimitPosition == 0)
             return null;

         int position;
         SCR_ResourceEncapsulator encapsulator;

         while (position < higherLimitPosition)
         {
             if (GetNextEncapsulatorCandidate(position, higherLimitPosition, encapsulator, resourceType))
                 break;
         }

         if (!encapsulator
         ||  position == m_aEncapsulators.Count()
         ||  resourceType != encapsulator.GetResourceType())
             return null;

         return encapsulator;
     }

     //------------------------------------------------------------------------------------------------
  bool GetEncapsulator(EResourceType resourceType, out SCR_ResourceEncapsulator encapsulator)
     {
         encapsulator = null;

         if (resourceType == EResourceType.INVALID || !m_aEncapsulators)
             return false;

         int higherLimitPosition = m_aEncapsulators.Count();

         if (higherLimitPosition == 0)
             return false;

         int position;

         while (position < higherLimitPosition)
         {
             if (GetNextEncapsulatorCandidate(position, higherLimitPosition, encapsulator, resourceType))
                 break;
         }

         if (!encapsulator
         ||  position == m_aEncapsulators.Count()
         ||  resourceType != encapsulator.GetResourceType())
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetNextEncapsulatorCandidate(inout int position, inout int higherLimitPosition, inout SCR_ResourceEncapsulator encapsulator, EResourceType resourceType)
     {
         int comparePosition = position + ((higherLimitPosition - position) >> 1);
         encapsulator        = m_aEncapsulators[comparePosition];

         if (!encapsulator)
             return null;

         EResourceType compareResourceType = encapsulator.GetResourceType();

         if (resourceType > compareResourceType)
             position = comparePosition + 1;
         else if (resourceType < compareResourceType)
             higherLimitPosition = comparePosition;
         else
         {
             encapsulator = m_aEncapsulators[comparePosition];

             return true;
         }

         encapsulator = null;

         return false;
     }

     //------------------------------------------------------------------------------------------------
     array<SCR_ResourceEncapsulator> GetEncapsulators()
     {
         if (!m_aEncapsulators)
             return null;

         int encapsulatorCount = m_aEncapsulators.Count();

         if (encapsulatorCount == 0)
             return null;

         array<SCR_ResourceEncapsulator> encapsulators = {};

         encapsulators.Reserve(encapsulatorCount);

         foreach (SCR_ResourceEncapsulator encapsulator: m_aEncapsulators)
         {
             encapsulators.Insert(encapsulator);
         }

         return encapsulators;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceContainer GetContainer(EResourceType resourceType)
     {
         if (resourceType == EResourceType.INVALID || !m_aContainerInstances)
             return null;

         int higherLimitPosition = m_aContainerInstances.Count();

         if (higherLimitPosition == 0)
             return null;

         int position;
         SCR_ResourceContainer container;

         while (position < higherLimitPosition)
         {
             if (GetNextContainerCandidate(position, higherLimitPosition, container, resourceType))
                 break;
         }

         if (!container
         ||  position == m_aContainerInstances.Count()
         ||  resourceType != container.GetResourceType())
             return null;

         return container;
     }

     //------------------------------------------------------------------------------------------------
  bool GetContainer(EResourceType resourceType, out SCR_ResourceContainer container)
     {
         container = null;

         if (resourceType == EResourceType.INVALID || !m_aContainerInstances)
             return false;

         int higherLimitPosition = m_aContainerInstances.Count();
         if (higherLimitPosition == 0)
             return false;

         int position;

         while (position < higherLimitPosition)
         {
             if (GetNextContainerCandidate(position, higherLimitPosition, container, resourceType))
                 break;
         }

         if (!container
         ||  position == m_aContainerInstances.Count()
         ||  resourceType != container.GetResourceType())
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetNextContainerCandidate(inout int position, inout int higherLimitPosition, inout SCR_ResourceContainer container, EResourceType resourceType)
     {
         int comparePosition = position + ((higherLimitPosition - position) >> 1);
         container           = m_aContainerInstances[comparePosition];

         if (!container)
             return null;

         EResourceType compareResourceType = container.GetResourceType();

         if (resourceType > compareResourceType)
         {
             position = comparePosition + 1;
         }
         else if (resourceType < compareResourceType)
         {
             higherLimitPosition = comparePosition;
         }
         else
         {
             container = m_aContainerInstances[comparePosition];

             return true;
         }

         container = null;

         return false;
     }

     //------------------------------------------------------------------------------------------------
     array<SCR_ResourceContainer> GetContainers()
     {
         int containerCount = m_aContainerInstances.Count();

         if (containerCount == 0)
             return null;

         array<SCR_ResourceContainer> containers = {};

         containers.Reserve(containerCount);

         foreach (SCR_ResourceContainer container: m_aContainerInstances)
         {
             containers.Insert(container);
         }

         return containers;
     }

     //------------------------------------------------------------------------------------------------
     SCR_ResourceConsumer GetConsumer(EResourceGeneratorID identifier, EResourceType resourceType)
     {
         if (identifier == EResourceGeneratorID.INVALID || resourceType == EResourceType.INVALID || !m_aConsumers)
             return null;

         int higherLimitPosition = m_aConsumers.Count();
         if (higherLimitPosition == 0)
             return null;

         int position;
         SCR_ResourceConsumer consumer;

         while (position < higherLimitPosition)
         {
             if (GetNextConsumerCandidate(position, higherLimitPosition, consumer, identifier, resourceType))
                 break;
         }

         if (!consumer
         ||  position == m_aConsumers.Count()
         ||  identifier != consumer.GetGeneratorIdentifier()
         ||  resourceType != consumer.GetResourceType())
             return null;

         return consumer;
     }

     //------------------------------------------------------------------------------------------------
  bool GetConsumer(EResourceGeneratorID identifier, EResourceType resourceType, inout SCR_ResourceConsumer consumer)
     {
         consumer = null;

         if (identifier == EResourceGeneratorID.INVALID || resourceType == EResourceType.INVALID || !m_aConsumers)
             return false;

         int higherLimitPosition = m_aConsumers.Count();
         if (higherLimitPosition == 0)
             return false;

         int position;

         while (position < higherLimitPosition)
         {
             if (GetNextConsumerCandidate(position, higherLimitPosition, consumer, identifier, resourceType))
                 break;
         }

         if (!consumer
         ||  position == m_aConsumers.Count()
         ||  identifier != consumer.GetGeneratorIdentifier()
         ||  resourceType != consumer.GetResourceType())
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetNextConsumerCandidate(inout int position, inout int higherLimitPosition, inout SCR_ResourceConsumer consumer, EResourceGeneratorID identifier, EResourceType resourceType)
     {
         int comparePosition = position + ((higherLimitPosition - position) >> 1);
         consumer            = m_aConsumers[comparePosition];

         if (!consumer)
             return false;

         EResourceType compareResourceType       = consumer.GetResourceType();
         EResourceGeneratorID comapareIdentifier = consumer.GetGeneratorIdentifier();

         if (identifier > comapareIdentifier)
         {
             position = comparePosition + 1;
         }
         else if (identifier < comapareIdentifier)
         {
             higherLimitPosition = comparePosition;
         }
         else if (resourceType > compareResourceType)
         {
             position = comparePosition + 1;
         }
         else if (resourceType < compareResourceType)
         {
             higherLimitPosition = comparePosition;
         }
         else
         {
             consumer = m_aConsumers[comparePosition];

             return true;
         }

         consumer = null;

         return false;
     }

     //------------------------------------------------------------------------------------------------
     array<SCR_ResourceConsumer> GetConsumers()
     {
         int consumerCount = m_aConsumers.Count();
         if (consumerCount == 0)
             return null;

         array<SCR_ResourceConsumer> consumers = {};
         consumers.Reserve(consumerCount);

         foreach (SCR_ResourceConsumer container: m_aConsumers)
         {
             consumers.Insert(container);
         }

         return consumers;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceGenerator GetGenerator(EResourceGeneratorID identifier, EResourceType resourceType)
     {
         if (identifier == EResourceGeneratorID.INVALID || resourceType == EResourceType.INVALID || !m_aGenerators)
             return null;

         int higherLimitPosition = m_aGenerators.Count();
         if (higherLimitPosition == 0)
             return null;

         int position;
         SCR_ResourceGenerator generator;

         while (position < higherLimitPosition)
         {
             if (GetNextGeneratorCandidate(position, higherLimitPosition, generator, identifier, resourceType))
                 break;
         }

         if (!generator
         ||  position == m_aGenerators.Count()
         ||  identifier != generator.GetIdentifier()
         ||  resourceType != generator.GetResourceType())
             return null;

         return generator;
     }

     //------------------------------------------------------------------------------------------------
  bool GetGenerator(EResourceGeneratorID identifier, EResourceType resourceType, out SCR_ResourceGenerator generator)
     {
         generator = null;

         if (identifier == EResourceGeneratorID.INVALID || resourceType == EResourceType.INVALID || !m_aGenerators)
             return false;

         int higherLimitPosition = m_aGenerators.Count();
         if (higherLimitPosition == 0)
             return false;

         int position;

         while (position < higherLimitPosition)
         {
             if (GetNextGeneratorCandidate(position, higherLimitPosition, generator, identifier, resourceType))
                 break;
         }

         if (!generator
         ||  position == m_aGenerators.Count()
         ||  identifier != generator.GetIdentifier()
         ||  resourceType != generator.GetResourceType())
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetNextGeneratorCandidate(inout int position, inout int higherLimitPosition, inout SCR_ResourceGenerator generator, EResourceGeneratorID identifier, EResourceType resourceType)
     {
         int comparePosition = position + ((higherLimitPosition - position) >> 1);
         generator           = m_aGenerators[comparePosition];

         if (!generator)
             return false;

         EResourceType compareResourceType       = generator.GetResourceType();
         EResourceGeneratorID comapareIdentifier = generator.GetIdentifier();

         if (identifier > comapareIdentifier)
         {
             position = comparePosition + 1;
         }
         else if (identifier < comapareIdentifier)
         {
             higherLimitPosition = comparePosition;
         }
         else if (resourceType > compareResourceType)
         {
             position = comparePosition + 1;
         }
         else if (resourceType < compareResourceType)
         {
             higherLimitPosition = comparePosition;
         }
         else
         {
             generator = m_aGenerators[comparePosition];

             return true;
         }

         generator = null;

         return false;
     }

     //------------------------------------------------------------------------------------------------
     array<SCR_ResourceGenerator> GetGenerators()
     {
         int generatorCount = m_aGenerators.Count();

         if (generatorCount == 0)
             return null;

         array<SCR_ResourceGenerator> generators = {};

         generators.Reserve(generatorCount);

         foreach (SCR_ResourceGenerator generator: m_aGenerators)
         {
             generators.Insert(generator);
         }

         return generators;
     }

     //------------------------------------------------------------------------------------------------
  int GetGridUpdateId()
     {
         return m_iGridUpdateId;
     }

     //------------------------------------------------------------------------------------------------
     vector GetLastPosition()
     {
         return m_LastPosition;
     }

     //------------------------------------------------------------------------------------------------
  bool HasParent()
     {
         return m_bHasParent;
     }

     //------------------------------------------------------------------------------------------------
     Color GetDebugColor()
     {
         return Color.FromInt(m_DebugColor.PackToInt());
     }

     //------------------------------------------------------------------------------------------------
  bool IsGridUpdateIdGreaterThan(int gridUpdateId)
     {
         return m_iGridUpdateId > gridUpdateId;
     }

     //------------------------------------------------------------------------------------------------
  bool IsDebugVisualizationEnabled()
     {
         return m_bEnableDebugVisualization;
     }

     //------------------------------------------------------------------------------------------------
  bool IsVisible()
     {
         return m_bIsVisible;
     }

     //------------------------------------------------------------------------------------------------
  bool IsOwnerActive()
     {
         return m_bIsOwnerActive;
     }

     //------------------------------------------------------------------------------------------------
     protected bool ShouldConsumersBeReplicated()
     {
         return m_aConsumers != null;
     }

     //------------------------------------------------------------------------------------------------
     protected bool ShouldGeneratorsBeReplicated()
     {
         return m_aGenerators != null;
     }

     //------------------------------------------------------------------------------------------------
  void SetGridUpdateId(int gridUpdateId)
     {
         if (m_iGridUpdateId > gridUpdateId)
             return;

         m_iGridUpdateId = gridUpdateId;
     }

     //------------------------------------------------------------------------------------------------
  void SetIsVisible(bool state)
     {
         m_bIsVisible = state;
         m_bIsNetDirty = true;

         ReplicateEx();
         OnVisibilityChanged();
     }

     //------------------------------------------------------------------------------------------------
  void SetGridContainersBoundsMins(int mins)
     {
         m_iGridContainersBoundsMins = mins;
     }

     //------------------------------------------------------------------------------------------------
  void SetGridContainersBoundsMaxs(int maxs)
     {
         m_iGridContainersBoundsMaxs = maxs;
     }

     //------------------------------------------------------------------------------------------------
  void SetGridContainersBounds(int mins, int maxs)
     {
         m_iGridContainersBoundsMins = mins;
         m_iGridContainersBoundsMaxs = maxs;
     }

     //------------------------------------------------------------------------------------------------
  void UpdateLastPosition()
     {
         m_LastPosition = GetOwner().GetOrigin();
     }

     //------------------------------------------------------------------------------------------------
  void FlagForProcessing()
     {
         if (m_bIsFlaggedForProcessing)
             return;

         GetGame().GetResourceGrid().FlagResourceItem(this);

         m_bIsFlaggedForProcessing = true;
     }

     //------------------------------------------------------------------------------------------------
  void UnflagForProcessing()
     {
         if (!m_bIsFlaggedForProcessing)
             return;

         GetGame().GetResourceGrid().UnflagResourceItem(this);

         m_bIsFlaggedForProcessing = false;
     }

     //------------------------------------------------------------------------------------------------
  void DeleteConsumers()
     {
         if (!m_aConsumers)
             return;

         delete m_aConsumers;
     }

     //------------------------------------------------------------------------------------------------
  void DeleteGenerators()
     {
         if (!m_aGenerators)
             return;

         delete m_aGenerators;
     }

     //------------------------------------------------------------------------------------------------
  void DeleteQueryInteractors()
     {
         delete m_aConsumers;
         delete m_aGenerators;
     }

     //------------------------------------------------------------------------------------------------
  override event protected void OnPostInit(IEntity owner)
     {
         super.OnPostInit(owner);
         Initialize(owner);
         SetEventMask(owner, EntityEvent.INIT);

         //~ Remove any duplicate entries
         if (m_aDisabledResourceTypes.IsEmpty())
         {
             //~ TODO: Make this cleaner

             set<EResourceType> duplicateRemoveSet = new set<EResourceType>();

             foreach (EResourceType resourceType : m_aDisabledResourceTypes)
             {
                 duplicateRemoveSet.Insert(resourceType);
             }

             m_aDisabledResourceTypes.Clear();
             foreach (EResourceType resourceType : duplicateRemoveSet)
             {
                 m_aDisabledResourceTypes.Insert(resourceType);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  void Initialize(notnull IEntity owner)
     {
         if (m_bIsInitialized)
             return;

         //The replication component is a must, as the authority is the only one allowed to perform an update on the container and/or consumer.
         m_ReplicationComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
         FactionAffiliationComponent factionAffiliationComponentTemp;
         IEntity parentEntity = owner.GetParent();

         while (parentEntity)
         {
             factionAffiliationComponentTemp = FactionAffiliationComponent.Cast(parentEntity.FindComponent(FactionAffiliationComponent));

             if (factionAffiliationComponentTemp)
                 m_FactionAffiliationComponent = factionAffiliationComponentTemp;

             parentEntity = parentEntity.GetParent();
         }

         // In the case that no parent has a faction affiliation component, then get the owner's.
         if (!m_FactionAffiliationComponent)
             m_FactionAffiliationComponent = FactionAffiliationComponent.Cast(owner.FindComponent(FactionAffiliationComponent));

         Physics physics = owner.GetPhysics();
         float maxLength;
         vector tempBoundsMaxs, tempBoundsMins;

         // ---------------------------------------------------------------- Container initialisation.
         // The container is configured through SCR_ResourceComponentClass.
         // Note: Order matters, containers should be processed first for the optimization on when the self resource right is enabled on them.
         SCR_ResourceComponentClass prefabData = SCR_ResourceComponentClass.Cast(GetComponentData(owner));

         if (prefabData)
         {
             // Container instances holding the initial configuration for this component instance's containers.
             array<ref SCR_ResourceContainer> containers = prefabData.GetContainers();

             if (containers)
             {
                 SCR_ResourceContainer containerInstance;
                 typename containerInstanceTypename;

                 m_aContainerInstances.Reserve(containers.Count());

                 foreach (SCR_ResourceContainer container: containers)
                 {
                     containerInstanceTypename = container.Type();

                     containerInstance = SCR_ResourceContainer.Cast(containerInstanceTypename.Spawn());

                     // The copying of the container configuration in the prefab data happens here.
                     containerInstance.Initialize(owner, container);

                     int maxPosition = m_aContainerInstances.Count();
                     int position;
                     EResourceType resourceType = container.GetResourceType();
                     EResourceType compareResourceType;
                     int comparePosition;
                     SCR_ResourceContainer compareContainer;

                     while (position < maxPosition)
                     {
                         comparePosition     = position + ((maxPosition - position) >> 1);
                         compareContainer    = m_aContainerInstances[comparePosition];
                         compareResourceType = compareContainer.GetResourceType();

                         if (resourceType > compareResourceType)
                             position = comparePosition + 1;

                         else if (resourceType < compareResourceType)
                             maxPosition = comparePosition;

                         else
                             break;
                     }

                     // Clean container instance to copy the prefab container configuration to.
                     m_aContainerInstances.InsertAt(containerInstance, position);

                     if (container.IsIsolated())
                         continue;

                     containerInstance.GetAxisAlignedBoundingVolume(tempBoundsMins, tempBoundsMaxs);
                 }

                 m_vGridContainersBoundingVolumeMaxs = tempBoundsMaxs;
                 m_vGridContainersBoundingVolumeMins = tempBoundsMins;
             }
         }

         // ------------------------------------------------------------- Encapsulator initialization.
         if (m_aEncapsulators)
         {
             array<ref SCR_ResourceEncapsulator> encapsulators = {};
             encapsulators.Reserve(m_aEncapsulators.Count());

             foreach (SCR_ResourceEncapsulator encapsulator: m_aEncapsulators)
             {
                 encapsulator.Initialize(owner);

                 int position;
                 int maxPosition = encapsulators.Count();
                 EResourceType resourceType = encapsulator.GetResourceType();
                 EResourceType compareResourceType;
                 int comparePosition;
                 SCR_ResourceEncapsulator compareEncapsulator;

                 while (position < maxPosition)
                 {
                     comparePosition = position + ((maxPosition - position) >> 1);
                     compareEncapsulator = encapsulators[comparePosition];
                     compareResourceType = compareEncapsulator.GetResourceType();

                     if (resourceType > compareResourceType)
                         position = comparePosition + 1;

                     else if (resourceType < compareResourceType)
                         maxPosition = comparePosition;

                     else
                         break;
                 }

                 encapsulators.InsertAt(encapsulator, position);
             }

             m_aEncapsulators = encapsulators;
         }

         // ----------------------------------------------------------------- Consumer initialization.
         if (m_aConsumers)
         {
             array<ref SCR_ResourceConsumer> consumers = {};
             consumers.Reserve(m_aConsumers.Count());

             foreach (SCR_ResourceConsumer consumer: m_aConsumers)
             {
                 SCR_ResourceContainer container = GetContainer(consumer.GetResourceType());

                 if (container && container.IsEncapsulated())
                     continue;

                 consumer.Initialize(owner);

                 int position;
                 int maxPosition = consumers.Count();
                 EResourceGeneratorID generatorIdentifier = consumer.GetGeneratorIdentifier();
                 EResourceGeneratorID compareGeneratorIdentifier;
                 EResourceType resourceType = consumer.GetResourceType();
                 EResourceType compareResourceType;
                 int comparePosition;
                 SCR_ResourceConsumer compareConsumer;

                 while (position < maxPosition)
                 {
                     comparePosition = position + ((maxPosition - position) >> 1);
                     compareConsumer = consumers[comparePosition];
                     compareGeneratorIdentifier  = compareConsumer.GetGeneratorIdentifier();
                     compareResourceType = compareConsumer.GetResourceType();

                     if (generatorIdentifier > compareGeneratorIdentifier)
                         position = comparePosition + 1;
                     else if (generatorIdentifier < compareGeneratorIdentifier)
                         maxPosition = comparePosition;
                     else if (resourceType > compareResourceType)
                         position = comparePosition + 1;
                     else if (resourceType < compareResourceType)
                         maxPosition = comparePosition;
                     else
                         break;
                 }

                 consumers.InsertAt(consumer, position);
             }

             m_aConsumers = consumers;
         }

         // ---------------------------------------------------------------- Generator initialization.
         if (m_aGenerators)
         {
             array<ref SCR_ResourceGenerator> generators = {};
             generators.Reserve(m_aGenerators.Count());

             foreach (SCR_ResourceGenerator generator: m_aGenerators)
             {
                 SCR_ResourceContainer container = GetContainer(generator.GetResourceType());

                 if (container && container.IsEncapsulated())
                     continue;

                 generator.Initialize(owner);

                 int position;
                 int maxPosition = generators.Count();
                 EResourceGeneratorID identifier = generator.GetIdentifier();
                 EResourceGeneratorID comapareIdentifier;
                 EResourceType resourceType = generator.GetResourceType();
                 EResourceType compareResourceType;
                 int comparePosition;
                 SCR_ResourceGenerator compareGenerator;

                 while (position < maxPosition)
                 {
                     comparePosition     = position + ((maxPosition - position) >> 1);
                     compareGenerator    = generators[comparePosition];
                     comapareIdentifier  = compareGenerator.GetIdentifier();
                     compareResourceType = compareGenerator.GetResourceType();

                     if (identifier > comapareIdentifier)
                         position = comparePosition + 1;
                     else if (identifier < comapareIdentifier)
                         maxPosition = comparePosition;
                     else if (resourceType > compareResourceType)
                         position = comparePosition + 1;
                     else if (resourceType < compareResourceType)
                         maxPosition = comparePosition;
                     else
                         break;
                 }

                 generators.InsertAt(generator, position);
             }

             m_aGenerators = generators;
         }

         if (m_bIsAddedToParentBuffered)
             OnAddedToParentEx(GetOwner().GetChildren(), GetOwner().GetParent());

         Vehicle vehicle = Vehicle.Cast(GetOwner());

         if (vehicle)
             vehicle.GetOnPhysicsActive().Insert(OnVehiclePhysicsActive);

         m_bIsInitialized = true;

         Replicate();
     }

     //------------------------------------------------------------------------------------------------
  override event protected void OnChildAdded(IEntity parent, IEntity child)
     {
         SCR_ResourceComponent childResourceComponent = SCR_ResourceComponent.Cast(child.FindComponent(SCR_ResourceComponent));
         if (!childResourceComponent)
             return;

         Initialize(GetOwner());
     }

     //------------------------------------------------------------------------------------------------
  override event protected void EOnInit(IEntity owner)
     {
         SCR_ResourceContainer container;
         SCR_ResourceContainerQueueBase queue;

         foreach (SCR_ResourceEncapsulator encapsulator: m_aEncapsulators)
         {
             queue = encapsulator.GetContainerQueue();;

             for (int i = queue.GetContainerCount() - 1; i >= 0; --i)
             {
                 container = queue.GetContainerAt(i);

                 if (container.GetResourceValue() == 0.0 && GetGame().GetWorld() && !GetGame().GetWorld().IsEditMode() && container.GetOnEmptyBehavior() == EResourceContainerOnEmptyBehavior.HIDE)
                     container.GetComponent().SetIsVisible(false);
             }
         }

         bool isCompletelyIsolated = true;

         for (int i = m_aContainerInstances.Count() - 1; i >= 0; --i)
         {
             container = m_aContainerInstances[i];
             isCompletelyIsolated &= container.IsIsolated();
         }

         if (isCompletelyIsolated)
         {
             UpdateLastPosition();

             return;
         }

         Vehicle vehicle = Vehicle.Cast(GetOwner().GetRootParent());
         if (vehicle)
             vehicle.GetOnPhysicsActive().Insert(OnVehiclePhysicsActive);

         UpdateLastPosition();
     }

     //------------------------------------------------------------------------------------------------
  protected void DebugDraw()
     {
         // Height for the white arrow.
         float height;

         // The white arrow will point to this position, the origin of the owner entity in this case.
         vector origin = GetOwner().GetOrigin();

         // TODO: Cache the height value and only change it on a event basis.
         if (m_aConsumers && m_eDebugVisualizationFlags & EResourceDebugVisualizationFlags.CONSUMER)
         {
             foreach (SCR_ResourceConsumer consumer: m_aConsumers)
             {
                 // Processes and presents the debugging visualization for the consumer.
                 consumer.DebugDraw();

                 // Sets the height of the arrow to be the same as the consumer resource range if the current height is less than it.
                 height = Math.Max(height, consumer.GetResourceRange());
             }
         }

         if (m_aEncapsulators && m_eDebugVisualizationFlags & EResourceDebugVisualizationFlags.ENCAPSULATOR)
         {
             foreach (SCR_ResourceEncapsulator encapsulator: m_aEncapsulators)
             {
                 encapsulator.DebugDraw();
             }
         }

         // TODO: Cache the height value and only change it on a event basis.
         if (m_aGenerators && m_eDebugVisualizationFlags & EResourceDebugVisualizationFlags.GENERATOR)
         {
             foreach (SCR_ResourceGenerator generator: m_aGenerators)
             {
                 // Processes and presents the debugging visualisation for the generator.
                 generator.DebugDraw();

                 // Sets the height of the arrow to be the same as the generator resource range if the current height is less than it.
                 height = Math.Max(height, generator.GetStorageRange());
             }
         }

         // TODO: Cache the height value and only change it on a event basis.
         if (m_aContainerInstances && m_eDebugVisualizationFlags & EResourceDebugVisualizationFlags.CONTAINER)
         {
             foreach (SCR_ResourceContainer container: m_aContainerInstances)
             {
                 // Processes and presents the debugging visualisation for the container.
                 container.DebugDraw();

                 // Sets the height of the arrow to be the same as the container storage range if the
                 // current height (Same as resource range of the consumer if a consumer is present on the
                 // component or 0.0 otherwise) is less than it.
                 //height = Math.Max(height, container.GetStorageRange());
             }
         }

         Shape.CreateArrow((origin + vector.Up * height), origin, 1.0, 0xFFFFFFFF, ShapeFlags.ONCE | ShapeFlags.NOZBUFFER);
     }

     //------------------------------------------------------------------------------------------------
  void Replicate()
     {
         if (!m_ReplicationComponent || m_ReplicationComponent.IsProxy())
             return;

         m_bIsNetDirty = true;
     }

     //------------------------------------------------------------------------------------------------
  void ReplicateEx()
     {
         if (!m_bIsNetDirty)
             return;

         Replication.BumpMe();

         m_bIsNetDirty = false;

         TEMP_OnInteractorReplicated();
     }

 #ifdef WORKBENCH
     //------------------------------------------------------------------------------------------------
  override event void _WB_AfterWorldUpdate(IEntity owner, float timeSlice)
     {
         if (m_bEnableDebugVisualization)
             DebugDraw();

         super._WB_AfterWorldUpdate(timeSlice);
     }

     //------------------------------------------------------------------------------------------------
  override event bool _WB_OnKeyChanged(IEntity owner, BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
     {
         SCR_ResourceGrid grid = GetGame().GetResourceGrid();

         if (m_aConsumers)
         {
             foreach (SCR_ResourceConsumer consumer: m_aConsumers)
             {
                 consumer.GetContainerQueue().Clear();
             }
         }

         if (m_aGenerators)
         {
             foreach (SCR_ResourceGenerator generator: m_aGenerators)
             {
                 generator.GetContainerQueue().Clear();
             }
         }

         grid.IncreaseGridUpdateId();
         grid.UnregisterResourceItem(this);

         return super._WB_OnKeyChanged(owner, src, key, ownerContainers, parent);
     }
 #endif

     //------------------------------------------------------------------------------------------------
     void OnVisibilityChanged()
     {
         IEntity owner = GetOwner();

         if (m_bIsVisible)
         {
             owner.SetFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE, true);
             SCR_PhysicsHelper.ChangeSimulationState(owner, SimulationState.COLLISION, true);

             return;
         }

         owner.ClearFlags(EntityFlags.VISIBLE | EntityFlags.TRACEABLE, true);
         SCR_PhysicsHelper.ChangeSimulationState(owner, SimulationState.NONE, true);
     }

     //------------------------------------------------------------------------------------------------
     override event protected void OnAddedToParent(IEntity child, IEntity parent)
     {
         if (!m_ReplicationComponent || m_ReplicationComponent.IsProxy())
             return;

         if (m_bIsInitialized)
             OnAddedToParentEx(child, parent);
         else
             m_bIsAddedToParentBuffered = true;
     }

     //------------------------------------------------------------------------------------------------
     protected void OnAddedToParentEx(IEntity child, IEntity parent)
     {
         foreach (SCR_ResourceContainer container: m_aContainerInstances)
         {
             if (!container || container.GetStorageType() != EResourceContainerStorageType.ORPHAN)
                 continue;

             container.EnableDecay(false);
         }

         m_bIsAddedToParentBuffered = false;
         m_bHasParent = true;
         bool isCompletelyIsolated = true;

         foreach (SCR_ResourceContainer container: m_aContainerInstances)
         {
             isCompletelyIsolated &= container.IsEncapsulated();
         }

         if (isCompletelyIsolated)
             return;

         Vehicle vehicle = Vehicle.Cast(GetOwner().GetRootParent());

         if (!vehicle)
             return;

         GetGame().GetResourceGrid().PromoteResourceItemToDynamic(this);
         m_bIsDynamic = true;

         vehicle.GetOnPhysicsActive().Insert(OnVehiclePhysicsActive);
     }

     //------------------------------------------------------------------------------------------------
     override event protected void OnRemovedFromParent(IEntity child, IEntity parent)
     {
         if (!m_ReplicationComponent || m_ReplicationComponent.IsProxy())
             return;

         m_bHasParent = false;

         foreach (SCR_ResourceContainer container: m_aContainerInstances)
         {
             if (!container || container.GetStorageType() != EResourceContainerStorageType.ORPHAN)
                 continue;

             container.EnableDecay(true);
         }
     }

     //------------------------------------------------------------------------------------------------
  void OnVehiclePhysicsActive(IEntity owner, bool activeState)
     {
         ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
         if (!world)
             return;

         SCR_ResourceSystem updateSystem = SCR_ResourceSystem.Cast(world.FindSystem(SCR_ResourceSystem));

         m_bIsOwnerActive = activeState;

         if (m_bIsOwnerActive)
         {
             m_bIsDynamic = true;

             GetGame().GetResourceGrid().PromoteResourceItemToDynamic(this);
             updateSystem.RegisterDynamicComponent(this);
         }
         else
         {
             m_bIsDynamic = false;
             GetGame().GetResourceGrid().PromoteResourceItemToStatic(this);
             updateSystem.UnregisterDynamicComponent(this);
         }
     }

     //------------------------------------------------------------------------------------------------
     override event protected void OnDelete(IEntity owner)
     {
         UnflagForProcessing();

         ArmaReforgerScripted game = GetGame();
         if (!game)
             return;

         ChimeraWorld world = ChimeraWorld.CastFrom(game.GetWorld());
         if (!world)
             return;

         SCR_ResourceGrid grid = GetGame().GetResourceGrid();

         grid.IncreaseGridUpdateId();
         grid.UnregisterResourceItem(this);

         SCR_ResourceSystem updateSystem = SCR_ResourceSystem.Cast(world.FindSystem(SCR_ResourceSystem));
         if (!updateSystem)
             return;

         updateSystem.UnregisterDynamicComponent(this);

         delete m_aConsumers;
         delete m_aGenerators;
         delete m_aEncapsulators;
         delete m_aContainerInstances;
     }
 }