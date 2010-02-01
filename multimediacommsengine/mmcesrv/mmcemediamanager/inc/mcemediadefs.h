/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:    Includes SDP-bounded enumerations etc.
*
*/





#ifndef MCEMEDIADEFS_H
#define MCEMEDIADEFS_H

//  INCLUDES
#include <stringpool.h>
#include "mcesrv.h"

// CONSTANTS

_LIT8( KMceSdpDefaultSessionName, "-" );
_LIT8( KMceSdpDefaultUserName, "-" );
_LIT8( KMceSdpOptionalEncodingParam, "");
_LIT8( KMceSdpNullFormatList, "0");
_LIT8( KMceVideoFrameSizeFormat, "%u %d-%d" ); 
_LIT8( KMceVideoFrameSizeAttribute, "framesize" );

const TInt KMceSdpMaxMediaLineLength = 100;
const TInt KMceSdpAmrFmtpAttributesLength = 150;
const TInt KMceSdpFormatListLength = 30;
const TInt KMceSdpNumericAttributeLength = 11;

#define SDP_STRINGL( stringIndex )\
    SdpCodecStringPool::StringPoolL().\
    StringF( stringIndex, SdpCodecStringPool::StringTableL() )
#define MCE_AUDIO( stream )\
    static_cast<CMceComAudioStream&>( stream )
#define MCE_VIDEO( stream )\
    static_cast<CMceComVideoStream&>( stream )

#define MCE_DEFINE_DECSTR( attr, value )\
    TBuf8<KMceSdpNumericAttributeLength> attr;\
    attr.Num( value, EDecimal );
#define MCE_DEFINE_AUDIO( stream, mediaStream )\
    CMceComAudioStream& stream = MCE_AUDIO( mediaStream )
#define MCE_DEFINE_AUDIO_CODEC( codec, aCodec )\
    CMceComAudioCodec& codec = static_cast<CMceComAudioCodec&>( aCodec )
#define MCE_DEFINE_VIDEO( stream, mediaStream )\
    CMceComVideoStream& stream = MCE_VIDEO( mediaStream )
#define MCE_DEFINE_VIDEO_CODEC( codec, aCodec )\
    CMceComVideoCodec& codec = static_cast<CMceComVideoCodec&>( aCodec )
#define MCE_DEFINE_CODECS( codecs, stream )\
    RPointerArray< CMceComCodec >& codecs = reinterpret_cast< RPointerArray< CMceComCodec >& >( (stream).Codecs() )
#define MCE_ITERATOR_FIND_NEXT( iterator, item, condition )\
        do { iterator.Next( item ); } while ( item && ! (condition) )

typedef TUint TMceMMState;

const TMceMMState KMceMediaIdle = 1;
const TMceMMState KMceOfferingMedia = 2;
const TMceMMState KMceAnsweringMedia = 3;
const TMceMMState KMceAnsweringMediaUpdate = 4;
const TMceMMState KMceMediaNegotiated = 5;

const TMceMMState KMcePreparingOffererStreams = 6;
const TMceMMState KMceStartingOffererReceiveStreams = 7;
const TMceMMState KMceStartingOffererSendStreams = 8;
const TMceMMState KMcePreparingAnswererStreams = 9;
const TMceMMState KMceStartingAnswererStreams = 10;


enum TMceNegotiationRole
    {
    EMceRoleOfferer,
    EMceRoleAnswerer
    };


typedef TUint TMceNegotiationAnswerType;
const TMceNegotiationAnswerType KMceNegotiationAnswerTypeNotDefined = 0;
const TMceNegotiationAnswerType KMceNegotiationAnswerTypeIntermediate = 1;
const TMceNegotiationAnswerType KMceNegotiationAnswerTypeFinal = 2;


const TInt KMceNoSignalling = 0;
const TInt KMceRequiresSipSignallingOnly = 1;
const TInt KMceRequiresSignalling = 2;
    

const TUint KMceFastForwardMultiplier = 5;

const TUint KMceCodecSelectionOff = 0;
const TUint KMceCodecSelectionMerge = 1;

#endif      // MCEMEDIADEFS_H
            
// End of File
