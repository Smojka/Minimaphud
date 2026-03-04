 class SCR_ConfirmBasesSignalsWidgetComponent : ScriptedWidgetComponent
 {
 #ifdef ENABLE_CAMPAIGN_CONFIGURATOR
     //------------------------------------------------------------------------------------------------
  override bool OnClick(Widget w, int x, int y, int button)
     {
         //SCR_MilitaryBaseManager.GetInstance().UpdateBasesSettings();

         return false;
     }
 #endif
 }