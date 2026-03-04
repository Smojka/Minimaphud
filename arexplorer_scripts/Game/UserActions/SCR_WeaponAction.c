 enum EWeaponActionType
 {
     Bipod
 };

 class SCR_WeaponAction : ScriptedUserAction
 {
     [Attribute( uiwidget: UIWidgets.CheckBox, defvalue: "1", desc: "Should action behave as a toggle")]
     protected bool m_bIsToggle;

     [Attribute( uiwidget: UIWidgets.CheckBox, defvalue: "1", desc: "Target state of the action, ignored if toggle")]
     protected bool m_bTargetState;

     [Attribute( uiwidget: UIWidgets.Auto, defvalue: "#AR-UserAction_State_On", desc: "Description of action to toggle on")]
     protected string m_sActionStateOn;

     [Attribute( uiwidget: UIWidgets.Auto, defvalue: "#AR-UserAction_State_Off", desc: "Description of action to toggle off")]
     protected string m_sActionStateOff;

     [Attribute("1", UIWidgets.ComboBox, "", "", ParamEnumArray.FromEnum(EWeaponActionType) )]
     protected EWeaponActionType m_WeaponActionType;

     protected BaseWeaponComponent m_Weapon;

     override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
     {
         m_Weapon = WeaponComponent.Cast(pOwnerEntity.FindComponent(WeaponComponent));
     }

     //------------------------------------------------------------------------------------------------
     override bool CanBeShownScript(IEntity user)
     {
         if (!m_Weapon)
             return false;

         ChimeraCharacter character = ChimeraCharacter.Cast(user);
         if (!character)
             return false;

         CharacterControllerComponent controller = character.GetCharacterController();
         if (!controller)
             return false;

         if (!controller.GetInspect())
             return false;

         if (m_WeaponActionType == EWeaponActionType.Bipod)
             return m_Weapon && m_Weapon.HasBipod();

         return true;
     }

     //------------------------------------------------------------------------------------------------
     override bool CanBePerformedScript(IEntity user)
     {
         return m_bIsToggle || (GetState() != m_bTargetState);
     }

     //------------------------------------------------------------------------------------------------
  bool GetState()
     {
         if (!m_Weapon)
             return false;

         if (m_WeaponActionType == EWeaponActionType.Bipod)
             return m_Weapon.GetBipod();

         return false;
     }

     //------------------------------------------------------------------------------------------------
  void SetState(bool enable)
     {
         if (!m_Weapon)
             return;

         if (m_WeaponActionType == EWeaponActionType.Bipod)
             m_Weapon.SetBipod(enable);
     }

     //------------------------------------------------------------------------------------------------
  override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
     {
         if (m_bIsToggle)
             SetState(!GetState());
         else
             SetState(m_bTargetState);
     }

     //------------------------------------------------------------------------------------------------
     override bool GetActionNameScript(out string outName)
     {
         if (m_WeaponActionType == EWeaponActionType.Bipod)
         {
             if (GetState())
                 outName = "#AR-Keybind_BipodRetract";
             else
                 outName = "#AR-Keybind_Bipod";

             return true;
         }

         UIInfo uiInfo = GetUIInfo();
         string prefix;
         if (uiInfo)
             prefix = uiInfo.GetName();

         if (!prefix.IsEmpty() && m_bIsToggle)
         {
             prefix += " ";
             if (GetState())
                 outName = prefix + m_sActionStateOff;
             else
                 outName = prefix + m_sActionStateOn;
         }
         else
             outName = prefix;
         return true;
     }

     //------------------------------------------------------------------------------------------------
     override bool HasLocalEffectOnlyScript()
     {
         return true;
     }
 };