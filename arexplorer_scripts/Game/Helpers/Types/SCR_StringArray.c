 // typedef?

 class SCR_StringArray : array<string>
 {
     //------------------------------------------------------------------------------------------------
  int CountEmptyEntries()
     {
         int result;
         for (int i = Count() - 1; i >= 0; --i)
         {
             if (Get(i).IsEmpty())
                 result++;
         }

         return result;
     }

     //------------------------------------------------------------------------------------------------
  int CountEmptyOrWhiteSpaceEntries()
     {
         int result;
         for (int i = Count() - 1; i >= 0; --i)
         {
             if (SCR_StringHelper.IsEmptyOrWhiteSpace(Get(i)))
                 result++;
         }

         return result;
     }

     //------------------------------------------------------------------------------------------------
  string Join(string separator = string.Empty, bool joinEmptyEntries = true)
     {
         return SCR_StringHelper.Join(separator, this, joinEmptyEntries);
     }

     //------------------------------------------------------------------------------------------------
  bool HasEmptyEntry()
     {
         for (int i = Count() - 1; i >= 0; --i)
         {
             if (Get(i).IsEmpty())
                 return true;
         }
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool HasEmptyOrWhiteSpaceEntry()
     {
         for (int i = Count() - 1; i >= 0; --i)
         {
             if (SCR_StringHelper.IsEmptyOrWhiteSpace(Get(i)))
                 return true;
         }
         return false;
     }

     //------------------------------------------------------------------------------------------------
  int RemoveEmptyEntries()
     {
         int removed;
         for (int i = Count() - 1; i >= 0; --i)
         {
             if (Get(i).IsEmpty())
             {
                 RemoveOrdered(i);
                 ++removed;
             }
         }

         return removed;
     }

     //------------------------------------------------------------------------------------------------
  int RemoveEmptyOrWhiteSpaceEntries()
     {
         int removed;
         for (int i = Count() - 1; i >= 0; --i)
         {
             if (SCR_StringHelper.IsEmptyOrWhiteSpace(Get(i)))
             {
                 RemoveOrdered(i);
                 ++removed;
             }
         }

         return removed;
     }

     //------------------------------------------------------------------------------------------------
  void ToLower()
     {
         foreach (int i, string entry : this)
         {
             entry.ToLower();
             Set(i, entry);
         }
     }

     //------------------------------------------------------------------------------------------------
  void ToUpper()
     {
         foreach (int i, string entry : this)
         {
             entry.ToUpper();
             Set(i, entry);
         }
     }

     //------------------------------------------------------------------------------------------------
  void Trim()
     {
         foreach (int i, string entry : this)
         {
             entry.TrimInPlace();
             Set(i, entry);
         }
     }
 }