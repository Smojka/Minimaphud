 class CharacterCameraSet : ScriptedCameraSet
 {
     static const int    CHARACTERCAMERA_DEBUG                   = 0;
     static const int    CHARACTERCAMERA_1ST                     = 1;
     static const int    CHARACTERCAMERA_1ST_BONE_TRANSFORM      = 2;
     static const int    CHARACTERCAMERA_1ST_VEHICLE             = 3;
     static const int    CHARACTERCAMERA_1ST_VEHICLE_TRANSITION  = 4;
     static const int    CHARACTERCAMERA_1ST_TURRET              = 5;
     static const int    CHARACTERCAMERA_1ST_READY               = 6;
     static const int    CHARACTERCAMERA_1ST_FLOAT               = 7;

     static const int    CHARACTERCAMERA_3RD_ERC                 = 10;
     static const int    CHARACTERCAMERA_3RD_ERC_SPR             = 11;
     static const int    CHARACTERCAMERA_3RD_CRO                 = 12;
     static const int    CHARACTERCAMERA_3RD_PRO                 = 13;
     static const int    CHARACTERCAMERA_3RD_JUMP                = 14;
     static const int    CHARACTERCAMERA_3RD_CLIMB               = 15;
     static const int    CHARACTERCAMERA_3RD_VEHICLE             = 16;
     static const int    CHARACTERCAMERA_3RD_TURRET              = 17;
     static const int    CHARACTERCAMERA_3RD_SITTING             = 18;
     static const int    CHARACTERCAMERA_3RD_FLOAT               = 19;
     static const int    CHARACTERCAMERA_3RD_UNCONSCIOUS         = 20;

     static const int    CHARACTERCAMERA_ADS                     = 30;
     static const int    CHARACTERCAMERA_OPTICS                  = 31;
     static const int    CHARACTERCAMERA_BINOCULARS              = 32;

     static const int    CHARACTERCAMERA_ADS_VEHICLE             = 33;

     static const int    CHARACTERCAMERA_1ST_UNCONSCIOUS         = 34;

     static const int DEBUGVIEW_NONE = 0;
     static const int DEBUGVIEW_FACE = 1;
     static const int DEBUGVIEW_WEAPON = 2;
     static const int DEBUGVIEW_LEFT = 3;
     static const int DEBUGVIEW_RIGHT = 4;
     static const int DEBUGVIEW_FRONT = 5;
     static const int DEBUGVIEW_REAR = 6;
     static const int DEBUGVIEW_TOP = 7;
     static const int DEBUGVIEW_BOTTOM = 8;

     // some times for camera changes
     static const float  TIME_CAMERACHANGE_01                = 0.1;
     static const float  TIME_CAMERACHANGE_02                = 0.2;
     static const float  TIME_CAMERACHANGE_03                = 0.3;

     //------------------------------------------------------------------------------------------------
  override void Init()
     {
         //
         // this registers camera ids for camera script class implementation
         // which camera is used at the time is specified by CameraHandlerComponent.CameraSelector which returns id
         //
         RegisterCameraCreator(CHARACTERCAMERA_DEBUG, CharacterCameraDebug);

         //
         // 1PV
         //
         RegisterCameraCreator(CHARACTERCAMERA_1ST, CharacterCamera1stPerson);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_BONE_TRANSFORM, CharacterCamera1stPersonBoneTransform);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_VEHICLE, CharacterCamera1stPersonVehicle);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_VEHICLE_TRANSITION, CharacterCamera1stPersonVehicleTransition);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_TURRET, CharacterCamera1stPersonTurret);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_READY, CharacterCamera1stPersonReady);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_FLOAT, CharacterCamera1stPersonFloat);
         RegisterCameraCreator(CHARACTERCAMERA_1ST_UNCONSCIOUS, CharacterCamera1stPersonUnconscious);

         //
         // 3PV
         //
         RegisterCameraCreator(CHARACTERCAMERA_3RD_ERC, CharacterCamera3rdPersonErc);
         //RegisterCameraCreator(CHARACTERCAMERA_3RD_ERC_SPR, CharacterCamera3rdPersonErcSpr);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_CRO, CharacterCamera3rdPersonCrouch);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_PRO, CharacterCamera3rdPersonProne);
         //RegisterCameraCreator(CHARACTERCAMERA_3RD_JUMP, CharacterCamera3rdPersonJump);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_CLIMB, CharacterCamera3rdPersonClimb);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_VEHICLE, CharacterCamera3rdPersonVehicle);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_TURRET, CharacterCamera3rdPersonTurret);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_SITTING, CharacterCamera3rdPersonSitting);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_FLOAT, CharacterCamera3rdPersonFloat);
         RegisterCameraCreator(CHARACTERCAMERA_3RD_UNCONSCIOUS, CharacterCamera3rdPersonUnconscious);

         //
         // ADS + FOCUS
         //
         RegisterCameraCreator(CHARACTERCAMERA_ADS, CharacterCameraADS);
         //RegisterCameraCreator(CHARACTERCAMERA_OPTICS, CharacterCameraOptics);
         RegisterCameraCreator(CHARACTERCAMERA_BINOCULARS, CharacterCameraBinoculars);
         RegisterCameraCreator(CHARACTERCAMERA_ADS_VEHICLE, CharacterCameraADSVehicle);

         RegisterTransitionTime(CHARACTERCAMERA_3RD_ERC, CHARACTERCAMERA_3RD_CRO, 0.4, true);
         RegisterTransitionTime(CHARACTERCAMERA_3RD_PRO, CHARACTERCAMERA_3RD_CRO, 0.6, true);
         RegisterTransitionTime(CHARACTERCAMERA_1ST_VEHICLE, CHARACTERCAMERA_ADS_VEHICLE, 0.0, true);
         RegisterTransitionTime(CHARACTERCAMERA_3RD_VEHICLE, CHARACTERCAMERA_ADS_VEHICLE, 0.0, true);
     }
 }