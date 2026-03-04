 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 sealed class ChimeraWorld: World
 {
     proto external TimeAndWeatherManagerEntity GetTimeAndWeatherManager();
     // Returns garbage manager instance or null if none.
     proto external GarbageSystem GetGarbageSystem();
     proto external RadioManagerEntity GetRadioManager();
     proto external ItemPreviewManagerEntity GetItemPreviewManager();
     proto external MusicManager GetMusicManager();
     proto external SoundWorld GetSoundWorld();
     proto external WorldTimestamp GetServerTimestamp();
     proto external WorldTimestamp GetLocalTimestamp();
     proto external void PauseGameTime(bool state);
     proto external bool IsGameTimePaused();
     proto external void RegisterEntityToBeUpdatedWhileGameIsPaused(IEntity entity);
     proto external void UnregisterEntityToBeUpdatedWhileGameIsPaused(IEntity entity);
     proto external int GetObservers(notnull array<vector> observers);
     proto external int GetMPObservers(notnull array<vector> observers);
     static proto ChimeraWorld CastFrom(BaseWorld world);
 }
