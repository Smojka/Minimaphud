 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_VehicleDrivingAssistanceCondition : SCR_AvailableActionCondition
 {
     [Attribute(defvalue: SCR_Enum.GetDefault(EVehicleDrivingAssistanceMode.FULL), uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EVehicleDrivingAssistanceMode))]
     EVehicleDrivingAssistanceMode m_eDrivingAssistanceMode;

     //------------------------------------------------------------------------------------------------
  override bool IsAvailable(SCR_AvailableActionsConditionData data)
     {
         if (!data)
             return false;

         if(GetGame().GetIsClientAuthority())
         {
             EVehicleDrivingAssistanceMode drivingAssistanceMode = CarControllerComponent.GetDrivingAssistanceMode();
             bool result = m_eDrivingAssistanceMode == drivingAssistanceMode;
             return GetReturnResult(result);
         }
         else
         {
             EVehicleDrivingAssistanceMode drivingAssistanceMode = CarControllerComponent_SA.GetDrivingAssistanceMode();
             bool result = m_eDrivingAssistanceMode == drivingAssistanceMode;
             return GetReturnResult(result);
         }
     }
 };