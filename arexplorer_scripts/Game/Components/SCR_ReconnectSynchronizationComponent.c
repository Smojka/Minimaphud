 class SCR_ReconnectSynchronizationComponentClass : ScriptComponentClass
 {
 }

 class SCR_ReconnectSynchronizationComponent : ScriptComponent
 {
     protected const string DIALOG_RECON_RESTORE = "reconnect_restored";
     protected const string DIALOG_RECON_DISCARD = "reconnect_discarded";

     protected ref ScriptInvokerInt m_OnPlayerReconnect = new ScriptInvokerInt();    // param is SCR_EReconnectState

     //------------------------------------------------------------------------------------------------
  ScriptInvokerInt GetOnPlayerReconnect()
     {
         return m_OnPlayerReconnect;
     }

     //------------------------------------------------------------------------------------------------
  void CreateReconnectDialog(SCR_EReconnectState reconState)
     {
         Rpc(RPC_DoSendReconnectState, reconState);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     protected void RPC_DoSendReconnectState(int state)
     {
         m_OnPlayerReconnect.Invoke(state);

         if (state == SCR_EReconnectState.ENTITY_AVAILABLE)
             SCR_CommonDialogs.CreateDialog(DIALOG_RECON_RESTORE);
         else if (state == SCR_EReconnectState.ENTITY_DISCARDED)
             SCR_CommonDialogs.CreateDialog(DIALOG_RECON_DISCARD);
     }
 }