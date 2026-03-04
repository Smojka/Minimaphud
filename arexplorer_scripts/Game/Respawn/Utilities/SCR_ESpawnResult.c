 /*
     This enum represents possible results/responses sent from the authority after a SCR_SpawnRequestComponent
     issues a request to respawn to a SCR_SpawnHandlerComponent.

     When the spawning process is successfull, the return value is SCR_ESpawnResult.OK,
     anything else is a request which has failed and resulted in the player not spawning.
 */
 enum SCR_ESpawnResult
 {
     OK = 0,

     BAD_REQUEST = 1000,

     UNSUPPORTED_SPAWN_METHOD = 2000,

     MISSING_HANDLER,

     SPAWN_NOT_ALLOWED = 3000,

     NOT_ALLOWED_TIMER,
     NOT_ALLOWED_SPAWNPOINT_DISABLED,
     NOT_ALLOWED_SPAWNING_DISABLED,
     NOT_ALLOWED_VEHICLE_FULL,
     NOT_ALLOWED_VEHICLE_MOVING,
     NOT_ALLOWED_RADIO_VEHICLE_SPAWNING_DISABLED,
     NOT_ALLOWED_SPAWNPOINT_OCCUPIED_BY_HOSTILE,
     NOT_ALLOWED_SPAWNING_DISABLED_ENEMIES_NEARBY,
     NOT_ALLOWED_SPAWNPOINT_DISABLED_OUT_OF_RESPAWNS,
     NOT_ALLOWED_NOT_ENOUGH_SUPPLIES,

     INVALID_PREFAB = 5000,

     CANNOT_VALIDATE,

     CANNOT_SPAWN,

     CANNOT_PREPARE,

     CANNOT_POSSES,

     CANNOT_ASSIGN,

     INTERNAL_ERROR = 666666,

     UNKNOWN_ERROR = 999999,
 };