 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PlayerControllerClass: GenericControllerClass
 {
 }

 class PlayerController: GenericController
 {
     proto external IEntity GetControlledEntity();
     proto external bool SetControlledEntity(IEntity entity);
     proto external PlayerCamera GetPlayerCamera();
     [Obsolete("Use SCR_RespawnComponent.CanSpawn instead!")]
     proto external bool CanRequestRespawn();
     [Obsolete("Use SCR_RespawnComponent.RequestSpawn instead!")]
     proto external void RequestRespawn();
     proto external ActionManager GetActionManager();
     proto external RespawnComponent GetRespawnComponent();
     proto external HUDManagerComponent GetHUDManagerComponent();
     proto external bool IsChatAllowed();
     proto external bool IsVonAllowed();
     proto external bool HasRole(EPlayerRole role);
     proto external bool SetCharacterCameraRenderActive(bool active);
     proto external int GetPlayerId();
     proto external int GetRplIdentity();
     proto external void SetPlayerBlockedState(int playerId, bool blocked);
     proto external void SetPlayerMutedState(int playerId, bool blocked);
     proto external PermissionState GetPlayerBlockedState(int playerId);
     proto external PermissionState GetPlayerMutedState(int playerId);
     proto external bool CanViewContentCreatedBy(int playerId);

     // callbacks

     event protected void OnInit(IEntity owner);
     /*
     Event raised during ownership changes.

     The process is as following:
         1. Notify the owner that ownership is about to change.
             Event is raised as changing=true and becameOwner=!IsOwner
             This way we can prepare for the ownership change, e.g. if there are any queued inputs we can process them first with all IsOwner checks as they are.
         2. The owner sends and acknowledgment to the server
         3. The server performs the same thing as owner in (1).
         4. The server passes the ownership.
             Event is raised as changing=false, at this point becameOwner always reflects real state.
     */
     event protected void OnOwnershipChanged(bool changing, bool becameOwner);
     event protected void OnControlledEntityChanged(IEntity from, IEntity to);
     event protected void OnDestroyed(notnull Instigator killer);
     event void OnUpdate(float timeSlice);
 }
