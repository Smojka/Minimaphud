 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MapEntityClass: GenericEntityClass
 {
 }

 class MapEntity: GenericEntity
 {
     proto external void UpdateTexts();
     proto external void AbsorbData();
     proto external vector Size();
     proto external float TerrainUnitScale();
     proto external void SetSelection( vector start, vector end );
     proto external void ResetSelection();
     proto external void SetFrame( vector start, vector end );
     proto external void ZoomChange( float level );
     proto external void PosChange( float x, float y );
     proto external void CursorChange( float x, float y );
     proto external float MinElevation();
     proto external float MaxElevation();
     proto external float ElevationAt( vector pos );
     proto external void ShowDiag( bool bEnable );
     proto external MapItem CreateCustomMapItem();
     proto external int CountOfVisible();
     proto external MapItem GetVisible( int iIndex );
     proto external int GetVisibleAll( out notnull array<MapItem> outItems);
     proto external int GetInsideRect( out notnull array<MapItem> outItems, vector from, vector to );
     proto external int GetInsideCircle( out notnull array<MapItem> outItems, vector origin, float range );
     proto external MapItem GetClose( vector origin, float range );
     proto external int GetSelected( out notnull array<MapItem> outItems );
     proto external int GetByType( out notnull array<MapItem> outItems, /*EMapDescriptorType*/ int type );
     proto external void ResetSelected();
     proto external void ResetHovering();
     proto external void ResetHighlighted();
     proto external void ShowMultiple( int type, bool bEnable );
     proto external void ShowEditor( bool bEnable );
     proto external void ShowDebug( bool bEnable );
     proto external void EnableVisualisation( bool bValue );
     proto external void EnableGrid( bool bValue );
     proto external void EnableOverlay( bool bValue );
     proto external protected void EnableLegend( bool bValue );
     proto external void InitializeLayers(int count, int factionSize = 4);
     proto external int LayerCount();
     proto external MapLayer GetLayer( int index );
     proto external void SetLayer( int index );
     proto external int GetLayerIndex();
     proto external void SetImagesetMapping(notnull array<int> values);
 }
