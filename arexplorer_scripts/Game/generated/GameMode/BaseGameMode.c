 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseGameModeClass: GenericEntityClass
 {
 }

 class BaseGameMode: GenericEntity
 {
     event protected void OnGameStart();
     /*
     Event is called when player connecting Session hosting current Game Mode where is required authentication verification via. platform services
     AuditSuccess() call specifically mean this verification was successful
     Basically audited player has access to persistency/ etc. related functionality provided by online services.
     \param iPlayerID is index of player in game, equal to the one assigned at PlayerController
     */
     event protected void OnPlayerAuditSuccess( int iPlayerID ) {};
     /*
     Event is called when player connecting Session hosting current Game Mode
     AuditFail() call may be called under two occassion:
     1) verification is required but failed (account is not valid, player is banned, internet issues)
     2) player cannot be verified as authentication is not required or possible - where it may be valid behavior (server online connectivity turned off for example)
     Basically non-audited player cannot access persistency/ etc. related functionality provided by online services.
     \param iPlayerID is index of player in game, equal to the one assigned at PlayerController
     */
     event protected void OnPlayerAuditFail( int iPlayerID) {};
     /*
     Event is called when player connected to Session was kicked and did not reconnected in time
     This mean that slot reservation can be canceled.
     \param iPlayerID is index of player in game, equal to the one assigned at PlayerController
     */
     event protected void OnPlayerAuditTimeouted( int iPlayerID) {};
     /*
     Event is called when player reconnected successfully back to Session after kick
     This mean that slot reservation need to be finished (closed).
     \param iPlayerID is index of player in game, equal to the one assigned at PlayerController
     */
     event protected void OnPlayerAuditRevived( int iPlayerID) {};
     event protected void OnPlayerConnected(int playerId);
     event protected void OnPlayerRegistered(int playerId);
     event protected void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout);
     event protected void OnPlayerSpawned(int playerId, IEntity controlledEntity);
     event protected bool HandlePlayerKilled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer) { return true; };
     event protected void OnPlayerKilled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer);
     event protected void OnControllableSpawned(IEntity entity) {};
     /*
     When a controllable entity is destroyed (damaged enough), this event is raised.
     Controllable entity is such that has BaseControllerComponent and can be
     possessed either by a player, an AI or stay unpossessed.
     \param entity Destroyed entity that raised this event
     \param killerEntity entity of the instigator. If killerEntity is null, you can use instigator.GetInstigatorEntity() if appropiate.
     \param instigator Instigator that destroyed our victim
     */
     event protected void OnControllableDestroyed(IEntity entity, IEntity killerEntity, notnull Instigator instigator) {};
     /*
     Prior to a controllable entity being DELETED, this event is raised.
     Controllable entity is such that has BaseControllerComponent and can be
     possessed either by a player, an AI or stay unpossessed.
     \param entity Entity about to be deleted
     */
     event protected void OnControllableDeleted(IEntity entity) {};
     event protected void OnPlayerRoleChange(int playerId, EPlayerRole roleFlags) {};
     event void OnWorldPostProcess(World world) {};
 }
