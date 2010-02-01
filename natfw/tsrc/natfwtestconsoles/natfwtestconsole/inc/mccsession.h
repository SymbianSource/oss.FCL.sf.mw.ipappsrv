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
* Description:    
*
*/




#ifndef __MCCSESSION_H__
#define __MCCSESSION_H__

#include <mmccctrlobserver.h>
#include <mmccnetworksettings.h>
#include "mnatfwsocketmediaconnwrapper.h"
#include "mccmediastream.h"

class CNATFWConnectivityApi;
class CMccCodecInformation;
class CMccInterface;
class CMccLink;

class CMccSession : public MMccCtrlObserver
{
public:

    static CMccSession* NewL( TUint aIapId );
    
    void CreateLinkL( TInt32& aLinkId, TInt aLinkType, TInetAddr& aDestAddr, 
        TRequestStatus& aStatus );
    
    void CreateLinkL( TInt32& aLinkId, TInt aLinkType, TInetAddr& aDestAddr,
        TUint aRtcpPort, TRequestStatus& aStatus );
  
    void CreateMediaStreamL( TInt32& aStreamId, TInt32 aLinkId, TStreamDirection aStreamDirection,
        TRequestStatus& aStatus );
    
    void StartStreamL( TInt32 aStreamId, TRequestStatus& aStatus );

    void SetRemoteAddressL( TInt32 aLinkId, TInetAddr& aDestAddr );
    
    void SetRemoteAddressL( TInt32 aLinkId, TInetAddr& aDestAddr, TUint aRtcpPort );
    
    void CloseLinkL( TInt32 aLinkId );
    
    void CloseMediaStreamL( TInt32 aStreamId );
    
    CMccInterface* MccInterface();

    TUint32 MccSessionId();
         
    TMccNetSettings& NetSettings();
    
    virtual ~CMccSession();
    
protected:
    void ConstructL();
     
    CMccSession( TUint aIapId ); 
    
    CMccMediaStream* StreamByIdL( TInt32 aStreamId );
    
    CMccLink* LinkByIdL( TInt32 aLinkId );
     
// MCC controller observer
    void MccEventReceived( const TMccEvent& aEvent );
 
    void MccMediaStarted( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );
                          
    void MccMediaStopped( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );

    void MccMediaPaused( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );

    void MccMediaResumed( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );

    void MccMediaPrepared( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );

    void MccMediaInactive( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );
    
    void MccMediaActive( TUint32 aSessionId, TUint32 aLinkId,
        TUint32 aStreamId, TUint32 aSinkSourceId );
    
    void MccCtrlError( TInt aError, TUint32 aSessionId,
        TUint32 aLinkId, TUint32 aStreamId, TUint32 aSinkSourceId );
                       
    void UnknownMediaReceived(  TUint32 aSessionId, TUint32 aLinkId, 
        TUint32 aStreamId, TUint32 aSinkSourceId, TUint8 aPayloadType );
       
private: // data
    TUint iIapId;
    TRequestStatus* iObserverStatus;
    TUint32 iMccSessionId;
    RPointerArray< CMccLink > iMccLinks;
    RPointerArray< CMccMediaStream > iMccStreams;
    CMccInterface* iMccInterface;
    TMccNetSettings iNetSettings;
};

#endif //__MEDIASTREAM_H__
