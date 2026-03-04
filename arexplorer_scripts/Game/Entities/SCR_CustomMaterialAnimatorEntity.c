
 [EntityEditorProps(category: "GameScripted/MaterialAnimation", description: "This entity plays .emat animations consistently from the given starting frame to the given end frame. After the animation is done it can: Loop (default), hide the entity, delete the entity or delete the entity and children")]
 class SCR_CustomMaterialAnimatorEntityClass: GenericEntityClass
 {
 };
 class SCR_CustomMaterialAnimatorEntity: GenericEntity
 {
     [Attribute("1", params: "1 99999999", category: "Material Animation", desc: "Starting frame (texture in emat Albedo Map) of the animation. If m_bDeleteWhenDone is false then it will also restart on this frame again, Use in combination with m_iEndFrameIndex to play specific sections of the material animation. The first frame is 1 (so it is not an index)")]
     protected int m_iStartingFrame;

     [Attribute("-1", params: "-1 99999999", category: "Material Animation", desc: "At which frame (texture in the Albedo Map) should the animation stop playing (loop again or destroy entity). This should never be higher then Albedo Map texture count else there might be unintentional consequences.")]
     protected int m_iEndFrame;

     [Attribute("60", params: "0.001 99999999", category: "Material Animation", desc: "Use this to change the speed of the animation. Higher value means faster animation, if you have 60 frames and this value is 60 then the animation will be done in 1 second")]
     protected float m_fAnimationSpeed;

     [Attribute(defvalue: "0", category: "Material Animation", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EActionWhenDone), desc: "What will happen when the animation is done? By default it will loop but can also set to: Hide entity (sets visiblity flag), delete entity and delete entity & children")]
     protected EActionWhenDone m_iActionWhenDone;

     //~ This value is the actual varriable the emat will read. See the top of this script how to set up the emat.
     protected int m_iMaterialAnimationFrame = 0;

     //~ This value is used to calculate the next m_iMaterialAnimationFrame
     protected float m_fMaterialAnimationFrameCalculator = 0;


     override protected void EOnFrame(IEntity owner, float timeSlice)
     {
         //~ Increase the frame calculater
         m_fMaterialAnimationFrameCalculator += m_fAnimationSpeed * timeSlice;

         //~ Set frame to converted int of calculator. The m_iMaterialAnimationFrame is the actual value the emat will read
         m_iMaterialAnimationFrame = m_fMaterialAnimationFrameCalculator;

         //End of animation reached
         if (m_iMaterialAnimationFrame >= m_iEndFrame)
         {
             //~ Animation done, Loop again to m_iStartingFrame
             if (m_iActionWhenDone == EActionWhenDone.LOOP)
             {
                 m_fMaterialAnimationFrameCalculator = m_iStartingFrame;
                 m_iMaterialAnimationFrame = m_iStartingFrame;
             }
             //~ Animation done, remove on Frame and hide the entity
             else if (m_iActionWhenDone == EActionWhenDone.HIDE)
             {
                 ClearEventMask(EntityEvent.FRAME);
                 ClearFlags(EntityFlags.VISIBLE, true);
             }
             //~ Animation done delete self
             else if (m_iActionWhenDone == EActionWhenDone.DELETE_SELF)
             {
                 delete this;
             }
             //~ Animation done delete self and children
             else if (m_iActionWhenDone == EActionWhenDone.DELETE_SELF_AND_CHILDREN)
             {
                 SCR_EntityHelper.DeleteEntityAndChildren(this);
             }
         }
     }

     void SCR_CustomMaterialAnimatorEntity(IEntitySource src, IEntity parent)
     {
         if (m_iEndFrame < 1)
             Print("'SCR_CustomMaterialAnimatorEntity': 'm_iEndFrame' is not set correctly! Check the textures in the Albedo Map of the emat to find out what to set the 'm_iEndFrame' value to", LogLevel.WARNING);
         else if (m_iStartingFrame > m_iEndFrame)
             Print("'SCR_CustomMaterialAnimatorEntity': 'm_iStartingFrame' is higher then 'm_iEndFrame', make sure 'm_iStartingFrame' is lower then 'm_iEndFrame'!", LogLevel.WARNING);


         //~ System works with indexes so while varriables follow the same logic as the albinoMap starting at frame 1 and ending at count. The actual logic starts at index 0 and ends at count -1.
         m_iStartingFrame -= 1;
         m_iEndFrame -= 1;

         m_fMaterialAnimationFrameCalculator = m_iStartingFrame;
         m_iMaterialAnimationFrame = m_iStartingFrame;

         SetEventMask(EntityEvent.FRAME);
     }
 };

 enum EActionWhenDone
 {
     LOOP = 0,
     HIDE = 1,
     DELETE_SELF = 2,
     DELETE_SELF_AND_CHILDREN = 3,
 };








