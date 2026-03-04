 class SCR_Math
 {
     //~ Random generator used in scripted Math functions
     protected const static ref RandomGenerator RANDOM_GENERATOR = new RandomGenerator();

     //------------------------------------------------------------------------------------------------
  // TODO: [Obsolete("Use Math.Mod instead")]
     static float fmod(float dividend, float divisor)
     {
         if (divisor == 0)
             return 0;

         return dividend - Math.Floor(dividend / divisor) * divisor;
     }

     //------------------------------------------------------------------------------------------------
  static float LerpAngle(float a, float b, float t)
     {
         float dt = fmod(b - a, 360);
         if (dt > 180)
             dt -= 360;

         return fmod(Math.Lerp(a, a + dt, t), 360);
     }

     //------------------------------------------------------------------------------------------------
  static float DeltaAngle(float a, float b)
     {
         return 180 - Math.AbsFloat(fmod(Math.AbsFloat(b - a), 360) - 180);
     }

     //------------------------------------------------------------------------------------------------
  static int IntegerMask(int x)
     {
         x |= (x >> 1);
         x |= (x >> 2);
         x |= (x >> 4);
         x |= (x >> 8);
         x |= (x >> 16);
         return x;
     }

     //------------------------------------------------------------------------------------------------
  static float GetDistanceToStop(float speed, float deceleration)
     {
         float distance = 0;
         if (deceleration > 0)
             distance = 0.5 * speed * speed / deceleration;
         else if (speed > 0)
             distance = 1e10;

         return distance;
     }

     //------------------------------------------------------------------------------------------------
  static float GetSpeedToReachDistance(float distance, float deceleration)
     {
         float speed = 0;
         if (distance > 0 && deceleration > 0)
             speed = Math.Sqrt(2 * distance * deceleration);

         return speed;
     }

     //------------------------------------------------------------------------------------------------
  static float GetSpeedToReachDistanceInTime(float distance, float deceleration, float time)
     {
         if (time <= 0)
             return 1e10;

         // Get the decelerated part
         float speed = GetSpeedToReachDistance(distance, deceleration);
         float averageSpeed = distance / time;
         float additionalSpeed = averageSpeed - speed*0.5;
         speed += additionalSpeed;

         return speed;
     }

     //------------------------------------------------------------------------------------------------
  static RandomGenerator GetMathRandomGenerator()
     {
         return RANDOM_GENERATOR;
     }
 };