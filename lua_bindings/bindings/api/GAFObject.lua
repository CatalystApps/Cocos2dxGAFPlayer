
--------------------------------
-- @module GAFObject
-- @extend GAFSprite
-- @parent_module gaf

--------------------------------
-- / @note do not forget to call setAnimationStartedNextLoopDelegate(nullptr) before deleting your subscriber
-- @function [parent=#GAFObject] setAnimationStartedNextLoopDelegate 
-- @param self
-- @param #function delegate
        
--------------------------------
-- / @note do not forget to call setAnimationFinishedPlayDelegate(nullptr) before deleting your subscriber
-- @function [parent=#GAFObject] setAnimationFinishedPlayDelegate 
-- @param self
-- @param #function delegate
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getBoundingBoxForCurrentFrame 
-- @param self
-- @return rect_table#rect_table ret (return value: rect_table)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] setFps 
-- @param self
-- @param #unsigned int value
        
--------------------------------
-- @overload self, string         
-- @overload self, string         
-- @function [parent=#GAFObject] getObjectByName
-- @param self
-- @param #string name
-- @return GAFObject#GAFObject ret (return value: gaf.GAFObject)

--------------------------------
-- / Stops playing an animation as a sequence
-- @function [parent=#GAFObject] clearSequence 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getIsAnimationRunning 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @overload self, unsigned int         
-- @overload self, string         
-- @function [parent=#GAFObject] gotoAndStop
-- @param self
-- @param #string frameLabel
-- @return bool#bool ret (return value: bool)

--------------------------------
-- 
-- @function [parent=#GAFObject] getStartFrame 
-- @param self
-- @param #string frameLabel
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- / @note do not forget to call setFramePlayedDelegate(nullptr) before deleting your subscriber
-- @function [parent=#GAFObject] setFramePlayedDelegate 
-- @param self
-- @param #function delegate
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getCurrentFrameIndex 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getTotalFrameCount 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] start 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#GAFObject] init 
-- @param self
-- @param #gaf.GAFAsset anAnimationData
-- @param #gaf.GAFTimeline timeline
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] isVisibleInCurrentFrame 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] isDone 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- / Plays animation sequence with specified name/ @param name a sequence name/ @param looped if true - sequence should play in cycle/ @param resume if true - animation will be played immediately, if false - playback will be paused after the first frame is shown/ @param hint specific animation playback parameters
-- @function [parent=#GAFObject] playSequence 
-- @param self
-- @param #string name
-- @param #bool looped
-- @param #bool resume
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- / schedule/unschedule/ @note this function is automatically called in start/stop
-- @function [parent=#GAFObject] enableTick 
-- @param self
-- @param #bool val
        
--------------------------------
-- 
-- @function [parent=#GAFObject] stop 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#GAFObject] setAnimationRunning 
-- @param self
-- @param #bool value
-- @param #bool recurcive
        
--------------------------------
-- 
-- @function [parent=#GAFObject] isReversed 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] setFrame 
-- @param self
-- @param #unsigned int index
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] setLooped 
-- @param self
-- @param #bool looped
-- @param #bool recursive
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getEndFrame 
-- @param self
-- @param #string frameLabel
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- / Pauses animation including enclosed timelines
-- @function [parent=#GAFObject] pauseAnimation 
-- @param self
        
--------------------------------
-- @overload self, unsigned int         
-- @overload self, string         
-- @function [parent=#GAFObject] gotoAndPlay
-- @param self
-- @param #string frameLabel
-- @return bool#bool ret (return value: bool)

--------------------------------
-- 
-- @function [parent=#GAFObject] isLooped 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- / Resumes animation including enclosed timelines
-- @function [parent=#GAFObject] resumeAnimation 
-- @param self
        
--------------------------------
-- 
-- @function [parent=#GAFObject] setReversed 
-- @param self
-- @param #bool reversed
-- @param #bool fromCurrentFrame
        
--------------------------------
-- 
-- @function [parent=#GAFObject] hasSequences 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getFps 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] create 
-- @param self
-- @param #gaf.GAFAsset anAsset
-- @param #gaf.GAFTimeline timeline
-- @return GAFObject#GAFObject ret (return value: gaf.GAFObject)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] getNodeToParentTransform 
-- @param self
-- @return mat4_table#mat4_table ret (return value: mat4_table)
        
--------------------------------
-- 
-- @function [parent=#GAFObject] GAFObject 
-- @param self
        
return nil
