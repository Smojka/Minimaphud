 [EntityEditorProps(category: "GameScripted/Camera", description: "Manager for camera shake.")]
 class SCR_CameraShakeManagerComponentClass : ScriptComponentClass
 {
 }

 class SCR_CameraShakeManagerComponent : ScriptComponent
 {
     protected vector m_mShakeMatrix[4];
     protected float m_fFovScale;

     const int CAMERA_SHAKE_INSTANCES = 16;
     ref SCR_NoisyCameraShakeProgress m_aShakeInstances[CAMERA_SHAKE_INSTANCES];

     ref array<ref SCR_BaseCameraShakeProgress> m_aAdditionalInstances = {};

     // Current instance of manager or null if none
     private static SCR_CameraShakeManagerComponent s_Instance;

     //------------------------------------------------------------------------------------------------
  static void AddCameraShake(float linearMagnitude = 1.0, float angularMagnitude = 1.0, float inTime = 0.01, float sustainTime = 0.1, float outTime = 0.24)
     {
         if (s_Instance)
             s_Instance.DoAddCameraShake(linearMagnitude, angularMagnitude, inTime, sustainTime, outTime);
     }

     //------------------------------------------------------------------------------------------------
  void DoAddCameraShake(float linearMagnitude = 1.0, float angularMagnitude = 1.0, float inTime = 0.01, float sustainTime = 0.1, float outTime = 0.24)
     {
         for (int i = 0; i < CAMERA_SHAKE_INSTANCES; i++)
         {
             // Find first free instance that can be used
             SCR_NoisyCameraShakeProgress shake = m_aShakeInstances[i];
             if (shake.IsFinished())
             {
                 shake.SetParams(linearMagnitude, angularMagnitude, inTime, sustainTime, outTime);
                 shake.Start();
                 return;
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  static void AddCameraShake(SCR_BaseCameraShakeProgress instance)
     {
         if (s_Instance)
             s_Instance.DoAddCameraShake(instance);
     }

     //------------------------------------------------------------------------------------------------
  void DoAddCameraShake(SCR_BaseCameraShakeProgress instance)
     {
         if (!instance)
         {
             Print("Invalid camera shake instance passed to manager!", LogLevel.ERROR);
             return;
         }

         m_aAdditionalInstances.Insert(instance);
         instance.Start();
     }

     //------------------------------------------------------------------------------------------------
  static void ApplyCameraShake(inout vector matrix[4], inout float fov)
     {
         if (s_Instance)
             s_Instance.DoApplyCameraShake(matrix, fov);
     }

     //------------------------------------------------------------------------------------------------
  void DoApplyCameraShake(inout vector matrix[4], inout float fov)
     {
         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CHARACTER_ADDITIONAL_CAMERASHAKE_DISABLE))
             return;
         #endif

         Math3D.MatrixMultiply4( matrix, m_mShakeMatrix, matrix );
         fov *= m_fFovScale;
     }

     //------------------------------------------------------------------------------------------------
  static void ClearCameraShake()
     {
         if (s_Instance)
             s_Instance.DoClearCameraShake();
     }

     //------------------------------------------------------------------------------------------------
  void DoClearCameraShake()
     {
         for (int i = 0; i < CAMERA_SHAKE_INSTANCES; i++)
         {
             SCR_BaseCameraShakeProgress shake = m_aShakeInstances[i];
             if (!shake.IsFinished())
                 shake.Clear();
         }

         m_aAdditionalInstances.Clear();
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         Math3D.MatrixIdentity4(m_mShakeMatrix);

         // Skip the initialization of shake manager on headless
         // clients and dedicated server, as the visual can be ommitted completely.
         if (System.IsConsoleApp())
             return;

         s_Instance = this;
         SetEventMask(owner, EntityEvent.FRAME);
         GenericEntity.Cast(owner).Activate();

         // Pre-cache instances
         for (int i = 0; i < CAMERA_SHAKE_INSTANCES; i++)
         {
             m_aShakeInstances[i] = new SCR_NoisyCameraShakeProgress();
         }
     }

     //------------------------------------------------------------------------------------------------
     override void EOnFrame(IEntity owner, float timeSlice)
     {
         // Clear out matrix and FOV
         Math3D.MatrixIdentity4(m_mShakeMatrix);
         m_fFovScale = 1.0;

         // Update generic shakes
         for (int i = 0; i < CAMERA_SHAKE_INSTANCES; i++)
         {
             SCR_BaseCameraShakeProgress shake = m_aShakeInstances[i];
             if (!shake.IsFinished())
             {
                 shake.Update(owner, timeSlice);
                 shake.Apply(m_mShakeMatrix, m_fFovScale);
             }
         }

         // Update custom instances
         for (int i = 0; i < m_aAdditionalInstances.Count();)
         {
             SCR_BaseCameraShakeProgress shake = m_aAdditionalInstances[i];
             if (!shake || shake.IsFinished())
             {
                 m_aAdditionalInstances.Remove(i);
                 continue;
             }

             ++i;

             shake.Update(owner, timeSlice);
             shake.Apply(m_mShakeMatrix, m_fFovScale);
         }

         super.EOnFrame(owner, timeSlice);

         // Draw diag window
         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CHARACTER_CAMERASHAKE_TEST_WINDOW))
         {
             DbgUI.Begin("Camera Shake");
             {
                 float linear = 1, angular = 1, inTime = 0.01, sustainTime = 0.1, outTime = 1.0;
                 DbgUI.Text("New shake: ");
                 DbgUI.InputFloat("Linear: ", linear);
                 DbgUI.InputFloat("Angular: ", angular);
                 DbgUI.InputFloat("InTime: ", inTime);
                 DbgUI.InputFloat("SustainTime: ", sustainTime);
                 DbgUI.InputFloat("OutTime: ", outTime);

                 if (DbgUI.Button("Add new shake"))
                 {
                     AddCameraShake(linear, angular, inTime, sustainTime, outTime);
                 }

                 if (DbgUI.Button("Add custom shake"))
                 {
                     // Just for testing purposes
                     SCR_NoisyCameraShakeProgress progress = new SCR_NoisyCameraShakeProgress();
                     progress.SetParams(linear, angular, inTime, sustainTime, outTime);

                     AddCameraShake(progress);
                 }

                 DbgUI.Spacer(16);

                 if (DbgUI.Button("Clear shake"))
                 {
                     ClearCameraShake();
                 }
             }
             DbgUI.End();
         }
         #endif
     }
 }