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
* Description:    RTCP Receiver class from RTP Datasource
*
*/




// INCLUDE FILES
#include "mccrtcpreceiver.h"
#include "mccdef.h"
#include "mccinternaldef.h"
#include "mccsubcontrollerlogs.h"
#include "mccinternalevents.h"
#include "mccresources.h"
#include "mccrtpmanager.h"


// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS


// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccRtcpReceiver::CMccRtcpReceiver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMccRtcpReceiver::CMccRtcpReceiver( 
    CMccRtpManager* aRtpManager,
    MAsyncEventHandler& aEventHandler, 
    MMccResources& aMccResources,
    CRtpAPI& aRtpAPI ) : 
    iStream( KNullId ),
    iEventHandler( aEventHandler ), 
    iMccResources( aMccResources ),
    iRtpAPI( aRtpAPI ),
    iSessionId( KNullId ),
    iRtpManager( aRtpManager )
    {

    }

// -----------------------------------------------------------------------------
// CMccRtcpReceiver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMccRtcpReceiver* CMccRtcpReceiver::NewL( 
    CMccRtpManager* aRtpManager,
    MAsyncEventHandler& aEventHandler, 
    MMccResources& aMccResources,
    CRtpAPI& aRtpAPI )
    {
    CMccRtcpReceiver* self = new( ELeave ) CMccRtcpReceiver( aRtpManager,
                                                             aEventHandler,
                                                             aMccResources, 
                                                             aRtpAPI );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMccRtcpReceiver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::ConstructL( )
    {
    }
    
// -----------------------------------------------------------------------------
// CMccRtcpReceiver::~CMccRtcpReceiver
// Destructor
// -----------------------------------------------------------------------------
//
CMccRtcpReceiver::~CMccRtcpReceiver()
    {
    __SUBCONTROLLER( "CMccRtcpReceiver::~CMccRtcpReceiver()" )
    
    if ( iSessionId != KNullId )
        {
        iRtpAPI.SetNonRTPDataObserver( iSessionId, NULL );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtcpReceiver::HandleReceiving
// -----------------------------------------------------------------------------   
//
TInt CMccRtcpReceiver::HandleReceiving( TRtpId aSessionId, TBool aEnableReceiving )
    {
    __SUBCONTROLLER( "CMccRtcpReceiver::HandleReceiving()" )
    TInt err = KErrNone;
    
    // TBD! Never disable (maybe disabling is needed in future)
    //
    aEnableReceiving = ETrue;
    
    if ( iEnableRtcp != aEnableReceiving )
        { 
        iSessionId = aSessionId;
        iEnableRtcp = aEnableReceiving;
        if ( iEnableRtcp )
            {
            __SUBCONTROLLER( "CMccRtcpReceiver::HandleReceiving(), enabling" )
            err = iRtpAPI.RegisterRtcpObserver( iSessionId, *this );
            if ( !err )
                {
                err = iRtpAPI.SetNonRTPDataObserver( iSessionId, this ); 
                }
            }
        else
            {
            __SUBCONTROLLER( "CMccRtcpReceiver::HandleReceiving(), disabling" )
            iRtpAPI.UnregisterRtcpObserver( iSessionId );
            iRtpAPI.SetNonRTPDataObserver( iSessionId, NULL );
            }
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CMccRtcpReceiver::SdesReceived()
// From MRtcpObserver
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::SdesReceived( TRtpSSRC aSSRC, 
                                     const TRtpSdesParams& aParams )
    { 
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SdesReceived() SSRC:", aSSRC )
    
    // Take copies of sdes member data and create new sdes object which will point
    // to those new data. This will prevent data loss while transfering the event
    // through IPC.

    TPckgBuf<TRtpSdesParams> tempSdesPackage( aParams );
    
    TInt sdesPackageLen( sizeof( tempSdesPackage ) );
    
    TInt sdesLen = sdesPackageLen +
                   aParams.iCName.Length() + 
                   aParams.iUserName.Length() + 
                   aParams.iEmail.Length() + 
                   aParams.iPhoneNumber.Length() + 
                   aParams.iLocation.Length() + 
                   aParams.iSwToolName.Length() + 
                   aParams.iNoticeStatus.Length() + 
                   aParams.iPrivate.Length();
                    
    if ( sdesLen <= KMccMaxRtcpPacketDataLength )
        {
        ClearEventData();
        
        TRtpSdesParams sdes;
        iEventData.iSsrc = aSSRC;
        
        TUint8* currentPtr = const_cast<TUint8*>( iEventData.iRtcpPacketData.Ptr() );
        currentPtr += sdesPackageLen;
        
        iEventData.iRtcpPacketData.SetLength( sdesPackageLen );
        
        TInt currentPos( sdesPackageLen );
        
        DoSdesMemberCopy( sdes.iCName, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iCName );
                          
        DoSdesMemberCopy( sdes.iUserName, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iUserName );
                          
        DoSdesMemberCopy( sdes.iEmail, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iEmail ); 
                          
        DoSdesMemberCopy( sdes.iPhoneNumber, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iPhoneNumber ); 
                          
        DoSdesMemberCopy( sdes.iLocation, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iLocation ); 
                          
        DoSdesMemberCopy( sdes.iSwToolName, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iSwToolName ); 
                          
        DoSdesMemberCopy( sdes.iNoticeStatus, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iNoticeStatus );
                          
        DoSdesMemberCopy( sdes.iPrivate, &currentPtr, currentPos, 
                          iEventData.iRtcpPacketData, aParams.iPrivate );
        
        // Finally copy sdes into beginning of the data
        TPckgBuf<TRtpSdesParams> sdesPackage( sdes );
        iEventData.iRtcpPacketData.Insert( 0, sdesPackage );
        
        SendRtcpEventToClient( KRtcpSdesPacket, ETrue, EFalse );
        }
    }
    
// -----------------------------------------------------------------------------
// CMccRtcpReceiver::ByeReceived()
// From MRtcpObserver
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::ByeReceived( TRtpId aStreamId, 
                                    TRtpSSRC aSSRC, 
                                    const TDesC8& aReason )    
    {
    __SUBCONTROLLER_INT2( "CMccRtcpReceiver::ByeReceived() STREAM:", aStreamId, 
                          "SSRC:", aSSRC )

    iStream = aStreamId;
    
    SendRtcpEventWithPacketDataToClient( KRtcpByePacket, aSSRC, aReason, ETrue );
    }
    
// -----------------------------------------------------------------------------
// CMccRtcpReceiver::AppReceived()
// From MRtcpObserver
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::AppReceived( TRtpId aStreamId, 
                                    TRtpSSRC aSSRC, 
                                    const TRtcpApp& aApp )    
    {
    __SUBCONTROLLER_INT2( "CMccRtcpReceiver::AppReceived() STREAM:", aStreamId,
                          "SSRC:", aSSRC )
    
    iStream = aStreamId;
    
    TPckgBuf<TRtcpApp> app( aApp );
    SendRtcpEventWithPacketDataToClient( KRtcpAppPacket, aSSRC, app, ETrue );
    }
    
// -----------------------------------------------------------------------------
// CMccRtcpReceiver::SrReceived()
// From MRtcpObserver
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::SrReceived( TRtpId aStreamId, 
                                   TRtpSSRC aSSRC, 
                                   const TTimeStamps& aTimeStamps )
    {
    __SUBCONTROLLER_INT2( "CMccRtcpReceiver::SrReceived() STREAM:", aStreamId,
                          "SSRC:", aSSRC )
    
    iStream = aStreamId;
    
    ClearEventData();
    
    iEventData.iSsrc = aSSRC;
    iEventData.iTimeStamps = aTimeStamps;
    iRtpAPI.GetStreamStatistics( iStream, iEventData.iStats );
    
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() packets sent:", iEventData.iStats.iNumPacketsSent )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() octets sent:", iEventData.iStats.iCumNumOctetsSent )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() roundtrip delay:", iEventData.iStats.iRoundTripDelay )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() tx bandwidth:", iEventData.iStats.iTxBandwidth )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() packets lost:", iEventData.iStats.iCumNumPacketsLost )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() fractions lost:", iEventData.iStats.iFractionLost )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() arrival jitter:", iEventData.iStats.iArrivalJitter )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() rx bandwidth:", iEventData.iStats.iRxBandwidth )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() channelbuffer size:", iEventData.iStats.iChannelBufferSize )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() ntp timestamp sec:", iEventData.iStats.iNTPTimeStampSec )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() ntp timestamp frac:", iEventData.iStats.iNTPTimeStampFrac )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SrReceived() timestamp:", iEventData.iStats.iTimeStamp )
    
    SendRtcpEventToClient( KRtcpSrPacket, EFalse, ETrue );
    }
    
// -----------------------------------------------------------------------------
// CMccRtcpReceiver::RrReceived()
// From MRtcpObserver
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::RrReceived( TRtpId aStreamId, TRtpSSRC aSSRC )
    {
    __SUBCONTROLLER_INT2( "CMccRtcpReceiver::RrReceived() STREAM:", aStreamId,
                          "SSRC:", aSSRC )
    
    iStream = aStreamId;
    
    ClearEventData();
     
    iEventData.iSsrc = aSSRC;
    iRtpAPI.GetStreamStatistics( iStream, iEventData.iStats );
    
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() packets sent:", iEventData.iStats.iNumPacketsSent )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() octets sent:", iEventData.iStats.iCumNumOctetsSent )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() roundtrip delay:", iEventData.iStats.iRoundTripDelay )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() tx bandwidth:", iEventData.iStats.iTxBandwidth )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() packets lost:", iEventData.iStats.iCumNumPacketsLost )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() fractions lost:", iEventData.iStats.iFractionLost )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() arrival jitter:", iEventData.iStats.iArrivalJitter )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() rx bandwidth:", iEventData.iStats.iRxBandwidth )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() channelbuffer size:", iEventData.iStats.iChannelBufferSize )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() ntp timestamp sec:", iEventData.iStats.iNTPTimeStampSec )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() ntp timestamp frac:", iEventData.iStats.iNTPTimeStampFrac )
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::RrReceived() timestamp:", iEventData.iStats.iTimeStamp )
    
    SendRtcpEventToClient( KRtcpRrPacket, EFalse, ETrue );
    }

// -----------------------------------------------------------------------------
// CMccRtcpReceiver::NonRTPDataReceived()
// From MRtcpObserver
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::NonRTPDataReceived( TUint /*aPort*/,
                                           TBool aRTPPort,
                                           const TDesC8& aNonRTPData )
    {
    __SUBCONTROLLER( "CMCCRtcpReceiver::NonRTPDataReceived()" )

    // Currently, Non-RTP data is accepted only on the RTCP port
    if ( !aRTPPort )
        {
        SendRtcpEventWithPacketDataToClient( KRtcpPacketUndefined, 
                                             0, 
                                             aNonRTPData,
                                             EFalse );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtcpEvent::SendRtcpEventWithPacketDataToClient()
// Constructs RTCP event containing RTCP packet data and sends the event to
// client side
// -----------------------------------------------------------------------------
//
TInt CMccRtcpReceiver::SendRtcpEventWithPacketDataToClient( 
    TMccRtcpPacketType aPacketType, 
    const TRtpSSRC& aSSRC,
    const TDesC8& aData,
    TBool aResolveEndpointBySsrc )
    {
    if ( aData.Length() > KMccMaxRtcpPacketDataLength )
        {
        __SUBCONTROLLER( "CMCCRtcpReceiver::SendRtcpEventWithPacketDataToClient(), data size too big ")
        
        return KErrOverflow;
        }
    
    ClearEventData();
         
    iEventData.iSsrc = aSSRC;
    iEventData.iRtcpPacketData.Copy( aData );

    SendRtcpEventToClient( aPacketType, aResolveEndpointBySsrc, EFalse );
        
    return KErrNone;   
    }

// -----------------------------------------------------------------------------
// CMccRtcpEvent::SendRtcpEventToClient()
// Sends the RTCP event to client side
// -----------------------------------------------------------------------------
//
void CMccRtcpReceiver::SendRtcpEventToClient( 
    TMccRtcpPacketType aPacketType,
    TBool aResolveEndpointBySsrc,
    TBool aResolveEndpointByRtpStreamId )
    {    
    __SUBCONTROLLER_INT1( "CMccRtcpReceiver::SendRtcpEventToClient() packetType:", aPacketType )
    
    TUint32 endpointId( 0 );
    
    if ( aResolveEndpointBySsrc )
        {
        __SUBCONTROLLER( "CMCCRtcpReceiver::SendRtcpEventToClient(), resolve endpointId by ssrc" )
        MDataSource* source = 0;
        MDataSink* sink = 0;
        if ( !iMccResources.FindNetworkResourceByRtpSsrc( iEventData.iSsrc, &source, &sink ) )
            {
            __SUBCONTROLLER( "CMCCRtcpReceiver::SendRtcpEventToClient(), resolved" )
            endpointId = source != 0 ? MCC_ENDPOINT_ID( source ) : 
                                       MCC_ENDPOINT_ID( sink );
            }
        }
    
    if ( aResolveEndpointByRtpStreamId )
        {
        __SUBCONTROLLER( "CMCCRtcpReceiver::SendRtcpEventToClient(), resolve endpointId by streamId" )
        MDataSource* source = 0;
        MDataSink* sink = 0;
        if ( !iMccResources.FindNetworkResourceByRtpStreamId
                ( iRtpManager->MccSessionId(), iStream, &source, &sink ) )
            {
            __SUBCONTROLLER( "CMCCRtcpReceiver::SendRtcpEventToClient(), resolved" )
            endpointId = source != 0 ? MCC_ENDPOINT_ID( source ) : 
                                       MCC_ENDPOINT_ID( sink );
            }
        }

    iEventData.iRtcpPacketType = aPacketType;
    
    TMccEvent event;
    event.iEventCategory = KMccEventCategoryRtcp;
    event.iEventType = KMccRtcpReceived;
    event.iEndpointId = endpointId;
    
    // Save some stack memory
    {
    event.iEventData.Copy( TMccRtcpEventDataPackage( iEventData ) );
    }
    
    TMccInternalEvent internalEvent( KMccRtpSourceUid, 
                                     EMccInternalEventNone,
                                     event );
    
    // Stream id, link id and session id will be filled on the way up
    iEventHandler.SendEventToClient( internalEvent );	
    }

// -----------------------------------------------------------------------------
// CMccRtcpEvent::DoSdesMemberCopy()
// -----------------------------------------------------------------------------
//   
void CMccRtcpReceiver::DoSdesMemberCopy( 
    TPtrC8& aDestination,
    TUint8** aCurrentPtr, 
    TInt& aCurrentPos,
    TDes8& aRtcpPacketData, 
    const TDesC8& aSource )
    {
    TInt len( aSource.Length() );
    
    if ( aCurrentPos + len <= aRtcpPacketData.MaxLength() )
        {
        aRtcpPacketData.Insert( aCurrentPos, aSource );
        
        aDestination.Set( *aCurrentPtr, len );
        
        *aCurrentPtr += len;
        aCurrentPos += len;
        }
    else
        {
        aDestination.Set( *aCurrentPtr, 0 );
        }
    }

// -----------------------------------------------------------------------------
// CMccRtcpEvent::ClearEventData()
// -----------------------------------------------------------------------------
//     
void CMccRtcpReceiver::ClearEventData()
    {
    iEventData = TMccRtcpEventData();
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
