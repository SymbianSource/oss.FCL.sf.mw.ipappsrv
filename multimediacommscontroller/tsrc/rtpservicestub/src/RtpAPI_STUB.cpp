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




// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>
#include "rtpapi.h"

/*****************************************************************************/ 
/****************************** API FUNCTIONS   ******************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CRtpAPI::CRtpAPI( void )
    {
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CRtpAPI::ConstructL( MRtpErrNotify& /*aErrNotify*/ )
    {
    //iManager = CRtpManager::NewL( aErrNotify );
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CRtpAPI* CRtpAPI::NewL( MRtpErrNotify& aErrNotify )
    {
    CRtpAPI* self = new ( ELeave ) CRtpAPI;

    CleanupStack::PushL( self );
    self->ConstructL( aErrNotify );   
    CleanupStack::Pop();  //self

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CRtpAPI::~CRtpAPI( void )
    {
    //delete iManager;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::OpenL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::OpenL( const TRtpSdesParams& /*aSdesInfo*/,
                              const TDesC* /*aRtpPacketDll*/,
                              const RSocketServ* /*aSocketServPtr*/,
                              const RConnection* /*aConnPtr*/ )
    {
    /**
    RSocketServ* socketServPtr = const_cast<RSocketServ*>( aSocketServPtr );
	RConnection* connPtr       = const_cast<RConnection*>( aConnPtr );
	
    return iManager->OpenL( aSdesInfo, aRtpPacketDll, socketServPtr, connPtr );
     */
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::StartConnection()
// Synchronous version.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::StartConnection( TInt /*aIapId*/ )
    {
    // return iManager->StartConnection( aIapId );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::StartConnection()
// Asynchronous version.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::StartConnection( TRequestStatus& aStatus, TInt /*aIapId*/ )
    {
    //
    // return iManager->StartConnection( aStatus, aIapId );
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::CancelStart()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::CancelStart()
    {
    //iManager->CancelStart();
    }

// ---------------------------------------------------------------------------
// CRtpAPI::Close()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::Close( void )
    {
    //iManager->Close();
    }

// ---------------------------------------------------------------------------
// CRtpAPI::SetLocalSdes()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::SetLocalSdes( const TRtpSdesParams& /*aSdesInfo*/ )
    {
    //iManager->SetLocalSdes( aSdesInfo );
    }

// ---------------------------------------------------------------------------
// CRtpAPI::Version()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TVersion CRtpAPI::Version() const
    {
    return TVersion(2, 0, 0);
    }

// ---------------------------------------------------------------------------
// TInetAddr& CRtpAPI::GetLocalIPAddressL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInetAddr& CRtpAPI::GetLocalIPAddressL()
    {
    TInetAddr adressi;
    adressi.SetAddress( INET_ADDR( 127,0,0,1 ) );
    return adressi;
    // return iManager->GetLocalIPAddressL();
    }
    
 

/*****************************************************************************/ 
/***************************** SESSION FUNCTIONS  ****************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// TRtpId CRtpAPI::CreateSessionL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TRtpId CRtpAPI::CreateSessionL( const TCreateSessionParams& aSessionParams,
                                         TUint& /*aPort*/,
                                         TBool /*aEnableRtcp*/,
                                         const TRtcpParams* /*aRtcpParams*/ )
    {
    
    // return iManager->CreateSessionL( aSessionParams, aPort, aEnableRtcp, aRtcpParams );
    TRtpId rtpid( 1 );
    iReserved4 = aSessionParams.iSocketBufSize;
    return rtpid;
    }

// ---------------------------------------------------------------------------
// TRtpId CRtpAPI::CreateSessionL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TRtpId CRtpAPI::CreateSessionL( const TCreateSessionParams& aSessionParams,
                                         TUint& /*aPort*/,
                                         TBool /*aEnableRtcp*/,
                                         const TRtcpParams* /*aRtcpParams*/,
                                         CSRTPSession& /*aSession*/ )
    {
    TRtpId rtpid( 1 );
    iReserved3 = ETrue;
    iReserved4 = aSessionParams.iSocketBufSize;
    return rtpid;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SetRemoteAddress()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SetRemoteAddress( TRtpId /*aSessionId*/, const TInetAddr& /*aRemoteAddr*/ )
    {
    // return iManager->SetRemoteAddress( aSessionId, const_cast< TInetAddr& >( aRemoteAddr ) );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// TInt CRtpAPI::SetRemoteRtcpAddress()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SetRemoteRtcpAddress( TRtpId /*aSessionId*/, const TInetAddr& /*aRemoteRtcpAddr*/ )
    {
    // return iManager->SetRemoteRtcpAddress( aSessionId, const_cast< TInetAddr& >( aRemoteRtcpAddr ) );
    return KErrNone;
    }
    
// ---------------------------------------------------------------------------
// TInt CRtpAPI::StartSession()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::StartSession( TRtpId /*aSessionId*/ )
    {
    // return iManager->StartSession( aSessionId );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::CloseSession()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::CloseSession( TRtpId /*aSessionId*/ )
    {
    //iManager->CloseSession( aSessionId );
    }

// ---------------------------------------------------------------------------
// TRtpId CRTPAPI::GetSessionId()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TRtpId CRtpAPI::GetSessionId( TRtpId /*aStreamId*/ )
    {
    // return iManager->GetSessionId( aStreamId );
    TRtpId rtpid(1);
    return rtpid;
    }
    




/*****************************************************************************/ 
/****************************** STREAM FUNCTIONS  ****************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// TRtpId CRtpAPI::CreateReceiveStreamL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TRtpId CRtpAPI::CreateReceiveStreamL( TRtpId /*aSessionId*/, 
                                               const TRcvStreamParams& /*aParams*/ )
    {
    // return iManager->CreateReceiveStreamL( aSessionId, aParams );
    TRtpId rtpid(2);
    return rtpid;
    }

// ---------------------------------------------------------------------------
// TRtpId CRtpAPI::CreateTransmitStreamL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TRtpId CRtpAPI::CreateTransmitStreamL( TRtpId /*aSessionId*/, 
                                                const TTranStreamParams& /*aParams*/, 
                                                TRtpSSRC& /*aSSRC*/ )
    {
    // return iManager->CreateTransmitStreamL( aSessionId, aParams, aSSRC );
    TRtpId rtpid(3);
    return rtpid;
    }

// ---------------------------------------------------------------------------
// TRtpId CRtpAPI::CreateTransmitStreamExtL()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TRtpId CRtpAPI::CreateTransmitStreamExtL( TRtpId /*aSessionId*/,
                                                   const TTranStreamParams& /*aParams*/,
                                                   const TRtpSSRC /*aSSRC*/ )
    {
    // return iManager->CreateTransmitStreamExtL( aSessionId, aParams, aSSRC );
    TRtpId rtpid(0);
    return rtpid;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::CloseStream()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::CloseStream( TRtpId /*aStreamId*/ )
    {
    //iManager->CloseStream( aStreamId );
    }
    
// ---------------------------------------------------------------------------
// TInt CRtpAPI::GetStreamStatistics()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::GetStreamStatistics( TRtpId /*aStreamId*/, TRtpPeerStat& /*aStat*/ )
    {
    // return iManager->GetStreamStatistics( aStreamId, aStat );
    return KErrNone;
    }
    


/*****************************************************************************/ 
/****************************** RTP FUNCTIONS  *******************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// TInt CRtpAPI::RegisterRtpObserver()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::RegisterRtpObserver( TRtpId /*aSessionId*/, MRtpObserver& /*aObserver*/ )
    {
    //return iManager->RegisterRtpObserver( aSessionId, aObserver );
    return 0;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::UnregisterRtpObserver()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::UnregisterRtpObserver( TRtpId /*aSessionId*/ )
    {
    //iManager->UnregisterRtpObserver( aSessionId );
    }


// ---------------------------------------------------------------------------
// RSocket* CRtpAPI::GetRtpSocket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C RSocket* CRtpAPI::GetRtpSocket( TRtpId /*aSessionId*/ )
    {
    // return iManager->GetRtpSocket( aSessionId );
    return NULL;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtpPacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtpPacket( TRtpId /*aTranStreamId*/,
                                      const TRtpSendHeader& /*aHeaderInfo*/,
                                      const TDesC8& /*aPayloadData*/ )
    {
    // return iManager->SendRtpPacket( aTranStreamId, aHeaderInfo, aPayloadData );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtpPacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtpPacket( TRtpId /*aTranStreamId*/,
                                      TRtpSequence /*aSequenceNum*/,
                                      const TRtpSendHeader& /*aHeaderInfo*/,
                                      const TDesC8& /*aPayloadData*/,
                                      TRequestStatus& aStatus )
    {
    // return iManager->SendRtpPacket( aTranStreamId, aSequenceNum, aHeaderInfo, 
    //                              aPayloadData, aStatus );
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtpPacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtpPacket( TRtpId /*aTranStreamId*/,
                                      const TRtpSendHeader& /*aHeaderInfo*/,
                                      const TDesC8& /*aPayloadData*/,
                                      TRequestStatus& aStatus )
    {
    // return iManager->SendRtpPacket( aTranStreamId, aHeaderInfo, aPayloadData, aStatus );
    
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// void CancelSend( TRtpId aSessionId )
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::CancelSend( TRtpId /*aSessionId*/ )
    {
    //iManager->CancelSend( aSessionId );
    }



/*****************************************************************************/ 
/****************************** RTCP FUNCTIONS  ******************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// TInt CRtpAPI::RegisterRtcpObserver()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::RegisterRtcpObserver( TRtpId /*aSessionId*/, MRtcpObserver& /*aObserver*/ )
    {
    // return iManager->RegisterRtcpObserver( aSessionId, aObserver );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CRtpAPI::UnregisterRtcpObserver()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CRtpAPI::UnregisterRtcpObserver( TRtpId /*aSessionId*/ )
    {
    //iManager->UnregisterRtcpObserver( aSessionId );
    }


// ---------------------------------------------------------------------------
// TInt CRtpAPI::SetNonRTPDataObserver()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SetNonRTPDataObserver( TRtpId /*aSessionId*/, 
                                    MNonRTPDataObserver* /*aNonRTPDataObserver*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// RSocket* CRtpAPI::GetRtcpSocket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C RSocket* CRtpAPI::GetRtcpSocket( TRtpId /*aSessionId*/ )
    {
    //return iManager->GetRtcpSocket( aSessionId );
    return NULL;
    }


// ---------------------------------------------------------------------------
// TInt CRtpAPI::SetRtcpParameters()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SetRtcpParameters( TRtpId /*aSessionId*/,
                                          const TRtcpParams& /*aRtcpParams*/ )
    {
    // return iManager->SetRtcpParameters( aSessionId, aRtcpParams );
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtcpByePacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtcpByePacket( TRtpId /*aTranStreamId*/,
                                          const TDesC8& /*aReason*/ )
    {
    //return iManager->SendRtcpByePacket( aTranStreamId, aReason );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtcpAppPacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtcpAppPacket( TRtpId /*aTranStreamId*/,
                                          const TRtcpApp& /*aApp*/ )
    {
    //return iManager->SendRtcpAppPacket( aTranStreamId, aApp );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtcpSrPacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtcpSrPacket( TRtpId /*aTranStreamId*/ )
    {
    //return iManager->SendRtcpSrPacket( aTranStreamId );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SendRtcpRrPacket()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SendRtcpRrPacket( TRtpId /*aTranStreamId*/ )
    {
    //return iManager->SendRtcpRrPacket( aTranStreamId );
    return KErrNone;
    }

EXPORT_C void CRtpAPI::SendDataL( TRtpId /*aSessionId*/,
                                TBool /*aUseRTPSocket*/,
                                const TDesC8& /*aData*/,
                                TRequestStatus& aStatus )
    {
    TRequestStatus* status = &aStatus;
    User::RequestComplete( status, KErrNone );
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SuspendRtcpSending()
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SuspendRtcpSending( TRtpId /*aSessionId*/,
                                           TBool /*aAutoSending*/ )
    {
    //return iManager->SuspendRtcpSending( aSessionId, aAutoSending );
    return KErrNone;
    }
        
// ---------------------------------------------------------------------------
// TInt CRtpAPI::IsRtcpSendingSuspended()
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::IsRtcpSendingSuspended( TRtpId /*aSessionId*/,
                                               TBool& /*aAutoSending*/ )
    {
    //return iManager->IsRtcpSendingSuspended( aSessionId, aAutoSending );
    return KErrNone;
    }

/*****************************************************************************/ 
/***************************** SAMPLING FUNCTIONS ****************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// TUint32 CRtpAPI::GetSamplingRate()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TUint32 CRtpAPI::GetSamplingRate( TUint8 /*aPayloadType*/ )
    {
    //return iManager->GetSamplingRate( aPayloadType );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::SetSamplingRate()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::SetSamplingRate( TUint8 /*aPayloadType*/, TUint32 /*aSampleRate*/ )
    {
    //return iManager->SetSamplingRate( aPayloadType, aSampleRate );
    return KErrNone;
    }


/*****************************************************************************/ 
/***************************** CUSTOM FUNCTIONS ******************************/ 
/*****************************************************************************/ 
// ---------------------------------------------------------------------------
// TInt CRtpAPI::CustomCommandSync()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::CustomCommandSync( TInt aFunction,
                                          const TDesC8& /*aInputData1*/,
                                          const TDesC8& /*aInputData2*/,
                                          TDes8& /*aOutputData*/ )
    {
    if ( aFunction == 3 )
        {
        return iReserved3;
        }
    if ( aFunction == 4 )
        {
        return iReserved4;
        }
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// TInt CRtpAPI::CustomCommandAsync()
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRtpAPI::CustomCommandAsync( TInt /*aFunction*/,
                                           const TDesC8& /*aInputData1*/,
                                           const TDesC8& /*aInputData2*/,
                                           TDes8& /*aOutputData*/,
                                           TRequestStatus& /*aStatus*/ )
    {
    return KErrNotSupported;
    }


/*****************************************************************************/ 
/******************************** DLL FUNCTIONS ******************************/ 
/*****************************************************************************/ 
#if !defined ( EKA2 ) && !defined ( RTP_UNIT_TEST )
// ---------------------------------------------------------------------------
// All E32 DLLs need an entry point...
// DLL entry point
// ---------------------------------------------------------------------------
//
GLDEF_C TInt E32Dll( TDllReason /*aReason*/ )
    {
    return ( KErrNone );
    }
#endif

//  End of File
