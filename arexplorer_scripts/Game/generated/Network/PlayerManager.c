 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class PlayerManager
 {
     proto external void KickPlayer(int iPlayerId, PlayerManagerKickReason reason, int timeout = 0);
     proto external int GetPlayerCount();
     proto external int GetAllPlayerCount();
     proto external int GetDisconnectedPlayerCount();
     proto external int GetPlayers(out notnull array<int> outPlayers);
     proto external int GetAllPlayers(out notnull array<int> outPlayers);
     proto external int GetDisconnectedPlayers(out notnull array<int> outPlayers);
     proto external string GetPlayerName(int iPlayerId);
     proto external IEntity GetPlayerControlledEntity(int iPlayerId);
     proto external RespawnComponent GetPlayerRespawnComponent(int iPlayerId);
     proto external int GetPlayerIdFromControlledEntity(IEntity controlled);
     proto external int GetPlayerIdFromEntityRplId(int entityRplId);
     proto external PlayerController GetPlayerController(int iPlayerId);
     proto external PlatformKind GetPlatformKind(int iPlayerId);
     proto external bool IsPlayerConnected(int iPlayerId);
     proto external bool HasPlayerRole(int iPlayerId, EPlayerRole role);
     proto external EPlayerRole GetPlayerRoles(int iPlayerId);
     proto external bool GivePlayerRole(int iPlayerId, EPlayerRole role);
     proto external bool ClearPlayerRole(int iPlayerId, EPlayerRole role);
     proto external void ShowUserProfile(int iPlayerId);
     proto external bool IsUserProfileAvailable(int iPlayerId);
     proto external bool ShowMultiplayerActivityInvite();
     proto external bool IsMultiplayerActivityInviteAvailable();
 }
