 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterIdentityComponentClass: GameComponentClass
 {
 }

 class CharacterIdentityComponent: GameComponent
 {
     proto external IEntity GetHeadEntity();
     /*
     Sets the Alpha for the head.
     \param a target Aplha between 0-255
     */
     proto external void SetHeadAlpha(int a);
     /*
     Sets the visibility of all submeshes.
     \param visible Whether all submeshes should be drawn or not.
     */
     proto external void SetVisibleAll(bool visible);
     /*
     Returns the visibility of all submeshes.
     \return Returns whether all submesh visibility is enabled or not.
     */
     proto external bool IsVisibleAll();
     /*
     Sets wound state for specific bodyPart.
     \param bodyPart Name of the part to be changed
     \param wound Whether part should be wounded or not.
     */
     proto external bool SetWoundState(string bodyPart, bool wound);
     proto external bool IsCovered(string bodyPart);
     /*
     Reuse currently stored Identity
     As an example
         CharacterIdentityComponent identityComponent = CharacterIdentityComponent.Cast( soldierEntity.FindComponent(CharacterIdentityComponent));
             if (!identityComponent)
                 return;
         VisualIdentity visId = identityComponent.GetIdentity().GetVisualIdentity();
         visId.SetHead("{24D28E910BF9F648}Prefabs/Characters/Heads/Head_Asian_02.et");
         identityComponent.CommitChanges();
     */
     proto external void CommitChanges();
     proto external Identity GetIdentity();
     proto external bool CheckIdentityIDs(int alias, int name, int surname, int soundIdentity, int visualIdentity);
     proto external bool SetIdentityFromIDs(int alias, int name, int surname, int soundIdentity, int visualIdentity);
     proto external void SetIdentity(Identity cfg);

     // callbacks

     event void OnBodyPartStateChanged(string bodyPart, bool visible, bool wounded);
 }
