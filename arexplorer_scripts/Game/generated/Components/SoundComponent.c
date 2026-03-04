 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class SoundComponentClass: SimpleSoundComponentClass
 {
 }

 class SoundComponent: SimpleSoundComponent
 {
     proto external AudioHandle SoundEvent(string eventName);
     proto external AudioHandle SoundEventTransform(string eventName, vector transf[]);
     proto external AudioHandle SoundEventBone(string eventName, string bone);
     proto external AudioHandle SoundEventOffset(string eventName, vector offset);
 }
