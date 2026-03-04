 class SCR_EntityHelper
 {
     static Managed FindComponent(notnull IEntity entity, typename componentType, SCR_EComponentFinderQueryFlags queryFlags = SCR_EComponentFinderQueryFlags.ENTITY | SCR_EComponentFinderQueryFlags.SLOTS)
     {
         Managed foundComponent;

         //~ Find on entity itself
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.ENTITY))
         {
             foundComponent = entity.FindComponent(componentType);
             if (foundComponent)
                 return foundComponent;
         }

         //~ Find on slotted entities
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.SLOTS))
         {
             SlotManagerComponent slotManager = SlotManagerComponent.Cast(entity.FindComponent(SlotManagerComponent));
             if (slotManager)
             {
                 array<EntitySlotInfo> slotInfos = {};
                 slotManager.GetSlotInfos(slotInfos);
                 IEntity slotEntity;

                 foreach (EntitySlotInfo slotInfo : slotInfos)
                 {
                     slotEntity = slotInfo.GetAttachedEntity();
                     if (!slotEntity)
                         continue;

                     foundComponent = slotEntity.FindComponent(componentType);
                     if (foundComponent)
                         return foundComponent;
                 }
             }
         }

         //~ Find on children
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.CHILDREN))
         {
             IEntity child = entity.GetChildren();

             while (child)
             {
                 foundComponent = child.FindComponent(componentType);
                 if (foundComponent)
                     return foundComponent;

                 child = child.GetSibling();
             }
         }

         IEntity parent;

         //~ Find in parent
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.PARENT))
         {
             parent = entity.GetParent();

             if (parent)
             {
                 foundComponent = parent.FindComponent(componentType);
                 if (foundComponent)
                     return foundComponent;
             }
         }

         //~ Find on slotted entities of parent
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.PARENT_SLOTS))
         {
             if (!parent)
                 parent = entity.GetParent();

             if (parent)
             {
                 foundComponent = SCR_EntityHelper.FindComponent(parent, componentType, SCR_EComponentFinderQueryFlags.SLOTS);
                 if (foundComponent)
                     return foundComponent;
             }
         }

         IEntity rootParent;

         //~ Find in root parent
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.PARENT))
         {
             rootParent = entity.GetRootParent();

             if (rootParent)
             {
                 foundComponent = rootParent.FindComponent(componentType);
                 if (foundComponent)
                     return foundComponent;
             }
         }

         //~ Find on slotted entities of root parent
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.PARENT_SLOTS))
         {
             if (!rootParent)
                 rootParent = entity.GetRootParent();

             if (rootParent)
             {
                 foundComponent = SCR_EntityHelper.FindComponent(rootParent, componentType, SCR_EComponentFinderQueryFlags.SLOTS);
                 if (foundComponent)
                     return foundComponent;
             }
         }

         //~ Find in siblings
         if (SCR_Enum.HasFlag(queryFlags, SCR_EComponentFinderQueryFlags.SIBLINGS))
         {
             if (!parent)
                 parent = entity.GetParent();

             if (parent)
             {
                 //~ Get siblings from parent
                 IEntity child = parent.GetChildren();

                 while (child)
                 {
                     //~ Ignore self
                     if (child == entity)
                     {
                         child = child.GetSibling();
                         continue;
                     }

                     foundComponent = child.FindComponent(componentType);
                     if (foundComponent)
                         return foundComponent;

                     child = child.GetSibling();
                 }
             }
         }

         //~ Not found
         return null;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static int GetChildrenCount(IEntity parent, bool recursive = false)
     {
         if (!parent)
             return 0;

         int num = 0;
         IEntity child = parent.GetChildren();
         while (child)
         {
             num++;
             if (recursive)
                 num += GetChildrenCount(child);
             child = child.GetSibling();
         }

         return num;
     }

     //------------------------------------------------------------------------------------------------
  static void DeleteEntityAndChildren(IEntity entity)
     {
         if (entity)
             RplComponent.DeleteRplEntity(entity, false);
     }

     //------------------------------------------------------------------------------------------------
  static vector GetEntitySize(notnull IEntity entity)
     {
         vector entMins, entMaxs;
         entity.GetBounds(entMins, entMaxs);

         return entMaxs - entMins;
     }

     //------------------------------------------------------------------------------------------------
  static vector GetEntityCenterWorld(notnull IEntity entity)
     {
         vector entMins, entMaxs;
         entity.GetBounds(entMins, entMaxs);
         return entity.CoordToParent((entMaxs + entMins) * 0.5);
     }

     //------------------------------------------------------------------------------------------------
  static float GetEntityRadius(notnull IEntity entity)
     {
         return GetEntitySize(entity).Length() * 0.5;
     }

     //------------------------------------------------------------------------------------------------
  static void GetHierarchyEntityList(notnull IEntity entity, notnull inout array<IEntity> output)
     {
         IEntity child = entity.GetChildren();
         while (child)
         {
             GetHierarchyEntityList(child, output);
             output.Insert(child);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void SnapToGround(notnull IEntity entity, array<IEntity> excludeArray = null, float maxLength = 10, vector startOffset = "0 0 0", bool onlyStatic = false)
     {
         vector origin = entity.GetOrigin();

         // Trace against terrain and entities to detect nearest ground
         TraceParam param = new TraceParam();
         param.Start = origin + startOffset;
         param.End = origin - vector.Up * maxLength;
         param.Flags = TraceFlags.WORLD | TraceFlags.ENTS;

         if (excludeArray)
         {
             excludeArray.Insert(entity);
             param.ExcludeArray = excludeArray;
         }
         else
         {
             param.Exclude = entity;
         }

         param.LayerMask = EPhysicsLayerPresets.Projectile;
         BaseWorld world = entity.GetWorld();
         float traceDistance;

         if (onlyStatic)
             traceDistance = world.TraceMove(param, OnlyStaticCallback);
         else
             traceDistance = world.TraceMove(param, null);

         if (float.AlmostEqual(traceDistance, 1.0))
             return;

         entity.SetOrigin(traceDistance * (param.End - param.Start) + param.Start);
     }

     //------------------------------------------------------------------------------------------------
  static void OrientUpToVector(vector newUp, inout vector mat[4])
     {
         vector origin = mat[3];
         vector perpend = newUp.Perpend();
         Math3D.DirectionAndUpMatrix(perpend, newUp, mat);

         vector basis[4];
         Math3D.AnglesToMatrix(Vector(-perpend.VectorToAngles()[0], 0, 0), basis);
         Math3D.MatrixMultiply3(mat, basis, mat);
         mat[3] = origin;
     }

     //------------------------------------------------------------------------------------------------
  // used by SnapToGround() which is unused
     protected static bool OnlyStaticCallback(notnull IEntity entity)
     {
         Physics physics = entity.GetPhysics();
         if (physics && physics.IsDynamic())
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static IEntity GetMainParent(IEntity entity, bool self = false)
     {
         if (!entity)
             return null;

         IEntity parent = entity.GetRootParent();
         if (parent != entity)
             return parent;

         // root element
         if (self)
             return entity;
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  static IEntity GetPlayer()
     {
         return EntityUtils.GetPlayer();
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool IsPlayer(IEntity entity)
     {
         return entity && entity == EntityUtils.GetPlayer();
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool IsAPlayer(IEntity entity)
     {
         return entity && EntityUtils.IsPlayer(entity);
     }

     //------------------------------------------------------------------------------------------------
  static void SetHierarchyTransform(notnull IEntity entity, vector newTransform[4])
     {
         vector oldTransform[4];
         entity.GetTransform(oldTransform);
         entity.SetTransform(newTransform);

         IEntity child = entity.GetChildren();
         while (child)
         {
             SetHierarchyChildTransform(child, oldTransform, newTransform, true);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // used by SetHierarchyTransform
     protected static void SetHierarchyChildTransform(notnull IEntity entity, vector oldTransform[4], vector newTransform[4], bool recursive = true)
     {
         Physics entPhys = entity.GetPhysics();
         if (entPhys)
         {
             if (entPhys.IsDynamic())
             {
                 vector mat[4];
                 entity.GetTransform(mat);

                 vector diffMat[4];
                 Math3D.MatrixInvMultiply4(oldTransform, mat, diffMat);
                 Math3D.MatrixMultiply4(newTransform, diffMat, mat);

                 entity.SetTransform(mat);
             }
         }

         IEntity child = entity.GetChildren();
         while (child)
         {
             SetHierarchyChildTransform(child, oldTransform, newTransform, recursive);
             child = child.GetSibling();
         }
     }
 }

 class SCR_EntityHelperT<Class T>
 {
     //------------------------------------------------------------------------------------------------
  static T GetEntityInHierarchy(notnull IEntity parent)
     {
         IEntity child = parent.GetChildren();
         while (child)
         {
             if (T.Cast(child))
                 return T.Cast(child);

             child = child.GetSibling();
         }

         return null;
     }
 }

 //------------------------------------------------------------------------------------------------
 enum SCR_EComponentFinderQueryFlags
 {
     ENTITY =                1 << 0,
     SLOTS =                 1 << 1,
     CHILDREN =              1 << 2,
     PARENT =                1 << 3,
     PARENT_SLOTS =          1 << 4,
     ROOT_PARENT =           1 << 5,
     ROOT_PARENT_SLOTS =     1 << 6,
     SIBLINGS =              1 << 7,
 };