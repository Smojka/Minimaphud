 class SCR_ConfigHelper
 {
     //------------------------------------------------------------------------------------------------
  static BaseContainer GetBaseContainerByPath(notnull Resource resource, string subChildPath = "", bool removeEntryPart = false)
     {
         if (!resource.IsValid())
             return null;

         if (subChildPath.IsEmpty())
             return resource.GetResource().ToBaseContainer();

         return GetChildBaseContainerByPath(resource.GetResource().ToBaseContainer(), subChildPath, removeEntryPart);
     }

     //------------------------------------------------------------------------------------------------
  protected static BaseContainer GetChildBaseContainerByPath(BaseContainer container, string subChildPath, bool removeEntryPart = false)
     {
         if (!container)
             return null;

         array<string> paths = {};
         SplitConfigPath(subChildPath, paths, removeEntryPart);
         if (paths.IsEmpty())
             return container;

         return GetChildBaseContainerByPath(container, paths);
     }

     //------------------------------------------------------------------------------------------------
  // to do: split in smaller methods?
     protected static BaseContainer GetChildBaseContainerByPath(notnull BaseContainer container, array<string> paths)
     {
         if (!container)
             return null;

         if (!paths || paths.IsEmpty())
             return container;

         string path;
         string nextPath;
         BaseContainer prevChild = container;
         BaseContainer child = container;
         BaseContainerList containerList;
         for (int i, cntMinus1 = paths.Count() - 1; i <= cntMinus1; i++)
         {
             path = paths[i];
             if (i < cntMinus1)
                 nextPath = paths[i + 1];
             else
                 nextPath = string.Empty;

             prevChild = child;
             child = prevChild.GetObject(path);
             if (child)
                 continue; // child is found, we need to go deeper

             if (nextPath.IsEmpty())
                 return null;

             containerList = prevChild.GetObjectArray(path);
             if (!containerList)
                 return null; // no child, no array of children, get out

             child = GetChildFromList(containerList, nextPath);
             if (!child)
                 return null;

             i++; // moved one step ahead with list â child
         }

         return child;
     }

     //------------------------------------------------------------------------------------------------
  protected static BaseContainer GetChildFromList(notnull BaseContainerList containerList, string childName)
     {
         if (childName.IsEmpty())
             return null;

         string arrayItemName;
         BaseContainer containerListElement;
         for (int i, cnt = containerList.Count(); i < cnt; i++)
         {
             containerListElement = containerList[i];
             arrayItemName = containerListElement.GetName();

             if (arrayItemName.IsEmpty())
                 arrayItemName = containerListElement.GetClassName(); // if .et

             if (arrayItemName.IsEmpty())
                 continue;

             arrayItemName.ToLower();
             childName.ToLower();
             if (arrayItemName == childName)
                 return containerListElement;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static string SplitConfigPath(string input, out array<string> output, bool removeLastPart = false)
     {
         input.Split("/", output, true);

         string lastPart;
         if (output.IsEmpty())
             return lastPart;

         int lastIndex = output.Count() -1;
         lastPart = output[lastIndex].Trim();
         if (removeLastPart)
             output.Remove(lastIndex);

         for (int i, cnt = output.Count(); i < cnt; i++)
         {
             output[i] = output[i].Trim();
         }

         return lastPart;
     }

     //------------------------------------------------------------------------------------------------
  static string GetGUID(ResourceName resourceName, bool removeBrackets = false)
     {
         int guidIndex = resourceName.LastIndexOf("}");
         if (guidIndex < 0)
             return string.Empty;

         if (removeBrackets)
             return resourceName.Substring(1, guidIndex - 1);
         else
             return resourceName.Substring(0, guidIndex + 1);
     }
 }

 class SCR_ConfigHelperT<Class T>
 {
     //------------------------------------------------------------------------------------------------
  static T GetConfigObject(ResourceName configPath)
     {
         if (configPath.IsEmpty())
             return null;

         Resource resource = BaseContainerTools.LoadContainer(configPath);
         if (!resource || !resource.IsValid())
             return null;

         BaseContainer container = resource.GetResource().ToBaseContainer();
         if (!container)
             return null;

         return T.Cast(BaseContainerTools.CreateInstanceFromContainer(container));
     }
 }