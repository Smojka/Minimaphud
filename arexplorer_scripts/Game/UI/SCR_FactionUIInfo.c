 class SCR_FactionUIInfo : UIInfo
 {
     [Attribute("", UIWidgets.EditBox, "Upper case name.", "")]
     private string m_sNameUpper;

     string GetFactionNameUpperCase()
     {
         return m_sNameUpper;
     }


 };