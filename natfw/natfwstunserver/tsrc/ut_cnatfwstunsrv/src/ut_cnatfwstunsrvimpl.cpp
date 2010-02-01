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
#include "ut_cnatfwstunsrvimpl.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "natfwstunsrvimpl.h"
#include "cncmconnectionmultiplexer.h"
#include "natfwunsafbindingrequest.h"
#include "natfwcredentials.h"
#include "natfwunsafusernameattribute.h"
#include "natfwunsafattribute.h"
#include "natfwunsafpriorityattribute.h"
#include "natfwunsafusecandidateattribute.h"
#include "natfwunsafmessage.h"
#include "natfwunsafdataindication.h"
#include "natfwunsafdataattribute.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafmagiccookieattribute.h"
#include "natfwunsaficecontrollingattribute.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsaffingerprintattribute.h"
#include "natfwunsafbindingindication.h"
#include "natfwunittestmacros.h"

_LIT8( KUsername, "userB:userAA" );
_LIT8( KUsername2, "userB:userCC" );
_LIT8( KUsernameA, "userAA:" );
_LIT8( KTransactionId,"aaaabbbb");
_LIT8( KPassword,"aaaabbbbccccdddd");
_LIT8( KPassword2,"wrongpasswd");
_LIT( KRemoteAddress, "127.0.0.0" );

void ut_cnatfwstunsrvimpl::STUNRequestReceivedL( const TInetAddr& /*aLocalAddr*/,
    const TInetAddr& /*aFromAddr*/, const TInetAddr& /*aPeerAddr*/,
    TUint /*aPriority*/, TBool /*aRemoteFavored*/ )
    {    
    }
                                      
// CONSTRUCTION
ut_cnatfwstunsrvimpl* ut_cnatfwstunsrvimpl::NewL()
    {
    ut_cnatfwstunsrvimpl* self = ut_cnatfwstunsrvimpl::NewLC();
    CleanupStack::Pop();

    return self;
    }

ut_cnatfwstunsrvimpl* ut_cnatfwstunsrvimpl::NewLC()
    {
    ut_cnatfwstunsrvimpl* self = new( ELeave ) ut_cnatfwstunsrvimpl();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
ut_cnatfwstunsrvimpl::~ut_cnatfwstunsrvimpl()
    {
    }

// Default constructor
ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl()
    {
    }

// Second phase construct
void ut_cnatfwstunsrvimpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void ut_cnatfwstunsrvimpl::SetupL(  )
    {
    iConnMux = CNcmConnectionMultiplexer::NewL( *this );
    iStunSrvImpl = CNATFWSTUNSrvImpl::NewL( *this, *iConnMux ); 
    }


void ut_cnatfwstunsrvimpl::Teardown(  )
    {
    delete iStunSrvImpl;
    delete iConnMux;
    }

// Encodes requests
HBufC8* ut_cnatfwstunsrvimpl::EncodeMessageL( 
    const CNATFWUNSAFMessage& aRequest, const TBool aUseSharedSecret,
    const TBool aAddFingerPrint )
    {
    CBufBase* message = NULL;       
         
    if ( !aUseSharedSecret )
        {
        message = aRequest.EncodeL(); 
        }
    else if ( !aAddFingerPrint )
        {
        message = aRequest.EncodeL( KPassword );
        }
    else
        {
        // ETrue indicates that fingerprint attribute will be added
        message = aRequest.EncodeL( KPassword, ETrue );
        } 
           
    CleanupStack::PushL( message );
    TInt length = message->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    message->Read( 0, writable, length );

    CleanupStack::Pop( readBuf );
    CleanupStack::PopAndDestroy( message );
    
    return readBuf;
    }
    
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_AddAuthenticationParamsLL(  )
    {
    TBufC8<16> password( KPassword );
    TBufC8<7> usernameA( KUsernameA );    
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();    
    credential->SetPasswordL( password );
    credential->SetUsernameL( usernameA ); 
       
    RPointerArray<CNATFWCredentials> identifications;
    
    identifications.Append( credential );
    CleanupClosePushL( identifications );

    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( 
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( 
        iStunSrvImpl->RemoveAuthenticationParamsL( identifications ) );

    CleanupStack::PopAndDestroy( &identifications );
    CleanupStack::PopAndDestroy( credential );   
    }

void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_RemoveAuthenticationParamsL(  )
    {
    _LIT8( KUsernameA1, "userCC" );
    _LIT8( KPassword1,"aaaabbbbcccceeee");    
    TBufC8<16> password1( KPassword1 );
    TBufC8<7> usernameA1( KUsernameA1 );
    
    CNATFWCredentials* identification1 = CNATFWCredentials::NewLC();

    identification1->SetPasswordL( password1 );
    identification1->SetUsernameL( usernameA1 );
    
    TBufC8<12> username( KUsername );
    TBufC8<16> password( KPassword );
    TBufC8<7> usernameA( KUsernameA );

    CNATFWCredentials* credential = CNATFWCredentials::NewLC();

    credential->SetPasswordL( password );
    credential->SetUsernameL( usernameA );
    
    CNATFWCredentials* identification2 = CNATFWCredentials::NewLC();
          
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    
    identifications.Append( credential );
    identifications.Append( identification1 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );
    
    identifications.Append( identification2 );                                                                        
    iStunSrvImpl->RemoveAuthenticationParamsL( identifications ); 
    
    //identifications.ResetAndDestroy();
    CleanupStack::PopAndDestroy( &identifications );
    CleanupStack::PopAndDestroy( identification2 );
    CleanupStack::PopAndDestroy( credential );
    CleanupStack::PopAndDestroy( identification1 );
    }
    
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_SetOperationModeLL()
    {
    TUint64 tieBraker = 32;
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->SetRoleL( EIceRoleControlled, tieBraker ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->SetRoleL( EIceRoleControlling, tieBraker ) );
    }

void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_IncomingMessageLL(  )
    {               
    TBufC8<12> username( KUsername );
    TBufC8<16> password( KPassword );
    TBufC8<7> usernameA( KUsernameA );
    TBufC8<12> username2( KUsername2 );
        
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();

    credential->SetPasswordL( password );
    credential->SetUsernameL( usernameA );
    
    CNATFWCredentials* identification2 = CNATFWCredentials::NewLC();

    identification2->SetPasswordL( password );
    identification2->SetUsernameL( username );
    
    CNATFWCredentials* identification3 = CNATFWCredentials::NewLC();

    identification3->SetPasswordL( password );
    identification3->SetUsernameL( username2 );
       
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    
    identifications.Append( credential );
    identifications.Append( identification2 );
    identifications.Append( identification3 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );  
    
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Copy( KTransactionId );
    
    TBool consumed( EFalse );
    TUint streamId(0);
    TInetAddr localAddress;
    TInetAddr fromAddr;
    _LIT( KAddress, "127.0.0.0" );
    fromAddr.SetPort(250);
    fromAddr.Input( KAddress );
    TInetAddr peerRemoteAddress;

    CNATFWUNSAFBindingRequest* request =
        CNATFWUNSAFBindingRequest::NewLC( transactionID );

// Missing priority attribute  
  
    HBufC8* message = EncodeMessageL( *request );
    CleanupStack::PushL( message );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL( 
        streamId,  *message, localAddress, fromAddr, peerRemoteAddress, consumed ) ); 
    EUNIT_ASSERT( ETrue == consumed );
    CleanupStack::PopAndDestroy( message );

// No username attribute

    request->AddAttributeL( CNATFWUNSAFPriorityAttribute::NewLC( 
                                        CNATFWUNSAFAttribute::EPriority ) );
    CleanupStack::Pop(); // CNATFWUNSAFPriorityAttribute
    message = EncodeMessageL( *request );
    CleanupStack::PushL( message );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL(
        streamId, *message, localAddress, fromAddr, peerRemoteAddress, consumed ) );       
    EUNIT_ASSERT( ETrue == consumed );
    CleanupStack::PopAndDestroy( message );
    
// OK    
    
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    request->AddAttributeL( CNATFWUNSAFUseCandidateAttribute::NewLC() );
    CleanupStack::Pop(); // CNATFWUNSAFUseCandidateAttribute
    
    message = EncodeMessageL( *request );
    CleanupStack::PushL( message );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL(
        streamId, *message, localAddress, fromAddr, peerRemoteAddress, consumed ) );        
    EUNIT_ASSERT( ETrue == consumed ); 
    CleanupStack::PopAndDestroy( message );
    
    CleanupStack::PopAndDestroy( request );

// Data indication including stun request

    CNATFWUNSAFDataIndication* indication =
        CNATFWUNSAFDataIndication::NewLC( transactionID );
    CNATFWUNSAFBindingRequest* request1 =
        CNATFWUNSAFBindingRequest::NewLC( transactionID );
    request1->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    request1->AddAttributeL( CNATFWUNSAFPriorityAttribute::NewLC( 
                                        CNATFWUNSAFAttribute::EPriority ) );
    CleanupStack::Pop(); // CNATFWUNSAFPriorityAttribute 
    request1->AddAttributeL( CNATFWUNSAFUseCandidateAttribute::NewLC() );
    CleanupStack::Pop(); // CNATFWUNSAFUseCandidateAttribute
    
    message = EncodeMessageL( *request1 );
    CleanupStack::PushL( message );
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( *message ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    CleanupStack::PopAndDestroy( message );

    message = EncodeMessageL( *indication, ETrue, EFalse );
    CleanupStack::PushL( message );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL(
        streamId, *message, localAddress, fromAddr, peerRemoteAddress, consumed ) );
    EUNIT_ASSERT( ETrue == consumed );
    CleanupStack::PopAndDestroy( message );

    CleanupStack::PopAndDestroy( request1 );
    CleanupStack::PopAndDestroy( indication );
    

    CleanupStack::PopAndDestroy( &identifications );   
    CleanupStack::PopAndDestroy( identification3 );
    CleanupStack::PopAndDestroy( identification2 );
    CleanupStack::PopAndDestroy( credential ); 
    }

void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_IncomingMessageL2L(  )
    {
    TBufC8<12> username( KUsername );
    TBufC8<16> password( KPassword );    
    TBufC8<7> usernameA( KUsernameA );
    TBufC8<12> username2( KUsername2 );
        
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();

    credential->SetPasswordL( password );
    credential->SetUsernameL( usernameA );
    
    CNATFWCredentials* identification2 = CNATFWCredentials::NewLC();

    identification2->SetPasswordL( password );
    identification2->SetUsernameL( username );
    
    CNATFWCredentials* identification3 = CNATFWCredentials::NewLC();

    identification3->SetPasswordL( password );
    identification3->SetUsernameL( username2 );
       
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    
    identifications.Append( credential );
    identifications.Append( identification2 );
    identifications.Append( identification3 );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );
    
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Copy( KTransactionId );
    
    TBool consumed( EFalse );
    TUint streamId(0);
    TInetAddr localAddress;
    TInetAddr fromAddr;
    _LIT( KAddress, "127.0.0.0" );
    fromAddr.SetPort(250);
    fromAddr.Input( KAddress );
    TInetAddr peerRemoteAddress;   
    
// Keepalive stun binding indication

    CNATFWUNSAFBindingIndication* binding =
        CNATFWUNSAFBindingIndication::NewLC( transactionID );
    HBufC8* message = EncodeMessageL( *binding );
    CleanupStack::PushL( message );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL( 
        streamId, *message, localAddress, fromAddr, peerRemoteAddress, consumed ) );
    EUNIT_ASSERT( ETrue == consumed );
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( binding );

// Data indication including some other message
    
    CNATFWUNSAFDataIndication* indication =
        CNATFWUNSAFDataIndication::NewLC( transactionID );
    CNATFWUNSAFBindingResponse* response =
        CNATFWUNSAFBindingResponse::NewLC( transactionID );
    
    message = EncodeMessageL( *response );
    CleanupStack::PushL( message );
    indication->AddAttributeL( CNATFWUNSAFDataAttribute::NewLC( *message ) );
    CleanupStack::Pop(); // CNATFWUNSAFDataAttribute
    CleanupStack::PopAndDestroy( message );
    
    message = EncodeMessageL( *indication, ETrue, EFalse ); 
    CleanupStack::PushL( message );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL(
        streamId,  *message, localAddress, fromAddr, peerRemoteAddress, consumed ) );
    
    EUNIT_ASSERT( EFalse == consumed );

    CleanupStack::PopAndDestroy( message );
    
    CleanupStack::PopAndDestroy( response );
    CleanupStack::PopAndDestroy( indication );
    
// Some other message

    response = CNATFWUNSAFBindingResponse::NewLC( transactionID );
    response->AddAttributeL( CNATFWUNSAFMappedAddressAttribute::NewLC( fromAddr ) );
    CleanupStack::Pop(); // CNATFWUNSAFMappedAddressAttribute
    
    message = EncodeMessageL( *response );
    CleanupStack::PushL( message );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->IncomingMessageL(
        streamId,  *message, localAddress, fromAddr, peerRemoteAddress, consumed ) );
    
    EUNIT_ASSERT( EFalse == consumed );
    
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( response );

    CleanupStack::PopAndDestroy( &identifications );   
    CleanupStack::PopAndDestroy( identification3 );
    CleanupStack::PopAndDestroy( identification2 );
    CleanupStack::PopAndDestroy( credential );
    }


void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_IsRoleConflictPresentL()
    {
    TUint64 tieBraker = 32;
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFBindingRequest* request =
        CNATFWUNSAFBindingRequest::NewLC( transactionID );
    
// no role attribute present
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), EFalse );
    
// stun srv has controlled attribute, request controlling    
    request->AddAttributeL( CNATFWUNSAFIceControllingAttribute::NewLC( tieBraker ) );
    CleanupStack::Pop(); // CNATFWUNSAFIceControllingAttribute
    
    iStunSrvImpl->SetRoleL( EIceRoleControlled, tieBraker );
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), EFalse );
    
// both have controlling attribute, stun srv has bigger or equal tiebraker
         
    iStunSrvImpl->SetRoleL( EIceRoleControlling, tieBraker );    
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), ETrue );
    
// both have controlling attribute, stun srv has smaller tiebraker
    iStunSrvImpl->iTieBreaker = 16;
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), EFalse );
    
// both have controlled attribute, stun srv has smaller tiebraker    
    //request->DetachAttribute( CNATFWUNSAFIceControllingAttribute::NewL( tieBraker ) );
    request->DeleteAttribute( CNATFWUNSAFAttribute::EIceControlling );
    request->AddAttributeL(
        CNATFWUNSAFIceControlledAttribute::NewLC( tieBraker ) );
    CleanupStack::Pop(); // CNATFWUNSAFIceControllingAttribute 
    
    tieBraker = 16;    
    iStunSrvImpl->SetRoleL( EIceRoleControlled, tieBraker );
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), ETrue );

// both have controlled attribute, stun srv has bigger or equal tiebraker    
    iStunSrvImpl->iTieBreaker = 64;
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), EFalse );
    
// stun srv has controlling attribute, request controlled
    iStunSrvImpl->SetRoleL( EIceRoleControlling, tieBraker );
    EUNIT_ASSERT_EQUALS(
        iStunSrvImpl->IsRoleConflictPresent( *request ), EFalse );   

    CleanupStack::PopAndDestroy( request ); 
    }
  
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_SetPeerRemoteAddressL()
    {
    TInetAddr localAddress;
    TInetAddr fromAddr;
    TInetAddr peerRemoteAddress;
    
    iStunSrvImpl->SetPeerRemoteAddress( fromAddr, peerRemoteAddress );
    peerRemoteAddress.Input( KRemoteAddress );
    iStunSrvImpl->SetPeerRemoteAddress( fromAddr, peerRemoteAddress );
    iStunSrvImpl->SetPeerRemoteAddress( fromAddr, peerRemoteAddress );
    }
    
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_DecodeMessageLL()
    {
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFBindingResponse* response =
        CNATFWUNSAFBindingResponse::NewLC( transactionID );
    
    CBufBase* msg = response->EncodeL();
    CleanupStack::PushL( msg );        
    TInt length = msg->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    msg->Read( 0, writable );
     
    CNATFWUNSAFMessage* messge = NULL; 
    messge = iStunSrvImpl->DecodeMessageL( *readBuf );
    EUNIT_ASSERT( messge != NULL );
    
    CleanupStack::PopAndDestroy( readBuf );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( response );
    delete messge;				                
    }
    
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_EncodeMessageLL()
    {
    TBufC8<16> password( KPassword );    
    TBufC8<12> username( KUsername2 );    
    
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFBindingRequest* request =
        CNATFWUNSAFBindingRequest::NewLC( transactionID );
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    request->AddAttributeL( CNATFWUNSAFPriorityAttribute::NewLC( 
                                        CNATFWUNSAFAttribute::EPriority ) );
    CleanupStack::Pop(); // CNATFWUNSAFPriorityAttribute
    request->AddAttributeL( CNATFWUNSAFUseCandidateAttribute::NewLC() );
    CleanupStack::Pop(); // CNATFWUNSAFUseCandidateAttribute
    
    HBufC8* msg = iStunSrvImpl->EncodeMessageL( *request, password, KErrNone );
    delete msg;
    msg = iStunSrvImpl->EncodeMessageL( *request, password, KErrNone );
    delete msg;
    msg = iStunSrvImpl->EncodeMessageL( *request, password, 432 );
    delete msg;
    msg = iStunSrvImpl->EncodeMessageL( *request, password, 401 );
    delete msg;
    msg = iStunSrvImpl->EncodeMessageL( *request, password, 487 );
    delete msg;

    CleanupStack::PopAndDestroy( request );			                
    }    
    
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_CheckIntegrityLL()
    {
    TBufC8<16> password( KPassword );
    TBufC8<12> username( KUsername2 );
       
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();
    credential->SetPasswordL( password );
    credential->SetUsernameL( username );
       
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    identifications.Append( credential );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );
    
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFBindingRequest* request =
        CNATFWUNSAFBindingRequest::NewLC( transactionID );
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    
    HBufC8* dummy = NULL;
    EUNIT_ASSERT_EQUALS( iStunSrvImpl->CheckIntegrityL(
        *request, *dummy, password ), EFalse );
        
    CBufBase* msg = request->EncodeL( password );
    CleanupStack::PushL( msg );        
    TInt length = msg->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    msg->Read( 0, writable );  
    
    CNATFWUNSAFMessage* messge = NULL; 
    messge = iStunSrvImpl->DecodeMessageL( *readBuf );
    CleanupStack::PushL( messge );
    EUNIT_ASSERT( messge != NULL );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->CheckIntegrityL( *messge, *readBuf, password ) );
	
	iStunSrvImpl->iRequestInsideIndication = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->CheckIntegrityL( *messge, *readBuf, password ) );
		
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->RemoveAuthenticationParamsL( identifications ) );
    
    CleanupStack::PopAndDestroy( messge );
    CleanupStack::PopAndDestroy( readBuf );
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( request );
    CleanupStack::PopAndDestroy( &identifications );
    CleanupStack::PopAndDestroy( credential );	                
    }    

void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_GetPasswordL()
    {
    TNATFWUNSAFTransactionID transactionID;
    CNATFWUNSAFBindingRequest* request = 
        CNATFWUNSAFBindingRequest::NewLC( transactionID );        
    TBufC8<16> password( KPassword );    
    TBufC8<12> username( KUsername2 );
     
    HBufC8* passwd; 
    TInt error;
    passwd = iStunSrvImpl->GetPassword( *request, error );
    EUNIT_ASSERT( passwd == NULL );

    CNATFWCredentials* identification3 = CNATFWCredentials::NewLC();    
    identification3->SetPasswordL( password );
    identification3->SetUsernameL( username );
    identification3->SetDirection( CNATFWCredentials::EOutbound );    
       
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();    
    credential->SetPasswordL( password );
    credential->SetUsernameL( username );
    
    TBufC8<12> username3( KUsernameA );
    CNATFWCredentials* identification2 = CNATFWCredentials::NewLC();    
    identification2->SetPasswordL( password );
    identification2->SetUsernameL( username3 );     
       
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    identifications.Append( identification3 );
    identifications.Append( identification2 );
    identifications.Append( credential );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );      

    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    passwd = iStunSrvImpl->GetPassword( *request, error );
    EUNIT_ASSERT( passwd != NULL );   
    delete passwd;
    passwd = NULL;
    
// No semicolon in username
    
    _LIT8( Kname, "userAA" );
    request->DeleteAttribute( CNATFWUNSAFAttribute::EUsername );
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( Kname ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    passwd = iStunSrvImpl->GetPassword( *request, error ); 
    EUNIT_ASSERT( passwd == NULL ); 
/*
// NULL username 

    HBufC8* dummy = NULL;
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute    
    request->DeleteAttribute( CNATFWUNSAFAttribute::EUsername );
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( *dummy ) );
    passwd = iStunSrvImpl->GetPassword( *request ); 
    EUNIT_ASSERT( passwd == NULL );       
*/
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->RemoveAuthenticationParamsL( identifications ) );

    request->DeleteAttribute( CNATFWUNSAFAttribute::EUsername );
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );  
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
   
    passwd = iStunSrvImpl->GetPassword( *request, error );
    EUNIT_ASSERT( passwd == NULL );
    
    CleanupStack::PopAndDestroy( &identifications );
    CleanupStack::PopAndDestroy( identification2 );
    CleanupStack::PopAndDestroy( credential );
    CleanupStack::PopAndDestroy( identification3 );
    
    CleanupStack::PopAndDestroy( request );        				                
    } 
    
void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_SendResponseLL()
    {
    TBufC8<16> password( KPassword );
    TBufC8<12> username( KUsername2 );
       
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();
    credential->SetPasswordL( password );
    credential->SetUsernameL( username ); 
       
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    identifications.Append( credential );
    
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );    
    
    TNATFWUNSAFTransactionID transactionID;
    transactionID.Append(TUint8(0));
    transactionID.Append(TUint8(1));
    transactionID.Append(TUint8(2));
    transactionID.Append(TUint8(3));
    transactionID.Append(TUint8(4));
    transactionID.Append(TUint8(5));
    transactionID.Append(TUint8(6));
    transactionID.Append(TUint8(7));
    transactionID.Append(TUint8(8));
    transactionID.Append(TUint8(9));
    transactionID.Append(TUint8(0xa));
    transactionID.Append(TUint8(0xb));
        
    CNATFWUNSAFBindingRequest* request = CNATFWUNSAFBindingRequest::NewLC( transactionID );   
    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->SendResponseL( *request, KErrNone, password ) );
    
    request->AddAttributeL( CNATFWUNSAFMagicCookieAttribute::NewLC() );
    CleanupStack::Pop(); // CNATFWUNSAFMagicCookieAttribute
    
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->SendResponseL( *request, KErrNone, password) );
 
    HBufC8* nullPasswd = NULL;
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->SendResponseL( *request, 401, *nullPasswd ) );
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->SendResponseL( *request, 487, *nullPasswd ) );
    NATFW_EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iStunSrvImpl->SendResponseL( *request, 500, password ), KErrArgument );
  
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->RemoveAuthenticationParamsL( identifications ) );

    CleanupStack::PopAndDestroy( request );
    CleanupStack::PopAndDestroy( &identifications );
    CleanupStack::PopAndDestroy( credential );     				                
    }      

void ut_cnatfwstunsrvimpl::ut_cnatfwstunsrvimpl_IsRequestValidLL()
    {    
    TNATFWUNSAFTransactionID transactionID;
    TBufC8<12> username( KUsername2 ); 
    TBufC8<16> password( KPassword );
    TBufC8<16> password2( KPassword2 );
    
    CNATFWUNSAFBindingRequest* request =
        CNATFWUNSAFBindingRequest::NewLC( transactionID );
    request->AddAttributeL( CNATFWUNSAFPriorityAttribute::NewLC( 
                                CNATFWUNSAFAttribute::EPriority ) );
    CleanupStack::Pop(); // CNATFWUNSAFPriorityAttribute
    
    request->AddAttributeL( CNATFWUNSAFUseCandidateAttribute::NewLC() );
    CleanupStack::Pop(); // CNATFWUNSAFUseCandidateAttribute 
    
    CNATFWCredentials* credential = CNATFWCredentials::NewLC();
    credential->SetPasswordL( password );
    credential->SetUsernameL( username ); 
       
    RPointerArray<CNATFWCredentials> identifications;
    CleanupClosePushL( identifications );
    identifications.Append( credential );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        iStunSrvImpl->AddAuthenticationParamsL( identifications ) );

    HBufC8* encodedMessage = EncodeMessageL( *request );
    CleanupStack::PushL( encodedMessage );
    CNATFWUNSAFMessage* message = NULL; 
    message = iStunSrvImpl->DecodeMessageL( *encodedMessage );
    CleanupStack::PushL( message );
    
    EUNIT_ASSERT( message != NULL );
 
// no passwd found
    TBool isValid;

    NATFW_EUNIT_ASSERT_NO_LEAVE(
        isValid= iStunSrvImpl->IsRequestValidL( *message, *encodedMessage ) );
    EUNIT_ASSERT( !isValid );

    request->AddAttributeL( CNATFWUNSAFUsernameAttribute::NewLC( username ) );
    CleanupStack::Pop(); // CNATFWUNSAFUsernameAttribute
    
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( encodedMessage );

    encodedMessage = EncodeMessageL( *request );
    CleanupStack::PushL( encodedMessage );
    message = iStunSrvImpl->DecodeMessageL( *encodedMessage );
    CleanupStack::PushL( message );
    EUNIT_ASSERT( message != NULL );
    
// ok
    NATFW_EUNIT_ASSERT_NO_LEAVE( 
        isValid = iStunSrvImpl->IsRequestValidL( *message, *encodedMessage ) );
    EUNIT_ASSERT( isValid );

// role conflict
    iStunSrvImpl->iRoleConflict = ETrue;
    NATFW_EUNIT_ASSERT_NO_LEAVE( 
        isValid = iStunSrvImpl->IsRequestValidL( *message, *encodedMessage ) );
    EUNIT_ASSERT( !isValid );
    iStunSrvImpl->iRoleConflict = EFalse;

// no fingerprint attribute found
    
    request->DeleteAttribute( CNATFWUNSAFAttribute::EMessageIntegrity );
    NATFW_EUNIT_ASSERT_NO_LEAVE(
        isValid = iStunSrvImpl->IsRequestValidL( *request, *encodedMessage ) );
    EUNIT_ASSERT( !isValid );
    
    CleanupStack::PopAndDestroy( message );
    CleanupStack::PopAndDestroy( encodedMessage );
    
// failed integrity check
    /*
    // ETrue indicates that fingerprint attribute will be added
    CBufBase* msg = request->EncodeL( KPassword2, ETrue ); // wrong passwd
    CleanupStack::PushL( msg );
    TInt length = msg->Size();
    HBufC8* readBuf = HBufC8::NewLC( length );
    TPtr8 writable( readBuf->Des() );
    msg->Read( 0, writable, length );

    CleanupStack::Pop( readBuf );
    CleanupStack::PopAndDestroy( msg );
    
    //encodedMessage = EncodeMessageL( *request );       
    //message = iStunSrvImpl->DecodeMessageL( *encodedMessage );
    //EUNIT_ASSERT( message != NULL ); 
    
    //iStunSrvImpl->iRequestInsideIndication = ETrue;
    isValid = iStunSrvImpl->IsRequestValidL( *message, *readBuf );
    EUNIT_ASSERT( !isValid );
    delete encodedMessage;    
    */
    NATFW_EUNIT_ASSERT_NO_LEAVE( iStunSrvImpl->RemoveAuthenticationParamsL( identifications ) );

    CleanupStack::PopAndDestroy( &identifications );
    CleanupStack::PopAndDestroy( credential );
    CleanupStack::PopAndDestroy( request );   			                
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    ut_cnatfwstunsrvimpl,
    "Add test suite description here.",
    "UNIT" )

EUNIT_TEST(
    "AddAuthenticationParamsL - test ",
    "CNATFWSTUNSrvImpl",
    "AddAuthenticationParamsL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_AddAuthenticationParamsLL, Teardown)

EUNIT_TEST(
    "RemoveAuthenticationParams - test ",
    "CNATFWSTUNSrvImpl",
    "RemoveAuthenticationParams",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_RemoveAuthenticationParamsL, Teardown)

EUNIT_TEST(
    "SetRoleL - test ",
    "CNATFWSTUNSrvImpl",
    "SetRoleL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_SetOperationModeLL, Teardown)    

EUNIT_TEST(
    "IncomingMessageL - test ",
    "CNATFWSTUNSrvImpl",
    "IncomingMessageL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_IncomingMessageLL, Teardown)

EUNIT_TEST(
    "IncomingMessageL - test 2 ",
    "CNATFWSTUNSrvImpl",
    "IncomingMessageL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_IncomingMessageL2L, Teardown)

// private methods

EUNIT_TEST(
    "IsRoleConflictPresent - test ",
    "CNATFWSTUNSrvImpl",
    "IsRoleConflictPresent",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_IsRoleConflictPresentL, Teardown)

EUNIT_TEST(
    "SetPeerRemoteAddress - test ",
    "CNATFWSTUNSrvImpl",
    "SetPeerRemoteAddress",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_SetPeerRemoteAddressL, Teardown)     

EUNIT_TEST(
    "DecodeMessageL - test ",
    "CNATFWSTUNSrvImpl",
    "DecodeMessageL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_DecodeMessageLL, Teardown)
    
EUNIT_TEST(
    "EncodeMessageL - test ",
    "CNATFWSTUNSrvImpl",
    "EncodeMessageL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_EncodeMessageLL, Teardown)    

EUNIT_TEST(
    "CheckIntegrityL - test ",
    "CNATFWSTUNSrvImpl",
    "CheckIntegrityL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_CheckIntegrityLL, Teardown) 

EUNIT_TEST(
    "GetPassword - test ",
    "CNATFWSTUNSrvImpl",
    "GetPassword",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_GetPasswordL, Teardown)
  
EUNIT_TEST(
    "SendResponseL - test ",
    "CNATFWSTUNSrvImpl",
    "SendResponseL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_SendResponseLL, Teardown)            

EUNIT_TEST(
    "IsRequestValidL - test ",
    "CNATFWSTUNSrvImpl",
    "IsRequestValidL",
    "FUNCTIONALITY",
    SetupL, ut_cnatfwstunsrvimpl_IsRequestValidLL, Teardown)   

EUNIT_END_TEST_TABLE

//  END OF FILE
