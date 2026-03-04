 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class GarbageSystem: GameSystem
 {
     proto external bool Insert(notnull IEntity entity, float lifetime = -1, bool ignoreBlacklist = false);
     proto external bool IsInserted(notnull IEntity entity);
     proto external bool Withdraw(notnull IEntity entity);
     proto external bool Bump(notnull IEntity entity, float additionalLifetime);
     proto external float GetLifetime(notnull IEntity entity);
     proto external float GetRemainingLifetime(notnull IEntity entity);
     proto external void Flush(float minTrackedSeconds = 60);
     proto external bool UpdateBlacklist(notnull IEntity entity, bool blacklisted);

     // callbacks

     /*
     React to or change the behavior of the garbage insertion.
     \param ent Entity instance to check logic for.
     \param lifetime Original lifetime the insertion was requested with.
     \return Returns a lifetime >0 for insertion and <= 0 to prevent it.

     Example override to use in modded classes:
     @code
     override protected float OnInsertRequested(IEntity entity, float lifetime)
     {
         if (someCondition)
             return -1; // Prevent insertion by returning negative values

         if (someOtherCondition)
             return lifetime * 0.5; // Cut lifetime in half based on specific needs.

         return lifetime; // Return unmodified value
     }
     @endcode
     */
     event protected float OnInsertRequested(IEntity entity, float lifetime) {return lifetime;};
     /*
     React to or skip the deletion of a tracked entity of which the lifetime has run out.
     \param ent Entity instance to check logic for.
     \return Return true to proceed with deletion, false to skip it (tracking for the item will end regardless).
     */
     event protected bool OnBeforeDelete(IEntity entity) {return true;};
 }
