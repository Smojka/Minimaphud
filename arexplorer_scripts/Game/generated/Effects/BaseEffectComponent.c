 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseEffectComponentClass: GameComponentClass
 {
 }

 class BaseEffectComponent: GameComponent
 {
     proto external IEntity GetParticleEntity();
     proto external SignalsManagerComponent GetSignalsManager();
     proto external bool HasActiveParticles();
 }
