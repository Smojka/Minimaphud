 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ParametricMaterialInstanceComponent: GameComponent
 {
     proto external void SetColor(int argbColor);
     proto external int GetColor();
     /*
     Use it to set emissive color.
     \param argbColor The new emissive color.
     */
     proto external void SetEmissiveColor(int argbColor);
     proto external int GetEmissiveC();
     /*
     Use it to set emissive multiplier.
     \param multiplier The new multiplier in <0, 1000>
     */
     proto external void SetEmissiveMultiplier(int multiplier);
     proto external int GetEmissiveM();
     /*
     Use it to set user parameter modifier specified in Material (Dirt, Mud etc..).
     \param param The new parameter value in <0, 255>
     */
     proto external void SetUserParam1(int param);
     proto external int GetUserParam1();
     /*
     Use it to set user parameter modifier specified in Material (Dirt, Mud etc..).
     \param param The new parameter value in <0, 255>
     */
     proto external void SetUserParam2(int param);
     proto external int GetUserParam2();
     /*
     Use it to set user parameter modifier specified in Material (Dirt, Mud etc..).
     \param param The new parameter value in <0, 255>
     */
     proto external void SetUserParam3(int param);
     proto external int GetUserParam3();
     /*
     Use it to set user parameter modifier specified in Material (Dirt, Mud etc..).
     \param param The new parameter value in <0, 255>
     */
     proto external void SetUserParam4(int param);
     proto external int GetUserParam4();
     /*
     Use it to set user alpha test parameter.
     \param alphaTest The new alpha test value in <0, 255>
     */
     proto external void SetUserAlphaTestParam(int alphaTest);
     proto external int GetUserAlphaTestParam();
     /*
     Sets if custom wetness is enabled. Wetness value itself can be then set using SetUserParam3.
     \param val Use true to enable custom wetness, false otherwise.
     */
     proto external void SetCustomWetnessEnabled(bool val);
     proto external bool GetCustomWetnessEnabled();
     /*
     Sets if rain sliding drops are enabled.
     \param val Use true to enable rain sliding drops, false otherwise.
     */
     proto external void SetSlidingDropsEnabled(bool val);
     proto external bool GetSlidingDropsEnabled();
     proto external bool SaveState(ScriptBitWriter writer);
     proto external bool LoadState(ScriptBitReader sreader);
 }
