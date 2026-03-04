 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ItemPreviewManagerEntityClass: GenericEntityClass
 {
 }

 class ItemPreviewManagerEntity: GenericEntity
 {
     proto external void SetPreviewItem(ItemPreviewWidget widget, IEntity item, PreviewRenderAttributes attributes = null, bool forceRefresh = false);
     proto external void SetPreviewItemFromPrefab(ItemPreviewWidget widget, ResourceName prefabResource, PreviewRenderAttributes attributes = null, bool forceRefresh = false);
     proto external IEntity ResolvePreviewEntityForPrefab(ResourceName prefabResource);
 }
