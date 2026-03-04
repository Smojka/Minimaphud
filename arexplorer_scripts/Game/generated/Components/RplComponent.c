 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class RplComponentClass: BaseRplComponentClass
 {
 }

 class RplComponent: BaseRplComponent
 {
     static proto void DeleteRplEntity(IEntity entity, bool releaseFromReplication);
     proto external void GiveExt(RplIdentity newOwner, bool alwaysNotify);
     proto external void ForceNodeMovement(vector previousPos);
     proto external void EnableStreaming(bool enable);
     proto external void EnableStreamingConNode(RplIdentity identity, bool enable);
     static proto void EnableStreamingForConnection(RplIdentity identity, bool enable);
     /*
     Insert MPObserver at position, for identity.
     Allows for static observer that then streams in/preloads replicated entities

     \warning Ensure you delete these observers after usage, they can cause performance issues.
     \param identity Client connection.
     \param x Position of observer.
     \param z Position of observer.
     */
     static proto void InsertMPObserver(RplIdentity identity, float x, float z);
     /*
     Remove MPObserver for identity.

     \param identity Client connection.
     */
     static proto void RemoveMPObserver(RplIdentity identity);
 }
