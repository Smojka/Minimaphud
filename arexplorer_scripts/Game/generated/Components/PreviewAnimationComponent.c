 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PreviewAnimationComponentClass: AnimationControllerComponentClass
 {
 }

 class PreviewAnimationComponent: AnimationControllerComponent
 {
     proto external void UpdateFrameStep(IEntity owner, float ts);
     proto external void SetGraphResource(IEntity owner, ResourceName graphResource, ResourceName animInstanceResource, string startNode);
     proto external void UpdateAnimInstance(IEntity owner, ResourceName instanceResource);
     proto external void SetHandsIKPose(IEntity owner, ResourceName ikPoseResource);
     proto external void SetIkState(bool leftHand, bool rightHand);
 }
