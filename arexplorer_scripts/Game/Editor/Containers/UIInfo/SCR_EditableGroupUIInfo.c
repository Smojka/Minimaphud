 [BaseContainerProps()]
 class SCR_EditableGroupUIInfo : SCR_EditableEntityUIInfo
 {
     [Attribute()]
     protected ref SCR_MilitarySymbol m_MilitarySymbol;

     protected SCR_MilitarySymbol m_MilitarySymbolInstance; //--- No 'ref'

     //------------------------------------------------------------------------------------------------
  SCR_MilitarySymbol GetMilitarySymbol()
     {
         if (m_MilitarySymbolInstance)
             return m_MilitarySymbolInstance;
         else
             return m_MilitarySymbol;
     }

     //------------------------------------------------------------------------------------------------
  void SetInstance(SCR_MilitarySymbol symbol, LocalizedString name)
     {
         Name = name;
         m_MilitarySymbolInstance = symbol;
     }

     //------------------------------------------------------------------------------------------------
     override string GetName()
     {
         if (!m_MilitarySymbolInstance)
         {
             SCR_GroupIdentityCore core = SCR_GroupIdentityCore.Cast(SCR_GroupIdentityCore.GetInstance(SCR_GroupIdentityCore));
             if (core)
             {
                 SCR_GroupNameConfig nameManager = core.GetNames();
                 if (nameManager)
                 {
                     return nameManager.GetGroupName(GetMilitarySymbol());
                 }
             }
         }
         return super.GetName();
     }

     //------------------------------------------------------------------------------------------------
  override bool GetEntityBudgetCost(out notnull array<ref SCR_EntityBudgetValue> outBudgets)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  bool GetGroupBudgetCost(out notnull array<ref SCR_EntityBudgetValue> outBudgets)
     {
         return super.GetEntityBudgetCost(outBudgets);
     }

     //------------------------------------------------------------------------------------------------
     //--- Override without 'protected' keyword
     override void CopyFrom(SCR_UIName source)
     {
         SCR_EditableGroupUIInfo editableGroupSource = SCR_EditableGroupUIInfo.Cast(source);
         if (editableGroupSource)
             m_MilitarySymbol = editableGroupSource.m_MilitarySymbol;

         super.CopyFrom(source);
     }
 }