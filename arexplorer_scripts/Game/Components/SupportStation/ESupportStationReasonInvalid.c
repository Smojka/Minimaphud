 //~ When no support station found it needs to know the reason.
 //~ It will always use the highest enum reason why the support station was not found
 enum ESupportStationReasonInvalid
 {
     NOT_IN_RANGE = 100,

     DISABLED = 150,

     DESTROYED_STATION = 200,

     INVALID_FACTION = 300,

     NO_SUPPLIES = 400,

     NO_FUEL_TO_GIVE = 500,
     FUEL_CANISTER_EMPTY,
     FUEL_TANK_FULL,

     HEAL_ENTITY_UNDAMAGED = 600,
     HEAL_CHARACTER_IS_BLEEDING,
     HEAL_MAX_HEALABLE_HEALTH_REACHED,
     HEAL_MAX_HEALABLE_HEALTH_REACHED_FIELD,
     HEAL_MAX_HEALABLE_HEALTH_REACHED_EMERGENCY,
     LOAD_CASUALTY_NO_SPACE,
     HEAL_ENTITY_ONFIRE,

     RESUPPLY_ENOUGH_ITEMS = 700,
     RESUPPLY_NOT_IN_STORAGE,
     RESUPPLY_INVENTORY_FULL,
     RESUPPLY_NO_VALID_WEAPON,

     IS_MOVING = 800,

     IN_USE = 9999,                  // !< The User action is being used by another player already
 }