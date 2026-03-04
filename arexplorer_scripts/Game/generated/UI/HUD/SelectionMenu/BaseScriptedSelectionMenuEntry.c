 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseScriptedSelectionMenuEntry: BaseSelectionMenuEntry
 {
     event bool CanBeShownScript(IEntity user, BaseSelectionMenu sourceMenu);
     event bool CanBePerformedScript(IEntity user, BaseSelectionMenu sourceMenu);
     event void OnPerform(IEntity user, BaseSelectionMenu sourceMenu);
     event bool GetEntryNameScript(out string outName);
     event bool GetEntryDescriptionScript(out string outDescription);
     event bool GetEntryIconPathScript(out string outIconPath);
     event UIInfo GetUIInfoScript();
 }
