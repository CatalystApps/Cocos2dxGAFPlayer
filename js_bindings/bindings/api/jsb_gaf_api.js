/**
 * @module gaf
 */
var gaf = gaf || {};

/**
 * @class GAFAnimationSequence
 */
gaf.GAFAnimationSequence = {

/**
 * @method length
 * @return {unsigned int}
 */
length : function (
)
{
    return 0;
},

};

/**
 * @class GAFAsset
 */
gaf.GAFAsset = {

/**
 * @method getSceneHeight
 * @return {unsigned int}
 */
getSceneHeight : function (
)
{
    return 0;
},

/**
 * @method setSceneHeight
 * @param {unsigned int} arg0
 */
setSceneHeight : function (
int 
)
{
},

/**
 * @method getSceneColor
 * @return {color4b_object}
 */
getSceneColor : function (
)
{
    return cc.Color4B;
},

/**
 * @method setSceneColor
 * @param {color4b_object} arg0
 */
setSceneColor : function (
color4b 
)
{
},

/**
 * @method createObjectAndRun
 * @return {gaf.GAFObject}
 */
createObjectAndRun : function (
)
{
    return gaf.GAFObject;
},

/**
 * @method getSceneWidth
 * @return {unsigned int}
 */
getSceneWidth : function (
)
{
    return 0;
},

/**
 * @method getRootTimeline
 * @return {gaf.GAFTimeline}
 */
getRootTimeline : function (
)
{
    return gaf.GAFTimeline;
},

/**
 * @method getTimelines
* @return {map_object|map_object}
*/
getTimelines : function(
)
{
    return map_object;
},

/**
 * @method getSceneFps
 * @return {unsigned int}
 */
getSceneFps : function (
)
{
    return 0;
},

/**
 * @method initWithGAFBundle
 * @param {String} arg0
 * @param {String} arg1
 * @param {function} arg2
 * @return {bool}
 */
initWithGAFBundle : function (
str, 
str, 
func 
)
{
    return false;
},

/**
 * @method setSceneWidth
 * @param {unsigned int} arg0
 */
setSceneWidth : function (
int 
)
{
},

/**
 * @method setTextureLoadDelegate
 * @param {function} arg0
 */
setTextureLoadDelegate : function (
func 
)
{
},

/**
 * @method initWithGAFFile
 * @param {String} arg0
 * @param {function} arg1
 * @return {bool}
 */
initWithGAFFile : function (
str, 
func 
)
{
    return false;
},

/**
 * @method setSceneFps
 * @param {unsigned int} arg0
 */
setSceneFps : function (
int 
)
{
},

/**
 * @method getHeader
 * @return {gaf.GAFHeader}
 */
getHeader : function (
)
{
    return gaf.GAFHeader;
},

/**
 * @method createObject
 * @return {gaf.GAFObject}
 */
createObject : function (
)
{
    return gaf.GAFObject;
},

/**
 * @method getTextureManager
 * @return {gaf.GAFAssetTextureManager}
 */
getTextureManager : function (
)
{
    return gaf.GAFAssetTextureManager;
},

/**
 * @method setRootTimelineWithName
 * @param {String} arg0
 */
setRootTimelineWithName : function (
str 
)
{
},

/**
 * @method isAssetVersionPlayable
 * @param {char} arg0
 * @return {bool}
 */
isAssetVersionPlayable : function (
char 
)
{
    return false;
},

/**
 * @method desiredCsf
 * @return {float}
 */
desiredCsf : function (
)
{
    return 0;
},

/**
 * @method create
 * @param {String} arg0
 * @param {function} arg1
 * @return {gaf.GAFAsset}
 */
create : function (
str, 
func 
)
{
    return gaf.GAFAsset;
},

/**
 * @method setDesiredCsf
 * @param {float} arg0
 */
setDesiredCsf : function (
float 
)
{
},

/**
 * @method createWithBundle
 * @param {String} arg0
 * @param {String} arg1
 * @param {function} arg2
 * @return {gaf.GAFAsset}
 */
createWithBundle : function (
str, 
str, 
func 
)
{
    return gaf.GAFAsset;
},

/**
 * @method GAFAsset
 * @constructor
 */
GAFAsset : function (
)
{
},

};

/**
 * @class GAFSprite
 */
gaf.GAFSprite = {

/**
 * @method setLocator
 * @param {bool} arg0
 */
setLocator : function (
bool 
)
{
},

/**
 * @method GAFSprite
 * @constructor
 */
GAFSprite : function (
)
{
},

};

/**
 * @class GAFObject
 */
gaf.GAFObject = {

/**
 * @method setAnimationStartedNextLoopDelegate
 * @param {function} arg0
 */
setAnimationStartedNextLoopDelegate : function (
func 
)
{
},

/**
 * @method setAnimationFinishedPlayDelegate
 * @param {function} arg0
 */
setAnimationFinishedPlayDelegate : function (
func 
)
{
},

/**
 * @method setLooped
 * @param {bool} arg0
 */
setLooped : function (
bool 
)
{
},

/**
 * @method getBoundingBoxForCurrentFrame
 * @return {rect_object}
 */
getBoundingBoxForCurrentFrame : function (
)
{
    return cc.Rect;
},

/**
 * @method setFps
 * @param {unsigned int} arg0
 */
setFps : function (
int 
)
{
},

/**
 * @method getObjectByName
* @param {String|String} str
* @return {gaf.GAFObject|gaf.GAFObject}
*/
getObjectByName : function(
str 
)
{
    return gaf.GAFObject;
},

/**
 * @method clearSequence
 */
clearSequence : function (
)
{
},

/**
 * @method getIsAnimationRunning
 * @return {bool}
 */
getIsAnimationRunning : function (
)
{
    return false;
},

/**
 * @method gotoAndStop
* @param {unsigned int|String} int
* @return {bool|bool}
*/
gotoAndStop : function(
str 
)
{
    return false;
},

/**
 * @method getStartFrame
 * @param {String} arg0
 * @return {unsigned int}
 */
getStartFrame : function (
str 
)
{
    return 0;
},

/**
 * @method setFramePlayedDelegate
 * @param {function} arg0
 */
setFramePlayedDelegate : function (
func 
)
{
},

/**
 * @method getCurrentFrameIndex
 * @return {unsigned int}
 */
getCurrentFrameIndex : function (
)
{
    return 0;
},

/**
 * @method getTotalFrameCount
 * @return {unsigned int}
 */
getTotalFrameCount : function (
)
{
    return 0;
},

/**
 * @method start
 */
start : function (
)
{
},

/**
 * @method isVisibleInCurrentFrame
 * @return {bool}
 */
isVisibleInCurrentFrame : function (
)
{
    return false;
},

/**
 * @method isDone
 * @return {bool}
 */
isDone : function (
)
{
    return false;
},

/**
 * @method playSequence
 * @param {String} arg0
 * @param {bool} arg1
 * @param {bool} arg2
 * @return {bool}
 */
playSequence : function (
str, 
bool, 
bool 
)
{
    return false;
},

/**
 * @method stop
 */
stop : function (
)
{
},

/**
 * @method isReversed
 * @return {bool}
 */
isReversed : function (
)
{
    return false;
},

/**
 * @method setFrame
 * @param {unsigned int} arg0
 * @return {bool}
 */
setFrame : function (
int 
)
{
    return false;
},

/**
 * @method setControlDelegate
 * @param {function} arg0
 */
setControlDelegate : function (
func 
)
{
},

/**
 * @method getEndFrame
 * @param {String} arg0
 * @return {unsigned int}
 */
getEndFrame : function (
str 
)
{
    return 0;
},

/**
 * @method pauseAnimation
 */
pauseAnimation : function (
)
{
},

/**
 * @method gotoAndPlay
* @param {unsigned int|String} int
* @return {bool|bool}
*/
gotoAndPlay : function(
str 
)
{
    return false;
},

/**
 * @method isLooped
 * @return {bool}
 */
isLooped : function (
)
{
    return false;
},

/**
 * @method resumeAnimation
 */
resumeAnimation : function (
)
{
},

/**
 * @method setReversed
 * @param {bool} arg0
 */
setReversed : function (
bool 
)
{
},

/**
 * @method hasSequences
 * @return {bool}
 */
hasSequences : function (
)
{
    return false;
},

/**
 * @method getFps
 * @return {unsigned int}
 */
getFps : function (
)
{
    return 0;
},

/**
 * @method GAFObject
 * @constructor
 */
GAFObject : function (
)
{
},

};

/**
 * @class GAFAssetTextureManager
 */
gaf.GAFAssetTextureManager = {

/**
 * @method getMemoryConsumptionStat
 * @return {unsigned int}
 */
getMemoryConsumptionStat : function (
)
{
    return 0;
},

/**
 * @method GAFAssetTextureManager
 * @constructor
 */
GAFAssetTextureManager : function (
)
{
},

};

/**
 * @class GAFMovieClip
 */
gaf.GAFMovieClip = {

/**
 * @method GAFMovieClip
 * @constructor
 */
GAFMovieClip : function (
)
{
},

};
