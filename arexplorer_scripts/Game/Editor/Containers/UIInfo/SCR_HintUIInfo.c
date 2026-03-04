 [BaseContainerProps(configRoot: true), SCR_BaseContainerLocalizedTitleField("Name")]
 class SCR_HintUIInfo : SCR_BlockUIInfo
 {
     [Attribute(SCR_Enum.GetDefault(EHint.UNDEFINED), UIWidgets.SearchComboBox, "When defined, the hint can be shown only certain amount of times (defined by Show Limit attribute).\nAfter that, calls to show the hint will be ignored.", enums: ParamEnumArray.FromEnum(EHint))]
     protected EHint m_Type;

     [Attribute("1", desc: "How many times can the hint be shown. Valid only if Type is defined!")]
     protected int m_iShowLimit;

     [Attribute()]
     protected int m_iPriority;

     [Attribute("0", desc: "For how long should the hint be shown.\nWhen 0, default duration from hint manager will be used.\nWhen -1, the hint will be shown until cleared manually.")]
     protected float m_fDuration;

     [Attribute(desc: "Widgets to be highlighted when this hint is displayed")]
     protected ref array<string> m_aHighlightWidgetsNames;

     [Attribute("0", desc: "Should the hint duration be animated.\nWhen false, the duration will not be visualised.")]
     protected bool m_bIsTimerVisible;

     [Attribute(SCR_Enum.GetDefault(EFieldManualEntryId.NONE), UIWidgets.SearchComboBox, enums: ParamEnumArray.FromEnum(EFieldManualEntryId))]
     protected EFieldManualEntryId m_FieldManualLink;

     protected int m_iSequencePage;
     protected int m_iSequenceCount;
     protected WorldTimestamp m_iTimeHintStarted;

     //------------------------------------------------------------------------------------------------
  EHint GetType()
     {
         return m_Type;
     }

     //------------------------------------------------------------------------------------------------
  int GetShowLimit()
     {
         return m_iShowLimit;
     }

     //------------------------------------------------------------------------------------------------
  bool IsTimerVisible()
     {
         return m_bIsTimerVisible;
     }

     //------------------------------------------------------------------------------------------------
  int GetPriority()
     {
         return m_iPriority;
     }

     //------------------------------------------------------------------------------------------------
  float GetDuration()
     {
         return m_fDuration;
     }

     //------------------------------------------------------------------------------------------------
     WorldTimestamp GetTimeStarted()
     {
         return m_iTimeHintStarted;
     }

     //------------------------------------------------------------------------------------------------
  void SetTimeStamp()
     {
         ChimeraWorld world = GetGame().GetWorld();
         m_iTimeHintStarted = world.GetServerTimestamp();
     }

     //------------------------------------------------------------------------------------------------
  int GetHighlightWidgetNames(out notnull array<string> outWidgetNames)
     {
         if (m_aHighlightWidgetsNames)
             return outWidgetNames.Copy(m_aHighlightWidgetsNames);
         else
             return 0;
     }

     //------------------------------------------------------------------------------------------------
  EFieldManualEntryId GetFieldManualLink()
     {
         return m_FieldManualLink;
     }

     //------------------------------------------------------------------------------------------------
  void InitSequence(int page, int count)
     {
         m_iSequencePage = page;
         m_iSequenceCount = count;
     }

     //------------------------------------------------------------------------------------------------
  int GetSequencePage()
     {
         return m_iSequencePage;
     }

     //------------------------------------------------------------------------------------------------
  int GetSequenceCount()
     {
         return m_iSequenceCount;
     }

     //------------------------------------------------------------------------------------------------
  bool IsInSequence()
     {
         return m_iSequenceCount != 0;
     }

     //------------------------------------------------------------------------------------------------
  static SCR_HintUIInfo CreateInfo(string description, string name, float duration, EHint type, EFieldManualEntryId fieldManualEntry, bool isTimerVisible)
     {
         SCR_HintUIInfo info = new SCR_HintUIInfo();
         info.Name = name;
         info.Description = description;
         info.m_fDuration = duration;
         info.m_Type = type;
         info.m_FieldManualLink = fieldManualEntry;
         info.m_bIsTimerVisible = isTimerVisible;
         return info;
     }

     //------------------------------------------------------------------------------------------------
     override bool SetDescriptionTo(TextWidget textWidget)
     {
         if (!textWidget)
             return false;

         Color sRGBA = Color.FromInt(UIColors.CONTRAST_COLOR.PackToInt());

         if (m_aHighlightWidgetsNames && !m_aHighlightWidgetsNames.IsEmpty())
             textWidget.SetTextFormat(GetDescription(), string.Format("color rgba=%1", UIColors.SRGBAFloatToInt(sRGBA)), "/color"); //--- ToDo: Don't hardcode
         else
             textWidget.SetText(GetDescription());
         return true;
     }
 }