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
#include "ut_cmcecssession.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "mcecssession.h"
#include "mcesipsession.h"
#include "mcesipmanager.h"
#include "mcecssessionimplementation.h"
#include "mceservercore.h"
#include "mcecsserveritc.h"
#include "mcecssessionreceiver.h"
#include "cmcetls.h"
#include "MCETestHelper.h"
#include "mcecomfilesource.h"
#include "mcecomcamerasource.h"


const TUint K200_OK = 200;
const TUint32 KIapId1 = 1;
const TUint32 KIapId2 = 2;

_LIT8( KReceiveMessage, "received");


// CONSTRUCTION
UT_CMceCsSession* UT_CMceCsSession::NewL()
    {
    UT_CMceCsSession* self = UT_CMceCsSession::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMceCsSession* UT_CMceCsSession::NewLC()
    {
    UT_CMceCsSession* self = new( ELeave ) UT_CMceCsSession();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMceCsSession::~UT_CMceCsSession()
    {
    }

// Default constructor
UT_CMceCsSession::UT_CMceCsSession()
    {
    }

// Second phase construct
void UT_CMceCsSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMceCsSession::SetupL()
    {
    CMCETls::OpenL();
	iServerCore = CMceServerCore::NewL();
    iSession = CMceCsSession::NewL( *iServerCore );
    EUNIT_ASSERT(iServerCore == &iSession->ServerCore());
    TUid clientid = iSession->Uid();
    CMceCsServerITC& itc = iSession->ITC();
    iStorage = CMCETls::Storage();
    } 

void UT_CMceCsSession::Teardown()
    {
    CMCETls::Close();
    delete iSession;
    delete iServerCore;
    }

void UT_CMceCsSession::UT_CMceCsSession_TestOpenSubSessionL()
    {
    TMceIds ids1;
    TMceIds ids2;
    ids1.iProfileID = 1;
    ids1.iManagerType = KMceCSSIPSession;
    ids1.iDialogType = KMceDlgTypeInvite;

    ids2 = ids1;
    
	CMceCsSubSession* sub1=0;
	CMceCsSubSession* sub2=0;
	TInt handle = 0;
    TInt err = KErrNone;
    EUNIT_ASSERT_EQUALS (0, iSession->Implementation().SubSessions().Count());
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( ids1, *msgParams );

    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );

    
    // Open a new subsession 
    iSession->CreateSubSessionL(ids1, RMessage2());
    EUNIT_ASSERT_EQUALS (1, iSession->Implementation().SubSessions().Count());
	sub1 = static_cast<CMceCsSubSession*>((iSession->Implementation().SubSessions())[0]);
	EUNIT_ASSERT(ids1.iSessionID == (TUint32) sub1);



	params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user14@host>") );
    
    msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( ids2, *msgParams );

    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
	
    // Open a second subsession with different IAP
    iSession->CreateSubSessionL(ids2,RMessage2());
    EUNIT_ASSERT_EQUALS (2, iSession->Implementation().SubSessions().Count());
	sub2 = static_cast<CMceCsSubSession*>((iSession->Implementation().SubSessions())[1]);
	EUNIT_ASSERT(ids2.iSessionID == (TUint32) sub2);


	// Close the first subsession
	iStorage->iValue = ids1.iSessionID;
	
    iSession->DeleteSubSessionL(ids1, RMessage2());
    EUNIT_ASSERT_EQUALS (1, iSession->Implementation().SubSessions().Count());
	sub1 = static_cast<CMceCsSubSession*>((iSession->Implementation().SubSessions())[0]);
    EUNIT_ASSERT (sub1 == sub2);

	// Close the second subsession
	iStorage->iValue = ids2.iSessionID;
    iSession->DeleteSubSessionL(ids2, RMessage2());
    EUNIT_ASSERT_EQUALS (0, iSession->Implementation().SubSessions().Count());

	params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user15@host>") );
    
    msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( ids2, *msgParams );

    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );

	// Open a new subsession, but do not close it, 
	// checking that CMceCsSession's destructor handles this correctly 
    iSession->CreateSubSessionL(ids1,RMessage2());
    EUNIT_ASSERT_EQUALS (1, iSession->Implementation().SubSessions().Count());
	sub1 = static_cast<CMceCsSubSession*>((iSession->Implementation().SubSessions())[0]);

// ---- gracefull termination testing
    CMceSipSession* sub3 = NULL;
    for( int i = 0; i < 6;i++ )
        {

        iStorage->iSipSentMethod = SIPStrings::StringF( SipStrConsts::EEmpty );
        
    	params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
    	CleanupStack::PushL( params );
    	params->AppendL( _L8("User <user2@host>") );
    	params->AppendL( _L8("User <user14@host>") );
        
        msgParams = new (ELeave) CMceMsgTextArray( *params );
        CleanupStack::PushL( msgParams );
        iStorage->StoreL( ids2, *msgParams );

        CleanupStack::PopAndDestroy( msgParams );
        CleanupStack::PopAndDestroy( params );
    	
        iSession->CreateSubSessionL( ids2, RMessage2());
        
    	sub3 = static_cast<CMceSipSession*>((iSession->Implementation().SubSessions())[1]);
        EUNIT_ASSERT_EQUALS (2, iSession->Implementation().SubSessions().Count());
    	
    	if ( i == 0 )
    	    {
    	    sub3->NextState( KMceStateClientEstablishing );
    	    sub3->CurrentState( ETrue );
    	    sub3->NextState( KMceStateOffering );
    	    }
    	else if ( i == 1 )
    	    {
    	    sub3->NextState( KMceStateOffering );
    	    sub3->CurrentState( ETrue );
    	    sub3->NextState( KMceStateEstablished );
    	    }
        else if ( i == 2 )
            {
            delete sub3->iBody;
            sub3->iBody = NULL;
            sub3->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    	    sub3->NextState( KMceStateEstablished );
    	    sub3->CurrentState( ETrue );
    	    sub3->NextState( KMceStateError );
            }
        else if ( i == 3 )
            {
            delete sub3->iBody;
            sub3->iBody = NULL;
            sub3->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
            sub3->SetSubState( CMceSipSession::EOffering );
    	    sub3->NextState( KMceStateClientEstablishing );
    	    sub3->CurrentState( ETrue );
    	    sub3->NextState( KMceStateError );
            }
        else if ( i == 4 )
            {
            delete sub3->iBody;
            sub3->iBody = NULL;
            sub3->iBody = CMceComSession::NewL( CMceComSession::EOutSession );
    	    sub3->NextState( KMceStateClientEstablishing );
    	    sub3->CurrentState( ETrue );
    	    sub3->NextState( KMceStateOffering );
            }
        else if ( i == 5 )
            {
    	    sub3->NextState( KMceStateIdle );
    	    sub3->CurrentState( ETrue );
    	    sub3->NextState( KMceStateClientEstablishing );
            }
            
    	
    	    
    	iStorage->iValue = ids2.iSessionID;    	
        iSession->DeleteSubSessionL(ids2, RMessage2() );

        if ( i < 3 )
            {
            MCE_DELETE_SUBSESSION_POSTCODITION( iSession, sub3 )
            }
        
        EUNIT_ASSERT_EQUALS (1, iSession->Implementation().SubSessions().Count());
        
        if ( i == 0 )
            {
            EUNIT_ASSERT (iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
            EUNIT_ASSERT_EQUALS (1, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
            }
    	else if ( i == 1 )
    	    {
            EUNIT_ASSERT (iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EBye ) );
            EUNIT_ASSERT_EQUALS (2, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
    	    }
        else if ( i == 2 )
            {
            EUNIT_ASSERT (iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EBye ) );
            EUNIT_ASSERT_EQUALS (2, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
            }
        else if ( i == 3 )
            {
            EUNIT_ASSERT (iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
            EUNIT_ASSERT_EQUALS (1, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
            }
        else if ( i == 4 )
            {
            EUNIT_ASSERT (iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
            EUNIT_ASSERT_EQUALS (2, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
            }
        else if ( i == 5 )
            {
            EUNIT_ASSERT (iStorage->iSipSentMethod == SIPStrings::StringF( SipStrConsts::EEmpty ) );
            EUNIT_ASSERT_EQUALS (3, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
            }
        }

	
    EUNIT_ASSERT_EQUALS (3, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
	sub3 = static_cast<CMceSipSession*>(iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions()[0]);
    sub3->NextState( KMceStateEstablished );
    iSession->ServerCore().Manager().Cleanup();
    MCE_CHECK_MEMORY_LEAVE( iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() != 3 )
    EUNIT_ASSERT_EQUALS (3, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
    
	sub3 = static_cast<CMceSipSession*>(iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions()[2]);
    sub3->NextState( KMceStateTerminated );
    iSession->ServerCore().Manager().Cleanup();
    EUNIT_ASSERT_EQUALS (1, iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions().Count() );
    
    //callin' orphan (offering)
    TMceIds ids;
    ids.iProfileID = 1;
    ids.iManagerType = KMceCSSIPSession;
    ids.iDialogType = KMceDlgTypeInvite;
    
    EUNIT_ASSERT_LEAVE( iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions()[0]->
            ServiceL( ids, EMceItcCancel, RMessage2() ) );
    
	sub3 = static_cast<CMceSipSession*>(iSession->ServerCore().Manager().iOrphanSubSessions->SubSessions()[0]);
	sub3->SetSubState( CMceSipSession::EOffering );
	
	EUNIT_ASSERT( sub3->IsOrphan() );
	
    CSIPClientTransaction* clitransaction = 
        MCETestHelper::ClientTransactionLC( SipStrConsts::EInvite, KMceSipDecline, 
                                            SipStrConsts::EPhraseDecline );
                                            
	sub3->SetPendingTransactionL( clitransaction );
    CleanupStack::Pop( clitransaction );
	
	sub3->DialogResponseReceived( *clitransaction );
	
	
    static_cast<CMceSipSession*>( sub1 )->NextState( KMceStateOffering );
	delete iSession; // Cleans up sub3
	iSession = NULL;
    }

void UT_CMceCsSession::UT_CMceCsSession_ServiceLL(  )
    {
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( TMceIds(), *msgParams );

    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    iStorage->iIds.iAppUID = 56789;    
    iStorage->iIds.iProfileID = 1;
    iStorage->iIds.iManagerType = KMceCSSIPSession;
    iStorage->iIds.iDialogType = KMceDlgTypeInvite;
    
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcSetAppUid;
    iSession->ServiceL(RMessage2());
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	EUNIT_ASSERT( iSession->Uid().iUid == 56789 );

    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcClientReadyToReceive;
    // May leave with KErrCompletion when running out of memory
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == -1 );
            
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcClientCancelReceive;
    // May leave with KErrCompletion when running out of memory
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );

    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcCreateSession;
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	
    iStorage->iValue = iStorage->iIds.iSessionID;
    iStorage->iIds.iSessionID = KMceNotAssigned;
    
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcDeleteSession;
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
    
    HBufC8* message = KReceiveMessage().AllocLC();
    iSession->ReceiverL( iStorage->iIds )->SendToClientL( iStorage->iIds, message );
    CleanupStack::Pop( message );
    
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcReceive;
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	EUNIT_ASSERT( iStorage->iContextServer != NULL );
	EUNIT_ASSERT( iStorage->iContextServer->Compare( KReceiveMessage ) == 0 );
    
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcGetSupportedAudioCodecs;
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );

    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcGetSupportedVideoCodecs;
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
			
	iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcDisplaysAvailable;
    iStorage->iIds.iMsgType = EMceItcMsgTypeReadString;
    TMceItcArgTInt displaysAvailable( 0 );
    iStorage->StoreL( iStorage->iIds, displaysAvailable );
    iSession->ServiceL(RMessage2());
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcCameraInfo;
    iStorage->iIds.iMsgType = EMceItcMsgTypeReadString;
    TMceCameraInfo cinfo;
    cinfo.iIndex = 0;
    
    TMceItcArgTCameraInfo cameraInfo( cinfo );
    iStorage->StoreL( iStorage->iIds, cameraInfo );
    iSession->ServiceL(RMessage2());
    EUNIT_ASSERT( iStorage->iCompletionCode == 0 );

        
	iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcMaxVolume;
    TMceItcArgTInt maxVol( 0 );
    iStorage->StoreL( iStorage->iIds, maxVol );
    iSession->ServiceL(RMessage2());
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	
	iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcMaxGain;
    TMceItcArgTInt maxGain( 0 );
    iStorage->StoreL( iStorage->iIds, maxGain );
    iSession->ServiceL(RMessage2());
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
	
	
	iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = EMceItcTranscodeWithSession;
    iStorage->iIds.iMsgType = EMceItcMsgTypeSession;
    
    // Test transcoding with session, will fail as stream is missing from session
    CMceComSession* transcodeSession = CMceComSession::NewL( CMceComSession::EOutSession );
    SetTranscodingItcParams( transcodeSession ); // Took ownership
    
    MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( iSession->ServiceL(RMessage2()), KErrArgument );
    
    // Test transcoding with session, will fails as stream has no source
    CMceComSession* transcodeSession2 = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( transcodeSession2 );
    CMceComVideoStream* videoStream = CMceComVideoStream::NewLC();
    transcodeSession2->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    CleanupStack::Pop( transcodeSession2 );
    SetTranscodingItcParams( transcodeSession2 ); // Took ownership
    MCE_EUNIT_ASSERT_SPECIFIC_LEAVE( iSession->ServiceL(RMessage2()), KErrArgument );
    
    // Test transcoding with session, success
    CMceComSession* transcodeSession3 = CMceComSession::NewL( CMceComSession::EOutSession );
    CleanupStack::PushL( transcodeSession3 );
    CMceComVideoStream* videoStream2 = CMceComVideoStream::NewLC();
    CMceComFileSource* fileSource = CMceComFileSource::NewLC();
    videoStream2->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );
    transcodeSession3->AddStreamL( videoStream2 );
    CleanupStack::Pop( videoStream2 );
    CleanupStack::Pop( transcodeSession3 );
    SetTranscodingItcParams( transcodeSession3 ); // Took ownership
    MCE_LEAVE_HANDLING( iSession->ServiceL(RMessage2()), KErrCompletion );
	EUNIT_ASSERT( iStorage->iCompletionCode == 0 );
    
    // Test incorrect itc func
    iStorage->iCompletionCode = -1;
    iStorage->iItcFunction = (TMceItcFunctions)KErrNotFound;
    TRAPD( error, iSession->ServiceL(RMessage2()) );
#ifdef ALLOW_NOT_SUPPORTED_ITC
    EUNIT_ASSERT( error == KErrNone || error == KErrCompletion );
#else
    EUNIT_ASSERT( error == KErrNotSupported );
#endif
    }

void UT_CMceCsSession::UT_CMceCsSession_UidL(  )
    {
    TMceIds ids;
    ids.iAppUID = 10;
    iSession->SetClientUidL(ids, RMessage2());
	EUNIT_ASSERT( iSession->Uid().iUid == 10 );
    }

void UT_CMceCsSession::UT_CMceCsSession_TestCancelClientReceiveL(  )
    {
    TMceIds ids;
    iSession->ClientReadyToReceiveL(ids,RMessage2());
    iSession->CancelClientReceiveL(ids);
    TRAPD(err, iSession->CancelClientReceiveL(ids));
    EUNIT_ASSERT(err == KErrNotFound);
    }

void UT_CMceCsSession::UT_CMceCsSession_SubSessionsL()
    {
    TMceIds ids;
    ids.iProfileID = 1;
    ids.iManagerType = KMceCSSIPSession;
    ids.iDialogType = KMceDlgTypeInvite;
    
	CDesC8ArrayFlat* params = new (ELeave) CDesC8ArrayFlat( KMceArrayGranularity );
	CleanupStack::PushL( params );
	params->AppendL( _L8("User <user2@host>") );
	params->AppendL( _L8("User <user@host>") );
    
    CMceMsgTextArray* msgParams = new (ELeave) CMceMsgTextArray( *params );
    CleanupStack::PushL( msgParams );
    iStorage->StoreL( TMceIds(), *msgParams );

    CleanupStack::PopAndDestroy( msgParams );
    CleanupStack::PopAndDestroy( params );
    
    
    TInt err;
    TRAP (err, iSession->CallSubSessionL(ids, 0, RMessage2()));
    if ( err == KErrNoMemory )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT (err != KErrNone);

    iSession->CreateSubSessionL(ids, RMessage2());
    
    //TODO: task 23771 commented the below code
/*
    iSession->CallSubSessionL(ids, EMceItcClientReadyToReceive, RMessage2());
    iSession->CallSubSessionL(ids, EMceItcClientCancelReceive, RMessage2());

	ids.iSessionID = 2121212121;
    TRAP(err, iSession->CallSubSessionL(ids, 17, RMessage2()));
    EUNIT_ASSERT (err != KErrNone);
  */  
    }

void UT_CMceCsSession::UT_CMceCsSession_SendToClientLL()
    {
    HBufC8* context = NULL;
    HBufC8* content = NULL;
    
    TMceIds ids;
    ids.iProfileID = 1;
    ids.iManagerType = KMceCSSIPSession;
    ids.iDialogType = KMceDlgTypeInvite;

    iSession->Implementation().SendToClientL( ids, context, content );
    

    context = _L8("data").AllocLC();
    
    iSession->Implementation().SendToClientL( ids, context, content );
    CleanupStack::Pop( context );
    
    context = _L8("data").AllocLC();
    iSession->ServerCore().Manager().iOrphanSubSessions->SendToClientL( ids, context, content );
    
    CleanupStack::Pop( context );    
    }
    
void UT_CMceCsSession::UT_CMceCsSession_StoreDefaultHeadersLL()
    {
    TMceComFactory factory;
    CMceMsgArray<TMceComFactory>* msgArray =
    	new (ELeave) CMceMsgArray<TMceComFactory>( factory );
    CleanupStack::PushL( msgArray );
    msgArray->PushL();
    
    CDesC8Array* methods = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( methods );
    methods->AppendL( _L8( "INVITE" ) );
    
    CMceMsgTextArray* methodsArray = new (ELeave) CMceMsgTextArray( *methods );
    CleanupStack::Pop( methods );
    CleanupStack::PushL( methodsArray );
    methodsArray->PushL();
    
    msgArray->iArray.AppendL( methodsArray );
    CleanupStack::Pop( methodsArray );
    
    
    CDesC8Array* headers = new( ELeave ) CDesC8ArrayFlat( KMceArrayGranularity );
    CleanupStack::PushL( headers );
    headers->AppendL( KHeader );
    headers->AppendL( KHeader2 );
    
    CMceMsgTextArray* headersArray = new (ELeave) CMceMsgTextArray( *headers );
    CleanupStack::Pop( headers );
    CleanupStack::PushL( headersArray );
    headersArray->PushL();
    
    msgArray->iArray.AppendL( headersArray );
    CleanupStack::Pop( headersArray );

    TMceIds ids;
    
    iStorage->StoreL( ids, *msgArray );
    
    iSession->StoreDefaultHeadersL( RMessage2() );
    
    CleanupStack::PopAndDestroy( msgArray );
    }
    
void UT_CMceCsSession::UT_CMceCsSession_DefaultHeadersLL()
    {
    iStorage->iContextClient = _L8( "INVITE" ).AllocL();
    iSession->DefaultHeadersL( RMessage2() );
    }

// Helpers

void UT_CMceCsSession::SetTranscodingItcParams( CMceComSession* aSession )
    {
    // NOTE, ownership of aSession is taken even if leave occurs
    CleanupStack::PushL( aSession );
    CMceMsgObject<CMceComSession>* transcodeSessionMsg = 
        new (ELeave) CMceMsgObject<CMceComSession>( *aSession, EMceItcMsgTypeSession );
    CleanupStack::Pop( aSession );
    transcodeSessionMsg->PushL();
    CleanupStack::PushL( transcodeSessionMsg );
    transcodeSessionMsg->EncodeL();
    TPtr8 externalizedMessage = transcodeSessionMsg->EncodeBuffer().Ptr(0);
    delete iStorage->iContextClient;
    iStorage->iContextClient = NULL;
    iStorage->iContextClient = externalizedMessage.AllocL();
    CleanupStack::PopAndDestroy( transcodeSessionMsg );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMceCsSession,
    "UT_CMceCsSession",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMceCsSession",
    "TestOpenSubSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_TestOpenSubSessionL, Teardown)

EUNIT_TEST(
    "ServiceL - test ",
    "CMceCsSession",
    "ServiceL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_ServiceLL, Teardown)


EUNIT_TEST(
    "Uid - test ",
    "CMceCsSession",
    "Uid",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_UidL, Teardown)

EUNIT_TEST(
    "ITC - test ",
    "CMceCsSession",
    "TestCancelClientReceiveL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_TestCancelClientReceiveL, Teardown)

EUNIT_TEST(
    "SubSessions - test ",
    "CMceCsSession",
    "SubSessions",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_SubSessionsL, Teardown)

EUNIT_TEST(
    "SendToClientL - test ",
    "CMceCsSession",
    "SendToClientL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_SendToClientLL, Teardown)

EUNIT_TEST(
    "StoreDefaultHeadersL - test ",
    "CMceCsSession",
    "StoreDefaultHeadersL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_StoreDefaultHeadersLL, Teardown)


EUNIT_TEST(
    "DefaultHeadersL - test ",
    "CMceCsSession",
    "DefaultHeadersL",
    "FUNCTIONALITY",
    SetupL, UT_CMceCsSession_DefaultHeadersLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
