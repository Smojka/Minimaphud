
 class SCR_ContainerBudgetManager<Class CONTAINER_TYPE, Class ITEM_TYPE>
 {
     protected CONTAINER_TYPE m_Container;
     protected int m_iBudgetSize;
     protected int m_iPivot;

     //------------------------------------------------------------------------------------------------
     CONTAINER_TYPE GetContainer()
     {
         return m_Container;
     }

     //------------------------------------------------------------------------------------------------
  void SetContainer(CONTAINER_TYPE container)
     {
         m_Container = container;
     }

     //------------------------------------------------------------------------------------------------
     array<ITEM_TYPE> ProcessNextBatch()
     {
         if (!m_Container)
             return null;

         ITEM_TYPE item;
         int containerCount = m_Container.Count();
         array<ITEM_TYPE> batch = {};

         batch.Reserve(m_iBudgetSize);

         if (containerCount == 0)
             return batch;

         for (int i = 0; (i < m_iBudgetSize && batch.Count() < m_iBudgetSize); ++i)
         {
             if (m_iPivot >= containerCount)
                 m_iPivot = 0;

             item = m_Container[m_iPivot++];

             batch.Insert(item);
         }

         return batch;
     }

     //------------------------------------------------------------------------------------------------
     void SCR_ContainerBudgetManager(CONTAINER_TYPE container, int budgetSize)
     {
         m_Container = container;
         m_iBudgetSize = budgetSize;
     }
 }