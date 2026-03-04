 //------------------------------------------------------------------------------------------------
 class SCR_FragmentEntityClass: BaseBuildingClass
 {
 };

 //------------------------------------------------------------------------------------------------
 class SCR_FragmentEntity : BaseBuilding
 {
     #ifdef ENABLE_DESTRUCTION
         protected SCR_DestructionFractalComponent m_DestructibleParent = null;
         protected int m_iIndex = -1;
         protected float m_fHealth = 10;

         //------------------------------------------------------------------------------------------------
  bool GetIsAnchor()
         {
             return m_DestructibleParent.GetCurrentFractalVariant().GetFragmentIndexIsAnchor(m_iIndex);
         }

         //------------------------------------------------------------------------------------------------
  int GetIndex()
         {
             return m_iIndex;
         }

         //------------------------------------------------------------------------------------------------
  float GetHealth()
         {
             return m_fHealth;
         }

         //------------------------------------------------------------------------------------------------
  bool GetDestroyed()
         {
             return GetHealth() <= 0;
         }

         //------------------------------------------------------------------------------------------------
  SCR_DestructionFractalComponent GetDestructibleParent()
         {
             return m_DestructibleParent;
         }

         //------------------------------------------------------------------------------------------------
  void Initialize(SCR_DestructionFractalComponent destructibleParent, int index, float health, ResourceName assetPath)
         {
             m_DestructibleParent = destructibleParent;
             m_iIndex = index;
             m_fHealth = health;

             // Load the model, set physics and add this as child to parent
             Resource resource = Resource.Load(assetPath);
             VObject asset = resource.GetResource().ToVObject();
             SetObject(asset, "");
             m_DestructibleParent.GetOwner().AddChild(this, -1);
             Update();
             Physics phys = Physics.CreateStatic(this, -1);
             if (phys)
                 SCR_PhysicsHelper.RemapInteractionLayer(phys, EPhysicsLayerDefs.Dynamic, EPhysicsLayerDefs.Static);
         }

         //------------------------------------------------------------------------------------------------
  override void EOnContact(IEntity owner, IEntity other, Contact contact)
         {
             if (GetDestroyed())
                 return;

             if (!m_DestructibleParent)
                 return;

             if (RplSession.Mode() == RplMode.Client) // Ignore impacts on clients
                 return;

             if (other && other.IsInherited(SCR_DebrisSmallEntity)) // Ignore impacts from debris
                 return;

             //if (other && other.IsInherited(ChimeraCharacter)) // Ignore character impacts
             //  return;

             // Get the physics of the dynamic object (if owner is static, then we use the other object instead)
             Physics physics = owner.GetPhysics();
             if ((!physics || !physics.IsDynamic()) && other && other.GetPhysics())
                 physics = other.GetPhysics();

             // In case neither had physics (this can only happen if contact is not called from physics step)
             if (!physics)
                 return;

             // Now get the relative force, which is the impulse divided by the mass of the dynamic object
             float relativeForce = 0;
             if (physics.IsDynamic())
                 relativeForce = contact.Impulse / physics.GetMass();

             if (relativeForce < m_DestructibleParent.m_fRelativeContactForceThresholdMinimum) // Below minimum threshold, ignore
                 return;

             float damage = relativeForce * m_DestructibleParent.m_fForceToDamageScale;

             IEntity otherParent = null;
             if (other)
                 otherParent = other.GetParent();

             vector outMat[3];
             vector relativeVelocityBefore = contact.VelocityBefore2 - contact.VelocityBefore1;

             outMat[0] = contact.Position; // Hit position
             outMat[1] = relativeVelocityBefore.Normalized(); // Hit direction
             outMat[2] = contact.Normal; // Hit normal

             // Send damage to damage handling
             OnDamage(damage, EDamageType.KINETIC, other, outMat, other, otherParent, string.Empty, contact.GetRelativeNormalVelocityAfter() - contact.GetRelativeNormalVelocityBefore());

             // Not destroyed yet, ignore
             if (!GetDestroyed())
                 return;

             physics = owner.GetPhysics();
             if (!physics)
                 return;

             // Set collision layer to none to avoid further contacts
             physics.SetInteractionLayer(EPhysicsLayerDefs.VehicleCast);

             // Static physics, so add previous velocity to impactor as it otherwise "bounces"
             if (!physics.IsDynamic())
             {
                 auto otherPhysics = other.GetPhysics();
                 if (otherPhysics && otherPhysics.IsDynamic())
                 {
                     vector relativeVelocityAfter = contact.VelocityAfter2 - contact.VelocityAfter1;
                     otherPhysics.SetVelocity((relativeVelocityAfter - relativeVelocityBefore) * -1 + otherPhysics.GetVelocity());
                 }
             }
         }

         //------------------------------------------------------------------------------------------------
  override void EOnFrame(IEntity owner, float timeSlice)
         {
             delete this;
         }

         //------------------------------------------------------------------------------------------------
  override void OnDamage(float damage, EDamageType type, IEntity pHitEntity, inout vector outMat[3], IEntity damageSource, notnull Instigator instigator, int colliderID, float speed)
         {
             if (!m_DestructibleParent)
                 return;

             if (damage < m_DestructibleParent.m_fDamageThresholdMinimum) // Below minimum threshold, ignore
                 return;

             if (RplSession.Mode() == RplMode.Client)
                 return;

             // Maximum damage threshold, destroy entirely
             if (damage >= m_DestructibleParent.m_fDamageThresholdMaximum)
             {
                 m_DestructibleParent.OnDamage(damage, type, pHitEntity, outMat, damageSource, damageSourceParent, colliderID, speed);
                 return;
             }

             // Reduce health
             m_fHealth -= damage;
             if (m_fHealth < 0)
                 m_fHealth = 0;

             if (m_fHealth > 0) // Health above 0, ignore
                 return;

             // Reached 0 health, destroy
             m_DestructibleParent.OnFragmentDestroyed(this, type, damage, outMat[0], outMat[1]);
             QueueDestroy(type, damage, outMat[0], outMat[1]);
         }

         //------------------------------------------------------------------------------------------------
         void QueueDestroy(EDamageType damageType, float damage, vector hitPosition, vector hitDirection, bool doParticleFX = true)
         {
             if (doParticleFX)
                 SCR_DestructionCommon.PlayParticleEffect_FractionDestruction(this, m_DestructibleParent.m_ParticleDestroyFragment, damageType, hitPosition, hitDirection);

             SCR_FragmentDebris debrisSettings = m_DestructibleParent.m_DebrisDestroyFragment;
             if (debrisSettings)
                 debrisSettings.Spawn(this, m_DestructibleParent.GetOwner().GetPhysics(), damage, hitDirection);

             m_DestructibleParent.GetOwner().RemoveChild(this);
             SetEventMask(EntityEvent.FRAME); // We will delete on the next frame
         }

         //------------------------------------------------------------------------------------------------
         void SCR_FragmentEntity(IEntitySource src, IEntity parent)
         {
             SetEventMask(EntityEvent.CONTACT);
         }
     #endif
 };