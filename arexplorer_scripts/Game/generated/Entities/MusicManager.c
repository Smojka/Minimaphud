 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MusicManagerClass: GenericEntityClass
 {
 }

 class MusicManager: GenericEntity
 {
     proto external void Play(string musicName);
     proto external void Stop(string musicName);
     proto external void MuteCategory(MusicCategory musicCategory, bool mute, bool stopNow);
     proto external void RequestServerMuteCategory(MusicCategory musicCategory, bool mute);
     proto external bool ServerIsCategoryMuted(MusicCategory musicCategory);
     proto external void SetManagerSignals();
     proto external int GetSignalIndex(string name);
     proto external void SetSignalValue(int index, float value);
     proto external void SetSignalValueStr(string signal, float value);
 }
