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
* Description:    
*
*/




//  CLASS HEADER
#include "UT_tmcesrvstreamiterator.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/EUnitMacros.h>
#include <sdpdocument.h>
#include "mcemediamanager.h"
#include "mcesdpsession.h"
#include "mcesrvsource.h"
#include "mcesrvsink.h"
#define RESET()\
    iServer->Reset()
    
#include "mcesrvstreamiterator.h"


//  INTERNAL INCLUDES

// CONSTRUCTION
UT_TMceSrvStreamIterator* UT_TMceSrvStreamIterator::NewL()
    {
    UT_TMceSrvStreamIterator* self = UT_TMceSrvStreamIterator::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_TMceSrvStreamIterator* UT_TMceSrvStreamIterator::NewLC()
    {
    UT_TMceSrvStreamIterator* self = new( ELeave ) UT_TMceSrvStreamIterator();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_TMceSrvStreamIterator::~UT_TMceSrvStreamIterator()
    {
    }

// Default constructor
UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator()
    {
    }

// Second phase construct
void UT_TMceSrvStreamIterator::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS


void UT_TMceSrvStreamIterator::SetupL()
    {
    iServer = new (ELeave) CMceServerStub();
    iManager = CMceMediaManager::NewL( *iServer );
    iSession = MceMediaManagerTestHelper::CreateVideoOutSessionL();
    iUpdate = iSession->CloneL();
    } 

void UT_TMceSrvStreamIterator::Teardown()
    {
	CMceComSession::Delete( iUpdate, *iManager );
	CMceComSession::Delete( iSession, *iManager );
	delete iManager;
	iManager = NULL;
	delete iServer;
	iServer = NULL;
    }


void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_BasicL()
    { 
    
    iSession->PrepareL( *iManager );
    
    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.iDirection == TMceSrvStreamIterator::EZero );
    EUNIT_ASSERT( iter.iStream == NULL );
    EUNIT_ASSERT( iter.iEndpoint == NULL );
    EUNIT_ASSERT( iter.iCurrentIndex == 0 );

    TMceSrvStreamIterator iter2( iSession->MccStreams(), TMceSrvStreamIterator::EZero );
    EUNIT_ASSERT( iter2.iDirection == TMceSrvStreamIterator::EZero );
    EUNIT_ASSERT( iter2.iStream == NULL );
    EUNIT_ASSERT( iter2.iEndpoint == NULL );
    EUNIT_ASSERT( iter2.iCurrentIndex == 0 );
    

    TMceSrvStreamIterator iter3( iSession->MccStreams(), TMceSrvStreamIterator::EAny );
    EUNIT_ASSERT( iter3.iDirection == TMceSrvStreamIterator::EAny );
    EUNIT_ASSERT( iter3.iStream == NULL );
    EUNIT_ASSERT( iter3.iEndpoint == NULL );
    EUNIT_ASSERT( iter3.iCurrentIndex == 0 );

    TMceSrvStreamIterator iter4( iSession->MccStreams(), TMceSrvStreamIterator::ESendOrReceive );
    EUNIT_ASSERT( iter4.iDirection == TMceSrvStreamIterator::ESendOrReceive );
    EUNIT_ASSERT( iter4.iStream == NULL );
    EUNIT_ASSERT( iter4.iEndpoint == NULL );
    EUNIT_ASSERT( iter4.iCurrentIndex == 0 );

    TMceSrvStreamIterator iter5( iSession->MccStreams(), TMceSrvStreamIterator::ESend );
    EUNIT_ASSERT( iter5.iDirection == TMceSrvStreamIterator::ESend );
    EUNIT_ASSERT( iter5.iStream == NULL );
    EUNIT_ASSERT( iter5.iEndpoint == NULL );
    EUNIT_ASSERT( iter5.iCurrentIndex == 0 );

    TMceSrvStreamIterator iter6( iSession->MccStreams(), TMceSrvStreamIterator::EReceive );
    EUNIT_ASSERT( iter6.iDirection == TMceSrvStreamIterator::EReceive );
    EUNIT_ASSERT( iter6.iStream == NULL );
    EUNIT_ASSERT( iter6.iEndpoint == NULL );
    EUNIT_ASSERT( iter6.iCurrentIndex == 0 );

    TMceSrvStreamIterator iter7( iSession->MccStreams(), TMceSrvStreamIterator::ESendOrLocal );
    EUNIT_ASSERT( iter7.iDirection == TMceSrvStreamIterator::ESendOrLocal );
    EUNIT_ASSERT( iter7.iStream == NULL );
    EUNIT_ASSERT( iter7.iEndpoint == NULL );
    EUNIT_ASSERT( iter7.iCurrentIndex == 0 );

    TMceSrvStreamIterator iter8( iSession->MccStreams(), TMceSrvStreamIterator::ELocal );
    EUNIT_ASSERT( iter8.iDirection == TMceSrvStreamIterator::ELocal );
    EUNIT_ASSERT( iter8.iStream == NULL );
    EUNIT_ASSERT( iter8.iEndpoint == NULL );
    EUNIT_ASSERT( iter8.iCurrentIndex == 0 );

    CMceComMediaStream& stream = *iSession->Streams()[0];
    TMceSrvStreamIterator iter9( iSession->MccStreams(), stream );
    EUNIT_ASSERT( iter9.iDirection == TMceSrvStreamIterator::EZero );
    EUNIT_ASSERT( iter9.iStream == &stream );
    EUNIT_ASSERT( iter9.iEndpoint == NULL );
    EUNIT_ASSERT( iter9.iCurrentIndex == 0 );

    CMceComMediaSource& source = *stream.Source();
    TMceSrvStreamIterator iter10( iSession->MccStreams(), source );
    EUNIT_ASSERT( iter10.iDirection == TMceSrvStreamIterator::EZero );
    EUNIT_ASSERT( iter10.iStream == NULL );
    EUNIT_ASSERT( iter10.iEndpoint == &source );
    EUNIT_ASSERT( iter10.iCurrentIndex == 0 );

    CMceComMediaSink& sink = *stream.Sinks()[0];
    TMceSrvStreamIterator iter11( iSession->MccStreams(), sink );
    EUNIT_ASSERT( iter11.iDirection == TMceSrvStreamIterator::EZero );
    EUNIT_ASSERT( iter11.iStream == NULL );
    EUNIT_ASSERT( iter11.iEndpoint == &sink );
    EUNIT_ASSERT( iter11.iCurrentIndex == 0 );



    }

//
// Tests
//
// CMceSrvStream* Next( TStreamStateMatchType aMatchType = ExactMatch )
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_Next_1L()
    {

    CMceComMediaStream& stream = *iSession->Streams()[0];
    CMceComMediaSource& source = *stream.Source();
    CMceComMediaSink& sink = *stream.Sinks()[0];
    MCE_DEFINE_VIDEO( videostream, stream );
    CMceComVideoCodec* codec = videostream.Codecs()[0];
    

    CMceSrvStream* s = NULL;
    iSession->PrepareL( *iManager );

    //no direction    
    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.Next( s ) );
    
    iter.Reset();
    EUNIT_ASSERT( !iter.Next( s, TMceSrvStreamIterator::ExactReverseMatch ) );
    iter.Reset();
    EUNIT_ASSERT( !iter.Next( s, TMceSrvStreamIterator::GreaterOrEqualMatch ) );
    iter.Reset();
    EUNIT_ASSERT( !iter.Next( s, TMceSrvStreamIterator::GreaterOrEqualReverseMatch ) );

    for (TInt i = 0;i<3;i++ )
        {
        if ( i == 1 )
            {
            EUNIT_ASSERT( stream.iStreamType == CMceComMediaStream::EReceiveStream );
            stream.iStreamType = CMceComMediaStream::EReceiveOnlyStream;
            }
        else if ( i == 2 )
            {
            stream.iStreamType = CMceComMediaStream::EReceiveStream;
            EUNIT_ASSERT( stream.BoundStreamL().iStreamType == CMceComMediaStream::ESendStream );
            stream.BoundStreamL().iStreamType = CMceComMediaStream::ESendOnlyStream;
            }
        iter.Reset();
        iter.iDirection = TMceSrvStreamIterator::ELocal;
        EUNIT_ASSERT( iter.Next( s ) );
        iter.Reset();
        iter.iDirection = TMceSrvStreamIterator::ESendOrReceive;
        EUNIT_ASSERT( iter.Next( s ) );
        iter.Reset();
        iter.iDirection = TMceSrvStreamIterator::ESend;
        EUNIT_ASSERT( iter.Next( s ) );
        iter.Reset();
        iter.iDirection = TMceSrvStreamIterator::EReceive;
        EUNIT_ASSERT( iter.Next( s ) );
        iter.Reset();
        iter.iDirection = TMceSrvStreamIterator::ESendOrLocal;
        EUNIT_ASSERT( iter.Next( s ) );
        iter.Reset();
        iter.iDirection = TMceSrvStreamIterator::EAny;
        EUNIT_ASSERT( iter.Next( s ) );
        
        }

    iter.Reset();
    iter.iDirection = TMceSrvStreamIterator::EReceive;
    stream.iStreamType = CMceComMediaStream::ESendOnlyStream;
    EUNIT_ASSERT( !iter.Next( s ) );
        
            
    stream.iStreamType = CMceComMediaStream::EReceiveStream;
    stream.BoundStreamL().iStreamType = CMceComMediaStream::ESendStream;
    iter.iDirection = TMceSrvStreamIterator::EZero;
    
    iter.Reset();
    iter.iStream = &stream;
    EUNIT_ASSERT( iter.Next( s ) )

    iter.Reset();
    iter.iEndpoint = &source;
    EUNIT_ASSERT( iter.Next( s ) );
    
    iter.Reset();
    iter.iEndpoint = &sink;
    EUNIT_ASSERT( iter.Next( s ) );
    
    iter.Reset();
    iter.iEndpoint = &sink;
    iter.iDirection = TMceSrvStreamIterator::ELocal;
    stream.iStreamType = CMceComMediaStream::ELocalStream;
    EUNIT_ASSERT( iter.Next( s ) );

    TMceSrvStreamIterator iter1( iSession->MccStreams(), *codec );
    iter1.iEndpoint = &source;
    EUNIT_ASSERT( iter1.Next( s ) );
    
    
    
    }


//
// Tests
//
// CMceSrvStream* Next( TMceMccComEvent& aRecipient,
//                     TStreamStateMatchType aMatchType = ExactMatch )
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_Next_2L()
    {
    CMceSrvStream* s = NULL;

    TMceMccComEvent event( KMccStreamPrepared );
    iSession->PrepareL( *iManager );

    //no direction    
    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.Next( s, event ) );

    event.iStreamId = 345;
    EUNIT_ASSERT( !iter.Next( s, event ) )

    EUNIT_ASSERT( !iter.Next( s, event ) );
    
    
        
    
    }

//
// Tests
//
// CMceSrvStream* Next( CMceSrvStream& aClone,
//                     TStreamStateMatchType aMatchType = ExactMatch )
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_Next_3L()
    {
    CMceSrvStream* s = NULL;
    CMceSrvStream* dummy = NULL;
    iSession->PrepareL( *iManager );

    //no direction    
    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.Next( s, *iSession->MccStreams()[0] ) );
    EUNIT_ASSERT( !iter.Next( s, *dummy ) )
    EUNIT_ASSERT( !iter.Next( s, *dummy ) )
    
    
    }

//
// Tests
//
// CMceSrvStream* Next( CMceSrvStream::TState aState, 
//                     TStreamStateMatchType aMatchType = ExactMatch )
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_Next_4L()
    {

    CMceSrvStream* s = NULL;
    
    iSession->PrepareL( *iManager );
    CMceSrvStream* stream = iSession->MccStreams()[0];
    TMceSrvStreamIterator iter( iSession->MccStreams() );

    stream->Source().iState = CMceSrvStream::EPrepared;
    stream->Sink().iState = CMceSrvStream::EPrepared;
    
    EUNIT_ASSERT( iter.Next( s, CMceSrvStream::EPrepared, TMceSrvStreamIterator::ExactMatch ) );
    EUNIT_ASSERT( !iter.Next( s, CMceSrvStream::EPrepared, TMceSrvStreamIterator::ExactMatch ) );
    
    iter.Reset();
    EUNIT_ASSERT( iter.Next( s, CMceSrvStream::EPrepared, TMceSrvStreamIterator::ExactReverseMatch ) );
    EUNIT_ASSERT( iter.Next( s, CMceSrvStream::EPrepared, TMceSrvStreamIterator::ExactReverseMatch ) );

    iter.Reset();
    EUNIT_ASSERT( iter.Next( s, CMceSrvStream::ECreated, TMceSrvStreamIterator::GreaterOrEqualMatch ) );
    EUNIT_ASSERT( iter.Next( s, CMceSrvStream::ECreated, TMceSrvStreamIterator::GreaterOrEqualMatch ) );

    iter.Reset();
    EUNIT_ASSERT( !iter.Next( s, CMceSrvStream::ECreated, TMceSrvStreamIterator::GreaterOrEqualReverseMatch ) );
    
    
    }


//
// Tests
//
// CMceSrvStream* NextIntersect( TMceSrvStreamIterator& aSet,
//                              CMceSrvStream*& aIntersectCandidate )
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_NextIntersectL()
    {

    CMceSrvStream* s = NULL;

    iSession->PrepareL( *iManager );
    iUpdate->PrepareL( *iManager );
    
    CMceSrvStream* update = NULL;
    CMceSrvStream* stream = NULL;
    
    TMceSrvStreamIterator iter( iUpdate->MccStreams() );
    TMceSrvStreamIterator iter2( iSession->MccStreams() );

    for( TInt i=0;i<iUpdate->MccStreams().Count();i++ )
        {
        EUNIT_ASSERT( iter.NextIntersect( update, iter2, stream ) );
        EUNIT_ASSERT( update != NULL );
        EUNIT_ASSERT( stream != NULL );
        EUNIT_ASSERT( update->IsEqual( *stream ) );
        EUNIT_ASSERT( update == iUpdate->MccStreams()[i] );
        EUNIT_ASSERT( stream == iSession->MccStreams()[i] );
        }

    EUNIT_ASSERT( !iter.NextIntersect( s, iter2, stream ) );
    EUNIT_ASSERT( stream == NULL );
    
    CMceComMediaStream* newStream = MceMediaManagerTestHelper::CreateVideoDownLinkL();
    CleanupStack::PushL( newStream );
    iUpdate->AddStreamL( newStream );
    CleanupStack::Pop( newStream );
    iUpdate->InitializeL();
    iUpdate->PrepareL( *iManager );

    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 4 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 3 );
    
    iter.Reset();
    for( TInt i=0;i<iSession->MccStreams().Count();i++ )
        {
        EUNIT_ASSERT( iter.NextIntersect( update, iter2, stream ) );
        EUNIT_ASSERT( update != NULL );
        EUNIT_ASSERT( stream != NULL );
        EUNIT_ASSERT( update->IsEqual( *stream ) );
        EUNIT_ASSERT( update == iUpdate->MccStreams()[i] );
        EUNIT_ASSERT( stream == iSession->MccStreams()[i] );
        }
        
    EUNIT_ASSERT( !iter.NextIntersect( s, iter2, stream ) );
    EUNIT_ASSERT( stream == NULL );
    EUNIT_ASSERT( !iter.NextIntersect( s, iter2, stream ) );
    EUNIT_ASSERT( stream == NULL );
    
    
    }

//
// Tests
//
// CMceSrvStream* NextComplement( TMceSrvStreamIterator& aSet )
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_NextComplementL()
    {

    CMceSrvStream* s = NULL;

    iSession->PrepareL( *iManager );
    iUpdate->PrepareL( *iManager );
    
    CMceSrvStream* update = NULL;
    CMceSrvStream* stream = NULL;
    
    TMceSrvStreamIterator iter( iUpdate->MccStreams() );
    TMceSrvStreamIterator iter2( iSession->MccStreams() );

    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 3 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 3 );

    EUNIT_ASSERT( !iter.NextComplement( s, iter2 ) );
    
    CMceComMediaStream* newStream = MceMediaManagerTestHelper::CreateVideoDownLinkL();
    CleanupStack::PushL( newStream );
    iUpdate->AddStreamL( newStream );
    CleanupStack::Pop( newStream );
    iUpdate->InitializeL();
    iUpdate->PrepareL( *iManager );

    EUNIT_ASSERT( iUpdate->MccStreams().Count() == 4 );
    EUNIT_ASSERT( iSession->MccStreams().Count() == 3 );
    
    iter.Reset();
    EUNIT_ASSERT( iter.NextComplement( s, iter2 ) );
    EUNIT_ASSERT( s == iUpdate->MccStreams()[3] );
    
    }
                              
//
// Tests
//
// void UT_TMceSrvStreamIterator::Reset()
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_ResetL()
    {

    CMceSrvStream* s = NULL;

    iSession->PrepareL( *iManager );
    
    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.iCurrentIndex == 0 );
    iter.Next( s );
    EUNIT_ASSERT( iter.iCurrentIndex == 1 );
    iter.Reset();
    EUNIT_ASSERT( iter.iCurrentIndex == 0 );
    
    
    
    }

//
// Tests
//
// TInt Current()
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_CurrentL()
    {
    
    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.Current() == KErrNotFound );
    
    iSession->PrepareL( *iManager );
    TMceSrvStreamIterator iter2( iSession->MccStreams() );
    EUNIT_ASSERT( iter2.Current() == 0 );

    TMceSrvStreamIterator iter3( iSession->MccStreams() );
    iter3.iCurrentIndex = iSession->MccStreams().Count();
    EUNIT_ASSERT( iter3.Current() == iSession->MccStreams().Count() - 1 );
    
    }


//
// Tests
//
// TBool IsEof()
//
///
void UT_TMceSrvStreamIterator::UT_TMceSrvStreamIterator_IsEofL()
    {

    CMceSrvStream* s = NULL;

    TMceSrvStreamIterator iter( iSession->MccStreams() );
    EUNIT_ASSERT( iter.iCurrentIndex == 0 );
    EUNIT_ASSERT( iter.iStreams.Count() == 0 );
    EUNIT_ASSERT( iter.IsEof() );
    
    iSession->PrepareL( *iManager );
    EUNIT_ASSERT( iter.iStreams.Count() > 0 );
    EUNIT_ASSERT( !iter.IsEof() );
    iter.Next( s );
    EUNIT_ASSERT( !iter.IsEof() );
    iter.Next( s );
    EUNIT_ASSERT( !iter.IsEof() );
    iter.Next( s );
    EUNIT_ASSERT( iter.IsEof() );
    iter.Next( s );
    EUNIT_ASSERT( iter.IsEof() );
    
    }

	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_TMceSrvStreamIterator,
    "TMceSrvStreamIterator tests",
    "UNIT" )


EUNIT_TEST(
    "Basic test",
    "TMceSrvStreamIterator",
    "<basic>",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_BasicL, Teardown)

EUNIT_TEST(
    "Next (plain) - test",
    "TMceSrvStreamIterator",
    "Next",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_Next_1L, Teardown)

EUNIT_TEST(
    "Next (event) - test",
    "TMceSrvStreamIterator",
    "Next",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_Next_2L, Teardown)

EUNIT_TEST(
    "Next (clone) - test",
    "TMceSrvStreamIterator",
    "Next",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_Next_3L, Teardown)

EUNIT_TEST(
    "Next (state) - test",
    "TMceSrvStreamIterator",
    "Next",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_Next_4L, Teardown)

EUNIT_TEST(
    "NextIntersect - test",
    "TMceSrvStreamIterator",
    "NextIntersect",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_NextIntersectL, Teardown)

EUNIT_TEST(
    "NextComplement - test",
    "TMceSrvStreamIterator",
    "NextComplement",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_NextComplementL, Teardown)

EUNIT_TEST(
    "Reset - test",
    "TMceSrvStreamIterator",
    "Reset",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_ResetL, Teardown)

EUNIT_TEST(
    "Current - test",
    "TMceSrvStreamIterator",
    "Current",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_CurrentL, Teardown)

EUNIT_TEST(
    "IsEof - test",
    "TMceSrvStreamIterator",
    "IsEof",
    "FUNCTIONALITY",
    SetupL, UT_TMceSrvStreamIterator_IsEofL, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE
