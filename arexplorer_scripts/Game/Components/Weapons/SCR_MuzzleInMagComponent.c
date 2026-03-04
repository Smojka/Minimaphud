 [ComponentEditorProps(category: "GameScripted/Misc", description: "")]
 class SCR_MuzzleInMagComponentClass : MuzzleInMagComponentClass
 {
 }

 class SCR_MuzzleInMagComponent : MuzzleInMagComponent
 {
     [Attribute("1", desc: "Some launchers are one off and can never be reloaded. This bool makes sure that the resupply functionality will never try to add magazines/projectiles of the weapon to the inventory on resupply.")]
     protected bool m_bCanBeReloaded;

     //------------------------------------------------------------------------------------------------
  bool CanBeReloaded()
     {
         return m_bCanBeReloaded;
     }
 }