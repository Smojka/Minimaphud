 enum ECharacterDamageState: EDamageState
 {
     WOUNDED = 3
 };

 enum ECharacterHealthState: EDamageState
 {
     MODERATE = 3,
     SERIOUS = 4,
     CRITICAL = 5
 };

 enum ECharacterBloodState: EDamageState
 {
     STRONG = 3,
     FAINTING = 4,
     UNCONSCIOUS = 5
 };

 enum ECharacterResilienceState : EDamageState
 {
     WEAKENED = 3,
     FAINTING = 4,
     UNCONSCIOUS = 5
 };

 enum EBandagingAnimationBodyParts
 {
     Invalid = 0,
     UpperHead = 1,
     LowerHead = 2,
     UpperTorso = 3,
     LowerTorso = 4,
     LeftHand = 5,
     RightHand = 6,
     LeftLeg = 7,
     RightLeg = 8
 };

 class SCR_BleedingHitZoneParameters : Managed
 {
     void SCR_BleedingHitZoneParameters(
         HitZone hitZone,
         float DamageOverTime
     )
     {
         m_hHitZone = hitZone;
         m_fDamageOverTime = DamageOverTime;
     }
     HitZone m_hHitZone;
     float m_fDamageOverTime;
 };

 //------------------------------------------------------------------------------------------------
 class SCR_CharacterHitZone : SCR_RegeneratingHitZone
 {
     protected static const float DAMAGE_TO_BLOOD_MULTIPLIER = 1.3; //Const value is based on previous testing by caliber

     [Attribute("0.25", UIWidgets.Auto, "Multiplier of hitzone health that will be applied as bleeding damage over time when damaged")]
     protected float m_fBleedingRateScale;

     [Attribute()]
     protected ref array<string> m_aDamageSubmeshes;

     [Attribute("", UIWidgets.Object)]
     protected ref array<ref LoadoutAreaType> m_aBleedingAreas;

     [Attribute(ECharacterHitZoneGroup.VIRTUAL.ToString(), UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ECharacterHitZoneGroup))]
     protected ECharacterHitZoneGroup m_eHitZoneGroup;

     [Attribute(EBandagingAnimationBodyParts.Invalid.ToString(), UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EBandagingAnimationBodyParts))]
     protected EBandagingAnimationBodyParts m_eBandageAnimationBodyPart;

     protected bool m_bIsWounded;

     //-----------------------------------------------------------------------------------------------------------
  override void OnDamage(notnull BaseDamageContext damageContext)
     {
         super.OnDamage(damageContext);

         if (this != damageContext.struckHitZone)
             return;

         // Request impact sound from damage manager
         bool critical = damageContext.damageType == EDamageType.FIRE || damageContext.damageValue >= GetCriticalDamageThreshold() * GetMaxHealth();

         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (manager)
             manager.SoundHit(critical, damageContext.damageType);

         // FireDamage shouldn't start bleedings
         if (damageContext.damageType == EDamageType.FIRE)
             return;

         // Only serious hits should cause bleeding
         if (!critical)
             return;

         // Adding immediately some blood to the clothes - currently it's based on the damage dealt.
         AddBloodToClothes(Math.Clamp(damageContext.damageValue * DAMAGE_TO_BLOOD_MULTIPLIER, 0, 255));

         if (IsProxy())
             return;

         if (Math.RandomFloat(0,1) < 0.3)
             return;

         AddBleeding(damageContext.colliderID);
     }

     //-----------------------------------------------------------------------------------------------------------
  override float ComputeEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT)
     {
         float rawDamage = damageContext.damageValue;
         if (damageContext.damageValue > 0 && !isDOT)
         {
             float protectionValue = GetArmorProtectionValue(damageContext.damageType);
             rawDamage = Math.Max(damageContext.damageValue - protectionValue, 0);
         }

         BaseDamageContext hack = BaseDamageContext.Cast(damageContext.Clone());
         hack.damageValue = rawDamage;

         return super.ComputeEffectiveDamage(hack, isDOT);
     }

     //-----------------------------------------------------------------------------------------------------------
  float GetArmorProtectionValue(EDamageType damageType)
     {
         SCR_CharacterDamageManagerComponent damageMgr = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (!damageMgr)
             return 0;

         return damageMgr.GetArmorProtection(this, damageType);
     }

     //-----------------------------------------------------------------------------------------------------------
  bool IsCovered()
     {
         CharacterIdentityComponent identity = CharacterIdentityComponent.Cast(GetOwner().FindComponent(CharacterIdentityComponent));
         if (!identity)
             return false;

         foreach (string submesh: m_aDamageSubmeshes)
         {
             if (identity.IsCovered(submesh))
                 return true;
         }

         return false;
     }

     //-----------------------------------------------------------------------------------------------------------
  void AddBleeding(int colliderID = -1)
     {
         if (!HasColliderNodes())
             return;

         int colliderDescriptorIndex = GetColliderDescriptorIndex(colliderID);
         if (colliderDescriptorIndex < 0)
         {
             int currentHealth = GetHealth();
             colliderDescriptorIndex = currentHealth % GetNumColliderDescriptors();
         }

         AddBleedingToCollider(colliderDescriptorIndex);
     }

     //-----------------------------------------------------------------------------------------------------------
  void AddBleedingToCollider(int colliderDescriptorIndex = -1)
     {
         if (!HasColliderNodes())
             return;

         // Currently we only have one bleeding per hitzone
         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (manager)
             manager.AddBleedingHitZone(this, colliderDescriptorIndex);
     }

     //-----------------------------------------------------------------------------------------------------------
     override void OnDamageStateChanged()
     {
         super.OnDamageStateChanged();

         UpdateSubmeshes();
         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (manager)
             manager.UpdateCharacterGroupDamage(GetHitZoneGroup());
     }

     //-----------------------------------------------------------------------------------------------------------
     void AddBloodToClothes(float immediateBloodEffect = 0)
     {
         EquipedLoadoutStorageComponent loadoutStorage = EquipedLoadoutStorageComponent.Cast(GetOwner().FindComponent(EquipedLoadoutStorageComponent));
         if (!loadoutStorage)
             return;

         IEntity clothEntity;
         ParametricMaterialInstanceComponent materialComponent;
         for (int i = m_aBleedingAreas.Count() - 1; i >= 0; i--)
         {
             clothEntity = loadoutStorage.GetClothFromArea(m_aBleedingAreas[i].Type());
             if (!clothEntity)
                 continue;

             materialComponent = ParametricMaterialInstanceComponent.Cast(clothEntity.FindComponent(ParametricMaterialInstanceComponent));
             if (!materialComponent)
                 continue;

             if (immediateBloodEffect > 0)
             {
                 materialComponent.SetUserParam2(Math.Clamp(materialComponent.GetUserParam2() + immediateBloodEffect, 1, 255));
                 continue;
             }

             materialComponent.SetUserParam2(Math.Clamp(materialComponent.GetUserParam2() + GetMaxBleedingRate() * 0.1, 1, 255));
         }
     }

     //-----------------------------------------------------------------------------------------------------------
     void SetWoundedSubmesh(bool wounded)
     {
         m_bIsWounded = wounded;

         CharacterIdentityComponent identity = CharacterIdentityComponent.Cast(GetOwner().FindComponent(CharacterIdentityComponent));
         if (!identity)
             return;

         foreach (string submesh: m_aDamageSubmeshes)
             identity.SetWoundState(submesh, wounded);
     }

     //-----------------------------------------------------------------------------------------------------------
  void UpdateSubmeshes()
     {
         bool isWounded = GetDamageStateThreshold(GetDamageState()) <= GetDamageStateThreshold(ECharacterDamageState.WOUNDED);
         if (m_bIsWounded != isWounded)
             SetWoundedSubmesh(isWounded);
     }

     //-----------------------------------------------------------------------------------------------------------
  float GetMaxBleedingRate()
     {
         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (!manager)
             return 1;

         float bleedingRate = m_fBleedingRateScale * GetMaxHealth();

         bleedingRate *= manager.GetDOTScale();

         return bleedingRate;
     }

     //-----------------------------------------------------------------------------------------------------------
     override EHitZoneGroup GetHitZoneGroup()
     {
         return m_eHitZoneGroup;
     }

     //-----------------------------------------------------------------------------------------------------------
     EBandagingAnimationBodyParts GetBodyPartToHeal()
     {
         return m_eBandageAnimationBodyPart;
     }
 };

 //------------------------------------------------------------------------------------------------
 class SCR_RegeneratingHitZone : SCR_HitZone
 {
     [Attribute("10", UIWidgets.Auto, "Time without receiving damage or bleeding to start regeneration\n[s]")]
     protected float m_fRegenerationDelay;

     [Attribute("200", UIWidgets.Auto, "Time to regenerate this hitzone fully\n[s]")]
     protected float m_fFullRegenerationTime;

     //-----------------------------------------------------------------------------------------------------------
  override void OnDamage(notnull BaseDamageContext damageContext)
     {
         super.OnDamage(damageContext);

         if (IsProxy())
             return;

         RemovePassiveRegeneration();
         ScheduleRegeneration();
     }

     //-----------------------------------------------------------------------------------------------------------
     override void OnHealthSet()
     {
         super.OnHealthSet();

         if (IsProxy())
             return;

         RemovePassiveRegeneration();
         ScheduleRegeneration();
     }

     //-----------------------------------------------------------------------------------------------------------
     // Calculate rate of passive regeneration
     float CalculatePassiveRegeneration()
     {
         if (m_fFullRegenerationTime <= 0)
             return 0;

         // Any local damage over time shall disrupt passive regeneration
         array<EDamageType> damageTypes = {};
         SCR_Enum.GetEnumValues(EDamageType, damageTypes);
         foreach (EDamageType damageType : damageTypes)
         {
             if (damageType == EDamageType.REGENERATION || damageType == EDamageType.HEALING)
                 continue;

             if (GetDamageOverTime(damageType) != 0)
                 return 0;
         }

         return -GetMaxHealth() / m_fFullRegenerationTime;
     }

     //-----------------------------------------------------------------------------------------------------------
     // Schedule healing over time, delay current scheduled regeneration
     void ScheduleRegeneration()
     {
         GetGame().GetCallqueue().Remove(UpdatePassiveRegeneration);
         GetGame().GetCallqueue().CallLater(UpdatePassiveRegeneration, 1000 * m_fRegenerationDelay, true);
     }

     //-----------------------------------------------------------------------------------------------------------
     // Remove current healing over time and any scheduled regeneration
     void RemovePassiveRegeneration()
     {
         GetGame().GetCallqueue().Remove(UpdatePassiveRegeneration);

         if (GetDamageOverTime(EDamageType.REGENERATION))
             SetDamageOverTime(EDamageType.REGENERATION, 0)
         }

     //-----------------------------------------------------------------------------------------------------------
     protected void UpdatePassiveRegeneration()
     {
         float regenerationRate = CalculatePassiveRegeneration();
         SetDamageOverTime(EDamageType.REGENERATION, regenerationRate);
         }

     //-----------------------------------------------------------------------------------------------------------
     override void OnDamageStateChanged()
     {
         super.OnDamageStateChanged();

         if (GetDamageState() == ECharacterDamageState.UNDAMAGED)
             RemovePassiveRegeneration();
     }
 };

 //-----------------------------------------------------------------------------------------------------------
 class SCR_CharacterHealthHitZone : SCR_HitZone
 {
     //-----------------------------------------------------------------------------------------------------------
     override float ComputeEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT)
     {
         // Handle falldamage. Falldamage is applied to defaultHZ, so it's propegated down to physical hitzones manually, then back up to the health HZ like normal damage.
         if (damageContext.damageType == EDamageType.COLLISION && damageContext.struckHitZone == this)
         {
             SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
             if (damageManager && damageContext.damageValue > GetDamageThreshold())
                 damageManager.HandleFallDamage(damageContext.damageValue);

             return 0;
         }

         return super.ComputeEffectiveDamage(damageContext, isDOT);
     }
 }

 //-----------------------------------------------------------------------------------------------------------
 class SCR_CharacterResilienceHitZone : SCR_RegeneratingHitZone
 {
     //-----------------------------------------------------------------------------------------------------------
     override void OnDamageStateChanged()
     {
         super.OnDamageStateChanged();

         UpdateConsciousness();
     }

     //-----------------------------------------------------------------------------------------------------------
     void UpdateConsciousness()
     {
         SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (damageManager)
             damageManager.UpdateConsciousness();
     }

     //-----------------------------------------------------------------------------------------------------------
     override float CalculatePassiveRegeneration()
     {
         float regeneration = super.CalculatePassiveRegeneration();

         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (!manager)
             return regeneration;

         regeneration *= manager.GetResilienceRegenScale();

         return regeneration;
     }

     //-----------------------------------------------------------------------------------------------------------
     override void OnInit(IEntity pOwnerEntity, GenericComponent pManagerComponent)
     {
         super.OnInit(pOwnerEntity, pManagerComponent);
         SCR_CharacterDamageManagerComponent characterDamageManager = SCR_CharacterDamageManagerComponent.Cast(pManagerComponent);
         if (characterDamageManager)
             characterDamageManager.SetResilienceHitZone(this);
     }
 };

 //-----------------------------------------------------------------------------------------------------------
 class SCR_CharacterBloodHitZone : SCR_RegeneratingHitZone
 {
     ref map<SCR_CharacterHitZone, ref SCR_BleedingHitZoneParameters> m_mHitZoneDOTMap;
     ref array<float> m_aHitZoneGroupBleedings;

     //-----------------------------------------------------------------------------------------------------------
     override void OnDamageStateChanged()
     {
         super.OnDamageStateChanged();

         UpdateConsciousness();
     }

     //-----------------------------------------------------------------------------------------------------------
     void UpdateConsciousness()
     {
         SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (damageManager)
             damageManager.UpdateConsciousness();
     }

     //-----------------------------------------------------------------------------------------------------------
     override void OnInit(IEntity pOwnerEntity, GenericComponent pManagerComponent)
     {
         super.OnInit(pOwnerEntity, pManagerComponent);

         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (manager)
             manager.SetBloodHitZone(this);
     }

     //-----------------------------------------------------------------------------------------------------------
     bool AddBleedingHZToMap(SCR_CharacterHitZone hitZone, SCR_BleedingHitZoneParameters localBleedingHZParams)
     {
         if (!m_mHitZoneDOTMap)
             m_mHitZoneDOTMap = new map<SCR_CharacterHitZone, ref SCR_BleedingHitZoneParameters>();

         if (m_mHitZoneDOTMap.Contains(hitZone))
         {
             m_mHitZoneDOTMap.Set(hitZone, localBleedingHZParams);
             return true;
         }

         return m_mHitZoneDOTMap.Insert(hitZone, localBleedingHZParams);
     }

     //-----------------------------------------------------------------------------------------------------------
     void RemoveBleedingHZFromMap(SCR_CharacterHitZone hitZone)
     {
         if (!m_mHitZoneDOTMap)
             return;

         m_mHitZoneDOTMap.Remove(hitZone);

         if (m_mHitZoneDOTMap.IsEmpty())
             m_mHitZoneDOTMap = null;
     }

     //-----------------------------------------------------------------------------------------------------------
     float GetPhysicalHZBleedingDOT(notnull SCR_CharacterHitZone hitZone)
     {
         if (!m_mHitZoneDOTMap || m_mHitZoneDOTMap.IsEmpty())
             return 0;

         SCR_BleedingHitZoneParameters localBleedingHZParams;

         if (m_mHitZoneDOTMap.Find(hitZone, localBleedingHZParams))
             return localBleedingHZParams.m_fDamageOverTime;

         return 0;
     }

     //-----------------------------------------------------------------------------------------------------------
     map<SCR_CharacterHitZone, ref SCR_BleedingHitZoneParameters> GetHitZoneDOTMap()
     {
         return m_mHitZoneDOTMap;
     }
 };

 //-----------------------------------------------------------------------------------------------------------
 class SCR_CharacterHeadHitZone : SCR_CharacterHitZone
 {
     //-----------------------------------------------------------------------------------------------------------
     override void OnInit(IEntity pOwnerEntity, GenericComponent pManagerComponent)
     {
         super.OnInit(pOwnerEntity, pManagerComponent);

         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(pManagerComponent);
         if (manager)
             manager.SetHeadHitZone(this);
     }

     //-----------------------------------------------------------------------------------------------------------
     override void OnDamageStateChanged()
     {
         super.OnDamageStateChanged();
         SoundKnockout();
     }

     //-----------------------------------------------------------------------------------------------------------
  void SoundKnockout()
     {
         if (GetDamageState() != EDamageState.DESTROYED)
             return;

         SCR_CharacterDamageManagerComponent manager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
         if (manager)
             manager.SoundKnockout();
     }
 };