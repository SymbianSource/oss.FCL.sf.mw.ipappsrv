/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Stores for stream and connection related data for
*                plug-in's internal use
*
*/




#ifndef NATFWTURNSTREAMDATA_H
#define NATFWTURNSTREAMDATA_H

class CSTUNRelayBinding;
class CNATFWCandidate;


/**
 *  Class for storing connection related data.
 *
 *  @lib turnplugin.lib
 *  @since S60 v3.2
 */
class TConnectionData
    {
    
public: // Enumerations

    enum TActDestState
        {
        EActDestNoneSet,
        EActDestSet,
        EActDestTransitioning
        };

public: // Constructors and destructor

    inline TConnectionData() :
        iConnectionId( 0 ),
        iTurnBinding( NULL ),
        iLocalCandidate( NULL ),
        iLocalAddr( KAFUnspec ),
        iServerAddr( KAFUnspec ),
        iPeerAddr( KInetAddrAny ),
        iActDestState( EActDestNoneSet ),
        iCurrentActDest( KAFUnspec ),
        iActDestReqResent( EFalse ),
        iTimerValue( 0 ),
        iSendingActivated( EFalse ),
        iReceivingActivated( EFalse ),
        iActDestAlreadySetOnServer( EFalse )
        {};

public: // Data

    /**
     * ID for used connection
     */
    TUint iConnectionId;

    /**
     * CSTUNBinding instance
     * Own.
     */
    CSTUNRelayBinding* iTurnBinding;

    /**
     * CNATFWCandidate instance
     * Not own.
     */
    CNATFWCandidate* iLocalCandidate;
    
    /**
     * Local end for TURN connection
     */
    TInetAddr iLocalAddr;
    
    /**
     * TURN-server address
     */
    TInetAddr iServerAddr;
    
    /**
     * Peer address
     */
    TInetAddr iPeerAddr;
    
    /**
     * Active Destination setting state
     */
    TConnectionData::TActDestState iActDestState;

    /**
     * Current active destination remote address
     */
    TInetAddr iCurrentActDest;
    
    /**
     * Flag defining that has Active Destination Request been resent
     */
    TBool iActDestReqResent;

    /**
     * TIMER-VAL attribute from TURN server
     */
    TUint32 iTimerValue;

    /**
     * Flag defining whether connection's sending is activated
     */
    TBool iSendingActivated;

    /**
     * Flag defining whether connection's receiving is activated
     */
    TBool iReceivingActivated;
    
    /**
     * Flag defining whether remote address from Active Destination
     * request is already set on TURN server.
     */
    TBool iActDestAlreadySetOnServer;

    };


/**
 *  Class for storing stream related data.
 *
 *  @lib turnplugin.lib
 *  @since S60 v3.2
 */
class TStreamData
    {

public: // Constructors and destructor

    inline TStreamData() :
        iStreamId( 0 ),
        iTransportProtocol( KProtocolInetUdp ),
        iRtoValue( 0 )
        {};

public: // Data

    /**
     * ID of used stream
     */
    TUint iStreamId;

    /**
     * Array for storing data for each connection.
     */
    RArray<TConnectionData> iConnArray;

    /**
     * Used transport protocol
     */
    TUint iTransportProtocol;

    /**
     * Retransmission timeout in milliseconds
     */
    TUint iRtoValue;

    };

#endif // NATFWTURNSTREAMDATA_H
