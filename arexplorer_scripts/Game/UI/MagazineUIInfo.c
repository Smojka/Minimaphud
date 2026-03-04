 class MagazineUIInfo : UIInfo
 {
     [Attribute("", UIWidgets.EditBox, "Magazine ammunition caliber.", "")]
     protected string m_sAmmoCaliber;

     // Returns m_sAmmoCalibar if it's defined.
     string GetAmmoCaliber()
     {
         return m_sAmmoCaliber;
     }



     [Attribute("", UIWidgets.EditBox, "Magazine type (AP, Tracer, Ball, ...). Used for Weapon Info UI.", "")]
     protected string m_sAmmoType;

     // Returns m_sAmmoCalibar if it's defined. Otherwise returns magazine name from the inventory component.
     string GetAmmoType()
     {
         return m_sAmmoType;
     }

     // Magazine icon behaviour in weapon UI
     [Attribute("false", UIWidgets.CheckBox, "Show ammo type extra text next to ammo type icon(s) in Weapon Info UI.")]
     protected bool m_bShowAmmoTypeText;

     bool ShowAmmoTypeText()
     {
         return m_bShowAmmoTypeText;
     }

     [Attribute("1", UIWidgets.Flags, "Set magazine ammo type flags.", "", ParamEnumArray.FromEnum(EAmmoType) )]
     EAmmoType m_eAmmoTypeFlags;

     EAmmoType GetAmmoTypeFlags()
     {
         return m_eAmmoTypeFlags;
     }

     [Attribute("", UIWidgets.Object)]
     ref SCR_MagazineIndicatorConfiguration m_MagIndicator;
 };