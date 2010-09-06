/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/

        
// INCLUDE FILES
#include "mccmsrpsource.h"
#include "mmccinterfacedef.h"
#include "mccmsrpsourcesinklogs.h"
#include "mccinternalevents.h"
#include "mccinternaldef.h"
#include "rtpheader.h"
#include "formatstatemachine.h"


// ============================= LOCAL FUNCTIONS ===============================

void CMccMsrpSource::IncomingMessage( HBufC8* aMessageContent, TInt aStatus )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::IncomingMessage" )
    __MSRPSOURCESINK_CONTROLL_INT1( "Status : %d    ----- ", aStatus)
    
	TRAP_IGNORE(PlayoutMsrpPacketL(aMessageContent));
    }
    
void CMccMsrpSource::IncomingReport( TInt aStatus, TInt aRangeStart,
                                           TInt aRangeEnd, TInt aTotal )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::IncomingReport" )
    __MSRPSOURCESINK_CONTROLL("\nReport has been received!\n");
    __MSRPSOURCESINK_CONTROLL_INT1("Status      :", aStatus);
    __MSRPSOURCESINK_CONTROLL_INT1("Range start :", aRangeStart);
    __MSRPSOURCESINK_CONTROLL_INT1("Range end   :", aRangeEnd);
    __MSRPSOURCESINK_CONTROLL_INT1("Total       :", aTotal);
    }
    
void CMccMsrpSource::SendResult( TInt aStatus, const TDesC8& aMessageid )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SendResult" )
    __MSRPSOURCESINK_CONTROLL("\nResult of send... ");
    __MSRPSOURCESINK_CONTROLL_INT1("Status :", aStatus);
    
    __MSRPSOURCESINK_CONTROLL_INT1("Message ID :", aMessageid);
    }
    
void CMccMsrpSource::ListenPortFailure( TInt aStatus )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::ListenPortFailure" )
    __MSRPSOURCESINK_CONTROLL("\nListen port failure. ");
    __MSRPSOURCESINK_CONTROLL_INT1("Status :", aStatus);
    //CActiveScheduler::Stop();
    }



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccMsrpSource::NewSourceL
// -----------------------------------------------------------------------------
//
MDataSource* CMccMsrpSource::NewSourceL( TUid /*aImplementationUid*/, 
                                         const TDesC8& /*aInitData*/ )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::NewSourceL" )
    
    CMccMsrpSource* self = new ( ELeave ) CMccMsrpSource();
    return static_cast<MDataSource*>( self );
    }

// -----------------------------------------------------------------------------
// CMccMsrpSource::ConstructSourceL
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::ConstructSourceL( const TDesC8& /*aInitData*/ )
    {    	
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::ConstructSourceL" )
    }
	    
// -----------------------------------------------------------------------------
// Constructor.
// -----------------------------------------------------------------------------
//
CMccMsrpSource::CMccMsrpSource() : CMccDataSource( KMccMsrpSourceUid )
    {
    }
        
// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMccMsrpSource::~CMccMsrpSource()
    {   
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::~CMccMsrpSource" )
    
    iFillBufferRequester = NULL;

    iBufferToBeFilled = NULL;
    iConsumer = NULL;
    }

	
// -----------------------------------------------------------------------------
// CMccMsrpSource::SourcePrimeL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::SourcePrimeL()
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourcePrimeL" )
    
    SendStreamEventToClient( KMccStreamPrepared );
	}
	    
// -----------------------------------------------------------------------------
// CMccMsrpSource::PlayL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::SourcePlayL()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourcePlayL" )
    
    if ( iPaused )
        {
        SendStreamEventToClient( KMccStreamResumed );
        }
    else
        {
    	SendStreamEventToClient( KMccStreamStarted );	
        }
		
    iPaused = EFalse;
    }

// -----------------------------------------------------------------------------
// CMccMsrpSource::PauseL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::SourcePauseL()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourcePauseL" )
    
    iPaused = ETrue;
    
    SendStreamEventToClient( KMccStreamPaused );
	}

// -----------------------------------------------------------------------------
// CMccMsrpSource::StopL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::SourceStopL()
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourceStopL" )
    
    SendStreamEventToClient( KMccStreamStopped );
    }

// -----------------------------------------------------------------------------
// CMccMsrpSource::SourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TFourCC CMccMsrpSource::SourceDataTypeCode( TMediaId /*aMediaId*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourceDataTypeCode" )
	
	return iFourCC;
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSource::SetSourceDataTypeCode()
// -----------------------------------------------------------------------------
//
TInt CMccMsrpSource::SetSourceDataTypeCode( TFourCC aCodec, 
                            TMediaId /*aMediaId*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SetSourceDataTypeCode" )
	
	iFourCC = aCodec;
	return KErrNone;
	}	

// -----------------------------------------------------------------------------
// CMccMsrpSource::BufferEmptiedL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::BufferEmptiedL( CMMFBuffer* /*aBuffer*/ )
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::BufferEmptiedL" )
    
   	User::Leave( KErrNotSupported );
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSource::CanCreateSourceBuffer()
// -----------------------------------------------------------------------------
//
TBool CMccMsrpSource::CanCreateSourceBuffer()
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::CanCreateSourceBuffer" )

    return EFalse;
	}	

// -----------------------------------------------------------------------------
// CMccMsrpSource::CreateSourceBufferL()
// -----------------------------------------------------------------------------
//
CMMFBuffer* CMccMsrpSource::CreateSourceBufferL( 
	TMediaId /*aMediaId*/, 
    TBool& /*aReference*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::CreateSourceBufferL" )

   	User::Leave( KErrNotSupported );
	
	return NULL;	
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSource::SourceThreadLogon()
// -----------------------------------------------------------------------------
//
TInt CMccMsrpSource::SourceThreadLogon( MAsyncEventHandler& aEventHandler )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourceThreadLogon" )
	
	iAsyncEventHandler = &aEventHandler;
	
    return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// CMccMsrpSource::SourceThreadLogoff()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::SourceThreadLogoff()
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SourceThreadLogoff" )
    
    iAsyncEventHandler = NULL;
	}

// -----------------------------------------------------------------------------
// CMccMsrpSource::FillBufferL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::FillBufferL( 
	CMMFBuffer* aBuffer,
    MDataSink* aConsumer,
    TMediaId /*aMediaId*/ )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::FillBufferL" )
	iBufferToBeFilled = static_cast<CMMFDataBuffer*>( aBuffer );
	iConsumer = aConsumer;
	}	
                  
// -----------------------------------------------------------------------------
// CMccMsrpSource::NegotiateSourceL()
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::NegotiateSourceL( MDataSink& aDataSink )
	{
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::NegotiateSourceL" )
	iFillBufferRequester = &aDataSink;
	}

// -----------------------------------------------------------------------------
// CMccMsrpSource::SendStreamEventToClient
// -----------------------------------------------------------------------------
//	
void CMccMsrpSource::SendStreamEventToClient( 
    TMccEventType aEventType, 
    TInt aError )
	{
	__MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::SendStreamEventToClient" )
	
	if ( iAsyncEventHandler )
	    {
        TMccEvent event( 0, 
                         0, 
                         0, 
                         MCC_ENDPOINT_ID( static_cast<MDataSource*>( this ) ), 
                         KMccEventCategoryStream, 
                         aEventType, 
                         aError, 
                         KNullDesC8 );
                     
        TMccInternalEvent internalEvent( KMccMsrpSourceUid, 
		                                 EMccInternalEventNone,
		                                 event );
		                         
		iAsyncEventHandler->SendEventToClient( internalEvent );
	    }
	}

// -----------------------------------------------------------------------------
// CMccMsrpSource::DataReceivedL
// -----------------------------------------------------------------------------
//	
void CMccMsrpSource::DataReceivedL( const TDesC8& aData )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSource::DataReceivedL" )
    if ( iBufferToBeFilled && iConsumer )
        {
        CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(iBufferToBeFilled);

        if (buffer->Data().MaxLength() >= aData.Length())
            {            
            buffer->Data().Copy( aData );
            iConsumer->BufferFilledL( iBufferToBeFilled );
        	iBufferToBeFilled = 0;
        	iConsumer = 0;
      		}      		
        }
    }


// -----------------------------------------------------------------------------
// CMccMsrpSink::BufferFilledL
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::SetSessionParamsL( const TMccMsrpSessionParams& aParams )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SetSessionParamsL IN" )
    
    User::LeaveIfNull( aParams.iMsrp );
    User::LeaveIfNull( aParams.iMsrpSession );
    
    iMsrp = aParams.iMsrp;
    iMsrpSession = aParams.iMsrpSession;
//    iMsrpObserver = aParams.iMsrpObserver;
    
//    iMsrpObserver->RegisterDataObserver(*this);  
    
    __MSRPSOURCESINK_CONTROLL( "CMccMsrpSink::SetSessionParamsL OUT" )
    }

// End of file

// -----------------------------------------------------------------------------
// CMccRtpDataSource::PlayoutRtpPacketL
// Pass filled buffer to the data sink of RTP data source.
// -----------------------------------------------------------------------------
//
void CMccMsrpSource::PlayoutMsrpPacketL( HBufC8* aMessageContent )
    {
    __MSRPSOURCESINK_CONTROLL( "CMccRtpDataSource::PlayoutRtpPacketL" )
    __ASSERT_ALWAYS( iBufferToBeFilled, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iFillBufferRequester, User::Leave( KErrNotReady ) );
    
    iBufferToBeFilled->Data().Copy( *aMessageContent );
    
    CPayloadFormatRead* sink = static_cast<CPayloadFormatRead*>( iFillBufferRequester );
    User::LeaveIfNull( sink );
    
    TRtpRecvHeader aHeaderInfo;
    sink->DataBufferFilledL( iBufferToBeFilled, aHeaderInfo );
    
    delete aMessageContent;
    }
