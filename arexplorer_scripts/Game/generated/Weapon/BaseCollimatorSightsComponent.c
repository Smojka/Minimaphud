 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseCollimatorSightsComponentClass: SightsComponentClass
 {
 }

 class BaseCollimatorSightsComponent: SightsComponent
 {
     proto external void SetReticleSize(float angularSize, float reticlePortion);
     proto external float GetReticleAngularSize();
     proto external float GetReticlePortion();
     proto external int GetNumReticles();
     proto external bool IsReticleValid(int index);
     proto external BaseCollimatorReticleInfo GetReticleByIndex(int index);
     proto external int GetCurrentReticleShape();
     proto external void ReticleNextShape();
     proto external void ReticlePreviousShape();
     proto external bool SetReticleShapeByIndex(int iIndex);
     proto external int GetNumColors();
     proto external int IsColorValid(int index);
     proto external BaseCollimatorReticleColor GetColorByIndex(int index);
     proto external int GetCurrentColor();
     proto external void ReticleNextColor();
     proto external void ReticlePreviousColor();
     proto external bool SetReticleColorByIndex(int iIndex);

     // callbacks

     event void UpdateReticlePosition(float u, float v, float uScale, float vScale);
     event void OnPostInit(IEntity owner);
     event void UpdateReticleShapeIndex(int index);
     event void UpdateReticleColor(vector inner, vector glow);
     event void OnSightADSActivate();
     event void OnSightADSDeactivated();
 }
