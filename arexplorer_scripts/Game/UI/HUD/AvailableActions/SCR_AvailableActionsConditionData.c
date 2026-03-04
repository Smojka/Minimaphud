 //------------------------------------------------------------------------------------------------
 class SCR_AvailableActionsConditionData
 {
     protected ChimeraCharacter m_Character;
     protected CharacterControllerComponent m_CharacterController;
     protected SCR_InventoryStorageManagerComponent m_StorageManager;

     protected bool m_bIsValid;

     protected ECharacterStance m_eCharacterStance;
     // Type of current compartment
     protected ECompartmentType m_eCompartmentType;
     protected bool m_bIsCharacterADS;
     protected bool m_bIsCharacterInVehicle;
     protected bool m_bIsCharacterGettingIn;
     protected bool m_bIsCharacterGettingOut;
     protected bool m_bIsCharacterSprinting;
     protected bool m_bIsCharacterSwimming;
     protected bool m_bIsCharacterFalling;
     protected bool m_bIsCharacterReloading;
     protected bool m_bCanCharacterGetOutVehicle;
     protected bool m_bIsCharacterWeaponRaised;
     protected bool m_bCanCharacterFireWeapon;
     protected bool m_bIsCharacterUsingItem;
     protected bool m_bIsCharacterBleeding;
     protected bool m_bIsInventoryOpen;
     protected bool m_bIsQuickSlotAvailable;
     protected bool m_bIsQuickSlotShown;
     protected bool m_bIsGadgetSelection;
     protected bool m_bIsWeaponManipulation;
     protected float m_fFocusMode;
     protected float m_fBleedingTime;
     protected float m_fSprintingTime;
     protected int m_iAdditionalMagazines;
     protected int m_iMedicalItemCount;
     protected int m_iMedicalItemCountInQuickSlots;
     protected IEntity m_CurrentItemEntity;
     protected IEntity m_CurrentWeaponEntity;
     protected BaseWeaponComponent m_CurrentWeapon;
     protected BaseMuzzleComponent m_CurrentMuzzle;
     protected BaseMagazineComponent m_CurrentMagazine;

     protected IEntity m_CurrentVehicle;
     protected float m_fTurboTime;
     protected BaseControllerComponent m_CurrentVehicleController;
     protected SignalsManagerComponent m_CurrentVehicleSignals;
     protected BaseWeaponComponent m_CurrentVehicleWeapon;

     // Character health state
     protected SCR_CharacterDamageManagerComponent m_CharacterDamageComponent;
     protected bool m_bIsCharacterConscious;
     protected bool m_bHasBleedingLimbs;
     protected bool m_bHasTourniquetLimb;

     // Von variables ----------------------
     protected SCR_VoNComponent m_VON;
     protected SCR_VONController m_VONController;

     protected bool m_bCharacterIsUsingRadio;
     protected bool m_bCharacterRadiosCount;
     bool m_bIsRadioLongRange;
     bool m_bOwnsLongRangeRadio;

     protected bool m_bCanFetchInventory = true;

     protected SCR_MapEntity m_MapEntity;
     bool m_bCanRotateMapElement;
     bool m_bCanActivateMapElement;
     bool m_bCanDragMapElement;

     //------------------------------------------------------------------------------------------------
     private void OnItemAddedListener(IEntity item, BaseInventoryStorageComponent storage)
     {
         m_bCanFetchInventory = true;
     }

     private void OnItemRemovedListener(IEntity item, BaseInventoryStorageComponent storage)
     {
         m_bCanFetchInventory = true;
     }

     private void OnInventoryOpen(bool open)
     {
         if (open)
         {
             SCR_InventoryAvailableCondition.IncrementCounter();
         }
         m_bIsInventoryOpen = open;
     }

     private void OnQuickSlotOpen(bool open)
     {
         if (open)
         {
             SCR_WeaponQuickSlotAvailableCondition.IncrementCounter();
         }
         m_bIsQuickSlotAvailable = open;
         m_bIsQuickSlotShown = open;
     }

     //------------------------------------------------------------------------------------------------
  private void Clear()
     {
         m_eCharacterStance = ECharacterStance.STAND;
         m_eCompartmentType = ECompartmentType.Pilot;
         m_bIsCharacterADS = false;
         m_bIsCharacterInVehicle = false;
         m_bIsCharacterGettingIn = false;
         m_bIsCharacterGettingOut = false;
         m_bIsCharacterSwimming = false;
         m_bIsCharacterSprinting = false;
         m_bIsCharacterFalling = false;
         m_bIsCharacterReloading = false;
         m_bIsCharacterWeaponRaised = false;
         m_bCanCharacterFireWeapon = false;
         m_bIsCharacterUsingItem = false;
         m_bIsGadgetSelection = false;
         m_bIsWeaponManipulation = false;
         m_fFocusMode = 0.0;
         //m_iAdditionalMagazines = 0;
         m_CurrentItemEntity = null;
         m_CurrentWeaponEntity = null;
         m_CurrentWeapon = null;
         m_CurrentMuzzle = null;
         m_CurrentMagazine = null;
         m_CurrentVehicleWeapon = null;

         // Health
         m_CharacterDamageComponent = null;
         m_bIsCharacterConscious = false;
         m_bHasBleedingLimbs = false;
         m_bHasTourniquetLimb = false;

         m_bIsValid = false;
     }

     /* Gadgets magic */
     protected SCR_GadgetManagerComponent m_GadgetManager;

     //------------------------------------------------------------------------------------------------
     // Getters
     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
     ChimeraCharacter GetCharacter()
     {
         return m_Character;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetHeldGadget()
     {
         if (m_GadgetManager)
             return m_GadgetManager.GetHeldGadget();
         return null;
     }

     //------------------------------------------------------------------------------------------------
     SCR_GadgetComponent GetHeldGadgetComponent()
     {
         if (m_GadgetManager)
             return m_GadgetManager.GetHeldGadgetComponent();
         return null;
     }

     //------------------------------------------------------------------------------------------------
  bool GetGadgetRaised()
     {
         if (m_GadgetManager)
             return m_GadgetManager.GetIsGadgetADS();
         return null;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetGadget(EGadgetType type)
     {
         if (m_GadgetManager)
             return m_GadgetManager.GetGadgetByType(type);
         return null;
     }

     //------------------------------------------------------------------------------------------------
     BaseMagazineComponent GetCurrentMagazine()
     {
         return m_CurrentMagazine;
     }

     //------------------------------------------------------------------------------------------------
  BaseMuzzleComponent GetCurrentMuzzle()
     {
         return m_CurrentMuzzle;
     }

     //------------------------------------------------------------------------------------------------
  BaseWeaponComponent GetCurrentWeapon()
     {
         return m_CurrentWeapon;
     }

     //------------------------------------------------------------------------------------------------
  ECharacterStance GetCharacterStance()
     {
         return m_eCharacterStance;
     }

     //------------------------------------------------------------------------------------------------
  ECompartmentType GetCompartmentType()
     {
         return m_eCompartmentType;
     }

     //------------------------------------------------------------------------------------------------
  float GetFocusModeAmount()
     {
         return m_fFocusMode;
     }

     //------------------------------------------------------------------------------------------------
  float GetCharacterBleedingTime()
     {
         return m_fBleedingTime;
     }

     //------------------------------------------------------------------------------------------------
  float GetCharacterSprintingTime()
     {
         return m_fSprintingTime;
     }

     //------------------------------------------------------------------------------------------------
  int GetAdditionalMagazinesCount()
     {
         return m_iAdditionalMagazines;
     }

     //------------------------------------------------------------------------------------------------
  int GetMedicalItemCount()
     {
         return m_iMedicalItemCount;
     }

     //------------------------------------------------------------------------------------------------
     int GetMedicalItemCountInQuickSlots()
     {
         return m_iMedicalItemCountInQuickSlots;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterInVehicle()
     {
         return m_bIsCharacterInVehicle;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterGettingIn()
     {
         return m_bIsCharacterGettingIn;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterGettingOut()
     {
         return m_bIsCharacterGettingOut;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterSprinting()
     {
         return m_bIsCharacterSprinting;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterSwimming()
     {
         return m_bIsCharacterSwimming;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterFalling()
     {
         return m_bIsCharacterFalling;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterReloading()
     {
         return m_bIsCharacterReloading;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterADS()
     {
         return m_bIsCharacterADS;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterWeaponRaised()
     {
         return m_bIsCharacterWeaponRaised;
     }

     //------------------------------------------------------------------------------------------------
  bool GetCanCharacterFireWeapon() { return m_bCanCharacterFireWeapon; }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterUsingItem()
     {
         return m_bIsCharacterUsingItem;
     }

     //------------------------------------------------------------------------------------------------
  bool GetCanCharacterGetOutVehicle()
     {
         return m_bCanCharacterGetOutVehicle;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsCharacterBleeding()
     {
         return m_bIsCharacterBleeding;
     }

     //------------------------------------------------------------------------------------------------
     bool GetIsCharacterUsingRadio()
     {
         return m_bCharacterIsUsingRadio;
     }

     //------------------------------------------------------------------------------------------------
     int GetCharacterRadiosCount()
     {
         return m_bCharacterRadiosCount;
     }

     //------------------------------------------------------------------------------------------------
     bool IsInventoryOpen()
     {
         return m_bIsInventoryOpen;
     }

     //------------------------------------------------------------------------------------------------
     bool IsQuickSlotAvailable()
     {
         return m_bIsQuickSlotAvailable;
     }

     //------------------------------------------------------------------------------------------------
     bool IsQuickSlotShown()
     {
         return m_bIsQuickSlotShown;
     }

     //------------------------------------------------------------------------------------------------
     bool IsGadgetSelection()
     {
         return m_bIsGadgetSelection;
     }

     //------------------------------------------------------------------------------------------------
     bool IsWeaponManipulation()
     {
         return m_bIsWeaponManipulation;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetCurrentItemEntity()
     {
         return m_CurrentItemEntity;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetCurrentWeaponEntity()
     {
         return m_CurrentWeaponEntity;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetCurrentVehicle()
     {
         return m_CurrentVehicle;
     }

     //------------------------------------------------------------------------------------------------
     BaseControllerComponent GetCurrentVehicleController()
     {
         return m_CurrentVehicleController;
     }

     //------------------------------------------------------------------------------------------------
     SignalsManagerComponent GetCurrentVehicleSignals()
     {
         return m_CurrentVehicleSignals;
     }

     //------------------------------------------------------------------------------------------------
  BaseWeaponComponent GetCurrentVehicleWeapon()
     {
         return m_CurrentVehicleWeapon;
     }

     //------------------------------------------------------------------------------------------------
  float GetCurrentVehicleTurboTime()
     {
         return m_fTurboTime;
     }

     //------------------------------------------------------------------------------------------------
     CharacterAnimationComponent GetAnimationComponent()
     {
         if (m_Character)
             return m_Character.GetAnimationComponent();

         return null;
     }

     //------------------------------------------------------------------------------------------------
     SCR_CharacterDamageManagerComponent GetCharacterDamageComponent()
     {
         return m_CharacterDamageComponent;
     }

     //------------------------------------------------------------------------------------------------
     bool GetIsCharacterConscious()
     {
         return m_bIsCharacterConscious;
     }

     //------------------------------------------------------------------------------------------------
     bool GetHasBleedingLimbs()
     {
         return m_bHasBleedingLimbs;
     }

     //------------------------------------------------------------------------------------------------
     bool GetHasTourniquetLimb()
     {
         return m_bHasTourniquetLimb;
     }

     //------------------------------------------------------------------------------------------------
     bool IsValid()
     {
         return m_bIsValid;
     }

     //------------------------------------------------------------------------------------------------
     // Fetch data
     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
  void FetchData(IEntity controlledEntity, float timeSlice)
     {
         // Detect when character changes
         ChimeraCharacter character = ChimeraCharacter.Cast(controlledEntity);
         if (character != m_Character)
         {
             m_Character = character;

             if (character)
             {
                 m_CharacterController = character.GetCharacterController();
                 m_GadgetManager = SCR_GadgetManagerComponent.GetGadgetManager(character);
                 m_VON = SCR_VoNComponent.Cast(character.FindComponent(SCR_VoNComponent));
             }
         }

         if (!m_Character)
         {
             m_CharacterController = null;
             m_GadgetManager = null;
             m_VON = null;

             m_bIsValid = false;
             return;
         }

         if (!m_CharacterController)
         {
             m_bIsValid = false;
             return;
         }

         // Add inventory listeners for efficient magazine/grenade count updates
         SCR_InventoryStorageManagerComponent storageManager = SCR_InventoryStorageManagerComponent.Cast(m_CharacterController.GetInventoryStorageManager());
         if (storageManager != m_StorageManager)
         {
             if (m_StorageManager)
             {
                 m_StorageManager.m_OnItemAddedInvoker.Remove(OnItemAddedListener);
                 m_StorageManager.m_OnItemRemovedInvoker.Remove(OnItemRemovedListener);
                 m_StorageManager.m_OnInventoryOpenInvoker.Remove(OnInventoryOpen);
                 m_StorageManager.m_OnQuickBarOpenInvoker.Remove(OnQuickSlotOpen);
             }

             m_StorageManager = storageManager;

             if (m_StorageManager)
             {
                 m_StorageManager.m_OnItemAddedInvoker.Insert(OnItemAddedListener);
                 m_StorageManager.m_OnItemRemovedInvoker.Insert(OnItemRemovedListener);
                 m_StorageManager.m_OnInventoryOpenInvoker.Insert(OnInventoryOpen);
                 m_StorageManager.m_OnQuickBarOpenInvoker.Insert(OnQuickSlotOpen);
             }
         }

         // Invalidates and clears any data prior to following collection
         Clear();

         // Current character stance
         m_eCharacterStance = m_CharacterController.GetStance();
         // Is character ADS?
         m_bIsCharacterADS = m_CharacterController.IsWeaponADS();
         // Is character sprinting?
         m_bIsCharacterSprinting = m_CharacterController.IsSprinting();
         // Is character swimming?
         m_bIsCharacterSwimming = m_CharacterController.IsSwimming();
         // Is character falling? (is airborne?)
         m_bIsCharacterFalling = m_CharacterController.IsFalling();
         // Is character in vehicle?
         m_bIsCharacterInVehicle = m_Character.IsInVehicle();
         // Can character get out?
         m_bCanCharacterGetOutVehicle = m_CharacterController.CanGetOutVehicleScript();
         // Is character weapon raised?
         m_bIsCharacterWeaponRaised = m_CharacterController.IsWeaponRaised();
         // Can character fire weapon
         m_bCanCharacterFireWeapon = m_CharacterController.CanFire();
         // Is character currently using an item?
         m_bIsCharacterUsingItem = m_CharacterController.IsUsingItem();
         // Item that character is holding in his right hand
         m_CurrentItemEntity = m_CharacterController.GetAttachedGadgetAtLeftHandSlot();

         // Temporary sprinting time tracking
         if (m_bIsCharacterSprinting && !m_CharacterController.GetIsSprintingToggle())
             m_fSprintingTime += timeSlice;
         else
             m_fSprintingTime = 0;

         // Vehicle compartment and controls
         CompartmentAccessComponent compartmentAccess = m_Character.GetCompartmentAccessComponent();

         if (compartmentAccess)
         {
             m_bCanCharacterGetOutVehicle = m_bCanCharacterGetOutVehicle && compartmentAccess.CanGetOutVehicle();
             m_bIsCharacterGettingIn = compartmentAccess.IsGettingIn();
             m_bIsCharacterGettingOut = compartmentAccess.IsGettingOut();

             // Vehicle compartment
             BaseCompartmentSlot slot = compartmentAccess.GetCompartment();
             if (slot)
             {
                 // Vehicle
                 IEntity vehicle = slot.GetOwner();

                 // In case the slot is actually in the injected compartment, like the backseats of Jeep
                 vehicle = GetVehicle(vehicle);

                 if (vehicle != m_CurrentVehicle)
                 {
                     m_CurrentVehicle = vehicle;

                     if (vehicle)
                     {
                         m_CurrentVehicleSignals = SignalsManagerComponent.Cast(vehicle.FindComponent(SignalsManagerComponent));
                     }
                 }

                 // Temporary turbo time tracking
                 // TODO: Condition activation time
                 m_eCompartmentType = slot.GetType();
                 if (m_eCompartmentType == ECompartmentType.Pilot && GetGame().GetInputManager().GetActionTriggered("CarTurbo"))
                     m_fTurboTime += timeSlice;
                 else
                     m_fTurboTime = 0;

                 // Turret controls
                 m_CurrentVehicleController = slot.GetController();
                 TurretControllerComponent turretController = TurretControllerComponent.Cast(m_CurrentVehicleController);
                 if (turretController)
                 {
                     BaseWeaponManagerComponent weaponManager = turretController.GetWeaponManager();
                     if (weaponManager)
                         m_CurrentVehicleWeapon = weaponManager.GetCurrentWeapon();
                 }
             }
             else
             {
                 m_CurrentVehicle = null;
             }
         }

         // Clear vehicle variables
         if (!m_CurrentVehicle)
         {
             m_CurrentVehicleSignals = null;
             m_CurrentVehicleController = null;
         }

         // Current character weapon manager
         // Current character weapon
         BaseWeaponManagerComponent weaponManager = m_CharacterController.GetWeaponManagerComponent();
         if (weaponManager)
         {
             // Weapon slot -> weapon entity
             WeaponSlotComponent currentSlot = weaponManager.GetCurrentSlot();
             if (currentSlot)
                 m_CurrentWeaponEntity = currentSlot.GetWeaponEntity();

             // BaseWeaponComponent
             BaseWeaponComponent currentWeapon = weaponManager.GetCurrent();
             if (currentWeapon != m_CurrentWeapon)
             {
                 m_CurrentWeapon = currentWeapon;
                 m_bCanFetchInventory = true;
             }

             // Muzzle and magazine
             if (m_CurrentWeapon)
             {
                 m_CurrentMuzzle = m_CurrentWeapon.GetCurrentMuzzle();
                 m_CurrentMagazine = m_CurrentWeapon.GetCurrentMagazine();
             }

             // Is character reloading
             m_bIsCharacterReloading = m_CharacterController.IsReloading();
         }

         // Does character have additional mags for current weapon?
         if (m_bCanFetchInventory && m_StorageManager)
         {
             if (m_CurrentWeapon)
                 m_iAdditionalMagazines = m_StorageManager.GetMagazineCountByWeapon(m_CurrentWeapon);

             m_iMedicalItemCount = m_StorageManager.GetHealthComponentCount();

             // Check medical items in quick slots
             SCR_CharacterInventoryStorageComponent characterStorage = m_StorageManager.GetCharacterStorage();
             if (characterStorage)
             {
                 m_iMedicalItemCountInQuickSlots = 0;

                 array<IEntity> items = characterStorage.GetQuickSlotItems();
                 foreach (IEntity item : items)
                 {
                     if (item && item.FindComponent(SCR_ConsumableItemComponent))
                         m_iMedicalItemCountInQuickSlots++;
                 }
             }

             m_bCanFetchInventory = false;
         }

         // Camera handler to check focus mode
         CameraHandlerComponent cameraHandler = m_CharacterController.GetCameraHandlerComponent();
         if (cameraHandler)
             m_fFocusMode = cameraHandler.GetFocusMode();

         // Fetch available gadgets
         m_bIsGadgetSelection = GetGame().GetInputManager().GetActionTriggered("GadgetSelection");
         m_bIsWeaponManipulation = GetGame().GetInputManager().GetActionTriggered("WeaponManipulation");

         // VON status
         if (m_VONController)
             m_bCharacterRadiosCount = m_VONController.GetVONEntryCount();

         // VON usage
         if (m_VON)
         {
             m_bCharacterIsUsingRadio = m_VON.IsTransmitingRadio();
             if (m_bCharacterIsUsingRadio)
                 m_bIsRadioLongRange = m_VONController.IsUsingLRR();

             m_bOwnsLongRangeRadio = m_VONController.IsLRRAvailable();
         }

         // Addition data
         FetchHealthData(timeSlice);

         // Map data
         if (m_MapEntity && m_MapEntity.IsOpen())
             FetchMapData(timeSlice);
     }

     //------------------------------------------------------------------------------------------------
  protected IEntity GetVehicle(IEntity vehicle)
     {
         IEntity parent;

         if (vehicle.Type() != Vehicle)
             parent = vehicle.GetParent();

         if (parent)
             vehicle = GetVehicle(parent);

         return vehicle;
     }

     //------------------------------------------------------------------------------------------------
  protected void FetchHealthData(float timeSlice)
     {
         m_CharacterDamageComponent = SCR_CharacterDamageManagerComponent.Cast(m_Character.GetDamageManager());
         if (!m_CharacterDamageComponent)
         {
             Print("[SCR_AvailableActionsConditionData] - can't fetch health data!", LogLevel.WARNING);
             return;
         }

         // Check bleeding
         if (m_CharacterDamageComponent)
             m_bIsCharacterBleeding = m_CharacterDamageComponent.IsDamagedOverTime(EDamageType.BLEEDING);

         // Bleeding time tracking
         if (m_bIsCharacterBleeding)
             m_fBleedingTime += timeSlice;
         else
             m_fBleedingTime = 0;

         if (m_CharacterController)
             m_bIsCharacterConscious = !m_CharacterController.IsUnconscious();

         // Has tourniquet on any limb
         array<ECharacterHitZoneGroup> limbs = {};
         m_CharacterDamageComponent.GetAllExtremities(limbs);

         for (int i = 0, count = limbs.Count(); i < count; i++)
         {
             m_bHasTourniquetLimb = m_CharacterDamageComponent.GetGroupTourniquetted(limbs[i]);
             if (m_bHasTourniquetLimb)
                 break;
         }
     }

     //------------------------------------------------------------------------------------------------
  protected void FetchMapData(float timeSlice)
     {
         m_bCanRotateMapElement = false;
         m_bCanDragMapElement = false;
         m_bCanActivateMapElement = false;

         array<Widget> mapWidgetsUnderCursor = SCR_MapCursorModule.GetMapWidgetsUnderCursor();

         SCR_MapElementMoveComponent moveComp;
         foreach (Widget widget : mapWidgetsUnderCursor)
         {
             if (!widget)
                 continue;

             moveComp = SCR_MapElementMoveComponent.Cast(widget.FindHandler(SCR_MapElementMoveComponent));
             if (!moveComp)
                 continue;

             if (moveComp.m_bCanRotate)
                 m_bCanRotateMapElement = true;

             if (moveComp.m_bCanDrag)
                 m_bCanDragMapElement = true;

             if (moveComp.m_bCanActivate)
                 m_bCanActivateMapElement = true;
         }
     }

     //------------------------------------------------------------------------------------------------
     // Contructor
     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
  void SCR_AvailableActionsConditionData()
     {
         m_VONController = SCR_VONController.Cast(GetGame().GetPlayerController().FindComponent(SCR_VONController));
         m_MapEntity = SCR_MapEntity.GetMapInstance();
     }
 };