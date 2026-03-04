 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ChimeraCharacterClass: GameEntityClass
 {
 }

 class ChimeraCharacter: GameEntity
 {
     // Returns HUD display for this character
     proto external BaseInfoDisplay GetInfoDisplay();
     proto external CharacterAnimationComponent GetAnimationComponent();
     proto external CompartmentAccessComponent GetCompartmentAccessComponent();
     proto external CharacterControllerComponent GetCharacterController();
     proto external SCR_DamageManagerComponent GetDamageManager();
     proto external BaseWeaponManagerComponent GetWeaponManager();
     proto external bool IsInVehicle();
     proto external bool IsInVehicleADS();
     proto external vector AimingPosition();
     proto external vector EyePosition();
     proto external vector EyePositionModel();
     proto external AimingComponent GetHeadAimingComponent();
     proto external AimingComponent GetWeaponAimingComponent();
     proto external void DoStartObjectAction(BaseUserAction pAction);
     proto external void DoPerformObjectAction(BaseUserAction pAction);
     proto external void DoPerformContinuousObjectAction(BaseUserAction pAction, float timeSlice);
     proto external void DoCancelObjectAction(BaseUserAction pAction);
 }
