 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterStaminaComponentClass: BaseStaminaComponentClass
 {
 }

 class CharacterStaminaComponent: BaseStaminaComponent
 {
     /*void SCR_OnBreathStateChanged(float breathValue)
     SCR_CALLBACK("protected void OnBreathStateChanged(float breathValue)");*/
     /* EXAMPLE
     class SCR_CharacterStaminaComp : CharacterStaminaComponent
     {
         override event void OnStaminaDrain(float pDrain)
         {
             Print("stamina drained");
         }
     }
     */

     proto external float GetBreathProgress();
     proto external float GetBreathFrequency();
     proto external float GetBreathMagnitude();
     proto external void AddStamina(float pStamina);

     // callbacks

     event void OnStaminaDrain(float pDrain);
 }
