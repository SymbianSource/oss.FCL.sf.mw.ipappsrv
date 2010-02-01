/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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





#ifndef MCCINTERNALEVENTS_H
#define MCCINTERNALEVENTS_H

//  INCLUDES
#include <e32std.h>
#include <mmf/common/mmfcontrollerframeworkbase.h>
#include "mmccevents.h"

// CONSTANTS

// To avoid the event type overlapping values defined in
// interface, start from 20
const TMccEventCategory KMccEventCategoryStreamControl = 20;

// To avoid the event type overlapping values defined in
// interface, start from 100
const TMccEventType KMccStandbyInactivityEvent = 100;
const TMccEventType KMccStandbyActivityEvent = 101;

const TUint32 KMccAutomaticEvent = 300;
const TUint32 KMccPayloadSpecificEvent = 301;

// MACROS

// DATA TYPES
enum TMccInternalEventType
    {
    EMccInternalEventNone = 200, // Avoid clash with TMccEventType values
    EMccInternalJitterEventUndefined,
    EMccInternalJitterEventStatusReport,
    EMccInternalJitterEventMmfEvent,
    EMccInternalRtpSinkError,
    EMccInternalRtpSinkLastBuffer,
    EMccInternalRtpSrcJitterUpdate,
    KMccInternalRtpSrcMmfEvent,
    EMccInternalAmrEventUndefined,
    EMccInternalAmrEventCmr
    };
    
    
// Disabling PC-lint warning 1554, for getting rid of it otherwise
// might not be possible
/*lint -e1554*/    
    
/**
*  AMR codec mode request ( narrow band and wide band).
*/
enum TAmrModeRequest
    {
    EAmrModeReq0 = 0,
    EAmrModeReq1 = 1,
    EAmrModeReq2 = 2,
    EAmrModeReq3 = 3,
    EAmrModeReq4 = 4,
    EAmrModeReq5 = 5,
    EAmrModeReq6 = 6,
    EAmrModeReq7 = 7,      // Narrowband max
    EAmrModeReq8 = 8,      // Wideband max
    EAmrModeReqNone = 15   // No mode request
    };
    
// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class TMccEvent;

// CLASS DECLARATION

class TMccInternalEvent : public TMMFEvent
    {
    public:
    
        /**
        * Default constructor
        */
        inline TMccInternalEvent() : 
            TMMFEvent( KNullUid, KErrNone ), 
            iInternalEventType( EMccInternalEventNone ), 
            iMccEvent( NULL )
            {
            }
            
        /**
        * Parametrized constructor
        */
        inline TMccInternalEvent( TUid aEventOriginator, 
                                  TMccInternalEventType aInternalEventType, 
                                  TMccEvent& aMccEvent ) : 
            TMMFEvent( aEventOriginator, KErrNone ),
            iInternalEventType( aInternalEventType ),
            iMccEvent( &aMccEvent )
            {
            }
            
        /**
        * Copy constructor
        */
        inline TMccInternalEvent( const TMccInternalEvent& aEvent  ) : 
            TMMFEvent( aEvent.iEventType, aEvent.iErrorCode ),
            iInternalEventType( aEvent.iInternalEventType ),
            iMccEvent( aEvent.iMccEvent )
            {
            }
    
    public: // Data
    
        TMccInternalEventType iInternalEventType;
        
        TMccEvent* iMccEvent;

    };

/**
* Jitter buffer event data
*/
class TMccJitterBufferEventData
    {
    public:
   
        /**
        * Default constructor
        */
        inline TMccJitterBufferEventData() : 
            iFramesPlayed( 0 ), iFramesReceived( 0 ), iFrameLoss( 0 ), iFramesRemoved( 0 ),
            iFramesInBuffer( 0 ), iBufferLength( 0 ), iLateFrames( 0 )
            {   
            }
            
        /**
        * Copy constructor
        */
        inline TMccJitterBufferEventData( const TMccJitterBufferEventData& aEvent ) :
            iFramesPlayed( aEvent.iFramesPlayed ),
            iFramesReceived( aEvent.iFramesReceived ), 
            iFrameLoss( aEvent.iFrameLoss ), 
            iFramesRemoved( aEvent.iFramesRemoved ), 
            iFramesInBuffer( aEvent.iFramesInBuffer ), 
            iBufferLength( aEvent.iBufferLength ), 
            iLateFrames( aEvent.iLateFrames )
            {   
            }
        
    public: // Data

        // Frames played
        TUint iFramesPlayed;
        // Frames received
        TUint iFramesReceived;
        // Frame loss
        TUint iFrameLoss;
        // Frames removed by adaptation control
        TUint iFramesRemoved;
        // Current frames in buffer
        TUint iFramesInBuffer;
        // Current buffer length
        TUint iBufferLength;
        // Frames late but not too late
        TUint iLateFrames;
    };
    
typedef TPckgBuf<TMccJitterBufferEventData> TMccJitterBufferEventDataPackage;

/**
* AMR payload formatter event data
*/
class TMccAmrEventData
    {
    public:
    
        /**
        * Default constructor
        */
        inline TMccAmrEventData() : 
            iModeRequestBitrate( 0 )
            {  
            }

        /**
        * Copy constructor
        */
        inline TMccAmrEventData( const TMccAmrEventData& aEvent ) : 
            iModeRequestBitrate( aEvent.iModeRequestBitrate )
            {   
            }

    public: // Data

        // AMR mode request bitrate
        TInt iModeRequestBitrate;
    };
    
typedef TPckgBuf<TMccAmrEventData> TMccAmrEventDataPackage;

/**
* Rate adaptation event data
*/
class TMccRateAdaptationEventData
    {
    public:
    
        /**
        * Default constructor
        */
        inline TMccRateAdaptationEventData() : 
            iRateAdaptationAdvice( 1 ),
            iFramerateOriginal( 0 ),
            iFramerateModified( 0 ),
            iBitrateOriginal( 0 ),
            iBitrateModified( 0 )
            {  
            }

        /**
        * Copy constructor
        */
        inline TMccRateAdaptationEventData( 
            const TMccRateAdaptationEventData& aEvent ) : 
            iRateAdaptationAdvice( aEvent.iRateAdaptationAdvice ),
            iFramerateOriginal( aEvent.iFramerateModified ),
            iFramerateModified( aEvent.iFramerateModified ),
            iBitrateOriginal( aEvent.iBitrateOriginal ),
            iBitrateModified( aEvent.iBitrateModified )
            {   
            }

    public: // Data

        // Rate adaptation amount (0.0 ... 1.0 scale)
        TReal iRateAdaptationAdvice;
        
        TReal iFramerateOriginal;
        
        TReal iFramerateModified;
        
        TUint iBitrateOriginal;
        
        TUint iBitrateModified;
    };
    
typedef TPckgBuf<TMccRateAdaptationEventData> TMccRateAdaptationEventDataPackage;
      
#endif      // MCCINTERNALEVENTS_H   
            
// End of File
