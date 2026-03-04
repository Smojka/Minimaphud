 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseSelectionMenuEntry: ScriptAndConfig
 {
     proto external bool CanBeShown(IEntity user, BaseSelectionMenu sourceMenu);
     proto external bool CanBePerformed(IEntity user, BaseSelectionMenu sourceMenu);
     proto external void Perform(IEntity user, BaseSelectionMenu sourceMenu);
     proto external string GetEntryName();
     proto external string GetEntryDescription();
     proto external string GetEntryIconPath();
     proto external UIInfo GetUIInfo();
 }
