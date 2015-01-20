/**
 * @module gaf
 */
var gaf = gaf || {};

/**
 * @class GAFAsset
 */
gaf.Asset = {

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
 * @method setRootTimeline
* @param {unsigned int|String} int
* @return {bool|bool}
*/
setRootTimeline : function(
str 
)
{
    return false;
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
 * @param {bool} arg0
 * @return {gaf::GAFObject}
 */
createObjectAndRun : function (
bool 
)
{
    return gaf::GAFObject;
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
 * @method createObject
 * @return {gaf::GAFObject}
 */
createObject : function (
)
{
    return gaf::GAFObject;
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
 * @method create
* @param {String|String} str
* @param {function} func
* @return {gaf::GAFAsset|gaf::GAFAsset}
*/
create : function(
str,
func 
)
{
    return gaf::GAFAsset;
},

/**
 * @method createWithBundle
 * @param {String} arg0
 * @param {String} arg1
 * @param {function} arg2
 * @return {gaf::GAFAsset}
 */
createWithBundle : function (
str, 
str, 
func 
)
{
    return gaf::GAFAsset;
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
gaf.Sprite = {

};

/**
 * @class GAFObject
 */
gaf.Object = {

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
 * @param {bool} arg1
 */
setLooped : function (
bool, 
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
* @return {gaf::GAFObject|gaf::GAFObject}
*/
getObjectByName : function(
str 
)
{
    return gaf::GAFObject;
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
 * @method init
 * @param {gaf::GAFAsset} arg0
 * @param {gaf::GAFTimeline} arg1
 * @return {bool}
 */
init : function (
gafasset, 
gaftimeline 
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
 * @method setAnimationRunning
 * @param {bool} arg0
 * @param {bool} arg1
 */
setAnimationRunning : function (
bool, 
bool 
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
 * @method create
 * @param {gaf::GAFAsset} arg0
 * @param {gaf::GAFTimeline} arg1
 * @return {gaf::GAFObject}
 */
create : function (
gafasset, 
gaftimeline 
)
{
    return gaf::GAFObject;
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
