 [BaseContainerProps()]
 class SCR_EditableEntityUIInfo : SCR_UIInfo
 {
     [Attribute(params: "edds imageset", uiwidget: UIWidgets.ResourcePickerThumbnail)]
     private ResourceName m_Image;

     [Attribute()]
     private FactionKey m_sFaction;

     [Attribute("1", UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel), desc: "Entity labels configured by developers, changes will be applied when updating editable entities")]
     private ref array<EEditableEntityLabel> m_aAuthoredLabels;

     [Attribute("1", UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EEditableEntityLabel), desc: "Autoconfigured labels, for manual changes edit AuthoredLabels array instead")]
     private ref array<EEditableEntityLabel> m_aAutoLabels;

     [Attribute("", category: "Editable Entity", desc: "Define budget costs")]
     private ref array<ref SCR_EntityBudgetValue> m_EntityBudgetCost;

     [Attribute("", category: "Editable Entity", desc: "Autoconfigured budget costs of child entities (group/composition entities)")]
     private ref array<ref SCR_EntityBudgetValue> m_EntityChildrenBudgetCost;

     [Attribute(category: "Editable Entity", params: "et", desc: "Prefab this entity extends.")]
     protected ResourceName m_SlotPrefab;

     //--- Hidden vars, used only in content browser where they're filled from component source
     protected EEditableEntityType m_EntityType;
     protected EEditableEntityFlag m_EntityFlags;

     //~ Hotfix to make sure Entity names that are not localized get a default fallback name
     static const LocalizedString EDITABLE_ENTITY_FALLBACK_NAME = "#AR-AttributesDialog_TitlePage_Entity_Text";

     //------------------------------------------------------------------------------------------------
     //~ Hotfix to make sure Entity names that are not localized get a default fallback name
     override LocalizedString GetName()
     {
         //~ If empty string, placeholder string it will use the the fallback string so the name at least is localized.
         //~ It checks for '(' as all placeholder names check for it as checking for '#' will override the string for modders
         if (Name.IsEmpty() || Name[0] == "(")
             return EDITABLE_ENTITY_FALLBACK_NAME;

         return Name;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetImage()
     {
         return m_Image;
     }

     //------------------------------------------------------------------------------------------------
     EEditableEntityType GetEntityTypex()
     {
         return 0;
     }

     //------------------------------------------------------------------------------------------------
     FactionKey GetFactionKey()
     {
         return m_sFaction;
     }

     //------------------------------------------------------------------------------------------------
     Faction GetFaction()
     {
         FactionManager factionManager = GetGame().GetFactionManager();
         if (factionManager)
             return factionManager.GetFactionByKey(m_sFaction);
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  int GetEntityLabels(out notnull array<EEditableEntityLabel> entityLabels)
     {
         entityLabels.InsertAll(m_aAutoLabels);
         entityLabels.InsertAll(m_aAuthoredLabels);
         return entityLabels.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool HasEntityLabel(EEditableEntityLabel label)
     {
         if (m_aAutoLabels.Contains(label))
             return true;

         if (m_aAuthoredLabels.Contains(label))
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool GetEntityBudgetCost(out notnull array<ref SCR_EntityBudgetValue> outBudgets)
     {
         SCR_EntityBudgetValue.MergeBudgetCosts(outBudgets, m_EntityBudgetCost);
         return !outBudgets.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  void GetEntityAndChildrenBudgetCost(out notnull array<ref SCR_EntityBudgetValue> outBudgets)
     {
         SCR_EntityBudgetValue.MergeBudgetCosts(outBudgets, m_EntityBudgetCost);
         SCR_EntityBudgetValue.MergeBudgetCosts(outBudgets, m_EntityChildrenBudgetCost);
     }

     //------------------------------------------------------------------------------------------------
  void GetEntityChildrenBudgetCost(out notnull array<ref SCR_EntityBudgetValue> outBudgets)
     {
         SCR_EntityBudgetValue.MergeBudgetCosts(outBudgets, m_EntityChildrenBudgetCost);
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetSlotPrefab()
     {
         return m_SlotPrefab;
     }

     //------------------------------------------------------------------------------------------------
  EEditableEntityType GetEntityType()
     {
         return m_EntityType;
     }

     //------------------------------------------------------------------------------------------------
  bool HasEntityFlag(EEditableEntityFlag flag)
     {
         return (m_EntityFlags & flag) == flag;
     }

     //------------------------------------------------------------------------------------------------
  bool SetAssetImageTo(ImageWidget imageWidget)
     {
         if (!imageWidget || m_Image.IsEmpty())
             return false;

         imageWidget.LoadImageTexture(0, m_Image);
         return true;
     }

     //------------------------------------------------------------------------------------------------
  void InitFromSource(IEntityComponentSource componentSource)
     {
         m_EntityType = SCR_EditableEntityComponentClass.GetEntityType(componentSource);
         m_EntityFlags = SCR_EditableEntityComponentClass.GetEntityFlags(componentSource);
     }

     //------------------------------------------------------------------------------------------------
     protected override void CopyFrom(SCR_UIName source)
     {
         SCR_EditableEntityUIInfo editableEntitySource = SCR_EditableEntityUIInfo.Cast(source);
         if (editableEntitySource)
         {
             m_Image = editableEntitySource.m_Image;
             m_sFaction = editableEntitySource.m_sFaction;
             m_aAuthoredLabels = editableEntitySource.m_aAuthoredLabels;
             m_aAutoLabels = editableEntitySource.m_aAutoLabels;
             m_EntityBudgetCost = editableEntitySource.m_EntityBudgetCost;
             m_EntityChildrenBudgetCost = editableEntitySource.m_EntityChildrenBudgetCost;
             m_SlotPrefab = editableEntitySource.m_SlotPrefab;
         }

         super.CopyFrom(source);
     }
 }