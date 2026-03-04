 class SCR_StringHelper
 {
     static const string LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
     static const string UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
     static const string LETTERS = LOWERCASE + UPPERCASE;
     static const string DIGITS = "0123456789";
     static const string UNDERSCORE = "_";
     static const string DASH = "-";
     static const string SPACE = " ";
     static const string TAB = "\t";
     static const string LINE_RETURN = "\n";
     static const string LIPSUM = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
     protected static const string TRANSLATION_KEY_CHARS = UNDERSCORE + DASH;

     //------------------------------------------------------------------------------------------------
  static bool ContainsDigit(string input)
     {
         for (int i, len = input.Length(); i < len; i++)
         {
             int asciiValue = input[i].ToAscii();
             if (asciiValue >= 48 && asciiValue <= 57)
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool ContainsUppercase(string input)
     {
         for (int i, len = input.Length(); i < len; i++)
         {
             int asciiValue = input[i].ToAscii();
             if (asciiValue >= 65 && asciiValue <= 90)
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool ContainsLowercase(string input)
     {
         for (int i, len = input.Length(); i < len; i++)
         {
             int asciiValue = input[i].ToAscii();
             if (asciiValue >= 97 && asciiValue <= 122)
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static int CountOccurrences(string haystack, string needle, bool caseInsensitive = false)
     {
         if (needle.IsEmpty() || haystack.IsEmpty())
             return 0;

         if (caseInsensitive)
         {
             needle.ToLower();
             haystack.ToLower();
         }

         int needleLength = needle.Length();
         int haystackLength = haystack.Length();

         if (needleLength > haystackLength)
             return 0;

         int result;
         int searchIndex;
         while (searchIndex < haystackLength)
         {
             int resultIndex = haystack.IndexOfFrom(searchIndex, needle);
             if (resultIndex < 0)
                 break;

             result++;
             searchIndex = resultIndex + needleLength;
         }

         return result;
     }

 //  //------------------------------------------------------------------------------------------------
 //  //! Cut a line if bigger than the wanted length, using #AR-String_Ellipsis
 //  //! \param ellipsisTranslation translation key or ellipsis string; use %1 in it to place the cut text, if %1 is not present default value will be used
 //  static string Ellipsis(string input, int length, string ellipsisTranslation = "#AR-String_Ellipsis")
 //  {
 //      if (input.Length() < length)
 //          return input;
 //
 //      if (ellipsisTranslation != "#AR-String_Ellipsis" && !ellipsisTranslation.Contains("%1") && !Translate(ellipsisTranslation).Contains("%1"))
 //          ellipsisTranslation = "#AR-String_Ellipsis";
 //
 //      return String.Format(ellipsisTranslation, input.Substring(0, length));
 //  }

     //------------------------------------------------------------------------------------------------
  static string Filter(string input, string characters, bool useCharactersAsBlacklist = false)
     {
         if (input.IsEmpty() || (!useCharactersAsBlacklist && characters.IsEmpty()))
             return string.Empty;

         string result;
         for (int i, length = input.Length(); i < length; i++)
         {
             string letter = input[i];
             if (characters.Contains(letter) != useCharactersAsBlacklist)
                 result += letter;
         }
         return result;
     }

     //------------------------------------------------------------------------------------------------
  static bool IsFormat(SCR_EStringFormat format, string input)
     {
         switch (format)
         {
             case SCR_EStringFormat.ALPHABETICAL_UC:     return CheckCharacters(input, false, true, false);
             case SCR_EStringFormat.ALPHABETICAL_LC:     return CheckCharacters(input, true, false, false);
             case SCR_EStringFormat.ALPHABETICAL_I:      return CheckCharacters(input, true, true, false);
             case SCR_EStringFormat.ALPHANUMERICAL_UC:   return CheckCharacters(input, false, true, true);
             case SCR_EStringFormat.ALPHANUMERICAL_LC:   return CheckCharacters(input, true, false, true);
             case SCR_EStringFormat.ALPHANUMERICAL_I:    return CheckCharacters(input, true, true, true);
             case SCR_EStringFormat.DIGITS_ONLY:         return CheckCharacters(input, false, false, true);
         }
         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool CheckCharacters(string input, bool allowLC, bool allowUC, bool allowDigits, bool allowUnderscore = false)
     {
         if (input.IsEmpty())
             return false;

         for (int i, len = input.Length(); i < len; i++)
         {
             int asciiValue = input[i].ToAscii();
             if (!(
                 (allowLC && asciiValue >= 97 && asciiValue <= 122) ||
                 (allowUC && asciiValue >= 65 && asciiValue <= 90) ||
                 (allowDigits && asciiValue >= 48 && asciiValue <= 57) ||
                 (allowUnderscore && asciiValue == 95)
             ))
                 return false;
         }

         return true;
     }

 //  //------------------------------------------------------------------------------------------------
 //  //! old method, 4-5Ã slower but allows for non-ASCII values, not useful for now
 //  protected static bool CheckCharactersOld(string input, bool allowLC, bool allowUC, bool allowDigits, bool allowUnderscore)
 //  {
 //      string filter;
 //      if (allowLC)
 //          filter += LOWERCASE;
 //      if (allowUC)
 //          filter += UPPERCASE;
 //      if (allowDigits)
 //          filter += DIGITS;
 //      if (allowUnderscore)
 //          filter += UNDERSCORE;
 //
 //      if (filter.IsEmpty())
 //          return false;
 //
 //      for (int i, len = input.Length(); i < len; i++)
 //      {
 //          if (!filter.Contains(input[i]))
 //              return false;
 //      }
 //
 //      return true;
 //  }

     //------------------------------------------------------------------------------------------------
  static string Format(string input, notnull array<string> arguments)
     {
         if (input.IsEmpty())
             return string.Empty;

         if (!input.Contains("%"))
             return input;

         switch (arguments.Count())
         {
             case 0: return string.Format(input);
             case 1: return string.Format(input, arguments[0]);
             case 2: return string.Format(input, arguments[0], arguments[1]);
             case 3: return string.Format(input, arguments[0], arguments[1], arguments[2]);
             case 4: return string.Format(input, arguments[0], arguments[1], arguments[2], arguments[3]);
             case 5: return string.Format(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
             case 6: return string.Format(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]);
             case 7: return string.Format(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6]);
             case 8: return string.Format(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7]);
         }

         // 9 and more
         return string.Format(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7], arguments[8]);
     }

     //------------------------------------------------------------------------------------------------
  static array<int> GetIntsFromString(string input)
     {
         array<int> result = {};
         array<string> splits = {};
         input.Split(SPACE, splits, true);

         foreach (string split : splits)
         {
             int value = split.ToInt();
             if (value != 0 || split == "0")
                 result.Insert(value);
         }
         return result;
     }

     //------------------------------------------------------------------------------------------------
  static int IndexOf(string input, notnull array<string> samples)
     {
         if (input.IsEmpty() || samples.IsEmpty())
             return -1;

         int result = int.MAX;
         foreach (string sample : samples)
         {
             int index = input.IndexOf(sample);
             if (index != -1 && index < result)
                 result = index;
         }

         if (result == int.MAX)
             return -1;

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static int IndexOfFrom(string input, int start, notnull array<string> samples)
     {
         if (start < 0 || start > input.Length() || input.IsEmpty() || samples.IsEmpty())
             return -1;

         int result = int.MAX;
         foreach (string sample : samples)
         {
             int index = input.IndexOfFrom(start, sample);
             if (index != -1 && index < result)
                 result = index;
         }

         if (result == int.MAX)
             return -1;

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static array<float> GetFloatsFromString(string input)
     {
         array<float> result = {};
         array<string> splits = {};
         input.Split(SPACE, splits, true);

         foreach (string split : splits)
         {
             float value = split.ToFloat();
             if (value != 0 || split.StartsWith("0"))
                 result.Insert(value);
         }

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static array<string> GetLines(string input, bool removeEmptyLines = false, bool trimLines = false)
     {
         if (!input)
         {
             if (removeEmptyLines)
                 return {};
             else
                 return { string.Empty };
         }

         array<string> result = {};
         input.Split("\n", result, removeEmptyLines);

         if (trimLines)
         {
             string line;
             for (int i = result.Count() - 1; i >= 0; --i)
             {
                 line = result[i];
                 line.TrimInPlace();
                 if (removeEmptyLines && !line)
                     result.RemoveOrdered(i);
                 else
                     result[i] = line;
             }
         }

         return result;
     }

     //------------------------------------------------------------------------------------------------
  static string InsertAt(string input, string insertion, int insertionIndex = 0)
     {
         if (input.IsEmpty() || insertion.IsEmpty() || insertionIndex < 0 || insertionIndex > input.Length())
             return input;

         if (insertionIndex == 0)
             return insertion + input;

         return input.Substring(0, insertionIndex) + insertion + input.Substring(insertionIndex, input.Length() - insertionIndex);
     }

     //------------------------------------------------------------------------------------------------
  static bool IsEmptyOrWhiteSpace(string input)
     {
         return input.Trim().IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  static bool IsTranslationKey(string input)
     {
         if (IsEmptyOrWhiteSpace(input))
             return false;

         if (input != input.Trim())
             return false;

         if (input.Length() < 2 || !input.StartsWith("#"))
             return false;

         for (int i, count = TRANSLATION_KEY_CHARS.Length(); i < count; i++)
         {
             if (input.EndsWith(TRANSLATION_KEY_CHARS[i]))
                 return false;
         }

         string filter = LETTERS;
         if (!filter.Contains(input[1])) // \#[a-zA-Z].*
             return false;

         filter += DIGITS + TRANSLATION_KEY_CHARS;
         for (int i, len = input.Length(); i < len; i++)
         {
             if (!filter.Contains(input[i]))
                 return false;
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static string Join(string separator, notnull array<string> pieces, bool joinEmptyEntries = true)
     {
         if (pieces.IsEmpty())
             return string.Empty;

         string result;
         foreach (int i, string piece : pieces)
         {
             if (i == 0)
                 result = piece;
             else
             if (joinEmptyEntries || piece) // !piece.IsEmpty()'s fast version
                 result += separator + piece;
         }
         return result;
     }

     //------------------------------------------------------------------------------------------------
  static string PadLeft(string input, int length, string padding = SPACE)
     {
         if (!padding)
             return input;

         if (input.Length() >= length)
             return input;

         int padW = padding.Length();
         for (int i = length - input.Length() - 1; i >= 0; i -= padW)
         {
             input = padding + input;
         }

         if (input.Length() > length)
             input = input.Substring(input.Length() - length, length);

         return input;
     }

     //------------------------------------------------------------------------------------------------
  static string PadRight(string input, int length, string padding = SPACE)
     {
         if (!padding)
             return input;

         if (input.Length() >= length)
             return input;

         int padW = padding.Length();
         for (int i = length - input.Length() - 1; i >= 0; i -= padW)
         {
             input += padding;
         }

         if (input.Length() > length)
             input = input.Substring(0, length);

         return input;
     }

     //------------------------------------------------------------------------------------------------
  static string ReplaceRecursive(string input, string sample, string replace)
     {
         if (input.IsEmpty() || sample.IsEmpty() || sample == replace || replace.Contains(sample))
             return input;

         while (input.IndexOf(sample) > -1)
         {
             input.Replace(sample, replace);
         }

         return input;
     }

     //------------------------------------------------------------------------------------------------
  static string ReplaceTimes(string input, string sample, string replace, int howMany = 1, int skip = 0)
     {
         if (howMany < 1 || input.IsEmpty() || sample.IsEmpty() || sample == replace)
             return input;

         int sampleLength = sample.Length();
         int replaceLength = replace.Length();

         int index;
         while (howMany > 0)
         {
             index = input.IndexOfFrom(index, sample);
             if (index < 0)
                 break;

             if (skip > 0)
             {
                 skip--;
                 index += sampleLength;
                 continue;
             }

             if (index == 0)
                 input = replace + input.Substring(sampleLength, input.Length() - sampleLength);
             else
                 input = input.Substring(0, index) + replace + input.Substring(index + sampleLength, input.Length() - (index + sampleLength));

             // no overlap
             index += replaceLength;

             howMany--;
         }

         return input;
     }

     //------------------------------------------------------------------------------------------------
  static string Reverse(string input)
     {
         string result;
         for (int i = input.Length() - 1; i >= 0; i--)
         {
             result += input[i];
         }
         return result;
     }

     //------------------------------------------------------------------------------------------------
  static bool ContainsAny(string input, notnull array<string> needles)
     {
         if (input.IsEmpty())
             return false;

         foreach (string needle : needles)
         {
             if (input.Contains(needle))
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool ContainsEvery(string input, notnull array<string> needles)
     {
         if (input.IsEmpty())
             return false;

         foreach (string needle : needles)
         {
             if (!input.Contains(needle))
                 return false;
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool StartsWithAny(string input, notnull array<string> lineStarts)
     {
         if (input.IsEmpty())
             return false;

         foreach (string lineStart : lineStarts)
         {
             if (input.StartsWith(lineStart))
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static bool EndsWithAny(string input, notnull array<string> lineEnds)
     {
         if (input.IsEmpty())
             return false;

         foreach (string lineEnd : lineEnds)
         {
             if (input.EndsWith(lineEnd))
                 return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  static string Translate(
         string input,
         string param1 = string.Empty,
         string param2 = string.Empty,
         string param3 = string.Empty,
         string param4 = string.Empty,
         string param5 = string.Empty,
         string param6 = string.Empty,
         string param7 = string.Empty,
         string param8 = string.Empty,
         string param9 = string.Empty)
     {
         return WidgetManager.Translate(input, param1, param2, param3, param4, param5, param6, param7, param8, param9);
     }

     //------------------------------------------------------------------------------------------------
  static string Translate(string input, notnull array<string> arguments)
     {
         if (input.IsEmpty())
             return string.Empty;

         if (!input.Contains("%"))
             return input;

         switch (arguments.Count())
         {
             case 0: return WidgetManager.Translate(input);
             case 1: return WidgetManager.Translate(input, arguments[0]);
             case 2: return WidgetManager.Translate(input, arguments[0], arguments[1]);
             case 3: return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2]);
             case 4: return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2], arguments[3]);
             case 5: return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
             case 6: return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]);
             case 7: return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6]);
             case 8: return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7]);
         }

         // 9 and more
         return WidgetManager.Translate(input, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7], arguments[8]);
     }

     //------------------------------------------------------------------------------------------------
  static string TrimLeft(string input)
     {
         if (input.IsEmpty())
             return string.Empty;

         for (int i, count = input.Length(); i < count; i++)
         {
             string character = input[i];
             if (character == SPACE || character == TAB || character == LINE_RETURN)
                 continue;

             return input.Substring(i, count - i);
         }

         return string.Empty;
     }

     //------------------------------------------------------------------------------------------------
  static string TrimRight(string input)
     {
         if (input.IsEmpty())
             return string.Empty;

         for (int i = input.Length() - 1; i >= 0; i--)
         {
             string character = input[i];
             if (character == SPACE || character == TAB || character == LINE_RETURN)
                 continue;

             return input.Substring(0, i + 1);
         }

         return string.Empty;
     }
 }

 enum SCR_EStringFormat
 {
     ALPHABETICAL_UC,
     ALPHABETICAL_LC,
     ALPHABETICAL_I,
     ALPHANUMERICAL_UC,
     ALPHANUMERICAL_LC,
     ALPHANUMERICAL_I,
     DIGITS_ONLY,
     // FLOATING_POINT,      //!< [0-9][0-9\.]+[0-9]
 }