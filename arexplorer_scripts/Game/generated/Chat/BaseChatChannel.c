 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseChatChannel: ScriptAndConfig
 {
     proto external string GetName();
     proto external BaseChatChannelStyle GetStyle();

     // callbacks

     event protected void OnInit (IEntity owner);
     event bool IsDelivering(BaseChatComponent sender, BaseChatComponent receiver) { return true; };
     event bool IsAvailable(BaseChatComponent sender) { return true; };
     event bool ProcessMessage(BaseChatComponent sender, string message, bool isAuthority) { return true; };
 }
