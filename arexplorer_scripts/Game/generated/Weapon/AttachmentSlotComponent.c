 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class AttachmentSlotComponentClass: BaseAttachmentSlotComponentClass
 {
 }

 class AttachmentSlotComponent: BaseAttachmentSlotComponent
 {
     proto external void SetAttachment(IEntity attachmentEntity);
     proto external IEntity GetAttachedEntity();
     proto external BaseAttachmentType GetAttachmentSlotType();
     proto external bool CanSetAttachment(IEntity attachmentEntity);
     proto external bool IsObstructing();
     proto external bool ShouldShowInInspection();

     // callbacks

     event bool ShouldSetAttachment(IEntity attachmentEntity) { return true; };
     event bool ShouldSetResource(Resource resource) { return true; };
 }
