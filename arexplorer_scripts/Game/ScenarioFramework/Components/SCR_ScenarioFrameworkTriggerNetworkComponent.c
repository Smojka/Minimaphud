 class SCR_ScenarioFrameworkTriggerNetworkComponentClass : ScriptComponentClass
 {
 }

 class SCR_ScenarioFrameworkTriggerNetworkComponent : ScriptComponent
 {
     //------------------------------------------------------------------------------------------------
  void ReplicateTriggerState(SCR_CharacterTriggerEntity trigger, bool left)
     {
         if (!left)
             Rpc(Rpc_InvokeTriggerUpdated, trigger.GetActivationCountdownTimer(), trigger.GetActivationCountdownTimerTemp(), trigger.GetPlayersCountByFactionInsideTrigger(trigger.GetOwnerFaction()), trigger.GetPlayersCountByFaction(), trigger.GetPlayerActivationNotificationTitle(), trigger.GetTriggerConditionsStatus(), trigger.GetMinimumPlayersNeededPercentage());
         else
             Rpc(Rpc_InvokePlayerLeftTrigger);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     void Rpc_InvokeTriggerUpdated(float activationCountdownTimer, float tempWaitTime, int playersCountByFactionInside, int playersCountByFaction, string playerActivationNotificationTitle, bool triggerConditionsStatus, float minimumPlayersNeededPercentage)
     {
         SCR_CharacterTriggerEntity.s_OnTriggerUpdated.Invoke(activationCountdownTimer, tempWaitTime, playersCountByFactionInside, playersCountByFaction, playerActivationNotificationTitle, triggerConditionsStatus, minimumPlayersNeededPercentage);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     void Rpc_InvokePlayerLeftTrigger()
     {
         SCR_CharacterTriggerEntity.s_OnTriggerUpdatedPlayerNotPresent.Invoke(0);
     }
 }