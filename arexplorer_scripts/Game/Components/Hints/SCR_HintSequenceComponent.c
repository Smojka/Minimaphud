 [ComponentEditorProps(category: "GameScripted/Misc", description: "")]
 class SCR_HintSequenceComponentClass : ScriptComponentClass
 {
     [Attribute()]
     protected ref SCR_HintSequenceList m_Sequence;

     //------------------------------------------------------------------------------------------------
  SCR_HintSequenceList GetSequence()
     {
         return m_Sequence;
     }
 }

 class SCR_HintSequenceComponent : ScriptComponent
 {
     protected bool m_bIsActive;
     protected ref ScriptInvoker m_OnSequenceChange = new ScriptInvoker;

     //--- Public functions

     //------------------------------------------------------------------------------------------------
  bool StartSequence()
     {
         SCR_HintSequenceComponentClass prefabData = SCR_HintSequenceComponentClass.Cast(GetComponentData(GetOwner()));
         if (!prefabData.GetSequence())
         {
             Debug.Error("No sequence defined!");
             return false;
         }

         SCR_HintManagerComponent hintManager = SCR_HintManagerComponent.GetInstance();
         if (!hintManager.Show(prefabData.GetSequence().GetHint(0), ignoreShown: true))
             return false;

         m_bIsActive = true;

         hintManager.GetOnHintShow().Insert(OnHintShow);
         hintManager.GetOnHintHide().Insert(OnHintHide);
         m_OnSequenceChange.Invoke(true);
         return true;
     }

     //------------------------------------------------------------------------------------------------
  void StopSequence()
     {
         SCR_HintSequenceComponentClass prefabData = SCR_HintSequenceComponentClass.Cast(GetComponentData(GetOwner()));
         SCR_HintManagerComponent hintManager = SCR_HintManagerComponent.GetInstance();
         if (hintManager && prefabData.GetSequence().FindHint(hintManager.GetCurrentHint()) != -1)
         {
             Clear();
             hintManager.Hide();
         }
     }

     //------------------------------------------------------------------------------------------------
  void ToggleSequence()
     {
         if (IsSequenceActive())
             StopSequence();
         else
             StartSequence();
     }

     //------------------------------------------------------------------------------------------------
  bool IsSequenceActive()
     {
         return m_bIsActive;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnSequenceChange()
     {
         return m_OnSequenceChange;
     }

     //--- Protected functions

     //------------------------------------------------------------------------------------------------
     protected void OnHintShow(SCR_HintUIInfo info, bool isSilent)
     {
         //--- Interrupted by unrelated hint
         SCR_HintSequenceComponentClass prefabData = SCR_HintSequenceComponentClass.Cast(GetComponentData(GetOwner()));
         if (prefabData.GetSequence().FindHint(info) == -1)
             Clear();
     }

     //------------------------------------------------------------------------------------------------
     protected void OnHintHide(SCR_HintUIInfo info, bool isSilent)
     {
         SCR_HintSequenceComponentClass prefabData = SCR_HintSequenceComponentClass.Cast(GetComponentData(GetOwner()));
         int index = prefabData.GetSequence().FindHint(info);
         if (index >= 0 && index < prefabData.GetSequence().CountHints() - 1)
         {
             SCR_HintManagerComponent.ShowHint(prefabData.GetSequence().GetHint(index + 1), ignoreShown: true);
         }
         else
         {
             //--- Last hint, terminate
             Clear();
         }
     }

     //------------------------------------------------------------------------------------------------
     protected void Clear()
     {
         m_bIsActive = false;

         SCR_HintManagerComponent hintManager = SCR_HintManagerComponent.GetInstance();
         hintManager.GetOnHintShow().Remove(OnHintShow);
         hintManager.GetOnHintHide().Remove(OnHintHide);

         m_OnSequenceChange.Invoke(false);
     }
 }