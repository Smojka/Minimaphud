 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_EditableVehicleUIInfo: SCR_EditableEntityUIInfo
 {
     [Attribute("1", desc: "If true when placing the entity in editor this will make sure that both crew and passengers are spawned as one group. Else passengers and crew are split in two groups")]
     protected bool m_bEditorPlaceAsOneGroup;

     [Attribute(desc: "Auto filled by plugin. Compartments Types that can be filled with characters within when using the editor to place the vehicle.", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ECompartmentType))]
     protected ref array<ECompartmentType> m_aOccupantFillCompartmentTypes;

     [Attribute("", category: "Editable Entity", desc: "Auto filled by plugin. Contains budget costs of default prefabs for pilot/crew slots")]
     private ref array<ref SCR_EntityBudgetValue> m_aCrewEntityBudgetCost;

     [Attribute("", category: "Editable Entity", desc: "Auto filled by plugin. Contains budget costs of default prefabs for passenger slots")]
     private ref array<ref SCR_EntityBudgetValue> m_aPassengerEntityBudgetCost;

     //------------------------------------------------------------------------------------------------
  bool GetEditorPlaceAsOneGroup()
     {
         return m_bEditorPlaceAsOneGroup;
     }

     //------------------------------------------------------------------------------------------------
  int GetOccupantFillCompartmentTypes(out notnull array<ECompartmentType> compartmentTypes)
     {
         compartmentTypes.Clear();
         compartmentTypes.Copy(m_aOccupantFillCompartmentTypes);

         return compartmentTypes.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool CanFillWithCrew()
     {
         return CanFillWithGivenTypes(SCR_BaseCompartmentManagerComponent.CREW_COMPARTMENT_TYPES);
     }

     //------------------------------------------------------------------------------------------------
  bool CanFillWithPassengers()
     {
         return CanFillWithGivenTypes(SCR_BaseCompartmentManagerComponent.PASSENGER_COMPARTMENT_TYPES);
     }

     //------------------------------------------------------------------------------------------------
  bool CanFillWithGivenTypes(array<ECompartmentType> typesToCheck)
     {
         if (m_aOccupantFillCompartmentTypes.IsEmpty() || !typesToCheck || typesToCheck.IsEmpty())
             return false;

         foreach (ECompartmentType checkType: typesToCheck)
         {
             foreach (ECompartmentType type: m_aOccupantFillCompartmentTypes)
             {
                 if (type == checkType)
                     return true;
             }
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  void GetFillBudgetCostsOfCrew(out notnull array<ref SCR_EntityBudgetValue> budgetCosts)
     {
         budgetCosts.Clear();

         if (!CanFillWithCrew())
             return;

         foreach(SCR_EntityBudgetValue budget: m_aCrewEntityBudgetCost)
             budgetCosts.Insert(budget);
     }

     //------------------------------------------------------------------------------------------------
  void GetFillBudgetCostsOfPassengers(out notnull array<ref SCR_EntityBudgetValue> budgetCosts)
     {
         budgetCosts.Clear();

         if (!CanFillWithPassengers())
             return;

         foreach(SCR_EntityBudgetValue budget: m_aPassengerEntityBudgetCost)
             budgetCosts.Insert(budget);
     }
 }