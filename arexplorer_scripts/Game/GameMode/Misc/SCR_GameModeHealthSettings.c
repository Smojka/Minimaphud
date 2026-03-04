 class SCR_GameModeHealthSettingsClass : ScriptComponentClass
 {
 }

 class SCR_GameModeHealthSettings : ScriptComponent
 {
     [Attribute(defvalue: "1", uiwidget: UIWidgets.Slider, desc: "Character bleeding rate multiplier", params: "0 5 0.001", precision: 3, category: "Game Mode")]
     protected float m_fDOTScale;

     [Attribute(defvalue: "1", uiwidget: UIWidgets.Slider, desc: "Character regeneration rate multiplier", params: "0 5 0.001", precision: 3, category: "Game Mode")]
     protected float m_fRegenScale;

     [Attribute(defvalue: "true", uiwidget: UIWidgets.CheckBox, desc: "Whether unconsciousness is allowed", category: "Game Mode")]
     protected bool m_bPermitUnconsciousness;

     [Attribute(defvalue: "false", uiwidget: UIWidgets.CheckBox, desc: "Whether using VON during unconsciousness is allowed", category: "Game Mode")]
     protected bool m_bPermitUnconsciousVON;

     [Attribute(defvalue: "0.75", uiwidget: UIWidgets.Slider, desc: "How much will the character be slowed down when having tourniquetted leg", params: "0 1 0.001", precision: 3, category: "Game Mode")]
     protected float m_fTourniquettedLegMovementSlowdown;

     [Attribute(defvalue: "0.1", uiwidget: UIWidgets.Slider, desc: "Affects how much the bleeding is reduced", params: "0 1 0.001", precision: 3, category: "Game Mode")]
     private float m_fTourniquetStrengthMultiplier;

     [Attribute(defvalue: "1", uiwidget: UIWidgets.Slider, desc: "How fast will character regenerate when situated in medical compartment", params: "0 5 0.001", precision: 3, category: "Game Mode")]
     protected float m_fCompartmentRegenerationRateMultiplier;

     [Attribute(defvalue: "10", uiwidget: UIWidgets.Slider, desc: "Time without receiving damage or bleeding to start regeneration [s]", params: "0 600 1", category: "Game Mode")]
     protected float m_fRegenerationDelay;

     [Attribute(defvalue: "200", uiwidget: UIWidgets.Slider, desc: "Time to fully regenerate resilience hit zone [s]", params: "0 600 1", category: "Game Mode")]
     protected float m_fFullRegenerationTime;

     [Attribute(defvalue: "0.75", uiwidget: UIWidgets.Slider, desc: "Minimum amount of stamina needed to regenerate character hit zones", params: "0 1 0.001", precision: 3, category: "Game Mode")]
     protected float m_fRegenerationMinStaminaLevel;

     [Attribute(defvalue: "40", uiwidget: UIWidgets.Slider, desc: "Maximal weight of all the items character can carry to regenerate character hit zones", params: "0 150 1", category: "Game Mode")]
     protected float m_fRegenerationMaxLoadoutWeight;

     [Attribute(defvalue: "2", uiwidget: UIWidgets.Slider, desc: "Maximal character movement speed to regenerate character hit zones", params: "0 25 0.001", precision: 3, category: "Game Mode")]
     protected float m_fRegenerationMaxMovementSpeed;

     [Attribute(defvalue: "1.333", uiwidget: UIWidgets.Slider, desc: "Character hit zone regeneration speed when in crouch", params: "0 5 0.001", precision: 3, category: "Game Mode")]
     protected float m_fRegenerationSpeedCrouch;

     [Attribute(defvalue: "1.666", uiwidget: UIWidgets.Slider, desc: "Character hit zone regeneration speed when in prone", params: "0 5 0.001", precision: 3, category: "Game Mode")]
     protected float m_fRegenerationSpeedProne;

     //------------------------------------------------------------------------------------------------
  float GetBleedingScale()
     {
         return m_fDOTScale;
     }

     //------------------------------------------------------------------------------------------------
  void SetBleedingScale(float rate)
     {
         m_fDOTScale = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetRegenScale()
     {
         return m_fRegenScale;
     }

     //------------------------------------------------------------------------------------------------
  void SetRegenScale(float rate)
     {
         m_fRegenScale = rate;
     }

     //------------------------------------------------------------------------------------------------
  bool IsUnconsciousnessPermitted()
     {
         return m_bPermitUnconsciousness;
     }

     //------------------------------------------------------------------------------------------------
  void SetUnconsciousnessPermitted(bool permitted)
     {
         m_bPermitUnconsciousness = permitted;
     }

     //------------------------------------------------------------------------------------------------
  bool IsUnconsciousVONPermitted()
     {
         return m_bPermitUnconsciousVON;
     }

     //------------------------------------------------------------------------------------------------
  void SetUnconsciousVONPermitted(bool permitted)
     {
         m_bPermitUnconsciousVON = permitted;
     }

     //------------------------------------------------------------------------------------------------
  float GetTourniquettedLegMovementSlowdown()
     {
         return m_fTourniquettedLegMovementSlowdown;
     }

     //------------------------------------------------------------------------------------------------
  void SetTourniquettedLegMovementSlowdown(float rate)
     {
         m_fTourniquettedLegMovementSlowdown = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetTourniquetStrengthMultiplier()
     {
         return m_fTourniquetStrengthMultiplier;
     }

     //------------------------------------------------------------------------------------------------
  void SetTourniquetStrengthMultiplier(float rate)
     {
         m_fTourniquetStrengthMultiplier = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetCompartmentRegenRateMultiplier()
     {
         return m_fCompartmentRegenerationRateMultiplier;
     }

     //------------------------------------------------------------------------------------------------
  void SetCompartmentRegenRateMultiplier(float rate)
     {
         m_fCompartmentRegenerationRateMultiplier = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetRegenerationDelay()
     {
         return m_fRegenerationDelay;
     }

     //------------------------------------------------------------------------------------------------
  void SetRegenerationDelay(float rate)
     {
         m_fRegenerationDelay = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetResilienceHzRegenTime()
     {
         return m_fFullRegenerationTime;
     }

     //------------------------------------------------------------------------------------------------
  void SetResilienceHzRegenTime(float rate)
     {
         m_fFullRegenerationTime = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetMinStaminaLevelForRegeneration()
     {
         return m_fRegenerationMinStaminaLevel;
     }

     //------------------------------------------------------------------------------------------------
  void SetMinStaminaLevelForRegeneration(float rate)
     {
         m_fRegenerationMinStaminaLevel = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetMaxCharWeightForRegeneration()
     {
         return m_fRegenerationMaxLoadoutWeight;
     }

     //------------------------------------------------------------------------------------------------
  void SetMaxCharWeightForRegeneration(float rate)
     {
         m_fRegenerationMaxLoadoutWeight = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetMaxCharMovementSpeedForRegeneration()
     {
         return m_fRegenerationMaxMovementSpeed;
     }

     //------------------------------------------------------------------------------------------------
  void SetMaxCharMovementSpeedForRegeneration(float rate)
     {
         m_fRegenerationMaxMovementSpeed = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetRegenerationSpeedCrouc()
     {
         return m_fRegenerationSpeedCrouch;
     }

     //------------------------------------------------------------------------------------------------
  void SetRegenerationSpeedCrouc(float rate)
     {
         m_fRegenerationSpeedCrouch = rate;
     }

     //------------------------------------------------------------------------------------------------
  float GetRegenerationSpeedProne()
     {
         return m_fRegenerationSpeedProne;
     }

     //------------------------------------------------------------------------------------------------
  void SetRegenerationSpeedProne(float rate)
     {
         m_fRegenerationSpeedProne = rate;
     }
 }