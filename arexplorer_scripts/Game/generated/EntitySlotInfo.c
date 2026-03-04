 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class EntitySlotInfo: PointInfo
 {
     private ref ScriptInvokerEntity AttachedEntityInvoker;
     private ref ScriptInvokerEntity DetachedEntityInvoker;
     ScriptInvokerEntity GetAttachedEntityInvoker()
     {
         if (!AttachedEntityInvoker)
             AttachedEntityInvoker = new ScriptInvokerEntity();
         return AttachedEntityInvoker;
     }
     ScriptInvokerEntity GetDetachedEntityInvoker()
     {
         if (!DetachedEntityInvoker)
             DetachedEntityInvoker = new ScriptInvokerEntity();
         return DetachedEntityInvoker;
     }
     //@TODO(Leo): find out why it is needed and refactor to cpp, everything is accessible here
  static EntitySlotInfo GetSlotInfo(notnull IEntity entity)
     {
         IEntity parent = entity.GetParent();
         if (!parent)
             return null;

         // Check slot managers
         array<Managed> slotManagers = {};
         parent.FindComponents(SlotManagerComponent, slotManagers);
         foreach (Managed managed : slotManagers)
         {
             SlotManagerComponent slotManager = SlotManagerComponent.Cast(managed);
             array<EntitySlotInfo> managerSlotInfos = {};
             slotManager.GetSlotInfos(managerSlotInfos);
             foreach (EntitySlotInfo slotInfo : managerSlotInfos)
             {
                 if (slotInfo && slotInfo.GetAttachedEntity() == entity)
                     return slotInfo;
             }
         }

         // Check individual slot components
         array<Managed> slots = {};
         parent.FindComponents(BaseSlotComponent, slots);
         foreach (Managed managed : slots)
         {
             BaseSlotComponent slot = BaseSlotComponent.Cast(managed);
             EntitySlotInfo slotInfo = slot.GetSlotInfo();
             if (slotInfo && slotInfo.GetAttachedEntity() == entity)
                 return slotInfo;
         }

         // Check weapon slot components
         array<Managed> weaponSlots = {};
         parent.FindComponents(WeaponSlotComponent, weaponSlots);
         foreach (Managed managed : weaponSlots)
         {
             WeaponSlotComponent slot = WeaponSlotComponent.Cast(managed);
             EntitySlotInfo slotInfo = slot.GetSlotInfo();
             if (slotInfo && slotInfo.GetAttachedEntity() == entity)
                 return slotInfo;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static void GetSlotInfos(notnull IEntity entity, inout notnull array<EntitySlotInfo> slotInfos)
     {
         // Get slot infos of slot managers
         array<Managed> slotManagers = {};
         entity.FindComponents(SlotManagerComponent, slotManagers);
         foreach (Managed managed : slotManagers)
         {
             SlotManagerComponent slotManager = SlotManagerComponent.Cast(managed);
             array<EntitySlotInfo> managerSlotInfos = {};
             slotManager.GetSlotInfos(managerSlotInfos);
             foreach (EntitySlotInfo slotInfo : managerSlotInfos)
             {
                 if (slotInfo)
                     slotInfos.Insert(slotInfo);
             }
         }

         // Get slot info of individual slot components
         array<Managed> slotComponents = {};
         entity.FindComponents(BaseSlotComponent, slotComponents);
         foreach (Managed managed : slotComponents)
         {
             BaseSlotComponent slot = BaseSlotComponent.Cast(managed);
             EntitySlotInfo slotInfo = slot.GetSlotInfo();
             if (slotInfo)
                 slotInfos.Insert(slotInfo);
         }

         // Get slot info of individual weapon slot components
         array<Managed> weaponSlotComponents = {};
         entity.FindComponents(WeaponSlotComponent, weaponSlotComponents);
         foreach (Managed managed : weaponSlotComponents)
         {
             WeaponSlotComponent slot = WeaponSlotComponent.Cast(managed);
             EntitySlotInfo slotInfo = slot.GetSlotInfo();
             if (slotInfo)
                 slotInfos.Insert(slotInfo);
         }
     }

     proto external bool IsEnabled();
     proto external IEntity GetAttachedEntity();
     proto external string GetSourceName();
     proto external void SetAdditiveTransformLS(vector matLS[4]);
     proto external void OverrideTransformLS(vector matLS[4]);
     proto external void DetachEntity(bool physicalChange = true);
     proto external void AttachEntity(IEntity entity);

     // callbacks

     event void OnAttachedEntity(IEntity entity) { if (AttachedEntityInvoker) AttachedEntityInvoker.Invoke(entity); };
     event void OnDetachedEntity(IEntity entity) { if (DetachedEntityInvoker) DetachedEntityInvoker.Invoke(entity); };
 }