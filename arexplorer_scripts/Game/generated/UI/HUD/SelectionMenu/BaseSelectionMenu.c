 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseSelectionMenu: ScriptAndConfig
 {
     proto external void Update(IEntity owner, float timeSlice);
     proto external void Open(IEntity owner);
     proto external void Close(IEntity owner);
     proto external bool IsOpen();
     proto external void ClearEntries();
     proto external void AddEntry(BaseSelectionMenuEntry entry);
     proto external void RemoveEntry(BaseSelectionMenuEntry entry);
     proto external bool ContainsEntry(BaseSelectionMenuEntry entry);
     proto external int GetEntriesCount();
     proto external int GetEntryList(out notnull array<BaseSelectionMenuEntry> outActions);
 }
