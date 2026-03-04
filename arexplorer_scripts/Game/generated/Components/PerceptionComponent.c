 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PerceptionComponentClass: AIComponentEntityClass
 {
 }

 class PerceptionComponent: AIComponentEntity
 {
     proto external int GetSimulateCounter();
     proto external int GetTargetsList(out notnull array<BaseTarget> outTargets, ETargetCategory category);
     proto external int GetTargetCount(ETargetCategory category);
     proto external BaseTarget GetLastSeenTarget(ETargetCategory category, float timeSinceSeenMax);
     proto external BaseTarget GetTargetPerceptionObject(IEntity entityToFind, ETargetCategory targetCategory);
     proto external BaseTarget FindTargetPerceptionObject(IEntity entityToFind);
     proto external BaseTarget GetClosestTarget(ETargetCategory category, float timeSinceSeenMax, float timeSinceDetectedMax);
     proto external void SetPerceptionFactor(float value);
     // Returns the desired update interval of Perception Component. It depends on LOD level.
     proto external float GetUpdateInterval();
     proto external IEntity GetFriendlyInLineOfFire();
     proto external void SetFriendlyFireCheck(bool on);
 }
