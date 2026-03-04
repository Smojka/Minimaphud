 class SCR_SortedArray<Class TValue>: Managed
 {
     protected ref array<int> m_aOrders = new array<int>();
     protected ref array<TValue> m_aValues = new array<TValue>();

     TValue Get(int n)
     {
         return m_aValues.Get(n);
     }
     void Set(int n, TValue value)
     {
         m_aValues.Set(n, value);
     }
     void Insert(int order, TValue value)
     {
         int index = Count();
         for (int i = 0; i < index; i++)
         {
             if (order < m_aOrders[i])
             {
                 index = i;
                 break;
             }
         }
         m_aOrders.InsertAt(order, index);
         m_aValues.InsertAt(value, index);
     }
     void Remove(int i)
     {
         m_aOrders.RemoveOrdered(i);
         m_aValues.RemoveOrdered(i);
     }
     void RemoveOrders(int order)
     {
         for (int i = Count() - 1; i >= 0; i--)
         {
             if (m_aOrders[i] == order)
             {
                 m_aOrders.RemoveOrdered(i);
                 m_aValues.RemoveOrdered(i);
             }
         }
     }
     void RemoveValues(TValue value)
     {
         for (int i = Count() - 1; i >= 0; i--)
         {
             if (m_aValues[i] == value)
             {
                 m_aOrders.RemoveOrdered(i);
                 m_aValues.RemoveOrdered(i);
             }
         }
     }
     int GetOrder(int i)
     {
         return m_aOrders[i];
     }
     TValue GetValue(int i)
     {
         return m_aValues[i];
     }
     int Count()
     {
         return m_aOrders.Count();
     }
     bool IsEmpty()
     {
         return m_aOrders.IsEmpty();
     }
     bool Contains(TValue value)
     {
         return m_aValues.Contains(value);
     }
     int Find(TValue value)
     {
         return m_aValues.Find(value);
     }
     int CopyFrom(notnull SCR_SortedArray<TValue> from)
     {
         Clear();
         int count = from.Count();
         for (int i = 0; i < count; i++)
         {
             m_aOrders.Insert(from.m_aOrders[i]);
             m_aValues.Insert(from.m_aValues[i]);
         }
         return count;
     }
     void Clear()
     {
         m_aOrders.Clear();
         m_aValues.Clear();
     }
     int ToArray(out notnull array<TValue> outArray)
     {
         return outArray.Copy(m_aValues);
     }
     void Debug()
     {
         PrintFormat("SCR_SortedArray count: %1", Count());
         for (int i, count = Count(); i < count; i++)
         {
             PrintFormat("[%1] => %2: %3", i, m_aOrders[i], m_aValues[i]);
         }
     }
 };