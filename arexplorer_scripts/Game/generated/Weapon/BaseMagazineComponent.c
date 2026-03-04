 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseMagazineComponentClass: GameComponentClass
 {
 }

 class BaseMagazineComponent: GameComponent
 {
     proto external IEntity GetOwner();
     proto external bool IsUsed();
     proto external void SetAmmoCount(int ammoCount);
     proto external int GetAmmoCount();
     proto external int GetMaxAmmoCount();
     proto external BaseMagazineWell GetMagazineWell();
     proto external UIInfo GetUIInfo();
 }
