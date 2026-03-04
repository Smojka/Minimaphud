 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterCommandClimb: CharacterCommand
 {
     proto external int GetState();
     proto external vector GetGrabPointWS();
     proto external vector GetClimbOverStandPointWS();
     /*
     debug draws climb heauristics
     pDebugDrawLevel viz DebugDrawClimb
     */
     static proto bool DoClimbTest(ChimeraCharacter pHuman, CharacterCommandClimbSettings pSettings, CharacterCommandClimbResult pResult);
 }
