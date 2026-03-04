 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseAnimPhysComponentClass: GameComponentClass
 {
 }

 class BaseAnimPhysComponent: GameComponent
 {
     proto external void SetCurrentCommand(AnimPhysCommandScripted pCommand);
     proto external AnimPhysCommandScripted GetCommandScripted();
     proto external TAnimGraphCommand    BindCommand(string pCommandName);
     proto external TAnimGraphVariable   BindVariableFloat(string pVariableName);
     proto external TAnimGraphVariable   BindVariableInt(string pVariableName);
     proto external TAnimGraphVariable   BindVariableBool(string pVariableName);
     proto external TAnimGraphTag        BindTag(string pTagName);
     proto external TAnimGraphEvent      BindEvent(string pEventName);
     proto external TAnimGraphPrediction BindPrediction(string pPredictionName);
     proto external void SetVariableFloat(TAnimGraphVariable varIdx, float value);
     proto external void SetVariableInt(TAnimGraphVariable varIdx, int value);
     proto external void SetVariableBool(TAnimGraphVariable varIdx, bool value);
     proto external void CallCommand(TAnimGraphCommand pCmdIndex, int intParam, float floatParam);
     proto external void CallCommand4I(TAnimGraphCommand pCmdIndex, int intParam1, int intParam2, int intParam3, int intParam4, float floatParam);
     proto external bool PhysicsIsFalling();
     proto external void PhysicsEnableGravity(bool pState);
 }
