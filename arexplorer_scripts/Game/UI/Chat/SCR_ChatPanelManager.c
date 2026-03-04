 // Type definition - callbacks of chat commands
 typedef func ChatCommandCallback;
 void ChatCommandCallback(SCR_ChatPanel panel, string data);
 typedef ScriptInvokerBase<ChatCommandCallback> ChatCommandInvoker;


 [BaseContainerProps(configRoot: true)]
 class SCR_ChatPanelManager : SCR_GameCoreBase
 {
     // Constants

     // Size of chat history. When more messages than this are added, old messages are deleted.
     protected const int CHAT_HISTORY_SIZE = 256;

     // Start character of each command
     // note: admin commands are different and are not handled by this chat UI system. They start with a different character.
     const string CHAT_COMMAND_CHARACTER = "/";


     protected bool m_bInitDone = false;

     protected ref array<ref SCR_ChatMessage> m_aMessages = {};
     protected ref array<SCR_ChatPanel> m_aChatPanels = {};

     // Registered chat commands
     protected ref map<string, ref ChatCommandInvoker> m_mCommands = new map<string, ref ScriptInvokerBase<ChatCommandCallback>>;

     ScriptedChatEntity m_ChatEntity;

     //------------------------------------------------------------------------------------------------
     // PUBLIC


     //------------------------------------------------------------------------------------------------
  void OpenChatPanel(notnull SCR_ChatPanel panel)
     {
         // Ensure all other panels are closed
         foreach (SCR_ChatPanel p : m_aChatPanels)
         {
             if (p != panel)
                 p.Internal_Close();
         }

         panel.Internal_Open();
     }


     //------------------------------------------------------------------------------------------------
  void CloseChatPanel(notnull SCR_ChatPanel panel)
     {
         // To be safe, just close all of them
         CloseAllChatPanels();
     }


     //------------------------------------------------------------------------------------------------
  void ToggleChatPanel(notnull SCR_ChatPanel panel)
     {
         // Ensure all other panels are closed
         foreach (SCR_ChatPanel p : m_aChatPanels)
         {
             if (p != panel)
                 p.Internal_Close();
         }

         if (panel.IsOpen())
             panel.Internal_Close();
         else
             panel.Internal_Open();
     }


     //------------------------------------------------------------------------------------------------
     void OnMenuClosed()
     {
         MenuManager menuManager = GetGame().GetMenuManager();

         if (!menuManager || !menuManager.IsAnyMenuOpen())
             ShowAllChatPanels();
     }

     //------------------------------------------------------------------------------------------------
     void ShowAllChatPanels()
     {
         foreach (SCR_ChatPanel panel : m_aChatPanels)
         {
             panel.GetWidget().SetVisible(true);
         }
     }

     //------------------------------------------------------------------------------------------------
     void ShowChatPanel(notnull SCR_ChatPanel panel)
     {
         panel.GetWidget().SetVisible(true);
     }

     //------------------------------------------------------------------------------------------------
     void HideAllChatPanels()
     {
         foreach (SCR_ChatPanel panel : m_aChatPanels)
         {
             panel.GetWidget().SetVisible(false);
         }
     }

     //------------------------------------------------------------------------------------------------
     void CloseAllChatPanels()
     {
         foreach (SCR_ChatPanel p : m_aChatPanels)
         {
             if (p.IsOpen())
                 p.Internal_Close();
         }
     }


     //------------------------------------------------------------------------------------------------
  static SCR_ChatPanelManager GetInstance()
     {
         return SCR_ChatPanelManager.Cast(SCR_GameCoresManager.GetCore(SCR_ChatPanelManager));
     }


     //------------------------------------------------------------------------------------------------
     array<ref SCR_ChatMessage> GetMessages()
     {
         return m_aMessages;
     }



     //------------------------------------------------------------------------------------------------
     array<string> GetAllRegisteredCommands()
     {
         array<string> a = {};
         foreach (string comm, auto invoker : m_mCommands)
             a.Insert(comm);
         return a;
     }


     //------------------------------------------------------------------------------------------------
     void ShowHelpMessage(string msg)
     {
         ref SCR_ChatMessage m = new SCR_ChatMessage(msg);

         this.OnNewMessage(m);
     }


     //------------------------------------------------------------------------------------------------
  ChatCommandInvoker GetCommandInvoker(string name)
     {
         // Bail if name is incorrect
         if (name.IsEmpty())
             return null;

         ChatCommandInvoker invoker = m_mCommands.Get(name);

         if (!invoker)
         {
             invoker = new ChatCommandInvoker;
             m_mCommands.Insert(name, invoker);
         }

         // Verify
         if (!m_mCommands.Get(name))
             Print(string.Format("Error while registering chat command: %1", name), LogLevel.WARNING);

         return invoker;
     }


     //------------------------------------------------------------------------------------------------
  bool GetAnyPanelFadedIn()
     {
         foreach (SCR_ChatPanel panel : m_aChatPanels)
         {
             if (panel.GetFadeIn())
                 return true;
         }

         return false;
     }


     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
     void Register(SCR_ChatPanel panel)
     {
         if (!m_aChatPanels.Contains(panel))
             m_aChatPanels.Insert(panel);

         CloseAllChatPanels();
     }

     //------------------------------------------------------------------------------------------------
     void Unregister(SCR_ChatPanel panel)
     {
         m_aChatPanels.RemoveItem(panel);
     }




     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
     void OnNewMessagePrivate(string msg, int senderId, int receiverId)
     {
         if (!m_ChatEntity)
             return;

         int playerId = GetGame().GetPlayerController().GetPlayerId();

         if (! (senderId == playerId || receiverId == playerId))
             return;

         SCR_ChatMessagePrivate m = new SCR_ChatMessagePrivate(
             msg,
             m_ChatEntity.GetWhisperChannel(),
             senderId,
             GetGame().GetPlayerManager().GetPlayerName(senderId),
             receiverId);

         OnNewMessage(m);
     }

     //------------------------------------------------------------------------------------------------
     void OnNewMessageRadioProtocol(string msg, int frequency)
     {
         SCR_ChatMessageRadioProtocol m = new SCR_ChatMessageRadioProtocol(
             msg,
             frequency);

         this.OnNewMessage(m);
     }

     //------------------------------------------------------------------------------------------------
     void OnNewMessageGeneral(string msg, int channelId, int senderId)
     {
         SCR_ChatMessageGeneral m = new SCR_ChatMessageGeneral(
             msg,
             m_ChatEntity.GetChannel(channelId),
             senderId,
             GetGame().GetPlayerManager().GetPlayerName(senderId));

         this.OnNewMessage(m);
     }

     //------------------------------------------------------------------------------------------------
     void OnNewMessage(string msg)
     {
         SCR_ChatMessage m = new SCR_ChatMessage(msg);

         this.OnNewMessage(m);
     }




     //------------------------------------------------------------------------------------------------


     //------------------------------------------------------------------------------------------------
     override void OnGameStart()
     {
         // Clear message array so we don't see messages from previous session
         m_aMessages.Clear();

         m_ChatEntity = ScriptedChatEntity.Cast(GetGame().GetChat());

         // OnGameStart runs several time for same object,
         // but this code must be run only once
         if (!m_bInitDone)
         {
             InitDefaultChatCommands();
             m_bInitDone = true;
         }
     }

     //------------------------------------------------------------------------------------------------
     override void OnGameEnd()
     {
         m_aMessages.Clear();
     }

     //------------------------------------------------------------------------------------------------
     protected void InitDefaultChatCommands()
     {
     }

     //------------------------------------------------------------------------------------------------
  void Internal_EnableAllChannels()
     {
         PlayerController pc = GetGame().GetPlayerController();

         if (!pc || !m_ChatEntity)
             return;

         BaseChatComponent chatComp = BaseChatComponent.Cast(pc.FindComponent(BaseChatComponent));

         if (!chatComp)
             return;

         for (int i = 0; i< m_ChatEntity.GetChannelsCount() ; i++)
         {
             BaseChatChannel chatChannel = m_ChatEntity.GetChannel(i);
             chatComp.SetChannel(i, true);
         }
     }


     //------------------------------------------------------------------------------------------------

     //------------------------------------------------------------------------------------------------
  void Internal_OnChatCommand(SCR_ChatPanel panel, string command, string otherData)
     {
         auto invoker = m_mCommands.Get(command);

         if (!invoker)
             return;

         invoker.Invoke(panel, otherData);
     }



     //------------------------------------------------------------------------------------------------
     // PROTECTED


     //------------------------------------------------------------------------------------------------
     protected void OnNewMessage(SCR_ChatMessage msg)
     {
         m_aMessages.Insert(msg);

         if (m_aMessages.Count() > CHAT_HISTORY_SIZE)
             m_aMessages.RemoveOrdered(0);

         foreach (SCR_ChatPanel panel : m_aChatPanels)
             panel.Internal_OnNewMessage(msg);
     }
 };