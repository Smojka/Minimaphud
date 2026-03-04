 #ifdef UNDEFINED_MACRO

 class SCR_ExampleDoxygen
 {
     //------------------------------------------------------------------------------------------------
  int m_iExamplePublicVariable;

     //------------------------------------------------------------------------------------------------
  protected int m_iExampleProtectedVariable;

     //------------------------------------------------------------------------------------------------
  void Example()
     {
         // comments without Doxygen markup like this one are ignored
         Print("Example", LogLevel.NORMAL);
     }

     //------------------------------------------------------------------------------------------------
  void ExampleWithParam(int exampleParam)
     {
         Print("ExampleWithParam: exampleParam1=" + exampleParam, LogLevel.NORMAL);
     }

     //------------------------------------------------------------------------------------------------
  void ExampleWithOutParam(out int outExampleParam)
     {
         outExampleParam = 42;
         Print("ExampleWithOutParam: outExampleParam=" + outExampleParam, LogLevel.NORMAL);
     }

     //------------------------------------------------------------------------------------------------
  void ExampleWithInOutParam(inout int inOutExampleParam)
     {
         int before = inOutExampleParam;
         inOutExampleParam *= 42;
         Print("ExampleWithInOutParam: before=" + before + ", after=" + inOutExampleParam, LogLevel.NORMAL);
     }

     //------------------------------------------------------------------------------------------------
  int ExampleWithReturnValue()
     {
         Print("ExampleWithReturnValue", LogLevel.NORMAL);
         return 42;
     }

     //------------------------------------------------------------------------------------------------
  int ExampleWithEverything(int exampleParam, out int outExampleParam)
     {
         Print("ExampleWithEverything: exampleParam=" + exampleParam + ", outExampleParam=" + outExampleParam, LogLevel.NORMAL);
         outExampleParam = exampleParam;
         return exampleParam + outExampleParam;
     }
 }

 enum SCR_EExampleDoxygen
 {
     EXAMPLE_1,
     EXAMPLE_2,
 }
 #endif // UNDEFINED_MACRO