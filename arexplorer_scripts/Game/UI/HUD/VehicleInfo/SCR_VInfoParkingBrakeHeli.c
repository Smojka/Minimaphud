 class SCR_VInfoParkingBrakeHeli : SCR_BaseVehicleInfo
 {
     protected SCR_HelicopterControllerComponent m_HelicoperController;
     protected SignalsManagerComponent m_SignalsManager;

     protected int m_iSignalIndexSpeed;
     protected const string SIGNAL_SPEED = "airspeed";

     protected int m_iSignalIndexAlt;
     protected const string SIGNAL_ALTITUDE = "altitudeAGL";

     //------------------------------------------------------------------------------------------------
  override EVehicleInfoState GetState()
     {
         if (!m_HelicoperController || !m_SignalsManager)
             return EVehicleInfoState.DISABLED;

         m_bIsBlinking = false;

         float speed = m_SignalsManager.GetSignalValue(m_iSignalIndexSpeed);
         float alt = m_SignalsManager.GetSignalValue(m_iSignalIndexAlt);

         bool bIsHandBrakeOn = m_HelicoperController.GetWheelBrake() || m_HelicoperController.GetPersistentWheelBrake();

         if (!bIsHandBrakeOn)
             return EVehicleInfoState.DISABLED;

         EVehicleInfoState state;

         if (m_HelicoperController.IsEngineOn() && speed > 5 && alt < 5)
         {
             state = EVehicleInfoState.WARNING;
         }
         else
         {
             state = EVehicleInfoState.ENABLED;
         }

         return state;
     }

     //------------------------------------------------------------------------------------------------
  override void DisplayInit(IEntity owner)
     {
         super.DisplayInit(owner);

         m_HelicoperController = SCR_HelicopterControllerComponent.Cast(owner.FindComponent(SCR_HelicopterControllerComponent));

         if (!m_HelicoperController)
             return;

         m_SignalsManager = m_HelicoperController.GetSignalsManager();

         if (!m_SignalsManager)
             return;

         m_iSignalIndexSpeed = m_SignalsManager.FindSignal(SIGNAL_SPEED);
         m_iSignalIndexAlt = m_SignalsManager.FindSignal(SIGNAL_ALTITUDE);
     }
 };