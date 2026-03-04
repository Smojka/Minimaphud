 //------------------------------------------------------------------------------------------------
 class SCR_NameTagElementTitle: BaseContainerCustomTitle
 {
     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         string val;
         source.Get("m_sWidgetName", val);
         title = val + "  " + source.GetClassName();
         return true;
     }
 };

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_NTElementBase
 {
     [Attribute("", UIWidgets.EditBox, "Widget name")]
     string m_sWidgetName;

     [Attribute("", UIWidgets.Object, "Entity state configuration")]
     protected ref array<ref SCR_NTStateConfigBase> m_aEntityStates;

     [Attribute("0", UIWidgets.CheckBox, "Whether this element should get scaled down with distance \n ! For scalable elements only")]
     protected bool m_bScaleElement;

     protected ref map<ENameTagEntityState, SCR_NTStateConfigBase> m_aEntityStatesMap = new map< ENameTagEntityState, SCR_NTStateConfigBase>;

     //------------------------------------------------------------------------------------------------
  protected void InitElementWidget(SCR_NameTagData data, int index)
     {
         Widget widget = data.m_NameTagWidget.FindAnyWidget(m_sWidgetName);
         if (!widget)
         {
             Print("Nametags: Improperly setup element in a zone config, cannot fetch requested widget " + this , LogLevel.WARNING);
             return;
         }

         data.m_aNametagElements[index] = widget; // add to data array

         SetDefaults(data, index);
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_NTStateConfigBase GetEntityStateConfig(SCR_NameTagData data)
     {
         ENameTagEntityState state = data.m_ePriorityEntityState;
         ENameTagEntityState stateMin = ENameTagEntityState.DEFAULT;
         SCR_NTStateConfigBase cfg;
         while (state != stateMin)
         {
             if (data.m_eEntityStateFlags & state)
             {
                 cfg = m_aEntityStatesMap.Get(state);
                 if (cfg)
                     return cfg;
             }

             state = state >> 1;
         }

         return m_aEntityStatesMap.Get(ENameTagEntityState.DEFAULT);
     }

     //------------------------------------------------------------------------------------------------
  void SetDefaults(SCR_NameTagData data, int index)
     {}

     //------------------------------------------------------------------------------------------------
  protected void ScaleElement(SCR_NameTagData data, int index)
     {}

     //------------------------------------------------------------------------------------------------
  void UpdateElement(SCR_NameTagData data, int index)
     {
         // Initial setup
         if ( !data.m_aNametagElements[index] )
         {
             InitElementWidget(data, index);
             return;
         }

         // scale
         if (m_bScaleElement)
         {
             data.m_fTimeSliceUpdate = 1.0;
             ScaleElement(data, index);
         }

     }

     //------------------------------------------------------------------------------------------------
     void SCR_NTElementBase()
     {
         foreach (SCR_NTStateConfigBase entState : m_aEntityStates)
         {
             ENameTagEntityState stateEnum = entState.m_eEntityState;
             if (stateEnum)
                 m_aEntityStatesMap.Insert(stateEnum, entState);
         }
     }
 };

 //------------------------------------------------------------------------------------------------
 class SCR_NameTagStateTitle: BaseContainerCustomTitle
 {
     //------------------------------------------------------------------------------------------------
     override bool _WB_GetCustomTitle(BaseContainer source, out string title)
     {
         ENameTagEntityState type;
         source.Get("m_eEntityState", type);
         title = typename.EnumToString(ENameTagEntityState, type);

         return true;
     }
 };

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps()]
 class SCR_NTStateConfigBase
 {
     [Attribute("", UIWidgets.ComboBox, "Configure selected entity state", "", ParamEnumArray.FromEnum(ENameTagEntityState))]
     int m_eEntityState;

     [Attribute("1 1 1 1", UIWidgets.ColorPicker, desc: "Color")]
     ref Color m_vColor;

     [Attribute("1", UIWidgets.Slider, desc: "Default opacity when in line of sight", "0 1 1")]
     float m_fOpacityDefault;

     [Attribute("true", UIWidgets.CheckBox, "If tag fade time is set, choose whether this state should skip the animation")]
     bool m_bAnimateTransition;
 };

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_NameTagStateTitle()]
 class SCR_NTStateText : SCR_NTStateConfigBase
 {
     [Attribute("0 0 0 1", UIWidgets.ColorPicker, desc: "Text shadow color")]
     ref Color m_vShadowColor;

     [Attribute("12", UIWidgets.CheckBox, "Text shadow size", params: "0 30")]
     int m_fShadowSize;

     [Attribute("1", UIWidgets.Slider, desc: "Text shadow opacity", "0 1 1")]
     float m_fShadowOpacity;
 };

 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_NameTagStateTitle()]
 class SCR_NTStateIcon : SCR_NTStateConfigBase
 {
     [Attribute("", UIWidgets.EditBox, desc: "Image selection",)]
     string m_sImagesetQuad;
 };