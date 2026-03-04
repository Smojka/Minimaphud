 //------------------------------------------------------------------------------------------------
 class SCR_GroupWeaponEntry : SCR_BaseGroupEntry
 {
     protected IEntity m_pOwner;
     protected WeaponSlotComponent m_pWeaponSlotComponent;

     //------------------------------------------------------------------------------------------------
  override void OnPerform(IEntity user, BaseSelectionMenu sourceMenu)
     {
         // We have no means of switching the weapons for our character
         // if the controller component is null
         auto pGenericOwner = GenericEntity.Cast(m_pOwner);
         if (!pGenericOwner)
             return;

         auto pCharacterController = CharacterControllerComponent.Cast(pGenericOwner.FindComponent(CharacterControllerComponent));
         if (!pCharacterController)
             return;

         pCharacterController.SelectWeapon(m_pWeaponSlotComponent);
     }

     //------------------------------------------------------------------------------------------------
  override void OnDonePerform(IEntity user, BaseSelectionMenu sourceMenu)
     {

     }

     protected override bool CanBePerformedScript(IEntity user, BaseSelectionMenu sourceMenu)
     {
         if (m_pWeaponSlotComponent && m_pWeaponSlotComponent.GetWeaponEntity())
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  protected override UIInfo GetUIInfoScript()
     {
         if (m_pWeaponSlotComponent)
             return m_pWeaponSlotComponent.GetUIInfo();

         return null;
     }

     //------------------------------------------------------------------------------------------------
  void SCR_GroupWeaponEntry(IEntity pOwner, WeaponSlotComponent pWeaponSlot)
     {
         // Assign the owner
         m_pOwner = pOwner;

         // Assign weapon component
         m_pWeaponSlotComponent = pWeaponSlot;
     }

 };