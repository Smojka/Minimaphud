 [ComponentEditorProps(category: "GameScripted/Misc", description: "")]
 class SCR_FuelManagerComponentClass : FuelManagerComponentClass
 {
 }

 class SCR_FuelManagerComponent : FuelManagerComponent
 {
     protected ref ScriptInvokerFloat m_OnFuelChanged;

     //------------------------------------------------------------------------------------------------
  int GetScriptedFuelNodesList(out notnull array<SCR_FuelNode> outScriptedNodes)
     {
         outScriptedNodes.Clear();

         array<BaseFuelNode> fuelNodes = {};
         GetFuelNodesList(fuelNodes);
         SCR_FuelNode scrNode;

         foreach(BaseFuelNode node : fuelNodes)
         {
             scrNode = SCR_FuelNode.Cast(node);
             if (!scrNode)
                 continue;

             outScriptedNodes.Insert(scrNode);
         }

         return outScriptedNodes.Count();
     }

     //------------------------------------------------------------------------------------------------
  int GetScriptedFuelNodesList(out notnull array<SCR_FuelNode> outScriptedNodes, SCR_EFuelNodeTypeFlag hasFuelNodeFlags, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0, SCR_EFuelNodeFlagQueryType queryType = SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES)
     {
         //~ No need to check flags if none are given
         if (hasFuelNodeFlags <= 0 && ignoreIfHasFlags <= 0)
             return GetScriptedFuelNodesList(outScriptedNodes);

         outScriptedNodes.Clear();

         array<BaseFuelNode> fuelNodes = {};
         GetFuelNodesList(fuelNodes);
         SCR_FuelNode scrNode;

         foreach(BaseFuelNode node : fuelNodes)
         {
             scrNode = SCR_FuelNode.Cast(node);
             if (!scrNode)
                 continue;

             if (ignoreIfHasFlags > 0 && scrNode.HasAnyTypeFlag(ignoreIfHasFlags))
                 continue;

             if (hasFuelNodeFlags > 0)
             {
                 bool validNode;

                 switch (queryType)
                 {
                     case SCR_EFuelNodeFlagQueryType.EXACT_TYPE:
                     {
                         validNode = scrNode.HasExactTypeFlags(hasFuelNodeFlags);
                         break;
                     }
                     case SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES:
                     {
                         validNode = scrNode.HasAllTypeFlags(hasFuelNodeFlags);
                         break;
                     }
                     case SCR_EFuelNodeFlagQueryType.ANY_OF_GIVEN_TYPES:
                     {
                         validNode = scrNode.HasAnyTypeFlag(hasFuelNodeFlags);
                         break;
                     }
                 }

                 if (!validNode)
                     continue;
             }

             outScriptedNodes.Insert(scrNode);
         }

         return outScriptedNodes.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool CanBeRefueledScripted(array<int> fuelTanksReceivers = null, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0)
     {
         array<SCR_FuelNode> nodes = {};
         SCR_FuelNode node;
         for (int i = GetScriptedFuelNodesList(nodes, SCR_EFuelNodeTypeFlag.CAN_RECEIVE_FUEL, ignoreIfHasFlags) - 1; i >= 0; i--)
         {
             node = nodes.Get(i);

             if (!node || node.GetFuel() >= node.GetMaxFuel())
                 continue;

             if (fuelTanksReceivers && !fuelTanksReceivers.IsEmpty())
             {
                 if (!fuelTanksReceivers.Contains(node.GetFuelTankID()))
                     continue;
             }
             //~ Passes all tests
             return true;
         }

         //~ Has no nodes to refuel
         return false;
     }

     //~ Todo: move logic from SCR_FuelSupportStationComponent to SCR_FuelManagerComponent
     //------------------------------------------------------------------------------------------------
 //  float TransferFuelWithFlow(float fuelToTransfer, SCR_FuelManagerComponent provider = null, array<int> fuelTanksReceivers = null, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0)
 //  {
 //  }

     //------------------------------------------------------------------------------------------------
  static int GetAllFuelManagers(notnull IEntity entity, out notnull array<SCR_FuelManagerComponent> fuelManagers)
     {
         fuelManagers.Clear();

         SCR_FuelManagerComponent fuelManager = SCR_FuelManagerComponent.Cast(entity.FindComponent(SCR_FuelManagerComponent));
         if (fuelManager)
             fuelManagers.Insert(fuelManager);

         SlotManagerComponent slotManager = SlotManagerComponent.Cast(entity.FindComponent(SlotManagerComponent));
         if (!slotManager)
             return fuelManagers.Count();

         array<EntitySlotInfo> slotInfos = {};
         slotManager.GetSlotInfos(slotInfos);

         IEntity slotEntity;

         foreach (EntitySlotInfo slotInfo: slotInfos)
         {
             slotEntity = slotInfo.GetAttachedEntity();

             if (!slotEntity)
                 continue;

             fuelManager = SCR_FuelManagerComponent.Cast(slotEntity.FindComponent(SCR_FuelManagerComponent));
             if (fuelManager)
                 fuelManagers.Insert(fuelManager);
         }

         return fuelManagers.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool HasFuelToProvide(SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0)
     {
         //~ Get all fuel providers
         array<SCR_FuelNode> nodes = {};
         GetScriptedFuelNodesList(nodes, SCR_EFuelNodeTypeFlag.CAN_PROVIDE_FUEL, ignoreIfHasFlags);

         //~ No nodes
         if (nodes.IsEmpty())
             return false;

         //~ Loop through fuel providers and check if it has any fuel
         foreach(SCR_FuelNode node: nodes)
         {
             //~ Can provide and has fuel.
             if (!float.AlmostEqual(node.GetFuel(), 0))
                 return true;
         }

         //~ No valid fuel nodes found that can provide fuel
         return false;
     }

     //------------------------------------------------------------------------------------------------
  void GetTotalValuesOfFuelNodes(out float totalFuel, out float totalMaxFuel, out float totalFuelPercentage, SCR_EFuelNodeTypeFlag hasFuelNodeFlags = 0, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0, SCR_EFuelNodeFlagQueryType queryType = SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES)
     {
         totalFuel = 0;
         totalMaxFuel = 0;
         totalFuelPercentage = 0;

         //~ No flags are set no need to loop through all nodes
         if (hasFuelNodeFlags <= 0 && ignoreIfHasFlags <= 0)
         {
             totalFuel = GetTotalFuel();
             totalMaxFuel = GetTotalMaxFuel();
         }
         else
         {
             array<BaseFuelNode> fuelNodes = {};
             GetFuelNodesList(fuelNodes);
             SCR_FuelNode scrFuelNode;

             foreach(BaseFuelNode node: fuelNodes)
             {
                 //~ If flags set
                 scrFuelNode = SCR_FuelNode.Cast(node);

                 //~ Get valid fuel node (non-SCR are always valid)
                 if (scrFuelNode)
                 {
                     //~ Has a flag it should not
                     if (ignoreIfHasFlags > 0 && scrFuelNode.HasAnyTypeFlag(ignoreIfHasFlags))
                         continue;

                     //~ Check if node is valid
                     if (hasFuelNodeFlags > 0)
                     {
                         bool validNode;

                         switch (queryType)
                         {
                             case SCR_EFuelNodeFlagQueryType.EXACT_TYPE:
                             {
                                 validNode = scrFuelNode.HasExactTypeFlags(hasFuelNodeFlags);
                                 break;
                             }
                             case SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES:
                             {
                                 validNode = scrFuelNode.HasAllTypeFlags(hasFuelNodeFlags);
                                 break;
                             }
                             case SCR_EFuelNodeFlagQueryType.ANY_OF_GIVEN_TYPES:
                             {
                                 validNode = scrFuelNode.HasAnyTypeFlag(hasFuelNodeFlags);
                                 break;
                             }
                         }

                         if (!validNode)
                             continue;
                     }
                 }

                 totalFuel += node.GetFuel();
                 totalMaxFuel += node.GetMaxFuel();
             }
         }

         //~ Safty
         if (totalMaxFuel <= 0)
         {
             totalFuel = 0;
             totalMaxFuel = 0;
             totalFuelPercentage = 0;
             return;
         }

         totalFuelPercentage = totalFuel / totalMaxFuel;
     }

     //------------------------------------------------------------------------------------------------
  static void GetTotalValuesOfFuelNodesOfFuelManagers(notnull array<SCR_FuelManagerComponent> fuelManagers, out float totalFuel, out float totalMaxFuel, out float totalFuelPercentage, SCR_EFuelNodeTypeFlag hasfuelNodeFlags = 0, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0, SCR_EFuelNodeFlagQueryType queryType = SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES)
     {
         totalFuel = 0;
         totalMaxFuel = 0;
         totalFuelPercentage = 0;

         if (fuelManagers.IsEmpty())
             return;

         float nodeTotalFuel, nodeTotalMaxFuel, nodeTotalFuelPercentage;

         //~ Get values of all fuelmanagers
         foreach (SCR_FuelManagerComponent fuelManager: fuelManagers)
         {
             if (!fuelManager)
                 continue;

             fuelManager.GetTotalValuesOfFuelNodes(nodeTotalFuel, nodeTotalMaxFuel, nodeTotalFuelPercentage, hasfuelNodeFlags, ignoreIfHasFlags, queryType);

             totalFuel += nodeTotalFuel;
             totalMaxFuel += nodeTotalMaxFuel;
         }

         //~ Safty
         if (totalMaxFuel <= 0)
         {
             totalFuel = 0;
             totalMaxFuel = 0;
             totalFuelPercentage = 0;
             return;
         }

         //~ Calculate total percentage
         totalFuelPercentage = totalFuel / totalMaxFuel;
     }

     //------------------------------------------------------------------------------------------------
  void SetTotalFuelPercentage(float percentage, SCR_EFuelNodeTypeFlag hasFuelNodeFlags = 0, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0, SCR_EFuelNodeFlagQueryType queryType = SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES)
     {
         //~ Safty clamp
         percentage = Math.Clamp(percentage, 0 , 1);

         array<BaseFuelNode> fuelNodes = {};
         array<BaseFuelNode> validFuelNodes = {};
         SCR_FuelNode scrFuelNode;

         float totalFuel = 0;
         float totalMaxFuel 0;

         GetFuelNodesList(fuelNodes);

         //~ Get total fuel and max fuel of all non providers
         foreach(BaseFuelNode node: fuelNodes)
         {
             //~ If flags set
             if (hasFuelNodeFlags > 0 || ignoreIfHasFlags > 0)
             {
                 scrFuelNode = SCR_FuelNode.Cast(node);

                 //~ Get valid fuel node (non-SCR are always valid)
                 if (scrFuelNode)
                 {
                     //~ Has a flag it should not
                     if (ignoreIfHasFlags > 0 && scrFuelNode.HasAnyTypeFlag(ignoreIfHasFlags))
                         continue;

                     //~ Check if node is valid
                     if (hasFuelNodeFlags > 0)
                     {
                         bool validNode;

                         switch (queryType)
                         {
                             case SCR_EFuelNodeFlagQueryType.EXACT_TYPE:
                             {
                                 validNode = scrFuelNode.HasExactTypeFlags(hasFuelNodeFlags);
                                 break;
                             }
                             case SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES:
                             {
                                 validNode = scrFuelNode.HasAllTypeFlags(hasFuelNodeFlags);
                                 break;
                             }
                             case SCR_EFuelNodeFlagQueryType.ANY_OF_GIVEN_TYPES:
                             {
                                 validNode = scrFuelNode.HasAnyTypeFlag(hasFuelNodeFlags);
                                 break;
                             }
                         }

                         if (!validNode)
                             continue;
                     }
                 }
             }

             validFuelNodes.Insert(node);
             totalFuel += node.GetFuel();
             totalMaxFuel += node.GetMaxFuel();
         }

         float newFuel = totalMaxFuel * percentage;
         float diff = newFuel - totalFuel;

         foreach(BaseFuelNode node: validFuelNodes)
         {
             float fuel = node.GetFuel();
             float maxFuel = node.GetMaxFuel();

             if (diff > 0)
             {
                 // Distribute remaining capacity
                 if (totalMaxFuel > totalFuel)
                     fuel += diff * (maxFuel - fuel) / (totalMaxFuel - totalFuel);
             }
             else if (diff < 0)
             {
                 // Distribute remaining fuel
                 if (totalFuel > 0)
                     fuel += diff * fuel / totalFuel;
             }

             node.SetFuel(fuel);
         }
     }

     //------------------------------------------------------------------------------------------------
  static void SetTotalFuelPercentageOfFuelManagers(notnull array<SCR_FuelManagerComponent> fuelManagers, float percentage, SCR_EFuelNodeTypeFlag hasFuelNodeFlags = 0, SCR_EFuelNodeTypeFlag ignoreIfHasFlags = 0, SCR_EFuelNodeFlagQueryType queryType = SCR_EFuelNodeFlagQueryType.ALL_GIVEN_TYPES)
     {
         if (fuelManagers.IsEmpty())
             return;

         foreach (SCR_FuelManagerComponent fuelManager: fuelManagers)
         {
             if (!fuelManager)
                 continue;

             fuelManager.SetTotalFuelPercentage(percentage, hasFuelNodeFlags, ignoreIfHasFlags, queryType);
         }
     }

     //------------------------------------------------------------------------------------------------
  ScriptInvokerFloat GetOnFuelChanged()
     {
         if (!m_OnFuelChanged)
         {
             m_OnFuelChanged = new ScriptInvokerFloat();

             array<SCR_FuelNode> fuelNodes = {};

             GetScriptedFuelNodesList(fuelNodes);
             foreach (SCR_FuelNode scrNode : fuelNodes)
             {
                 scrNode.GetOnFuelChanged().Insert(OnFuelNodeAmountChanged);
             }
         }

         return m_OnFuelChanged;
     }

     //------------------------------------------------------------------------------------------------
     protected void OnFuelNodeAmountChanged(float newFuel)
     {
         m_OnFuelChanged.Invoke(GetTotalFuel());
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_FuelManagerComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
         if (SCR_Global.IsEditMode())
             return;

         #ifdef WORKBENCH
         //~ Safty check if the Fuelmanager has any non SCR_FuelNode's in the array
         array<BaseFuelNode> fuelNodes = {};
         GetFuelNodesList(fuelNodes);

         foreach(BaseFuelNode node : fuelNodes)
         {
             if (!node.IsInherited(SCR_FuelNode))
             {
                 Print("'SCR_FuelManagerComponent' has non SCR_FuelNode in the array! This is not supported with many of the SCR_Fuelmanager logics!", LogLevel.WARNING);
                 break;
             }
         }
         #endif
     }
 }

 //~ Fuel node type flags
 enum SCR_EFuelNodeFlagQueryType
 {
     EXACT_TYPE,
     ALL_GIVEN_TYPES,
     ANY_OF_GIVEN_TYPES,
 }