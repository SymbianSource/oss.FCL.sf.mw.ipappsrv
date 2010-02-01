/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Mcc Subthread client baseclass
*
*/




// INCLUDE FILES
#include "mccsubthreadclientbase.h"
#include "mccrtpmanager.h"
#include "mccsubcontrollerlogs.h"
#include "mccsymstreambase.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::~CMccSubThreadClientBase
// Destructor
// -----------------------------------------------------------------------------
CMccSubThreadClientBase::~CMccSubThreadClientBase()
    {
    iDatasink = NULL;
    iDatasource = NULL;
    iObserver = NULL;
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::GetCodecL
// Returns the used codec
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::CodecInformationL( TUint32 aStreamId,
                                                 TMccCodecInfo& aCodec )
    {
    __SUBCONTROLLER_INT1( "CMccSubThreadClientBase::GetCodecL streamId", aStreamId )
    
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    
    const TInt index = FindStreamL( aStreamId );
    iStreams[index]->GetCodecL( aCodec );
    
	__SUBCONTROLLER( "CMccSubThreadClientBase::GetCodecL exit" )
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SendMediaSignalL
// Default implementation of the signal sending
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::SendMediaSignalL( const TMccEvent& /*aEvent*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SetCodecInformationL
// Sets the codec information for this stream
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::SetCodecInformationL( TUint32  aStreamId,
    const TMccCodecInfo& aCodecInfo,
    const TDesC8& aFmtp )
    {
    __SUBCONTROLLER_INT1( "CMccSubThreadClientBase::SetCodecInformationL streamId",
        aStreamId )
    __SUBCONTROLLER_INT1( "CMccSubThreadClientBase::SetCodecInformationL streamCount",
        iStreams.Count() )
    
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
	
    const TInt index = FindStreamL( aStreamId );
    iStreams[ index ]->LoadCodecL( aCodecInfo, aFmtp );
    iStreams[ index ]->SetMMFPriority( aCodecInfo.iPriority );
   
	__SUBCONTROLLER( "CMccSubThreadClientBase::SetCodecInformationL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SetRemoteAddressL
// Sets the remote address of uplink stream
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::SetRemoteAddressL( TInetAddr /*aRemAddr*/ )
    {
    User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SetRemoteRtcpAddrL
// Sets the remote address of uplink stream
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::SetRemoteRtcpAddrL( TInetAddr /*aRemAddr*/ )
    {
    User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::InitializeLinkL
// Creates the RTP session in the subthread, STEP #1
// -----------------------------------------------------------------------------
//
void CMccSubThreadClientBase::InitializeLinkL( TRequestStatus& /*aStatus*/, 
                                               TInt /*aIapId*/ )
    {
    User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::CreateSessionL
// Creates the RTP session in the subthread, STEP #2
// -----------------------------------------------------------------------------
//
void CMccSubThreadClientBase::CreateRtpSessionL( TUint /*aPort*/, 
                                                 TBool /*aEnableRTCP*/,
                                                 TInt /*aIpTos*/,
                                                 CMccRtpMediaClock& /*aClock*/ )
    {
    User::Leave( KErrNotSupported );    
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SetLinkId
// Creates the RTP session in the subthread
// -----------------------------------------------------------------------------
//
void CMccSubThreadClientBase::SetLinkId( TUint32 aLinkId )
    {
    iLinkId = aLinkId;
    if ( iRtpmanager )
        {
        iRtpmanager->SetLinkId( aLinkId );
        }
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::GetLinkIndex
// Creates the RTP session in the subthread
// -----------------------------------------------------------------------------
//
TUint32 CMccSubThreadClientBase::GetLinkId() const
    {
    return iLinkId;
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::MccSessionId
// -----------------------------------------------------------------------------
//
TUint32 CMccSubThreadClientBase::MccSessionId() const
    {
    return iSessionId;
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::CloseL 
// Closes all links
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::CloseL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::StreamsExistsL 
// -----------------------------------------------------------------------------
TBool CMccSubThreadClientBase::StreamsExistsL()
    {
    return  EFalse;
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::StartInactivityTimer
// Starts inactivity timer for a stream 
// -----------------------------------------------------------------------------    
void CMccSubThreadClientBase::StartInactivityTimerL( const TUint32 /*aStreamId*/,
                                            		 TUint32 /*aTimeoutTime*/ )
	{
	User::Leave( KErrNotSupported );
	}
       
// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::StopInactivityTimer
// Stops inactivity timer for a stream 
// -----------------------------------------------------------------------------    
void CMccSubThreadClientBase::StopInactivityTimerL( const TUint32 /*aStreamId*/ )
	{
	User::Leave( KErrNotSupported );
	}

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::GetSupportedBitratesL
// Gets the supported bitrates from the codec of the specified stream
// -----------------------------------------------------------------------------    
void CMccSubThreadClientBase::GetSupportedBitratesL( const TUint32 /*aStreamId*/,
                                  RArray<TUint>& /*aBitrates*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SendRTCPReceiverReportL
// Sends a RTCP receiver report
// -----------------------------------------------------------------------------
//  
void CMccSubThreadClientBase::SendRTCPReceiverReportL( const TUint32 /*aStreamId*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SendRTCPSenderReportL
// Sends a RTCP sender report
// -----------------------------------------------------------------------------
// 
void CMccSubThreadClientBase::SendRTCPSenderReportL( const TUint32 /*aStreamId*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SendRTCPDataL
// Sends Non-RTCP data
// -----------------------------------------------------------------------------
// 
void CMccSubThreadClientBase::SendRTCPDataL( const TUint32 /*aStreamId*/,
                                             const TDesC8& /*aData*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::GetFmtpAttrL
// Fetches the FMTP attribute of the current codec
// -----------------------------------------------------------------------------
// 
void CMccSubThreadClientBase::GetFmtpAttrL( const TUint32 /*aStreamId*/, TDes8& /*aFmtp*/ )
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::SetPrioritySettingsL 
// Sets priority
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::SetPrioritySettingsL( TUint32 aStreamId,
    const TMMFPrioritySettings& aPrioritySet )
    {
    __SUBCONTROLLER_INT1( "CMccSubThreadClientBase::SetPrioritySettingsL, streamId", 
	                      aStreamId )
    
	__ASSERT_ALWAYS( iStreams.Count(), User::Leave( KErrNotFound ) );
    
    const TInt index = FindStreamL( aStreamId );
    iStreams[ index ]->SetMMFPriority( aPrioritySet );
    
	__SUBCONTROLLER( "CMccSubThreadClientBase::SetPrioritySettingsL, exit" )	
    }
    
// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::GetSSRCL 
// -----------------------------------------------------------------------------
void CMccSubThreadClientBase::GetSSRCL( const TUint32  /*aStreamId*/, TUint32& /*aSSRCValue*/ )
	{
 	User::Leave( KErrNotSupported );	
	}

        
// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::FindStreamL
// Finds a streams index from the stream array
// -----------------------------------------------------------------------------
//
TInt CMccSubThreadClientBase::FindStreamL( TUint32 aStreamId )
    {
    const TInt index = FindStream( aStreamId );
    User::LeaveIfError( index );	
    return index;
    }

// -----------------------------------------------------------------------------
// CMccSubThreadClientBase::FindStream
// Finds a streams index from the stream array
// -----------------------------------------------------------------------------
//
TInt CMccSubThreadClientBase::FindStream( TUint32 aStreamId )
    {
	__SUBCONTROLLER_INT1( "CMccSubThreadClientBase::FindStream, streamId", aStreamId )
    
    const TInt strmCount = iStreams.Count();
    for( TInt k = 0; k < strmCount; k++ )
        {
        if( aStreamId == iStreams[k]->MccStreamId() )
            {
            return k;
            }
        }
    
    return KErrNotFound;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
