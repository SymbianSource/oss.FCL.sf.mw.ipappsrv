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
* Description:    Observer class for events.
*
*/




#ifndef MMCCCTRLOBSERVER_H
#define MMCCCTRLOBSERVER_H

// INCLUDES
#include <e32def.h>
#include <mmf/common/mmfbase.h>
#include <mmccevents.h>

/**
*  Observer interface for clients to receive events from MCC.
*
*  @lib mmccinterface.lib
*/
class MMccCtrlObserver
    {
public:
    
    /**
    * Callback function to receive media events
    * @param aSignal Received media events
    * @return void
    */
    virtual void MccEventReceived( const TMccEvent& aEvent ) = 0;
    
    /**
    * Callback function to receive media started event.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaStarted( TUint32 aSessionId,
                                  TUint32 aLinkId,     
                                  TUint32 aStreamId,
                                  TUint32 aSinkSourceId ) = 0;
    
    /**
    * Callback function to receive media stopped event.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaStopped( TUint32 aSessionId,
                                  TUint32 aLinkId,
                                  TUint32 aStreamId,
                                  TUint32 aSinkSourceId ) = 0;

    /**
    * Callback function to receive media paused event.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaPaused( TUint32 aSessionId,
                                 TUint32 aLinkId,
                                 TUint32 aStreamId,
                                 TUint32 aSinkSourceId ) = 0;

    /**
    * Callback function to receive media resumed event.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaResumed( TUint32 aSessionId,
                                  TUint32 aLinkId,
                                  TUint32 aStreamId,
                                  TUint32 aSinkSourceId ) = 0;

    /**
    * Callback function to receive media prepared event.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaPrepared( TUint32 aSessionId,
                                   TUint32 aLinkId,
                                   TUint32 aStreamId,
                                   TUint32 aSinkSourceId ) = 0;

    /**
    * Callback function to receive media inactivity event.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaInactive( TUint32 aSessionId,
                                   TUint32 aLinkId,
                                   TUint32 aStreamId,
                                   TUint32 aSinkSourceId ) = 0;

    /**
    * Callback function to receive media activity event. The
    * function is called when the first RTP packet is received
    * by a downlink stream.
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aStreamId Stream ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccMediaActive( TUint32 aSessionId,
                                 TUint32 aLinkId,
                                 TUint32 aStreamId,
                                 TUint32 aSinkSourceId ) = 0;

    
    /**
    * Callback function to receive error event.
    * @param aError Error code
    * @param aSessionId Session ID
    * @param aLinkId Link ID
    * @param aSinkSourceId Sink or source ID
    */
    virtual void MccCtrlError( TInt aError,
                               TUint32 aSessionId,
                               TUint32 aLinkId,
                               TUint32 aStreamId,
                               TUint32 aSinkSourceId ) = 0;
    
    
    /**
     * Callback function to notify client of unknown media packet.
     * Called when packet with unknown payload type is 
     * received by a downlink stream.
     *
     * @since Series 60 3.0
     * @param aSessionId Session ID
     * @param aLinkId Link ID
     * @param aStreamId Stream ID
     * @param aSinkSourceId Sink or source ID
     * @param aPayloadType Payload type of received packet
     * @return void
     */
    inline virtual void UnknownMediaReceived( TUint32 /*aSessionId*/,
                                       TUint32 /*aLinkId*/,
                                       TUint32 /*aStreamId*/,
                                       TUint32 /*aSinkSourceId*/,
                                       TUint8 /*aPayloadType*/ ){};                           
};
    
#endif    // MMCCCTRLOBSERVER_H
