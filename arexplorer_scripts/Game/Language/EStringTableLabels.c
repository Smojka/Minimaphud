
 enum EStringTableLabels
 {
     UI,
     CHARACTERS,
     WEAPONS,
     VEHICLES,
     DIALOGUE,

     //Releasables
     PATCH_0_9_6 = 96,

     EDITOR = 200,
     EDITOR_MODE_EDIT = 201,
     EDITOR_MODE_STRATEGY = 202,
     EDITOR_MODE_PHOTO = 203,
     EDITOR_MODE_ADMIN = 204,
     EDITOR_MODE_SPECTATE = 205,
     EDITOR_ATTRIBUTE = 206,
     EDITOR_CONTEXT_ACTION = 207,
     EDITOR_OVERLAY = 208,
     EDITOR_FILTER = 209,
     EDITOR_TOOLBAR = 210,
     EDITOR_TOOLTIP_DETAIL = 211,
     EDITOR_CONTENT_BROWSER = 212,
     EDITOR_CONTENT_BROWSER_FILTER = 213,

     EDITABLE_ENTITY = 300,

     CONTROLS_HINTS = 400, // Available actions display aka controls hints
     USER_ACTIONS = 401, // Interaction handler, action menu
     QUICK_MENU = 402, // Quick menu - gadget & VON selection
     VON = 403, // Onscreen visualization for outgoing & incomming VON transmission
     CHAT = 404, // Chat tags, labels, names
     NOTIFICATION = 405, //Notification
     FIELD_MANUAL = 406,
     HINT = 407,
     VOTING = 408,
     KEYBIND = 409,

     RANKS = 500, // Military ranks

     CAMPAIGN = 600, // Campaign-specific
     CAMPAIGN_TASKS = 601,   // Tasks (objectives)

     ROLES = 700, // Soldier roles
     GROUPS = 800, // Military groups

     ITEMS = 900, // Iventory items
     AMMUNITION = 1000, // All kinds of ammunition
     MAGAZINES = 1100, // Magazines, clips, belts - anything that can be loaded into weapon
     AMMOBOXES = 1150, //Ammoboxes
     GEAR = 1200, // Personal gear - uniforms, backpacks, helmets, vests, load-bearing systems and their individual pouches

     INVENTORY = 1300, // Inventory texts - UI and interactions

     //Units and generic
     COMPASS = 1400,
     COMPASS_SHORT = 1401,
     DATE = 1402,
     VALUE_UNIT = 1403,
     VALUE_UNIT_SHORT = 1404,
     TIME_OF_DAY = 1405,
     WEATHER = 1406,
     BLOODTYPE = 1407,

     //Map
     MAP_SETTLEMENTS = 2000, // Settlement names on map (Cities, town, villages)
     MAP_HYDRONYMS = 2001,
     MAP_AGRONYMS = 2002,
     MAP_ORONYMS = 2003,
     MAP_DRYMONYMS = 2004,
     MAP_URBANONYMS = 2005,
     MAP_TOPONYMS = 2006,

     //Dialog
     DIALOGUE_PROTOCOL = 2100,
     DIALOGUE_CONFLICT = 2101,

     END_SCREEN = 2200,

     // Main menu
     MAIN_MENU = 2300,
     SERVER_BROWSER = 2301,
     WORKSHOP = 2302,
     SERVER_HOSTING = 2303,
 };