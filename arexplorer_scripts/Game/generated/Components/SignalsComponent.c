 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class SignalsComponentClass: GameComponentClass
 {
 }

 class SignalsComponent: GameComponent
 {
     proto external void Update();
     proto external int GetSignalInIndex(string name);
     proto external int GetSignalOutIndex(string name);
     proto external bool SetSignalInValue(int index, float value);
     proto external float GetSignalInValue(int index);
     proto external float GetSignalOutValue(int index);
     proto external int GetSignalIns(out notnull array<string> ins);
     proto external int GetSignalOuts(out notnull array<string> outs);
 }
