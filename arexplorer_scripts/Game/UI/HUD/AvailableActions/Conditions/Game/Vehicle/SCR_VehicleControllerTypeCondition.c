 //------------------------------------------------------------------------------------------------
 enum SCR_EVehicleControllerType
 {
     ANY,
     WHEELED,
     HELICOPTER
 }

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_VehicleControllerTypeCondition : SCR_AvailableActionCondition
 {
     [Attribute(defvalue: SCR_Enum.GetDefault(SCR_EVehicleControllerType.ANY), uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(SCR_EVehicleControllerType))]
     SCR_EVehicleControllerType m_eVehicleControllerType;

     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         BaseControllerComponent controller = data.GetCurrentVehicleController();
         if (!controller)
             return false;

         bool result;
         switch (m_eVehicleControllerType)
         {
             case SCR_EVehicleControllerType.WHEELED:
             {
                 result = CarControllerComponent_SA.Cast(controller) || CarControllerComponent.Cast(controller);
                 break;
             }

             case SCR_EVehicleControllerType.HELICOPTER:
             {
                 result = HelicopterControllerComponent.Cast(controller);
                 break;
             }

             default:
             {
                 result = VehicleControllerComponent_SA.Cast(controller) || VehicleControllerComponent.Cast(controller);
                 break;
             }
         }

         return GetReturnResult(result);
     }
 }