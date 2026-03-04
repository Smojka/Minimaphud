 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MapItem
 {
     proto external IEntity Entity();
     proto external MapDescriptorComponent Descriptor();
     proto external void Recycle();
     proto external void SetVisible( bool enable );
     proto external bool IsVisible();
     proto external void SetEditor( bool enable );
     proto external bool IsEditor();
     proto external void SetHovering( bool enable );
     proto external bool IsHovering();
     proto external void SetHighlighted( bool enable );
     proto external bool IsHighlighted();
     proto external void Select( bool enable );
     proto external bool IsSelected();
     proto external void SetLayerLinkProps( bool enable );
     proto external bool IsUseLayerLinkProps();
     proto external string GetDisplayName();
     proto external void SetDisplayName(string name, array<string> params = null);
     proto external MapLink LinkTo( MapItem item );
     proto external int GetGroupType();
     proto external void SetGroupType( int type );
     proto external int GetPriority();
     proto external void SetPriority( int iPriority );
     proto external void UnLink( MapItem item );
     proto external int GetLinks( out notnull array<MapLink> outItems );
     proto external void ClearLinks();
     proto external string GetInfoText();
     proto external void SetInfoText(string name, array<string> params = null);
     proto external string GetImageDef();
     proto external void SetImageDef( string name );
     proto external float GetRange();
     proto external void SetRange( float range );
     proto external void SetAngle( float angle );
     proto external float GetAngle();
     proto external vector GetPos();
     proto external void SetPos( float x, float z );
     /*
     \brief Get access to object local properties
     */
     proto external MapDescriptorProps GetProps();
     proto external void SetProps(MapDescriptorProps newProps);
     proto external int GetBaseType();
     proto external void SetBaseType( int type );
     proto external int GetFactionIndex();
     proto external void SetFactionIndex( int iIndex );
 }
