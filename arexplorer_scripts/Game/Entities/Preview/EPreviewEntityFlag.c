 enum EPreviewEntityFlag
 {
     HORIZONTAL          = 1 << 0,
     ORIENT_CHILDREN     = 1 << 1,
     UNDERWATER          = 1 << 2, //< Entity is being edited under water surface. Valid only for root entity.
     IGNORE_TERRAIN      = 1 << 3, //< Initial terrain under entity will not be evaluated. Valid only for root entity.
     IGNORE_PREFAB       = 1 << 4,
     ONLY_EDITABLE       = 1 << 5,
     REGISTER_EDITABLE   = 1 << 6,
     GEOMETRY            = 1 << 7,
     EDITABLE            = 1 << 8,
 };