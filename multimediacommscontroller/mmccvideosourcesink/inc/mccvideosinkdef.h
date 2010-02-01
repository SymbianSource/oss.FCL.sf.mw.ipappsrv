/*
* Copyright (c) 2002-2003 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef MCCDISPLAYSINKDEF_H
#define MCCDISPLAYSINKDEF_H

#include "rtpdef.h"

// CONSTANTS
_LIT8(KSDP,"Sdp");
_LIT8(KLIVESTREAM,"LiveStream");

/**
*  The audio/video pair
*
*  @lib 
*  @since 
*/
class TAVPorts
    {
    public:
        TUint iAudioPort;
        TUint iVideoPort;
    };

/**
*  The parameters needed for local setup
*
*  @lib 
*  @since 
*/
class TLocalParameters
    {
    public:
        TAVPorts iPorts;
        TRtpSdesParams iSdes;
        TRtcpParams iRtcpAudioParams;
        TRtcpParams iRtcpVideoParams;
        TRtpPayloadType iAudioPayloadType;
        TRtpPayloadType iVideoPayloadType; 
    };

/**
*  The parameters needed for remote setup and swis communication.
*
*  @lib 
*  @since 
*/
class TRemoteParameters
    {
    public:
        TInetAddr iRemoteAddrAudio;
        TInetAddr iRemoteAddrVideo;
        TPtrC8 iSdpText;
        TTimeIntervalMicroSeconds32 iPausedTimeOut;
        TTimeIntervalMicroSeconds32 iDisconnectedTimeOut;
    };


#endif // MCCDISPLAYSINKDEF_H
