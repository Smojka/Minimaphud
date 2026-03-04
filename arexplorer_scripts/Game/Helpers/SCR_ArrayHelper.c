 class SCR_ArrayHelper
 {
     //------------------------------------------------------------------------------------------------
  static int GetWeightedIndex(notnull array<float> weights, float value01)
     {
         if (weights.IsEmpty())
             return -1;

         if (value01 < 0 || value01 > 1)
             value01 = Math.Mod(value01, 1);

         float probabilitySum;
         for (int i, count = weights.Count(); i < count; i++)
         {
             probabilitySum += weights[i];
         }

         float add = 0;
         probabilitySum *= value01;

         for (int i, count = weights.Count(); i < count; i++)
         {
             add += weights[i];
             if (add > probabilitySum)
                 return i;
         }

         return 0;
     }
 }

 class SCR_ArrayHelperT<Class T>
 {
     //------------------------------------------------------------------------------------------------
  static void CopyReferencesFromTo(notnull array<ref T> source, notnull array<ref T> destination)
     {
         destination.Clear();
         foreach (T sourceRef : source)
         {
             destination.Insert(sourceRef);
         }
     }

     //------------------------------------------------------------------------------------------------
  // NATIVE version
     static array<T> GetCopy(array<T> source)
     {
         if (!source)
             return null;

         array<T> result = {};
         result.Copy(source);
         return result;
     }

     //------------------------------------------------------------------------------------------------
  // REFERENCE version
     static array<ref T> GetCopy(array<ref T> source)
     {
         if (!source)
             return null;

         array<ref T> result = {};
         foreach (T sourceRef : source)
         {
             result.Insert(sourceRef);
         }
         return result;
     }

     //------------------------------------------------------------------------------------------------
  // NATIVE version
     static void Intersect(notnull array<T> array1, notnull array<T> array2, notnull out array<T> result/*, bool unique = true */)
     {
         result.Clear();
         int count1 = array1.Count();
         int count2 = array2.Count();
         if (count1 > count2)
         {
             for (int i = array2.Count() - 1; i >= 0; i--)
             {
                 if (array1.Contains(array2[i]))
                     result.Insert(array2[i]);
             }
         }
         else
         {
             for (int i = array1.Count() - 1; i >= 0; i--)
             {
                 if (array2.Contains(array1[i]))
                     result.Insert(array1[i]);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  // REFERENCE version
     static void Intersect(notnull array<ref T> array1, notnull array<ref T> array2, notnull out array<ref T> result/*, bool unique = true */)
     {
         result.Clear();
         int count1 = array1.Count();
         int count2 = array2.Count();
         if (count1 > count2)
         {
             for (int i = array2.Count() - 1; i >= 0; i--)
             {
                 if (array1.Contains(array2[i]))
                     result.Insert(array2[i]);
             }
         }
         else
         {
             for (int i = array1.Count() - 1; i >= 0; i--)
             {
                 if (array2.Contains(array1[i]))
                     result.Insert(array1[i]);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  // NATIVE version
     static void RemoveDuplicates(notnull inout array<T> items)
     {
         for (int i = items.Count() - 1; i >= 0; --i)
         {
             if (items.Find(items[i]) != i)
                 items.RemoveOrdered(i);
         }
     }

     //------------------------------------------------------------------------------------------------
  // REFERENCE version
     static void RemoveDuplicates(notnull inout array<ref T> items)
     {
         for (int i = items.Count() - 1; i >= 0; --i)
         {
             if (items.Find(items[i]) != i)
                 items.RemoveOrdered(i);
         }
     }

     //------------------------------------------------------------------------------------------------
  // NATIVE version
     static void Reverse(notnull inout array<T> items)
     {
         int itemsCount = items.Count();
         if (itemsCount < 2)
             return;

         int flooredMiddle = itemsCount * 0.5;
         itemsCount--;

         // 4 indices
         // flooredMiddle = 2
         // 0 <-> 3
         // 1 <-> 2
         // { 0 1 2 3 }
         // { 3 2 1 0 }

         // 5 indices
         // flooredMiddle = 2
         // 0 <-> 4
         // 1 <-> 3
         // 2 is central
         // { 0 1 2 3 4 }
         // { 4 3 2 1 0 }

         for (int i; i < flooredMiddle; i++)
         {
             items.SwapItems(i, itemsCount - i);
         }
     }

     //------------------------------------------------------------------------------------------------
  // REFERENCE version
     static void Reverse(notnull inout array<ref T> items)
     {
         int itemsCount = items.Count();
         if (itemsCount < 2)
             return;

         int flooredMiddle = itemsCount * 0.5;
         itemsCount--;

         // 4 indices
         // flooredMiddle = 2
         // 0 <-> 3
         // 1 <-> 2
         // { 0 1 2 3 }
         // { 3 2 1 0 }

         // 5 indices
         // flooredMiddle = 2
         // 0 <-> 4
         // 1 <-> 3
         // 2 is central
         // { 0 1 2 3 4 }
         // { 4 3 2 1 0 }

         for (int i; i < flooredMiddle; i++)
         {
             items.SwapItems(i, itemsCount - i);
         }
     }

     //------------------------------------------------------------------------------------------------
  // NATIVE version
     static void Shuffle(notnull inout array<T> items, int shuffles = 1)
     {
         if (items.Count() < 2)
             return;

         // two checks are faster than Math.ClampInt
         if (shuffles < 1)
             shuffles = 1;

         if (shuffles > 10)
             shuffles = 10;

         while (shuffles > 0)
         {
             for (int i = 0, count = items.Count(); i < count; i++)
             {
                 int index1 = Math.RandomInt(0, count);
                 int index2 = Math.RandomInt(0, count);
                 if (index1 != index2)
                     items.SwapItems(index1, index2);
             }

             shuffles--;
         }
     }

     //------------------------------------------------------------------------------------------------
  // REFERENCE version
     static void Shuffle(notnull inout array<ref T> items, int shuffles = 1)
     {
         if (items.Count() < 2)
             return;

         // two checks are faster than Math.ClampInt
         if (shuffles < 1)
             shuffles = 1;

         if (shuffles > 10)
             shuffles = 10;

         while (shuffles > 0)
         {
             for (int i = 0, count = items.Count(); i < count; i++)
             {
                 int index1 = Math.RandomInt(0, count);
                 int index2 = Math.RandomInt(0, count);
                 if (index1 != index2)
                     items.SwapItems(index1, index2);
             }

             shuffles--;
         }
     }

 //  //------------------------------------------------------------------------------------------------
 //  //!
 //  //! \param[in] toConvert
 //  //! \param[in] result
 //  // NATIVE version
 //  static void ArrayToSet(notnull array<T> toConvert, notnull set<T> result)
 //  {
 //      result.Clear();
 //      foreach (T item : toConvert)
 //      {
 //          result.Insert(item);
 //      }
 //  }
 //
 //  //------------------------------------------------------------------------------------------------
 //  //!
 //  //! \param[in] toConvert
 //  //! \param[in] result
 //  // REFERENCE version
 //  static void ArrayToSet(notnull array<ref T> toConvert, notnull set<ref T> result)
 //  {
 //      result.Clear();
 //      foreach (T item : toConvert)
 //      {
 //          result.Insert(item);
 //      }
 //  }
 //
 //  //------------------------------------------------------------------------------------------------
 //  //! \param[in] toConvert
 //  //! \param[in] result
 //  // NATIVE version
 //  static void SetToArray(notnull set<T> toConvert, notnull array<T> result)
 //  {
 //      result.Clear();
 //      foreach (T item : toConvert)
 //      {
 //          result.Insert(item);
 //      }
 //  }
 //
 //  //------------------------------------------------------------------------------------------------
 //  //! \param[in] toConvert
 //  //! \param[in] result
 //  // REFERENCE version
 //  static void SetToArray(notnull set<ref T> toConvert, notnull array<ref T> result)
 //  {
 //      result.Clear();
 //      foreach (T item : toConvert)
 //      {
 //          result.Insert(item);
 //      }
 //  }
 }