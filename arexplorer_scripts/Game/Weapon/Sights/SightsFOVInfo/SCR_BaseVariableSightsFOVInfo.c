 class SCR_BaseVariableSightsFOVInfo: ScriptedSightsFOVInfo
 {
     //------------------------------------------------------------------------------------------------
  int GetCount() { return 0; }

     //------------------------------------------------------------------------------------------------
  int GetCurrentIndex() { return -1; }

     //------------------------------------------------------------------------------------------------
  protected int GetNextIndex(bool allowOverflow = true)
     {
         int count = GetCount();
         if (count < 1)
             return -1;

         int current = GetCurrentIndex();
         int next = current + 1;
         if (next > count - 1)
         {
             if (allowOverflow)
                 return 0;
             else
                 return -1;
         }

         return next;
     }

     //------------------------------------------------------------------------------------------------
  protected int GetPreviousIndex(bool allowUnderflow = true)
     {
         int count = GetCount();
         if (count < 1)
             return -1;

         int current = GetCurrentIndex();
         int previous = current - 1;
         if (previous < 0)
         {
             if (allowUnderflow)
                 return count - 1;
             else return -1;
         }

         return previous;
     }

     //------------------------------------------------------------------------------------------------
  protected void SetIndex(int index);

     //------------------------------------------------------------------------------------------------
  bool SetNext(bool allowOverflow = true)
     {
         int next = GetNextIndex(allowOverflow);
         if (next != -1)
         {
             SetIndex(next);
             return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool SetPrevious(bool allowUnderflow = true)
     {
         int prev = GetPreviousIndex(allowUnderflow);
         if (prev != -1)
         {
             SetIndex(prev);
             return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool IsAdjusting()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  float GetBaseFOV()
     {
         return GetCurrentFOV();
     }
 };