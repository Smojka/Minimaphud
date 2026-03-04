 [BaseContainerProps()]
 class SCR_ResourceContainerQueueBase : Managed
 {
     static const int FIRST_CONTAINER_INDEX      = 0;
     static const int INVALID_CONTAINER_INDEX    = -1;
     static const int INVALID_CONTAINER_COUNT    = -1;

     protected SCR_ResourceComponent m_ResourceComponent;
     protected ref array<SCR_ResourceContainer> m_aRegisteredContainers = new array<SCR_ResourceContainer>();
     protected float m_fAggregatedResourceValue;
     protected float m_fAggregatedMaxResourceValue;

     //------------------------------------------------------------------------------------------------
  float GetAggregatedResourceValue()
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (m_fAggregatedResourceValue < SCR_ResourceActor.RESOURCES_LOWER_LIMIT)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_fAggregatedResourceValue is lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.", "The current aggregated resources can not be lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.");

             return SCR_ResourceActor.RESOURCES_LOWER_LIMIT;
         }

         return m_fAggregatedResourceValue;
     #else
         return m_fAggregatedResourceValue;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  float GetAggregatedMaxResourceValue()
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (m_fAggregatedMaxResourceValue < SCR_ResourceActor.RESOURCES_LOWER_LIMIT)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_fAggregatedMaxResourceValue is lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.", "The current maximum aggregated resources can not be lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.");

             return SCR_ResourceActor.RESOURCES_LOWER_LIMIT;
         }

         return m_fAggregatedMaxResourceValue;
     #else
         return m_fAggregatedMaxResourceValue;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceContainer GetFirstContainer()
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return null;
         }

         if (m_aRegisteredContainers.IsEmpty())
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is empty.", "The method should not be called if the array of registered containers is empty.");

             return null;
         }

         SCR_ResourceContainer container = m_aRegisteredContainers[SCR_ResourceContainerQueueBase.FIRST_CONTAINER_INDEX];

         if (!container)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::GetFirstContainer found null instead of a resource container.", "There should never be null values in the registered containers array.");

             return null;
         }

         return container;
     #else
         return m_aRegisteredContainers[SCR_ResourceContainerQueueBase.FIRST_CONTAINER_INDEX];
     #endif
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceContainer GetContainerAt(int index)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return null;
         }

         if (m_aRegisteredContainers.IsEmpty())
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is empty.", "The method should not be called if the array of registered containers is empty.");

             return null;
         }

         if (!m_aRegisteredContainers.IsIndexValid(index))
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers has no element at the provided index.", "The method should not be called if the array of registered containers does not contain an element at the provided index.");

             return null;
         }

         SCR_ResourceContainer container = m_aRegisteredContainers[index];

         if (!container)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::GetContainerAt found null instead of a resource container.", "There should never be null values in the registered containers array.");

             return null;
         }

         return container;
     #else
         return m_aRegisteredContainers[index];
     #endif
     }

     //------------------------------------------------------------------------------------------------
  int GetContainerCount()
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return SCR_ResourceContainerQueueBase.INVALID_CONTAINER_COUNT;
         }

         return m_aRegisteredContainers.Count();
     #else
         return m_aRegisteredContainers.Count();
     #endif
     }

     //------------------------------------------------------------------------------------------------
  int RegisterContainer(notnull SCR_ResourceContainer container)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG

         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return SCR_ResourceContainerQueueBase.INVALID_CONTAINER_INDEX;
         }

         if (m_aRegisteredContainers.Contains(container))
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::RegisterContainer tried to register an already registered resource container.", "A resource container should not be registered more than once.");

             return SCR_ResourceContainerQueueBase.INVALID_CONTAINER_INDEX;
         }

         return m_aRegisteredContainers.Insert(container);
     #else
         return m_aRegisteredContainers.Insert(container);
     #endif
     }

     //------------------------------------------------------------------------------------------------
  int FindContainer(notnull SCR_ResourceContainer container)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return SCR_ResourceContainerQueueBase.INVALID_CONTAINER_INDEX;
         }

         return m_aRegisteredContainers.Find(container);
     #else
         return m_aRegisteredContainers.Find(container);
     #endif
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceContainer PopFirstContainer()
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return null;
         }

         if (m_aRegisteredContainers.IsEmpty())
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is empty.", "The method should not be called if the array of registered containers is empty.");

             return null;
         }

         SCR_ResourceContainer container = m_aRegisteredContainers[SCR_ResourceContainerQueueBase.FIRST_CONTAINER_INDEX];

         m_aRegisteredContainers.RemoveOrdered(SCR_ResourceContainerQueueBase.FIRST_CONTAINER_INDEX);

         return container;
     #else
         SCR_ResourceContainer container = m_aRegisteredContainers[SCR_ResourceContainerQueueBase.FIRST_CONTAINER_INDEX];

         m_aRegisteredContainers.RemoveOrdered(SCR_ResourceContainerQueueBase.FIRST_CONTAINER_INDEX);

         return container;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  SCR_ResourceContainer PopContainerAt(int index)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return null;
         }

         if (m_aRegisteredContainers.IsEmpty())
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is empty.", "The method should not be called if the array of registered containers is empty.");

             return null;
         }

         if (!m_aRegisteredContainers.IsIndexValid(index))
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers has no element at the provided index.", "The method should not be called if the array of registered containers does not contain an element at the provided index.");

             return null;
         }

         SCR_ResourceContainer container = m_aRegisteredContainers[index];

         if (!container)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::PopContainerAt found null instead of a resource container.", "There should never be null values in the registered containers array.");

             return null;
         }

         m_aRegisteredContainers.RemoveOrdered(index);

         return container;
     #else
         if (!m_aRegisteredContainers.IsIndexValid(index))
             return null;

         SCR_ResourceContainer container = m_aRegisteredContainers[index];

         m_aRegisteredContainers.RemoveOrdered(index);

         return container;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  void SetAggregatedResourceValue(float value)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (value < SCR_ResourceActor.RESOURCES_LOWER_LIMIT)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::SetAggregatedResourceValue The value is lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.", "The current aggregated resources can not be lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.");

             return;
         }

         if (value > m_fAggregatedMaxResourceValue)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::SetAggregatedResourceValue The value is greater than the current maximum aggregated resources value.", "The current aggregated resources can not be greater than than the current maximum aggregated resources value.");

             return;
         }

         m_fAggregatedResourceValue = value;
     #else
         m_fAggregatedResourceValue = value;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  void SetAggregatedMaxResourceValue(float value)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (value < SCR_ResourceActor.RESOURCES_LOWER_LIMIT)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::SetAggregatedMaxResourceValue The value is lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.", "The current maximum aggregated resources can not be lower than SCR_ResourceActor.RESOURCES_LOWER_LIMIT.");

             return;
         }

         if (value < m_fAggregatedResourceValue)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::SetAggregatedMaxResourceValue The value is lower than the current aggregated resources value.", "The current maximum aggregated resources can not be lower than than the current aggregated resources value.");

             return;
         }

         m_fAggregatedMaxResourceValue = value;
     #else
         m_fAggregatedMaxResourceValue = value;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  void IncreaseAggregatedResourceValue(float increment)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (increment < 0)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::IncreaseAggregatedResourceValue The increment is lower than 0.0.", "The increment has to be a positive number.");

             return;
         }

         SetAggregatedResourceValue(m_fAggregatedResourceValue + increment);
     #else
         m_fAggregatedResourceValue += increment;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  void DecreaseAggregatedResourceValue(float decrement)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (decrement < 0)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::DecreaseAggregatedResourceValue The decrement is lower than 0.0.", "The decrement has to be a positive number.");

             return;
         }

         SetAggregatedResourceValue(m_fAggregatedResourceValue - decrement);
     #else
         m_fAggregatedResourceValue -= decrement;
     #endif

     }

     //------------------------------------------------------------------------------------------------
  void IncreaseAggregatedMaxResourceValue(float increment)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (increment < 0)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::IncreaseAggregatedMaxResourceValue The increment is lower than 0.0.", "The increment has to be a positive number.");

             return;
         }

         SetAggregatedMaxResourceValue(m_fAggregatedResourceValue + increment);
     #else
         m_fAggregatedMaxResourceValue += increment;
     #endif
     }

     //------------------------------------------------------------------------------------------------
  void DecreaseAggregatedMaxResourceValue(float decrement)
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         if (decrement < 0)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::DecreaseAggregatedMaxResourceValue The decrement is lower than 0.0.", "The decrement has to be a positive number.");

             return;
         }

         SetAggregatedMaxResourceValue(m_fAggregatedResourceValue - decrement);
     #else
         m_fAggregatedMaxResourceValue -= decrement;
     #endif
     }

     //------------------------------------------------------------------------------------------------
     array<SCR_ResourceContainer> Clear()
     {
     #ifdef SANDBOX_RESOURCES_DEBUG
         array<SCR_ResourceContainer> clearedContainers = {};

         if (!m_aRegisteredContainers)
         {
             Debug.Error2("SCR_ResourceContainerQueueBase::m_aRegisteredContainers is null.", "The array of registered containers has to be present.");

             return clearedContainers;
         }

         for (int index = m_aRegisteredContainers.Count() - 1; index >= 0; --index)
         {
             if (!m_aRegisteredContainers[index])
             {
                 Debug.Error2("SCR_ResourceContainerQueueBase::Clear found null instead of a resource container.", "There should never be null values in the registered containers array.");

                 return clearedContainers;
             }

             clearedContainers.Insert(PopContainerAt(index));
         }

         m_aRegisteredContainers.Clear();
         SetAggregatedResourceValue(SCR_ResourceActor.RESOURCES_LOWER_LIMIT);
         SetAggregatedMaxResourceValue(SCR_ResourceActor.RESOURCES_LOWER_LIMIT);

         return clearedContainers;
     #else
         array<SCR_ResourceContainer> clearedContainers = {};

         for (int index = m_aRegisteredContainers.Count() - 1; index >= 0; --index)
         {
             clearedContainers.Insert(PopContainerAt(index));
         }

         m_aRegisteredContainers.Clear();

         m_fAggregatedResourceValue      = SCR_ResourceActor.RESOURCES_LOWER_LIMIT;
         m_fAggregatedMaxResourceValue   = SCR_ResourceActor.RESOURCES_LOWER_LIMIT;

         return clearedContainers;
     #endif
     }

     //------------------------------------------------------------------------------------------------
     void PerformSorting();

     //------------------------------------------------------------------------------------------------
     void Initialize(notnull SCR_ResourceInteractor interactor)
     {

     }

     //------------------------------------------------------------------------------------------------
     void DebugDraw()
     {

     }

     //------------------------------------------------------------------------------------------------
     float UpdateContainerResourceValue(float currentValue, float previousValue)
     {

     }

     //------------------------------------------------------------------------------------------------
     float UpdateContainerMaxResourceValue(float currentValue, float previousValue)
     {

     }

     //------------------------------------------------------------------------------------------------
  void SCR_ResourceContainerQueueBase()
     {

     }

     //------------------------------------------------------------------------------------------------
  void ~SCR_ResourceContainerQueueBase()
     {
         Clear();
     }
 }

 [BaseContainerProps()]
 class SCR_ResourceContainerQueue<Class ResourceInteractorType> : SCR_ResourceContainerQueueBase
 {
     protected ResourceInteractorType m_Interactor;

     [Attribute(uiwidget: UIWidgets.Object)]
     protected ref array<ref SCR_ResourceStoragePolicyBase<ResourceInteractorType>> m_StoragePolicies;

     //------------------------------------------------------------------------------------------------
  protected int GetStoragePolicyCount()
     {
         return m_StoragePolicies.Count();
     }

     //------------------------------------------------------------------------------------------------
  override int RegisterContainer(notnull SCR_ResourceContainer container)
     {
         int position = SCR_ResourceContainerQueueBase.INVALID_CONTAINER_INDEX;
         float resourceValue;
         bool shouldIncrementOffset;
         SCR_ResourceContainerStorageQueue<ResourceInteractorType> storageQueue;

         EResourceContainerStorageType storageType = container.GetStorageType();

         foreach (SCR_ResourceStoragePolicyBase<ResourceInteractorType> policy: m_StoragePolicies)
         {
             storageQueue = policy.GetStorageQueue();

             if (shouldIncrementOffset)
             {
                 storageQueue.IncrementOffset(1);
                 continue;
             }

             resourceValue = container.GetResourceValue();

             if (!policy.IsStorageTypeValid(storageType))
                 continue;

             m_fAggregatedResourceValue      += resourceValue;
             m_fAggregatedMaxResourceValue   += container.GetMaxResourceValue();

             shouldIncrementOffset = true;
             position = storageQueue.RegisterContainer(container, m_Interactor);
         }

         return position;
     }

     //------------------------------------------------------------------------------------------------
     override SCR_ResourceContainer PopFirstContainer()
     {
         bool shouldDecrementOffset;
         SCR_ResourceContainerStorageQueue<ResourceInteractorType> storageQueue;
         SCR_ResourceContainer container;

         foreach (SCR_ResourceStoragePolicyBase<ResourceInteractorType> policy: m_StoragePolicies)
         {
             storageQueue = policy.GetStorageQueue();

             if (storageQueue.IsEmpty())
                 continue;

             if (shouldDecrementOffset)
             {
                 storageQueue.DecrementOffset(1);
                 continue;
             }

             container = m_aRegisteredContainers[0];
             shouldDecrementOffset = true;

             if (container)
             {
                 m_fAggregatedResourceValue      -= container.GetResourceValue();
                 m_fAggregatedMaxResourceValue   -= container.GetMaxResourceValue();

                 m_Interactor.OnContainerUnregistered(container);
             }

             storageQueue.UnregisterFirstContainer();
         }

         return container;
     }

     //------------------------------------------------------------------------------------------------
     override SCR_ResourceContainer PopContainerAt(int index)
     {
         if (!m_aRegisteredContainers.IsIndexValid(index))
             return null;

         int offsetPosition;
         bool shouldDecrementOffset;
         SCR_ResourceContainerStorageQueue<ResourceInteractorType> storageQueue;
         SCR_ResourceContainer container;

         foreach (SCR_ResourceStoragePolicyBase<ResourceInteractorType> policy: m_StoragePolicies)
         {
             storageQueue = policy.GetStorageQueue();

             if (storageQueue.IsEmpty())
                 continue;

             if (shouldDecrementOffset)
             {
                 storageQueue.DecrementOffset(1);
                 continue;
             }

             offsetPosition = storageQueue.GetOffsetPosition();

             if (index < offsetPosition || index > offsetPosition + storageQueue.GetContainerCount())
                 continue;

             container = m_aRegisteredContainers[index];
             shouldDecrementOffset = true;

             storageQueue.UnregisterContainerAt(index - offsetPosition);

             if (container)
             {
                 m_fAggregatedResourceValue      -= container.GetResourceValue();
                 m_fAggregatedMaxResourceValue   -= container.GetMaxResourceValue();

                 m_Interactor.OnContainerUnregistered(container);
             }
         }

         return container;
     }

     override void PerformSorting()
     {
         array<SCR_ResourceContainer> registeredContainers = Clear();

         foreach (SCR_ResourceContainer container: registeredContainers)
         {
             if (!container)
                 continue;

             m_Interactor.RegisterContainer(container);
         }
     }

     //------------------------------------------------------------------------------------------------
  override array<SCR_ResourceContainer> Clear()
     {
         array<SCR_ResourceContainer> clearedContainers = {};
         clearedContainers.Copy(m_aRegisteredContainers);

         for (int index = m_aRegisteredContainers.Count() - 1; index >= 0; --index)
         {
             m_Interactor.UnregisterContainer(index);
         }

         m_aRegisteredContainers.Clear();

         m_fAggregatedResourceValue      = SCR_ResourceActor.RESOURCES_LOWER_LIMIT;
         m_fAggregatedMaxResourceValue   = SCR_ResourceActor.RESOURCES_LOWER_LIMIT;

         foreach (SCR_ResourceStoragePolicyBase<ResourceInteractorType> policy: m_StoragePolicies)
         {
             policy.ResetStorageQueue();
         }

         return clearedContainers;
     }

     //------------------------------------------------------------------------------------------------
     override void Initialize(notnull SCR_ResourceInteractor interactor)
     {
         SCR_ResourceContainerStorageQueue<ResourceInteractorType> storageQueue;
         m_Interactor = ResourceInteractorType.Cast(interactor);

         foreach (int idx, SCR_ResourceStoragePolicyBase<ResourceInteractorType> policy: m_StoragePolicies)
         {
             storageQueue = policy.GetStorageQueue();

             if (!storageQueue)
             {
                 // TODO: Do proper default behavior or just block progress perhaps?.
                 storageQueue = new SCR_ResourceContainerStorageQueue<ResourceInteractorType>();

                 policy.SetStorageQueue(storageQueue);

                 Print(string.Format("%1:\n\tPolicy at index %2 does not have a defined storage queue.\n\tUsing default storage queue instead.", Type().ToString(), idx), LogLevel.WARNING);
             }

             storageQueue.Initialize(m_aRegisteredContainers);
         }
     }

     //------------------------------------------------------------------------------------------------
     override float UpdateContainerResourceValue(float currentValue, float previousValue)
     {
         float previousAggregatedValue   = m_fAggregatedResourceValue;
         m_fAggregatedResourceValue      += currentValue - previousValue;

         return previousAggregatedValue;
     }

     //------------------------------------------------------------------------------------------------
     override float UpdateContainerMaxResourceValue(float currentValue, float previousValue)
     {
         float previousAggregatedMaxValue    = m_fAggregatedMaxResourceValue;
         m_fAggregatedMaxResourceValue       += currentValue - previousValue;

         return previousAggregatedMaxValue;
     }
 };