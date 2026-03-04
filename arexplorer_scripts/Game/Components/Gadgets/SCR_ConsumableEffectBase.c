 enum SCR_EConsumableType
 {
     NONE,
     BANDAGE,
     HEALTH,
     TOURNIQUET,
     SALINE,
     MORPHINE,
     MED_KIT
 }

 enum SCR_EConsumableFailReason
 {
     NONE = 0,
     UNKOWN = 1,
     IS_BLEEDING = 10,
     NOT_BLEEDING = 11,
     ALREADY_APPLIED = 12,
     DAMAGED = 13,
     UNDAMAGED = 14
 }

 [BaseContainerProps()]
 class SCR_ConsumableEffectBase : Managed
 {
     protected TAnimGraphCommand m_iPlayerApplyToSelfCmdId = -1;
     protected TAnimGraphCommand m_iPlayerApplyToOtherCmdId = -1;
     protected TAnimGraphCommand m_iPlayerReviveCmdId = -1;

     [Attribute("true", UIWidgets.CheckBox, "Whether consumable should be deleted directly after completing use", category: "General")]
     protected bool m_bDeleteOnUse;

     [Attribute("1", UIWidgets.EditBox, "Duration of the animation for using consumable on self", category: "General")]
     protected float m_fApplyToSelfDuration;

     [Attribute("1", UIWidgets.EditBox, "Duration of the animation for using consumable on other", category: "General")]
     protected float m_fApplyToOtherDuration;

     SCR_EConsumableType m_eConsumableType;

     //------------------------------------------------------------------------------------------------
  bool ActivateEffect(IEntity target, IEntity user, IEntity item, ItemUseParameters animParams = null)
     {
         ChimeraCharacter character = ChimeraCharacter.Cast(user);
         if (!character)
             return false;

         CharacterControllerComponent controller = character.GetCharacterController();
         if (!controller)
             return false;

         bool activatedAction;

         if (animParams)
         {
             activatedAction = controller.TryUseItemOverrideParams(animParams);
         }
         else
             activatedAction = controller.TryUseItem(item);

         return activatedAction;
     }

     //------------------------------------------------------------------------------------------------
  void ApplyEffect(notnull IEntity target, notnull IEntity user, IEntity item, ItemUseParameters animParams);

     //------------------------------------------------------------------------------------------------
  bool CanApplyEffect(notnull IEntity target, notnull IEntity user, out SCR_EConsumableFailReason failReason = SCR_EConsumableFailReason.NONE);

     //------------------------------------------------------------------------------------------------
  bool UpdateAnimationCommands(IEntity user);

     //------------------------------------------------------------------------------------------------
  ItemUseParameters GetAnimationParameters(IEntity item, notnull IEntity target, ECharacterHitZoneGroup group = ECharacterHitZoneGroup.VIRTUAL)
     {
         ItemUseParameters params = ItemUseParameters();
         params.SetEntity(item);
         params.SetAllowMovementDuringAction(false);
         params.SetKeepInHandAfterSuccess(false);
         params.SetCommandID(GetApplyToSelfAnimCmnd(target));
         params.SetCommandIntArg(1);
         params.SetCommandFloatArg(0.0);
         params.SetMaxAnimLength(m_fApplyToSelfDuration);
         params.SetIntParam(0);

         return params;
     }

     //------------------------------------------------------------------------------------------------
  bool GetDeleteOnUse()
     {
         return m_bDeleteOnUse;
     }

     //------------------------------------------------------------------------------------------------
  float GetApplyToOtherDuraction()
     {
         return m_fApplyToOtherDuration;
     }

     //------------------------------------------------------------------------------------------------
     TAnimGraphCommand GetApplyToSelfAnimCmnd(IEntity user)
     {
         UpdateAnimationCommands(user);
         return m_iPlayerApplyToSelfCmdId;
     }

     //------------------------------------------------------------------------------------------------
     TAnimGraphCommand GetApplyToOtherAnimCmnd(IEntity user)
     {
         UpdateAnimationCommands(user);
         return m_iPlayerApplyToOtherCmdId;
     }
 }