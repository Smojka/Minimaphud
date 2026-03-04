 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseWeaponManagerComponentClass: GameComponentClass
 {
 }

 class BaseWeaponManagerComponent: GameComponent
 {
     ref ScriptInvoker<BaseWeaponComponent> m_OnWeaponChangeCompleteInvoker = new ScriptInvoker<BaseWeaponComponent>();
     ref ScriptInvoker<BaseWeaponComponent> m_OnWeaponChangeStartedInvoker = new ScriptInvoker<BaseWeaponComponent>();

     proto external IEntity GetOwner();
     proto external void Throw(vector vDirection, float fSpeedScale);
     proto external int GetDefaultWeaponIndex();
     proto external BaseWeaponComponent GetCurrent();
     proto external BaseWeaponComponent GetCurrentWeapon();
     proto external BaseWeaponComponent GetCurrentGrenade();
     proto external WeaponSlotComponent GetCurrentSlot();
     proto external GrenadeSlotComponent GetCurrentGrenadeSlot();
     proto external SightsComponent GetCurrentSights();
     proto external int GetCurrentSightsIndex();
     proto external bool SelectWeapon(BaseWeaponComponent weapon);
     proto external IEntity SetSlotWeapon(WeaponSlotComponent pSlot, IEntity pWeaponEntity);
     proto external void SetVisibleAllWeapons(bool state);
     proto external void SetVisibleCurrentWeapon(bool state);
     proto external int GetWeaponsSlots(out notnull array<WeaponSlotComponent> outSlots);
     proto external int GetWeaponsList(out notnull array<IEntity> outWeapons);
     proto external bool GetCurrentMuzzleTransform(vector outMatrix[4]);
     proto bool GetCurrentSightsTransform(out vector outWorldMatrix[4], out vector outLocalMatrix[4], out float fov);
     proto bool GetCurrentSightsCameraTransform(out vector outLocalMatrix[4], out float fov);

     // callbacks

     event protected void OnWeaponChangeComplete(BaseWeaponComponent newWeaponSlot) { m_OnWeaponChangeCompleteInvoker.Invoke(newWeaponSlot); };
     event protected void OnWeaponChangeStarted(BaseWeaponComponent newWeaponSlot) { m_OnWeaponChangeStartedInvoker.Invoke(newWeaponSlot); };
 }
