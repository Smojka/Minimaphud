 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 sealed class GameAnimationUtils
 {
     private void GameAnimationUtils();
     private void ~GameAnimationUtils();

     static proto AnimationEventID RegisterAnimationEvent(string animationEventString);
     static proto AnimationTagID RegisterAnimationTag(string animationTagString);
     static proto string GetEventString(AnimationEventID eventID);
     static proto string GetTagString(AnimationTagID tagID);
     static proto void AnimateColliders(IEntity entity);
     static proto int FindMeshIndex(IEntity ent, string meshName);
     static proto void ShowMesh(IEntity ent, int meshIndex, bool visible);
 }