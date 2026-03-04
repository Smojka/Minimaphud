
 enum ENotificationColor
 {
     NEUTRAL,
     WARNING,
     POSITIVE,
     NEGATIVE,
     GM,

     //Always keep the first faction related color 1000 or higher! This is checked in code if a faction related color should be used. It checks for FACTION_FRIENDLY_IS_NEGATIVE so always add new colors below
     FACTION_FRIENDLY_IS_NEGATIVE = 1000,
     FACTION_FRIENDLY_IS_POSITIVE = 1001,
     FACTION_ENEMY_IS_NEGATIVE_ONLY = 1002,
     FACTION_FRIENDLY_IS_POSITIVE_ONLY = 1003,
 };