

 class SCR_ManualCameraParam
 {
     protected const float TRACE_DIS = 250;

     float timeSlice;
     BaseWorld world;
     IEntity target;
     private vector cursorWorldPos;

     EManualCameraFlag flag;

     vector transformOriginal[4];
     vector transform[4];
     vector rotOriginal;
     vector rotDelta;
     vector velocityOriginal;

     vector multiplier;

     float fovOriginal;
     float fov;

     bool isManualInputEnabled;
     bool isManualInput;
     bool isCursorEnabled = true;
     bool isDirty;
     bool isDirtyExternal;

     //------------------------------------------------------------------------------------------------
  void SetDeltaPos(vector deltaPos)
     {
         transform[3] = transform[3]
             + (transform[0] * deltaPos[0]) //--- Lateral
             + (transform[1] * deltaPos[1]) //--- Vertical
             + (transform[2] * deltaPos[2]); //--- Longitudinal
     }

     //------------------------------------------------------------------------------------------------
  bool GetCursorWorldPos(out vector outPos = vector.Zero)
     {
         //--- Return cached position
         if (cursorWorldPos != vector.Zero)
         {
             outPos = cursorWorldPos;
             return true;
         }

         //-- Calculate new position
         WorkspaceWidget workspace = GetGame().GetWorkspace();
         if (!workspace || !world) return false;

         int mouseX, mouseY;
         if (GetGame().GetInputManager().IsUsingMouseAndKeyboard())
         {
             WidgetManager.GetMousePos(mouseX, mouseY);
         }
         else
         {
             mouseX = workspace.GetWidth();
             mouseY = workspace.GetHeight();
             mouseX /= 2;
             mouseY /= 2;
         }

         vector outDir;
         vector startPos = workspace.ProjScreenToWorld(workspace.DPIUnscale(mouseX), workspace.DPIUnscale(mouseY), outDir, world, -1);
         outDir *= TRACE_DIS;

         autoptr TraceParam trace = new TraceParam();
         trace.Start = startPos;
         trace.End = startPos + outDir;
         trace.Flags = TraceFlags.WORLD | TraceFlags.OCEAN | TraceFlags.ENTS;
         trace.LayerMask = EPhysicsLayerPresets.Projectile;

         float traceDis = world.TraceMove(trace, null);
         if (traceDis == 1) return false;

         if (trace.TraceEnt && !trace.TraceEnt.IsInherited(GenericTerrainEntity)) target = trace.TraceEnt;
         cursorWorldPos = startPos + outDir * traceDis;
         outPos = cursorWorldPos;
         return true;
     }

     //------------------------------------------------------------------------------------------------
  void SetCursorWorldPos(vector pos)
     {
         cursorWorldPos = pos;
     }

     //------------------------------------------------------------------------------------------------
  void ShowDebug(SCR_ManualCamera camera)
     {
         DbgUI.Begin("SCR_ManualCameraParam", 0, 0);

         DbgUI.Text(string.Format("GetParent() = %1", camera.GetParent()));
         DbgUI.Text("flag = ");
         array<int> flagValues = new array<int>;
         SCR_Enum.BitToIntArray(flag, flagValues);
         foreach (int i: flagValues)
         {
             DbgUI.Text("  " + typename.EnumToString(EManualCameraFlag, i));
         }
         DbgUI.Text(string.Format("target = %1", target));
         DbgUI.Text("isDirty = " + isDirty.ToString());
         DbgUI.Text("isManualInput = " + isManualInput.ToString());
         DbgUI.Text("isManualInputEnabled = " + isManualInputEnabled.ToString());
         DbgUI.Text("isCursorEnabled = " + isCursorEnabled.ToString());
         DbgUI.Text("multiplier = " + multiplier.ToString());
         DbgUI.Text("fov = " + fov.ToString());
         DbgUI.Text("rotOriginal = " + rotOriginal.ToString());
         DbgUI.Text("rotDelta = " + rotDelta.ToString());
         DbgUI.Text("posDelta = " + (transform[3] - transformOriginal[3]).ToString());
         DbgUI.Text("transformOriginal: ");
         DbgUI.Text("o[0] = " + transformOriginal[0].ToString());
         DbgUI.Text("o[1] = " + transformOriginal[1].ToString());
         DbgUI.Text("o[2] = " + transformOriginal[2].ToString());
         DbgUI.Text("o[3] = " + transformOriginal[3].ToString());
         DbgUI.Text("transform: ");
         DbgUI.Text("[0] = " + transform[0].ToString());
         DbgUI.Text("[1] = " + transform[1].ToString());
         DbgUI.Text("[2] = " + transform[2].ToString());
         DbgUI.Text("[3] = " + transform[3].ToString());
         DbgUI.Text("velocityOriginal = " + velocityOriginal.ToString());

         DbgUI.Spacer(30);
         bool showGraphs;
         DbgUI.Check("Show Graphs", showGraphs);
         if (showGraphs)
         {
             DbgUI.Spacer(20);
             DbgUI.Text("multiplier");
             DbgUI.PlotLive("multiplier0", 500, 50, multiplier[0], 0.01, 100, ARGBF(1, 1, 0, 0));
             DbgUI.PlotLive("multiplier1", 500, 50, multiplier[1], 0.01, 100, ARGBF(1, 0, 1, 0));
             DbgUI.PlotLive("multiplier2", 500, 50, multiplier[2], 0.01, 100, ARGBF(1, 0, 0, 1));

             DbgUI.Spacer(20);
             DbgUI.Text("transformDelta");
             DbgUI.PlotLive("transform0", 500, 50, transform[3][0] - transformOriginal[3][0], 0.01, 100, ARGBF(1, 1, 0, 0));
             DbgUI.PlotLive("transform1", 500, 50, transform[3][1] - transformOriginal[3][1], 0.01, 100, ARGBF(1, 0, 1, 0));
             DbgUI.PlotLive("transform2", 500, 50, transform[3][2] - transformOriginal[3][2], 0.01, 100, ARGBF(1, 0, 0, 1));

             DbgUI.Spacer(20);
             DbgUI.Text("rotDelta");
             DbgUI.PlotLive("rotDelta0", 500, 50, rotDelta[0], 0.01, 100, ARGBF(1, 1, 0, 0));
             DbgUI.PlotLive("rotDelta1", 500, 50, rotDelta[1], 0.01, 100, ARGBF(1, 0, 1, 0));
             DbgUI.PlotLive("rotDelta2", 500, 50, rotDelta[2], 0.01, 100, ARGBF(1, 0, 0, 1));
         }
         DbgUI.End();
     }

     //------------------------------------------------------------------------------------------------
  void ProcessTransform()
     {
         //--- Rotation (prevent from looking straigh up or down)
         vector angles = Math3D.MatrixToAngles(transform);
         angles += rotDelta * timeSlice;
         angles[1] = Math.Clamp(angles[1], -89, 89);
         Math3D.AnglesToMatrix(angles, transform);
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_ManualCameraParam(vector transformOriginalIn[4], float timeSliceIn, BaseWorld worldIn, float fovIn, bool isManualInputEnabledIn, vector velocityOriginalIn, EManualCameraFlag flagIn)
     {
         timeSlice = timeSliceIn;
         //isDirty = timeSlice != 0;
         multiplier[0] = timeSliceIn;
         multiplier[1] = timeSliceIn;
         flag = flagIn;
         isManualInputEnabled = isManualInputEnabledIn;
         fov = fovIn;
         fovOriginal = fovIn;
         world = worldIn;
         transformOriginal = transformOriginalIn;
         velocityOriginal = velocityOriginalIn;
         Math3D.MatrixCopy(transformOriginal, transform);
         rotOriginal = Math3D.MatrixToAngles(transformOriginal);
     }
 }