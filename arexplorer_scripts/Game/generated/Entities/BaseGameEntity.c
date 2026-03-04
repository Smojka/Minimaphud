 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseGameEntityClass: GenericEntityClass
 {
 }

 class BaseGameEntity: GenericEntity
 {
     proto external void Teleport(vector mat[4]);
     proto external RplComponent GetRplComponent();
     proto external bool GetAncestorToLocalTransform(IEntity ancestor, out vector result[4]);
     proto external bool GetLocalToAncestorTransform(IEntity ancestor, out vector result[4]);
 }
