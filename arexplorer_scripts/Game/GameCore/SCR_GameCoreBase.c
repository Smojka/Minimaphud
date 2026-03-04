
 [BaseContainerProps()]
 class SCR_GameCoreBase
 {
     [Attribute(defvalue: "1", desc: "When disabled, the core will not be initialized upon start of the game.")]
     private bool m_bEnabled;

     [Attribute(desc: "Core processing priority. Higher value = higher priority")]
     private int m_iPriority;

     //------------------------------------------------------------------------------------------------
  void OnAfterInit();

     //------------------------------------------------------------------------------------------------
  void OnGameStart();

     //------------------------------------------------------------------------------------------------
  void OnWorldPostProcess(World world);

     //------------------------------------------------------------------------------------------------
  void OnUpdate(float timeSlice);

     //------------------------------------------------------------------------------------------------
  void OnGameEnd();

     //------------------------------------------------------------------------------------------------
  int GetPriority()
     {
         return m_iPriority;
     }

     //------------------------------------------------------------------------------------------------
  bool IsEnabled()
     {
         return m_bEnabled;
     }

     //------------------------------------------------------------------------------------------------
  bool CanCreate()
     {
         return true;
     }

     //---
  static SCR_GameCoreBase GetInstance(typename type)
     {
         return SCR_GameCoresManager.GetCore(type);
     }
 }