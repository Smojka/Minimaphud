 [BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
 class SCR_BaseEditorAttribute
 {
     [Attribute()]
     protected ref SCR_EditorAttributeUIInfo m_UIInfo;

     [Attribute(defvalue: "1", desc: "True to get the value from server.")]
     private bool m_bIsServer;

     [Attribute(params: "conf")]
     protected ResourceName m_CategoryConfig;

     [Attribute(params: "layout")]
     protected ResourceName m_Layout;

     [Attribute(desc: "Holds a list of dynamic descriptions from attributes. When hovered and when changing the attribute the system will loop through all entries and display the dynamic description for the first valid. If none are valid than the default description is shown")]
     protected ref array<ref SCR_BaseAttributeDynamicDescription> m_aAttributeDynamicDescriptions;

     //State
     protected bool m_bAttributeEnabled = true;
     protected bool m_bIsSubAttribute;
     protected bool m_bInitCalled;
     protected bool m_bConflictingAttributeWasReset;

     //Conflicting attributes
     protected bool m_bIsMultiSelect = false;
     protected bool m_bHasConflictingValues = false;
     protected bool m_bOverridingValues = false;

     //Vars
     protected ref SCR_BaseEditorAttributeVar m_Var;
     protected ref SCR_BaseEditorAttributeVar m_CopyVar; //Used when has conflicting values. m_Var is deleted and a copy is saved if the var is disabled

     protected ref SSnapshot m_Snapshot;
     protected ref ScriptInvoker Event_OnExternalChange;
     protected ref ScriptInvoker Event_OnVarChanged = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnToggleEnable = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnToggleButtonSelected = new ScriptInvoker;
     protected ref ScriptInvoker Event_OnSetAsSubAttribute = new ScriptInvoker;

     //------------------------------------------------------------------------------------------------
  void ResetAttribute()
     {
         SetInitCalled(false);
         ClearCopyVar();
         SetIsOverridingValues(false);
         SetConflictingAttributeWasReset(false);
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditorAttributeUIInfo GetUIInfo()
     {
         return m_UIInfo;
     }

     //------------------------------------------------------------------------------------------------
  int GetDynamicDescriptionArray(notnull out array<SCR_BaseAttributeDynamicDescription> dynamicDescriptionArray)
     {
         foreach (SCR_BaseAttributeDynamicDescription description : m_aAttributeDynamicDescriptions)
         {
             dynamicDescriptionArray.Insert(description);
         }

         return dynamicDescriptionArray.Count();
     }

     //------------------------------------------------------------------------------------------------
  bool IsServer()
     {
         return m_bIsServer;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetCategoryConfig()
     {
         return m_CategoryConfig;
     }

     //------------------------------------------------------------------------------------------------
     ResourceName GetLayout()
     {
         return m_Layout;
     }

     //------------------------------------------------------------------------------------------------
  bool BoolAllowDuplicate()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool IsSerializable()
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
     sealed bool SetVariable(SCR_BaseEditorAttributeVar var)
     {
         if (!var)
             return false;

         if (m_Var)
         {
             if (var.Type() == m_Var.Type())
             {
                 m_Var = var;
                 Event_OnVarChanged.Invoke(var);
                 return true;
             }
             else
             {
                 PrintFormat("Trying to set attribute var to type '%1' but is type '%2'", var.Type().ToString(), m_Var.Type().ToString(), LogLevel.WARNING);
                 return false;
             }
         }
         else
         {
             m_Var = var;
             Event_OnVarChanged.Invoke(var);
             return true;
         }
     }

     //------------------------------------------------------------------------------------------------
     sealed void ClearVar()
     {
         m_Var = null;
     }

     //------------------------------------------------------------------------------------------------
     sealed void ClearCopyVar()
     {
         m_CopyVar = null;
     }

     //------------------------------------------------------------------------------------------------
     sealed ScriptInvoker GetOnVarChanged()
     {
         return Event_OnVarChanged;
     }

     //------------------------------------------------------------------------------------------------
     sealed void Enable(bool enabled)
     {
         m_bAttributeEnabled = enabled;
         Event_OnToggleEnable.Invoke(enabled);
     }

     //------------------------------------------------------------------------------------------------
     sealed void ToggleSelected(bool selected, int index)
     {
         Event_OnToggleButtonSelected.Invoke(selected, index, true);
     }

     //------------------------------------------------------------------------------------------------
     sealed void SetAsSubAttribute()
     {
         m_bIsSubAttribute = true;
         Event_OnSetAsSubAttribute.Invoke();
     }

     //------------------------------------------------------------------------------------------------
  bool GetInitCalled()
     {
         return m_bInitCalled;
     }

     //------------------------------------------------------------------------------------------------
  void SetInitCalled(bool initCalled)
     {
         m_bInitCalled = initCalled;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsMultiSelect()
     {
         return m_bIsMultiSelect;
     }

     //------------------------------------------------------------------------------------------------
  void SetIsMultiSelect(bool isMultiSelect)
     {
         m_bIsMultiSelect = isMultiSelect;
     }

     //------------------------------------------------------------------------------------------------
  bool GetHasConflictingValues()
     {
         return m_bHasConflictingValues;
     }

     //------------------------------------------------------------------------------------------------
  void SetHasConflictingValues(bool hasConflictingValues)
     {
         m_bHasConflictingValues = hasConflictingValues;
     }

     //------------------------------------------------------------------------------------------------
  void SetIsOverridingValues(bool isOverridingValues)
     {
         m_bOverridingValues = isOverridingValues;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsOverridingValues()
     {
         return m_bOverridingValues;
     }

     //------------------------------------------------------------------------------------------------
  bool IsEnabled()
     {
         return m_bAttributeEnabled;
     }

     //------------------------------------------------------------------------------------------------
  bool GetIsSubAttribute()
     {
         return m_bIsSubAttribute;
     }

     //------------------------------------------------------------------------------------------------
     sealed ScriptInvoker GetOnToggleEnable()
     {
         return Event_OnToggleEnable;
     }

     //------------------------------------------------------------------------------------------------
     sealed ScriptInvoker GetOnToggleButtonSelected()
     {
         return Event_OnToggleButtonSelected;
     }

     //------------------------------------------------------------------------------------------------
     sealed ScriptInvoker GetOnSetAsSubAttribute()
     {
         return Event_OnSetAsSubAttribute;
     }

     //------------------------------------------------------------------------------------------------
     sealed SCR_BaseEditorAttributeVar GetVariable(bool createWhenNull = false)
     {
         if (!m_Var && createWhenNull)
             m_Var = CreateDefaultVariable();

         return m_Var;
     }

     //------------------------------------------------------------------------------------------------
     sealed SCR_BaseEditorAttributeVar GetCopyVariable()
     {
         return m_CopyVar;
     }

     //------------------------------------------------------------------------------------------------
     sealed SCR_BaseEditorAttributeVar GetVariableOrCopy()
     {
         if (!m_Var)
             return m_CopyVar;

         return m_Var;
     }

     //------------------------------------------------------------------------------------------------
     sealed void CreateCopyVariable()
     {
         if (m_Var)
             m_CopyVar = SCR_BaseEditorAttributeVar.CreateVector(m_Var.GetVector());
         else
             m_CopyVar = null;
     }

     //------------------------------------------------------------------------------------------------
     sealed bool GetConflictingAttributeWasReset()
     {
         return m_bConflictingAttributeWasReset;
     }

     //------------------------------------------------------------------------------------------------
     sealed void SetConflictingAttributeWasReset(bool wasReset)
     {
         m_bConflictingAttributeWasReset = wasReset;
     }

     //------------------------------------------------------------------------------------------------
     sealed SSnapshot GetSnapshot()
     {
         return m_Snapshot;
     }

     //------------------------------------------------------------------------------------------------
     sealed ScriptInvoker GetOnExternalnChange()
     {
         return Event_OnExternalChange;
     }

     //------------------------------------------------------------------------------------------------
     sealed void StartEditing(SCR_BaseEditorAttributeVar var, SSnapshot snapshot)
     {
         m_Var = var;
         m_Snapshot = snapshot;
         Event_OnExternalChange = new ScriptInvoker;
     }

     //------------------------------------------------------------------------------------------------
     sealed void StopEditing()
     {
         m_Var = null;
         m_Snapshot = null;
         Event_OnExternalChange = null;

         //Stop preview
         PreviewVariable(false, null);
     }

     //------------------------------------------------------------------------------------------------
     sealed void TelegraphChange(bool isReset)
     {
         //If has conflicting values on reset
         if (isReset)
         {
             if (GetHasConflictingValues())
             {
                 SetConflictingAttributeWasReset(true);
                 SetIsOverridingValues(false);
             }
         }

         Event_OnExternalChange.Invoke(m_Var, isReset);
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_BaseEditorAttributeVar CreateDefaultVariable()
     {
         return SCR_BaseEditorAttributeVar.CreateBool(false);
     }

     //------------------------------------------------------------------------------------------------
  SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager);

     //------------------------------------------------------------------------------------------------
  void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID);

     //------------------------------------------------------------------------------------------------
  void PreviewVariable(bool setPreview, SCR_AttributesManagerEditorComponent manager);

     //------------------------------------------------------------------------------------------------
  void UpdateInterlinkedVariables(SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, bool isInit = false);

     //------------------------------------------------------------------------------------------------
  int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
     {
         return 0;
     }

     //------------------------------------------------------------------------------------------------
  protected bool IsGameMode(Managed item)
     {
         return item == GetGame().GetGameMode();
     }
 }

 class SCR_BaseEditorAttributeEntry
 {
 }

 class SCR_BaseEditorAttributeEntryText: SCR_BaseEditorAttributeEntry
 {
     private string m_sText;

     //------------------------------------------------------------------------------------------------
     string GetText()
     {
         return m_sText;
     }

     //------------------------------------------------------------------------------------------------
     void SCR_BaseEditorAttributeEntryText(string text)
     {
         m_sText = text;
     }
 }

 class SCR_BaseEditorAttributeEntryUIInfo : SCR_BaseEditorAttributeEntry
 {
     private ref SCR_UIInfo m_Info;
     private int m_iValue;

     //------------------------------------------------------------------------------------------------
     SCR_UIInfo GetInfo()
     {
         return m_Info;
     }

     //------------------------------------------------------------------------------------------------
     int GetValue()
     {
         return m_iValue;
     }

     //------------------------------------------------------------------------------------------------
     void SCR_BaseEditorAttributeEntryUIInfo(SCR_UIInfo info, int value = 0)
     {
         m_Info = info;
         m_iValue = value;
     }
 }

 class SCR_BaseEditorAttributeEntrySlider: SCR_BaseEditorAttributeEntry
 {
     protected float m_fMin;
     protected float m_fMax;
     protected float m_fStep;
     protected int m_sDecimals;
     protected string m_sSliderValueFormating;
     protected string m_sSliderLabel;

     //------------------------------------------------------------------------------------------------
  void GetSliderMinMaxStep(out float min, out float max, out float step)
     {
         min = m_fMin;
         max = m_fMax;
         step = m_fStep;
     }

     //------------------------------------------------------------------------------------------------
     void SetSliderLabel(string label)
     {
         m_sSliderLabel = label;
     }

     //------------------------------------------------------------------------------------------------
     string GetSliderlabel()
     {
          return m_sSliderLabel;
     }

     //------------------------------------------------------------------------------------------------
  string GetText(float value)
     {
         //--- ToDo: Use native fixed length conversion once it's implemented
         float coef = Math.Pow(10, m_sDecimals);
         value = Math.Round(value * coef);
         string valueText = value.ToString();
         if (m_sDecimals > 0)
         {
             for (int i = 0, count = m_sDecimals - valueText.Length() + 1; i < count; i++)
             {
                 valueText = "0" + valueText;
             }
             int length = valueText.Length();
             valueText = valueText.Substring(0, length - m_sDecimals) + "." + valueText.Substring(length - m_sDecimals, m_sDecimals);
         }

         return valueText;
     }

     //------------------------------------------------------------------------------------------------
  // Formatt*ing
     string GetSliderValueFormating()
     {
         return m_sSliderValueFormating;
     }

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_BaseEditorAttributeEntrySlider(SCR_EditorAttributeBaseValues sliderValues)
     {
         string sliderValueSymbol;
         string sliderValueFormating;
         float min, max, step, decimals;

         sliderValues.GetSliderValues(sliderValueFormating, min, max, step, decimals);

         m_fMin = min;
         m_fMax = max;
         m_fStep = step;
         m_sDecimals = decimals;
         m_sSliderValueFormating = sliderValueFormating;
     }
 }

 class SCR_BaseEditorAttributeEntryTimeSlider: SCR_BaseEditorAttributeEntry
 {
     protected ETimeFormatParam m_eHideIfZero;
     protected bool m_bAlwaysHideSeconds;

     //------------------------------------------------------------------------------------------------
  void GetTimeSliderValues(out ETimeFormatParam hideIfZero, out bool alwaysHideSeconds)
     {
         hideIfZero = m_eHideIfZero;
         alwaysHideSeconds = m_bAlwaysHideSeconds;
     }

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_BaseEditorAttributeEntryTimeSlider(ETimeFormatParam hideIfZero, bool alwaysHideSeconds)
     {
         m_eHideIfZero = hideIfZero;
         m_bAlwaysHideSeconds = alwaysHideSeconds;
     }
 }

 class SCR_BaseEditorAttributeFloatStringValues : SCR_BaseEditorAttributeEntry
 {
     protected array<ref SCR_EditorAttributeFloatStringValueHolder> m_aValues;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_BaseEditorAttributeFloatStringValues(array<ref SCR_EditorAttributeFloatStringValueHolder> values)
     {
         m_aValues = values;
     }

     //------------------------------------------------------------------------------------------------
  int GetValueCount()
     {
         return m_aValues.Count();
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditorAttributeFloatStringValueHolder GetValuesEntry(int index)
     {
         if (!m_aValues.IsIndexValid(index))
             return null;

         return m_aValues[index];
     }

     //------------------------------------------------------------------------------------------------
  float GetEntryFloatValue(int index)
     {
         if (!m_aValues.IsIndexValid(index))
             return 0;

         return m_aValues[index].GetFloatValue();
     }
 }

 class SCR_EditorAttributeEntryBool : SCR_BaseEditorAttributeEntry
 {
     protected bool m_bBool;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_EditorAttributeEntryBool(bool newBool)
     {
         SetBool(newBool);
     }

     //------------------------------------------------------------------------------------------------
     void SetBool(bool newBool)
     {
         m_bBool = newBool;
     }

     //------------------------------------------------------------------------------------------------
     bool GetBool()
     {
         return m_bBool;
     }
 }

 class SCR_EditorAttributeEntryStringArray : SCR_BaseEditorAttributeEntry
 {
     protected array<ref LocalizedString> m_aValues;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_EditorAttributeEntryStringArray(array<ref LocalizedString> values)
     {
         m_aValues = values;
     }

     //------------------------------------------------------------------------------------------------
     int GetCount()
     {
         return m_aValues.Count();
     }

     //------------------------------------------------------------------------------------------------
  string GetEntry(int index)
     {
         return m_aValues[index];
     }
 }

 class SCR_EditorAttributeEntryIntArray : SCR_BaseEditorAttributeEntry
 {
     protected array<ref int> m_aValues;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_EditorAttributeEntryIntArray(array<ref int> values)
     {
         m_aValues = values;
     }

     //------------------------------------------------------------------------------------------------
     int GetCount()
     {
         return m_aValues.Count();
     }

     //------------------------------------------------------------------------------------------------
     int GetEntry(int index)
     {
         if (index < 0 || index >= m_aValues.Count())
             return 0;

         return m_aValues[index];
     }
 }

 class SCR_EditorAttributeEntryInt : SCR_BaseEditorAttributeEntry
 {
     protected int m_iValue;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_EditorAttributeEntryInt(int value)
     {
         m_iValue = value;
     }

     //------------------------------------------------------------------------------------------------
     int GetInt()
     {
         return m_iValue;
     }
 }


 class SCR_EditorAttributePresetEntry : SCR_BaseEditorAttributeEntry
 {
     protected int m_iButtonsOnRow;
     protected bool m_bHasRandomizeButton;
     protected ResourceName m_sIconOfRandomizeButton;
     protected bool m_bHasIcon;
     protected bool m_bHasButtonDescription;
     protected string m_sButtonDescription;
     protected float m_iButtonHeight;

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_EditorAttributePresetEntry(int buttonsOnRow, bool hasRandomizeButton, ResourceName iconOfRandomizeButton = string.Empty, bool hasIcon = false, bool hasButtonDescription = false, string buttonDescription = string.Empty, int buttonHeight = -1)
     {
         m_iButtonsOnRow = buttonsOnRow;
         m_bHasRandomizeButton = hasRandomizeButton;
         m_sIconOfRandomizeButton = iconOfRandomizeButton;
         m_bHasIcon = hasIcon;
         m_bHasButtonDescription = hasButtonDescription;
         m_sButtonDescription = buttonDescription;
         m_iButtonHeight = buttonHeight;

     }

     //------------------------------------------------------------------------------------------------
     void GetPresetValues(out int buttonsOnRow, out bool hasRandomizeButton, out ResourceName iconOfRandomizeButton, out bool hasIcon, out bool hasButtonDescription, out string buttonDescription, out int buttonHeight)
     {
         buttonsOnRow = m_iButtonsOnRow;
         hasRandomizeButton = m_bHasRandomizeButton;
         iconOfRandomizeButton = m_sIconOfRandomizeButton;
         hasIcon = m_bHasIcon;
         buttonHeight = m_iButtonHeight;
         buttonDescription = m_sButtonDescription;
         hasButtonDescription = m_bHasButtonDescription;
     }
 }

 class SCR_EditorAttributePresetMultiSelectEntry : SCR_EditorAttributePresetEntry
 {
     protected bool m_bCustomFlags;

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_EditorAttributePresetMultiSelectEntry(int buttonsOnRow, bool hasRandomizeButton, ResourceName iconOfRandomizeButton = string.Empty, bool hasIcon = false, bool hasButtonDescription = false, string buttonDescription = string.Empty, int buttonHeight = -1, int customFlags = false)
     {
         SCR_EditorAttributePresetEntry(buttonsOnRow, hasRandomizeButton, iconOfRandomizeButton, hasIcon, hasButtonDescription, buttonDescription, buttonHeight);
         m_bCustomFlags = customFlags;
     }

     //------------------------------------------------------------------------------------------------
     bool GetUsesCustomFlags()
     {
         return m_bCustomFlags;
     }
 }

 // A bool that makes sure that has override visuals are
 class SCR_EditorAttributeEntryOverride : SCR_BaseEditorAttributeEntry
 {
     protected bool m_bOverrideToggled;
     protected typename m_LinkedOverrideAttributeType;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_EditorAttributeEntryOverride(bool overrideToggled, typename linkedOverrideAttributeType)
     {
         m_bOverrideToggled = overrideToggled;
         m_LinkedOverrideAttributeType = linkedOverrideAttributeType;
     }

     //------------------------------------------------------------------------------------------------
     void GetToggleStateAndTypename(out bool overrideToggleState, out typename linkedOverrideAttributeType)
     {
         overrideToggleState = m_bOverrideToggled;
         linkedOverrideAttributeType = m_LinkedOverrideAttributeType;
     }
 }

 class SCR_BaseEditorAttributeDefaultFloatValue : SCR_BaseEditorAttributeEntry
 {
     protected int m_fDefaultFloatValue;

     //------------------------------------------------------------------------------------------------
     // constructor
     void SCR_BaseEditorAttributeDefaultFloatValue(float floatValue)
     {
         m_fDefaultFloatValue = floatValue;
     }

     //------------------------------------------------------------------------------------------------
     float GetDefaultFloatValue()
     {
         return m_fDefaultFloatValue;
     }
 }

 class SCR_BaseEditorAttributeCustomTitle : BaseContainerCustomTitle
 {
     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         title = source.GetClassName();
         title.Replace("SCR_", "");
         title.Replace("EditorAttribute", "");

         bool isServer;
         source.Get("m_bIsServer", isServer);
         if (!isServer)
             title += " (Local)";

         return true;
     }
 }