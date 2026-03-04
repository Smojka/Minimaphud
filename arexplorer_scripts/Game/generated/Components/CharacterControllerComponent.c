 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterControllerComponentClass: PrimaryControllerComponentClass
 {
 }

 class CharacterControllerComponent: PrimaryControllerComponent
 {
     proto external CharacterAimingComponent GetAimingComponent();
     proto external CharacterHeadAimingComponent GetHeadAimingComponent();
     proto external CharacterAnimationComponent GetAnimationComponent();
     proto external BaseWeaponManagerComponent GetWeaponManagerComponent();
     proto external CameraHandlerComponent GetCameraHandlerComponent();
     proto external InventoryStorageManagerComponent GetInventoryStorageManager();
     proto external VoNComponent GetVONComponent();
     proto external AIControlComponent GetAIControlComponent();
     proto external EntitySlotInfo GetRightHandPointInfo();
     proto external EntitySlotInfo GetLeftHandPointInfo();
     proto external CharacterInputContext GetInputContext();
     proto external float GetMovementSpeed();
     proto external void SetMovement(float movementSpeed, vector movementDirModel);
     proto external void SetHeadingAngle(float newHeadingAngle, bool adjustAimingYaw = false);
     proto external ECharacterStance GetStance();
     proto external SCR_ChimeraCharacter GetCharacter();
     proto external void SetFireWeaponWanted(bool val);
     proto external void SetThrow(bool val, bool cancelThrow);
     proto external void SetMovementDirWorld(vector movementDirWorld);
     proto external vector GetMovementDirWorld();
     proto external void SetWeaponRaised(bool val);
     proto external void SetWeaponADS(bool val);
     proto external void SetFreeLook(bool input, bool mouse, bool controller);
     proto external bool GetFreeLookInput();
     proto external bool GetFreeLookMouse();
     proto external bool GetFreeLookController();
     proto external void SetForcedFreeLook(bool enabled);
     proto external bool IsFreeLookForced();
     proto external void ResetPersistentStates(bool resetADSState = true, bool resetGadgetState = true);
     proto external void SetDynamicSpeed(float value);
     proto external float GetDynamicSpeed();
     proto external void SetShouldApplyDynamicSpeedOverride(bool shouldApply);
     proto external void SetDynamicStance(float value);
     proto external bool CanSetDynamicStance(float value);
     proto external float GetDynamicStance();
     proto external void SetInspect(IEntity targetItem);
     proto external bool CanInspect(IEntity targetItem);
     proto external bool GetInspect();
     proto external bool GetInspectCurrentWeapon();
     proto external IEntity GetInspectEntity();
     proto external void SetInspectState(int state);
     proto external int GetInspectState();
     proto external bool GetIsWeaponDeployed();
     proto external bool CanDeployWeapon();
     proto external bool GetIsWeaponDeployedBipod();
     proto external IEntity GetCurrentItemInHands();
     proto external void SetStanceChange(int stance);
     proto external bool CanChangeStance(int stance);
     proto external void ForceStance(int stance);
     proto external void ForceStanceUp(int stance);
     proto external void SetRoll(int val);
     proto external bool IsRoll();
     proto external bool CanRoll(int val);
     proto external bool ShouldHoldInputForRoll();
     proto external void EnableHoldInputForRoll(bool enable);
     proto external bool IsPlayerControlled();
     proto external void SetJump(float val);
     proto external void SetWantedLeaning(float val);
     proto external void SetBanking(float val);
     proto external void SetMeleeAttack(bool val);
     proto external float GetLeaning();
     proto external float GetWantedLeaning();
     proto external bool IsAdjustingLeaning();
     proto external float GetCurrentLeanAmount();
     proto external bool IsLeaning();
     proto external float GetADSTime();
     proto external bool IsWeaponRaised();
     proto external EWeaponObstructedState GetWeaponObstructedState();
     proto external float GetObstructionAlpha();
     proto external bool IsClimbing();
     proto external bool IsSwimming();
     proto external bool IsSprinting();
     proto external bool IsChangingStance();
     proto external bool IsWeaponADS();
     proto external bool IsChangingFireMode();
     proto external bool IsPlayingGesture();
     proto external bool IsFreeLookEnabled();
     proto external bool IsTrackIREnabled();
     proto external bool IsFocusMode();
     proto external bool GetWeaponADSInput();
     proto external bool IsChangingItem();
     proto external bool IsFalling();
     proto external bool IsReloading();
     proto external bool CanFire();
     proto external bool IsDead();
     proto external bool IsUsingItem();
     proto external bool IsMeleeAttack();
     proto external bool CanEngageChangeItem();
     proto external bool SelectWeapon(BaseWeaponComponent newWeapon);
     proto external bool SetMuzzle(int index);
     proto external bool SetFireMode(int index);
     proto external void SetSightsRange(int index);
     proto external void SetWeaponADSInput(bool val);
     proto external bool SetSafety(bool safety, bool automatic);
     //Returns the current stamina value in <0, 1>. -1 if there is no stamina component attached to the current owner.
     proto external float GetStamina();
     proto external bool ReloadWeapon();
     // mag or projectile
     proto external bool ReloadWeaponWith(IEntity ammunitionEntity, bool bForceDetach = false);
     //------------------------------------------------------------------------
     proto external ECharacterLifeState GetLifeState();
     proto external void SetUnconscious(bool enabled);
     proto external bool IsUnconscious();
     proto external bool CanReviveCharacter();
     // Returns true if there is nothing blocking starting unconscious - it does not check whether unconsciousness should actually start.
     proto external bool CanEnterUnconsciousness();
     proto external void EnableReviveCharacter(bool enabled);
     proto external void Ragdoll(bool broadcast = true);
     proto external void ForceDeath();
     /* @NOTE(Leo): Temp solution, eventually will be solved by setting respective gadget graph attachments,
     unfortunately current state of Enf animation system is not allowing it and will be solved not earlier then 10.07 by @Michal Å½Ã¡k
     from conversation with @ThÃ©o Escamez:
     so heres how it works :
     for now we have 4 items> compass adrianov, compass SY183, Radio ANPRC68 and Radio R148
     ...where they are triggered respectively by integers 1 2 3 and 4...
     */
     proto external void TakeGadgetInLeftHand(IEntity gadget, int gadgetType, bool autoFocus = false, bool skipAnimations = false);
     // Performs gadget equip validation
     proto external bool CanEquipGadget(IEntity gadget);
     proto external IEntity GetAttachedGadgetAtLeftHandSlot();
     proto external bool TryUseBandage(IEntity bandage, int bodyPart, float duration = 4.0, bool bandageSelf = true);
     proto external void RemoveGadgetFromHand(bool skipAnimations = false);
     proto external void RecoverHiddenGadget(bool respectSettings, bool skipAnims);
     proto external bool IsGadgetInHands();
     proto external bool SetGadgetRaisedModeWanted(bool newRaised);
     proto external bool IsGadgetRaisedModeWanted();
     proto external bool TryEquipRightHandItem(IEntity item, EEquipItemType type, bool swap = false, BaseUserAction callbackAction = null);
     proto external bool TryRecoverLastRightHandItem(bool swap = false, BaseUserAction callbackAction = null);
     proto external IEntity GetRightHandItem();
     proto external bool TryUseItem(IEntity item, bool allowMovementDuringAction = false, bool keepInHandAfterSuccessfulAction = false);
     proto external bool TryUseItemOverrideParams(notnull ItemUseParameters params);
     proto external bool CanUseItem();
     proto external bool TryStartCharacterGesture(int gesture, int durationMS = 0);
     proto external void StopCharacterGesture();
     proto external bool TryPlayItemGesture(EItemGesture gesture, BaseUserAction callbackAction = null, string confirmEvent = "");
     proto external bool IsPlayingItemGesture();
     proto external bool CanPlayItemGesture();
     proto external bool CanUseLadder(IEntity pLadderOwner, int ladderComponentIndex = 0, float maxTestDistance = -1.0, float maxEntryAngle = -1.0, bool performTraceCheck = false);
     proto external bool TryUseLadder(IEntity pLadderOwner, int ladderComponentIndex = 0, float maxTestDistance = -1.0, float maxEntryAngle = -1.0);
     proto external void DropWeapon(WeaponSlotComponent weaponSlot);
     proto external void DropItemFromLeftHand();
     proto external void StopDeployment();
     // Script
     proto external void RequestActionByID(int actionID, float value);
     proto external bool IsPartiallyLowered();
     proto external bool CanPartialLower();
     proto external void SetPartialLower(bool state);
     //------------------------------------------------------------------------
     proto external vector GetCameraWeaponOffset();
     //------------------------------------------------------------------------
     proto external bool GetDisableMovementControls();
     //------------------------------------------------------------------------
     proto external void SetDisableMovementControls(bool other);
     //------------------------------------------------------------------------
     proto external bool GetDisableViewControls();
     //------------------------------------------------------------------------
     proto external void SetDisableViewControls(bool other);
     //------------------------------------------------------------------------
     proto external void SetAimingSensitivity(float mouse, float gamepad, float ads);
     //------------------------------------------------------------------------
     proto external void GetAimingSensitivity(out float mouse, out float gamepad, out float ads);
     // Sets gadget handling into persistent mode
     proto external void SetStickyGadget(bool enable);
     // Sets ads and gadget focus into persistent mode
     proto external void SetStickyADS(bool enable);
     //------------------------------------------------------------------------
     proto external void SetMaxZoomInADS(bool enable);
     //------------------------------------------------------------------------
     proto external bool GetMaxZoomInADS();
     //------------------------------------------------------------------------
     proto external void SetMouseControlAircraft(bool enable);
     //------------------------------------------------------------------------
     proto external bool GetMouseControlAircraft();
     //------------------------------------------------------------------------
     proto external bool GetDisableWeaponControls();
     //------------------------------------------------------------------------
     proto external void SetDisableWeaponControls(bool other);
     //------------------------------------------------------------------------
     proto external vector GetMovementVelocity();
     //------------------------------------------------------------------------
     proto external vector GetMovementInput();
     //------------------------------------------------------------------------
     proto external vector GetVelocity();
     //------------------------------------------------------------------------
     proto external bool GetIsSprintingToggle();
     //------------------------------------------------------------------------
     proto external float GetLastStanceChangeDelay();
     //------------------------------------------------------------------------
     proto external int GetCurrentMovementPhase();
     //------------------------------------------------------------------------
     proto external bool GetCanFireWeapon();
     //------------------------------------------------------------------------
     proto external bool GetCanThrow();
     //------------------------------------------------------------------------
     proto external void SetWeaponNoFireTime(float t);
     //------------------------------------------------------------------------
     proto external bool IsInThirdPersonView();
     // Caching third person view since profiling showed minor impact on cpu time when calling in to scripted method
     proto external void SetInThirdPersonView(bool state);
     proto external float GetStanceChangeDelayTime();
     proto external float GetJumpSpeed();
     proto external bool GetMeleeAttackInput();
     proto external bool IsFreeLookEnforced();
     proto external bool GetPositionInView(vector pos, float angMax);
     [Obsolete("This method will be removed soon!")]
     proto external void GetWeaponModifiers(float baseScale, float moveScale, out vector scaleA, out vector scaleB);
     proto external CharacterStaminaComponent GetStaminaComponent();
     // Check if character is not moving and not any other locomotion related action is being performed
     proto external bool IsCharacterStationary();

     // callbacks

     event void OnInspectionModeChanged(bool newState);
     event void UpdateDrowning(float timeSlice, vector waterLevel);
     event protected void OnInit(IEntity owner);
     event protected void OnDiag(IEntity owner, float timeslice);
     event protected void OnReloaded(IEntity owner, BaseWeaponComponent weapon);
     event protected void OnPrepareControls(IEntity owner, ActionManager am, float dt, bool player);
     event protected void OnApplyControls(IEntity owner, float timeSlice);
     event protected void OnMeleeDamage(bool started);
     event bool GetCanMeleeAttack() { return true; };
     event bool GetCanEquipGadget(IEntity gadget) { return true; };
     event protected bool OnPerformAction() { return false; };
     event bool CanGetOutVehicleScript() { return true; };
     event bool CanJumpOutVehicleScript() { return true; };
     event protected void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState);
     event protected void OnConsciousnessChanged(bool conscious);
     event protected void OnDeath(IEntity instigatorEntity, notnull Instigator instigator);
     event protected void OnGadgetStateChanged(IEntity gadget, bool isInHand, bool isOnGround);
     event protected void OnGadgetFocusStateChanged(IEntity gadget, bool isFocused);
     event protected void OnItemUseBegan(ItemUseParameters itemUseParams);
     event protected void OnItemUseEnded(ItemUseParameters itemUseParams, bool successful);
     event protected void OnAnimationEvent(AnimationEventID animEventType, AnimationEventID animUserString, int intParam, float timeFromStart, float timeToEnd);
     event float GetInspectTargetLookAt(out vector targetAngles);
     /*Should return true if during CharacterHeadingAnimComponent aligning, the aiming angles should influence aiming angles.*/
     event bool ShouldAligningAdjustAimingAngles();
     event bool ShouldGadgetBeDropped(IEntity gadget);
     event protected void OnWeaponDropped(IEntity pWeaponEntity, WeaponSlotComponent pWeaponSlot);
     event protected void OnItemDroppedFromLeftHand(IEntity pItemEntity);
     //------------------------------------------------------------------------
     event bool SCR_GetDisableMovementControls();
     //------------------------------------------------------------------------
     event void SCR_OnDisabledJumpAction();
     //------------------------------------------------------------------------
     event bool SCR_GetDisableViewControls();
     event protected void OnControlledByPlayer(IEntity owner, bool controlled);
 }
