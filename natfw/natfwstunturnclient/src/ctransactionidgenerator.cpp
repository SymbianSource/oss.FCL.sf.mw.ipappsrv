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




#include <hash.h>
#include <e32math.h>
#include "stunassert.h"
#include "ctransactionidgenerator.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CTransactionIDGenerator::NewL
// ---------------------------------------------------------------------------
//
CTransactionIDGenerator* CTransactionIDGenerator::NewL()
    {    
    return new ( ELeave ) CTransactionIDGenerator();
    }

// ---------------------------------------------------------------------------
// CTransactionIDGenerator::CTransactionIDGenerator
// ---------------------------------------------------------------------------
//
CTransactionIDGenerator::CTransactionIDGenerator()
    {
    TUint ticks = User::TickCount();
    TTime now;
    now.UniversalTime();
    TInt64 us = now.Int64();

    iSeed = static_cast<TInt64>( ticks ) + us;
    iCounter = I64LOW( us ) - ticks;
    }

// ---------------------------------------------------------------------------
// CTransactionIDGenerator::CTransactionIDGenerator
// Dummy implementation, as copy constructor is declared private and not used.
// ---------------------------------------------------------------------------
//
CTransactionIDGenerator::CTransactionIDGenerator(
    const CTransactionIDGenerator& /*aTransactionIDGenerator*/ ) :
    CBase()
    {    
    }

// ---------------------------------------------------------------------------
// CTransactionIDGenerator::~CTransactionIDGenerator
// ---------------------------------------------------------------------------
//
CTransactionIDGenerator::~CTransactionIDGenerator()
    {
    }
    
// ---------------------------------------------------------------------------
// CTransactionIDGenerator::GetIDL
// CMD5 exists only during hash computing, to save memory.
// ---------------------------------------------------------------------------
//
void CTransactionIDGenerator::GetIDL( TAny* aObject,
                                      TInt aObjectSize,
                                      TNATFWUNSAFTransactionID& aTransactionID )
    {
    ++iCounter;
    HBufC8* data = BuildInputDataLC( aObject, aObjectSize );

    CMD5* md5 = CMD5::NewL();
    CleanupStack::PushL( md5 );
    TPtrC8 hash = md5->Hash( *data );

    __STUN_ASSERT_L( md5->HashSize() >= KMaxNATFWUNSAFTransactionIdLength,
                     KErrUnderflow );
    TPtrC8 ptrToHash = hash.Left(KMaxNATFWUNSAFTransactionIdLength);
    aTransactionID = ptrToHash;

    CleanupStack::PopAndDestroy( md5 );
    CleanupStack::PopAndDestroy( data );
    }

// ---------------------------------------------------------------------------
// CTransactionIDGenerator::BuildInputDataL
// ---------------------------------------------------------------------------
//
HBufC8* CTransactionIDGenerator::BuildInputDataLC( TAny* aObject,
                                                   TInt aObjectSize )
    {
    const TInt KAmountOfRandomNbrsToFill = 10;

    //Size of the input data buffer. It contains the following items:
    //
    //item                        item's size
    //----                        -----------
    //iCounter                       sizeof( iCounter )
    //aObject's state              aObjectSize    
    //checksum of this               sizeof( TUint16 )
    //clock info                  sizeof( TInt64 ) + sizeof( TUint )
    //system info                  sizeof( TUint16 ) + sizeof( TUint8 )
    //KAmountOfRandomNbrsToFill
    //random integer values     KAmountOfRandomNbrsToFill * sizeof( TInt )
    TInt dataSize = sizeof( iCounter ) + aObjectSize +
                    sizeof( TUint16 ) + sizeof( TInt64 ) + sizeof( TUint ) +
                    sizeof( TUint16 ) + sizeof( TUint8 ) +
                    KAmountOfRandomNbrsToFill * sizeof( TInt );
    HBufC8* buf = HBufC8::NewLC( dataSize );

    TPtr8 ptr = buf->Des();

    ptr.Append( reinterpret_cast<const TUint8*>( &iCounter ),
                sizeof( iCounter ) );
    ptr.Append( reinterpret_cast<const TUint8*>( aObject ), aObjectSize );
    
    ComputeChecksum( ptr, this, sizeof( this ) );

    AddClockInfo( ptr );
    AddSystemInfo( ptr );    

    TInt random = 0;
    TInt randomNumberSize = sizeof( random );
    while ( ptr.Size() <= ( ptr.MaxSize() - randomNumberSize ) )
        {
        random = Math::Rand( iSeed );
        ptr.Append( reinterpret_cast<const TUint8*>( &random ),
                    sizeof( random ) );
        }

    return buf;
    }

// -----------------------------------------------------------------------------
// CTransactionIDGenerator::AddClockInfo
// -----------------------------------------------------------------------------
//
void CTransactionIDGenerator::AddClockInfo( TDes8& aBuf ) const
    {
    TTime now;
    now.UniversalTime();
    TInt64 timeAsInt = now.Int64();

    aBuf.Append( reinterpret_cast<const TUint8*>( &timeAsInt ),
                 sizeof( timeAsInt ) );

    TUint ticks = User::TickCount();
    aBuf.Append( reinterpret_cast<const TUint8*>( &ticks ), sizeof( ticks ) );
    }

// -----------------------------------------------------------------------------
// CTransactionIDGenerator::AddSystemInfo
// -----------------------------------------------------------------------------
//
void CTransactionIDGenerator::AddSystemInfo( TDes8& aBuf ) const
    {
    TInt biggestBlock = 0;
    TInt totalAvailable = User::Available( biggestBlock );
    TInt value = biggestBlock + totalAvailable - User::CountAllocCells();
    ComputeChecksum( aBuf, &value, sizeof( value ) );

    TTimeIntervalSeconds inactivity = User::InactivityTime();
    if ( inactivity.Int() > 0 )
        {
        TUint8 byteVal = static_cast<TUint8>( inactivity.Int() & 0xff );
        aBuf.Append( &byteVal, sizeof( byteVal ) );
        }
    }

// ---------------------------------------------------------------------------
// CTransactionIDGenerator::ComputeChecksum
// ---------------------------------------------------------------------------
//
void CTransactionIDGenerator::ComputeChecksum( TDes8& aBuf,
                                               const TAny* aPtr,
                                               TInt aLength ) const
    {
    __STUN_ASSERT_RETURN( aPtr != NULL, KErrArgument );

    TUint16 cs = 0;
    Mem::Crc( cs, aPtr, aLength );
    aBuf.Append( reinterpret_cast<const TUint8*>( &cs ), sizeof( cs ) );
    }
