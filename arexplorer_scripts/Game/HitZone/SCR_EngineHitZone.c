 class SCR_EngineHitZone : SCR_VehicleHitZone
 {
     [Attribute("25", UIWidgets.Auto, desc: "Minimum engine power scale (% of initial)", params: "0 100 0.1")]
     protected float m_fMinimumEnginePowerScale;

     [Attribute("20", UIWidgets.Auto, desc: "Minimum engine startup chance (% of initial)", params: "0 100 0.1")]
     protected float m_fMinimumEngineStartupChance;

     protected SCR_PowerComponent m_Power;

     //------------------------------------------------------------------------------------------------
     float GetEfficiency()
     {
         EDamageState state = GetDamageState();
         float efficiency = GetDamageStateThreshold(state);

         // Battery influence on engine power output
         if (m_Power && m_Power.ShouldApplyNoPowerPenalty() && !m_Power.HasPower())
             efficiency *= m_Power.GetNoPowerMultiplier();

         return Math.Lerp(0.01 * m_fMinimumEnginePowerScale, 1, efficiency);
     }

     override void OnInit(IEntity pOwnerEntity, GenericComponent pManagerComponent)
     {
         super.OnInit(pOwnerEntity, pManagerComponent);

         Vehicle vehicle = Vehicle.Cast(pOwnerEntity.GetRootParent());
         if (!vehicle)
             return;

         SCR_VehicleDamageManagerComponent damageManager = SCR_VehicleDamageManagerComponent.Cast(vehicle.FindComponent(SCR_VehicleDamageManagerComponent));
         if (damageManager)
             damageManager.RegisterVehicleHitZone(this);

         m_Power = SCR_PowerComponent.Cast(vehicle.FindComponent(SCR_PowerComponent));
         if (m_Power)
             m_Power.GetEventBatteryStateChanged().Insert(UpdateEngineState);
     }

     //------------------------------------------------------------------------------------------------
  override void OnDamage(notnull BaseDamageContext damageContext)
     {
         super.OnDamage(damageContext);

         // Powerful impact should always stop engine
         if (damageContext.damageValue < GetCriticalDamageThreshold()*GetMaxHealth())
             return;

         BaseVehicleNodeComponent node = BaseVehicleNodeComponent.Cast(GetOwner().FindComponent(BaseVehicleNodeComponent));
         if (GetGame().GetIsClientAuthority())
         {
             VehicleControllerComponent controller;
             if (node)
                 controller = VehicleControllerComponent.Cast(node.FindComponent(VehicleControllerComponent));

             if (controller && controller.IsEngineOn())
                 controller.StopEngine(false);
         }
         else
         {
             VehicleControllerComponent_SA controller;
             if (node)
                 controller = VehicleControllerComponent_SA.Cast(node.FindComponent(VehicleControllerComponent_SA));

             if (controller && controller.IsEngineOn())
                 controller.StopEngine(false);
         }
     }

     //------------------------------------------------------------------------------------------------
  override void OnDamageStateChanged()
     {
         super.OnDamageStateChanged();

         UpdateEngineState();
     }

     //------------------------------------------------------------------------------------------------
  void UpdateEngineState()
     {
         Vehicle vehicle = Vehicle.Cast(GetOwner().GetRootParent());
         if (!vehicle)
             return;

         SCR_VehicleDamageManagerComponent damageManager = SCR_VehicleDamageManagerComponent.Cast(vehicle.GetDamageManager());
         if (damageManager)
             damageManager.UpdateVehicleState();

         // Damage and battery influence on starter and engine power output
         // TODO: Delegate this feature to controller/damage manager
         EDamageState state = GetDamageState();
         float health = GetDamageStateThreshold(state);
         float startupChance;
         if (m_Power && !m_Power.HasPower())
             startupChance = 0;
         else
             startupChance = Math.Lerp(m_fMinimumEngineStartupChance, 100, health);

         if (GetGame().GetIsClientAuthority())
         {
             VehicleControllerComponent controller = VehicleControllerComponent.Cast(vehicle.GetVehicleController());
             if (controller)
                 controller.SetEngineStartupChance(startupChance);
         }
         else
         {
             VehicleControllerComponent_SA controller = VehicleControllerComponent_SA.Cast(vehicle.GetVehicleController());
             if (controller)
                 controller.SetEngineStartupChance(startupChance);
         }
     }
 }