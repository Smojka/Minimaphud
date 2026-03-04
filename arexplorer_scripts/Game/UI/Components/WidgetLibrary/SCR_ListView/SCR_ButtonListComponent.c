 /*
 Component for handling button specific list view
 */

 //------------------------------------------------------------------------------------------------
 class SCR_ButtonListViewComponent : SCR_ListViewComponent
 {
     protected ref array<SCR_ButtonComponent> m_aButtons = new array<SCR_ButtonComponent>;

     ref ScriptInvoker m_OnEntryFocus = new ScriptInvoker;

     //-------------------------------------
     // List component override
     //-------------------------------------

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         super.HandlerAttached(w);
     }

     //------------------------------------------------------------------------------------------------
  override protected void SetupEntryBehavior(Widget entry)
     {
         // Entry check
         if (!entry)
             return;

         // Find server entry component
         SCR_ButtonComponent button = SCR_ButtonComponent.Cast(entry.FindHandler(SCR_ButtonComponent));
         if (!button)
             return;

         // Invokers
         button.m_OnFocus.Insert(OnEntryFocus);

         // Insert to list
         m_aButtons.Insert(button);
     }

     //-------------------------------------
     // Invoker actions
     //-------------------------------------

     //------------------------------------------------------------------------------------------------
  protected void OnEntryFocus(Widget w)
     {
         m_OnEntryFocus.Invoke();

         m_iFocusedEntryId = m_aEntryWidgets.Find(w) + Math.Floor(m_fScrollPosition);
     }

     //-------------------------------------
     // Get & Set
     //-------------------------------------

     //------------------------------------------------------------------------------------------------
     array<SCR_ButtonComponent> GetButtons() { return m_aButtons; }
 };