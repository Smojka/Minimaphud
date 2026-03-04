 [BaseContainerProps(), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_VotingUIInfo : SCR_UIInfo
 {
     [Attribute(desc: "Text used when starting a vote about something.")]
     protected LocalizedString m_sStartVotingName;

     [Attribute("", desc: "Text used when canceling your vote about something.")]
     protected LocalizedString m_sCancelVotingName;

     [Attribute("", desc: "Text used when currently abstaining but want to revote to yes. Leave empty to use normal vote string")]
     protected LocalizedString m_sRevoteName;

     [Attribute("", desc: "When a vote is in progress and the player has not yet voted or abstained.")]
     protected LocalizedString m_sAbstainVoteName;

     [Attribute(desc: "Text shown consistently when the vote is active. %1 is player that is voted for")]
     protected LocalizedString m_sStickyNotificationText;

     [Attribute(ENotification.UNKNOWN.ToString(), desc: "The Start notification id to which the voting type is linked. Leave UNKNOWN to not have a notification", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ENotification))]
     protected ENotification m_iStartNotificationId;

     [Attribute(ENotification.UNKNOWN.ToString(), desc: "The Succeed notification id to which the voting type is linked. Leave UNKNOWN to not have a notification", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ENotification))]
     protected ENotification m_iSucceededNotificationId;

     [Attribute(ENotification.UNKNOWN.ToString(), desc: "The Fail notification id to which the voting type is linked. Leave UNKNOWN to not have a notification", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ENotification))]
     protected ENotification m_iFailedNotificationId;

     [Attribute(ENotification.UNKNOWN.ToString(), desc: "Local notification send when player casts vote. Leave UNKNOWN to not have a notification", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ENotification))]
     protected ENotification m_eVoteCastedLocalNotification;

     [Attribute(ENotification.UNKNOWN.ToString(), desc: "Local notification send when player abstains from voting. Leave UNKNOWN to not have a notification", uiwidget: UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(ENotification))]
     protected ENotification m_eVoteAbstainedLocalNotification;

     //------------------------------------------------------------------------------------------------
  string GetStartVotingName()
     {
         if (!SCR_StringHelper.IsEmptyOrWhiteSpace(m_sStartVotingName))
             return m_sStartVotingName;
         else
             return "ERROR MISSING START VOTE IN UIINFO";
     }

     //------------------------------------------------------------------------------------------------
  string GetCancelVotingName()
     {
         if (!SCR_StringHelper.IsEmptyOrWhiteSpace(m_sCancelVotingName))
             return m_sCancelVotingName;
         else
             return "ERROR MISSING CANCEL VOTE IN UIINFO";
     }

     //------------------------------------------------------------------------------------------------
  string GetRevoteName()
     {
         if (!SCR_StringHelper.IsEmptyOrWhiteSpace(m_sRevoteName))
             return m_sRevoteName;
         else
             return GetName();
     }

     //------------------------------------------------------------------------------------------------
  string GetAbstainVoteName()
     {
         if (!SCR_StringHelper.IsEmptyOrWhiteSpace(m_sAbstainVoteName))
             return m_sAbstainVoteName;
         else
             return "ERROR MISSING ABSTAIN VOTE IN UIINFO";
     }

     //------------------------------------------------------------------------------------------------
  string GetStickyNotificationText()
     {
         return m_sStickyNotificationText;
     }

     //------------------------------------------------------------------------------------------------
  ENotification GetVotingStartNotification()
     {
         return m_iStartNotificationId;
     }

     //------------------------------------------------------------------------------------------------
  ENotification GetVotingSucceedNotification()
     {
         return m_iSucceededNotificationId;
     }

     //------------------------------------------------------------------------------------------------
  ENotification GetVotingFailNotification()
     {
         return m_iFailedNotificationId;
     }

     //------------------------------------------------------------------------------------------------
  ENotification GetLocalVoteCastNotification()
     {
         return m_eVoteCastedLocalNotification;
     }

     //------------------------------------------------------------------------------------------------
  ENotification GetLocalVoteAbstainedNotification()
     {
         return m_eVoteAbstainedLocalNotification;
     }
 }