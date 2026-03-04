 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterCommandMove: CharacterCommand
 {
     proto external float GetAdjustedStanceAmount();
     proto bool GetCurrentInputAngle(out float pAngle);
     proto external float GetCurrentMovementAngle();
     proto external float GetCurrentMovementSpeed();
     proto external float GetMovementSlopeAngle();
     proto external float GetLateralSlopeAngle();
     proto external bool IsRolling();
     proto external bool IsBlendingOutRoll();
 }
