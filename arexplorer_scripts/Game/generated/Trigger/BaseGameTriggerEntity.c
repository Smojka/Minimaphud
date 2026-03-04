 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseGameTriggerEntityClass: GenericEntityClass
 {
 }

 class BaseGameTriggerEntity: GenericEntity
 {
     protected ref ScriptInvoker Event_OnQueryFinished = new ScriptInvoker();

     ScriptInvoker GetOnQueryFinished()
     {
         return Event_OnQueryFinished;
     }

     // Check if the given entity is inside the Trigger shape (bypasses the defined query and filter)
     proto external bool QueryEntityInside(notnull IEntity ent);
     // Queries entities which are inside the Trigger shape based on the defined filter
     proto external void QueryEntitiesInside();
     // Compares the given entity with the defined filters (Name and/or Class)
     proto external bool DefaultEntityFilterForQuery(IEntity ent);
     proto external int GetEntitiesInside(out notnull array<IEntity> outEntities);
     // Sets the sphere radius of the Trigger
     proto external void SetSphereRadius(float radius);
     // Gets the current sphere radius of the Trigger
     proto external float GetSphereRadius();
     // Sets the update rate of the trigger in seconds
     proto external void SetUpdateRate(float updateRate);
     // Gets the update rate of the trigger in seconds
     proto external float GetUpdateRate();
     // Set the periodic queries
     proto external void EnablePeriodicQueries(bool enable);
     // Is periodic queries enabled?
     proto external bool IsPeriodicQueriesEnabled();
     proto external void AddFilterName(string name);
     proto external bool RemoveFilterName(string name);
     proto external void AddClassType(typename classType);
     proto external bool RemoveClassType(typename classType);
     proto external void AddPrefabFilter(PrefabFilter prefabFilter);
     proto external bool RemovePrefabFilter(PrefabFilter prefabFilter);
     proto external EQueryEntitiesFlags GetTraceMask();
     proto external void SetTraceMask(EQueryEntitiesFlags flags);
     proto external void ClearTraceMask(EQueryEntitiesFlags flags);

     // callbacks

     event protected void OnActivate(IEntity ent);
     event protected void OnDeactivate(IEntity ent);
     event protected void OnQueryFinished(bool bIsEmpty){ Event_OnQueryFinished.Invoke(this); Event_OnQueryFinished.Clear(); };
 }
