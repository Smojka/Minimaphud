 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseSightsComponentClass: GameComponentClass
 {
 }

 class BaseSightsComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external void ForceSightsZeroValue(vector offset, vector angles, vector turnOffset);
     proto external void ForceSightsZeroValueEnablel(bool bOnOff);
     proto external bool IsSightADSActive();
     proto external float GetADSActivationPercentage();
     proto external float GetADSDeactivationPercentage();
     proto external bool AreSightsValid(bool front = true, bool rear = true);
     proto external vector GetSightsRearPosition(bool localSpace = false);
     proto external vector GetSightsFrontPosition(bool localSpace = false);
     proto external vector GetSightsDirection(bool localSpace = false, bool normalize = true);
     proto external vector GetSightsDirectionUntransformed(bool localSpace = false, bool normalize = true);
     proto external vector GetSightsOffset();
     proto external bool GetSightsTransform(out vector transform[4], bool localSpace = false);
     proto external float GetFOV();
     proto external vector GetCurrentSightsRange();
     proto external SightsFOVInfo GetFOVInfo();
     proto external float GetCameraRecoilAmount();
     proto external bool GetSightsPriority();
     proto external bool GetSightsSkipSwitch();
     proto external void SetSightsVisibility(float visFactor);

     // callbacks

     event protected bool WB_GetZeroingData(IEntity owner, BaseSightsComponent sights, float weaponAngle, out vector offset, out vector angles);
 }
