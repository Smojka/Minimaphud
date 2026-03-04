 [BaseContainerProps()]
 class SCR_BaseContextAction : SCR_BaseEditorAction
 {
     [Attribute()]
     private bool m_bIsServer;

     override bool IsServer()
     {
         return m_bIsServer;
     }
 };