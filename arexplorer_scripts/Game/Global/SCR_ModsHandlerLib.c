 //------------------------------------------------------------------------------------------------
 //------------------------------------------------------------------------------------------------
 class SCR_ModHandlerLib
 {
     //------------------------------------------------------------------------------------------------
  static float GetModListSize(array<Dependency> dependencies)
     {
         // Empty array check
         if (dependencies.IsEmpty())
             return -1;

         // Size count
         float size = 0;

         foreach (Dependency dependency : dependencies)
         {
             size += dependency.TotalFileSize();
         }

         // Result
         return size;
     }

     //------------------------------------------------------------------------------------------------
  static float GetModListSize(array<ref SCR_WorkshopItem> items)
     {
         // Empty array check
         if (items.IsEmpty())
             return -1;

         // Size count
         float size = 0;

         foreach (SCR_WorkshopItem item : items)
         {
             size += item.GetSizeBytes();
         }

         // Result
         return size;
     }

     //------------------------------------------------------------------------------------------------
  static bool AllItemsCached(array<Dependency> aDependencies, out array<Dependency> aDepsToLoad)
     {
         for (int i = 0; i < aDependencies.Count(); i++)
         {
             WorkshopItem item = aDependencies[i].GetCachedItem();

             // Is cashed item missing?
             if(!item)
             {
                 // Items to load batch
                 aDepsToLoad.Insert(aDependencies[i]);
                 continue;
             }
         }

         // Are there dependecies to load
         if (aDepsToLoad.Count() == 0)
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool AllModsUpdated(array<Dependency> dependencies)
     {
         // No dependencies in list
         if (dependencies.Count() == 0)
             return true;

         // Fill items array to check
         array<WorkshopItem> aRoomItems = new array<WorkshopItem>;

         foreach (Dependency dependency : dependencies)
         {
             WorkshopItem item = dependency.GetCachedItem();

             if (item)
                 aRoomItems.Insert(item);
         }

         // Compare items
         foreach (WorkshopItem item : aRoomItems)
         {
             int flags = item.GetStateFlags();

             if (flags & EWorkshopItemState.EWSTATE_OFFLINE)
             {
                 if (flags & EWorkshopItemState.EWSTATE_OUTDATED)
                     return false;
             }
             else
                 return false;
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
     // TODO@wernejak - setup this debug when needed data are ready
     static void SetupDebug()
     {
         // Debug menu
         /*DiagMenu.RegisterMenu(SCR_DebugMenuID.DEBUGUI_ADDONS_INFO, "Enabled addons", "Addons");
         DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_ADDONS_INFO, "", "Show enabled addons list", "Enabled addons", false);*/

         //SCR_AddonManager.GetAllAddons();
     }

     //------------------------------------------------------------------------------------------------
  protected static bool ItemVersionMatch(SCR_WorkshopItem item)
     {
         // Check item
         WorkshopItem wsItem = item.GetWorkshopItem();
         if (!wsItem)
             return false;

         // Dependency check
         Dependency dependency = item.GetDependency();
         if (!dependency)
             return false;

         // Does active version match required?
         return Revision.AreEqual(item.GetCurrentLocalRevision(), dependency.GetRevision());
     }
 };

 //------------------------------------------------------------------------------------------------
 enum ERoomModStates
 {
     MISSING,
     LOCAL_VERSION_MATCH,
     LOCAL_VERSION_MISMATCH,
     CLIENT_REPORTED,
     BANNED
 };