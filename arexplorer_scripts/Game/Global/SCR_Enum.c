 class SCR_Enum
 {
     //------------------------------------------------------------------------------------------------
  static bool HasFlag(int flags, int condition)
     {
         return (flags & condition) == condition;
     }

     //------------------------------------------------------------------------------------------------
  static bool HasPartialFlag(int flags, int condition)
     {
         return (condition & ~flags) != condition;
     }

     //------------------------------------------------------------------------------------------------
  static int SetFlag(int flags, int flagToSet)
     {
         flags |= flagToSet;
         return flags;
     }

     //------------------------------------------------------------------------------------------------
  static int RemoveFlag(int flags, int flagToRemove)
     {
         flags &= ~flagToRemove;
         return flags;
     }

     //------------------------------------------------------------------------------------------------
  static int BitToIntArray(int bitValue, out notnull array<int> intValues)
     {
         intValues.Clear();
         int count = 0;
         int i = 1;
         while (i > 0 && i < int.MAX)
         {
             if (bitValue & i)
             {
                 intValues.Insert(i);
                 count++;
             }
             i *= 2;
         }
         return count;
     }

     //------------------------------------------------------------------------------------------------
  static int IntToBitArray(notnull array<int> intValues)
     {
         int bitValue;
         foreach (int intValue: intValues)
         {
             bitValue |= intValue;
         }
         return bitValue;
     }

     //------------------------------------------------------------------------------------------------
  static string GetEnumName(typename enumType, int enumValue)
     {
         return typename.EnumToString(enumType, enumValue);
     }

     //------------------------------------------------------------------------------------------------
  static int GetEnumNames(typename enumType, out notnull array<string> stringValues)
     {
         stringValues.Clear();
         array<int> values = {};
         GetEnumValues(enumType, values);
         for (int i, count = values.Count(); i < count; i++)
         {
             stringValues.Insert(typename.EnumToString(enumType, values[i]));
         }
         return stringValues.Count();
     }

     //------------------------------------------------------------------------------------------------
  static int GetEnumValues(typename enumType, out notnull array<int> intValues)
     {
         int val;
         for (int i, count = enumType.GetVariableCount(); i < count; i++)
         {
             if (enumType.GetVariableValue(null, i, val))
                 intValues.Insert(val);
         }
         return intValues.Count();
     }

     //------------------------------------------------------------------------------------------------
  static int GetFlagValues(typename enumType)
     {
         int val, flags;
         for (int i, count = enumType.GetVariableCount(); i < count; i++)
         {
             if (enumType.GetVariableValue(null, i, val))
                 flags |= val;
         }
         return flags;
     }

     //------------------------------------------------------------------------------------------------
  static bool GetRange(typename enumType, out int min, out int max)
     {
         min = int.MAX;
         max = int.MIN;
         int val, count = enumType.GetVariableCount();
         for (int i = 0; i < count; i++)
         {
             if (enumType.GetVariableValue(null, i, val))
             {
                 min = Math.Min(min, val);
                 max = Math.Max(max, val);
             }
         }
         return count > 0;
     }

     //------------------------------------------------------------------------------------------------
  static ParamEnumArray GetList(typename e, ParamEnum customValue1 = null, ParamEnum customValue2 = null, ParamEnum customValue3 = null)
     {
         ParamEnumArray params = ParamEnumArray.FromEnum(e);

         if (customValue3)
             params.InsertAt(customValue3, 0);
         if (customValue2)
             params.InsertAt(customValue2, 0);
         if (customValue1)
             params.InsertAt(customValue1, 0);

         return params;
     }

     //------------------------------------------------------------------------------------------------
  static ParamEnumArray GetList(notnull array<string> names)
     {
         ParamEnumArray params = {};
         for (int i, count = names.Count(); i < count; i++)
         {
             params.Insert(new ParamEnum(names[i], i.ToString()));
         }
         return params;
     }

     //------------------------------------------------------------------------------------------------
  static ParamEnumArray GetFlags(notnull array<string> names)
     {
         ParamEnumArray params = {};
         for (int i, count = names.Count(); i < count; i++)
         {
             params.Insert(new ParamEnum(names[i], Math.Pow(2, i).ToString()));
         }
         return params;
     }

     //------------------------------------------------------------------------------------------------
  static string FlagsToString(typename e, int flags, string delimiter = ", ", string noValue = "N/A")
     {
         if (flags <= 0)
             return noValue;

         array<int> outValues = {};

         int count = BitToIntArray(flags, outValues);
         if (count == 0)
             return string.Empty;

         string result = typename.EnumToString(e, outValues[0]);
         for (int i = 1; i < count; i++)
         {
             result += delimiter;
             result += typename.EnumToString(e, outValues[i]);
         }
         return result;
     }

     //------------------------------------------------------------------------------------------------
  static string GetDefault(int enumValue)
     {
         return string.Format("%1", enumValue);
     }

     //------------------------------------------------------------------------------------------------
  static string GetDefault(typename enumType)
     {
         return GetDefault(GetFlagValues(enumType));
     }
 };