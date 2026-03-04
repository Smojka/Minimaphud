 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class VoNComponentClass: GameComponentClass
 {
 }

 class VoNComponent: GameComponent
 {
     proto external bool SetCapture(bool isCapturing);
     proto external void SetCommMethod(ECommMethod type);
     proto external ECommMethod GetCommMethod();
     proto external void SetTransmitRadio(BaseTransceiver transceiver);
     proto external BaseTransceiver GetTransmitRadio();
     proto external void SoundEventPriority(string eventname, array<float> values, int priority, bool ignoreQueue = false);

     // callbacks

     event protected void OnCapture(BaseTransceiver transmitter);
     event protected void OnReceive(int playerId, BaseTransceiver receiver, int frequency, float quality);
 }
