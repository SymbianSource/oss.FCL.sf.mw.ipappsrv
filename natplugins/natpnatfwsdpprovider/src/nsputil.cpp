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
* Description:  Utility class implementation.
*
*/

#include <sdpcodecstringpool.h>
#include <sdpcodecstringconstants.h>
#include <sdpdocument.h>
#include <sdpconnectionfield.h>
#include <sdporiginfield.h>
#include <sdpmediafield.h>
#include "natfwcandidate.h"
#include "nsputil.h"
#include "nspdefs.h"
#include "nspmediastream.h"

const TUint KMaxLengthOfFQDN = 255;
const TUint KRejectPort = 0;

// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// NSPUtil::FindMediaStream
// ---------------------------------------------------------------------------
//
CNSPMediaStream* NSPUtil::FindMediaStream( const CSdpMediaField& aMediaField,
        RPointerArray<CNSPMediaStream>& aMediaStreams )
    {
    const TInt mediastreamscount( aMediaStreams.Count() );
    for ( TInt index = 0; index < mediastreamscount; index++ )
        {
        if ( aMediaStreams[index]->MediaField().Media() == aMediaField.Media() )
            {
            return aMediaStreams[index];
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// NSPUtil::FindMediaStream
// ---------------------------------------------------------------------------
//
CNSPMediaStream* NSPUtil::FindMediaStream(
        const CNSPMediaStream& aMediaStream,
        RPointerArray<CNSPMediaStream>& aMediaStreams )
    {
    const TInt mediastreamscount( aMediaStreams.Count() );
    for ( TInt index = 0; index < mediastreamscount; index++ )
        {
        if ( aMediaStreams[index]->MediaField().Media() ==
             aMediaStream.MediaField().Media() )
            {
            return aMediaStreams[index];
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// NSPUtil::FindMediaField
// ---------------------------------------------------------------------------
//
CSdpMediaField* NSPUtil::FindMediaField( const CNSPMediaStream& aMediaStream,
        RPointerArray<CSdpMediaField>& aMediaFields )
    {
    const TInt mediafieldscount( aMediaFields.Count() );
    for ( TInt index = 0; index < mediafieldscount; index++ )
        {
        if ( aMediaFields[index]->Media() == aMediaStream.MediaField().Media() )
            {
            return aMediaFields[index];
            }
        }
    
    return NULL;
    }


// ---------------------------------------------------------------------------
// NSPUtil::FindMediaField
// ---------------------------------------------------------------------------
//
CSdpMediaField* NSPUtil::FindMediaField( const CSdpMediaField& aMediaField,
        RPointerArray<CSdpMediaField>& aMediaFields )
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
// NSPUtil::SortCandidatesL
// ---------------------------------------------------------------------------
//
void NSPUtil::SortCandidatesL( RPointerArray<CNATFWCandidate>& aLocalCands )
    {
    __ASSERT_ALWAYS( &aLocalCands, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( aLocalCands.Count(), User::Leave( KErrArgument ) );
    
    TLinearOrder<CNATFWCandidate> order( CNATFWCandidate::PriorityOrder );
    aLocalCands.Sort( order );
    }


// ---------------------------------------------------------------------------
// NSPUtil::UpdateConnectionFieldL
// ---------------------------------------------------------------------------
//
void NSPUtil::UpdateConnectionFieldL(
        CSdpConnectionField& aField, const TDesC8& aAddress )
    {
    __ASSERT_ALWAYS( &aField, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( &aAddress != NULL, User::Leave( KErrArgument ) );
    
    TBool unspecf( EFalse );
    RStringF netType = aField.NetType();
    RStringF addressType = aField.AddressType();
    const TDesC8& address = aField.Address();
    
    TInetAddr addr;
    HBufC* addrBuf = HBufC::NewLC( KMaxLengthOfFQDN );
    TPtr addrPtr( addrBuf->Des() );
    addrPtr.Copy( address );
    
    if ( !addr.Input( addrBuf->Des() ) ) // valid IP
        {
        unspecf = addr.IsUnspecified();
        addrPtr.Copy( aAddress );
        
        if ( !unspecf && !addr.Input( addrBuf->Des() ) )
            {
            CSdpConnectionField* field = CSdpConnectionField::NewL( addr );
            addressType = field->AddressType();
            delete field;
            }
        }
    
    if ( !unspecf )
        {
        aField.SetAddressL( netType, addressType, aAddress );
        }
    
    CleanupStack::PopAndDestroy( addrBuf );
    }


// ---------------------------------------------------------------------------
// NSPUtil::UpdateOriginFieldL
// ---------------------------------------------------------------------------
//
void NSPUtil::UpdateOriginFieldL(
        CSdpOriginField& aField, const TDesC8& aAddress )
    {
    __ASSERT_ALWAYS( &aField, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( &aAddress != NULL, User::Leave( KErrArgument ) );
    
    TBool unspecf( EFalse );
    RStringF netType = aField.NetType();
    RStringF addressType = aField.AddressType();
    const TDesC8& address = aField.Address();
    
    TInetAddr addr;
    HBufC* addrBuf = HBufC::NewLC( KMaxLengthOfFQDN );
    TPtr addrPtr( addrBuf->Des() );
    addrPtr.Copy( address );
    
    if ( !addr.Input( addrBuf->Des() ) ) // valid IP
        {
        unspecf = addr.IsUnspecified();
        addrPtr.Copy( aAddress );
        
        if ( !unspecf && !addr.Input( addrBuf->Des() ) )
            {
            CSdpConnectionField* field = CSdpConnectionField::NewL( addr );
            addressType = field->AddressType();
            delete field;
            }
        }
    
    if ( !unspecf )
        {
        aField.SetAddressL( aAddress, netType, addressType );
        }
    
    CleanupStack::PopAndDestroy( addrBuf );    
    }


// ---------------------------------------------------------------------------
// NSPUtil::RejectL
// ---------------------------------------------------------------------------
//
void NSPUtil::RejectL( CSdpMediaField& aField )
    {
    __ASSERT_ALWAYS( &aField, User::Leave( KErrArgument ) );
    aField.SetPortL( KRejectPort );
    }


// ---------------------------------------------------------------------------
// NSPUtil::IsReject
// ---------------------------------------------------------------------------
//
TBool NSPUtil::IsReject( const CSdpMediaField& aField )
    {
    if ( &aField )
        {
        return ( KRejectPort == aField.Port() );
        }
    else
        {
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// NSPUtil::IsDuplicate
// ---------------------------------------------------------------------------
//
TBool NSPUtil::IsDuplicate( const CNATFWCandidate& aCand,
        const RPointerArray<CNATFWCandidate>& aCandidates )
    {
    const TUint candidatecount( aCandidates.Count() );
    for ( TUint index = 0; index < candidatecount; index++ )
        {
        if ( aCand == *aCandidates[index] )
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }


// ---------------------------------------------------------------------------
// NSPUtil::IsUnspecifiedL
// ---------------------------------------------------------------------------
//
TBool NSPUtil::IsUnspecifiedL( const TDesC8& aAddress, TUint aPort )
	{
	TInetAddr addr;
	TRAPD( error, addr = ConvertTDesC8ToTInetAddrL( aAddress, aPort ) );
	User::LeaveIfError( KErrNoMemory == error ? KErrNoMemory : KErrNone );
	return ( KErrNone == error ? addr.IsUnspecified() : EFalse );
	}


// ---------------------------------------------------------------------------
// NSPUtil::CleanupArrayItem
// ---------------------------------------------------------------------------
//
void NSPUtil::CleanupArrayItem( TAny* anArray )
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
