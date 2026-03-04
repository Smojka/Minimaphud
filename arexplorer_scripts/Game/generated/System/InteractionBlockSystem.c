 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class InteractionBlockSystem: GameSystem
 {
     proto external bool CanCreateInteractionBlocks(IEntity ent, IEntity otherEnt);
     proto external bool Create(IEntity ent, IEntity otherEnt);
     proto external bool CreateSimpleBlock(IEntity ent, IEntity otherEnt);
     proto external bool DoesExist(IEntity ent, IEntity otherEnt);
     proto external bool Remove(IEntity ent);
     proto external bool RemoveInteractionBlocks(IEntity ent, IEntity otherEnt);
     proto external bool RemoveSimpleBlock(IEntity ent, IEntity otherEnt);
 }
