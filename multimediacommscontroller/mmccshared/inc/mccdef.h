/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    
*
*/




#ifndef MCCDEF_H
#define MCCDEF_H

//  INCLUDES
#include <e32cmn.h>
#include <mmf/common/mmffourcc.h>
#include "rtpdef.h"

// CONSTANTS
const TUid KMccUidInterface = {0x101F8CA6};
const TUid KMccControllerUidInterface = {0x101F8CA6};
const TInt KMaxBalance = 100;

const TUint8 KG711PcmUPT = 0; // PCMU
const TUint8 KG711PcmAPT = 8; // PCMA
const TUint8 KG729PT = 18;    // G.729
const TUint8 KComfortNoisePT = 13; // ComfortNoise
const TUint8 KOldComfortNoisePT = 19; // Old ComfortNoise PayloadType, Cisco seems to use this
const TUint8 KUnassignedPT = 20; // Unassigned payload type for dummy rtp packets
const TUint8 KDynamicPT = 96; // dynamic payload type 96-127
const TUint8 KPayloadDefined = 128;

const TInt  KMono(1);
const TInt  KStereo(2);

const TInt KMccDefaultVideoFrameRate = 15;
const TInt KMccDefaultVideoFrequency = 90000;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
*  The TMccMediaStreamId encapsulates unique identifier for a media stream.
*
*  @lib N/A
*  @since Series 60 3.0
*/
class TMccMediaStreamId
	{
	public:
	    inline TMccMediaStreamId()
	        :
	        iSessionId( -1 ),
	        iStreamId( -1 )
	        { }
	        
	    inline TMccMediaStreamId( TInt aSessionId, TInt aStreamId )
	        :
	        iSessionId( aSessionId ),
	        iStreamId( aStreamId )
	        { }

        inline TBool operator==( const TMccMediaStreamId& aStream ) const
            {
            return ( iSessionId == aStream.iSessionId && iStreamId == aStream.iStreamId );
            }
            
        inline TBool operator!=( const TMccMediaStreamId& aStream ) const
            {
            return ( iSessionId != aStream.iSessionId || iStreamId != aStream.iStreamId );
            }            
            
		TInt iSessionId;
		TInt iStreamId;
	};

#endif // MCCDEF_H
