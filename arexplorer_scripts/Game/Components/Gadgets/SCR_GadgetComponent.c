 [EntityEditorProps(category: "GameScripted/Gadgets", description: "Gadget base", color: "0 0 255 255")]
 class SCR_GadgetComponentClass : ScriptGameComponentClass
 {
 }

 //------------------------------------------------------------------------------------------------
 enum EGadgetType
 {
     NONE            = 1,
     MAP             = 1 << 1,
     COMPASS         = 1 << 2,
     BINOCULARS      = 1 << 3,
     FLASHLIGHT      = 1 << 4,
     RADIO           = 1 << 5,
     RADIO_BACKPACK  = 1 << 6,
     WRISTWATCH      = 1 << 7,
     CONSUMABLE      = 1 << 8,
     BUILDING_TOOL   = 1 << 9,
     SPECIALIST_ITEM = 1 << 10,
     NIGHT_VISION    = 1 << 11,
 }

 enum EGadgetMode
 {
     ON_GROUND = 0,  // ground
     IN_STORAGE,     // within storage but not slotted
     IN_SLOT,        // in equipment slot
     IN_HAND,        // held in left hand
     LAST
 }

 // @NOTE(Leo) : short term animation solution
 // from conversation with @ThÃ©o Escamez: //for now we have 4 items> compass adrianov, compass SY183, Radio ANPRC68 and Radio R148
 enum EGadgetAnimVariable
 {
     NONE = 0,
     ADRIANOV = 1,
     SY183 = 2,
     ANPRC68 = 3,
     R148 = 4,
     MX991 = 5,
     FLASHLIGHT_SOVIET_01 = 6,
     M22 = 7,
     B12 = 8,
     MAP = 9
 }

 class SCR_GadgetComponent : ScriptGameComponent
 {
     [Attribute("", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EGadgetAnimVariable), desc: "Gadget anim variable", category: "Gadget")]
     protected EGadgetAnimVariable m_eAnimVariable;

     [Attribute("0 0 0", UIWidgets.Coords, desc: "Adjusted position of prefab within equipment slot, for when items placed intto a same slot have different sizes like flashlights", category: "Gadget")]
     protected vector m_vEquipmentSlotOffset;

     bool m_bFocused;
     protected bool m_bActivated = false;                    // current state if the gadget can be toggled on
     protected EGadgetMode m_iMode = EGadgetMode.ON_GROUND;  // curent gadget mode
     protected IEntity m_CharacterOwner;                     // current entity in posession of this gadget

     //------------------------------------------------------------------------------------------------
     EGadgetAnimVariable GetAnimVariable()
     {
         return m_eAnimVariable;
     }

     //------------------------------------------------------------------------------------------------
  bool IsToggledOn()
     {
         return m_bActivated;
     }

     //------------------------------------------------------------------------------------------------
  void OnToggleActive(bool state);

     //------------------------------------------------------------------------------------------------
  void OnParentSlotChanged(InventoryStorageSlot oldSlot, InventoryStorageSlot newSlot)
     {
         if (newSlot == null)    // ground
         {
             IEntity parentOwner, owner;

             BaseInventoryStorageComponent storageComp = oldSlot.GetStorage();
             if (storageComp)
                 parentOwner = storageComp.GetOwner();
             else
                 parentOwner = oldSlot.GetOwner();   // LoadoutSlotInfo wont return storage comp

             owner = parentOwner;

             while (owner != null)
             {
                 parentOwner = owner;
                 owner = owner.GetParent();
             }

             if ( !SCR_ChimeraCharacter.Cast(parentOwner) )  // remove from char is currently handled by gadget manager
                 SCR_GadgetManagerComponent.SetGadgetModeStashed(this, EGadgetMode.ON_GROUND);
         }
         else if (oldSlot == null)
         {
             IEntity parentOwner, owner;

             BaseInventoryStorageComponent storageComp = newSlot.GetStorage();
             if (storageComp)
                 parentOwner = storageComp.GetOwner();
             else
                 parentOwner = newSlot.GetOwner();   // LoadoutSlotInfo wont return storage comp

             owner = parentOwner;

             while (owner != null)
             {
                 parentOwner = owner;
                 owner = owner.GetParent();
             }


             if ( !SCR_ChimeraCharacter.Cast(parentOwner) ) // add to char is currently handled by gadget manager
                 SCR_GadgetManagerComponent.SetGadgetModeStashed(this, EGadgetMode.IN_STORAGE);
         }
     }

     //------------------------------------------------------------------------------------------------
  void OnModeChanged(EGadgetMode mode, IEntity charOwner)
     {
         // Clear last mode
         ModeClear(m_iMode);

         // Update current mode
         m_iMode = mode;

         // Set new mode
         ModeSwitch(mode, charOwner);
     }

     //------------------------------------------------------------------------------------------------
  protected void ModeSwitch(EGadgetMode mode, IEntity charOwner)
     {
         UpdateVisibility(mode);

         // if removing from inventory
         if (mode == EGadgetMode.ON_GROUND)
             m_CharacterOwner = null;
         else
             m_CharacterOwner = charOwner;
     }

     //------------------------------------------------------------------------------------------------
  protected void ModeClear(EGadgetMode mode)
     {
     }

     //------------------------------------------------------------------------------------------------
  void ActivateGadgetUpdate()
     {
         ConnectToGadgetsSystem();
     }

     //------------------------------------------------------------------------------------------------
  void DeactivateGadgetUpdate()
     {
         DisconnectFromGadgetsSystem();
     }


     //------------------------------------------------------------------------------------------------
  // \param[in] inHand states whether the gadget is currently held in hand
     void UpdateVisibility(EGadgetMode mode)
     {
         // mode1
         InventoryItemComponent itemComponent = InventoryItemComponent.Cast(GetOwner().FindComponent(InventoryItemComponent));
         if (!itemComponent)
             return;

         // if in hand or visible while slotted
         if ( mode == EGadgetMode.IN_HAND || (mode == EGadgetMode.IN_SLOT && IsVisibleEquipped()) )
         {
             // slotted,  Set positioning of visible slotted gadgets on character -> configured in item prefab to allow gadgets of different sizes
             if (mode != EGadgetMode.IN_HAND)
             {
                 InventoryStorageSlot slot = itemComponent.GetParentSlot();
                 ItemAnimationAttributes animAttr = ItemAnimationAttributes.Cast(itemComponent.FindAttribute(ItemAnimationAttributes));
                 if (slot && animAttr)
                 {
                     vector matLS[4];
                     animAttr.GetAdditiveTransformLS(matLS);
                     matLS[3] = matLS[3] + m_vEquipmentSlotOffset;
                     slot.SetAdditiveTransformLS(matLS);
                 }

                 EquipmentStorageSlot equipSlot = EquipmentStorageSlot.Cast(slot);
                 if (equipSlot && equipSlot.IsOccluded())
                 {
                     itemComponent.HideOwner();  // slotted but occluded
                     return;
                 }
             }

             itemComponent.ShowOwner();
         }
         // if slotted and not visible or not slotted
         else
         {
             if (mode == EGadgetMode.ON_GROUND)
                 itemComponent.ShowOwner();
             else
                 itemComponent.HideOwner();
         }
     }

     //------------------------------------------------------------------------------------------------
  void ToggleActive(bool state)
     {
         if (!m_CharacterOwner)
             return;

         RplComponent rplComponent = RplComponent.Cast(m_CharacterOwner.FindComponent(RplComponent));
         if (!rplComponent || !rplComponent.IsOwner())
             return;                 // NOT owner of the character in possession of this gadget

         // Client side
         rplComponent = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
         if (rplComponent && rplComponent.IsProxy())
             OnToggleActive(state);  // activate client side to avoid server delay

         // Sync
         if (m_CharacterOwner)
             SCR_GadgetManagerComponent.GetGadgetManager(m_CharacterOwner).AskToggleGadget(this, state);
     }

     //------------------------------------------------------------------------------------------------
  void ActivateAction();

     //------------------------------------------------------------------------------------------------
  void ToggleFocused(bool enable)
     {
         m_bFocused = enable;

         if (m_CharacterOwner != SCR_PlayerController.GetLocalControlledEntity())
             return;

         SCR_PlayerController controller = SCR_PlayerController.Cast(GetGame().GetPlayerController());
         if (controller)
             controller.SetGadgetFocus(enable);
     }

     //------------------------------------------------------------------------------------------------
     IEntity GetCharacterOwner()
     {
         return m_CharacterOwner;
     }

     //------------------------------------------------------------------------------------------------
     EGadgetType GetType()
     {
         return EGadgetType.NONE;
     }

     //------------------------------------------------------------------------------------------------
     EGadgetMode GetMode()
     {
         return m_iMode;
     }

     //------------------------------------------------------------------------------------------------
  bool CanBeHeld()
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  bool CanBeToggled()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool CanBeRaised()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool IsUsingADSControls()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool IsVisibleEquipped()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  //Called by Gadgets system
     void Update(float timeSlice);

     //------------------------------------------------------------------------------------------------
  protected void ConnectToGadgetsSystem()
     {
         World world = GetOwner().GetWorld();
         GadgetsSystem gadgetSystem = GadgetsSystem.Cast(world.FindSystem(GadgetsSystem));
         if (!gadgetSystem)
             return;

         gadgetSystem.Register(this);
     }

     //------------------------------------------------------------------------------------------------
     protected void DisconnectFromGadgetsSystem()
     {
         World world = GetOwner().GetWorld();
         GadgetsSystem gadgetSystem = GadgetsSystem.Cast(world.FindSystem(GadgetsSystem));
         if (!gadgetSystem)
             return;

         gadgetSystem.Unregister(this);
     }

     //------------------------------------------------------------------------------------------------
     override bool RplSave(ScriptBitWriter writer)
     {
         writer.WriteIntRange(m_iMode, 0, EGadgetMode.LAST-1);

         return true;
     }

     //------------------------------------------------------------------------------------------------
     override bool RplLoad(ScriptBitReader reader)
     {
         reader.ReadIntRange(m_iMode, 0, EGadgetMode.LAST-1);

         UpdateVisibility(m_iMode);

         return true;
     }

     //------------------------------------------------------------------------------------------------
     override void OnDelete(IEntity owner)
     {
         DisconnectFromGadgetsSystem();

         super.OnDelete(owner);
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         SetEventMask( owner, EntityEvent.INIT);

         InventoryItemComponent invComp = InventoryItemComponent.Cast(GetOwner().FindComponent(InventoryItemComponent));
         if (invComp)
             invComp.m_OnParentSlotChangedInvoker.Insert(OnParentSlotChanged);
     }
 }