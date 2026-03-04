 //------------------------------------------------------------------------------------------------
 //------------------------------------------------------------------------------------------------
 class SCR_Global
 {
     static Widget m_DebugLayoutCanvas;

     static IEntity g_TraceFilterEnt = null;

     static ref set<IEntity> g_TraceFilterList = new set<IEntity>();

     static float g_fPhysicsHz = 60;

     //------------------------------------------------------------------------------------------------
     static bool IsScope2DEnabled()
     {
         BaseContainer gameplaySettings = GetGame().GetGameUserSettings().GetModule("SCR_GameplaySettings");
         if (gameplaySettings)
         {
             bool bval;
             if (gameplaySettings.Get("m_b2DScopes", bval))
                 return bval;
         }

         // 2d scopes enabled by default = supresses PIP which is less performant
         return true;
     }

     //------------------------------------------------------------------------------------------------
     static string GetPlatformName(PlatformKind kind)
     {
         switch (kind)
         {
             case PlatformKind.NONE:
                 return "platform-windows";

             case PlatformKind.PSN:
                 return "platform-playstation";

             case PlatformKind.XBOX:
                 return "platform-xbox";

             case PlatformKind.STEAM:
                 return "platform-windows";
         }

         return string.Empty;
     }

     //------------------------------------------------------------------------------------------------
     static string GetProfileName()
     {
         string name;

         if (GetGame().GetBackendApi().IsLocalPlatformAssigned())
         {
             Print("Profile - Using Local Platform Name!");
             name = GetGame().GetBackendApi().GetCredentialsItem(EBackendCredentials.EBCRED_PLATFORMUID);
             return name;
         }

         BaseContainer gameplaySettings = GetGame().GetGameUserSettings().GetModule("SCR_GameplaySettings");
         if (gameplaySettings)
         {
             gameplaySettings.Get("m_sProfileName", name);
         }

         if (name == string.Empty)
         {
             name = System.GetProfileName();
             if (name == string.Empty)
             {
                 Print("Profile - Using MachineName!");
                 name = System.GetMachineName();
             }
         }

         return name;
     }

     //------------------------------------------------------------------------------------------------
  // unused (and to fix!)
     static vector FixVector180(vector vec)
     {
         for (int a = 0; a < 3; a++)
         {
             float v = vec[a];
             while (v > 180)
                 v -= 360;
             while (v < -180)
                 v += 360;
             vec[a] = v;
         }

         return vec;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static vector GetVectorClampedToWorldBounds(vector pos)
     {
         if (!GetGame().GetWorldEntity().GetWorld().IsOcean())
             return pos;

         if (!GetGame().GetWorldEntity())
             return pos;

         vector min, max;
         GetGame().GetWorldEntity().GetWorldBounds(min, max);

         for (int a = 0; a < 3; a++)
         {
             if (pos[a] < min[a])
                 pos[a] = min[a];
             if (pos[a] > max[a])
                 pos[a] = max[a];
         }

         return pos;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool GetIntersectPointPlane(vector start, vector end, out vector result, float height)
     {
         vector dir = end - start;
         float dist = dir.NormalizeSize();

         vector mins = start;
         vector maxs = start;

         for (int a = 0; a < 3; a++)
         {
             if (a == 1)
                 continue;

             float val1 = start[a];
             float val2 = end[a];
             if (val1 < mins[a])
                 mins[a] = val1;
             if (val2 < mins[a])
                 mins[a] = val2;
             if (val1 > maxs[a])
                 maxs[a] = val1;
             if (val2 > maxs[a])
                 maxs[a] = val2;
         }
         mins[1] = height;
         maxs[1] = height;
         float intersectPct = Math3D.IntersectionRayBox(start, end, mins - "1 0 1", maxs + "1 0 1");
         if (intersectPct < 0)
             return false;

         result = dir * dist * intersectPct + start;

         return true;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void GetDistForHUD(float dist, bool imperial, out string tgtDist, out string distFormat)
     {
         if (imperial)
         {
             dist *= 0.9144; // To yards
             if (dist >= 1000)
             {
                 dist /= 1760; // To miles
                 dist = Math.Floor(dist * 10) * 0.1;
                 tgtDist = dist.ToString();
                 distFormat = "mi";
             }
             else if (dist >= 100)
             {
                 dist = Math.Floor(dist * 10) * 0.1;
                 tgtDist = dist.ToString();
                 distFormat = "yd";
             }
             else if (dist >= 1)
             {
                 dist = Math.Floor(dist * 100) * 0.01;
                 tgtDist = dist.ToString();
                 distFormat = "yd";
             }
             else
             {
                 dist = Math.Floor(dist * 360) * 0.1;
                 tgtDist = dist.ToString();
                 distFormat = "in";
             }
         }
         else
         {
             if (dist >= 1000)
             {
                 dist /= 1000; // To kilometers
                 dist = Math.Floor(dist * 10) * 0.1;
                 tgtDist = dist.ToString();
                 distFormat = "km";
             }
             else if (dist >= 100)
             {
                 dist = Math.Floor(dist * 10) * 0.1;
                 tgtDist = dist.ToString();
                 distFormat = "m";
             }
             else if (dist >= 1)
             {
                 dist = Math.Floor(dist * 100) * 0.01;
                 tgtDist = dist.ToString();
                 distFormat = "m";
             }
             else
             {
                 dist = Math.Floor(dist * 1000) * 0.1;
                 tgtDist = dist.ToString();
                 distFormat = "cm";
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  static bool FilterCallback_IgnoreNotInList(notnull IEntity target)
     {
         if (g_TraceFilterList.Contains(target))
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool FilterCallback_IgnoreCharactersWithChildren(notnull IEntity target)
     {
         while (target)
         {
             if (ChimeraCharacter.Cast(target))
                 return false;

             target = target.GetParent();
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool FilterCallback_IgnoreCharacters(notnull IEntity target)
     {
         if (ChimeraCharacter.Cast(target))
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool FilterCallback_IgnoreEntityWithChildren(notnull IEntity target, vector rayorigin, vector raydirection)
     {
         if (g_TraceFilterEnt == null)
             return true;

         while (target)
         {
             if (target == g_TraceFilterEnt)
                 return false;

             target = target.GetParent();
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool FilterCallback_IgnoreAllButEntityWithChildren(notnull IEntity target, vector rayorigin, vector raydirection)
     {
         if (g_TraceFilterEnt == null)
             return false;

         while (target)
         {
             if (target == g_TraceFilterEnt)
                 return true;

             target = target.GetParent();
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool FilterCallback_IgnoreAllButEntity(notnull IEntity target, vector rayorigin, vector raydirection)
     {
         if (g_TraceFilterEnt == null)
             return false;

         if (target == g_TraceFilterEnt)
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool FilterCallback_IgnoreAllButMeleeAttackable(notnull IEntity target, vector rayorigin, vector raydirection)
     {
         typename type = target.Type();

         if (ChimeraCharacter == type)
             return true;

         if (Tree == type)
             return true;

         if (Building == type)
             return true;

         if (target.FindComponent(DamageManagerComponent))
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool FilterCallback_IgnoreAllButBuildingRegions(notnull IEntity target, vector rayorigin, vector raydirection)
     {
         typename type = target.Type();
         if (SCR_BuildingRegionEntity == type)
             return true;

         if (SCR_DestructibleBuildingEntity == type)
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static float GetScaledStructuralDamage(float damage, EDamageType type)
     {
         float dmgScale = 1;

         switch (type)
         {
             case EDamageType.REGENERATION:
             case EDamageType.BLEEDING:
             {
                 dmgScale = 0;
                 break;
             }
         }

         return damage * dmgScale;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static int GetChildIndex(IEntity ent)
     {
         IEntity parent = ent.GetParent();
         if (!parent)
             return -1;

         int index = 0;
         IEntity child = parent.GetChildren();
         while (child)
         {
             if (child == ent)
                 return index;

             index++;
             child = child.GetSibling();
         }

         return -1;
     }

     //------------------------------------------------------------------------------------------------
  static array<ref ParamEnum> GetBonesAsParamEnums(IEntity entity)
     {
         array<ref ParamEnum> retEnums = new array<ref ParamEnum>;
         array<string> boneNames = new array<string>;
         Animation anim = entity.GetAnimation();
         anim.GetBoneNames(boneNames);

         retEnums.Insert(new ParamEnum("NONE", "-1", "")); // Always have NONE as an option
         foreach (string s : boneNames)
         {
             int nodeid = anim.GetBoneIndex(s);
             retEnums.Insert(new ParamEnum(s, nodeid.ToString(), ""));
         }

         return retEnums;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void SetEntityAsChildToParent(IEntity parent, IEntity child)
     {
         vector parentMat[4];
         parent.GetTransform(parentMat);

         vector childMat[4];
         child.GetTransform(childMat);

         vector childLocalMat[4];
         Math3D.MatrixInvMultiply4(parentMat, childMat, childLocalMat);
         child.SetTransform(childLocalMat);

         parent.AddChild(child, -1);
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static vector ScaleVectorByVector(vector inputVec, vector scaleVec)
     {
         vector result = vector.Zero;

         result[0] = inputVec[0] * scaleVec[0];
         result[1] = inputVec[1] * scaleVec[1];
         result[2] = inputVec[2] * scaleVec[2];

         return result;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static float GetGlobalYawForMat(vector mat[4])
     {
         vector fw = mat[0] * vector.Up;
         fw.Normalize();
         vector angs = fw.VectorToAngles();

         return angs[0];
     }

     //------------------------------------------------------------------------------------------------
     // Lerps between 2 matrices
     // unused
     static void LerpMatrix(vector mat1[4], vector mat2[4], out vector matOut[4], float pct)
     {
         if (pct == 0)
         {
             matOut[0] = mat1[0];
             matOut[1] = mat1[1];
             matOut[2] = mat1[2];
             matOut[3] = mat1[3];
             return;
         }
         else if (pct == 1)
         {
             matOut[0] = mat2[0];
             matOut[1] = mat2[1];
             matOut[2] = mat2[2];
             matOut[3] = mat2[3];
             return;
         }

         float q1[4], q2[4], qOut[4];
         Math3D.MatrixToQuat(mat1, q1);
         Math3D.MatrixToQuat(mat2, q2);
         Math3D.QuatLerp(qOut, q1, q2, pct);
         Math3D.QuatToMatrix(qOut, matOut);
         matOut[3] = (mat2[3] - mat1[3]) * pct + mat1[3];
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void GetLocalMatrix(vector parentMat[4], vector childMat[4])
     {
         vector childOrigMat[4];
         Math3D.MatrixCopy(childMat, childOrigMat);
         Math3D.MatrixInvMultiply4(parentMat, childOrigMat, childMat);
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void GetWorldBoundsForEntity(vector mat[4], out vector mins, out vector maxs)
     {
         vector pt1, pt2;
         pt1 = mins.Multiply4(mat);
         pt2 = maxs.Multiply4(mat);
         mins = pt1;
         maxs = pt2;
         for (int i = 0; i < 3; i++)
         {
             if (pt1[i] < mins[i])
                 mins[i] = pt1[i];
             if (pt2[i] < mins[i])
                 mins[i] = pt2[i];
             if (pt1[i] > maxs[i])
                 maxs[i] = pt1[i];
             if (pt2[i] > maxs[i])
                 maxs[i] = pt2[i];
         }
     }

     //------------------------------------------------------------------------------------------------
  static void GetWorldBoundsWithChildren(IEntity entity, out vector min, out vector max, bool isChild = false)
     {
         if (!entity)
             return;

         if (!isChild)
         {
             min = Vector(float.MAX, float.MAX, float.MAX);
             max = -Vector(float.MAX, float.MAX, float.MAX);
         }

         if (entity.GetVObject())
         {
             vector entityMin, entityMax;
             entity.GetWorldBounds(entityMin, entityMax);

             min[0] = Math.Min(min[0], entityMin[0]);
             min[1] = Math.Min(min[1], entityMin[1]);
             min[2] = Math.Min(min[2], entityMin[2]);

             max[0] = Math.Max(max[0], entityMax[0]);
             max[1] = Math.Max(max[1], entityMax[1]);
             max[2] = Math.Max(max[2], entityMax[2]);
         }

         entity = entity.GetChildren();
         while (entity)
         {
             GetWorldBoundsWithChildren(entity, min, max, true);
             entity = entity.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool GetObjectAtOffsetFromObject(IEntity parent, IEntity child, vector localPos, vector localAng, float tolerancePos, float toleranceAng)
     {
         vector parentMat[4];
         parent.GetTransform(parentMat);

         vector offsetMat[4];
         Math3D.AnglesToMatrix(localAng, offsetMat);
         offsetMat[3] = localPos;

         vector parentOffsetMat[4];
         Math3D.MatrixMultiply4(parentMat, offsetMat, parentOffsetMat);

         vector childMat[4];
         child.GetTransform(childMat);

         vector relativeMat[4];
         Math3D.MatrixInvMultiply4(parentOffsetMat, childMat, relativeMat);

         vector pos = relativeMat[3];
         if (pos.Length() > tolerancePos)
             return false;

         float cross;
         vector dir;

         dir = relativeMat[0];
         cross = dir * vector.Right;
         if (Math.AbsFloat(cross) > toleranceAng)
             return false;

         dir = relativeMat[1];
         cross = dir * vector.Up;
         if (Math.AbsFloat(cross) > toleranceAng)
             return false;

         dir = relativeMat[2];
         cross = dir * vector.Forward;
         if (Math.AbsFloat(cross) > toleranceAng)
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool IntersectBoxPoint(vector pos, vector mins, vector maxs)
     {
         for (int i = 0; i < 3; i++)
         {
             if (pos[i] > maxs[i])
                 return false;
             if (pos[i] < mins[i])
                 return false;
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool IntersectBoxSphere(vector center, float radius, vector mins, vector maxs, out float intersectDist = 0)
     {
         float dist_sq = -Math.Pow(radius, 2);

         if (center[0] < mins[0])
             dist_sq += Math.Pow(center[0] - mins[0], 2);
         else if (center[0] > maxs[0])
             dist_sq += Math.Pow(center[0] - maxs[0], 2);
         if (center[1] < mins[1])
             dist_sq += Math.Pow(center[1] - mins[1], 2);
         else if (center[1] > maxs[1])
             dist_sq += Math.Pow(center[1] - maxs[1], 2);
         if (center[2] < mins[2])
             dist_sq += Math.Pow(center[2] - mins[2], 2);
         else if (center[2] > maxs[2])
             dist_sq += Math.Pow(center[2] - maxs[2], 2);

         if (dist_sq < 0)
             intersectDist = -Math.Sqrt(-dist_sq);
         else
             intersectDist = Math.Sqrt(dist_sq);

         if (dist_sq < 0)
             return true;
         else
             return false;
     }

     //------------------------------------------------------------------------------------------------
  static float FractionOf(float input, float fracOf)
     {
         float result = input / fracOf;

         while (result >= 1)
             result -= 1;
         while (result <= -1)
             result += 1;

         return result;
     }

     //------------------------------------------------------------------------------------------------
  // local usage, can be protected
     static float ClampToGrid(float input, float grid)
     {
         float frac = FractionOf(input, grid);

         if (frac >= 0.5) input += (1 - frac) * grid;
         else if (frac > 0 && frac < 0.5) input -= frac * grid;
         else if (frac <= -0.5) input -= (1 + frac) * grid;
         else if (frac < 0 && frac > -0.5) input += -frac * grid;

         return input;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static vector GetDirectionAngles(vector viewMat[4], vector posTo)
     {
         vector dir = posTo - viewMat[3];
         dir.Normalize();
         vector dirAng = dir.InvMultiply3(viewMat);
         dirAng = dirAng.VectorToAngles();
         dirAng = dirAng.MapAngles();

         return dirAng;
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_Math.fmod or Math.Repeat instead")]
     static float fmod(float dividend, float divisor)
     {
         if (divisor == 0)
             return 0;
         return dividend - Math.Floor(dividend/divisor) * divisor;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void WorldClampMatrixWithinBounds(vector mat[4], vector mins, vector maxs)
     {
         vector pos = mat[3];
         for (int i = 0; i < 3; i++)
         {
             if (pos[i] > maxs[i])
                 pos[i] = maxs[i];
             if (pos[i] < mins[i])
                 pos[i] = mins[i];
         }
         mat[3] = pos;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static int VectorToRGBA255(vector colorVec, float alpha)
     {
         float x,y,z;
         int r,g,b,a;

         x = colorVec[0];
         y = colorVec[1];
         z = colorVec[2];

         a = (int)alpha << 24;
         r = (int)x << 16;
         g = (int)y << 8;
         b = z;

         return r | g | b | a;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void WorldClampObjectAndMatrixWithinBounds(GenericEntity ent, vector mat[4], vector mins, vector maxs, vector gridSize)
     {
         bool snapToGrid = false;
         if (gridSize != vector.Zero)
             snapToGrid = true;

         vector entmins, entmaxs;
         ent.GetBounds(entmins, entmaxs);
         vector point1 = entmins.Multiply3(mat);
         vector point2 = entmaxs.Multiply3(mat);
         entmins = point1;
         entmaxs = point2;
         for (int i = 0; i < 3; i++)
         {
             if (point1[i] < entmins[i])
                 entmins[i] = point1[i];
             if (point2[i] < entmins[i])
                 entmins[i] = point2[i];
             if (point1[i] > entmaxs[i])
                 entmaxs[i] = point1[i];
             if (point2[i] > entmaxs[i])
                 entmaxs[i] = point2[i];
         }

         vector pos = mat[3];
         for (int i = 0; i < 3; i++)
         {
             float posAxis = pos[i];
             float min = entmins[i];
             float max = entmaxs[i];
             float grid = gridSize[i];

             if (snapToGrid)
             {
                 min = ClampToGrid(min, grid * 0.5);
                 max = ClampToGrid(max, grid * 0.5);
             }
             if (pos[i] > maxs[i] - max)
                 posAxis = maxs[i] - max;
             if (pos[i] < mins[i] - min)
                 posAxis = mins[i] - min;

             pos[i] = posAxis;
         }
         mat[3] = pos;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void LocalClampObjectAndMatrixWithinBounds(vector localMat[4], GenericEntity ent, vector origMat[4], vector localMins, vector localMaxs, vector gridSize)
     {
         vector mat[4];
         Math3D.MatrixInvMultiply4(localMat, origMat, mat);

         bool snapToGrid = false;
         if (gridSize != vector.Zero)
             snapToGrid = true;

         vector entmins, entmaxs;
         ent.GetBounds(entmins, entmaxs);
         vector point1 = entmins.Multiply3(mat);
         vector point2 = entmaxs.Multiply3(mat);
         entmins = point1;
         entmaxs = point2;
         for (int i = 0; i < 3; i++)
         {
             if (point1[i] < entmins[i])
                 entmins[i] = point1[i];
             if (point2[i] < entmins[i])
                 entmins[i] = point2[i];
             if (point1[i] > entmaxs[i])
                 entmaxs[i] = point1[i];
             if (point2[i] > entmaxs[i])
                 entmaxs[i] = point2[i];
         }

         vector pos = mat[3];
         for (int i = 0; i < 3; i++)
         {
             float posAxis = pos[i];
             float min = entmins[i];
             float max = entmaxs[i];
             float grid = gridSize[i];

             if (snapToGrid)
             {
                 min = ClampToGrid(min, grid * 0.5);
                 max = ClampToGrid(max, grid * 0.5);
             }
             if (pos[i] > localMaxs[i] - max)
                 posAxis = localMaxs[i] - max;
             if (pos[i] < localMins[i] - min)
                 posAxis = localMins[i] - min;

             pos[i] = posAxis;
         }
         mat[3] = pos;

         Math3D.MatrixMultiply4(localMat, mat, origMat);
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void WorldSnapMatrix(vector mat[4], vector gridSize)
     {
         // Snap position
         if (gridSize != vector.Zero)
         {
             vector tempPos = mat[3];
             for (int i = 0; i < 3; i++)
             {
                 float grid = gridSize[i];
                 float pos = ClampToGrid(tempPos[i], grid);

                 tempPos[i] = pos;
             }
             mat[3] = tempPos;
         }

         // Create world oriented normals
         vector norms[6];
         norms[0] = "1 0 0";
         norms[1] = "0 1 0";
         norms[2] = "0 0 1";
         norms[3] = "-1 0 0";
         norms[4] = "0 -1 0";
         norms[5] = "0 0 -1";

         // Snap orientation
         vector mat0 = mat[0];
         vector mat1 = mat[1];
         vector mat2 = mat[2];
         vector bestVecs[3];
         float bestVecsVal[3];
         bestVecsVal[0] = -1;
         bestVecsVal[1] = -1;
         bestVecsVal[2] = -1;

         for (int i = 0; i < 6; i++)
         {
             vector norm = norms[i];
             float calc = mat0 * norm;
             if (calc > bestVecsVal[0])
             {
                 bestVecsVal[0] = calc;
                 bestVecs[0] = norms[i];
             }

             calc = mat1 * norm;
             if (calc > bestVecsVal[1])
             {
                 bestVecsVal[1] = calc;
                 bestVecs[1] = norms[i];
             }

             calc = mat2 * norm;
             if (calc > bestVecsVal[2])
             {
                 bestVecsVal[2] = calc;
                 bestVecs[2] = norms[i];
             }
         }

         float bestAxisVal1 = -1;
         int bestAxisNum1;
         for (int i = 0; i < 3; i++)
         {
             if (bestVecsVal[i] > bestAxisVal1)
             {
                 bestAxisVal1 = bestVecsVal[i];
                 bestAxisNum1 = i;
             }
         }
         float bestAxisVal2 = -1;
         int bestAxisNum2;
         for (int i = 0; i < 3; i++)
         {
             if (i == bestAxisNum1)
                 continue;

             if (bestVecsVal[i] > bestAxisVal2)
             {
                 bestAxisVal2 = bestVecsVal[i];
                 bestAxisNum2 = i;
             }
         }

         vector bestAxis1 = bestVecs[bestAxisNum1];
         vector bestAxis2 = bestVecs[bestAxisNum2];
         vector bestAxis3;
         if (bestAxisNum1 == 2 && bestAxisNum2 == 0)
             bestAxis3 = bestAxis1 * bestAxis2;
         else if (bestAxisNum1 == 0 && bestAxisNum2 == 2)
             bestAxis3 = bestAxis2 * bestAxis1;
         else if (bestAxisNum2 < bestAxisNum1)
             bestAxis3 = bestAxis2 * bestAxis1;
         else
             bestAxis3 = bestAxis1 * bestAxis2;

         for (int i = 0; i < 3; i++)
         {
             if (i == bestAxisNum1 || i == bestAxisNum2)
                 continue;

             mat[i] = bestAxis3;
             break;
         }
         mat[bestAxisNum1] = bestAxis1;
         mat[bestAxisNum2] = bestAxis2;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void LocalSnapMatrix(vector localMat[4], vector origMat[4], vector gridSize)
     {
         vector mat[4];
         Math3D.MatrixInvMultiply4(localMat, origMat, mat);

         // Snap position
         if (gridSize != vector.Zero)
         {
             vector tempPos = mat[3];
             for (int i = 0; i < 3; i++)
             {
                 float grid = gridSize[i];
                 float pos = ClampToGrid(tempPos[i], grid);

                 tempPos[i] = pos;
             }
             mat[3] = tempPos;
         }

         // Create world oriented normals
         vector norms[6];
         norms[0] = "1 0 0";
         norms[1] = "0 1 0";
         norms[2] = "0 0 1";
         norms[3] = "-1 0 0";
         norms[4] = "0 -1 0";
         norms[5] = "0 0 -1";

         // Snap orientation
         vector mat0 = mat[0];
         vector mat1 = mat[1];
         vector mat2 = mat[2];
         vector bestVecs[3];
         float bestVecsVal[3];
         bestVecsVal[0] = -1;
         bestVecsVal[1] = -1;
         bestVecsVal[2] = -1;

         for (int i = 0; i < 6; i++)
         {
             vector norm = norms[i];
             float calc = mat0 * norm;
             if (calc > bestVecsVal[0])
             {
                 bestVecsVal[0] = calc;
                 bestVecs[0] = norms[i];
             }

             calc = mat1 * norm;
             if (calc > bestVecsVal[1])
             {
                 bestVecsVal[1] = calc;
                 bestVecs[1] = norms[i];
             }

             calc = mat2 * norm;
             if (calc > bestVecsVal[2])
             {
                 bestVecsVal[2] = calc;
                 bestVecs[2] = norms[i];
             }
         }

         float bestAxisVal1 = -1;
         int bestAxisNum1;
         for (int i = 0; i < 3; i++)
         {
             if (bestVecsVal[i] > bestAxisVal1)
             {
                 bestAxisVal1 = bestVecsVal[i];
                 bestAxisNum1 = i;
             }
         }
         float bestAxisVal2 = -1;
         int bestAxisNum2;
         for (int i = 0; i < 3; i++)
         {
             if (i == bestAxisNum1)
                 continue;

             if (bestVecsVal[i] > bestAxisVal2)
             {
                 bestAxisVal2 = bestVecsVal[i];
                 bestAxisNum2 = i;
             }
         }

         vector bestAxis1 = bestVecs[bestAxisNum1];
         vector bestAxis2 = bestVecs[bestAxisNum2];
         vector bestAxis3;
         if (bestAxisNum1 == 2 && bestAxisNum2 == 0)
             bestAxis3 = bestAxis1 * bestAxis2;
         else if (bestAxisNum1 == 0 && bestAxisNum2 == 2)
             bestAxis3 = bestAxis2 * bestAxis1;
         else if (bestAxisNum2 < bestAxisNum1)
             bestAxis3 = bestAxis2 * bestAxis1;
         else
             bestAxis3 = bestAxis1 * bestAxis2;

         for (int i = 0; i < 3; i++)
         {
             if (i == bestAxisNum1 || i == bestAxisNum2)
                 continue;

             mat[i] = bestAxis3;
             break;
         }
         mat[bestAxisNum1] = bestAxis1;
         mat[bestAxisNum2] = bestAxis2;

         Math3D.MatrixMultiply4(localMat, mat, origMat);
     }

     //------------------------------------------------------------------------------------------------
  // can be protected
     static float GetMatAndBoundsSurfaceAreaInDir(IEntity ent, vector dir, vector mat[4], vector mins, vector maxs)
     {
         float min_x, min_y, min_z;
         float max_x, max_y, max_z;
         min_x = mins[0];
         min_y = mins[1];
         min_z = mins[2];
         max_x = maxs[0];
         max_y = maxs[1];
         max_z = maxs[2];

         float surfArea_top = Math.AbsFloat(max_z - min_z) * Math.AbsFloat(max_x - min_x);
         float surfArea_right = Math.AbsFloat(max_z - min_z) * Math.AbsFloat(max_y - min_y);
         float surfArea_front = Math.AbsFloat(max_x - min_x) * Math.AbsFloat(max_y - min_y);

         vector objRt = mat[0];
         vector objUp = mat[1];
         vector objFw = mat[2];

         float result = Math.AbsFloat(dir * objFw * surfArea_front);
         result += Math.AbsFloat(dir * objRt * surfArea_right);
         result += Math.AbsFloat(dir * objUp * surfArea_top);

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static float GetSurfaceAreaInDir(IEntity ent, vector dir)
     {
         vector matObj[4];
         ent.GetTransform(matObj);

         vector mins, maxs;
         ent.GetBounds(mins, maxs); // Get the bounding box

         return GetMatAndBoundsSurfaceAreaInDir(ent, dir, matObj, mins, maxs);
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static vector GetEntityCenterLocal(IEntity ent)
     {
         vector entMins, entMaxs;
         ent.GetBounds(entMins, entMaxs);

         return (entMaxs + entMins) * 0.5;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool GetStringContainsInvalidCharacters(string s)
     {
         int ascii_FwSlash = 47;
         int ascii_BkSlash = 92;
         int ascii_Quote = 34;

         if (s == "") return true;
         else if (s.Contains("~")) return true;
         else if (s.Contains("#")) return true;
         else if (s.Contains("%")) return true;
         else if (s.Contains("\"")) return true;
         else if (s.Contains("&")) return true;
         else if (s.Contains("*")) return true;
         else if (s.Contains("{")) return true;
         else if (s.Contains("}")) return true;
         else if (s.Contains(ascii_BkSlash.AsciiToString())) return true;
         else if (s.Contains(":")) return true;
         else if (s.Contains("<")) return true;
         else if (s.Contains(">")) return true;
         else if (s.Contains("?")) return true;
         else if (s.Contains(ascii_FwSlash.AsciiToString())) return true;
         else if (s.Contains("|")) return true;
         else if (s.Contains(ascii_Quote.AsciiToString())) return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void DisplayEntityNameText(IEntity ent, int color, int camIndex)
     {
         vector pos = SCR_EntityHelper.GetEntityCenterWorld(ent);

         vector textMat[4];
         ent.GetWorld().GetCamera(camIndex, textMat);

         float distScale = vector.Distance(textMat[3], pos) * 0.07;
         distScale = Math.Clamp(distScale, 0.5, 10);

         float textEndSize = (0.2 * distScale) / vector.Distance(textMat[3], ent.GetOrigin());
         if (textEndSize < 0.005)
             return;

         textMat[3] = pos + textMat[1] * 0.7;
         CreateSimpleText(ent.GetName(), textMat, 0.2 * distScale, color, ShapeFlags.NOZBUFFER | ShapeFlags.ONCE, null);
     }

     //------------------------------------------------------------------------------------------------
  protected static void SetHierarchyChildVelocity(notnull IEntity ent, vector newVelocity, bool recursive = true)
     {
         Physics entPhys = ent.GetPhysics();
         if (entPhys)
         {
             if (entPhys.IsDynamic())
                 entPhys.SetVelocity(newVelocity);
         }

         IEntity child = ent.GetChildren();
         while (child)
         {
             SetHierarchyChildVelocity(child, newVelocity, recursive);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void SetHierarchyVelocity(notnull IEntity ent, vector newVelocity)
     {
         Physics entPhys = ent.GetPhysics();
         if (entPhys)
         {
             if (entPhys.IsDynamic())
                 entPhys.SetVelocity(newVelocity);
         }

         IEntity child = ent.GetChildren();
         while (child)
         {
             SetHierarchyChildVelocity(child, newVelocity, true);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  protected static void SetHierarchyChildAngularVelocity(notnull IEntity ent, vector newAngularVelocity, IEntity entFrom, bool recursive = true)
     {
         Physics entPhys = ent.GetPhysics();
         Physics entFromPhys = entFrom.GetPhysics();
         if (entPhys)
         {
             if (entPhys.IsDynamic())
             {
                 entPhys.SetAngularVelocity(newAngularVelocity);
                 if (entFrom && entFromPhys && entFromPhys.IsDynamic())
                 {
                     vector velAt = entFromPhys.GetVelocityAt(ent.GetOrigin());
                     entPhys.SetVelocity(velAt);
                 }
             }

         }

         IEntity child = ent.GetChildren();
         while (child)
         {
             SetHierarchyChildAngularVelocity(child, newAngularVelocity, entFrom, recursive);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void SetHierarchyAngularVelocity(notnull IEntity ent, vector newAngularVelocity)
     {
         Physics entPhys = ent.GetPhysics();
         if (entPhys && entPhys.IsDynamic())
             entPhys.SetAngularVelocity(newAngularVelocity);

         IEntity child = ent.GetChildren();
         while (child)
         {
             SetHierarchyChildAngularVelocity(child, newAngularVelocity, ent, true);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  protected static void SetHierarchyChildBodyActive(notnull IEntity ent, ActiveState activeState, bool recursive = true, bool resetVelocity = false)
     {
         Physics entPhys = ent.GetPhysics();
         if (entPhys)
         {
             if (entPhys.IsDynamic())
             {
                 entPhys.SetActive(activeState);
                 if (resetVelocity)
                 {
                     entPhys.SetVelocity( vector.Zero );
                     entPhys.SetAngularVelocity( vector.Zero );
                 }

             }
         }

         IEntity child = ent.GetChildren();
         while (child)
         {
             SetHierarchyChildBodyActive(child, activeState, resetVelocity);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static void SetHierarchyBodyActive(notnull IEntity ent, ActiveState activeState, bool resetVelocity = false)
     {
         Physics entPhys = ent.GetPhysics();
         if (entPhys)
         {
             if (entPhys.IsDynamic())
             {

                 entPhys.SetActive(activeState);
                 if (resetVelocity)
                 {
                     entPhys.SetVelocity( vector.Zero );
                     entPhys.SetAngularVelocity( vector.Zero );
                 }

             }
         }

         IEntity child = ent.GetChildren();
         while (child)
         {
             SetHierarchyChildBodyActive(child, activeState, true);
             child = child.GetSibling();
         }
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static bool IsAnyInherited(notnull IEntity entity, array<typename> typenames)
     {
         bool ret;
         for (int i = 0; i < typenames.Count(); i++)
         {
             ret = ret || entity.IsInherited( typenames.Get(i) );
         }

         return ret;
     }

     //------------------------------------------------------------------------------------------------
     static bool ApplyDamage(IEntity entity, IEntity weapon, float dmg, EDamageType dmgType, string hitZone, SurfaceProperties surface, int nodeIdx)
     {
         vector hitPosDirNorm[3];

         /*
         Tree tree = Tree.Cast(entity);
         if (tree)
         {
             tree.OnDamage(
                 dmg,
                 dmgType,
                 entity,
                 hitPosDirNorm,
                 weapon,
                 null,
                 hitZone,
                 0, 0);

             return true;
         }
         else
         {
             Building building = Building.Cast(entity);
             if (building)
             {
                 building.OnDamage(
                 dmg,
                 dmgType,
                 entity,
                 hitPosDirNorm,
                 weapon,
                 null,
                 hitZone,
                 0, 0);

                 return true;
             }
         }
         */
         GenericEntity genEnt = GenericEntity.Cast(entity);
         if (!genEnt)
             return false;

         DamageManagerComponent dmc = DamageManagerComponent.Cast(genEnt.FindComponent(DamageManagerComponent));
         if (dmc)
         {
             bool useDefaultHZ = false;
             if (hitZone.Empty)
                 useDefaultHZ = true;

             if (useDefaultHZ && dmc.GetDefaultHitZone())
                 hitZone = dmc.GetDefaultHitZone().GetName();



             return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static ActionsManagerComponent FindActionsManagerComponent( IEntity entity, bool activeOnly = true)
     {
         auto genericEntity = GenericEntity.Cast(entity);
         if (!genericEntity)
             return null;

         auto actionsManager = ActionsManagerComponent.Cast(genericEntity.FindComponent(ActionsManagerComponent));
         if (actionsManager != null)
         {
             if (activeOnly && !actionsManager.IsActive())
                 return null;

             return actionsManager;
         }

         return null;
     }

     //------------------------------------------------------------------------------------------------
  // unused
     static SCR_EditorActionsManagerComponent FindEditorActionsManagerComponent( IEntity entity)
     {
         auto genericEntity = GenericEntity.Cast(entity);
         if (!genericEntity)
             return null;

         auto actionsManager = SCR_EditorActionsManagerComponent.Cast(genericEntity.FindComponent(SCR_EditorActionsManagerComponent));
         if (actionsManager != null)
             return actionsManager;

         return null;
     }

     //------------------------------------------------------------------------------------------------
  static bool GetModelAndRemapFromResource(ResourceName resourcePath, out ResourceName modelPath, out string remap)
     {
         modelPath = ResourceName.Empty;
         remap = string.Empty;

         if (resourcePath == ResourceName.Empty)
             return false;

         Resource resource = Resource.Load(resourcePath);
         if (!resource)
             return false;

         BaseResourceObject prefabBase = resource.GetResource();
         if (!prefabBase)
             return false;

         // Is a model not a prefab
         if (prefabBase.ToVObject())
         {
             modelPath = resourcePath;

             return true;
         }

         // Prefab
         BaseContainer prefabSrc = prefabBase.ToBaseContainer();
         if (!prefabSrc)
             return false;

         BaseContainerList components = prefabSrc.GetObjectArray("components");
         if (!components)
             return false;

         BaseContainer meshComponent = null;
         for (int c = components.Count() - 1; c >= 0; c--)
         {
             meshComponent = components.Get(c);
             if (meshComponent.GetClassName() == "MeshObject")
                 break;

             meshComponent = null;
         }

         if (!meshComponent)
             return false;

         meshComponent.Get("Object", modelPath);
         BaseContainerList materialsRemap = meshComponent.GetObjectArray("Materials");
         if (materialsRemap)
         {
             ResourceName matSrc = ResourceName.Empty;
             ResourceName matTgt = ResourceName.Empty;
             for (int m = materialsRemap.Count() - 1; m >= 0; m--)
             {
                 BaseContainer material = materialsRemap.Get(m);

                 material.Get("SourceMaterial", matSrc);
                 material.Get("AssignedMaterial", matTgt);
                 remap += "$remap '" + matSrc + "' '" + matTgt + "';";
             }
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool GetResourceContainsComponent(ResourceName resourcePath, string componentClassName, out bool isPrefab)
     {
         isPrefab = false;

         if (resourcePath == ResourceName.Empty)
             return false;

         Resource resource = Resource.Load(resourcePath);
         if (!resource)
             return false;

         BaseResourceObject prefabBase = resource.GetResource();
         if (!prefabBase)
             return false;

         // Is not a prefab
         if (!prefabBase.ToEntitySource())
             return false;

         // Prefab
         BaseContainer prefabSrc = prefabBase.ToBaseContainer();
         if (!prefabSrc)
             return false;

         isPrefab = true;

         BaseContainerList components = prefabSrc.GetObjectArray("components");
         if (!components)
             return false;

         BaseContainer component = null;
         for (int c = components.Count() - 1; c >= 0; c--)
         {
             component = components.Get(c);
             if (component.GetClassName() == componentClassName)
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsEditMode()
     {
         ArmaReforgerScripted game = GetGame();
         return !game || !game.InPlayMode();
     }

     //------------------------------------------------------------------------------------------------
  static bool IsEditMode(notnull IEntity entity)
     {
         BaseWorld world = entity.GetWorld();
         return world && world.IsEditMode();
     }

     //------------------------------------------------------------------------------------------------
  static vector ProjWorldEditorMouseScreenToWorld(GenericEntity referenceEntity)
     {
         vector camMat[4];
         referenceEntity.GetWorld().GetCurrentCamera(camMat);
         vector dir = camMat[2];

         #ifdef WORKBENCH
             WorldEditorAPI worldEdAPI = referenceEntity._WB_GetEditorAPI();
             WorkspaceWidget workspace = GetGame().GetWorkspace();
             if (worldEdAPI && workspace)
             {
                 int imouseX = worldEdAPI.GetMousePosX(true);
                 int imouseY = worldEdAPI.GetMousePosY(true);
                 int realScreenX = worldEdAPI.GetScreenWidth();
                 int realScreenY = worldEdAPI.GetScreenHeight();
                 if (imouseX > 0 && imouseX < realScreenX && imouseY > 0 && imouseY < realScreenY)
                 {
                     float xMult = realScreenX / realScreenY;
                     float mouseXPct = (imouseX / realScreenX - 0.5) * xMult;
                     float mouseYPct = imouseY / realScreenY;
                     float width, height;
                     workspace.GetScreenSize(width, height);
                     float mouseX = mouseXPct * width + width * 0.5;
                     float mouseY = mouseYPct * height;
                     workspace.ProjScreenToWorldNative(mouseX, mouseY, dir, referenceEntity.GetWorld(), referenceEntity.GetWorld().GetCurrentCameraId());
                 }
             }
         #endif

         return dir;
     }

     //------------------------------------------------------------------------------------------------
  static bool TeleportLocalPlayer(vector worldPosition, SCR_EPlayerTeleportedReason teleportReason = SCR_EPlayerTeleportedReason.DEFAULT)
     {
         return TeleportPlayer(SCR_PlayerController.GetLocalPlayerId(), worldPosition, teleportReason);
     }

     //------------------------------------------------------------------------------------------------
  static bool TeleportPlayer(int playerId, vector worldPosition, SCR_EPlayerTeleportedReason teleportReason = SCR_EPlayerTeleportedReason.DEFAULT)
     {
         IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
         if (!player)
             return false;

         //~ Player teleport feedback
         PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
         if (playerController)
         {
             SCR_PlayerTeleportedFeedbackComponent teleportFeedback = SCR_PlayerTeleportedFeedbackComponent.Cast(playerController.FindComponent(SCR_PlayerTeleportedFeedbackComponent));
             if (teleportFeedback)
                 teleportFeedback.PlayerTeleported(player, false, teleportReason);
         }

         vector startingPosition = player.GetOrigin();

         //--- When in a vehcile, teleport the vehicle instead
         SCR_CompartmentAccessComponent compartmentAccess = SCR_CompartmentAccessComponent.Cast(player.FindComponent(SCR_CompartmentAccessComponent));
         if (compartmentAccess)
         {
             IEntity vehicle = compartmentAccess.GetVehicle();
             if (vehicle)
                 player = vehicle;
         }

         //--- Apply transformation
         vector transform[4];
         player.GetWorldTransform(transform);
         transform[3] = worldPosition;

         //~ Align to terrain if not a character
         if (!ChimeraCharacter.Cast(player))
             SCR_TerrainHelper.OrientToTerrain(transform);

         BaseGameEntity baseGameEntity = BaseGameEntity.Cast(player);
         if (baseGameEntity)
             baseGameEntity.Teleport(transform);
         else
             player.SetWorldTransform(transform);

         Physics phys = player.GetPhysics();
         if (phys)
         {
             phys.SetVelocity(vector.Zero);
             phys.SetAngularVelocity(vector.Zero);
         }

 #ifdef ENABLE_DIAG
     #ifndef WORKBENCH
         //~ Send notification to all players if player teleported and Diag is enabled
         if (teleportReason == SCR_EPlayerTeleportedReason.DEFAULT && Replication.IsRunning())
             SCR_NotificationsComponent.SendToEveryone(ENotification.PLAYER_TELEPORTED_SELF, SCR_PlayerController.GetLocalPlayerId(), vector.Distance(startingPosition, player.GetOrigin()) * 100);
     #endif
 #endif

         return true;

         /*
         ArmaReforgerScripted game = GetGame();
         if (!world && game)
         {
             world = game.GetWorld();
         }
         if (!world) return false;

         if (!playerEntity) return false;

         ChimeraCharacter character = ChimeraCharacter.Cast(playerEntity);
         if (character)
         {
             CompartmentAccessComponent compartmentAccess = character.GetCompartmentAccessComponent();
             if (compartmentAccess && compartmentAccess.IsInCompartment())
             {
                 BaseCompartmentSlot compartmentSlot = compartmentAccess.GetCompartment();
                 if (compartmentSlot && compartmentSlot.GetOwner())
                 {
                     playerEntity = compartmentSlot.GetOwner();
                 }
             }
         }

         BaseGameEntity baseGameEntity = BaseGameEntity.Cast(playerEntity);
         if (baseGameEntity)
         {
             vector mat[4];
             baseGameEntity.GetWorldTransform(mat);
             mat[3] = worldPosition;
             baseGameEntity.Teleport(mat);
         }
         else
             playerEntity.SetOrigin(worldPosition);
         return true;
         */
     }

     //------------------------------------------------------------------------------------------------
  static void DrawMatrix(vector matrix[4], float scale = 1, ShapeFlags flags = ShapeFlags.ONCE | ShapeFlags.NOZBUFFER, int colorX = Color.RED, int colorY = Color.GREEN, int colorZ = Color.BLUE)
     {
         vector line[2] = {matrix[3], matrix[3] + matrix[0] * scale};
         Shape.CreateLines(colorX, flags, line, 2);

         line = {matrix[3], matrix[3] + matrix[1] * scale};
         Shape.CreateLines(colorY, flags, line, 2);

         line = {matrix[3], matrix[3] + matrix[2] * scale};
         Shape.CreateLines(colorZ, flags, line, 2);
     }

     //------------------------------------------------------------------------------------------------
  static void DrawTrace(TraceParam trace, float traceCoef = -1, ShapeFlags flags = ShapeFlags.ONCE | ShapeFlags.NOZBUFFER | ShapeFlags.NOOUTLINE, int colorEnd = Color.BLACK, int colorIntersect = Color.PINK)
     {
         vector lines[2];
         lines[0] = trace.Start;

         if (traceCoef != -1)
         {
             lines[0] = vector.Lerp(trace.Start, trace.End, traceCoef);
             lines[1] = trace.Start;
             Shape.CreateLines(colorIntersect, flags, lines, 2);
             Shape.CreateSphere(colorIntersect, flags, lines[0], 0.05);
         }

         lines[1] = trace.End;
         Shape.CreateLines(colorEnd, flags, lines, 2);
         Shape.CreateSphere(colorEnd, flags, trace.Start, 0.05);
     }

     //------------------------------------------------------------------------------------------------
  static void SetMaterial(IEntity entity, ResourceName material, bool recursively = true)
     {
         //--- Remap textures of the current mesh
         VObject mesh = entity.GetVObject();
         if (mesh)
         {
             string remap;
             string materials[256];
             int numMats = mesh.GetMaterials(materials);
             for (int i = 0; i < numMats; i++)
             {
                 remap += string.Format("$remap '%1' '%2';", materials[i], material);
             }
             entity.SetObject(mesh, remap);
         }

         //--- Iterate through children
         if (recursively)
         {
             IEntity child = entity.GetChildren();
             while (child)
             {
                 SetMaterial(child, material);
                 child = child.GetSibling();
             }
         }
     }

     //------------------------------------------------------------------------------------------------
  static bool IsChangedMouseAndKeyboard(EInputDeviceType oldDevice, EInputDeviceType newDevice)
     {
         return (oldDevice == EInputDeviceType.KEYBOARD && newDevice == EInputDeviceType.MOUSE) || (oldDevice == EInputDeviceType.MOUSE && newDevice == EInputDeviceType.KEYBOARD);
     }

     //------------------------------------------------------------------------------------------------
  static ResourceName GetRootWorld(ResourceName worldPath = ResourceName.Empty)
     {
         if (!worldPath)
             worldPath = GetResourceName(GetGame().GetWorldFile());

         Resource worldResource = BaseContainerTools.LoadContainer(worldPath);
         if (!worldResource || !worldResource.IsValid())
             return ResourceName.Empty;

         BaseContainer container = worldResource.GetResource().ToBaseContainer();
         ResourceName parent;
         container.Get("Parent", parent);
         if (parent)
             return GetRootWorld(parent);
         else
             return worldPath;
     }

     //------------------------------------------------------------------------------------------------
  static ResourceName GetResourceName(string path)
     {
         Resource meta = BaseContainerTools.LoadContainer(path + ".meta");
         if (!meta || !meta.IsValid())
             return ResourceName.Empty;

         ResourceName resourceName;
         meta.GetResource().ToBaseContainer().Get("Name", resourceName);
         return resourceName;
     }

     //------------------------------------------------------------------------------------------------
  static int GetFrequencies(SCR_GadgetManagerComponent gadgetManager, out notnull set<int> outFrequencies)
     {
         if (!gadgetManager)
             return 0;

         BaseRadioComponent radioComponent;
         array<SCR_GadgetComponent> gadgets = gadgetManager.GetGadgetsByType(EGadgetType.RADIO);
         gadgets.InsertAll(gadgetManager.GetGadgetsByType(EGadgetType.RADIO_BACKPACK));
         foreach (SCR_GadgetComponent gadget : gadgets)
         {
             radioComponent = BaseRadioComponent.Cast(gadget.GetOwner().FindComponent(BaseRadioComponent));

             if (!radioComponent || !radioComponent.IsPowered() || radioComponent.TransceiversCount() == 0)
                 continue;

             for (int i = radioComponent.TransceiversCount() - 1; i >= 0; --i)
             {
                 outFrequencies.Insert(radioComponent.GetTransceiver(i).GetFrequency());
             }
         }
         return outFrequencies.Count();
     }

     //------------------------------------------------------------------------------------------------
  static bool IsAdminRole(EPlayerRole role)
     {
         return SCR_Enum.HasPartialFlag(role, EPlayerRole.ADMINISTRATOR | EPlayerRole.SESSION_ADMINISTRATOR);
     }

     //------------------------------------------------------------------------------------------------
  static bool IsAdmin(int playerID)
     {
         return GetGame().GetPlayerManager().HasPlayerRole(playerID, EPlayerRole.ADMINISTRATOR)
             || GetGame().GetPlayerManager().HasPlayerRole(playerID, EPlayerRole.SESSION_ADMINISTRATOR);
     }

     //------------------------------------------------------------------------------------------------
  static bool IsAdmin()
     {
         int playerID = GetGame().GetPlayerController().GetPlayerId();
         return GetGame().GetPlayerManager().HasPlayerRole(playerID, EPlayerRole.ADMINISTRATOR)
             || GetGame().GetPlayerManager().HasPlayerRole(playerID, EPlayerRole.SESSION_ADMINISTRATOR);
     }

     //------------------------------------------------------------------------------------------------
  static ResourceName GetPrefabAttributeResource(notnull IEntity entity, string containerType, string attributeName)
     {
         EntityPrefabData prefabData = entity.GetPrefabData();
         if (!prefabData)
             return ResourceName.Empty;

         BaseContainer prefabSource = prefabData.GetPrefab();
         if (!prefabSource)
             return ResourceName.Empty;

         IEntitySource entitySource = prefabSource.ToEntitySource();
         if (!entitySource)
             return ResourceName.Empty;

         BaseContainer container = SCR_BaseContainerTools.FindComponentSource(entitySource, containerType);
         if (!container)
             return ResourceName.Empty;

         ResourceName resourceName;
         container.Get(attributeName, resourceName);

         return resourceName;
     }
 };