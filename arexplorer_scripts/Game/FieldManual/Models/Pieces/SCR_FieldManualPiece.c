 [BaseContainerProps(insertable: false)]
 class SCR_FieldManualPiece
 {
     [Attribute(defvalue: SCR_EInputTypeCondition.ALL_INPUTS.ToString(), uiwidget: UIWidgets.ComboBox, desc: "Define if this piece should be displayed for keyboard users, gamepad users or both", enums: ParamEnumArray.FromEnum(SCR_EInputTypeCondition))]
     protected SCR_EInputTypeCondition m_eInputDisplayCondition;

     //------------------------------------------------------------------------------------------------
  // To be overridden by child pieces
     void CreateWidget(notnull Widget parent);

     //------------------------------------------------------------------------------------------------
  bool CanCreateWidget()
     {
         // I am very proud of this ugly one-liner (that is technically a two-liner)
         return m_eInputDisplayCondition == SCR_EInputTypeCondition.ALL_INPUTS ||
             GetGame().GetInputManager().IsUsingMouseAndKeyboard() == (m_eInputDisplayCondition == SCR_EInputTypeCondition.KEYBOARD_ONLY);
     }

     //------------------------------------------------------------------------------------------------
  bool CanRefresh()
     {
         return m_eInputDisplayCondition != SCR_EInputTypeCondition.ALL_INPUTS;
     }
 }