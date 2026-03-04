 [EntityEditorProps(category: "GameScripted/Editor", description: "Core Editor manager", color: "251 91 0 255")]
 class SCR_EditorBaseEntityClass: GenericEntityClass
 {
 };

 class SCR_EditorBaseEntity : GenericEntity
 {

     ScriptInvoker GetOnInit()
     {
         return null;
     }
     ScriptInvoker GetOnRequest()
     {
         return null;
     }
     ScriptInvoker GetOnOpened()
     {
         return null;
     }
     ScriptInvoker GetOnPreActivate()
     {
         return null;
     }
     ScriptInvoker GetOnActivate()
     {
         return null;
     }
     ScriptInvoker GetOnPostActivate()
     {
         return null;
     }
     ScriptInvoker GetOnDeactivate()
     {
         return null;
     }
     ScriptInvoker GetOnClosed()
     {
         return null;
     }
     ScriptInvoker GetOnDebug()
     {
         return null;
     }


     ScriptInvoker GetOnOpenedServer()
     {
         return null;
     }
     ScriptInvoker GetOnOpenedServerCallback()
     {
         return null;
     }
     ScriptInvoker GetOnClosedServer()
     {
         return null;
     }
     ScriptInvoker GetOnClosedServerCallback()
     {
         return null;
     }
     ScriptInvoker GetOnActivateServer()
     {
         return null;
     }
     ScriptInvoker GetOnDeactivateServer()
     {
         return null;
     }

     bool IsOpened()
     {
         return false;
     }
     SCR_EditorManagerEntity GetManager()
     {
         return null;
     }
     int FindEditorComponents(out array<SCR_BaseEditorComponent> outComponents)
     {
         if (!outComponents) outComponents = {};
         array<Managed> components = {};
         for (int i = 0, count = FindComponents(SCR_BaseEditorComponent, components); i < count; i++)
         {
             outComponents.Insert(SCR_BaseEditorComponent.Cast(components[i]));
         }
         return outComponents.Count();
     }

     protected void InitComponents(bool isServer)
     {
         array<Managed> components = {};
         for (int i = 0, count = FindComponents(SCR_BaseEditorComponent, components); i < count; i++)
         {
             InitComponent(SCR_BaseEditorComponent.Cast(components[i]), isServer);
         }
     }
     protected void InitComponent(SCR_BaseEditorComponent component, bool isServer)
     {
         if (isServer)
             component.InitServer();
         else
             component.InitOwner();

         //--- Initialize sub-components
         array<GenericComponent> components = {};
         int componentsCount = component.FindComponents(SCR_BaseEditorComponent, components);
         for (int i = 0; i < componentsCount; i++)
         {
             InitComponent(SCR_BaseEditorComponent.Cast(components[i]), isServer);
         }
     }
 };