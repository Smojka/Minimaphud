 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseChatComponentClass: GameComponentClass
 {
 }

 class BaseChatComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external void SetChannel(int channelId, bool enabled);
     proto external bool GetChannelState(int channelId);
     proto external void SendMessage(string msgStr, int channelId);
     proto external void SendPrivateMessage(string msgStr, int receiverId);

     // callbacks

     event protected void OnNewMessage(string msg, int channelId, int senderId);
     event protected void OnNewPrivateMessage(string msg, int senderId, int receiverId);
     event protected void ShowMessage(string msg);
 }
