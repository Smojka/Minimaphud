 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class AIWeaponTargetSelector: ScriptAndConfig
 {
     proto external void Init(notnull IEntity owner);
     proto external void SetSelectionProperties(float maxLastSeenDirect, float maxLastSeenIndirect, float maxLastSeen, float minTraceFractionIndirect, float maxDistanceInfantry, float maxDistanceVehicles, float maxTimeSinceEndangered, float maxDistanceDisarmed);
     proto external void SetTargetScoreConstants(EAIUnitType targetUnitType, float offset, float slope);
     proto external bool SelectWeaponAndTarget(notnull array<IEntity> assignedTargets, float assignedTargetsScoreIncrement, float dangerTargetsScoreIncrement, bool useCompartmentWeapons, array<int> weaponTypesWhitelist = null, array<int> weaponTypesBlacklist = null);
     proto BaseTarget GetSelectedTarget();
     proto void GetSelectedWeapon(out BaseWeaponComponent outWeapon, out int outMuzzleId, out BaseMagazineComponent outMagazine);
     proto external EAIUnitType GetUnitTypesCanAttack();
     proto void GetSelectedWeaponProperties(out float outMinDistance, out float outMaxDistance, out bool outDirectDamage);
     proto void GetMostRelevantTargetCantAttack(out BaseTarget target, out float targetScore);
     proto external bool SelectWeaponAgainstUnitType(EAIUnitType targetUnitType, bool useCompartmentWeapons);
     proto external float CalculateTargetScore(BaseTarget target);
     proto external bool HasWeaponOfType(EWeaponType weaponType, bool searchCompartment);
     proto external BaseWeaponComponent FindWeaponOfType(EWeaponType weaponType, bool searchCompartment);
     proto external int GetMagazineCount(typename magazineWellType, bool searchCompartment);
 }
