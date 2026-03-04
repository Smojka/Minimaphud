 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MapLayer
 {
     proto external int Index();
     proto external void SetFactionSize(int size);
     proto external MapDescriptorProps GetPropsFor( int iFaction, EMapDescriptorType type );
     proto external MapGridProps GetGridProps();
     proto external MapLegendProps GetLegendProps();
     proto external MapContourProps GetContourProps();
     proto external MapLinkProps GetMapLinkProps();
     proto external MapSelectionProperties GetSelectionProps();
     proto external MapRoadProps GetRoadProps();
     proto external MapPowerLineProps GetPowerLineProps();
     proto external MapBuildingProps GetBuildingProps();
     proto external MapWaterBodyProps GetWaterBodyProps();
     proto external MapHillProps GetHillProps();
     proto external MapAreaProperties GetAreaProps();
     proto external MapDrawingProps GetMapDrawingProps();
     proto void SetAll(  const EMapDescriptorType eType, const int iFactionIndex,  const int _fcolor, const int _bcolor, const int _ocolor, const int _tcolor,  const float _selfscale, const float _textscale,  const float selfScaleMin = 0.25, const float selfScaleMax = 4,  const float textScaleMin = 8, const float textScaleMax = 40,  vector offset = vector.Zero, ResourceName font = "" );
     proto external void ModifyFaction( int iFactionIndex, int uColor );
     proto external void SetCeiling(float limit);
     proto external float GetCeiling();
 }
