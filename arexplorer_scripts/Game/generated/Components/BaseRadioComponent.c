 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseRadioComponentClass: GameComponentClass
 {
 }

 class BaseRadioComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external int TransceiversCount();
     proto external BaseTransceiver GetTransceiver(int idx);
     proto external void SetEncryptionKey(string key);
     proto external string GetEncryptionKey();
     proto external void SetPower(bool powered);
     proto external bool IsPowered();
 }
