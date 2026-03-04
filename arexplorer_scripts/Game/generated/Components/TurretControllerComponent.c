 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class TurretControllerComponent: CompartmentControllerComponent
 {
     proto external BaseCompartmentSlot GetCompartmentSlot();
     proto external bool GetCanAimOnlyInADS();
     proto external bool IsFreeLookEnabled();
     proto external bool IsWeaponADS();
     proto external ETurretReloadState GetReloadingState();
     proto external float GetADSTime();
     proto external BaseSightsComponent GetCurrentSights();
     proto external bool GetCurrentSightsADS();
     proto external void SetCurrentSightsADS(bool on);
     proto bool GetCurrentSightsCameraTransform(out vector outWorldMatrix[4], out float fov);
     proto bool GetCurrentSightsCameraLocalTransform(out vector outLocalMatrix[4], out float fov);
     proto external bool AssembleTurret();
     proto external bool DisassembleTurret();
     proto external TurretComponent GetTurretComponent();
     proto external BaseWeaponManagerComponent GetWeaponManager();
     proto external InventoryStorageManagerComponent GetInventoryManager();
     proto external bool SelectWeapon(IEntity user, BaseWeaponComponent newWeapon);
     // Returns the reload duration in seconds.
     proto external float GetReloadDuration();
     // Returns the current reloading time, it goes from reload duration to 0.
     proto external float GetReloadTime();
     // Instantly reloads weapon with magazine
     proto external bool DoReloadWeaponWith(IEntity ammunitionEntity);
     proto external void SetFireWeaponWanted(bool val);
     proto external void SetAimingAngles(float yaw, float pitch);
 }
