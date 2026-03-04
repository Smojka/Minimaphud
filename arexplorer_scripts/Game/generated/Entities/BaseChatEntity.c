 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseChatEntityClass: GenericEntityClass
 {
 }

 class BaseChatEntity: GenericEntity
 {
     proto external int GetChannelsCount();
     proto external string GetChannelName(int channelId);
     proto external BaseChatChannel GetChannel(int channelId);
     proto external PrivateMessageChannel GetWhisperChannel();
     proto external BaseChatChannel GetDefaultChannel();
     proto external BaseChatChannelStyle GetStyle();
 }
