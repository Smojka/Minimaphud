 class SCR_BaseContainerTools
 {
     //------------------------------------------------------------------------------------------------
  static Managed CreateInstanceFromPrefab(ResourceName prefab, bool printError = false)
     {
         Resource resource = Resource.Load(prefab);
         if (!resource.IsValid())
         {
             if (printError)
                 Print(string.Format("'SCR_BaseContainerTools', method 'CreateInstanceFromPrefab': failed '%1' at the Resource.Load step!", prefab), LogLevel.ERROR);

             return null;
         }

         BaseContainer baseContainer = resource.GetResource().ToBaseContainer();
         if (!baseContainer)
         {
             if (printError)
                 Print(string.Format("'SCR_BaseContainerTools', method 'CreateInstanceFromPrefab': failed '%1' at the BaseContainer step!", prefab), LogLevel.ERROR);

             return null;
         }

         Managed managed = BaseContainerTools.CreateInstanceFromContainer(baseContainer);
         if (!managed)
         {
             if (printError)
                 Print(string.Format("'SCR_BaseContainerTools', method 'CreateInstanceFromPrefab': failed '%1' create instance step!", prefab), LogLevel.ERROR);

             return null;
         }

         return managed;
     }

     //------------------------------------------------------------------------------------------------
  static string GetContainerClassName(ResourceName prefab)
     {
         return GetContainerClassName(Resource.Load(prefab));
     }

     //------------------------------------------------------------------------------------------------
  static string GetContainerClassName(Resource prefabResource)
     {
         if (!prefabResource || !prefabResource.IsValid())
             return string.Empty;

         BaseResourceObject prefabContainer = prefabResource.GetResource();
         if (!prefabContainer)
             return string.Empty;

         BaseContainer prefabBase = prefabContainer.ToBaseContainer();
         if (!prefabBase)
             return string.Empty;

         return prefabBase.GetClassName();
     }

     //------------------------------------------------------------------------------------------------
  static IEntitySource FindEntitySource(Resource prefabResource)
     {
         if (!prefabResource || !prefabResource.IsValid())
             return null;

         BaseResourceObject prefabBase = prefabResource.GetResource();
         if (!prefabBase)
             return null;

         return prefabBase.ToEntitySource();
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource FindComponentSource(Resource prefabResource, string componentClassName)
     {
         if (!prefabResource || !prefabResource.IsValid())
             return null;

         IEntitySource prefabEntity = FindEntitySource(prefabResource);
         if (!prefabEntity)
             return null;

         return FindComponentSource(prefabEntity, componentClassName);
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource FindComponentSource(Resource prefabResource, typename componentClass)
     {
         if (!prefabResource || !prefabResource.IsValid())
             return null;

         IEntitySource prefabEntity = FindEntitySource(prefabResource);
         if (!prefabEntity)
             return null;

         return FindComponentSource(prefabEntity, componentClass);
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource FindComponentSource(IEntitySource prefabEntity, string componentClassName)
     {
         if (!prefabEntity)
             return null;

         IEntityComponentSource componentSource;
         for (int i, componentsCount = prefabEntity.GetComponentCount(); i < componentsCount; i++)
         {
             componentSource = prefabEntity.GetComponent(i);
             if (componentSource.GetClassName() == componentClassName)
                 return componentSource;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource FindComponentSource(IEntitySource prefabEntity, typename componentClass)
     {
         if (!prefabEntity)
             return null;

         IEntityComponentSource componentSource
         for (int i, componentsCount = prefabEntity.GetComponentCount(); i < componentsCount; i++)
         {
             componentSource = prefabEntity.GetComponent(i);
             if (componentSource.GetClassName().ToType().IsInherited(componentClass))
                 return componentSource;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static int FindComponentSources(Resource prefabResource, notnull array<string> componentClassNames, notnull out array<ref array<IEntityComponentSource>> componentSources)
     {
         int classNamesCount = componentClassNames.Count();
         componentSources.Clear();
         componentSources.Resize(classNamesCount);

         if (!prefabResource || !prefabResource.IsValid())
             return classNamesCount;

         IEntitySource prefabEntity = FindEntitySource(prefabResource);
         if (!prefabEntity)
             return classNamesCount;

         return FindComponentSources(prefabEntity, componentClassNames, componentSources);
     }

     //------------------------------------------------------------------------------------------------
  static int FindComponentSources(IEntitySource prefabEntity, notnull array<string> componentClassNames, notnull out array<ref array<IEntityComponentSource>> componentSources)
     {
         int classNamesCount = componentClassNames.Count();
         componentSources.Clear();
         componentSources.Resize(classNamesCount);

         if (!prefabEntity)
             return classNamesCount;

         IEntityComponentSource componentSource;
         array<IEntityComponentSource> components;
         for (int i, componentsCount = prefabEntity.GetComponentCount(); i < componentsCount; i++)
         {
             componentSource = prefabEntity.GetComponent(i);
             string componentClassName = componentSource.GetClassName();
             for (int j = 0; j < classNamesCount; j++)
             {
                 if (componentClassName == componentClassNames[j])
                 {
                     components = componentSources[j];
                     if (!components)
                     {
                         components = {};
                         componentSources.Set(j, components);
                     }

                     components.Insert(componentSource);
                     break;
                 }
             }
         }

         return classNamesCount;
     }

 //  //------------------------------------------------------------------------------------------------
 //  //! Check if the container contains any changes as opposed to its ancestor
 //  //! \param[in] container Queried container
 //  //! \return True if the container has been modified
 //  static bool IsChanged(BaseContainer container)
 //  {
 //      string varName;
 //      BaseContainerList objectArray;
 //      for (int i = 0, varsCount = container.GetNumVars(); i < varsCount; i++)
 //      {
 //          varName = container.GetVarName(i);
 //          if (container.GetObject(varName))
 //          {
 //              if (IsChanged(container.GetObject(varName)))
 //                  return true;
 //          }
 //          else if (container.GetObjectArray(varName))
 //          {
 //              objectArray = container.GetObjectArray(varName);
 //              for (int a = 0, arrayCount = objectArray.Count(); a < arrayCount; a++)
 //              {
 //                  if (IsChanged(objectArray.Get(a)))
 //                      return true;
 //              }
 //          }
 //          else
 //          {
 //              //PrintFormat("%1: %2: %3", container.GetClassName(), varName, container.IsVariableSetDirectly(varName));
 //              if (container.IsVariableSetDirectly(varName))
 //                  return true;
 //          }
 //      }
 //      return false;
 //  }

     //------------------------------------------------------------------------------------------------
  static BaseContainer GetPrefabContainer(BaseContainer container)
     {
         BaseContainer ancestor = container;
         while (ancestor)
         {
             if (container.GetResourceName().Contains("/"))
                 return ancestor;

             ancestor = ancestor.GetAncestor();
         }

         return container;
     }

     //------------------------------------------------------------------------------------------------
  static ResourceName GetPrefabResourceName(BaseContainer container)
     {
         while (container)
         {
             if (container.GetResourceName().Contains("/"))
                 return container.GetResourceName();

             container = container.GetAncestor();
         }

         return ResourceName.Empty;
     }

     //------------------------------------------------------------------------------------------------
  static array<string> GetPrefabSetValueNames(notnull BaseContainer baseContainer)
     {
         array<string> result = {};
         for (int i, count = baseContainer.GetNumVars(); i < count; i++)
         {
             string varName = baseContainer.GetVarName(i);
             if (baseContainer.IsVariableSetDirectly(varName))
                 result.Insert(varName);
         }

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static BaseContainer GetTopMostAncestor(notnull BaseContainer baseContainer)
     {
         BaseContainer ancestorContainer = baseContainer.GetAncestor();
         if (!ancestorContainer)
             return baseContainer;

         while (ancestorContainer.GetAncestor())
         {
             ancestorContainer = ancestorContainer.GetAncestor();
         }

         return ancestorContainer;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsKindOf(notnull BaseContainer container, ResourceName resourceName)
     {
         if (resourceName.IsEmpty())
             return false;

         while (container)
         {
             if (container.GetResourceName() == resourceName)
                 return true;

             container = container.GetAncestor();
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsKindOf(ResourceName checkedResourceName, ResourceName supposedAncestor)
     {
         if (checkedResourceName.IsEmpty())
             return false;

         Resource resource = Resource.Load(checkedResourceName);
         if (!resource.IsValid())
             return string.Empty;

         BaseResourceObject container = resource.GetResource();
         if (!container)
             return string.Empty;

         BaseContainer prefabBase = container.ToBaseContainer();
         if (!prefabBase)
             return string.Empty;

         return IsKindOf(prefabBase, supposedAncestor);
     }

     //------------------------------------------------------------------------------------------------
  static vector GetWorldCoords(IEntitySource entitySource, vector coords)
     {
         vector coordsEntity;
         while (entitySource)
         {
             if (entitySource.Get("coords", coordsEntity))
                 coords += coordsEntity;

             entitySource = entitySource.GetParent();
         }

         return coords;
     }

     //------------------------------------------------------------------------------------------------
  static vector GetLocalCoords(IEntitySource entitySource, vector coords)
     {
         vector coordsEntity;
         while (entitySource)
         {
             if (entitySource.Get("coords", coordsEntity))
                 coords -= coordsEntity;

             entitySource = entitySource.GetParent();
         }

         return coords;
     }

     //------------------------------------------------------------------------------------------------
  static string GetArrayValue(notnull array<int> values)
     {
         string result;
         foreach (int i, int value : values)
         {
             if (i > 0)
                 result += ",";

             result += value.ToString();
         }

         return result;
     }
 }