
 class SCR_BaseCameraShakeProgress
 {
     bool IsRunning();

     sealed bool IsFinished()
     {
         return !IsRunning();
     }

     void Start();

     void Update(IEntity owner, float timeSlice);

     void Apply(inout vector transformMatrix[4], inout float fieldOfView);

     void Clear();
 };