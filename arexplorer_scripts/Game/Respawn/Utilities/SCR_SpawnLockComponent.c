 class SCR_SpawnLockComponentClass : ScriptComponentClass
 {
 }

 class SCR_SpawnLockComponent : ScriptComponent
 {
     protected ref set<Managed> m_RequestLocks = new set<Managed>();

     protected ref set<Managed> m_AuthorityLocks = new set<Managed>();

     //------------------------------------------------------------------------------------------------
     protected override void OnPostInit(IEntity owner)
     {
         super.OnPostInit(owner);
         if (!PlayerController.Cast(owner))
             Debug.Error(string.Format("%1 is not attached to a %2!", Type().ToString(), PlayerController));
     }

     //------------------------------------------------------------------------------------------------
  void Lock(Managed source, bool auth)
     {
         if (auth)
             m_AuthorityLocks.Insert(source);
         else
             m_RequestLocks.Insert(source);

         #ifdef _ENABLE_RESPAWN_LOGS
         PrintLocks("SCR_SpawnLockComponent::Lock()", source, auth);
         #endif
     }

     //------------------------------------------------------------------------------------------------
  void Unlock(Managed source, bool auth)
     {
         if (auth)
         {
             int index = m_AuthorityLocks.Find(source);
             if (index == -1)
                 return;

             m_AuthorityLocks.Remove(index);
         }
         else
         {
             int index = m_RequestLocks.Find(source);
             if (index == -1)
                 return;

             m_RequestLocks.Remove(index);
         }

         #ifdef _ENABLE_RESPAWN_LOGS
         PrintLocks("SCR_SpawnLockComponent::Unlock()", source, auth);
         #endif
     }

     //------------------------------------------------------------------------------------------------
  bool IsLocked(bool auth)
     {
         if (auth)
             return !m_AuthorityLocks.IsEmpty();
         else
             return !m_RequestLocks.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  bool TryLock(Managed source, bool auth)
     {
         if (IsLocked(auth))
             return false;

         Lock(source, auth);
         return true;
     }

     #ifdef _ENABLE_RESPAWN_LOGS

     //------------------------------------------------------------------------------------------------
  void PrintLocks(string ctx, Managed src, bool auth)
     {
         Print(string.Format("%1 (reqCnt: %2 | authCnt: %3 | (src: %4, auth: %5)",
             ctx, m_RequestLocks.Count(), m_AuthorityLocks.Count(), src, auth), LogLevel.NORMAL);
     }
     #endif
 }