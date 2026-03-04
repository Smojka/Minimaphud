 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MapDescriptorProps
 {
     // todo: Api to set sizes and colors
     // todo: WB props

     proto external void Activate( bool enable );
     proto external void Setup( bool enable );
     proto external bool IsVisible();
     proto external void SetVisible( bool enable );
     proto external bool IsIconVisible();
     proto external void SetIconVisible( bool enable );
     proto void SetFont(ResourceName fontPath);
     proto external bool IsTextVisible();
     proto external void SetTextVisible( bool enable );
     proto external void SetTextOffset(vector offset);
     proto external void SetTextOffsetX(float x);
     proto external void SetTextOffsetY(float y);
     proto external void SetIconTextAlignH(float h);
     proto external void SetIconTextAlignV(float v);
     proto external void SetAlignWithParent(bool b);
     proto external void SetTextBold();
     proto external void SetTextItalic();
     proto external void SetIconSize( float size, float minScale, float maxScale );
     proto external float GetIconSize();
     proto external void SetTextSize( float size, float minScale, float maxScale );
     proto external float GetTextSize();
     proto external void SetDetail( int iDetail );
     proto external int GetDetail();
     proto external string GetImageDef();
     proto external void SetImageDef( string name );
     proto external void SetTextAngle(float angle);
     proto external float GetTextAngle();
     proto external void SetGroupType(int type);
     proto external int GetGroupType();
     proto external void SetGroupScale(float scale);
     proto external float GetGroupScale();
     proto external void SetPriority(int priority);
     proto external float GetPriority();
     proto external void SetFrontColor( notnull Color color );
     proto ref Color GetFrontColor();
     proto external void SetBackgroundColor( notnull Color color );
     proto ref Color GetBackgroundColor();
     proto external void SetOutlineColor( notnull Color color );
     proto ref Color GetOutlineColor();
     proto external void SetTextColor( notnull Color color );
     proto ref Color GetTextColor();
 }
