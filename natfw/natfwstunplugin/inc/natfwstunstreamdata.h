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




#ifndef NATFWSTUNSTREAMDATA_H
#define NATFWSTUNSTREAMDATA_H

#include <e32def.h>
#include <in_sock.h>

class CSTUNBinding;
class CNATFWCandidate;


/**
 *  Class for storing connection related data.
 *
 *  @lib stunplugin.lib
 *  @since S60 v3.2
 */
class TConnectionData
    {

public: // Constructors and destructor

    inline TConnectionData() :
        iConnectionId( 0 ),
        iStunBinding( NULL ),
        iLocalCandidate( NULL ),
        iDestAddr( KAFUnspec ),
        iStunSrvAddr( KAFUnspec ),
        iSendingActivated( EFalse ),
        iMediaSendingActivated( EFalse ),
        iReceivingActivated( EFalse )
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
    CSTUNBinding* iStunBinding;

    /**
     * CNATFWCandidate instance
     * Own.
     */
    CNATFWCandidate* iLocalCandidate;

    /**
     * Destination address
     */
    TInetAddr iDestAddr;

    /**
     * STUN server address
     */
    TInetAddr iStunSrvAddr;

    /**
     * Flag defining whether connection's sending is activated
     */
    TBool iSendingActivated;

    /**
     * Flag defining whether connection's media sending is activated.
     */
    TBool iMediaSendingActivated;

    /**
     * Flag defining whether connection's receiving is activated
     */
    TBool iReceivingActivated;
    };


/**
 *  Class for storing stream related data.
 *
 *  @lib stunplugin.lib
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
     * Retransmission timeout for STUN transactions
     */
    TUint iRtoValue;
    };

#endif // NATFWSTUNSTREAMDATA_H
