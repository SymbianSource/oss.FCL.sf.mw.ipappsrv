/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Implementation of NAT FW STUN server implementation
*
*/




#include <e32def.h>
#include <es_sock.h>
#include <in_sock.h>

#include "mncmconnectionmultiplexer.h"
#include "natfwstunsrvimpl.h"
#include "natfwcredentials.h"
#include "natfwstunserverlogs.h"
#include "natfwunsafbindingerrorresponse.h"
#include "natfwunsafbindingresponse.h"
#include "natfwunsaferrorcodeattribute.h"
#include "natfwunsafusernameattribute.h"
#include "natfwunsafmessagefactory.h"
#include "natfwunsafmessageintegrityattribute.h"
#include "natfwunsafpasswordattribute.h"
#include "natfwunsafmappedaddressattribute.h"
#include "natfwunsafusecandidateattribute.h"
#include "natfwunsafpriorityattribute.h"
#include "natfwunsafxormappedaddressattribute.h"
#include "natfwunsafdataindication.h"
#include "natfwunsafdataattribute.h"
#include "natfwstunsrvobserver.h"
#include "natfwunsaficecontrollingattribute.h"
#include "natfwunsaficecontrolledattribute.h"
#include "natfwunsaffingerprintattribute.h"

const TInt KErrorCode401        = 401;
const TInt KErrRoleConflict     = 487;
const TInt KMaxLength           = 23;
const TInt KMaxTrackedIdCount   = 16;

_LIT8( KUnauthorized, "Unauthorized" );
_LIT8( KRoleConflict, "Role Conflict" );
_LIT8( KColon, ":" );


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::CNATFWSTUNSrvImpl
// ---------------------------------------------------------------------------
//
CNATFWSTUNSrvImpl::CNATFWSTUNSrvImpl( MNATFWStunSrvObserver& aObserver ) :
                                      iObserver( aObserver ),
                                      iFromAddress( KInetAddrNone, 0 ),
                                      iPeerAddr( KInetAddrNone, 0 ),
                                      iLocalAddress( KInetAddrNone, 0 )
    {    
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::NewL
// ---------------------------------------------------------------------------
//
CNATFWSTUNSrvImpl* CNATFWSTUNSrvImpl::NewL( MNATFWStunSrvObserver& aObserver,
    MNcmConnectionMultiplexer& aMultiplexer )
    {
     __STUNSERVER( "CNATFWSTUNSrvImpl::NewL" )

    CNATFWSTUNSrvImpl* self =  new ( ELeave ) CNATFWSTUNSrvImpl( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aMultiplexer );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CNATFWSTUNSrvImpl::ConstructL( MNcmConnectionMultiplexer& aMultiplexer )
    {
     __STUNSERVER( "CNATFWSTUNSrvImpl::ConstructL" )
    
    iMultiplexer = &aMultiplexer; 
    iUsername = KNullDesC8().AllocL();
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSTUNSrvClientSession::~CNATFWSTUNSrvImpl 
// ---------------------------------------------------------------------------
//
 CNATFWSTUNSrvImpl::~CNATFWSTUNSrvImpl()
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::~CNATFWSTUNSrvImpl" )
    
    iIdentificationArray.Close();
    delete iUsername;
    iMultiplexer = NULL;
    
    iTransactionIds.Close();
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::AddAuthenticationParamsL 
// ---------------------------------------------------------------------------
//
void CNATFWSTUNSrvImpl::AddAuthenticationParamsL(
    const RPointerArray<CNATFWCredentials>& aIdentifications )
    {    
    __STUNSERVER( "CNATFWSTUNSrvImpl::AddAuthenticationParamsL" )
    
    TInt identificationCount( aIdentifications.Count() );
    for ( TInt i = 0; i < identificationCount; i++ )
        {
        TInt ind( iIdentificationArray.Find( 
            aIdentifications[i], CNATFWCredentials::Compare ) );
        if ( KErrNotFound == ind )
            {
            iIdentificationArray.AppendL( aIdentifications[i] );
            iMultiplexer->RegisterIncomingConnectionObserverL( 
                aIdentifications[i]->StreamId(), *this );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::RemoveAuthenticationParamsL
// ---------------------------------------------------------------------------
//     
void CNATFWSTUNSrvImpl::RemoveAuthenticationParamsL( 
    const RPointerArray<CNATFWCredentials>& aIdentifications )
    {
     __STUNSERVER( "CNATFWSTUNSrvImpl::RemoveAuthenticationParamsL" )
    
    TInt removeCount( aIdentifications.Count() );
    while ( removeCount-- )
        {
        TInt ind( iIdentificationArray.Find( 
            aIdentifications[removeCount], CNATFWCredentials::Compare ) );
        
        if ( KErrNotFound != ind )
            {
            iMultiplexer->UnregisterIncomingConnectionObserverL( 
                aIdentifications[removeCount]->StreamId(), *this );
            iIdentificationArray.Remove( ind );
            }
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::SetRoleL
// ---------------------------------------------------------------------------
//     
void CNATFWSTUNSrvImpl::SetRoleL( TNATFWIceRole aRole,
    TUint64 aTieBreaker ) 
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::SetRoleL" )
    
    iRole = aRole;
    iTieBreaker = aTieBreaker;
    }


// ---------------------------------------------------------------------------
// From class MNcmIncomingConnectionObserver
// CNATFWSTUNSrvImpl::IncomingMessageL 
// ---------------------------------------------------------------------------
//                                        
void CNATFWSTUNSrvImpl::IncomingMessageL( TUint aStreamId, 
    const TDesC8& aMessage, const TInetAddr& aLocalAddr, 
    const TInetAddr& aFromAddr, const TInetAddr& aPeerRemoteAddress,
    TBool& aConsumed )
    {
    __STUNSERVER_ADDRLOG(
        "CNATFWSTUNSrvImpl::IncomingMessageL LOCAL ADDR: ", aLocalAddr )
    __STUNSERVER_ADDRLOG(
        "CNATFWSTUNSrvImpl::IncomingMessageL FROM ADDR: ", aFromAddr )
    __STUNSERVER_ADDRLOG( 
        "CNATFWSTUNSrvImpl::IncomingMessageL PEER ADDR: ", aPeerRemoteAddress )
    
    aConsumed = EFalse;
    
    // decoding validates that message is well formed STUN message
    CNATFWUNSAFMessage* msg = DecodeMessageL( aMessage );
    CleanupStack::PushL( msg );
    
    TInetAddr peerAddr( aPeerRemoteAddress );
    SetPeerRemoteAddress( aFromAddr, peerAddr );
    __STUNSERVER_ADDRLOG( 
        "CNATFWSTUNSrvImpl::IncomingMessageL NEW PEER ADDR: ", 
        peerAddr )
    
    iRoleConflict = this->IsRoleConflictPresent( *msg );

    iStreamId = aStreamId;
    iFromAddress = aFromAddr;
    iPeerAddr = peerAddr;
    iLocalAddress = aLocalAddr;
    
    // Unpack data indication
    if ( msg->Type() == CNATFWUNSAFMessage::EDataIndication )
        {
        __STUNSERVER( "CNATFWSTUNSrvImpl::IncomingMessageL, DATA INDICATION" )
        
        iRequestInsideIndication = ETrue;
        CNATFWUNSAFMessage* prevMsg = msg;
        const TDesC8& data = static_cast<CNATFWUNSAFDataAttribute*>(
            msg->Attribute( CNATFWUNSAFAttribute::EData ) )->Value();
        
        CleanupStack::Pop( msg );
        TRAPD( err, msg = DecodeMessageL( data ) );
        CleanupStack::PushL( msg );
        if ( KErrNoMemory == err )
            {
            User::Leave( err );
            }
        else if ( KErrNone != err ||
             msg->Type() != CNATFWUNSAFMessage::EBindingRequest &&
             msg->Type() != CNATFWUNSAFMessage::EBindingIndication )
            {
             // Not acceptabe. Return pointer to data.
            iRequestInsideIndication = EFalse;
            delete prevMsg;
            CleanupStack::PopAndDestroy( msg );
            return;
            }
        else
            {
            //For avoid PC-Lint Note
            }
        delete prevMsg;
        }
    
    // STUN binding request and indication are accepted
    if ( msg->Type() == CNATFWUNSAFMessage::EBindingRequest )
        {       
        if ( this->IsRequestValidL( *msg, aMessage ) )
            {
            TBool favored = 
                msg->HasAttribute( CNATFWUNSAFAttribute::EUseCandidate );
            
            TUint priority = static_cast<
                CNATFWUNSAFPriorityAttribute*>( msg->Attribute( 
                CNATFWUNSAFAttribute::EPriority ) )->Priority(); 
            
            // Do not generate callback for retransmitted requests because
            // that increases call setup time.
            if ( !IsRetransmittedRequest( *msg ) )
                {
                SaveTransactionIdL( msg->TransactionID() );
                
                iObserver.STUNRequestReceivedL( aLocalAddr, aFromAddr,
                    iPeerAddr, priority, favored );
                }
            }
        
        aConsumed = ETrue;
        }
    
    if ( msg->Type() == CNATFWUNSAFMessage::EBindingIndication )
        {
        __STUNSERVER( "CNATFWSTUNSrvImpl::IncomingMessageL - binding indication" )
        aConsumed = ETrue;
        }
    
    CleanupStack::PopAndDestroy( msg );
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::IsRoleConflictPresent 
// ---------------------------------------------------------------------------
//     
TBool CNATFWSTUNSrvImpl::IsRoleConflictPresent( 
    const CNATFWUNSAFMessage& aRequest )
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::IsRoleConflictPresent" )
    
    if ( EIceRoleControlling == iRole )
        {
        if ( aRequest.HasAttribute( CNATFWUNSAFAttribute::EIceControlling ) )
            {            
            TUint64 tiebreaker = 
                static_cast<CNATFWUNSAFIceControllingAttribute*>( 
                aRequest.Attribute( CNATFWUNSAFAttribute::EIceControlling ) )
                ->IceControlling();
            
            if ( iTieBreaker >= tiebreaker )
                {
                return ETrue;
                }
            else
                {
                iRole = EIceRoleControlled;
                iObserver.RoleChangeNeeded( EIceRoleControlled );
                return EFalse;
                }
            }
        }
    
    if ( EIceRoleControlled == iRole )
        {
        if ( aRequest.HasAttribute( CNATFWUNSAFAttribute::EIceControlled ) )
            {
            TUint64 tiebreaker = 
                static_cast<CNATFWUNSAFIceControlledAttribute*>( 
                aRequest.Attribute( CNATFWUNSAFAttribute::EIceControlled ) )
                ->IceControlled();
            
            if ( iTieBreaker >= tiebreaker )
                {
                iRole = EIceRoleControlling;
                iObserver.RoleChangeNeeded( EIceRoleControlling );
                return EFalse;
                }
            else
                {
                return ETrue;
                }
            }
        }
    
    return EFalse; 
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::SetPeerRemoteAddress 
// ---------------------------------------------------------------------------
// 
void CNATFWSTUNSrvImpl::SetPeerRemoteAddress( const TInetAddr& aFromAddr,
    TInetAddr& aPeerRemoteAddress ) const
    {
    if ( aPeerRemoteAddress.IsUnspecified() )
        {
        aPeerRemoteAddress = aFromAddr;
        }
    }
    
    
// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::DecodeMessageL
// ---------------------------------------------------------------------------
//
CNATFWUNSAFMessage* CNATFWSTUNSrvImpl::DecodeMessageL( 
    const TDesC8& aData ) const
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::DecodeMessageL" )
    
    CNATFWUNSAFMessageFactory* decoder = CNATFWUNSAFMessageFactory::NewLC();
    CNATFWUNSAFMessage* msg = decoder->DecodeL( aData );
    CleanupStack::PopAndDestroy( decoder );    
    return msg;
    }
    

// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::EncodeMessageL
// ---------------------------------------------------------------------------
//
HBufC8* CNATFWSTUNSrvImpl::EncodeMessageL( 
    const CNATFWUNSAFMessage& aRequest, const TDesC8& aPassword,
    const TInt aErrorCode ) const
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::EncodeMessageL" )    
    
    CBufBase* message = NULL;       
         
    if ( KErrorCode401 == aErrorCode || KErrRoleConflict == aErrorCode )
        {
        message = aRequest.EncodeL(); 
        }
    else
        {
        // ETrue indicates that fingerprint attribute will be added
        message = aRequest.EncodeL( aPassword, ETrue );
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


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::IsRequestValidL
// ---------------------------------------------------------------------------
//    
TBool CNATFWSTUNSrvImpl::IsRequestValidL( const CNATFWUNSAFMessage& aRequest,
                                          const TDesC8& aByteStream )
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::IsRequestValidL" )
    
    TInt errorCode( KErrNone );
    HBufC8* password = this->GetPassword( aRequest, errorCode );
    if ( KErrNoMemory == errorCode )
        {
        User::Leave( errorCode );
        }
    else if ( KErrNone == errorCode )
        {
        CleanupStack::PushL( password );
        }
    else
        {
        // For avoid PC-Lint note
        }
    CNATFWUNSAFFingerprintAttribute* fingerPrint = 
                    static_cast<CNATFWUNSAFFingerprintAttribute*>(
                    aRequest.Attribute(CNATFWUNSAFAttribute::EFingerprint));

    if ( KErrNone != errorCode ||
        !aRequest.HasAttribute( CNATFWUNSAFAttribute::EPriority ) ||
        ( !fingerPrint || !fingerPrint->CheckFingerprintL( aByteStream ) ) )
        {
        if ( KErrNone == errorCode )
            {
            CleanupStack::PopAndDestroy( password );
            }
        return EFalse;
        }
    
    if ( iRoleConflict )
        {
        errorCode = KErrRoleConflict;
        iRoleConflict = EFalse;
        }
    __STUNSERVER_STR8( "CNATFWSTUNSrvImpl::IsRequestValidL - PASSWORD:",
        *password )
    __STUNSERVER_STR8( "CNATFWSTUNSrvImpl::IsRequestValidL - USERNAME:",
        *iUsername )

    TBool isValid( EFalse );
    TRAPD( error, isValid = CheckIntegrityL( aRequest, aByteStream, 
                                             *password ) );
    if ( KErrNoMemory == error )
        {
        User::Leave( error );
        }
    else if ( KErrNone != error || !isValid )
        {
        errorCode = KErrorCode401;
        }
    else
        {
        // For avoid PC-Lint note
        }
    
    this->SendResponseL( aRequest, errorCode, *password );
    CleanupStack::PopAndDestroy( password );
    
    return ( KErrNone == errorCode );
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::CheckIntegrityL
// ---------------------------------------------------------------------------
//      
TBool CNATFWSTUNSrvImpl::CheckIntegrityL( const CNATFWUNSAFMessage& aRequest,
                                          const TDesC8& aByteStream,
                                          const TDesC8& aPassword )
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::CheckIntegrityL" )
    
    TBool isIntegrityOk( EFalse );
    if ( !aRequest.HasAttribute( CNATFWUNSAFAttribute::EMessageIntegrity ) )
        {
        isIntegrityOk = EFalse;
        }
    else if ( iRequestInsideIndication )
        {
        iRequestInsideIndication = EFalse;
        
        HBufC8* msg = this->EncodeMessageL( aRequest, aPassword, KErrNone );
        CleanupStack::PushL( msg );
        
        if ( static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
             aRequest.Attribute( CNATFWUNSAFAttribute::EMessageIntegrity ) )
                ->CheckMessageIntegrityL( *msg, aPassword ) )
            {
            isIntegrityOk = ETrue;
            }
        
        CleanupStack::PopAndDestroy( msg );
        }
    else if ( static_cast<CNATFWUNSAFMessageIntegrityAttribute*>(
              aRequest.Attribute( CNATFWUNSAFAttribute::EMessageIntegrity ) )
                ->CheckMessageIntegrityL( aByteStream, aPassword ) )
        {
        isIntegrityOk = ETrue;
        }
    else
        {
        isIntegrityOk = EFalse;
        }
    
    return isIntegrityOk;
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::GetPasswordL
// ---------------------------------------------------------------------------
//    
HBufC8* CNATFWSTUNSrvImpl::GetPassword( const CNATFWUNSAFMessage& aRequest,
    TInt& aError )
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::GetPassword" )
    
    aError = KErrNone;
    
    if ( !aRequest.HasAttribute( CNATFWUNSAFAttribute::EUsername ) )
        {
        __STUNSERVER( "CNATFWSTUNSrvImpl::GetPassword, NO USERNAME ATTR" )
        aError = KErrNotFound;
        return NULL;
        }
    
    HBufC8* userName = static_cast<CNATFWUNSAFUsernameAttribute*>( 
        aRequest.Attribute( CNATFWUNSAFAttribute::EUsername ) )->
        Value().Alloc();

    // quick fix for OOM situation and to prevent crash
    if ( NULL == userName )
        {
        aError = KErrNoMemory;
        return NULL;
        }

    delete iUsername;
    iUsername = NULL;
    iUsername = userName;

    TInt ind = iUsername->Find( KColon );
    if ( KErrNotFound == ind )
        {
        __STUNSERVER( "CNATFWSTUNSrvImpl::GetPassword - INVALID USERNAME" )
        aError = KErrNotFound;
        return NULL;
        }
    
    HBufC8* usernameFragment = iUsername->Left( ind ).Alloc();
    if ( !usernameFragment )
        {
        aError = KErrNoMemory;
        return NULL;
        }

    TInt count( iIdentificationArray.Count() );
    __STUNSERVER_INT1( "CNATFWSTUNSrvImpl::GetPassword - array count: ", count )
    
    for ( TInt i = 0; i < count; i++ )
        {
        const CNATFWCredentials& ident = *iIdentificationArray[i];
        
        if ( CNATFWCredentials::EInbound == ident.Direction() 
             && 0 <= ident.Username().Compare( *usernameFragment ) )
            {
            delete usernameFragment;
            HBufC8* passwrd = ident.Password().Alloc();
            if ( !passwrd )
                {
                aError = KErrNoMemory;
                }
            return passwrd;
            }
        }
    delete usernameFragment;
    aError = KErrGeneral;
    return NULL;   
    }
    
 
// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::SendResponseL 
// ---------------------------------------------------------------------------
// 
void CNATFWSTUNSrvImpl::SendResponseL( const CNATFWUNSAFMessage& aRequest,
                                       TInt aErrorCode,
                                       const TDesC8& aPassword )
    {
    __STUNSERVER( "CNATFWSTUNSrvImpl::SendResponseL" )
             
    CNATFWUNSAFMessage* response( NULL );
    if ( KErrNone != aErrorCode )
        {
        TBufC8<KMaxLength> reason;
    
        switch( aErrorCode )
            {
            case KErrorCode401:
                reason = KUnauthorized;
                break;
            case KErrRoleConflict:
                reason = KRoleConflict;
                break;
            default:
                User::Leave( KErrArgument );
                break;
            }

        // Transaction ID must be same as in the request
        response = CNATFWUNSAFBindingErrorResponse::NewLC(
            aRequest.TransactionID() );
        response->AddAttributeL( 
            CNATFWUNSAFErrorCodeAttribute::NewLC( aErrorCode, reason ) );
        CleanupStack::Pop();
        }
    else
        {            
        response = CNATFWUNSAFBindingResponse::NewLC( 
            aRequest.TransactionID() );
      
        if ( aRequest.HasAttribute( CNATFWUNSAFAttribute::EMagicCookie ) )
            {
            response->AddAttributeL( CNATFWUNSAFXorMappedAddressAttribute::NewLC(
                                     iPeerAddr, aRequest.TransactionID() ) );
            CleanupStack::Pop();
            }
        else
            {
            response->AddAttributeL( CNATFWUNSAFMappedAddressAttribute::NewLC( 
                                     iPeerAddr ) );
            CleanupStack::Pop();
            }
        }
    HBufC8* msg = this->EncodeMessageL( *response, aPassword, aErrorCode );
    CleanupStack::PushL( msg );
    TRAPD( err, iMultiplexer->SendL( iStreamId, *msg, iFromAddress, iPeerAddr ) )
    if ( KErrNoMemory == err )
        {
        User::Leave( err );
        }
    CleanupStack::PopAndDestroy( msg );
    CleanupStack::PopAndDestroy( response );
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::IsRetransmittedRequest
// ---------------------------------------------------------------------------
//    
TBool CNATFWSTUNSrvImpl::IsRetransmittedRequest( 
    const CNATFWUNSAFMessage& aRequest ) const
    {
    TNATFWUNSAFTransactionID id = aRequest.TransactionID();
    
    if ( KErrNotFound == FindTransactionId( id ) )
        {
        __STUNSERVER_STR8( "CNATFWSTUNSrvImpl::IsRetransmittedRequest FALSE",
            id )
        return EFalse;
        }
    else
        {
        __STUNSERVER_STR8( "CNATFWSTUNSrvImpl::IsRetransmittedRequest TRUE",
            id )
        return ETrue;
        }
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::SaveTransactionIdL
// ---------------------------------------------------------------------------
//
void CNATFWSTUNSrvImpl::SaveTransactionIdL( 
    const TNATFWUNSAFTransactionID& aId )
    {
    __STUNSERVER_STR8( "CNATFWSTUNSrvImpl::SaveTransactionIdL", aId )
    
    if ( KMaxTrackedIdCount == iTransactionIds.Count() )
        {
        iTransactionIds.Remove( 0 );
        }
    
    iTransactionIds.AppendL( aId );
    }


// ---------------------------------------------------------------------------
// CNATFWSTUNSrvImpl::FindTransactionId
// ---------------------------------------------------------------------------
//
TInt CNATFWSTUNSrvImpl::FindTransactionId( 
        const TNATFWUNSAFTransactionID& aId ) const
    {
    TInt foundIdIndex( KErrNotFound );
    TInt index( iTransactionIds.Count() - 1 );
    while ( KErrNotFound == foundIdIndex && 0 <= index )
        {
        if ( aId == iTransactionIds[index] )
            {
            foundIdIndex = index;
            }
        
        index--;
        }
    
    return foundIdIndex;
    }
