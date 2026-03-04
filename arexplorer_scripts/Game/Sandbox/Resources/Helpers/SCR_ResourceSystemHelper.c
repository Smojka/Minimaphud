 class SCR_ResourceSystemHelper
 {
     //~ How many decimals are displayed when showing supplies. Use SCR_ResourceSystemHelper.SuppliesToString(supplies) to ignore any decimals ending in 0
     static const int DECIMALS_SUPPLIES = 1;

     //------------------------------------------------------------------------------------------------
  static bool IsGlobalResourceTypeEnabled(EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_BaseGameMode baseGameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
         if (!baseGameMode)
             return true;

         return baseGameMode.IsResourceTypeEnabled(resourceType);
     }

     //------------------------------------------------------------------------------------------------
  static bool GetStoredResources(notnull SCR_ResourceComponent resourceComponent, out float storedRescources, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_ResourceConsumer consumer;
         storedRescources = 0;

         if (resourceComponent.GetConsumer(EResourceGeneratorID.VEHICLE_UNLOAD, resourceType, consumer))
         {
             storedRescources = consumer.GetAggregatedResourceValue();
             return true;
         }


         if (resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT_STORAGE, resourceType, consumer))
         {
             storedRescources = consumer.GetAggregatedResourceValue();
             return true;
         }

         SCR_ResourceGenerator generator;
         if (resourceComponent.GetGenerator(EResourceGeneratorID.VEHICLE_LOAD, resourceType, generator))
         {
             storedRescources = generator.GetAggregatedResourceValue();
             return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static string SuppliesToString(float supplies)
     {
         return SCR_FormatHelper.FloatToStringNoZeroDecimalEndings(supplies, DECIMALS_SUPPLIES);
     }

     //------------------------------------------------------------------------------------------------
  static bool GetStoredAndMaxResources(notnull SCR_ResourceComponent resourceComponent, out float totalResources, out float maxResources, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_ResourceConsumer consumer;

         totalResources = 0;
         maxResources = 0;

         if (resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT_STORAGE, resourceType, consumer))
         {
             maxResources = consumer.GetAggregatedMaxResourceValue();
             totalResources = consumer.GetAggregatedResourceValue();

             if (maxResources > 0)
                 return true;
         }

         if (resourceComponent.GetConsumer(EResourceGeneratorID.VEHICLE_UNLOAD, resourceType, consumer))
         {
             maxResources = consumer.GetAggregatedMaxResourceValue();
             totalResources = consumer.GetAggregatedResourceValue();

             if (maxResources > 0)
                 return true;
         }

         SCR_ResourceGenerator generator;
         if (resourceComponent.GetGenerator(EResourceGeneratorID.VEHICLE_LOAD, resourceType, generator))
         {
             maxResources = generator.GetAggregatedMaxResourceValue();
             totalResources = generator.GetAggregatedResourceValue();

             if (maxResources > 0)
                 return true;
         }

         //~ Reset again
         totalResources = 0;
         maxResources = 0;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool GetAvailableResources(notnull SCR_ResourceComponent resourceComponent, out float availableResources, EResourceGeneratorID resourceID = EResourceGeneratorID.DEFAULT, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_ResourceConsumer consumer;
         availableResources = 0;

         if (resourceComponent.GetConsumer(resourceID, resourceType, consumer))
         {
             availableResources = consumer.GetAggregatedResourceValue();
             return true;
         }


         return false;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ResourceConsumer GetStorageConsumer(notnull SCR_ResourceComponent resourceComponent, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_ResourceConsumer consumer = resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT_STORAGE, resourceType);
         if (consumer)
             return consumer;

         consumer = resourceComponent.GetConsumer(EResourceGeneratorID.VEHICLE_UNLOAD, resourceType);
         if (consumer)
             return consumer;

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ResourceConsumer GetAvailableResourceConsumer(notnull SCR_ResourceComponent resourceComponent, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         return resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT, resourceType);
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ResourceConsumer GetFirstValidConsumer(notnull SCR_ResourceComponent resourceComponent, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_ResourceConsumer consumer = resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT_STORAGE, resourceType);
         if (consumer)
             return consumer;

         consumer = resourceComponent.GetConsumer(EResourceGeneratorID.VEHICLE_UNLOAD, resourceType);
         if (consumer)
             return consumer;

         consumer = resourceComponent.GetConsumer(EResourceGeneratorID.DEFAULT, resourceType);
         if (consumer)
             return consumer;

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_ResourceGenerator GetFirstValidGenerator(notnull SCR_ResourceComponent resourceComponent, EResourceType resourceType = EResourceType.SUPPLIES)
     {
         SCR_ResourceGenerator generator = resourceComponent.GetGenerator(EResourceGeneratorID.DEFAULT_STORAGE, resourceType);
         if (generator)
             return generator;

         generator = resourceComponent.GetGenerator(EResourceGeneratorID.VEHICLE_LOAD, resourceType);
         if (generator)
             return generator;

         generator = resourceComponent.GetGenerator(EResourceGeneratorID.DEFAULT, resourceType);
         if (generator)
             return generator;

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static float RoundRefundSupplyAmount(float refundCost)
     {
         return Math.Ceil(refundCost);
     }
 }