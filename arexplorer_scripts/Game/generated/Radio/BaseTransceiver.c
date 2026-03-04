 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseTransceiver: ScriptAndConfig
 {
     proto external int GetFrequency();
     proto external int GetMinFrequency();
     proto external int GetMaxFrequency();
     proto external void SetFrequency(int freq);
     proto external int GetFrequencyResolution();
     proto external float GetRange();
     proto external void SetRange(float range);
     proto external BaseRadioComponent GetRadio();
     proto external void BeginTransmission(BaseRadioMessage message);
 }
