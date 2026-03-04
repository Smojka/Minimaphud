 [EntityEditorProps(category: "GameScripted/Power", description: "This is the power pole entity.", color: "0 255 0 255", visible: false, dynamicBox: true)]
 class SCR_PowerPoleClass : PowerPoleEntityClass
 {
 }

 class SCR_PowerPole : PowerPoleEntity
 {
     [Attribute(desc: "Slots for connecting with other power poles", category: "Power Cable Slots")]
     protected ref array<ref SCR_PowerPoleSlotBase> m_aSlots;

     [Attribute(desc: "Draw debug shapes?", category: "Debug")]
     protected bool m_bDrawDebugShapes;

     protected ref array<ref Shape> m_aDebugShapes = {};
     protected IEntitySource m_Source;

     //------------------------------------------------------------------------------------------------
  int GetSlotsCount(bool sameLine = true)
     {
         return m_aSlots.Count();
     }

     //------------------------------------------------------------------------------------------------
     vector GetSlot(int index, bool sameLine)
     {
         if (!m_aSlots.IsIndexValid(index))
             return vector.Zero;

         return m_aSlots[index].m_vSlotA;
     }

     //------------------------------------------------------------------------------------------------
     vector TryGetSlot(int index, vector otherSlot, bool sameLine)
     {
         if (index < 0)
             return vector.Zero;

         if (index >= m_aSlots.Count())
             return m_aSlots[index % m_aSlots.Count()].m_vSlotA;

         SCR_PowerPoleSlot dualSlot = SCR_PowerPoleSlot.Cast(m_aSlots[index]);
         if (dualSlot)
         {
             if (vector.Distance(otherSlot, CoordToParent(dualSlot.m_vSlotA)) > vector.Distance(otherSlot, CoordToParent(dualSlot.m_vSlotB)))
                 return CoordToParent(dualSlot.m_vSlotB);
             else
                 return CoordToParent(dualSlot.m_vSlotA);
         }

         SCR_PowerPoleSlotSingle singleSlot = SCR_PowerPoleSlotSingle.Cast(m_aSlots[index]);
         if (singleSlot)
             return CoordToParent(singleSlot.m_vSlotA);

         return vector.Zero;
     }

     #ifdef WORKBENCH
     //------------------------------------------------------------------------------------------------
     override bool _WB_OnKeyChanged(BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
     {
         DrawDebugShapes();
         return false;
     }
     #endif

     //------------------------------------------------------------------------------------------------
     protected void DrawDebugShapes()
     {
         m_aDebugShapes.Clear();
         if (!m_bDrawDebugShapes)
             return;

         foreach (SCR_PowerPoleSlotBase slot : m_aSlots)
         {
             slot.DrawDebugShapes(m_aDebugShapes, this);
         }
     }

     //------------------------------------------------------------------------------------------------
     void SCR_PowerPole(IEntitySource src, IEntity parent)
     {
         DrawDebugShapes();

         m_Source = src;
         SetEventMask(EntityEvent.INIT);
     }
 }