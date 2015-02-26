#include "GAFPrecompiled.h"
#include "TagDefineAnimationFrames2.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFFile.h"
#include "GAFHeader.h"
#include "GAFTimeline.h"

#include "PrimitiveDeserializer.h"

#include "GAFSubobjectState.h"
#include "GAFAnimationFrame.h"
#include "GAFFilterData.h"

NS_GAF_BEGIN

TagDefineAnimationFrames2::~TagDefineAnimationFrames2()
{
    for (States_t::iterator it = m_currentStates.begin(), ie = m_currentStates.end(); it != ie; ++it)
    {
        it->second->release();
    }
}

void TagDefineAnimationFrames2::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)asset;
    unsigned int count = in->readU32();

    //assert(!timeline->getAnimationObjects().empty());

    for (AnimationObjects_t::const_iterator i = timeline->getAnimationObjects().begin(), e = timeline->getAnimationObjects().end(); i != e; ++i)
    {
        unsigned int objectId = i->first;
        GAFSubobjectState *state = new GAFSubobjectState();
        state->initEmpty(objectId);

        m_currentStates[objectId] = state;
    }

    unsigned int frameNumber = in->readU32();

    for (unsigned int i = 0; i < count; ++i)
    {
        char hasChangesInDisplayList = in->readUByte();
        char hasActions = in->readUByte();

        if (hasChangesInDisplayList)
        {
            unsigned int numObjects = in->readU32();

            typedef std::list<GAFSubobjectState*> StatesList_t;
            StatesList_t statesList;

            for (unsigned int j = 0; j < numObjects; ++j)
            {
                GAFSubobjectState* state = extractState(in);

                statesList.push_back(state);
            }

            for (StatesList_t::iterator it = statesList.begin(), ie = statesList.end(); it != ie; ++it)
            {
                GAFSubobjectState* st = *it;

                GAFSubobjectState* ps = m_currentStates[st->objectIdRef];

                if (ps)
                {
                    ps->release();
                }

                m_currentStates[st->objectIdRef] = st;
            }
        }

        GAFAnimationFrame* frame = new GAFAnimationFrame();

        for (States_t::iterator it = m_currentStates.begin(), ie = m_currentStates.end(); it != ie; ++it)
        {
            frame->pushObjectState(it->second);
        }

        if (hasActions)
        {   
            uint32_t actionsCount = in->readU32();
            for (uint32_t actionIdx = 0; actionIdx < actionsCount; actionIdx++)
            {
                GAFTimelineAction action;

                GAFActionType type = static_cast<GAFActionType>(in->readU32());
                std::string scope;
                in->readString(&scope);

                std::vector<std::string> params;

                unsigned int paramsLength = in->readU32();
                unsigned int startPosition = in->getPosition();
                while (paramsLength > in->getPosition() - startPosition)
                {
                    std::string paramValue;
                    in->readString(&paramValue);
                    params.push_back(paramValue);
                }

                action.setAction(type, params, scope);
                frame->pushTimelineAction(action);
            }
        }

        if (in->getPosition() < in->getTagExpectedPosition())
            frameNumber = in->readU32();

        timeline->pushAnimationFrame(frame);
    }

    for (States_t::iterator it = m_currentStates.begin(), ie = m_currentStates.end(); it != ie; ++it)
    {
        it->second->release();
    }
    m_currentStates.clear();
}

GAFSubobjectState* TagDefineAnimationFrames2::extractState(GAFStream* in)
{
    GAFSubobjectState* state = new GAFSubobjectState();

    float ctx[7];

    char hasColorTransform = in->readUByte();
    char hasMasks = in->readUByte();
    char hasEffect = in->readUByte();

    state->objectIdRef = in->readU32();
    state->zIndex = in->readS32();
    state->colorMults()[GAFCTI_A] = in->readFloat();

    PrimitiveDeserializer::deserialize(in, &state->affineTransform);

    if (hasColorTransform)
    {
        in->readNBytesOfT(ctx, sizeof(float)* 7);

        float* ctxOff = state->colorOffsets();
        float* ctxMul = state->colorMults();

        ctxOff[GAFCTI_A] = ctx[0];

        ctxMul[GAFCTI_R] = ctx[1];
        ctxOff[GAFCTI_R] = ctx[2];

        ctxMul[GAFCTI_G] = ctx[3];
        ctxOff[GAFCTI_G] = ctx[4];

        ctxMul[GAFCTI_B] = ctx[5];
        ctxOff[GAFCTI_B] = ctx[6];
    }
    else
    {
        state->ctxMakeIdentity();
    }

    if (hasEffect)
    {
        unsigned char effects = in->readUByte();

        for (unsigned int e = 0; e < effects; ++e)
        {
            GAFFilterType type = static_cast<GAFFilterType>(in->readU32());

            if (type == GAFFilterType::GFT_Blur)
            {
                cocos2d::Size p;
                PrimitiveDeserializer::deserialize(in, &p);
                GAFBlurFilterData* blurFilter = new GAFBlurFilterData();
                blurFilter->blurSize = p;
                state->pushFilter(blurFilter);
            }
            else if (type == GAFFilterType::GFT_ColorMatrix)
            {
                GAFColorColorMatrixFilterData* colorFilter = new GAFColorColorMatrixFilterData();
                for (unsigned int i = 0; i < 4; ++i)
                {
                    for (unsigned int j = 0; j < 4; ++j)
                    {
                        colorFilter->matrix[j * 4 + i] = in->readFloat();
                    }

                    colorFilter->matrix2[i] = in->readFloat() / 255.f;
                }

                state->pushFilter(colorFilter);
            }
            else if (type == GAFFilterType::GFT_Glow)
            {
                GAFGlowFilterData* filter = new GAFGlowFilterData();
                unsigned int clr = in->readU32();

                PrimitiveDeserializer::translateColor(filter->color, clr);

                PrimitiveDeserializer::deserialize(in, &filter->blurSize);

                filter->strength = in->readFloat();
                filter->innerGlow = in->readUByte() ? true : false;
                filter->knockout = in->readUByte() ? true : false;

                state->pushFilter(filter);
            }
            else if (type == GAFFilterType::GFT_DropShadow)
            {
                GAFDropShadowFilterData* filter = new GAFDropShadowFilterData();

                unsigned int clr = in->readU32();

                PrimitiveDeserializer::translateColor(filter->color, clr);

                PrimitiveDeserializer::deserialize(in, &filter->blurSize);
                filter->angle = in->readFloat();
                filter->distance = in->readFloat();
                filter->strength = in->readFloat();
                filter->innerShadow = in->readUByte() ? true : false;
                filter->knockout = in->readUByte() ? true : false;

                state->pushFilter(filter);
            }
        }
    }

    if (hasMasks)
    {
        state->maskObjectIdRef = in->readU32();
    }

    return state;
}

NS_GAF_END
