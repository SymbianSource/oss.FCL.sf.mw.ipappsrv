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
#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdpmediafield.h>
#include <sdpattributefield.h>
#include <in_sock.h>
#include "natfwcandidate.h"
#include "nspcontentparser.h"
#include "nspdefs.h"
#include "ut_nspcontentparser.h"
#include "nsptesthelper.h"
#include "nsptls.h"

// STUN
_LIT8( KCNSPStun, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 192.0.0.2\r\n\
t=0 0\r\n\
m=audio 49170 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n" );

// Simple
_LIT8( KCNSPSimple, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 192.0.0.2\r\n\
t=0 0\r\n\
m=audio 49170 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=candidate:2 1 UDP 1232003404 192.0.0.2 8931 typ srflx\r\n\
a=candidate:1 1 UDP 2130003404 10.0.0.3 12322 typ local\r\n" );

// Rtcp
_LIT8( KCNSPRtcp, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 192.0.0.2\r\n\
t=0 0\r\n\
m=audio 49170 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=candidate:2 1 UDP 1232003404 192.0.0.2 49170 typ srflx\r\n\
a=candidate:1 1 UDP 2130003404 10.0.0.3 12322 typ local\r\n\
a=rtcp:13233\r\n" );

// Rtcp
_LIT8( KCNSPNotMismatchRtcp, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 172.0.0.2\r\n\
t=0 0\r\n\
m=audio 34234 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=candidate:1 2 UDP 1232003404 192.0.0.2 49170 typ srflx\r\n\
a=candidate:1 1 UDP 2130003404 10.0.0.3 12322 typ local\r\n\
a=rtcp:49170 IN IP4 192.0.0.2\r\n" );

// Mismatch
_LIT8( KCNSPMismatch, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 172.0.0.2\r\n\
t=0 0\r\n\
m=audio 34234 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=candidate:1 1 UDP 2130003404 10.0.0.3 12322 typ local\r\n\
a=candidate:1 2 UDP 1232003404 192.0.0.2 49170 typ srflx\r\n\
a=rtcp:46613 IN IP4 192.0.0.2\r\n" );

// Mismatch-attribute
_LIT8( KCNSPMismatchAttr, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 172.0.0.2\r\n\
t=0 0\r\n\
m=audio 34234 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=ice-mismatch\r\n\
a=rtcp:46613 IN IP4 192.0.0.2\r\n" );

// Lite-attribute
_LIT8( KCNSPLiteAttr, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 172.0.0.2\r\n\
t=0 0\r\n\
a=ice-lite\r\n\
m=audio 34234 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=rtcp:46613 IN IP4 192.0.0.2\r\n" );

// Complex Rtcp
_LIT8( KCNSPComplexRtcp, "v=0\r\n\
o=alice 2890844526 2890844527 IN IP4 125.0.0.1\r\n\
s=-\r\n\
c=IN IP4 192.0.0.2\r\n\
t=0 0\r\n\
m=audio 49170 RTP/AVP 96\r\n\
a=rtpmap:96 AMR/8000\r\n\
a=candidate:1 1 UDP 2130003404 10.0.0.3 12322 typ local\r\n\
a=candidate:2 1 UDP 1232003404 192.0.0.2 49170 typ srflx\r\n\
a=rtcp:13233 IN IP4 10.0.0.6\r\n" );



//  METHODS:
// ---------------------------------------------------------------------------
// UT_CNSPContentParser::NewL
// ---------------------------------------------------------------------------
// 
UT_CNSPContentParser* UT_CNSPContentParser::NewL()
    {
    UT_CNSPContentParser* self = UT_CNSPContentParser::NewLC();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::NewLC
// ---------------------------------------------------------------------------
// 
UT_CNSPContentParser* UT_CNSPContentParser::NewLC()
    {
    UT_CNSPContentParser* self = new( ELeave ) UT_CNSPContentParser();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::~UT_CNSPContentParser
// ---------------------------------------------------------------------------
// 
UT_CNSPContentParser::~UT_CNSPContentParser()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_CNSPContentParser
// ---------------------------------------------------------------------------
// 
UT_CNSPContentParser::UT_CNSPContentParser()
    {
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::ConstructL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::SetupL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::SetupL()
    {
    NSPTls::OpenL();
    SdpCodecStringPool::OpenL();
    iStorage = NSPTls::Storage();
    iContentParser = CNSPContentParser::NewL();
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::Teardown
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::Teardown()
    {
    delete iContentParser;
    SdpCodecStringPool::Close();
    NSPTls::Close();
    }



// TESTS:
// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_NewLL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_NewLL()
	{
	// NOP, All done in SetupL
	}

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_IsIceSupported
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_IsIceSupported()
    {
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPSimple() );
    
    // OK, ICE SUPPORTED SDP DOC
    NSP_ASSERT( iContentParser->IsIceSupported( *doc ) );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPStun() );
    
    // OK, NOT ICE SUPPORTED
    NSP_ASSERT( !iContentParser->IsIceSupported( *doc ) );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_IsMismatchL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_IsMismatchL()
    {
    TBool mismatch( EFalse );
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPRtcp() );
    
    // OK, NO MISMATCH
    NSP_ASSERT_NO_LEAVE( mismatch = iContentParser->IsMismatchL( *doc ) );
    NSP_ASSERT( !mismatch );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPNotMismatchRtcp() );
    
    // OK, NO MISMATCH
    NSP_ASSERT_NO_LEAVE( mismatch = iContentParser->IsMismatchL( *doc ) );
    NSP_ASSERT( !mismatch );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPMismatch() );
    
    // OK, MISMATCH
    NSP_ASSERT_NO_LEAVE( mismatch = iContentParser->IsMismatchL( *doc ) );
    NSP_ASSERT( mismatch );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_IsMismatchAttribute
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_IsMismatchAttribute()
    {    
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPMismatchAttr() );
    
    // "a=ice-mismatch" ATTRIBUTE FOUND FROM SDP DOC
    NSP_ASSERT( iContentParser->IsMismatchAttribute( *doc->MediaFields()[0] ) );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPMismatch() );
    
    // ATTRIBUTE NOT FOUND
    NSP_ASSERT( !iContentParser->IsMismatchAttribute( *doc->MediaFields()[0] ) );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_IsLiteAttribute
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_IsLiteAttribute()
    {
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPLiteAttr() );
    
    // "a=ice-lite" ATTRIBUTE FOUND FROM SDP DOC
    NSP_ASSERT( iContentParser->IsLiteAttribute( *doc ) );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPSimple() );
    
    // ATTRIBUTE NOT FOUND
    NSP_ASSERT( !iContentParser->IsLiteAttribute( *doc ) );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_FindRTCP
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_FindRTCP()
    {
    _LIT8( TransportAddress, "10.0.0.6" );
    _LIT8( TransportAddress2, "192.0.0.2" );
    _LIT8( KInvalidRtcp, "a=rtcp:invalid\r\n" );
    TBuf8<255> address;
    TUint port( 0 );
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPComplexRtcp() );
    
    // "a=rtcp:.." ATTRIBUTE FOUND, WITH FULL ADDRESS
    NSP_ASSERT( iContentParser->FindRTCP( *doc->MediaFields()[0], address, port ) );
    NSP_ASSERT( !address.Compare( TransportAddress() ) );
    NSP_ASSERT( 13233 == port );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPRtcp() );
    address = doc->ConnectionField()->Address();
    
    // "a=rtcp:.." ATTRIBUTE FOUND, WITH FULL ADDRESS
    NSP_ASSERT( iContentParser->FindRTCP( *doc->MediaFields()[0], address, port ) );
    NSP_ASSERT( !address.Compare( TransportAddress2() ) );
    NSP_ASSERT( 13233 == port );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPSimple() );
    
    // ATTRIBUTE NOT FOUND
    NSP_ASSERT( !iContentParser->FindRTCP( *doc->MediaFields()[0], address, port ) );
    NSP_ASSERT( !address.Compare( TransportAddress2() ) );
    NSP_ASSERT( 13233 == port );
    
    NSP_ASSERT_NO_LEAVE( doc->MediaFields()[0]->AttributeFields().AppendL( CSdpAttributeField::DecodeL( KInvalidRtcp() ) ) );
    
    // "a=rtcp:.." ATTRIBUTE FOUND, WITH INVALID TRANSPORT ADDRESS
    NSP_ASSERT( iContentParser->FindRTCP( *doc->MediaFields()[0], address, port ) );
    NSP_ASSERT( !address.Compare( TransportAddress2() ) );
    NSP_ASSERT( 13233 == port );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_ModifyRTCPL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_ModifyRTCPL()
    {
    _LIT8( TransportAddress, "10.0.0.6" );
    _LIT8( IPv6TransportAddress, "2001:db8::1428:57ab" );
    TBuf8<255> address;
    TUint port( 3400 );
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPComplexRtcp() );
    
    // OK, GIVEN ADDRESS ADDED TO ATTRIBUTE
    NSP_ASSERT_NO_LEAVE( iContentParser->ModifyRTCPL(
            *doc->MediaFields()[0], TransportAddress(), port ) );
    NSP_ASSERT( iContentParser->FindRTCP( *doc->MediaFields()[0], address, port ) );
    NSP_ASSERT( !address.Compare( TransportAddress() ) );
    NSP_ASSERT( 3400 == port );
    
    CleanupStack::PopAndDestroy( doc );
    doc = CSdpDocument::DecodeLC( KCNSPRtcp() );
    
    // OK, GIVEN ADDRESS ADDED TO ATTRIBUTE
    NSP_ASSERT_NO_LEAVE( iContentParser->ModifyRTCPL(
            *doc->MediaFields()[0], IPv6TransportAddress(), 3400 ) );
    NSP_ASSERT( iContentParser->FindRTCP( *doc->MediaFields()[0], address, port ) );
    NSP_ASSERT( !address.Compare( IPv6TransportAddress() ) );
    NSP_ASSERT( 3400 == port );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_AddLiteL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_AddLiteL()
    {
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPSimple() );
    
    // ADD "a=ice-lite" ATTRIBUTE
    NSP_ASSERT_NO_LEAVE( iContentParser->AddLiteL( *doc ) );
    
    // CHECK "a=ice-lite" ATTRIBUTE
    NSP_ASSERT( iContentParser->IsLiteAttribute( *doc ) );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_AddMismatchL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_AddMismatchL()
    {
    CSdpDocument* doc = CSdpDocument::DecodeLC( KCNSPSimple() );
    
    // ADD "a=ice-mismatch" ATTRIBUTE
    NSP_ASSERT_NO_LEAVE( iContentParser->AddMismatchL( *doc->MediaFields()[0] ) );
    
    // CHECK "a=ice-mismatch" ATTRIBUTE
    NSP_ASSERT( iContentParser->IsMismatchAttribute( *doc->MediaFields()[0] ) );
    
    CleanupStack::PopAndDestroy( doc );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_EndodeCandidateLineL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_EndodeCandidateLineL()
    {
    CSdpAttributeField* attribute = NULL;
    CNATFWCandidate* natfwcand = CNATFWCandidate::NewLC();
    CNATFWCandidate* natfwcand2 = CNATFWCandidate::NewLC();
    
    natfwcand->SetFoundationL( _L8("1") );
    natfwcand->SetComponentId( 1 );
    natfwcand->SetTransportProtocol( KProtocolInetUdp );
    natfwcand->SetPriority( 2130003404 );
    natfwcand->SetTransportDomainAddrL( _L8("10.0.0.3") , 8998 );
    natfwcand->SetType( CNATFWCandidate::EHost );

    NSP_ASSERT_NO_LEAVE( attribute = iContentParser->EncodeCandidateLineL( *natfwcand ) );
    CleanupStack::PushL( attribute );
    NSP_ASSERT_NO_LEAVE( iContentParser->DecodeCandidateLineL( *attribute, *natfwcand2 ) );
    
    NSP_ASSERT( !natfwcand2->Foundation().CompareF( natfwcand->Foundation() ) );
    NSP_ASSERT( natfwcand2->ComponentId() == natfwcand->ComponentId() );
    NSP_ASSERT( natfwcand2->TransportProtocol() == natfwcand->TransportProtocol() );
    NSP_ASSERT( natfwcand2->Priority() == natfwcand->Priority() );
    NSP_ASSERT( !natfwcand2->TransportDomainAddr().Compare( natfwcand->TransportDomainAddr() ) );
    NSP_ASSERT( natfwcand2->TransportDomainPort() == natfwcand->TransportDomainPort() );
    NSP_ASSERT( natfwcand2->Priority() == natfwcand->Priority() );
    
    CleanupStack::PopAndDestroy( attribute );
    CleanupStack::PopAndDestroy( natfwcand2 );
    CleanupStack::PopAndDestroy( natfwcand );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_DecodeCandidateLineL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_DecodeCandidateLineL()
    {
    _LIT8( KCandidateLine, "a=candidate:1 1 udp 2130003404 10.0.0.3 8998 typ host\r\n" );
    _LIT8( KCandidateLineFqdn, "a=candidate:1 1 udp 2130003404 www.example.com 8998 typ host\r\n" );
    _LIT8( KCandidateLineInvalid, "a=candidate:1 totally invalid string\r\n" );
    CSdpAttributeField* candidate = CSdpAttributeField::DecodeLC( KCandidateLine );
    CNATFWCandidate* natfwcand = CNATFWCandidate::NewLC();
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->DecodeCandidateLineL( *candidate, *natfwcand ) );
    NSP_ASSERT( _L8("1") == natfwcand->Foundation() );
    NSP_ASSERT( TUint( 1 ) == natfwcand->ComponentId() );
    NSP_ASSERT( KProtocolInetUdp == natfwcand->TransportProtocol() );
    NSP_ASSERT( TUint( 2130003404 ) == natfwcand->Priority() );
    NSP_ASSERT( !natfwcand->TransportDomainAddr().Compare( _L8("10.0.0.3") ) );
    NSP_ASSERT( TUint( 8998 ) == natfwcand->TransportDomainPort() );
    NSP_ASSERT( CNATFWCandidate::EHost == natfwcand->Type() );
    
    CleanupStack::PopAndDestroy( natfwcand );
    CleanupStack::PopAndDestroy( candidate );
    candidate = CSdpAttributeField::DecodeLC( KCandidateLineFqdn );
    natfwcand = CNATFWCandidate::NewLC();
    
    // OK, FQDN
    NSP_ASSERT_NO_LEAVE( iContentParser->DecodeCandidateLineL( *candidate, *natfwcand ) );
    NSP_ASSERT( _L8("1") == natfwcand->Foundation() );
    NSP_ASSERT( TUint( 1 ) == natfwcand->ComponentId() );
    NSP_ASSERT( KProtocolInetUdp == natfwcand->TransportProtocol() );
    NSP_ASSERT( TUint( 2130003404 ) == natfwcand->Priority() );
    NSP_ASSERT( !natfwcand->TransportDomainAddr().Compare( _L8("www.example.com") ) );
    NSP_ASSERT( TUint( 8998 ) == natfwcand->TransportDomainPort() );
    NSP_ASSERT( CNATFWCandidate::EHost == natfwcand->Type() );
    
    CleanupStack::PopAndDestroy( natfwcand );
    CleanupStack::PopAndDestroy( candidate );
    candidate = CSdpAttributeField::DecodeLC( KCandidateLineInvalid );
    natfwcand = CNATFWCandidate::NewLC();

    // FAILURE DUE TO INVALID CANDIDATE LINE
    NSP_ASSERT_LEAVE( iContentParser->DecodeCandidateLineL( *candidate, *natfwcand ) );
    NSP_ASSERT( _L8("1") == natfwcand->Foundation() );
    
    CleanupStack::PopAndDestroy( natfwcand );
    CleanupStack::PopAndDestroy( candidate );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_EndodeRemoteCandidatesLineL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_EndodeRemoteCandidatesLineL()
    {
    TBuf8<255> address;
    TUint integer;
    CSdpAttributeField* attribute = NULL;
    
    RPointerArray<CNATFWCandidate> remotecand;
    CleanupStack::PushL( TCleanupItem( CNSPContentParser::CleanupArrayItem, &remotecand ) );
    
    CNATFWCandidate* natfwcand = CNATFWCandidate::NewLC();
    natfwcand->SetComponentId( 1 );
    natfwcand->SetTransportDomainAddrL( _L8( "10.0.0.3" ), 8998 );
    User::LeaveIfError( remotecand.Append( natfwcand ) );
    CleanupStack::Pop( natfwcand );
    
    CNATFWCandidate* natfwcand2 = CNATFWCandidate::NewLC();
    natfwcand2->SetComponentId( 2 );
    natfwcand2->SetTransportDomainAddrL( _L8( "10.0.0.18" ), 2321 );
    User::LeaveIfError( remotecand.Append( natfwcand2 ) );
    CleanupStack::Pop( natfwcand2 );
    
    // OK
    NSP_ASSERT_NO_LEAVE( attribute = iContentParser->EncodeRemoteCandidatesL( remotecand ) );
    CleanupStack::PushL( attribute );
    NSP_ASSERT( iContentParser->iRemoteCandidates == attribute->Attribute() );
    
    TLex8 lexer( attribute->Value() );
    
    // OK, CHECK CANDIDATE ONE
    NSP_ASSERT_NO_LEAVE( iContentParser->LexTDesC8ToTUintL( lexer.NextToken(), integer ) );
    NSP_ASSERT( integer == natfwcand->ComponentId() );
    NSP_ASSERT( !lexer.NextToken().Compare( natfwcand->TransportDomainAddr() ) );
    NSP_ASSERT_NO_LEAVE( iContentParser->LexTDesC8ToTUintL( lexer.NextToken(), integer ) );
    NSP_ASSERT( integer == natfwcand->TransportDomainPort() );
    
    // OK, CHECK CANDIDATE TWO
    NSP_ASSERT_NO_LEAVE( iContentParser->LexTDesC8ToTUintL( lexer.NextToken(), integer ) );
    NSP_ASSERT( integer == natfwcand2->ComponentId() );
    NSP_ASSERT( !lexer.NextToken().Compare( natfwcand2->TransportDomainAddr() ) );
    NSP_ASSERT_NO_LEAVE( iContentParser->LexTDesC8ToTUintL( lexer.NextToken(), integer ) );
    NSP_ASSERT( integer == natfwcand2->TransportDomainPort() );
    
    CleanupStack::PopAndDestroy( attribute );
    CleanupStack::PopAndDestroy( &remotecand );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_DecodeRemoteCandidatesLineL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_DecodeRemoteCandidatesLineL()
    {
    _LIT8( KValid, "1 121.123.123.12 12321 2 132.231.121.123 2312" );
    _LIT8( KInvalid, "1 121.123.123.12 2312 2 132.231.121.123 wafr" );
    RPointerArray<CNATFWCandidate> remotecand;
    CleanupStack::PushL( TCleanupItem( CNSPContentParser::CleanupArrayItem, &remotecand ) );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->DecodeRemoteCandidatesL( KValid(), remotecand ) );
    NSP_ASSERT( 2 == remotecand.Count() );
    NSP_ASSERT( TUint(1) == remotecand[0]->ComponentId() );
    NSP_ASSERT( !remotecand[0]->TransportDomainAddr().Compare( _L8("121.123.123.12") ) );
    NSP_ASSERT( TUint(12321) == remotecand[0]->TransportDomainPort() );
    NSP_ASSERT( TUint(2) == remotecand[1]->ComponentId() );
    NSP_ASSERT( !remotecand[1]->TransportDomainAddr().Compare( _L8("132.231.121.123") ) );
    NSP_ASSERT( TUint(2312) == remotecand[1]->TransportDomainPort() );
    
    remotecand.ResetAndDestroy();
    
    // FAILURE DUE TO INVALID PORT IN RTCP COMPONENT
    NSP_ASSERT_LEAVE( iContentParser->DecodeRemoteCandidatesL( KInvalid(), remotecand ) );
    NSP_ASSERT( 1 == remotecand.Count() );
    NSP_ASSERT( TUint(1) == remotecand[0]->ComponentId() );
    NSP_ASSERT( !remotecand[0]->TransportDomainAddr().Compare( _L8("121.123.123.12") ) );
    NSP_ASSERT( TUint(2312) == remotecand[0]->TransportDomainPort() );
    
    CleanupStack::PopAndDestroy(); // remotecand
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_LexTPtrC8ToTUintL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_LexTPtrC8ToTUintL()
    {
    _LIT8( KTUintText, "1" );
    _LIT8( KTIntNegText, "-10" );
    _LIT8( KCharText, "w" );
    TUint unsignedInteger( 0 );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->LexTDesC8ToTUintL( KTUintText(), unsignedInteger ) );
    NSP_ASSERT( TUint( 1 ) == unsignedInteger );
    
    // FAILURE DUE TO NEGATIVE VALUE
    NSP_ASSERT_LEAVE( iContentParser->LexTDesC8ToTUintL( KTIntNegText(), unsignedInteger ) );
    
    // FAILURE DUE TO CHARACTER
    NSP_ASSERT_LEAVE( iContentParser->LexTDesC8ToTUintL( KCharText(), unsignedInteger ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_SolveTransportProtocolL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_SolveTransportProtocolL()
    {
    _LIT8( KProtocolUdp, "udp" );
    _LIT8( KProtocolTcp, "tcp" );
    _LIT8( KProtocolKiosk, "kiosk" );
    TUint protocol;
    
    // OK, UDP
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveTransportProtocolL( KProtocolUdp(), protocol ) );
    NSP_ASSERT( KProtocolInetUdp == protocol );
    
    // OK, TCP
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveTransportProtocolL( KProtocolTcp(), protocol ) );
    NSP_ASSERT( KProtocolInetTcp == protocol );
    
    // FAILURE, UNKNOWN PROTOCOL
    NSP_ASSERT_LEAVE( iContentParser->SolveTransportProtocolL( KProtocolKiosk(), protocol ) );
    NSP_ASSERT( KProtocolInetTcp == protocol );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_InputTPtr8ToTInetAddrL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_InputTPtr8ToTInetAddrL()
    {
    _LIT8( KAddressText, "10.0.0.1" );
    _LIT8( KInvAddressText, "string" );
    TInetAddr address;
    
    // OK, VALID ADDRESS
    NSP_ASSERT_NO_LEAVE( iContentParser->InputTDesC8ToTInetAddrL( KAddressText(), address ) );
    NSP_ASSERT( address.Address() == INET_ADDR(10,0,0,1) );
    
    // FAILURE, CANT CONVERT THIS STRING TO INETADDRESS
    NSP_ASSERT_LEAVE( iContentParser->InputTDesC8ToTInetAddrL( KInvAddressText(), address ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_AppendNumToTDes8L
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_AppendNumToTDes8L()
    {
    _LIT8( KNumString, "15523552" );
    TBuf8<5> buffer;
    TBuf8<15> buffer2;
    TUint number = 15523552;
    
    // FAILURE DUE TO TOO SHORT DESCRIPTOR
    NSP_ASSERT_LEAVE( iContentParser->AppendNumToTDes8L( buffer, number ) );
    NSP_ASSERT( KErrNotFound == buffer2.FindF( KNumString() ) );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->AppendNumToTDes8L( buffer2, number ) );
    NSP_ASSERT( KErrNotFound != buffer2.FindF( KNumString() ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_AppendProtocolToTDes8L
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_AppendProtocolToTDes8L()
    {
    _LIT8( KUdpProto, "udp" );
    TBuf8<2> buffer;
    TBuf8<15> buffer2;
    TUint proto = KProtocolInetUdp;
    
    // FAILURE DUE TO TOO SHORT DESCRIPTOR
    NSP_ASSERT_LEAVE( iContentParser->AppendProtocolToTDes8L( buffer, proto ) );
    NSP_ASSERT( KErrNotFound == buffer.FindF( KUdpProto() ) );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->AppendProtocolToTDes8L( buffer2, proto ) );
    NSP_ASSERT( KErrNotFound != buffer2.FindF( KUdpProto() ) );
    
    proto = 0;
    
    // FAILURE DUE TO UNKNOWN PROTOCOL
    NSP_ASSERT_LEAVE( iContentParser->AppendProtocolToTDes8L( buffer2, proto ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_AppendTInetAddrToTDes8L
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_AppendTInetAddrToTDes8L()
    {
    _LIT8( KAddr, "10.0.0.6" );
    TBuf8<2> buffer;
    TBuf8<50> buffer2;
    TInetAddr address( INET_ADDR(10,0,0,6), 3440 );
    
    // FAILURE DUE TO TOO SHORT DESCRIPTOR
    NSP_ASSERT_LEAVE( iContentParser->AppendTInetAddrToTDes8L( buffer, address ) );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->AppendTInetAddrToTDes8L( buffer2, address ) );
    NSP_ASSERT( KErrNotFound != buffer2.FindF( KAddr() ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_AppendTypeToTDes8L
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_AppendTypeToTDes8L()
    {
    _LIT8( KHost, "typ host" );
    TBuf8<5> buffer;
    TBuf8<15> buffer2;
    CNATFWCandidate::TCandidateType type( CNATFWCandidate::EHost );
    
    // FAILURE DUE TO TOO SHORT BUFFER
    NSP_ASSERT_LEAVE( iContentParser->AppendTypeToTDes8L( buffer, type ) );
    NSP_ASSERT( KErrNotFound == buffer2.FindF( KHost() ) );
    
    // OK, "typ host" ADDED
    NSP_ASSERT_NO_LEAVE( iContentParser->AppendTypeToTDes8L( buffer2, type ) );
    NSP_ASSERT( KErrNotFound != buffer2.FindF( KHost() ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_SolveCandidateTypeL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_SolveCandidateTypeL()
    {
    _LIT8( KTyp, "typ" );
    _LIT8( KHost, "host" );
    _LIT8( KServRef, "srflx" );
    _LIT8( KPeerRef, "prflx" );
    _LIT8( KRelay, "relay" );
	// TCP NOT SUPPORTED YET
    //_LIT8( KActPass, "tcp-so" );
    //_LIT8( KAct, "tcp-act" );
    //_LIT8( KPass, "tcp-pass" );
    CNATFWCandidate::TCandidateType type;
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KHost(), type ) );
    NSP_ASSERT( CNATFWCandidate::EHost == type );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KServRef(), type ) );
    NSP_ASSERT( CNATFWCandidate::EServerReflexive == type );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KPeerRef(), type ) );
    NSP_ASSERT( CNATFWCandidate::EPeerReflexive == type );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KRelay(), type ) );
    NSP_ASSERT( CNATFWCandidate::ERelay == type );
    
    /* // TCP NOT SUPPORTED YET
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KActPass(), type ) );
    NSP_ASSERT( CNATFWCandidate::EActivePassive == type );
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KAct(), type ) );
    NSP_ASSERT( CNATFWCandidate::EActive == type );
    NSP_ASSERT_NO_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KPass(), type ) );
    NSP_ASSERT( CNATFWCandidate::EPassive == type );
    */
    
    // FAILURE DUE TO INVALID TYPE
    NSP_ASSERT_LEAVE( iContentParser->SolveCandidateTypeL( KTyp(), KTyp(), type ) );
    
    // FAILURE DUE TO INVALID ORDER OF STRINGS
    NSP_ASSERT_LEAVE( iContentParser->SolveCandidateTypeL( KHost(), KTyp(), type ) );
    }

// ---------------------------------------------------------------------------
// UT_CNSPContentParser::UT_LeaveIfTooLongL
// ---------------------------------------------------------------------------
// 
void UT_CNSPContentParser::UT_LeaveIfTooLongL()
    {
    _LIT8( KLengthTesting ,"testing leave" );
    TBuf8<20> buffer( KLengthTesting() );
    
    // FAILURE DUE TO TOO SHORT FREE SPACE
    NSP_ASSERT_LEAVE( iContentParser->LeaveIfTooLongL( buffer, 10 ) );
    
    // OK
    NSP_ASSERT_NO_LEAVE( iContentParser->LeaveIfTooLongL( buffer, 2 ) );
    }



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CNSPContentParser,
    "Individual method tests for content parser",
    "UNIT" )

NSP_TEST_METHOD(
	"NewL - test",
	"CNSPContentParser",
	"UT_NewLL",
	"FUNCTIONALITY",
	SetupL, UT_NewLL, Teardown )

NSP_TEST_METHOD(
    "IsIceSupported - test",
    "CNSPContentParser",
    "UT_IsIceSupported",
    "FUNCTIONALITY",
    SetupL, UT_IsIceSupported, Teardown )

NSP_TEST_METHOD(
    "IsMismatchL - test",
    "CNSPContentParser",
    "IsMismatchL",
    "FUNCTIONALITY",
    SetupL, UT_IsMismatchL, Teardown )

NSP_TEST_METHOD(
    "IsMismatchAttribute - test",
    "CNSPContentParser",
    "UT_IsMismatchAttribute",
    "FUNCTIONALITY",
    SetupL, UT_IsMismatchAttribute, Teardown )

NSP_TEST_METHOD(
    "IsLiteAttribute - test",
    "CNSPContentParser",
    "UT_IsLiteAttribute",
    "FUNCTIONALITY",
    SetupL, UT_IsLiteAttribute, Teardown )

NSP_TEST_METHOD(
    "FindRTCP - test",
    "CNSPContentParser",
    "UT_FindRTCP",
    "FUNCTIONALITY",
    SetupL, UT_FindRTCP, Teardown )

NSP_TEST_METHOD(
    "ModifyRTCPL - test",
    "CNSPContentParser",
    "UT_ModifyRTCPL",
    "FUNCTIONALITY",
    SetupL, UT_ModifyRTCPL, Teardown )

NSP_TEST_METHOD(
    "AddLiteL - test",
    "CNSPContentParser",
    "UT_AddLiteL",
    "FUNCTIONALITY",
    SetupL, UT_AddLiteL, Teardown )

NSP_TEST_METHOD(
    "AddMismatchL - test",
    "CNSPContentParser",
    "UT_AddMismatchL",
    "FUNCTIONALITY",
    SetupL, UT_AddMismatchL, Teardown )

NSP_TEST_METHOD(
    "EndodeCandidateLineL - test",
    "CNSPContentParser",
    "UT_EndodeCandidateLineL",
    "FUNCTIONALITY",
    SetupL, UT_EndodeCandidateLineL, Teardown )

NSP_TEST_METHOD(
    "DecodeCandidateLineL - test",
    "CNSPContentParser",
    "UT_DecodeCandidateLineL",
    "FUNCTIONALITY",
    SetupL, UT_DecodeCandidateLineL, Teardown )

NSP_TEST_METHOD(
    "EndodeRemoteCandidatesLineL - test",
    "CNSPContentParser",
    "UT_EndodeRemoteCandidatesLineL",
    "FUNCTIONALITY",
    SetupL, UT_EndodeRemoteCandidatesLineL, Teardown )

NSP_TEST_METHOD(
    "DecodeRemoteCandidatesLineL - test",
    "CNSPContentParser",
    "UT_DecodeRemoteCandidatesLineL",
    "FUNCTIONALITY",
    SetupL, UT_DecodeRemoteCandidatesLineL, Teardown )

NSP_TEST_METHOD(
    "LexTPtrC8ToTUintL - test",
    "CNSPContentParser",
    "UT_LexTPtrC8ToTUintL",
    "FUNCTIONALITY",
    SetupL, UT_LexTPtrC8ToTUintL, Teardown )

NSP_TEST_METHOD(
    "SolveTransportProtocolL - test",
    "CNSPContentParser",
    "UT_SolveTransportProtocolL",
    "FUNCTIONALITY",
    SetupL, UT_SolveTransportProtocolL, Teardown )

NSP_TEST_METHOD(
    "InputTPtr8ToTInetAddrL - test",
    "CNSPContentParser",
    "UT_InputTPtr8ToTInetAddrL",
    "FUNCTIONALITY",
    SetupL, UT_InputTPtr8ToTInetAddrL, Teardown )

NSP_TEST_METHOD(
    "AppendNumToTDes8L - test",
    "CNSPContentParser",
    "UT_AppendNumToTDes8LL",
    "FUNCTIONALITY",
    SetupL, UT_AppendNumToTDes8L, Teardown )

NSP_TEST_METHOD(
    "AppendProtocolToTDes8L - test",
    "CNSPContentParser",
    "UT_AppendProtocolToTDes8L",
    "FUNCTIONALITY",
    SetupL, UT_AppendProtocolToTDes8L, Teardown )

NSP_TEST_METHOD(
    "AppendTInetAddrToTDes8L - test",
    "CNSPContentParser",
    "UT_AppendTInetAddrToTDes8L",
    "FUNCTIONALITY",
    SetupL, UT_AppendTInetAddrToTDes8L, Teardown )

NSP_TEST_METHOD(
    "AppendTypeToTDes8L - test",
    "CNSPContentParser",
    "UT_AppendTypeToTDes8L",
    "FUNCTIONALITY",
    SetupL, UT_AppendTypeToTDes8L, Teardown )

NSP_TEST_METHOD(
    "SolveCandidateTypeL - test",
    "CNSPContentParser",
    "UT_SolveCandidateTypeL",
    "FUNCTIONALITY",
    SetupL, UT_SolveCandidateTypeL, Teardown )

NSP_TEST_METHOD(
    "LeaveIfTooLongL - test",
    "CNSPContentParser",
    "UT_LeaveIfTooLongL",
    "FUNCTIONALITY",
    SetupL, UT_LeaveIfTooLongL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
