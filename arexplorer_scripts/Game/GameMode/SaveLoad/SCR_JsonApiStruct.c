
 [BaseContainerProps()]
 class SCR_JsonApiStruct: JsonApiStruct
 {
     bool Serialize()
     {
         return false;
     }
     bool Deserialize()
     {
         return false;
     }
     void ClearCache()
     {
     }
     void Log()
     {
     }
 };