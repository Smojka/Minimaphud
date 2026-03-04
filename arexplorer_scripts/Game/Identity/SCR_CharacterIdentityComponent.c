 [ComponentEditorProps(category: "GameScripted/Identity", description: "")]
 class SCR_CharacterIdentityComponentClass : CharacterIdentityComponentClass
 {
 }

 class SCR_CharacterIdentityComponent : CharacterIdentityComponent
 {
     [Attribute("#AR-Idenity_Name_Format_Full")]
     protected LocalizedString m_IdentityFormatFull;

     [Attribute("#AR-Idenity_Name_Format_NoAlias")]
     protected LocalizedString m_IdentityFormatNoAlias;

     //------------------------------------------------------------------------------------------------
  void GetFormattedFullName(out string format, out string name, out string alias, out string surname)
     {
         name = GetIdentity().GetName();
         alias = GetIdentity().GetAlias();
         surname = GetIdentity().GetSurname();

         if (alias.IsEmpty())
             format = m_IdentityFormatNoAlias;
         else
             format = m_IdentityFormatFull;
     }

     //------------------------------------------------------------------------------------------------
  void GetFormattedFullName(out string format, out notnull array<string> names)
     {
         names.Clear();

         string name, alias, surname;
         GetFormattedFullName(format, name, alias, surname);

         names.Insert(name);
         names.Insert(alias);
         names.Insert(surname);
     }
 }