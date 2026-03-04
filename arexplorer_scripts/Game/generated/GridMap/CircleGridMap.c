 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CircleGridMap: BaseGridMap
 {
     void CircleGridMap(float cellSize = 100.0) {}

     proto external void Insert(IEntity entity, float radius, bool isDynamic, int mask = 0);
     proto external void UpdateRadius(IEntity entity, float radius);
 }
