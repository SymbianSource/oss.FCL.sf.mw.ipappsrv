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
* Description:  Class implementation for Sdp content parser.
*
*/

#include <e32cmn.h>
#include <in_sock.h>
#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdpattributefield.h>
#include <sdpmediafield.h>
#include "natfwcandidate.h"
#include "natfwcredentials.h"
#include "nspcontentparser.h"
#include "nspdefs.h"

const TInt KAttributesNumber = 7;
static const TText8* const AttributeArray[ KAttributesNumber ] =
    {
    _S8("candidate"),
    _S8("remote-candidates"),
    _S8("ice-lite"),
    _S8("ice-passive"),
    _S8("ice-ufrag"),
    _S8("ice-pwd"),
    _S8("ice-mismatch")
    };
const TInt KIceAttrCandidateIndex = 0;
const TInt KIceAttrRemoteCandidatesIndex = 1;
const TInt KIceAttrLiteIndex = 2;
const TInt KIceAttrPassiveIndex = 3;
const TInt KIceAttrUfragIndex = 4;
const TInt KIceAttrPwdIndex = 5;
const TInt KIceAttrMismatchIndex = 6;

_LIT8( KAttrRtcp, "rtcp" );
_LIT8( KAttrEmpty, " " );
_LIT8( KFieldSeparator, " " );

const TInt KTypeAttrNumber = 9;
static const TText8* const CandidateTypeArray[ KTypeAttrNumber ] = 
    {
    _S8( "typ" ),
    _S8( "host" ),
    _S8( "local" ),
    _S8( "srflx" ),
    _S8( "prflx" ),
    _S8( "relay" ),
    _S8( "tcp-so" ),
    _S8( "tcp-act" ),
    _S8( "tcp-pass" )
    };
const TInt KTypeAttrIndexType = 0;
const TInt KTypeAttrIndexHost = 1;
const TInt KTypeAttrIndexLocal = 2;
const TInt KTypeAttrIndexServerReflex = 3;
const TInt KTypeAttrIndexPeerReflex = 4;
const TInt KTypeAttrIndexRelay = 5;

const TUint KRejecPort = 0;

const TInt KMaxCandidateLength = 200;       // Max for candidate creation.
const TInt KMaxLengthNumberConversion = 40; // Priority field max length.
const TInt KMaxLengthTInetAddr = 40;        // Must support IPv4/IPv6
const TInt KMaxLengthOfFQDN = 255;          // For FQDNs
const TInt KMaxLengthRtcpField = 60;        // Max for rctp attr. value

#define FINDFIELD_L( aField, aMediafield, aFieldarray ) \
    User::LeaveIfNull( aField = CNSPContentParser::FindMediaField( aMediafield, aFieldarray ) )

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// CNSPContentParser::CNSPContentParser
// ---------------------------------------------------------------------------
//
CNSPContentParser::CNSPContentParser()
    {
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::ConstructL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::ConstructL()
    {
    RStringPool pool = SdpCodecStringPool::StringPoolL();
    
    // ICE attributes
    iCandidate = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrCandidateIndex ] ) );
    iRemoteCandidates = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrRemoteCandidatesIndex ] ) );
    iLite = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrLiteIndex ] ) );
    iPassive = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrPassiveIndex ] ) );
    iUfrag = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrUfragIndex ] ) );
    iPwd = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrPwdIndex ] ) );
    iMismatch = pool.OpenFStringL( 
            TPtrC8( AttributeArray[ KIceAttrMismatchIndex ] ) );
    
    // Other
    iRTCP = pool.OpenFStringL( KAttrRtcp() );    
    iUdp = pool.StringF( SdpCodecStringConstants::EProtocolUdp,
                         SdpCodecStringPool::StringTableL() );
    iTcp = pool.StringF( SdpCodecStringConstants::EProtocolTcp,
                         SdpCodecStringPool::StringTableL() );
    iIN = pool.StringF( SdpCodecStringConstants::ENetType,
                         SdpCodecStringPool::StringTableL() );
    iIPv4 = pool.StringF( SdpCodecStringConstants::EAddressTypeIP4,
                         SdpCodecStringPool::StringTableL() );
    iIPv6 = pool.StringF( SdpCodecStringConstants::EAddressType,
                         SdpCodecStringPool::StringTableL() );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::NewL
// ---------------------------------------------------------------------------
//
CNSPContentParser* CNSPContentParser::NewL()
    {
    CNSPContentParser* self = CNSPContentParser::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::NewLC
// ---------------------------------------------------------------------------
//
CNSPContentParser* CNSPContentParser::NewLC()
    {
    CNSPContentParser* self = new( ELeave ) CNSPContentParser;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::CNSPContentParser
// ---------------------------------------------------------------------------
//
CNSPContentParser::~CNSPContentParser()
    {
    iCandidate.Close();
    iRemoteCandidates.Close();
    iLite.Close();
    iPassive.Close();
    iUfrag.Close();
    iPwd.Close();
    iMismatch.Close();
    iRTCP.Close();
    iUdp.Close();
    iTcp.Close();
    iIN.Close();
    iIPv4.Close();
    iIPv6.Close();
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::IsIceSupported
// ---------------------------------------------------------------------------
//
TBool CNSPContentParser::IsIceSupported( CSdpDocument& aDocument ) const
    {
    RPointerArray<CSdpMediaField>& mediafields = aDocument.MediaFields();
    
    const TInt mediafieldcount( mediafields.Count() );
    for ( TInt index = 0; index < mediafieldcount; index++ )
        {
        RPointerArray<CSdpAttributeField>& attributes =
                mediafields[index]->AttributeFields();
        
        const TInt attributecount( attributes.Count() );
        for ( TInt jndex = 0; jndex < attributecount; jndex++ )
            {
            if ( iCandidate == attributes[jndex]->Attribute() )
                {
                return ETrue;
                }
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::IsMismatchL
// ---------------------------------------------------------------------------
//
TBool CNSPContentParser::IsMismatchL( CSdpDocument& aDocument ) const
    {
    const CSdpConnectionField* connField = aDocument.ConnectionField();
    const TDesC8& address = ( connField ? connField->Address() : KNullDesC8 );
    RPointerArray<CSdpMediaField>& mediafields = aDocument.MediaFields();
    
    TBool mismatch( EFalse );
    const TInt mediafieldcount( mediafields.Count() );
    for ( TInt index = 0; index < mediafieldcount && !mismatch; index++ )
        {
        const TDesC8& addr = CONN_ADDR( *mediafields[index], address );
        mismatch = IsMismatchAttribute( *mediafields[index] );
        
        if ( !mismatch && IsMismatchL( *mediafields[index], addr ) )
            {
            AddMismatchL( *mediafields[index] );
            mismatch = ETrue;
            }
        }
    
    return mismatch;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::IsMismatchL
// ---------------------------------------------------------------------------
//
TBool CNSPContentParser::IsMismatchL( CSdpMediaField& aMediaField,
        const TDesC8& aAddress ) const
    {
    const TDesC8& rtpAddr = CONN_ADDR( aMediaField, aAddress );
    const TUint rtpPort = aMediaField.Port();
    
    RBuf8 rtcpAddr;
    rtcpAddr.CreateL( KMaxLengthOfFQDN );
    rtcpAddr.CleanupClosePushL();
    rtcpAddr = rtpAddr;
    TUint rtcpPort = rtpPort;
    TBool isRtcp = FindRTCP( aMediaField, rtcpAddr, rtcpPort );
    
    RPointerArray<CNATFWCandidate> remotecand;
    CleanupStack::PushL( TCleanupItem(
            CNSPContentParser::CleanupArrayItem, &remotecand ) );
    
	GetCandidatesL( aMediaField, remotecand );
    
    const TInt candidatecount( remotecand.Count() );
    TBool mismatch = ( candidatecount ? ETrue : EFalse );
    for ( TInt index = 0; index < candidatecount; index++ )
        {
        const TDesC8& addr = remotecand[index]->TransportDomainAddr();
        const TUint port = remotecand[index]->TransportDomainPort();
        
        if ( ( !rtpAddr.Compare( addr ) && port == rtpPort ) ||
             ( isRtcp && !rtcpAddr.Compare( addr ) && port == rtcpPort ) )
            {
            mismatch = EFalse;
            break;
            }
        }
    
    CleanupStack::PopAndDestroy( &remotecand );
    CleanupStack::PopAndDestroy( &rtcpAddr );
    
    NSPLOG_INT( "CNSPContentParser::IsMismatchL, ret:", mismatch )
    return mismatch;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::IsMismatchAttribute
// ---------------------------------------------------------------------------
//
TBool CNSPContentParser::IsMismatchAttribute( CSdpMediaField& aMediaField ) const
    {
    RPointerArray<CSdpAttributeField>& attributes = aMediaField.AttributeFields();
    
    const TInt attributecount( attributes.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iMismatch == attributes[index]->Attribute() )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::IsLiteAttribute
// ---------------------------------------------------------------------------
//
TBool CNSPContentParser::IsLiteAttribute( CSdpDocument& aDocument ) const
    {
    const RPointerArray<CSdpAttributeField>& attributes =
            aDocument.AttributeFields();
    
    const TInt attributecount( attributes.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iLite == attributes[index]->Attribute() )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::FindRTCP
// ---------------------------------------------------------------------------
//
TBool CNSPContentParser::FindRTCP( CSdpMediaField& aMediaField,
        TDes8& aAddress, TUint& aPort ) const
    {
    const RPointerArray<CSdpAttributeField>& attributes =
            aMediaField.AttributeFields();
    
    const TInt attributecount( attributes.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iRTCP == attributes[index]->Attribute() )
            {
            TLex8 lexer( attributes[index]->Value() );
            
            TUint port( KRejecPort );
            TRAPD( error, LexTDesC8ToTUintL( lexer.NextToken(), port ) );
            aPort = ( KErrNone == error ? port : aPort );
            
            if ( KErrNone == error && !lexer.Eos() )
                {
                lexer.NextToken(); // nettype, not needed
                lexer.NextToken(); // addresstype, not needed
                aAddress = lexer.NextToken();
                }
            
            return ETrue;
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::ModifyRTCPL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::ModifyRTCPL( CSdpMediaField& aMediaField,
        const TDesC8& aAddress, TUint aPort ) const
    {
    TInetAddr address;
    CSdpAttributeField* attr = NULL;
    const RPointerArray<CSdpAttributeField>& attributes =
            aMediaField.AttributeFields();
    
    const TInt attributecount( attributes.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        attr = attributes[index];
        
        if ( iRTCP == attr->Attribute() )
            {
            HBufC8* newattribute = HBufC8::NewLC( KMaxLengthRtcpField );
            TPtr8 ptr( newattribute->Des() );
            
            AppendNumToTDes8L( ptr, aPort );
            AppendTDesC8ToTDes8L( ptr, iIN.DesC() );
            InputTDesC8ToTInetAddrL( aAddress, address );
            AppendTDesC8ToTDes8L( ptr, KAfInet == address.Family() ?
                    iIPv4.DesC() : iIPv6.DesC() );
            AppendTDesC8ToTDes8L( ptr, aAddress, EFalse );
            
            attr->SetL( attr->Attribute(), newattribute->Des() );
            CleanupStack::PopAndDestroy( newattribute );
            break;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddLiteL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddLiteL( CSdpDocument& aDocument ) const
    {
    CSdpAttributeField* attr = CSdpAttributeField::NewLC( iLite, KNullDesC8 );
    User::LeaveIfError( aDocument.AttributeFields().Append( attr ) );
    CleanupStack::Pop( attr );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddMismatchL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddMismatchL( CSdpDocument& aMismatch,
        CSdpDocument& aMismatched ) const
    {
    CSdpMediaField* field = NULL;
    RPointerArray<CSdpMediaField>& mismatchmedia = aMismatch.MediaFields();
    RPointerArray<CSdpMediaField>& mismatchedmedia = aMismatched.MediaFields();
    
    const TInt mediacount( mismatchmedia.Count() );
    for ( TInt index = 0; index < mediacount; index++ )
        {
        if ( IsMismatchAttribute( *mismatchmedia[index] ) )
            {
            FINDFIELD_L( field, *mismatchmedia[index], mismatchedmedia );
            AddMismatchL( *field );
            RemoveIceContent( *mismatchmedia[index] );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddMismatchL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddMismatchL( CSdpMediaField& aMediaField ) const
    {
    CSdpAttributeField* attr = CSdpAttributeField::NewLC( iMismatch, KNullDesC8 );
    User::LeaveIfError( aMediaField.AttributeFields().Append( attr ) );
    CleanupStack::Pop( attr );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddCandidatesL( CSdpMediaField& aMediaField,
        const RPointerArray<CNATFWCandidate>& aCandidates ) const
    {
    CSdpAttributeField* attr = NULL;
    RPointerArray<CSdpAttributeField>& attributeArray =
            aMediaField.AttributeFields();
    
    const TInt candidatecount( aCandidates.Count() );
    for ( TInt index = 0; index < candidatecount; index++ )
        {
        TRAPD( error, attr = EncodeCandidateLineL( *aCandidates[index] ) )
        User::LeaveIfError( KErrNone == error ?
                attributeArray.Append( attr ) : error );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddRemoteCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddRemoteCandidatesL( CSdpMediaField& aMediaField,
        const RPointerArray<CNATFWCandidate>& aCandidates ) const
    {
    RPointerArray<CSdpAttributeField>& attributeArray =
            aMediaField.AttributeFields();
    
    CSdpAttributeField* attribute = EncodeRemoteCandidatesL( aCandidates );
    CleanupStack::PushL( attribute );
    User::LeaveIfError( attributeArray.Append( attribute ) );
    CleanupStack::Pop( attribute );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddCredentialsL( CSdpMediaField& aMediaField,
        const CNATFWCredentials& aCredentials ) const
    {
    RPointerArray<CSdpAttributeField>& attributeArray =
            aMediaField.AttributeFields();
    
    CSdpAttributeField* user = EncodeUfragL( aCredentials );
    CleanupStack::PushL( user );
    User::LeaveIfError( attributeArray.Append( user ) );
    CleanupStack::Pop( user );
    
    CSdpAttributeField* pwd = EncodePwdL( aCredentials );
    CleanupStack::PushL( pwd );
    User::LeaveIfError( attributeArray.Append( pwd ) );
    CleanupStack::Pop( pwd );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AddCredentialsL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AddCredentialsL( CSdpDocument& aDocument,
        const CNATFWCredentials& aCredentials ) const
    {
    RPointerArray<CSdpAttributeField>& attributeArray =
            aDocument.AttributeFields();
    
    CSdpAttributeField* user = EncodeUfragL( aCredentials );
    CleanupStack::PushL( user );
    User::LeaveIfError( attributeArray.Append( user ) );
    CleanupStack::Pop( user );
    
    CSdpAttributeField* pwd = EncodePwdL( aCredentials );
    CleanupStack::PushL( pwd );
    User::LeaveIfError( attributeArray.Append( pwd ) );
    CleanupStack::Pop( pwd );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::GetCandidatesL
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::GetCandidatesL( CSdpMediaField& aMediaField,
        RPointerArray<CNATFWCandidate>& aCandidates ) const
    {
    TInt warning = KErrNone;
    CNATFWCandidate* cand = NULL;
    const RPointerArray<CSdpAttributeField>& attrArray =
            aMediaField.AttributeFields();
    
    const TInt attributecount( attrArray.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iCandidate == attrArray[index]->Attribute() )
            {
            cand = CNATFWCandidate::NewLC();
            
            TRAPD( error, DecodeCandidateLineL( *attrArray[index], *cand ) );
            
            if ( KErrNone == error )
                {
                User::LeaveIfError( aCandidates.Append( cand ) );
                CleanupStack::Pop( cand );
                }
            else if ( KErrNoMemory == error )
            	{
            	User::Leave( KErrNoMemory );
            	}
            else
                {
                warning = error;
                CleanupStack::PopAndDestroy( cand );
                }
            }
        }
    
    return warning;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::GetRemoteCandidatesL
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::GetRemoteCandidatesL( CSdpDocument& aDocument,
        RPointerArray<CNATFWCandidate>& aRemoteCands ) const
    {
    const RPointerArray<CSdpMediaField>& mediafieldArray =
            aDocument.MediaFields();
    
    const TInt mediafieldcount( mediafieldArray.Count() );
    for ( TInt index = 0; index < mediafieldcount; index++ )
        {
        GetRemoteCandidatesL( *mediafieldArray[index], aRemoteCands );
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::GetRemoteCandidatesL
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::GetRemoteCandidatesL( CSdpMediaField& aMediaField,
        RPointerArray<CNATFWCandidate>& aRemoteCands ) const
    {
    const RPointerArray<CSdpAttributeField>& attributeArray =
            aMediaField.AttributeFields();
    
    const TInt attributecount( attributeArray.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iRemoteCandidates == attributeArray[index]->Attribute() )
            {
            DecodeRemoteCandidatesL( attributeArray[index]->Value(),
                                     aRemoteCands );
            }
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::GetCredentialsL
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::GetCredentialsL( CSdpMediaField& aMediaField,
        CNATFWCredentials& aCredentials ) const
    {
    const RPointerArray<CSdpAttributeField>& attributeArray =
            aMediaField.AttributeFields();
    
    const TInt attributecount( attributeArray.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iUfrag == attributeArray[index]->Attribute() )
            {
            DecodeUfragL( *attributeArray[index], aCredentials );
            }
        else if ( iPwd == attributeArray[index]->Attribute() )
            {
            DecodePwdL( *attributeArray[index], aCredentials );
            }
        else
            {
            // Nothing to do here
            }
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::GetCredentialsL
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::GetCredentialsL( CSdpDocument& aDocument,
        CNATFWCredentials& aCredentials ) const
    {
    const RPointerArray<CSdpAttributeField>& attributeArray =
            aDocument.AttributeFields();
    
    const TInt attributecount( attributeArray.Count() );
    for ( TInt index = 0; index < attributecount; index++ )
        {
        if ( iUfrag == attributeArray[index]->Attribute() )
            {
            DecodeUfragL( *attributeArray[index], aCredentials );
            }
        else if ( iPwd == attributeArray[index]->Attribute() )
            {
            DecodePwdL( *attributeArray[index], aCredentials );
            }
        else
            {
            // Nothing to do here
            }
        }
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::RemoveIceContent
// ---------------------------------------------------------------------------
//
void CNSPContentParser::RemoveIceContent( CSdpDocument& aDocument ) const
    {
    RPointerArray<CSdpAttributeField>& attributes = aDocument.AttributeFields();
    
    for ( TInt index = 0; index < attributes.Count() ; )
        {
        RStringF attribute = attributes[index]->Attribute();
        
        if ( iCandidate == attribute ||
             iRemoteCandidates == attribute ||
             iLite == attribute ||
             iPassive == attribute ||
             iUfrag == attribute ||
             iPwd == attribute || 
             iMismatch == attribute )
            {
            delete ( attributes[index] );
            attributes.Remove( index );
            attributes.Compress();
            }
        else
            {
            index++;
            }
        }
    
    RPointerArray<CSdpMediaField>& mediafields = aDocument.MediaFields();
    
    const TInt mediafieldcount( mediafields.Count() );
    for ( TInt index = 0; index < mediafieldcount ; index++ )
        {
        RemoveIceContent( *mediafields[index] );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::RemoveIceContent
// ---------------------------------------------------------------------------
//
void CNSPContentParser::RemoveIceContent( CSdpMediaField& aMediaField ) const
    {
    RPointerArray<CSdpAttributeField>& attributes = aMediaField.AttributeFields();
    
    for ( TInt index = 0; index < attributes.Count() ; )
        {
        RStringF attribute = attributes[index]->Attribute();
        
        if ( iCandidate == attribute ||
             iRemoteCandidates == attribute ||
             iLite == attribute ||
             iPassive == attribute ||
             iUfrag == attribute ||
             iPwd == attribute ||
             iMismatch == attribute )
            {
            delete ( attributes[index] );
            attributes.Remove( index );
            attributes.Compress();
            }
        else
            {
            index++;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::EncodeCandidateLineL
// ---------------------------------------------------------------------------
//
CSdpAttributeField* CNSPContentParser::EncodeCandidateLineL(
        const CNATFWCandidate& aCandidate ) const
    {
    HBufC8* content = HBufC8::NewLC( KMaxCandidateLength );
    TPtr8 ptr( content->Des() );
    
    // 1. Foundation
    AppendTDesC8ToTDes8L( ptr, aCandidate.Foundation() );
    
    // 2. Component Id
    AppendNumToTDes8L( ptr, aCandidate.ComponentId() );
    
    // 3. Protocol
    AppendProtocolToTDes8L( ptr, aCandidate.TransportProtocol() );
    
    // 4. Priority
    AppendNumToTDes8L( ptr, aCandidate.Priority() );
    
    // 5. IP address & Port
    AppendTDesC8ToTDes8L( ptr, aCandidate.TransportDomainAddr() );
    AppendNumToTDes8L( ptr, aCandidate.TransportDomainPort() );
    
    // 6. Type
    AppendTypeToTDes8L( ptr, aCandidate.Type(), EFalse );
    
    // 7. Optional information, i.e. related address and etc.
    
    CSdpAttributeField* attribute = CSdpAttributeField::NewL( iCandidate, *content );
    CleanupStack::PopAndDestroy( content );
    
    return attribute;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::DecodeCandidateLineL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::DecodeCandidateLineL(
        const CSdpAttributeField& aAttribute, CNATFWCandidate& aCandidate ) const
    {
    TLex8 lex( aAttribute.Value() );
    TUint unsignedInteger( 0 );
    
    // 1. Foundation( char(s) )
    aCandidate.SetFoundationL( lex.NextToken() );
    
    // 2. Component Id( single digit )
    LexTDesC8ToTUintL( lex.NextToken(), unsignedInteger );
    aCandidate.SetComponentId( unsignedInteger );
    
    // 3. Protocol( string: UDP, TCP )
    TUint protocol( KProtocolInetUdp );
    SolveTransportProtocolL( lex.NextToken(), protocol );
    aCandidate.SetTransportProtocol( protocol );
    
    // 4. Priority( multiple digits, positive large integer )
    LexTDesC8ToTUintL( lex.NextToken(), unsignedInteger );
    aCandidate.SetPriority( unsignedInteger );
    
    // 5. IP address & Port
    TPtrC8 address = lex.NextToken();
    LexTDesC8ToTUintL( lex.NextToken(), unsignedInteger );
    aCandidate.SetTransportDomainAddrL( address, unsignedInteger );
    
    // 6. Type( typ "value" )
    CNATFWCandidate::TCandidateType type;
    TPtrC8 typ = lex.NextToken();
    TPtrC8 value = lex.NextToken();
    SolveCandidateTypeL( typ, value, type );
    aCandidate.SetType( type );
    
    // 7. Optional information, i.e. related address and etc.
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::EncodeRemoteCandidatesL
// ---------------------------------------------------------------------------
//
CSdpAttributeField* CNSPContentParser::EncodeRemoteCandidatesL(
        const RPointerArray<CNATFWCandidate>& aCandidates ) const
    {
    HBufC8* content = HBufC8::NewLC( KMaxCandidateLength );
    TPtr8 ptr( content->Des() );
    
    const TInt remotecandidatecount( aCandidates.Count() );
    for ( TInt index = 0; index < remotecandidatecount; index++ )
        {
        EncodeRemoteCandidateL( ptr, *aCandidates[index] );
        }

    CSdpAttributeField* attribute = CSdpAttributeField::NewL(
            iRemoteCandidates, *content );
    CleanupStack::PopAndDestroy( content );
    return attribute;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::EncodeRemoteCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::EncodeRemoteCandidateL( TDes8& aBuffer,
        const CNATFWCandidate& aCandidate ) const
    {
    // 1. Component Id( single digit )
    AppendNumToTDes8L( aBuffer, aCandidate.ComponentId() );
    
    // 2. IP address & Port
    AppendTDesC8ToTDes8L( aBuffer, aCandidate.TransportDomainAddr() );
    AppendNumToTDes8L( aBuffer, aCandidate.TransportDomainPort() );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::DecodeRemoteCandidatesL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::DecodeRemoteCandidatesL( const TDesC8& aValue,
        RPointerArray<CNATFWCandidate>& aCandidates ) const
    {
    TLex8 lex( aValue );
    TUint unsignedInteger( 0 );
    
    CNATFWCandidate* candidate = CNATFWCandidate::NewLC();
    
    // 1. Component Id( single digit )
    LexTDesC8ToTUintL( lex.NextToken(), unsignedInteger );
    candidate->SetComponentId( unsignedInteger );
    
    // 2. IP address & Port
    const TPtrC8 address = lex.NextToken();
    LexTDesC8ToTUintL( lex.NextToken(), unsignedInteger );
    candidate->SetTransportDomainAddrL( address, unsignedInteger );
    
    User::LeaveIfError( aCandidates.Append( candidate ) );
    CleanupStack::Pop( candidate );
    
    if ( !lex.Eos() )
        {
        DecodeRemoteCandidatesL( lex.Remainder(), aCandidates );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::EncodeUfragL
// ---------------------------------------------------------------------------
//
CSdpAttributeField* CNSPContentParser::EncodeUfragL(
        const CNATFWCredentials& aCredentials ) const
    {
    CSdpAttributeField* attribute = NULL;
    const TDesC8& username = aCredentials.Username();
    
    if ( !username.Length() )
        {
        attribute = CSdpAttributeField::NewL( iUfrag, KAttrEmpty );
        }
    else
        {
        attribute = CSdpAttributeField::NewL( iUfrag, username );
        }
    
    return attribute;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::DecodeUfragL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::DecodeUfragL( const CSdpAttributeField& aAttribute,
        CNATFWCredentials& aCredentials ) const
    {
    aCredentials.SetUsernameL( aAttribute.Value() );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::EncodePwdL
// ---------------------------------------------------------------------------
//
CSdpAttributeField* CNSPContentParser::EncodePwdL(
        const CNATFWCredentials& aCredentials ) const
    {
    CSdpAttributeField* attribute = NULL;
    const TDesC8& password = aCredentials.Password();
    
    if ( !password.Length() )
        {
        attribute = CSdpAttributeField::NewL( iPwd, KAttrEmpty );
        }
    else
        {
        attribute = CSdpAttributeField::NewL( iPwd, password );
        }
    
    return attribute;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::DecodePwdL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::DecodePwdL( const CSdpAttributeField& aAttribute,
        CNATFWCredentials& aCredentials ) const
    {
    aCredentials.SetPasswordL( aAttribute.Value() );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::LexTDesC8ToTUintL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::LexTDesC8ToTUintL( const TDesC8& aDesC8,
        TUint& aUint ) const
    {
    TLex8 lex8( aDesC8 );
    User::LeaveIfError( lex8.Val( aUint, EDecimal ) );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::SolveTransportProtocolL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::SolveTransportProtocolL( const TDesC8& aDesC8,
        TUint& aProto ) const
    {
    if ( !aDesC8.CompareF( iUdp.DesC() ) )
        {
        aProto = KProtocolInetUdp;
        }
    else if ( !aDesC8.CompareF( iTcp.DesC() ) )
        {
        aProto = KProtocolInetTcp;
        }
    else
        {
        User::Leave( KErrArgument );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::InputTDesC8ToTInetAddrL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::InputTDesC8ToTInetAddrL( const TDesC8& aDesC8,
        TInetAddr& aAddress ) const
    {
    HBufC* buffer = HBufC::NewLC( aDesC8.Length() );
    TPtr ptr( buffer->Des() );
    ptr.Copy( aDesC8 );
    User::LeaveIfError( aAddress.Input( buffer->Des() ) );
    CleanupStack::PopAndDestroy( buffer );
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::SolveCandidateTypeL
// ---------------------------------------------------------------------------
//    
void CNSPContentParser::SolveCandidateTypeL( const TDesC8& aDesC8Typ,
        const TDesC8& aDesC8Value, CNATFWCandidate::TCandidateType& aType ) const
    {
    TPtrC8 type( CandidateTypeArray[ KTypeAttrIndexType ] );
    User::LeaveIfError( !aDesC8Typ.CompareF( type ) );
    
    const TInt candidatetypeindex = FindCandidateType( aDesC8Value );
    
    if ( KErrNotFound == candidatetypeindex )
        {
        NSPLOG_STR(
        "CNSPContentParser::SolveCandidateTypeL(),KErrNotFound" )
        User::Leave( KErrArgument );
        }
    
    switch ( candidatetypeindex )
        {
        case KTypeAttrIndexHost:
        case KTypeAttrIndexLocal:
            {
            aType = CNATFWCandidate::EHost;
            break;
            }
        
        case KTypeAttrIndexServerReflex:
            {
            aType = CNATFWCandidate::EServerReflexive;
            break;
            }
        
        case KTypeAttrIndexPeerReflex:
            {
            aType = CNATFWCandidate::EPeerReflexive;
            break;
            }
        
        case KTypeAttrIndexRelay:
            {
            aType = CNATFWCandidate::ERelay;
            break;
            }
        
        default:
            {
            NSPLOG_STR(
            "CNSPContentParser::SolveCandidateTypeL(),default!" )
            User::Leave( KErrArgument ); // should never come here
            break;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::FindCandidateType
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::FindCandidateType( const TDesC8& aDesC8Value ) const
    {
    for ( TInt index = 0; index < KTypeAttrNumber; index++ )
        {
        if ( !aDesC8Value.CompareF( TPtrC8( CandidateTypeArray[index] ) ) )
            {
            return index;
            }
        }
    
    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::FindCandidateType
// ---------------------------------------------------------------------------
//
TInt CNSPContentParser::FindCandidateTypeString(
        const CNATFWCandidate::TCandidateType& aType ) const
    {
    switch ( aType )
        {
        case CNATFWCandidate::EHost:
            {
            return KTypeAttrIndexHost;
            }
        
        case CNATFWCandidate::EServerReflexive:
            {
            return KTypeAttrIndexServerReflex;
            }
        
        case CNATFWCandidate::EPeerReflexive:
            {
            return KTypeAttrIndexPeerReflex;
            }
        
        case CNATFWCandidate::ERelay:
            {
            return KTypeAttrIndexRelay;
            }
        
        default:
            {
            return KErrNotFound;
            }
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AppendTDesC8ToTDes8L
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AppendTDesC8ToTDes8L( TDes8& aDes8,
        const TDesC8& aDesC8, TBool aAddFieldSeparator ) const
    {
    LeaveIfTooLongL( aDes8, aDesC8.Length() );
    aDes8.Append( aDesC8 );
    
    if ( aAddFieldSeparator )
        {
        LeaveIfTooLongL( aDes8, 1 );
        aDes8.Append( KFieldSeparator );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AppendNumToTDes8L
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AppendNumToTDes8L( TDes8& aDes8,
        TUint aNumber, TBool aAddFieldSeparator ) const
    {
    TBuf< KMaxLengthNumberConversion > buffer;
    buffer.AppendNum( aNumber, EDecimal );
    LeaveIfTooLongL( aDes8, buffer.Length() );
    aDes8.AppendNum( aNumber, EDecimal );
    
    if ( aAddFieldSeparator )
        {
        LeaveIfTooLongL( aDes8, 1 );
        aDes8.Append( KFieldSeparator );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AppendProtocolToTDes8L
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AppendProtocolToTDes8L( TDes8& aDes8,
        const TUint& aProtocol, TBool aAddFieldSeparator ) const
    {
    LeaveIfTooLongL( aDes8, Max( iUdp.DesC().Length(), iTcp.DesC().Length() ) );
    
    if ( KProtocolInetUdp == aProtocol )
        {
        aDes8.Append( iUdp.DesC() );
        }
    else if ( KProtocolInetTcp == aProtocol )
        {
        aDes8.Append( iTcp.DesC() );
        }
    else // EUndefined
        {
        User::Leave( KErrNotSupported );
        }
    
    if ( aAddFieldSeparator )
        {
        LeaveIfTooLongL( aDes8, 1 );
        aDes8.Append( KFieldSeparator );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AppendTInetAddrToTDes8L
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AppendTInetAddrToTDes8L( TDes8& aDes8,
        const TInetAddr& aAddress, TBool aAddFieldSeparator ) const
    {
    TBuf<KMaxLengthTInetAddr> buffer;
    aAddress.Output( buffer );
    LeaveIfTooLongL( aDes8, buffer.Length() );
    aDes8.Append( buffer );
    
    if ( aAddFieldSeparator )
        {
        LeaveIfTooLongL( aDes8, 1 );
        aDes8.Append( KFieldSeparator );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::AppendTypeToTDes8L
// ---------------------------------------------------------------------------
//
void CNSPContentParser::AppendTypeToTDes8L( TDes8& aDes8,
        const CNATFWCandidate::TCandidateType& aType,
        TBool aAddFieldSeparator ) const
    {
    const TInt candidatetypeindex = FindCandidateTypeString( aType );
    
    if ( KErrNotFound != candidatetypeindex )
        {
        TPtrC8 type( CandidateTypeArray[ KTypeAttrIndexType ] );
        TPtrC8 value( CandidateTypeArray[ candidatetypeindex ] );
        
        LeaveIfTooLongL( aDes8, type.Length() + 
                KFieldSeparator().Length() + value.Length() );
        
        aDes8.Append( type );
        aDes8.Append( KFieldSeparator );
        aDes8.Append( value );
        
        if ( aAddFieldSeparator )
            {
            LeaveIfTooLongL( aDes8, 1 );
            aDes8.Append( KFieldSeparator );
            }
        }
    else
        {
        NSPLOG_STR(
        "CNSPContentParser::AppendTypeToTDes8L(),KErrNotFound!" )
        User::Leave( KErrArgument );
        }
    }


// ---------------------------------------------------------------------------
// CNSPContentParser::LeaveIfTooLongL
// ---------------------------------------------------------------------------
//
void CNSPContentParser::LeaveIfTooLongL( const TDes8& aDes8,
        const TInt aLength ) const
    {
    const TInt free = aDes8.MaxLength() - aDes8.Length();
    
    if ( free < aLength )
        {
        User::Leave( KErrTooBig );
        }
    }

// ---------------------------------------------------------------------------
// CNSPContentParser::FindMediaField
// ---------------------------------------------------------------------------
//
CSdpMediaField* CNSPContentParser::FindMediaField( const CSdpMediaField& aMediaField,
        const RPointerArray<CSdpMediaField>& aMediaFields ) const
    {
    const TInt mediafieldscount( aMediaFields.Count() );
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        if ( aMediaFields[index]->Media() == aMediaField.Media() )
            {
            return aMediaFields[index];
            }
        }
    
    return NULL;    
    }

// ---------------------------------------------------------------------------
// CNSPContentParser::CleanupArrayItem
// ---------------------------------------------------------------------------
//
void CNSPContentParser::CleanupArrayItem( TAny* anArray )
    {
    if ( anArray )
        {
        reinterpret_cast< RPointerArray<CNATFWCandidate>* >( anArray )->
            ResetAndDestroy();
        reinterpret_cast< RPointerArray<CNATFWCandidate>* >( anArray )->
            Close();
        }
    }


// end of file
