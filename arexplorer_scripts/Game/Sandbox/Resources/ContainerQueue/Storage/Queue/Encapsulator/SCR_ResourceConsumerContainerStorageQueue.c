 [BaseContainerProps()]
 class SCR_ResourceEncapsulatorContainerStorageQueue : SCR_ResourceContainerStorageQueue<SCR_ResourceEncapsulator>
 {
     //------------------------------------------------------------------------------------------------
  override int RegisterContainer(notnull SCR_ResourceContainer container, notnull SCR_ResourceEncapsulator actor)
     {
         return InsertContainerAt(container, ComputePosition(container, actor));
     }

     //------------------------------------------------------------------------------------------------
     override int ComputePosition(SCR_ResourceContainer container, SCR_ResourceEncapsulator actor)
     {
         return GetContainerCount();
     }
 }