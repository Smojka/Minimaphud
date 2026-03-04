 class SCR_Math3D
 {
     //------------------------------------------------------------------------------------------------
  static vector GetFixedAxisVector(vector toFlip)
     {
         return { toFlip[1], toFlip[0], toFlip[2] };
     }

     //------------------------------------------------------------------------------------------------
  static void RotateTowards(out float result[4], float from[4], float to[4], float maxDegreesDelta)
     {
         float num = Math3D.QuatAngle(from, to);
         if (float.AlmostEqual(num, 0.0))
         {
             Math3D.QuatCopy(to, result);
             return;
         }

         float t = Math.Min(1, maxDegreesDelta / num);
         Math3D.QuatLerp(result, from, to, t);
     }

     //------------------------------------------------------------------------------------------------
  static vector MoveTowards(vector start, vector target, float maxDistanceDelta)
     {
         vector diff = target - start;
         float magnitude = diff.Length();
         if (magnitude <= maxDistanceDelta || magnitude == 0)
             return target;

         return start + diff / magnitude * maxDistanceDelta;
     }

     //------------------------------------------------------------------------------------------------
  static vector FixEulerVector180(vector angles)
     {
         for (int i = 0; i < 3; i++)
         {
             if (angles[i] < -180 || angles[i] > 180)
                 angles[i] = Math.Repeat(180 + angles[i], 360) - 180;
         }

         return angles;
     }

     //------------------------------------------------------------------------------------------------
     // IEntity entity is the entity you want to be affected by extrapolation.
     // Physics physics is the physics that the extrapolation should calculate with.
     // vector netPosition is the last received position.
     // vector netVelocity is the last received velocity.
     // float netTeleportDistance is the max distance between position and netPosition, anything over this causes the entity to teleport.
     // float netRotation[4] is the last received rotation.
     // vector netVelocityAngular is the last received angular velocity.
     // float netTeleportAng is the max angle between current rotation and replicated rotation, anything over this causes the entity to teleport.
     // float timeSinceLastTick is the time since last synchronization of extrapolation relevant data was received, it should already be incremented by timeSlice by you!
     // float timeSlice is the time since last frame / simulation step.
     static void Extrapolate(IEntity entity, Physics physics, vector netPosition, vector netVelocityLinear, float netTeleportDistance, float netRotation[4], vector netVelocityAngular, float netTeleportAng, float timeSinceLastTick, float timeSlice, float netTickInterval)
     {
         float scale = entity.GetScale();
         vector currentMatrix[4];
         entity.GetWorldTransform(currentMatrix);

         // Lerp to positions/rotations received
         vector position = currentMatrix[3];
         float rotation[4];
         Math3D.MatrixToQuat(currentMatrix, rotation);

         // Static object, ensure exact rotation/position
         if (!physics || !physics.IsDynamic())
         {
             if (rotation != netRotation)
                 Math3D.QuatToMatrix(netRotation, currentMatrix);

             currentMatrix[3] = netPosition;

             entity.SetWorldTransform(currentMatrix);
             entity.SetScale(scale);
             return;
         }

         // Dynamic object, so calculate projected position/rotation based on last tick
         vector projectedPos = netPosition + netVelocityLinear * timeSinceLastTick;

         vector netVelocityAngularFlipped = SCR_Math3D.GetFixedAxisVector(netVelocityAngular * timeSinceLastTick);
         float projectedRotation[4];
         float netVelocityAngularQuat[4];
         netVelocityAngularFlipped.QuatFromAngles(netVelocityAngularQuat);
         Math3D.QuatMultiply(projectedRotation, netRotation, netVelocityAngularQuat);

         // Calculate the position and rotation error
         float posError = vector.Distance(projectedPos, position);
         float rotError = Math3D.QuatAngle(projectedRotation, rotation);

         // If too far off position, teleport
         if (posError > netTeleportDistance)
         {
             entity.SetOrigin(netPosition);
             posError = 0;
         }

         // If too far off rotation, teleport
         if (rotError > netTeleportAng)
         {
             Math3D.QuatToMatrix(netRotation, currentMatrix);
             currentMatrix[3] = entity.GetOrigin();
             entity.SetWorldTransform(currentMatrix);
             rotError = 0;
         }

         float timeStep = timeSlice / netTickInterval;
         float timeStepTick = Math.Clamp(timeSlice / netTickInterval, 0, 1);

         // Adjust to account for errors in position/rotation
         if (posError > 0.01)
         {
             entity.SetOrigin(SCR_Math3D.MoveTowards(position, projectedPos, posError * timeStep));
             physics.SetVelocity(physics.GetVelocity() + (projectedPos - position) * timeStepTick);
         }

         if (rotError > 0.01)
         {
             float outRot[4];
             Math3D.QuatRotateTowards(outRot, rotation, projectedRotation, (rotError * timeStep) * Math.RAD2DEG);
             Math3D.QuatToMatrix(outRot, currentMatrix);
             currentMatrix[3] = entity.GetOrigin();
             entity.SetWorldTransform(currentMatrix);

             float rotDiff[4];
             float rotInv[4];
             Math3D.QuatInverse(rotInv, rotation);
             Math3D.QuatMultiply(rotDiff, projectedRotation, rotInv);
             vector angularVelocity = Math3D.QuatToAngles(rotDiff);
             angularVelocity = SCR_Math3D.FixEulerVector180(angularVelocity) * Math.DEG2RAD * timeStepTick;
             angularVelocity += physics.GetAngularVelocity();
             physics.SetAngularVelocity(angularVelocity);
         }

         entity.SetScale(scale);
     }

     //------------------------------------------------------------------------------------------------
  static vector IntersectPlane(vector rayPos, vector rayVector, vector planePos, vector planeNormal)
     {
         return rayPos - rayVector * (vector.Dot(rayPos - planePos, planeNormal) / vector.Dot(rayVector, planeNormal));
     }

     //------------------------------------------------------------------------------------------------
  static bool MatrixEqual(vector matrixA[4], vector matrixB[4])
     {
         return matrixA[3] == matrixB[3]
             && matrixA[2] == matrixB[2]
             && matrixA[1] == matrixB[1]
             && matrixA[0] == matrixB[0];
     }

     //------------------------------------------------------------------------------------------------
  static bool IsMatrixEmpty(vector matrix[4])
     {
         return matrix[3] == vector.Zero
             && matrix[2] == vector.Zero
             && matrix[1] == vector.Zero
             && matrix[0] == vector.Zero;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsMatrixIdentity(vector matrix[4])
     {
         return matrix[3] == vector.Zero
             && matrix[2] == vector.Forward
             && matrix[1] == vector.Up
             && matrix[0] == vector.Right;
     }

     //------------------------------------------------------------------------------------------------
  static vector Min(vector vA, vector vB)
     {
         return Vector(
             Math.Min(vA[0], vB[0]),
             Math.Min(vA[1], vB[1]),
             Math.Min(vA[2], vB[2])
         );
     }

     //------------------------------------------------------------------------------------------------
  static vector Max(vector vA, vector vB)
     {
         return Vector(
             Math.Max(vA[0], vB[0]),
             Math.Max(vA[1], vB[1]),
             Math.Max(vA[2], vB[2])
         );
     }

     //------------------------------------------------------------------------------------------------
  static float GetDistanceFromSpline(notnull array<vector> points, vector point)
     {
         int count = points.Count();
         if (count < 1)
             return -1;

         if (count == 1)
             return vector.Distance(point, points[0]);

         float tempDistanceSq;
         vector segmentStart = points[0];
         float minDistanceSq = vector.DistanceSq(point, segmentStart);

         foreach (int i, vector segmentEnd : points)
         {
             if (i == 0)
                 continue;

             tempDistanceSq = Math3D.PointLineSegmentDistanceSqr(point, segmentStart, segmentEnd);
             if (tempDistanceSq < minDistanceSq)
                 minDistanceSq = tempDistanceSq;

             segmentStart = segmentEnd;
         }

         return Math.Sqrt(minDistanceSq);
     }

     //------------------------------------------------------------------------------------------------
  static float GetDistanceFromSplineXZ(notnull array<vector> points, vector point)
     {
         int count = points.Count();
         if (count < 1)
             return -1;

         if (count == 1)
             return vector.DistanceXZ(point, points[0]);

         float tempDistanceSq;
         vector segmentStart = points[0];
         float minDistanceSq = vector.DistanceSqXZ(point, segmentStart);
         segmentStart[1] = 0;    // 2D conversion
         point[1] = 0;           // 2D conversion

         foreach (int i, vector segmentEnd : points)
         {
             if (i == 0)
                 continue;

             segmentEnd[1] = 0;

             tempDistanceSq = Math3D.PointLineSegmentDistanceSqr(point, segmentStart, segmentEnd);
             if (tempDistanceSq < minDistanceSq)
                 minDistanceSq = tempDistanceSq;

             segmentStart = segmentEnd;
         }

         return Math.Sqrt(minDistanceSq);
     }

     //------------------------------------------------------------------------------------------------
  static bool IsPointWithinSplineDistance(notnull array<vector> points, vector point, float distance)
     {
         int count = points.Count();
         if (count < 1)
             return -1;

         if (count == 1)
             return vector.Distance(point, points[0]) <= distance;

         distance *= distance; // variable reuse
         vector segmentStart = points[0];

         foreach (int i, vector segmentEnd : points)
         {
             if (i == 0)
                 continue;

             if (Math3D.PointLineSegmentDistanceSqr(point, segmentStart, segmentEnd) < distance)
                 return true;

             segmentStart = segmentEnd;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsPointWithinSplineDistanceXZ(notnull array<vector> points, vector point, float distance)
     {
         int count = points.Count();
         if (count < 1)
             return -1;

         if (count == 1)
             return vector.DistanceXZ(point, points[0]) <= distance;

         distance *= distance; // variable reuse
         vector segmentStart = points[0];
         segmentStart[1] = 0;    // 2D conversion
         point[1] = 0;           // 2D conversion

         foreach (int i, vector segmentEnd : points)
         {
             if (i == 0)
                 continue;

             segmentEnd[1] = 0;  // 2D conversion

             if (Math3D.PointLineSegmentDistanceSqr(point, segmentStart, segmentEnd) < distance)
                 return true;

             segmentStart = segmentEnd;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static void QuatAngleAxis(float angle, vector axis, out float quat[4])
     {
         angle = angle * 0.5;
         float sin = Math.Sin(angle);

         axis.Normalize();
         quat[0] = axis[0] * sin;
         quat[1] = axis[1] * sin;
         quat[2] = axis[2] * sin;
         quat[3] = Math.Cos(angle);
     }

     //------------------------------------------------------------------------------------------------
  static vector QuatMultiply(float quat[4], vector vec)
     {
         vector xyz = Vector(quat[0], quat[1], quat[2]);
         vector t = 2.0 * (xyz * vec);
         return vec + quat[3] * t + (xyz * t);
     }

     //------------------------------------------------------------------------------------------------
  static void RotateAround(vector transform[4], vector pivot, vector axis, float angle, out vector result[4])
     {
         float q[4];
         QuatAngleAxis(angle, axis, q);
         result[3] = QuatMultiply(q, (transform[3] - pivot)) + pivot;

         float qt[4];
         Math3D.MatrixToQuat(transform, qt);
         Math3D.QuatMultiply(qt, q, qt);
         Math3D.QuatToMatrix(qt, result);
     }

     //------------------------------------------------------------------------------------------------
  static void LookAt(vector source, vector destination, vector up, out vector rotMat[4])
     {
         vector lookDir = destination - source;
         Math3D.DirectionAndUpMatrix(lookDir.Normalized(), up.Normalized(), rotMat);
     }

     //------------------------------------------------------------------------------------------------
  static vector ClampMagnitude(vector v, float magnitude)
     {
         if (v.LengthSq() > magnitude * magnitude)
             return v.Normalized() * magnitude;

         return v;
     }
 }