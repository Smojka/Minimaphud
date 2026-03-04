 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class GameSignalsManager
 {
     proto external int AddOrFindSignal(string signalName, float value = 0);
     proto external int AddOrFindMPSignal(string signalName, float valueThreshold, float blendSpeed, float value = 0, SignalCompressionFunc compressionFunc = SignalCompressionFunc.None);
     proto external int FindSignal(string signalName);
     proto external void SetSignalValue(int index, float value);
     proto external float GetSignalValue(int index);
 }