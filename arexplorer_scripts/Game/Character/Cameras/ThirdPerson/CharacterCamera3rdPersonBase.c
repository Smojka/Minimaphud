 // *************************************************************************************
 // ! CharacterCamera3rdPersonBase - base 3rd person camera
 // *************************************************************************************
 class CharacterCamera3rdPersonBase extends CharacterCameraBase
 {
     //-----------------------------------------------------------------------------
     static const float  CONST_UD_MIN    = -89.0;
     static const float  CONST_UD_MAX    = 89.0;

     static const float  CONST_LR_MIN    = -160.0;
     static const float  CONST_LR_MAX    = 160.0;

     //-----------------------------------------------------------------------------
     void CharacterCamera3rdPersonBase(CameraHandlerComponent pCameraHandler)
     {
         m_fFOVFilterVel = 0;
     }

     //-----------------------------------------------------------------------------
     override void OnActivate(ScriptedCameraItem pPrevCamera, ScriptedCameraItemResult pPrevCameraResult)
     {
         super.OnActivate(pPrevCamera, pPrevCameraResult);
         if (pPrevCamera)
         {
             // Use last shoulder state
             m_fShoulderLRPrefered = GetShoulderLastActive();
         }
         m_fFOVFilter = pPrevCameraResult.m_fFOV;
     }

     //-----------------------------------------------------------------------------
     override void OnUpdate(float pDt, out ScriptedCameraItemResult pOutResult)
     {
         pOutResult.m_vBaseAngles = GetBaseAngles();

         m_fFOV = GetBaseFOV();

         vector lookAngles = m_CharacterHeadAimingComponent.GetLookAngles();

         if (!m_bIgnoreCharacterPitch)
             lookAngles[1] = lookAngles[1] + m_OwnerCharacter.GetLocalYawPitchRoll()[1];

         Math3D.AnglesToMatrix(lookAngles, pOutResult.m_CameraTM);

         if (m_iBoneIndex != -1)
         {
             vector boneMatrix[4];
             m_OwnerCharacter.GetAnimation().GetBoneMatrix(m_iBoneIndex, boneMatrix);
             pOutResult.m_CameraTM[3]    = boneMatrix[3];
         }
         else
         {
             pOutResult.m_CameraTM[3]    = "0 0 0";
         }

         float leaning = m_ControllerComponent.GetLeaning();
         if( leaning < -0.1 )
             m_fShoulderLRPrefered = -1;
         else if( leaning > 0.1 )
             m_fShoulderLRPrefered = 1;

         m_fShoulderLROffset = Math.SmoothCD(m_fShoulderLROffset, m_fShoulderLRPrefered, m_fCameraLRShoulderVel, 0.14, 1000, pDt);

         m_fLeaningOffset = m_ControllerComponent.GetCurrentLeanAmount();
         float shoulderDist = GetShoulderDistance();

         vector msOffset = m_CameraOffsetMS;
         vector lsOffset = m_CameraOffsetLS;
         if (m_bShoulderInLS)
         {
             lsOffset[0] = lsOffset[0] + shoulderDist;
         }
         else
         {
             msOffset[0] = msOffset[0] + shoulderDist;
         }

         // character matrix
         vector charMat[4];
         m_OwnerCharacter.GetTransform(charMat);

         vector velocity = m_ControllerComponent.GetVelocity();
         velocity = velocity.InvMultiply3(charMat);
         for (int i = 0; i < 3; i++)
         {
             velocity[i] = Math.Clamp(velocity[i], -m_vMaxVelocity[i], m_vMaxVelocity[i]);
         }
         m_v3rd_VelocityAdd += (velocity - m_v3rd_VelocityAdd) * pDt * 2;

         msOffset = msOffset + m_v3rd_VelocityAdd * -0.05;
         // ls offset + ms offset + shoulder width
         pOutResult.m_CameraTM[3]        = pOutResult.m_CameraTM[3] + pOutResult.m_CameraTM[0] * lsOffset[0] + pOutResult.m_CameraTM[1] * lsOffset[1] + pOutResult.m_CameraTM[2] * lsOffset[2] + msOffset;

         float currY = pOutResult.m_CameraTM[3][1];
         if (m_fYoffsetPrevFrame == 0)
         {
             m_fYoffsetPrevFrame = currY;
         }
         m_fYoffsetPrevFrame = Math.Lerp(m_fYoffsetPrevFrame, currY, 0.01);
         float yDelta = 1;
         if (m_fYoffsetPrevFrame != 0)
         {
             yDelta = Math.Pow(currY/m_fYoffsetPrevFrame, 2); // easing
         }

         pOutResult.m_CameraTM[3][1] = m_fYoffsetPrevFrame * yDelta;

         m_fBobScale = m_CharacterCameraHandler.AddViewBobToTransform(pOutResult.m_CameraTM, 1, true);

         pOutResult.m_fDistance              = m_fDistance;
         pOutResult.m_iDirectBoneMode        = EDirectBoneMode.None;
         pOutResult.m_fUseHeading            = 1.0;
         pOutResult.m_fShoulderDist          = m_fShoulderLROffset;
         pOutResult.m_fNearPlane             = 0.04;
         pOutResult.m_fPositionModelSpace    = 1.0;
         pOutResult.m_pWSAttachmentReference = null;
         pOutResult.m_pOwner                 = m_OwnerCharacter;

         m_fFOVFilter = Math.SmoothCD(m_fFOVFilter, pOutResult.m_fFOV, m_fFOVFilterVel, 0.2, 1000, pDt);
         pOutResult.m_fFOV = m_fFOVFilter;
     }

     //-----------------------------------------------------------------------------
     override float GetBaseFOV()
     {
         CameraManager cameraManager = GetGame().GetCameraManager();
         if (!cameraManager)
             return 0;

         return cameraManager.GetThirdPersonFOV();
     }

     //-----------------------------------------------------------------------------
  int  GetActiveShoulder()
     {
         return m_fShoulderLRPrefered;
     }

     //-----------------------------------------------------------------------------
     override float GetShoulderDistance()
     {
         return m_fShoulderLROffset * m_fShoulderWidth + m_fLeanDistance * m_fLeaningOffset;
     }
     //-----------------------------------------------------------------------------
  protected int     m_iBoneIndex = -1;
     protected   vector  m_CameraOffsetMS;
     protected   vector  m_CameraOffsetLS;
     protected float m_fDistance;
     protected float   m_fShoulderWidth;
     protected bool    m_bShoulderInLS;
     protected float   m_fLeanDistance;
     protected   vector  m_vMaxVelocity = "2 2 2";

     protected float   m_fBobScale;

     protected int     m_fShoulderLRPrefered = 1;
     protected float   m_fShoulderLROffset;
     protected float   m_fLeaningOffset;
     protected float   m_fCameraLRShoulderVel;
     protected float   m_fLeaningOffsetVel;
     protected   vector  m_v3rd_VelocityAdd = vector.Zero;
     protected float   m_fYoffsetPrevFrame = 0.0;
 };