
 enum EEditableEntityFlag
 {
     PLACEABLE           = 1 << 0,
     VIRTUAL             = 1 << 1,
     HAS_FACTION         = 1 << 2,
     LOCAL               = 1 << 3,
     NON_INTERACTIVE     = 1 << 4,
     SLOT                = 1 << 5,
     HORIZONTAL          = 1 << 6,
     STATIC_POSITION     = 1 << 7,
     LAYER               = 1 << 8,
     GAME_HIERARCHY      = 1 << 9,
     ORIENT_CHILDREN     = 1 << 10,
     NON_DELETABLE       = 1 << 11,
     IGNORE_LAYERS       = 1 << 12,
     INDIVIDUAL_CHILDREN = 1 << 13,
     NON_SERIALIZABLE    = 1 << 14,
     HAS_AREA            = 1 << 15,
     DIRTY_HIERARCHY     = 1 << 16,
     LINKED_CHILDREN     = 1 << 17,
     SPAWN_UNFINISHED    = 1 << 18,
     FREE_ROAM_BUILDING_AI   = 1 << 19,
 };