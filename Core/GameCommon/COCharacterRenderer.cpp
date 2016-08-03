/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.3
 * 
 * Copyright (c) 2013-2015, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to use, install, execute and perform the Spine
 * Runtimes Software (the "Software") and derivative works solely for personal
 * or internal use. Without the written permission of Esoteric Software (see
 * Section 2 of the Spine Software License Agreement), you may not (a) modify,
 * translate, adapt or otherwise create derivative works, improvements of the
 * Software or develop new applications using the Software or (b) remove,
 * delete, alter or obscure any trademarks or any copyright, trademark, patent
 * or other intellectual property or proprietary rights notices on or in the
 * Software, including any copy thereof. Redistributions in binary or source
 * form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <COCharacterRenderer.h>
#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>
#include <algorithm>

USING_NS_CC;
using std::min;
using std::max;
using std::vector;

//void animationCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
//	((COCharacterRenderer*)state->rendererObject)->onAnimationStateEvent(trackIndex, type, event, loopCount);
//}
//
//void trackEntryCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
//	((COCharacterRenderer*)state->rendererObject)->onTrackEntryEvent(trackIndex, type, event, loopCount);
//}
//
//typedef struct _TrackEntryListeners {
//	StartListener startListener;
//	EndListener endListener;
//	CompleteListener completeListener;
//	EventListener eventListener;
//} _TrackEntryListeners;
//
//static _TrackEntryListeners* getListeners (spTrackEntry* entry) {
//	if (!entry->rendererObject) {
//		entry->rendererObject = NEW(spine::_TrackEntryListeners);
//		entry->listener = trackEntryCallback;
//	}
//	return (_TrackEntryListeners*)entry->rendererObject;
//}
//
//void disposeTrackEntry (spTrackEntry* entry) {
//	if (entry->rendererObject) FREE(entry->rendererObject);
//	_spTrackEntry_dispose(entry);
//}

//

COCharacterRenderer* COCharacterRenderer::createWithData (spSkeletonData* skeletonData, bool ownsSkeletonData) {
	COCharacterRenderer* node = new COCharacterRenderer(skeletonData, ownsSkeletonData);
	node->autorelease();
	return node;
}

COCharacterRenderer* COCharacterRenderer::createWithFile (const std::string& skeletonDataFile, spAtlas* atlas, float scale) {
	COCharacterRenderer* node = new COCharacterRenderer(skeletonDataFile, atlas, scale);
	node->autorelease();
	return node;
}

COCharacterRenderer* COCharacterRenderer::createWithFile (const std::string& skeletonDataFile, const std::string& atlasFile, float scale) {
	COCharacterRenderer* node = new COCharacterRenderer(skeletonDataFile, atlasFile, scale);
	node->autorelease();
	return node;
}

void COCharacterRenderer::initialize () {
//	_ownsAnimationStateData = true;
//	_state = spAnimationState_create(spAnimationStateData_create(_skeleton->data));
//	_state->rendererObject = this;
//	_state->listener = animationCallback;

//	_spAnimationState* stateInternal = (_spAnimationState*)_state;
//	stateInternal->disposeTrackEntry = disposeTrackEntry;
    
//    this->setDebugBonesEnabled(true); //!!!!!
}

COCharacterRenderer::COCharacterRenderer ()
		: SkeletonRenderer() {
}

COCharacterRenderer::COCharacterRenderer (spSkeletonData *skeletonData, bool ownsSkeletonData)
		: SkeletonRenderer(skeletonData, ownsSkeletonData) {
	initialize();
}

COCharacterRenderer::COCharacterRenderer (const std::string& skeletonDataFile, spAtlas* atlas, float scale)
		: SkeletonRenderer(skeletonDataFile, atlas, scale) {
	initialize();
}

COCharacterRenderer::COCharacterRenderer (const std::string& skeletonDataFile, const std::string& atlasFile, float scale)
		: SkeletonRenderer(skeletonDataFile, atlasFile, scale) {
	initialize();
}

COCharacterRenderer::~COCharacterRenderer () {
//	if (_ownsAnimationStateData) spAnimationStateData_dispose(_state->data);
//	spAnimationState_dispose(_state);
}

void COCharacterRenderer::update (float deltaTime) {
	super::update(deltaTime);

	deltaTime *= _timeScale;
//	spAnimationState_update(_state, deltaTime);
//	spAnimationState_apply(_state, _skeleton);
	spSkeleton_updateWorldTransform(_skeleton);
}

//void COCharacterRenderer::setAnimationStateData (spAnimationStateData* stateData) {
//	CCASSERT(stateData, "stateData cannot be null.");
//
//	if (_ownsAnimationStateData) spAnimationStateData_dispose(_state->data);
//	spAnimationState_dispose(_state);
//
//	_ownsAnimationStateData = false;
//	_state = spAnimationState_create(stateData);
//	_state->rendererObject = this;
////	_state->listener = animationCallback;
//}
//
//void COCharacterRenderer::setMix (const std::string& fromAnimation, const std::string& toAnimation, float duration) {
//	spAnimationStateData_setMixByName(_state->data, fromAnimation.c_str(), toAnimation.c_str(), duration);
//}
//
//spTrackEntry* COCharacterRenderer::setAnimation (int trackIndex, const std::string& name, bool loop) {
//	spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name.c_str());
//	if (!animation) {
//		log("Spine: Animation not found: %s", name.c_str());
//		return 0;
//	}
//	return spAnimationState_setAnimation(_state, trackIndex, animation, loop);
//}
//
//spTrackEntry* COCharacterRenderer::addAnimation (int trackIndex, const std::string& name, bool loop, float delay) {
//	spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name.c_str());
//	if (!animation) {
//		log("Spine: Animation not found: %s", name.c_str());
//		return 0;
//	}
//	return spAnimationState_addAnimation(_state, trackIndex, animation, loop, delay);
//}
//
//spTrackEntry* COCharacterRenderer::getCurrent (int trackIndex) {
//	return spAnimationState_getCurrent(_state, trackIndex);
//}
//
//void COCharacterRenderer::clearTracks () {
//	spAnimationState_clearTracks(_state);
//}
//
//void COCharacterRenderer::clearTrack (int trackIndex) {
//	spAnimationState_clearTrack(_state, trackIndex);
//}
//
//void COCharacterRenderer::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
//	switch (type) {
//	case SP_ANIMATION_START:
//		if (_startListener) _startListener(trackIndex);
//		break;
//	case SP_ANIMATION_END:
//		if (_endListener) _endListener(trackIndex);
//		break;
//	case SP_ANIMATION_COMPLETE:
//		if (_completeListener) _completeListener(trackIndex, loopCount);
//		break;
//	case SP_ANIMATION_EVENT:
//		if (_eventListener) _eventListener(trackIndex, event);
//		break;
//	}
//}

//void COCharacterRenderer::onTrackEntryEvent (int trackIndex, spEventType type, spEvent* event, int loopCount) {
//	spTrackEntry* entry = spAnimationState_getCurrent(_state, trackIndex);
//	if (!entry->rendererObject) return;
//	_TrackEntryListeners* listeners = (_TrackEntryListeners*)entry->rendererObject;
//	switch (type) {
//	case SP_ANIMATION_START:
//		if (listeners->startListener) listeners->startListener(trackIndex);
//		break;
//	case SP_ANIMATION_END:
//		if (listeners->endListener) listeners->endListener(trackIndex);
//		break;
//	case SP_ANIMATION_COMPLETE:
//		if (listeners->completeListener) listeners->completeListener(trackIndex, loopCount);
//		break;
//	case SP_ANIMATION_EVENT:
//		if (listeners->eventListener) listeners->eventListener(trackIndex, event);
//		break;
//	}
//}
//
//void COCharacterRenderer::setStartListener (const StartListener& listener) {
//	_startListener = listener;
//}
//
//void COCharacterRenderer::setEndListener (const EndListener& listener) {
//	_endListener = listener;
//}
//
//void COCharacterRenderer::setCompleteListener (const CompleteListener& listener) {
//	_completeListener = listener;
//}
//
//void COCharacterRenderer::setEventListener (const EventListener& listener) {
//	_eventListener = listener;
//}
//
//void COCharacterRenderer::setTrackStartListener (spTrackEntry* entry, const StartListener& listener) {
//	getListeners(entry)->startListener = listener;
//}
//
//void COCharacterRenderer::setTrackEndListener (spTrackEntry* entry, const EndListener& listener) {
//	getListeners(entry)->endListener = listener;
//}
//
//void COCharacterRenderer::setTrackCompleteListener (spTrackEntry* entry, const CompleteListener& listener) {
//	getListeners(entry)->completeListener = listener;
//}
//
//void COCharacterRenderer::setTrackEventListener (spTrackEntry* entry, const EventListener& listener) {
//	getListeners(entry)->eventListener = listener;
//}
//
//spAnimationState* COCharacterRenderer::getState() const {
//	return _state;
//}

const float horizontalSensivity = 1.f;
const float verticalSensivity = 1.f;

const std::string rightShoulderBoneName = "right shoulder";
const std::string torsoBoneName = "torso";
const std::string leftUpperLegBoneName = "left upper leg";
const std::string rightUpperLegBoneName = "right upper leg";

void COCharacterRenderer::onTouchMoved(Touch* touch)
{
    auto torsoBone = this->findBone(torsoBoneName);
    torsoBone->rotation += touch->getDelta().x * horizontalSensivity;
    
    auto leftUpperLegBone = this->findBone(leftUpperLegBoneName);
    leftUpperLegBone->rotation += touch->getDelta().y * verticalSensivity;
    
    auto rightUpperLegBone = this->findBone(rightUpperLegBoneName);
    rightUpperLegBone->rotation += touch->getDelta().y * verticalSensivity;
//    rootBone->x += touches[0]->getDelta().x * horizontalSensivity;
//    rootBone->y += touches[0]->getDelta().y * verticalSensivity;
}