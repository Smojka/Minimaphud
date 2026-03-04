 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CommunicationSoundComponentClass: SoundComponentClass
 {
 }

 class CommunicationSoundComponent: SoundComponent
 {
     proto external void GetMetadata(AudioHandle handle, out notnull array<string> metadata);
     proto external void SoundEventPriority(string eventName, int priority, bool ignoreQueue = false);

     // callbacks

     event void OnSoundEventFinished(string eventName, AudioHandle handle, int priority, bool terminated);
     event protected void HandleMetadata(array<string> metadata, int priority, float distance);
 }
