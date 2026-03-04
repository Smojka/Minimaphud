 //~ Incoming liters per minute
 enum EFuelFlowCapacityIn
 {
     MANUAL              =   50,
     VEHICLE_SMALL       =   250,            //standard rate
     VEHICLE_MEDIUM      =   700,            //usually trucks rate
     VEHICLE_BIG         =   1400,           //airplanes
     VEHICLE_HELICOPTER  =   2500,           //helicopter
     FUEL_CARGO          =   4500,           //Fuel Cargo like fuel tanks on truck
 }

 //~ Outgoing liters per minute
 enum EFuelFlowCapacityOut
 {
     MANUAL              =   50,
     VEHICLE_SMALL       =   250,            //standard rate
     VEHICLE_MEDIUM      =   700,            //usually trucks rate
     VEHICLE_BIG         =   1400,           //airplanes
     VEHICLE_HELICOPTER  =   2500,           //helicopter
     FUEL_CARGO          =   4500,           //Fuel Cargo like fuel tanks on truck
 }

 //~ Fuel node type flags
 enum SCR_EFuelNodeTypeFlag
 {
     CAN_RECEIVE_FUEL = 1 << 0,
     CAN_PROVIDE_FUEL = 1 << 1,
     CAN_BE_DRAINED = 1 << 2,

     IS_FUEL_STORAGE = 1 << 3,
 }

 class SCR_FuelNode : BaseFuelNode
 {
     [Attribute( defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Initial fuel level" )]
     protected float m_fInitialFuelTankState;    //tank state of the entity after it's created

     [Attribute(defvalue: EFuelFlowCapacityOut.MANUAL.ToString(), uiwidget: UIWidgets.SearchComboBox, desc: "Maximum Flow Capacity out eg: When draining the fuel tank to refuel another vehicle. Liters per minutes", enums: ParamEnumArray.FromEnum( EFuelFlowCapacityOut ) )]
     protected EFuelFlowCapacityOut m_MaxFlowCapacityOut;

     [Attribute(defvalue: EFuelFlowCapacityIn.VEHICLE_SMALL.ToString(), uiwidget: UIWidgets.SearchComboBox, desc: "Maximum Flow Capacity in eg: When filling the fuel tank. Liters per minutes", enums: ParamEnumArray.FromEnum( EFuelFlowCapacityIn ) )]
     protected EFuelFlowCapacityIn m_MaxFlowCapacityIn;

     [Attribute( defvalue: "20", uiwidget: UIWidgets.CheckBox, desc: "Maximum Leak Speed\n[l/min]" )]
     protected int m_iFuelLeakSpeed;

     [Attribute(SCR_EFuelNodeTypeFlag.CAN_RECEIVE_FUEL.ToString(), desc: "What type of fuel node is it? Can it provide and receive fuel? Maybe the fuel node is part of a fuel storage etc", uiwidget: UIWidgets.Flags, enums: ParamEnumArray.FromEnum(SCR_EFuelNodeTypeFlag))]
     protected SCR_EFuelNodeTypeFlag m_eFuelNodeType;

     [Attribute( defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Fuel tank ID (user action and hitzone)" )]
     protected int m_iFuelTankID;    //for pairing with the user action

     static ref ScriptInvoker s_OnRefuelingFinished = new ScriptInvoker();
     protected ref ScriptInvokerFloat m_OnFuelChanged;

     #ifndef DISABLE_FUEL
     protected const float TIME_STEP               = 1;
     protected float m_fTimeSkip;                                            // Just for not overloading the onFrame each frame
     protected IEntity               m_Owner;                                                // Parent entity
     protected float m_fHealth               = 1;                            // Damage of fuel tank, reduces capacity

     protected SignalsManagerComponent m_SignalManagerComp;

     protected int m_iSignalFuelStateIdx;
     protected int m_iSignalIndexFuelTank;
     protected string m_sSignalFuelState;
     protected const string SIGNAL_FUEL_TANK_PREFIX             = "fuel";
     protected const string SIGNAL_FUEL_TANK_ID                 = "fueltank";

     #ifdef DEBUG_FUELSYSTEM
         private float               m_fTestFuel = 0.0;
         private float               m_fTestWorldTime = 0.0;
         [RplProp(condition: RplCondition.NoOwner, onRplName: "OnTestChanged")]
         private float               m_fDebugTankState       = 0.0;
         private bool                m_bFuelTankHUDAllowed   = true;
     #endif

     //------------------------------------------------------------------------------------------------
  bool CanReceiveFuel()
     {
         return SCR_Enum.HasFlag(m_eFuelNodeType, SCR_EFuelNodeTypeFlag.CAN_RECEIVE_FUEL);
     }

     //------------------------------------------------------------------------------------------------
  bool CanProvideFuel()
     {
         return SCR_Enum.HasFlag(m_eFuelNodeType, SCR_EFuelNodeTypeFlag.CAN_PROVIDE_FUEL);
     }

     //------------------------------------------------------------------------------------------------
  bool HasExactTypeFlags(SCR_EFuelNodeTypeFlag typeFlag)
     {
         return typeFlag == m_eFuelNodeType;
     }

     //------------------------------------------------------------------------------------------------
  bool HasAllTypeFlags(SCR_EFuelNodeTypeFlag typeFlag)
     {
         return SCR_Enum.HasFlag(m_eFuelNodeType, typeFlag);
     }

     //------------------------------------------------------------------------------------------------
  bool HasAnyTypeFlag(SCR_EFuelNodeTypeFlag typeFlag)
     {
         return SCR_Enum.HasPartialFlag(m_eFuelNodeType, typeFlag);
     }

     //------------------------------------------------------------------------------------------------
  int GetFuelTankID()
     {
         return m_iFuelTankID;
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetOwner()
     {
         return m_Owner;
     }

     //------------------------------------------------------------------------------------------------
  void SetHealth(float health)
     {
         m_fHealth = Math.Clamp(health, 0, 1);

         if (m_fHealth == 1)
             return;

         if (ShouldSimulate())
             return;

         SetShouldSimulate(true);
         m_fTimeSkip = 0;
     }

     //------------------------------------------------------------------------------------------------
     override void OnFixedFrame(IEntity owner, float timeSlice)
     {
         // Work 1 time per second at most
         float leakableFuel;
         if (m_fHealth < 1)
             leakableFuel = GetLeakableFuel();

         if (leakableFuel > 0)
             m_fTimeSkip += timeSlice;
         else
             m_fTimeSkip = 0;

         if (m_fTimeSkip < TIME_STEP)
             return;

         // Reduce capacity of fuel tank by its damage
         // The fuel over reliable capacity will leak gradually
         float leak = Math.Min(leakableFuel, ((1 - m_fHealth) * m_iFuelLeakSpeed / 60) * m_fTimeSkip);

         SetFuel(GetFuel() - leak);

         m_fTimeSkip = 0;
     }

     //------------------------------------------------------------------------------------------------
     override protected void OnFuelChanged(float newFuel)
     {
         if (m_SignalManagerComp)
             m_SignalManagerComp.SetSignalValue(m_iSignalFuelStateIdx, newFuel);

         if (m_OnFuelChanged)
             m_OnFuelChanged.Invoke(newFuel);
     }

     //------------------------------------------------------------------------------------------------
  ScriptInvokerFloat GetOnFuelChanged()
     {
         if (!m_OnFuelChanged)
             m_OnFuelChanged = new ScriptInvokerFloat();

         return m_OnFuelChanged;
     }

     //------------------------------------------------------------------------------------------------
  float GetHealth()
     {
         return m_fHealth;
     }

     //------------------------------------------------------------------------------------------------
  float GetLeakableFuel()
     {
         return GetFuel() - GetMaxFuel() * m_fHealth;
     }

     //------------------------------------------------------------------------------------------------
     EFuelFlowCapacityOut GetMaxFlowCapacityOut()
     {
         return m_MaxFlowCapacityOut;
     }

     //------------------------------------------------------------------------------------------------
  EFuelFlowCapacityIn GetMaxFlowCapacityIn()
     {
         return m_MaxFlowCapacityIn;
     }

     //------------------------------------------------------------------------------------------------
     override void OnInit(IEntity owner)
     {
         m_Owner = owner;

         m_sSignalFuelState = SIGNAL_FUEL_TANK_PREFIX + GetFuelTankID().ToString();
         #ifdef DEBUG_FUELSYSTEM
             PrintFormat( "FUELSYSTEM: Inserting %1", this );
         #endif

         m_SignalManagerComp = SignalsManagerComponent.Cast(owner.FindComponent(SignalsManagerComponent));
         if (m_SignalManagerComp)
         {
             m_iSignalFuelStateIdx = m_SignalManagerComp.AddOrFindSignal(m_sSignalFuelState);

             #ifdef DEBUG_FUELSYSTEM
                 PrintFormat( "FUELSYSTEM: signal registered: %1 and set to: %2", m_sSignalFuelState, m_SignalManagerComp.GetSignalValue( m_iSignalFuelStateIdx ) );
             #endif
         }

         SetFuel(m_fInitialFuelTankState);
     }

     #else

     //------------------------------------------------------------------------------------------------
  // Keeping just the declarations of function when the system is disabled
     bool CanReceiveFuel();

     //------------------------------------------------------------------------------------------------
  bool CanProvideFuel();

     //------------------------------------------------------------------------------------------------
  int GetFuelTankID();

     //------------------------------------------------------------------------------------------------
     IEntity GetOwner();

     //------------------------------------------------------------------------------------------------
  void SetHealth(float health);

     //------------------------------------------------------------------------------------------------
     override void OnFixedFrame(IEntity owner, float timeSlice);

     //------------------------------------------------------------------------------------------------
     protected void OnFuelChanged();

     //------------------------------------------------------------------------------------------------
  float GetLeakableFuel();

     //------------------------------------------------------------------------------------------------
     override void OnInit(IEntity owner);
     #endif
 }