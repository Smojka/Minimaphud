 class MuzzleUIInfo : UIInfo
 {
     // Show caliber indicator in weapon UI
     [Attribute("false", UIWidgets.CheckBox, "Show caliber indicator in weapon UI.")]
     protected bool m_bShowCaliber;

     bool ShowCaliber()
     {
         return m_bShowCaliber;
     }

     [Attribute("", "auto", "Caliber identification, e.g. 7.62Ã39mm")]
     protected string m_sCaliber;

     string GetCaliber()
     {
         return m_sCaliber;
     }

     // Magazine icon behaviour in weapon UI
     [Attribute("true", UIWidgets.CheckBox, "Show firemode indicator.")]
     protected bool m_bShowFiremode;

     bool ShowFiremodeIcon()
     {
         return m_bShowFiremode;
     }


     [Attribute("{3BB05C675B05A74B}UI/Textures/WeaponInfo/icons_weaponInfo.imageset", UIWidgets.ResourceNamePicker, "Imageset with most of weapon info textures", "imageset")]
     protected ResourceName m_sFiremodeIconImageset;

     ResourceName GetFiremodeIconImageset()
     {
         return m_sFiremodeIconImageset;
     }

     [Attribute("{4E003F94B2A00561}UI/Textures/WeaponInfo/icons_weaponInfo-glow.imageset", UIWidgets.ResourceNamePicker, "Imageset with most of weapon info textures", "imageset")]
     protected ResourceName m_sFiremodeGlowImageset;

     ResourceName GetFiremodeGlowImageset()
     {
         return m_sFiremodeGlowImageset;
     }

     [Attribute("firemode-rifle-single", "auto", "Firemode indicator - single shot")]
     protected string m_sFiremodeSingle;

     [Attribute("firemode-rifle-burst3", "auto", "Firemode indicator - burst")]
     protected string m_sFiremodeBurst;

     [Attribute("firemode-rifle-auto", "auto", "Firemode indicator - full auto")]
     protected string m_sFiremodeAuto;

     [Attribute("firemode-safety", "auto", "Firemode indicator - safety", "edds")]
     protected string m_sFiremodeSafety;

     string GetFiremodeIconName(EWeaponFiremodeType firemode)
     {
         string icon = "";

         switch (firemode)
         {
             case EWeaponFiremodeType.Semiauto:
                 icon = m_sFiremodeSingle;
                 break;

             case EWeaponFiremodeType.Burst:
                 icon = m_sFiremodeBurst;
                 break;

             case EWeaponFiremodeType.Auto:
                 icon = m_sFiremodeAuto;
                 break;

             case EWeaponFiremodeType.Safety:
                 icon =  m_sFiremodeSafety;
                 break;

             default:
                 icon = m_sFiremodeSingle;
                 break;
         }

         return icon;
     }

     [Attribute("", UIWidgets.Object)]
     ref SCR_MagazineIndicatorConfiguration m_MagIndicator;
 };