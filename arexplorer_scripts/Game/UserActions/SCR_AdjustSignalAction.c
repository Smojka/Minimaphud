 class SCR_AdjustSignalAction : ScriptedSignalUserAction
 {
     [Attribute(defvalue: "0.1", desc: "Adjustment step [-1 to 1]", params: "-1 1 0.1")]
     protected float m_fAdjustmentStep;

     [Attribute(desc: "If action should wait for player to use their scroll wheel in order to change value")]
     protected bool m_bManualAdjustment;

     [Attribute(desc: "Determines if this action will start from the begining when max value is reached - or from the other side if Adjustment Step is below 0")]
     protected bool m_bLoopAction;

     [Attribute(defvalue: "SelectAction", desc: "Input action for increase")]
     protected string m_sActionIncrease;

     [Attribute(desc: "Input action for decrease")]
     protected string m_sActionDecrease;

     [Attribute(desc: "Action start sound event name")]
     protected string m_sActionStartSoundEvent;

     [Attribute(desc: "Action canceled sound event name")]
     protected string m_sActionCanceledSoundEvent;

     [Attribute(desc: "Movement sound event name")]
     protected string m_sMovementSoundEvent;

     [Attribute(desc: "Movement stop sound event name")]
     protected string m_sMovementStopSoundEvent;

     protected float m_fTargetValue;

     protected bool m_bIsAdjustedByPlayer;

     protected SoundComponent m_SoundComponent;

     protected float m_fLerpLast;

     protected AudioHandle m_MovementAudioHandle;

     //------------------------------------------------------------------------------------------------
  bool IsManuallyAdjusted()
     {
         return m_bManualAdjustment;
     }

     //------------------------------------------------------------------------------------------------
     override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
     {
         m_SoundComponent = SoundComponent.Cast(pOwnerEntity.FindComponent(SoundComponent));
         if (GetActionDuration() != 0)
             m_fAdjustmentStep /= Math.AbsFloat(GetActionDuration());
     }

     //------------------------------------------------------------------------------------------------
     override bool CanBeShownScript(IEntity user)
     {
         if (!m_bLoopAction && !m_bManualAdjustment)
         {
             if (m_fAdjustmentStep > 0 && GetCurrentValue() >= GetMaximumValue())
                 return false;

             if (m_fAdjustmentStep < 0 && GetCurrentValue() <= GetMinimumValue())
                 return false;
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  void ToggleActionBypass()
     {
         HandleAction(1);
     }

     //------------------------------------------------------------------------------------------------
     override void PerformContinuousAction(IEntity pOwnerEntity, IEntity pUserEntity, float timeSlice)
     {
         if (!m_bManualAdjustment)
             HandleAction(timeSlice);
     }

     //------------------------------------------------------------------------------------------------
     override void OnActionStart(IEntity pUserEntity)
     {
         if (m_SoundComponent && m_sActionStartSoundEvent != string.Empty)
             m_SoundComponent.SoundEvent(m_sActionStartSoundEvent);

         m_bIsAdjustedByPlayer = SCR_PlayerController.GetLocalControlledEntity() == pUserEntity;

         if (!m_bIsAdjustedByPlayer)
             return;

         m_fTargetValue = Math.InverseLerp(GetMinimumValue(), GetMaximumValue(), GetCurrentValue());
         if (!GetActionDuration())
             ToggleActionBypass();

         if (!m_bManualAdjustment)
             return;

         if (!m_sActionIncrease.IsEmpty())
             GetGame().GetInputManager().AddActionListener(m_sActionIncrease, EActionTrigger.VALUE, HandleAction);

         if (!m_sActionDecrease.IsEmpty())
             GetGame().GetInputManager().AddActionListener(m_sActionDecrease, EActionTrigger.VALUE, HandleActionDecrease);
     }

     //------------------------------------------------------------------------------------------------
     override void OnActionCanceled(IEntity pOwnerEntity, IEntity pUserEntity)
     {
         // Play sound
         if (m_SoundComponent && m_sActionCanceledSoundEvent != string.Empty)
                 m_SoundComponent.SoundEvent(m_sActionCanceledSoundEvent);

         if (!m_bIsAdjustedByPlayer)
             return;

         m_bIsAdjustedByPlayer = false;

         if (!m_bManualAdjustment)
             return;

         if (!m_sActionIncrease.IsEmpty())
             GetGame().GetInputManager().RemoveActionListener(m_sActionIncrease, EActionTrigger.VALUE, HandleAction);

         if (!m_sActionDecrease.IsEmpty())
             GetGame().GetInputManager().RemoveActionListener(m_sActionDecrease, EActionTrigger.VALUE, HandleActionDecrease);
     }

     //------------------------------------------------------------------------------------------------
  void HandleAction(float value)
     {
         if (value == 0)
             return;

         if (m_bManualAdjustment)
             value /= Math.AbsFloat(value);

         value *= m_fAdjustmentStep;

         m_fTargetValue += value;
         if (m_bLoopAction)
         {
             if (value > 0 && float.AlmostEqual(GetCurrentValue(), GetMaximumValue()))
                 m_fTargetValue = GetMinimumValue();
             else if (value < 0 && float.AlmostEqual(GetCurrentValue(), GetMinimumValue()))
                 m_fTargetValue = GetMaximumValue();
         }

         if (float.AlmostEqual(m_fTargetValue, GetCurrentValue(), Math.AbsFloat(m_fAdjustmentStep)))
             return;

         // Round to adjustment step
         m_fTargetValue = Math.Floor(m_fTargetValue / m_fAdjustmentStep) * m_fAdjustmentStep;

         // Limit to min/max value
         m_fTargetValue = Math.Clamp(m_fTargetValue, GetMinimumValue(), GetMaximumValue());

         if (!float.AlmostEqual(m_fTargetValue, GetCurrentValue()))
             SetSendActionDataFlag();
     }

     //------------------------------------------------------------------------------------------------
  void HandleActionDecrease(float value)
     {
         HandleAction(-value);
     }

     //------------------------------------------------------------------------------------------------
  void PlayMovementAndStopSound(float lerp)
     {
         if (!m_SoundComponent)
             return;

         if (m_fLerpLast == lerp)
             return;

         if (float.AlmostEqual(lerp, 1))
         {
             if (!float.AlmostEqual(m_fLerpLast, 1))
             {
                 m_SoundComponent.Terminate(m_MovementAudioHandle);
                 if (m_sMovementStopSoundEvent != string.Empty)
                     m_SoundComponent.SoundEvent(m_sMovementStopSoundEvent);
             }
         }
         else if (float.AlmostEqual(lerp, 0))
         {
             if (!float.AlmostEqual(m_fLerpLast, 0))
             {
                 m_SoundComponent.Terminate(m_MovementAudioHandle);
                 if (m_sMovementStopSoundEvent != string.Empty)
                     m_SoundComponent.SoundEvent(m_sMovementStopSoundEvent);
             }
         }
         else
         {
             if (m_SoundComponent.IsFinishedPlaying(m_MovementAudioHandle) && m_sMovementStopSoundEvent != string.Empty)
                 m_MovementAudioHandle = m_SoundComponent.SoundEvent(m_sMovementSoundEvent);
         }

         m_fLerpLast = lerp;
     }

     //------------------------------------------------------------------------------------------------
  override bool HasLocalEffectOnlyScript()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  override bool CanBroadcastScript()
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  override protected bool OnSaveActionData(ScriptBitWriter writer)
     {
         float lerp = Math.Lerp(GetMinimumValue(), GetMaximumValue(), m_fTargetValue);
         writer.WriteFloat01(lerp);

         SetSignalValue(m_fTargetValue);

         PlayMovementAndStopSound(lerp);

         return true;
     }

     //------------------------------------------------------------------------------------------------
  override protected bool OnLoadActionData(ScriptBitReader reader)
     {
         if (m_bIsAdjustedByPlayer)
             return true;

         float lerp;
         reader.ReadFloat01(lerp);

         m_fTargetValue = Math.InverseLerp(GetMinimumValue(), GetMaximumValue(), lerp);
         SetSignalValue(m_fTargetValue);

         PlayMovementAndStopSound(lerp);

         return true;
     }
 }