
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
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
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
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] getGAFFileName 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] createObjectAndRun 
-- @param self
-- @param #bool looped
-- @return GAFObject#GAFObject ret (return value: gaf.GAFObject)
        
--------------------------------
-- / Sets desired atlas scale. Will choose nearest atlas scale from available
-- @function [parent=#GAFAsset] setDesiredAtlasScale 
-- @param self
-- @param #float scale
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
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
-- / Desired atlas scale. Default is 1.0f
-- @function [parent=#GAFAsset] desiredAtlasScale 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] initWithGAFBundle 
-- @param self
-- @param #string zipfilePath
-- @param #string entryFile
-- @param #function delegate
-- @param #gaf.GAFLoader customLoader
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setSceneWidth 
-- @param self
-- @param #unsigned int 
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setTextureLoadDelegate 
-- @param self
-- @param #function delegate
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
--------------------------------
-- / Initializes asset with bGAF data
-- @function [parent=#GAFAsset] initWithGAFFile 
-- @param self
-- @param #string filePath
-- @param #function delegate
-- @param #gaf.GAFLoader customLoader
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- 
-- @function [parent=#GAFAsset] setSceneFps 
-- @param self
-- @param #unsigned int 
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
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
-- @overload self, string         
-- @overload self, string, function, gaf.GAFLoader         
-- @function [parent=#GAFAsset] create
-- @param self
-- @param #string gafFilePath
-- @param #function delegate
-- @param #gaf.GAFLoader customLoader
-- @return GAFAsset#GAFAsset ret (return value: gaf.GAFAsset)

--------------------------------
-- @overload self, string, string         
-- @overload self, string, string, function, gaf.GAFLoader         
-- @function [parent=#GAFAsset] createWithBundle
-- @param self
-- @param #string zipfilePath
-- @param #string entryFile
-- @param #function delegate
-- @param #gaf.GAFLoader customLoader
-- @return GAFAsset#GAFAsset ret (return value: gaf.GAFAsset)

--------------------------------
-- 
-- @function [parent=#GAFAsset] GAFAsset 
-- @param self
-- @return GAFAsset#GAFAsset self (return value: gaf.GAFAsset)
        
return nil
