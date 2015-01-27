
--------------------------------
-- @module GAFAsset
-- @extend Ref
-- @parent_module gaf

--------------------------------
-- 
-- @function [parent=#GAFAsset] getSceneHeight 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setSceneHeight 
-- @param self
-- @param #unsigned int 
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getSceneColor 
-- @param self
-- @return color4b_table#color4b_table ret (return value: color4b_table)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setSceneColor 
-- @param self
-- @param #color4b_table 
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] createObjectAndRun 
-- @param self
-- @param #bool looped
-- @return GAFObject#GAFObject ret (return value: gaf.GAFObject)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getSceneWidth 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getRootTimeline 
-- @param self
-- @return GAFTimeline#GAFTimeline ret (return value: gaf.GAFTimeline)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getSceneFps 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] initWithGAFBundle 
-- @param self
-- @param #string zipfilePath
-- @param #string entryFile
-- @param #function delegate
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setSceneWidth 
-- @param self
-- @param #unsigned int 
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setTextureLoadDelegate 
-- @param self
-- @param #function delegate
        
--------------------------------
-- / Initializes asset with bGAF data
-- @function [parent=#GAFAsset] initWithGAFFile 
-- @param self
-- @param #string filePath
-- @param #function delegate
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setSceneFps 
-- @param self
-- @param #unsigned int 
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] createObject 
-- @param self
-- @return GAFObject#GAFObject ret (return value: gaf.GAFObject)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getTextureManager 
-- @param self
-- @return GAFAssetTextureManager#GAFAssetTextureManager ret (return value: gaf.GAFAssetTextureManager)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getTimelineByName 
-- @param self
-- @param #string name
-- @return GAFTimeline#GAFTimeline ret (return value: gaf.GAFTimeline)
        
--------------------------------
-- / List of GAFAnimationFrame objects	
-- @function [parent=#GAFAsset] isAssetVersionPlayable 
-- @param self
-- @param #char version
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- / desired content scale factor
-- @function [parent=#GAFAsset] desiredCsf 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @overload self, string         
-- @overload self, string, function         
-- @function [parent=#GAFAsset] create
-- @param self
-- @param #string gafFilePath
-- @param #function delegate
-- @return GAFAsset#GAFAsset ret (return value: gaf.GAFAsset)

--------------------------------
-- / sets desired content scale factor
-- @function [parent=#GAFAsset] setDesiredCsf 
-- @param self
-- @param #float csf
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] createWithBundle 
-- @param self
-- @param #string zipfilePath
-- @param #string entryFile
-- @param #function delegate
-- @return GAFAsset#GAFAsset ret (return value: gaf.GAFAsset)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] GAFAsset 
-- @param self
        
return nil
