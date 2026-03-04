
 typedef array<ref SCR_AIGroupFireteamLock> TFireteamLockRefArray;
 typedef array<SCR_AIGroupFireteamLock> TFireteamLockArray;

 class SCR_AIGroupFireteamLock : Managed
 {
     protected ref SCR_AIGroupFireteam m_Fireteam;

     //--------------------------------------------------------------------------------------------------------
     void SCR_AIGroupFireteamLock(SCR_AIGroupFireteam ft)
     {
         m_Fireteam = ft;
     }

     //--------------------------------------------------------------------------------------------------------
     void ~SCR_AIGroupFireteamLock()
     {
         if (m_Fireteam)
             m_Fireteam.Internal_OnLockDestroyed();
     }

     //--------------------------------------------------------------------------------------------------------
     SCR_AIGroupFireteam GetFireteam()
     {
         return m_Fireteam;
     }



     //--------------------------------------------------------------------------------------------------------

     //--------------------------------------------------------------------------------------------------------
  static int FindFireteamLock(notnull TFireteamLockRefArray locksArray, SCR_AIGroupFireteam ft)
     {
         foreach (int i, SCR_AIGroupFireteamLock l : locksArray)
         {
             if (l.m_Fireteam == ft)
                 return i;
         }
         return -1;
     }
     static int FindFireteamLock(notnull TFireteamLockArray locksArray, SCR_AIGroupFireteam ft)
     {
         foreach (int i, SCR_AIGroupFireteamLock l : locksArray)
         {
             if (l.m_Fireteam == ft)
                 return i;
         }
         return -1;
     }

     //--------------------------------------------------------------------------------------------------------
  static int FindFireteamLock(notnull TFireteamLockRefArray locksArray, SCR_AIGroupFireteamLock ft)
     {
         foreach (int i, SCR_AIGroupFireteamLock l : locksArray)
         {
             if (l == ft)
                 return i;
         }
         return -1;
     }
     static int FindFireteamLock(notnull TFireteamLockArray locksArray, SCR_AIGroupFireteamLock ft)
     {
         foreach (int i, SCR_AIGroupFireteamLock l : locksArray)
         {
             if (l == ft)
                 return i;
         }
         return -1;
     }

     //--------------------------------------------------------------------------------------------------------
  static bool RemoveFireteamLock(TFireteamLockRefArray locksArray, SCR_AIGroupFireteam ft)
     {
         int id = FindFireteamLock(locksArray, ft);
         if (id == -1)
             return false;

         locksArray.Remove(id);
         return true;
     }
     static bool RemoveFireteamLock(TFireteamLockArray locksArray, SCR_AIGroupFireteam ft)
     {
         int id = FindFireteamLock(locksArray, ft);
         if (id == -1)
             return false;

         locksArray.Remove(id);
         return true;
     }

     //--------------------------------------------------------------------------------------------------------
  static void TryLockFireteams(notnull array<SCR_AIGroupFireteam> fireteams, notnull TFireteamLockRefArray locks, bool clearLockArray)
     {
         if (clearLockArray)
             locks.Clear();

         foreach (SCR_AIGroupFireteam ft : fireteams)
         {
             SCR_AIGroupFireteamLock lock = ft.TryLock();
             if (lock)
                 locks.Insert(lock);
         }
     }

     //--------------------------------------------------------------------------------------------------------
  static TFireteamLockRefArray CopyLockArray(TFireteamLockRefArray other)
     {
         TFireteamLockRefArray a = {};
         foreach (auto l : other)
             a.Insert(l);
         return a;
     }
 };