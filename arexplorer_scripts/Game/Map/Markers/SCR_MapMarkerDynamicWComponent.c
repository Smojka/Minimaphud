 class SCR_MapMarkerDynamicWComponent : SCR_ScriptedWidgetComponent
 {
     protected int m_iLayerID;           // map layer ID
     protected ImageWidget m_wMarkerIcon;
     protected TextWidget m_wMarkerText;
     protected SCR_MapMarkerEntity m_MarkerEnt;

     //------------------------------------------------------------------------------------------------
  void SetMarkerEntity(notnull SCR_MapMarkerEntity marker)
     {
         m_MarkerEnt = marker;
     }

     //------------------------------------------------------------------------------------------------
     void SetLayerID(int id)
     {
         m_iLayerID = id;
     }

     //------------------------------------------------------------------------------------------------
  void SetImage(ResourceName icon, string quad, float aspectRatio = 1, int sizeFlag = 64)
     {
         m_wMarkerIcon.LoadImageFromSet(0, icon, quad);
         if (aspectRatio != 1 && aspectRatio > 0)
             m_wMarkerIcon.SetSize(sizeFlag * 0.9, (sizeFlag / aspectRatio) * 0.9); // todo, temp size adjust before symbols group side are fixed
     }

     //------------------------------------------------------------------------------------------------
  void SetText(string text)
     {
         m_wMarkerText.SetText(text);
     }

     //------------------------------------------------------------------------------------------------
  void SetTextVisible(bool state)
     {
         m_wMarkerText.SetVisible(state);
     }

     //------------------------------------------------------------------------------------------------
  void SetColor(Color color)
     {
         m_wMarkerIcon.SetColor(color);
     }

     //------------------------------------------------------------------------------------------------
     override void HandlerAttached(Widget w)
     {
         super.HandlerAttached(w);

         m_wMarkerIcon = ImageWidget.Cast(m_wRoot.FindAnyWidget("MarkerIcon"));
         m_wMarkerText = TextWidget.Cast(m_wRoot.FindAnyWidget("MarkerText"));
     }
 }