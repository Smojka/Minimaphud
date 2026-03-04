 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class Faction: ScriptAndConfig
 {
     proto external FactionIdentity GetFactionIdentity();
     proto external bool IsFactionFriendly(Faction otherFaction);
     proto external bool IsFactionEnemy(Faction otherFaction);
     proto external string GetFactionName();
     proto external FactionKey GetFactionKey();
     proto ref Color GetFactionColor();
     proto external UIInfo GetUIInfo();
 }
