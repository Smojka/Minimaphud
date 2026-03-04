 class SCR_BaseEditorAttributeVar
 {
     static const int SNAPSHOT_SIZE_VALUE = 12; //--- Vector

     protected vector m_vValue; //--- All variables are saved as a vector in the end (ToDo: Find lighter solution?)

     //--- Integer
  void SetInt(int value)
     {
         m_vValue[0] = value;
     }
     int GetInt()
     {
         return m_vValue[0];
     }

     //--- Float
  void SetFloat(float value)
     {
         m_vValue[0] = value;
     }
     float GetFloat()
     {
         return m_vValue[0];
     }

     //--- Bool
  void SetBool(bool value)
     {
         m_vValue[0] = value;
     }
     bool GetBool()
     {
         return m_vValue[0] != 0;
     }

     //--- Vector
  void SetVector(vector value)
     {
         m_vValue = value;
     }
     vector GetVector()
     {
         return m_vValue;
     }

     //--- Network functions
     static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet)
     {
         snapshot.Serialize(packet, SNAPSHOT_SIZE_VALUE);
     }
     static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot)
     {
         return snapshot.Serialize(packet, SNAPSHOT_SIZE_VALUE);
     }
     static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint)
     {
         return lhs.CompareSnapshots(rhs, SNAPSHOT_SIZE_VALUE);
     }
     static bool PropCompare(SCR_BaseEditorAttributeVar prop, SSnapSerializerBase snapshot, ScriptCtx hint)
     {
         return snapshot.Compare(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
     }
     static bool Extract(SCR_BaseEditorAttributeVar prop, ScriptCtx hint, SSnapSerializerBase snapshot)
     {
         snapshot.SerializeBytes(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
         return true;
     }
     static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, SCR_BaseEditorAttributeVar prop)
     {
         return Extract(prop, hint, snapshot);
     }

     //--- Default functions
     static SCR_BaseEditorAttributeVar CreateInt(int value)
     {
         SCR_BaseEditorAttributeVar var = new SCR_BaseEditorAttributeVar();
         var.SetInt(value);
         return var;
     }
     static SCR_BaseEditorAttributeVar CreateFloat(float value)
     {
         SCR_BaseEditorAttributeVar var = new SCR_BaseEditorAttributeVar();
         var.SetFloat(value);
         return var;
     }
     static SCR_BaseEditorAttributeVar CreateBool(bool value)
     {
         SCR_BaseEditorAttributeVar var = new SCR_BaseEditorAttributeVar();
         var.SetBool(value);
         return var;
     }
     static SCR_BaseEditorAttributeVar CreateVector(vector value)
     {
         SCR_BaseEditorAttributeVar var = new SCR_BaseEditorAttributeVar();
         var.SetVector(value);
         return var;
     }
 };