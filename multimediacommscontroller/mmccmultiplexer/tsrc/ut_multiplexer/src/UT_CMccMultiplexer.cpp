/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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




//  CLASS HEADER
#include "UT_CMccMultiplexer.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <ecom/ecom.h>

//  INTERNAL INCLUDES
#include "mccsinkitem.h"
#include "mccmultiplexer.h"
#include "mccrtpdatasource.h"
#include "mccrtpdatasink.h"
#include "Mccdldatapath.h"
#include "mccresourcepool.h"
#include "Mcctesteventhandler.h"
#include "MccFileSource.h"
#include "amrpayloadformatread.h"
#include "mccuids.hrh"
#include "mmccinterfacedef.h"
#include "mccfilesourceimpl.h"
#include "mccfileaudio.h"

#include "mccunittestmacros.h"

const TUid KRtpDataSource =  { KImplUidRtpDataSource };
const TUid KRtpDataSink =  { KImplUidRtpDataSink };
const TUid KMediaDecoder = { KImplUidAmrPayloadFormatDecode };


// CONSTRUCTION
UT_CMccMultiplexer* UT_CMccMultiplexer::NewL()
    {
    UT_CMccMultiplexer* self = UT_CMccMultiplexer::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMccMultiplexer* UT_CMccMultiplexer::NewLC()
    {
    UT_CMccMultiplexer* self = new( ELeave ) UT_CMccMultiplexer();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMccMultiplexer::~UT_CMccMultiplexer()
    {
    }

// Default constructor
UT_CMccMultiplexer::UT_CMccMultiplexer()
    {
    }

// Second phase construct
void UT_CMccMultiplexer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMccMultiplexer::SetupL(  )
    {
    iRtpDataSourceStub = CMccRtpDataSource::NewSourceL( KRtpDataSource, KNullDesC8 );
    iRead = CAmrPayloadFormatRead::NewL( iRtpDataSourceStub );
    
    TMediaId media = KUidMediaTypeAudio;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( iEventHandler, mccresource, media ); 
                                        
    iRead->FillBufferL( NULL, iDLDataPathStub, TMediaId() );  
    
    iSinkItem = CMccSinkItem::NewLC( iRead, ETrue, KNullUid, ETrue );
    CleanupStack::Pop(iSinkItem);
    iMultiplexer = CMccMultiplexer::NewL( iRtpDataSourceStub );
    }

void UT_CMccMultiplexer::Setup2L(  )
    {
    TFileName fileName;
    fileName = _L("c:\\CrazyFrog.3gp");
    TPckgBuf< TFileName > initParam( fileName );
    iFileSource = CMccFileSource::NewSourceL( KMccFileSourceUid, initParam );

    TRAPD( err, static_cast< CMccFileSource* >( iFileSource )->ConstructSourceL( initParam ) );
    if ( err == KErrNoMemory )
    	{
    	User::Leave( KErrNoMemory );
    	}
    iFileSource->SourceDataTypeCode( KUidMediaTypeVideo );
    iFileSource->SourceDataTypeCode( KUidMediaTypeAudio );
    
    iRead = CAmrPayloadFormatRead::NewL( iFileSource );
    
    TMediaId media = KUidMediaTypeVideo;
    CMccResourcePool* mccresource = NULL;
    iDLDataPathStub = CMccDlDataPath::NewL( iEventHandler, mccresource, media ); 
                                        
    iRead->FillBufferL( NULL, iDLDataPathStub, TMediaId() );  
    
    iSinkItem = CMccSinkItem::NewLC( iRead, ETrue, KUidMediaTypeVideo, ETrue );
    CleanupStack::Pop( iSinkItem );
    iMultiplexer = CMccMultiplexer::NewL( iFileSource );
    }
    
void UT_CMccMultiplexer::Teardown(  )
    {
    delete iMultiplexer;
    delete iSinkItem;
    delete iDLDataPathStub;
    delete iRead;
    delete iRtpDataSourceStub;
    iRtpDataSourceStub = NULL;
    delete iFileSource;
    iFileSource = NULL;
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_PassAllRequestsToSource()
    {
    TFileName fileName;
    fileName = _L("c:\\CrazyFrog.3gp");
    TPckgBuf<TFileName> initParam( fileName );
    
    iFileSource = static_cast< CMccFileSource* >( CMccFileSource::NewSourceL( KMccFileSourceUid, initParam ) );

    // May leave with KErrGeneral or KErrNotSupported when using alloc failure, ignore those errors
    TRAPD( err, static_cast< CMccFileSource* >( iFileSource )->ConstructSourceL( initParam ) );
    if ( err == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    CMccSinkItem* sinkItem = CMccSinkItem::NewLC( iRead, ETrue, KMccFileSourceUid, ETrue );
    CMccMultiplexer* multiplexer = CMccMultiplexer::NewL( iFileSource );
    CleanupStack::PushL( multiplexer );
    EUNIT_ASSERT( !multiplexer->PassAllRequestsToSource( *sinkItem ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( multiplexer->AddSinkL( iRead, ETrue, KMccFileSourceUid, ETrue ) );
    EUNIT_ASSERT( !multiplexer->PassAllRequestsToSource( *sinkItem ) );
    CleanupStack::PopAndDestroy( multiplexer );
    CleanupStack::PopAndDestroy( sinkItem );
    }
    
void UT_CMccMultiplexer::UT_CMccMultiplexer_AddSinkL()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ), KErrAlreadyExists );
    }
    
void UT_CMccMultiplexer::UT_CMccMultiplexer_UpdateSinkModes()
    {
    iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue );
    iMultiplexer->UpdateSinkModes();
    EUNIT_ASSERT( iMultiplexer->iResolveFilledBufferMediaType );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_FindSinkItemIndex()
    {
    EUNIT_ASSERT( iMultiplexer->FindSinkItemIndex( iRead ) == KErrNotFound );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    EUNIT_ASSERT( iMultiplexer->FindSinkItemIndex( iRead ) ==  KErrNone );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_FindSinkItemL()
    {
    CMccSinkItem* sinkItemFound = NULL;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( sinkItemFound = iMultiplexer->FindSinkItemL( iRead ), KErrNotFound );
    EUNIT_ASSERT( !sinkItemFound );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    sinkItemFound = iMultiplexer->FindSinkItemL( iRead );
    EUNIT_ASSERT( sinkItemFound );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_IsPrimarySink()
    {
    EUNIT_ASSERT( !iMultiplexer->IsPrimarySink( iRead ) );  
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    EUNIT_ASSERT( iMultiplexer->IsPrimarySink( iRead ) );  
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SendStreamEventToClient()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SetCurrentUserL( *iRead ) );
    TMccEventType mccEvent( KMccEventNone );
    
    iMultiplexer->SendStreamEventToClient( mccEvent );
    }
    
void UT_CMccMultiplexer::UT_CMccMultiplexer_SetMultiplexerState()
    {
    EUNIT_ASSERT( !iMultiplexer->SetMultiplexerState( CMccMultiplexer::ENotSet ) );
    
    EUNIT_ASSERT( iMultiplexer->SetMultiplexerState( CMccMultiplexer::EPrepared ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EPrepared );
    
    EUNIT_ASSERT( iMultiplexer->SetMultiplexerState( CMccMultiplexer::EPlaying ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EPlaying );

    EUNIT_ASSERT( iMultiplexer->SetMultiplexerState( CMccMultiplexer::EResourcePaused ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EResourcePaused );

    EUNIT_ASSERT( iMultiplexer->SetMultiplexerState( CMccMultiplexer::EPlaying ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EPlaying );

    EUNIT_ASSERT( !iMultiplexer->SetMultiplexerState( CMccMultiplexer::EPlaying ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EPlaying );
    
    // State is not "downgraded"
    EUNIT_ASSERT( !iMultiplexer->SetMultiplexerState( CMccMultiplexer::EPrepared ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EPlaying );

    // State is also set to current user
    iMultiplexer->iCurrentUser = iSinkItem;
    EUNIT_ASSERT( iMultiplexer->SetMultiplexerState( CMccMultiplexer::EResourceStopped ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EResourceStopped );
    EUNIT_ASSERT( iSinkItem->iState == CMccMultiplexer::EResourceStopped );
    
    // If no current user, state is not set to item
    iMultiplexer->iCurrentUser = NULL;
    EUNIT_ASSERT( iMultiplexer->SetMultiplexerState( CMccMultiplexer::EResourcePaused ) );
    EUNIT_ASSERT( iMultiplexer->iState == CMccMultiplexer::EResourcePaused );
    EUNIT_ASSERT( iSinkItem->iState == CMccMultiplexer::EResourceStopped ); // still stopped
    }
    
void UT_CMccMultiplexer::UT_CMccMultiplexer_DoBufferFilledL()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KMediaDecoder, ETrue ) );

    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    TRtpRecvHeader rtpHeader;
  
    CMccSinkItem* sinkItem = NULL;
    MCC_EUNIT_ASSERT_NO_LEAVE( sinkItem = iMultiplexer->FindSinkItemL( iRead ) );
    EUNIT_ASSERT( sinkItem );
    MCC_EUNIT_ASSERT_NO_LEAVE( sinkItem->FillBufferRequestL( buffer ) );
    iMultiplexer->iResolveFilledBufferMediaType = ETrue;
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->DoBufferFilledL( buffer, &rtpHeader ) );
    CleanupStack::PopAndDestroy( buffer );
    }
                          

void UT_CMccMultiplexer::UT_CMccMultiplexer_IsSupportedMediaType()
    {
    EUNIT_ASSERT( iMultiplexer->IsSupportedMediaType(KUidMediaTypeVideo) );   
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SendEventToAllClients()
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );

    TMccInternalEvent internalEvent;
    iMultiplexer->SendEventToAllClients( internalEvent );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourceThreadLogonL(  )
    {
    MAsyncEventHandler* eventHandler = NULL;
    EUNIT_ASSERT( iMultiplexer->SourceThreadLogon( *eventHandler ) == KErrNotSupported );
    MCC_EUNIT_ASSERT_EQUALS( iMultiplexer->MultiplexerSourceThreadLogon( *eventHandler, *iRead, ETrue, KMediaDecoder, ETrue ), KErrNone );
    MCC_EUNIT_ASSERT_EQUALS( iMultiplexer->MultiplexerSourceThreadLogon( *eventHandler, *iRead, ETrue, KMediaDecoder, ETrue ), KErrAlreadyExists );
    iMultiplexer->MultiplexerSourceThreadLogoff( iRead );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourceThreadLogoffL(  )
    {
    iMultiplexer->SourceThreadLogoff();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SetCurrentUserL( *iRead ) );
    
    CAmrPayloadFormatRead* read2 = CAmrPayloadFormatRead::NewL( iRtpDataSourceStub );
    CleanupStack::PushL( read2 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( read2, ETrue, KNullUid, ETrue ) );
    
    CAmrPayloadFormatRead* read3 = CAmrPayloadFormatRead::NewL( iRtpDataSourceStub );
    CleanupStack::PushL( read3 );
    MCC_EUNIT_ASSERT_NO_LEAVE(iMultiplexer->AddSinkL(read3, ETrue, KNullUid, ETrue));
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->FillBufferL( buffer, read2, TMediaId() ) );

    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( iRead ) == KErrNone );
    
    // Primary buffer is updated. As there is standing buffer request for read2,
    // multiplexer buffer is used
    EUNIT_ASSERT( iMultiplexer->iDataBuf != NULL );
    CMccRtpDataSource* dataSource = static_cast<CMccRtpDataSource*>( iRtpDataSourceStub );
    EUNIT_ASSERT( dataSource->iBufferToFill == iMultiplexer->iDataBuf );

    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( read2 ) == KErrNone );
    
    // Primary buffer is updated. As there is no standing buffer requests,
    // NULL buffer is used
    EUNIT_ASSERT( dataSource->iBufferToFill == NULL );
    
    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( read2 ) == KErrNotFound );
    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( NULL ) == KErrArgument );
    
    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( read3 ) == KErrNone );

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( read3 );
    CleanupStack::PopAndDestroy( read2 );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourceThreadLogoff2L(  )
    {
    iMultiplexer->SourceThreadLogoff();
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KUidMediaTypeVideo, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SetCurrentUserL( *iRead ) );
    
    CAmrPayloadFormatRead* read2 = CAmrPayloadFormatRead::NewL( iFileSource );
    CleanupStack::PushL( read2 );
    MCC_EUNIT_ASSERT_NO_LEAVE(iMultiplexer->AddSinkL( read2, ETrue, KUidMediaTypeVideo, ETrue ) );
    
    CAmrPayloadFormatRead* read3 = CAmrPayloadFormatRead::NewL( iFileSource );
    CleanupStack::PushL( read3 );
    MCC_EUNIT_ASSERT_NO_LEAVE(iMultiplexer->AddSinkL( read3, ETrue, KUidMediaTypeVideo, ETrue ) );
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->FillBufferL( buffer, read2, TMediaId() ) );

    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( iRead ) == KErrNone );

    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( read2 ) == KErrNone );
    
    EUNIT_ASSERT( iMultiplexer->MultiplexerSourceThreadLogoff( read3 ) == KErrNone );

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( read3 );
    CleanupStack::PopAndDestroy( read2 );
    }
    
void UT_CMccMultiplexer::UT_CMccMultiplexer_SetCurrentUserLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SetCurrentUserL( *iRead ) );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_RegClientForPayloadsLL()
    {
    RArray< TUint > payloads;
    CleanupClosePushL( payloads );
    payloads.AppendL( 96 );

    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->RegClientForPayloadsL( *iRead, payloads ) );

    CleanupStack::PopAndDestroy( &payloads );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_StreamsL(  )
    {
    EUNIT_ASSERT( iMultiplexer->Streams( KNullUid ) == 1 );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_FrameTimeIntervalL(  )
    {
    TMediaId mediaId;
    TTimeIntervalMicroSeconds frameinterval = iMultiplexer->FrameTimeInterval( mediaId );
    EUNIT_ASSERT( frameinterval.Int64() == 0 );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_DurationL(  )
    {
    TMediaId mediaId;

    TTimeIntervalMicroSeconds duration = iMultiplexer->Duration( mediaId );
    
    EUNIT_ASSERT( duration.Int64() == 0 );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_CanCreateSourceBufferL(  )
    {
    EUNIT_ASSERT( iMultiplexer->CanCreateSourceBuffer() );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_CreateSourceBufferLL(  )
    {
    CMMFBuffer* buffer = NULL;
    TMediaId mediaId; 
    TBool reference = ETrue;
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        buffer = iMultiplexer->CreateSourceBufferL( mediaId, reference), KErrNotReady );
    iMultiplexer->iSuggestedBufSize = 10;
    
    MCC_EUNIT_ASSERT_NO_LEAVE( 
        buffer = iMultiplexer->CreateSourceBufferL( mediaId, reference) );
    
    EUNIT_ASSERT( buffer );
    EUNIT_ASSERT( iMultiplexer->iSuggestedBufSize == 0 );
    delete buffer;
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SuggestSourceBufferSizeL( )
    {
    iMultiplexer->SuggestSourceBufferSize( 0 );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_DataSourceTypeL(  )
    {
    EUNIT_ASSERT( iMultiplexer->DataSourceType() == KMccMultiplexerUid );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_FillBufferLL(  )
    {
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    MCC_EUNIT_ASSERT_SPECIFIC_LEAVE( iMultiplexer->FillBufferL( NULL, NULL, TMediaId() ), KErrArgument );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iDLDataPathStub, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePlayL() );
    CMccSinkItem* sinkItemFound = NULL;
    MCC_EUNIT_ASSERT_NO_LEAVE( sinkItemFound = iMultiplexer->FindSinkItemL( iDLDataPathStub ) );
    EUNIT_ASSERT( sinkItemFound );
    sinkItemFound->SetSourceMode( ETrue );
        
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->FillBufferL( buffer, iDLDataPathStub, TMediaId() ) );

    CleanupStack::PopAndDestroy( buffer );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_FillBufferL2L(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iDLDataPathStub, ETrue, KNullUid, ETrue ) );
    
    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    CMMFDescriptorBuffer* buffer2 = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer2 );
    
    CMccResourcePool* mccresource = NULL;
    TMediaId media = KUidMediaTypeAudio;
    CMccDlDataPath* dlDataPathStub = CMccDlDataPath::NewL( iEventHandler, mccresource, media ); 
    CleanupStack::PushL( dlDataPathStub );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( dlDataPathStub, ETrue, KNullUid, ETrue ) );
    iMultiplexer->SetCurrentUserL( *dlDataPathStub );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePlayL() );
    // Primary sink is in command, secondary cannot get its fillbuffer to the source
    
    iMultiplexer->SetCurrentUserL( *iDLDataPathStub );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePlayL() );    
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->FillBufferL( buffer, iDLDataPathStub, media ) );
    CMccFileSource* filesource = static_cast<CMccFileSource*>( iFileSource );
    EUNIT_ASSERT( filesource->iSourceImpl->iFileAudio->iConsumerBuffer == buffer );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->FillBufferL( buffer2, dlDataPathStub, media ) );
    EUNIT_ASSERT( filesource->iSourceImpl->iFileAudio->iConsumerBuffer == buffer );

    // Primary sink is not in control (it's not playing, secondary sink can control the source)
    iMultiplexer->iSinkItems[ 0 ]->SetMultiplexerState( CMccMultiplexer::EPrepared );    
    iMultiplexer->SetCurrentUserL( *dlDataPathStub );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->FillBufferL( buffer2, dlDataPathStub, media ) );
    EUNIT_ASSERT( filesource->iSourceImpl->iFileAudio->iConsumerBuffer == buffer2 );
      
    CleanupStack::PopAndDestroy( dlDataPathStub );
    CleanupStack::PopAndDestroy( buffer2 );
    CleanupStack::PopAndDestroy( buffer );
    }
    
void UT_CMccMultiplexer::UT_CMccMultiplexer_SourcePrimeLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourcePlayLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePlayL() );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourcePauseLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePlayL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePauseL() );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourceStopLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePrimeL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePlayL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourcePauseL() );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SourceStopL() );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SourceDataTypeCodeL(  )
    {
   	TMediaId mediaId;
    EUNIT_ASSERT( iMultiplexer->SourceDataTypeCode( mediaId ) == TFourCC() );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SetSourceDataTypeCodeL(  )
    {
    TFourCC fourcc(' ','A','M','R' );
    TMediaId mediaId;
    EUNIT_ASSERT( iMultiplexer->SetSourceDataTypeCode( fourcc, mediaId ) == KErrNone );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_NegotiateSourceLL(  )
    {
    MDataSink* sink = NULL;
    EUNIT_ASSERT(! iMultiplexer->iSourceNegotiationDone );

    EUNIT_ASSERT_NO_LEAVE( iMultiplexer->NegotiateSourceL( *sink ) );
    EUNIT_ASSERT( iMultiplexer->iSourceNegotiationDone );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_BufferFilledLL(  )
    {
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KMediaDecoder, ETrue ) );

    CMMFDescriptorBuffer* buffer = CMMFDescriptorBuffer::NewL( 10 );
    CleanupStack::PushL( buffer );
    TRtpRecvHeader rtpHeader;
  
    CMccSinkItem* sinkItem = NULL;
    MCC_EUNIT_ASSERT_NO_LEAVE( sinkItem = iMultiplexer->FindSinkItemL( iRead ) );
    EUNIT_ASSERT( sinkItem );
    MCC_EUNIT_ASSERT_NO_LEAVE( sinkItem->FillBufferRequestL( buffer ) );
    iMultiplexer->iResolveFilledBufferMediaType = ETrue;
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->DataBufferFilledL( buffer, rtpHeader ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->BufferFilledL( buffer ) );

    CleanupStack::PopAndDestroy( buffer );    
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_ConfigurePayloadFormatLL(  )
    {
    HBufC8* buf = HBufC8::NewLC( 10 );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->ConfigurePayloadFormatL( *buf ) );
    CleanupStack::PopAndDestroy( buf );
    }

void UT_CMccMultiplexer::UT_CMccMultiplexer_SendEventToClientL(  )
    {
    TMccInternalEvent internalEvent;
     // IS_MCC_EVENT ETrue
    internalEvent.iEventType = KUidMediaTypeAudio; 
    TMccEvent mccEvent;
  
    // iMccEvent != NULL && MCC_STREAM_STATE_CHANGE_EVENT ETrue
    mccEvent.iEventCategory = KMccEventCategoryStream;
    mccEvent.iEventType = KMccStreamPrepared;
    // mccEvent.iEventNumData != KMccAutomaticEvent
    mccEvent.iEventNumData = 100;
    mccEvent.iEventData.Copy( _L8("foofoo") );
    internalEvent.iMccEvent = &mccEvent;
    //if ( !iCurrentUser )
    EUNIT_ASSERT( iMultiplexer->SendEventToClient( internalEvent ) == KErrNotReady );
    
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->AddSinkL( iRead, ETrue, KNullUid, ETrue ) );
    MCC_EUNIT_ASSERT_NO_LEAVE( iMultiplexer->SetCurrentUserL( *iRead ) );
    
    EUNIT_ASSERT( iMultiplexer->SendEventToClient( internalEvent ) == KErrNone );
    
    mccEvent.iEventCategory = KMccEventCategoryStreamControl;
    EUNIT_ASSERT( iMultiplexer->SendEventToClient( internalEvent ) == KErrNone );
    
    mccEvent.iEventCategory = KMccStandbyInactivityEvent;
    internalEvent.iEventType = KNullUid;
    EUNIT_ASSERT( iMultiplexer->SendEventToClient( internalEvent ) == KErrNone );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccMultiplexer,
    "Add test suite description here.",
    "UNIT" )
 
EUNIT_TEST(
    "PassAllRequestsToSource - test ",
    "CMccMultiplexer",
    "PassAllRequestsToSource",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_PassAllRequestsToSource, Teardown)
  
     
EUNIT_TEST(
    "AddSinkL - test ",
    "CMccMultiplexer",
    "AddSinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_AddSinkL, Teardown)

EUNIT_TEST(
    "UpdateSinkModes - test ",
    "CMccMultiplexer",
    "UpdateSinkModes",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_UpdateSinkModes, Teardown)
        
        
EUNIT_TEST(
    "FindSinkItemIndex - test ",
    "CMccMultiplexer",
    "FindSinkItemIndex",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_FindSinkItemIndex, Teardown)


EUNIT_TEST(
    "FindSinkItemL - test ",
    "CMccMultiplexer",
    "FindSinkItemL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_FindSinkItemL, Teardown)
    
EUNIT_TEST(
    "IsPrimarySink - test ",
    "CMccMultiplexer",
    "IsPrimarySink",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_IsPrimarySink, Teardown)
    
EUNIT_TEST(
    "SendStreamEventToClient - test ",
    "CMccMultiplexer",
    "SendStreamEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SendStreamEventToClient, Teardown)
          
  
EUNIT_TEST(
    "SetMultiplexerState - test ",
    "CMccMultiplexer",
    "SetMultiplexerState",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SetMultiplexerState, Teardown)
        
EUNIT_TEST(
    "IsSupportedMediaType - test ",
    "CMccMultiplexer",
    "IsSupportedMediaType",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_IsSupportedMediaType, Teardown)
              
EUNIT_TEST(
    "DoBufferFilledL - test ",
    "CMccMultiplexer",
    "DoBufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_DoBufferFilledL, Teardown)
      


EUNIT_TEST(
    "SendEventToAllClients - test ",
    "CMccMultiplexer",
    "SendEventToAllClients",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SendEventToAllClients, Teardown)
                   
EUNIT_TEST(
    "SourceThreadLogon - test ",
    "CMccMultiplexer",
    "SourceThreadLogon",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourceThreadLogonL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff - test ",
    "CMccMultiplexer",
    "SourceThreadLogoff",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourceThreadLogoffL, Teardown)

EUNIT_TEST(
    "SourceThreadLogoff2 - test ",
    "CMccMultiplexer",
    "SourceThreadLogoff2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccMultiplexer_SourceThreadLogoff2L, Teardown)
    
EUNIT_TEST(
    "SetCurrentUserL - test ",
    "CMccMultiplexer",
    "SetCurrentUserL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SetCurrentUserLL, Teardown)

EUNIT_TEST(
    "RegClientForPayloadsL - test ",
    "CMccMultiplexer",
    "RegClientForPayloadsL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_RegClientForPayloadsLL, Teardown)

EUNIT_TEST(
    "Streams - test ",
    "CMccMultiplexer",
    "Streams",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_StreamsL, Teardown)

EUNIT_TEST(
    "FrameTimeInterval - test ",
    "CMccMultiplexer",
    "FrameTimeInterval",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_FrameTimeIntervalL, Teardown)

EUNIT_TEST(
    "Duration - test ",
    "CMccMultiplexer",
    "Duration",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_DurationL, Teardown)

EUNIT_TEST(
    "CanCreateSourceBuffer - test ",
    "CMccMultiplexer",
    "CanCreateSourceBuffer",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_CanCreateSourceBufferL, Teardown)

EUNIT_TEST(
    "CreateSourceBufferL - test ",
    "CMccMultiplexer",
    "CreateSourceBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_CreateSourceBufferLL, Teardown)

EUNIT_TEST(
    "SuggestSourceBufferSize - test ",
    "CMccMultiplexer",
    "SuggestSourceBufferSize",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SuggestSourceBufferSizeL, Teardown)

EUNIT_TEST(
    "DataSourceType - test ",
    "CMccMultiplexer",
    "DataSourceType",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_DataSourceTypeL, Teardown)

EUNIT_TEST(
    "FillBufferL - test ",
    "CMccMultiplexer",
    "FillBufferL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_FillBufferLL, Teardown)

EUNIT_TEST(
    "FillBufferL 2 - test ",
    "CMccMultiplexer",
    "FillBufferL 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccMultiplexer_FillBufferL2L, Teardown)
    
EUNIT_TEST(
    "SourcePrimeL - test ",
    "CMccMultiplexer",
    "SourcePrimeL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourcePrimeLL, Teardown)

EUNIT_TEST(
    "SourcePlayL - test ",
    "CMccMultiplexer",
    "SourcePlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourcePlayLL, Teardown)

EUNIT_TEST(
    "SourcePauseL - test ",
    "CMccMultiplexer",
    "SourcePauseL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourcePauseLL, Teardown)

EUNIT_TEST(
    "SourceStopL - test ",
    "CMccMultiplexer",
    "SourceStopL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourceStopLL, Teardown)

EUNIT_TEST(
    "SourceDataTypeCode - test ",
    "CMccMultiplexer",
    "SourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "SetSourceDataTypeCode - test ",
    "CMccMultiplexer",
    "SetSourceDataTypeCode",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SetSourceDataTypeCodeL, Teardown)

EUNIT_TEST(
    "NegotiateSourceL - test ",
    "CMccMultiplexer",
    "NegotiateSourceL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_NegotiateSourceLL, Teardown)

EUNIT_TEST(
    "BufferFilledL - test ",
    "CMccMultiplexer",
    "BufferFilledL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_BufferFilledLL, Teardown)

EUNIT_TEST(
    "ConfigurePayloadFormatL - test ",
    "CMccMultiplexer",
    "ConfigurePayloadFormatL",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_ConfigurePayloadFormatLL, Teardown)

EUNIT_TEST(
    "SendEventToClient - test ",
    "CMccMultiplexer",
    "SendEventToClient",
    "FUNCTIONALITY",
    SetupL, UT_CMccMultiplexer_SendEventToClientL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
