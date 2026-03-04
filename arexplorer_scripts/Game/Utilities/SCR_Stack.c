 //-----------------------------------------------------------------------------------------------------------
 // Temporal wrapper for array that works as a stack
 // Items are stored as 'first in, last out'
 // Items inside are always ref, so bear that in mind
 // Might be replaced with cpp implementation with script api later on
 class SCR_Stack<Class T>
 {
     protected ref array<ref T> m_aArray;

     //-----------------------------------------------------------------------------------------------------------
  void Push(T item)
     {
         m_aArray.Insert(item);
     }

     //-----------------------------------------------------------------------------------------------------------
     T Pop()
     {
         int length = m_aArray.Count();
         if (length >= 1)
         {
             int index = length-1;

             ref T poppedItem = m_aArray[index];
             m_aArray.Remove(index);

             return poppedItem;
         }

         return null;
     }

     //-----------------------------------------------------------------------------------------------------------
  bool IsEmpty()
     {
         return (m_aArray.Count() == 0);
     }

     //-----------------------------------------------------------------------------------------------------------
  int Count()
     {
         return m_aArray.Count();
     }

     //-----------------------------------------------------------------------------------------------------------
  void SCR_Stack()
     {
         m_aArray = new array<ref T>();
     }

     //-----------------------------------------------------------------------------------------------------------
  void ~SCR_Stack()
     {
         if (m_aArray)
         {
             m_aArray.Clear();
             m_aArray = null;
         }
     }
 };