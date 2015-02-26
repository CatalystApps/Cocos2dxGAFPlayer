#include "GAFPrecompiled.h"
#include "TagDefineAnimationFrames.h"

#include "GAFStream.h"
#include "GAFAsset.h"
#include "GAFFile.h"
#include "GAFHeader.h"

#include "PrimitiveDeserializer.h"

#include "GAFSubobjectState.h"
#include "GAFAnimationFrame.h"
#include "GAFFilterData.h"

NS_GAF_BEGIN

TagDefineAnimationFrames::~TagDefineAnimationFrames()
{
    for (States_t::iterator it = m_currentStates.begin(), ie = m_currentStates.end(); it != ie; ++it)
    {
        it->second->release();
    }
}

void TagDefineAnimationFrames::read(GAFStream* in, GAFAsset* asset, GAFTimeline* timeline)
{
    (void)asset;
    in->readU32(); // read count. Unused here

    if (timeline->getAnimationObjects().empty()) return;

    for (AnimationObjects_t::const_iterator i = timeline->getAnimationObjects().begin(), e = timeline->getAnimationObjects().end(); i != e; ++i)
    {
        unsigned int objectId = i->first;
        GAFSubobjectState *state = new GAFSubobjectState();
        state->initEmpty(objectId);

        m_currentStates[objectId] = state;
    }

    const unsigned short totalFrameCount = in->getInput()->getHeader().framesCount;

    unsigned int frameNumber = in->readU32();

    for (unsigned int i = 0; i < totalFrameCount; ++i)
    {
        if ((frameNumber - 1) == i)
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

            if (in->getPosition() < in->getTagExpectedPosition())
                frameNumber = in->readU32();
        }

        GAFAnimationFrame* frame = new GAFAnimationFrame();

        for (States_t::iterator it = m_currentStates.begin(), ie = m_currentStates.end(); it != ie; ++it)
        {
            frame->pushObjectState(it->second);
        }

        timeline->pushAnimationFrame(frame);
    }
}

GAFSubobjectState* TagDefineAnimationFrames::extractState(GAFStream* in)
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
