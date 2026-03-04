 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class DoorComponent: GameComponent
 {
     proto external vector GetDoorPivotPointWS();
     proto external float GetAngleRange();
     proto external bool CanCharacterPass(float characterWidth);
     proto external bool IsOpen();
     proto external bool IsOpening();
     proto external bool IsClosing();
     proto external void SetActionInstigator(IEntity instigator);
     proto external void SetControlValue(float controlValue);
     proto external float GetControlValue();
     proto external float GetDoorState();
     proto external vector GetDoorNormal();
 }
