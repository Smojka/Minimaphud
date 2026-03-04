 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PreviewRenderAttributes: BaseItemAttributeData
 {
     proto external void RotateItemCamera(vector deltaRotation, vector limitMin, vector limitMax);
     proto external void ResetDeltaRotation();
     proto external void ZoomCamera(float zoomIncrement, float minFOV = 25.0, float maxFOV = 120.0);
 }
