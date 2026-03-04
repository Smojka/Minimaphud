 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseFuelNode: ScriptAndConfig
 {
     proto external void SetShouldSimulate(bool value);
     proto external bool ShouldSimulate();
     proto external void SetFuel(float value);
     proto external float GetFuel();
     proto external float GetMaxFuel();
     proto external EFuelType GetFuelType();
     proto external PointInfo GetFuelCapPointInfo();

     // callbacks

     event protected void OnInit(IEntity owner);
     event protected void OnFixedFrame(IEntity owner, float timeSlice);
     event protected void OnFuelChanged(float newFuel);
 }
