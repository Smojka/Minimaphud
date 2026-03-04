 class ParamEnumAddons : array<ref ParamEnum>
 {
     protected static const int CORE_MODULE_COUNT = 2;

     //------------------------------------------------------------------------------------------------
  static ParamEnumArray FromEnum(int titleFormat = 2, int hideCoreModules = 0)
     {
         ParamEnumArray params = new ParamEnumArray();
         array<string> addonGUIDs = {};
         GameProject.GetLoadedAddons(addonGUIDs);

         for (int i, count = addonGUIDs.Count(); i < count; i++)
         {
             string addonGUID = addonGUIDs[i];

             if (hideCoreModules == 1 && GameProject.IsVanillaAddon(addonGUID))
                 continue;

             if (hideCoreModules == 2 && count > CORE_MODULE_COUNT && GameProject.IsVanillaAddon(addonGUID))
                 continue;

             string title;
             switch (titleFormat)
             {
                 case 0: title = GameProject.GetAddonID(addonGUID); break;
                 case 1: title = GameProject.GetAddonTitle(addonGUID); break;
                 default:
                 case 2: title = string.Format("%1 (%2)", GameProject.GetAddonTitle(addonGUID), GameProject.GetAddonID(addonGUID)); break;
             }
             params.Insert(new ParamEnum(title, i.ToString()));
         }

         return params;
     }
 }

 // TODO: rename to SCR_AddonTools?
 class SCR_AddonTool
 {
     protected static const ref array<string> CORE_ADDONS = { "core", "ArmaReforger" };

     //------------------------------------------------------------------------------------------------
  static array<string> GetResourceAddons(ResourceName prefab, bool ignoreCoreAddons = false)
     {
         array<string> addonNames = {};

         if (prefab.IsEmpty())
             return addonNames;

         Resource prefabResource = BaseContainerTools.LoadContainer(prefab);
         if (!prefabResource)
             return addonNames;

         BaseResourceObject configContainer;
         configContainer = prefabResource.GetResource();
         if (!configContainer)
             return addonNames;

         BaseContainer configBase = configContainer.ToBaseContainer();
         if (!configBase)
             return addonNames;

         configBase.GetSourceAddons(addonNames);

         if (ignoreCoreAddons && !addonNames.IsEmpty())
         {
             foreach (string item : CORE_ADDONS)
             {
                 addonNames.RemoveItem(item);
             }
         }

         return addonNames;
     }

     //------------------------------------------------------------------------------------------------
  static string GetResourceLastAddon(ResourceName prefab)
     {
         array<string> addonNames = GetResourceAddons(prefab);
         if (addonNames.IsEmpty())
             return string.Empty;

         return addonNames[addonNames.Count() - 1];
     }

     //------------------------------------------------------------------------------------------------
  static string GetAddonIndex(int index)
     {
         array<string> addons = {};
         GameProject.GetLoadedAddons(addons);

         return GameProject.GetAddonID(addons[index]);
     }

     //------------------------------------------------------------------------------------------------
  static string StripFileSystem(string fileSystemPath)
     {
         int length = fileSystemPath.Length();
         if (fileSystemPath.IsEmpty())
             return fileSystemPath;

         if (!fileSystemPath.StartsWith("$"))
             return fileSystemPath;

         int colonIndex /* ha, ha */ = fileSystemPath.IndexOf(":");
         if (colonIndex < 0)
             return fileSystemPath;

         colonIndex++;
         if (colonIndex == length)
             return string.Empty;

         return fileSystemPath.Substring(colonIndex, length - colonIndex);
     }

     //------------------------------------------------------------------------------------------------
  static string ToFileSystem(string addon)
     {
         return "$" + addon + ":";
     }

 #ifdef WORKBENCH
     //------------------------------------------------------------------------------------------------
  static bool GetAddonAbsolutePath(int addonId, string relativeDirPath, out string result, bool mustExist = true)
     {
         return Workbench.GetAbsolutePath(ToFileSystem(GetAddonIndex(addonId)) + relativeDirPath, result, mustExist);
     }

     //------------------------------------------------------------------------------------------------
  static bool GetAddonAbsolutePath(int addonId, ResourceName directory, out string result, bool mustExist = true)
     {
         return Workbench.GetAbsolutePath(ToFileSystem(GetAddonIndex(addonId)) + directory.GetPath(), result, mustExist);
     }
 #endif
 }