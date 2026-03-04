 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CharacterAnimationComponentClass: BaseAnimPhysComponentClass
 {
 }

 class CharacterAnimationComponent: BaseAnimPhysComponent
 {
     proto external void SetAnimationLayerTPP();
     proto external void SetAnimationLayerFPP();
     proto external bool SetIKTarget(string bindingName, string boneName, vector position, vector rotationRad);
     // Set an IK target plane to a plane given by specified normal vector and a dist distance from the world origin.
     proto external bool SetIKTargetPlane(string bindingName, float normalX, float normalY, float normalZ, float dist);
     proto external bool GetCollisionMinMax(ECharacterStance whichStance, out vector outMin, out vector outMax);
     proto external void GetMovementState(out CharacterMovementState movementState);
     proto external CharacterHeadingAnimComponent GetHeadingComponent();
     proto external CharacterBuoyancyComponent GetKinematicBuoyancyComponent();
     proto external bool IsWeaponADSTag();
     proto external bool IsPrimaryTag(AnimationTagID tagID);
     proto external bool IsSecondaryTag(AnimationTagID tagID);
     proto external float GetMaxSpeed(float inputForward, float inputRight, int moveType);
     proto external float GetTopSpeed(int moveType = -1, bool ignoreStance = false);
     proto external vector GetInertiaSpeed();
     proto external CharacterCommandHandlerComponent GetCommandHandler();
     proto external bool IsRagdollActive();
     proto external void AddRagdollEffectorDamage(vector posLS, vector dirLS, float force, float radius, float maxLifeTime);
 }
