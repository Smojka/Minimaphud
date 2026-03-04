 [ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_EditableEntityComponentClass : ScriptComponentClass
 {
     [Attribute("0", UIWidgets.ComboBox, category: "Editable Entity", desc: "System type of the entity.", enums: ParamEnumArray.FromEnum(EEditableEntityType))]
     protected EEditableEntityType m_EntityType;

     [Attribute(category: "Visualization", desc: "GUI representation")]
     protected ref SCR_UIInfo m_UIInfo;

     [Attribute("", UIWidgets.Auto, category: "Visualization", desc: "Bone to which the icon is attached to")]
     protected string m_sIconBoneName;

     [Attribute(category: "Editable Entity")]
     protected ref SCR_EditableEntityInteraction m_EntityInteraction;

     //------------------------------------------------------------------------------------------------
  EEditableEntityType GetEntityType()
     {
         return m_EntityType;
     }

     //------------------------------------------------------------------------------------------------
  SCR_UIInfo GetInfo()
     {
         return m_UIInfo;
     }

     //------------------------------------------------------------------------------------------------
  string GetIconBoneName()
     {
         return m_sIconBoneName;
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditableEntityInteraction GetEntityInteraction()
     {
         if (m_EntityInteraction)
             return m_EntityInteraction;

         SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
         if (core)
             return core.GetEntityInteraction(GetEntityType());
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource GetEditableEntitySource(Resource entityResource)
     {
         if (!entityResource)
             return null;

         BaseResourceObject entityBase = entityResource.GetResource();
         if (!entityBase)
             return null;

         IEntitySource entitySource = entityBase.ToEntitySource();
         if (!entitySource)
             return null;

         return GetEditableEntitySource(entitySource);
     }

     //------------------------------------------------------------------------------------------------
  static IEntityComponentSource GetEditableEntitySource(IEntitySource entitySource)
     {
         if (!entitySource)
             return null;

         int componentsCount = entitySource.GetComponentCount();
         for (int i = 0; i < componentsCount; i++)
         {
             IEntityComponentSource componentSource = entitySource.GetComponent(i);
             if (componentSource.GetClassName().ToType().IsInherited(SCR_EditableEntityComponent))
                 return componentSource;
         }
         return null;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_EditableEntityUIInfo GetInfo(IEntityComponentSource componentSource)
     {
         BaseContainer infoSource = componentSource.GetObject("m_UIInfo");
         if (!infoSource)
             return null;

         SCR_EditableEntityUIInfo info = SCR_EditableEntityUIInfo.Cast(BaseContainerTools.CreateInstanceFromContainer(infoSource));
         info.InitFromSource(componentSource);
         return info;
     }

     //------------------------------------------------------------------------------------------------
  static EEditableEntityType GetEntityType(IEntityComponentSource componentSource)
     {
         EEditableEntityType type;
         componentSource.Get("m_EntityType", type);
         return type;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_EditableEntityInteraction GetEntityInteraction(IEntityComponentSource componentSource)
     {
         BaseContainer container = componentSource.GetObject("m_EntityInteraction");
         if (container)
             return SCR_EditableEntityInteraction.Cast(BaseContainerTools.CreateInstanceFromContainer(container));

         SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
         if (core)
             return core.GetEntityInteraction(GetEntityType(componentSource));
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  static ResourceName GetSlotPrefab(IEntityComponentSource componentSource)
     {
         BaseContainer info = componentSource.GetObject("m_UIInfo");
         if (!info)
             return ResourceName.Empty;

         ResourceName slotPrefab;
         info.Get("m_SlotPrefab", slotPrefab);
         return slotPrefab;
     }

     //------------------------------------------------------------------------------------------------
  static EEditableEntityFlag GetEntityFlags(IEntityComponentSource componentSource)
     {
         EEditableEntityFlag flags;
         componentSource.Get("m_Flags", flags);
         return flags;
     }

     //------------------------------------------------------------------------------------------------
  static bool HasFlag(IEntityComponentSource componentSource, EEditableEntityFlag flag)
     {
         EEditableEntityFlag flags;
         componentSource.Get("m_Flags", flags);
         return flags & flag;
     }

     //------------------------------------------------------------------------------------------------
  static bool GetEntitySourceBudgetCost(IEntityComponentSource editableEntitySource, out notnull array<ref SCR_EntityBudgetValue> budgetValues)
     {
         if (!editableEntitySource)
             return false;

         SCR_EditableEntityUIInfo editableEntityUIInfo = SCR_EditableEntityUIInfo.Cast(SCR_EditableEntityComponentClass.GetInfo(editableEntitySource));
         if (editableEntityUIInfo)
             return editableEntityUIInfo.GetEntityBudgetCost(budgetValues);

         return !budgetValues.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  static bool GetEntitySourceChildrenBudgetCosts(IEntityComponentSource editableEntitySource, out notnull array<ref SCR_EntityBudgetValue> budgetValues)
     {
         if (!editableEntitySource)
             return false;

         SCR_EditableEntityUIInfo editableEntityUIInfo = SCR_EditableEntityUIInfo.Cast(SCR_EditableEntityComponentClass.GetInfo(editableEntitySource));
         if (editableEntityUIInfo)
             editableEntityUIInfo.GetEntityChildrenBudgetCost(budgetValues);

         return !budgetValues.IsEmpty();
     }
 }