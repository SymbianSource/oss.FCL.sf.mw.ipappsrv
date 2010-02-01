/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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



#include "cnatfwunsafquerysrv.h"
#include "mnatfwunsafhostresolver.h"

_LIT8(KDot8, ".");
_LIT8(KUnderLine8, "_");
// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::CNATFWUNSAFQuerySrv
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQuerySrv::CNATFWUNSAFQuerySrv()
:CNATFWUNSAFQueryBase()
    {
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::ConstructL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQuerySrv::ConstructL( const TDesC8& aTarget )
    {
    SetTargetL( aTarget );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::NewL
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQuerySrv* CNATFWUNSAFQuerySrv::NewL( const TDesC8& aTarget )
    {
    CNATFWUNSAFQuerySrv* self = new ( ELeave ) CNATFWUNSAFQuerySrv();
    CleanupStack::PushL( self );
    self->ConstructL( aTarget );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::~CNATFWUNSAFQuerySrv
// ----------------------------------------------------------------------------
//
CNATFWUNSAFQuerySrv::~CNATFWUNSAFQuerySrv()
    {
    delete iTarget;
    iRecordArray.Reset();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::SetTargetProtocolL
// ----------------------------------------------------------------------------
//
HBufC8* CNATFWUNSAFQuerySrv::SetTargetProtocolL( const TDesC8& aTarget,
                                            const TDesC8& aProtocol,
                                            const TDesC8& aServiceName )
    {
    HBufC8* temp = HBufC8::NewLC( KUnderLine8().Length() +
                                  aServiceName.Length()  +
                                  KDot8().Length()       +
                                  KUnderLine8().Length() +
                                  aProtocol.Length()     +
                                  KDot8().Length()       +
                                  aTarget.Length() );

    TPtr8 tempPtr = temp->Des();
    tempPtr.Append( KUnderLine8 );
    tempPtr.Append( aServiceName );
    tempPtr.Append( KDot8 );
    tempPtr.Append( KUnderLine8 );
    tempPtr.Append( aProtocol );
    tempPtr.Append( KDot8 );
    tempPtr.Append( aTarget );
    CleanupStack::Pop( temp );
    return temp;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::AddL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQuerySrv::AddL( const TDnsRespSRV& aSrv )
    {
    TLinearOrder<TDnsRespSRV> order( CNATFWUNSAFQuerySrv::Compare );
    User::LeaveIfError(
        iRecordArray.InsertInOrderAllowRepeats( aSrv, order ) );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::SetTargetL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQuerySrv::SetTargetL( const TDesC8& aTarget )
    {
    const TInt KMaxlength(256);
    __ASSERT_ALWAYS ( aTarget.Length() > 0, User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS ( aTarget.Length() < KMaxlength, User::Leave( KErrArgument ) );
    HBufC8* temp = aTarget.AllocL();
    delete iTarget;
    iTarget = temp;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::RecordArray
// ----------------------------------------------------------------------------
//
RArray<TDnsRespSRV>& CNATFWUNSAFQuerySrv::RecordArray()
    {
    return iRecordArray;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::SRVRecordL
// ----------------------------------------------------------------------------
//
TDnsRespSRV& CNATFWUNSAFQuerySrv::SRVRecordL( TInt aIndex )
    {
    __ASSERT_ALWAYS ( aIndex < iRecordArray.Count(),
                      User::Leave( KErrArgument ) );

    return iRecordArray[aIndex];
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::Compare
// ----------------------------------------------------------------------------
//
TInt CNATFWUNSAFQuerySrv::Compare( const TDnsRespSRV& aFirst,
                            const TDnsRespSRV& aSecond )
    {
    if ( aFirst.Priority() < aSecond.Priority() )
        {
        return -1;
        }
    if ( aFirst.Priority() > aSecond.Priority() )
        {
        return 1;
        }
    if ( aFirst.Priority() == aSecond.Priority() )
        {
        if ( aSecond.Weight() == 0 )
            {
            return -1;
            }
        else
            {
            return 1;
            }
        }
    return 0;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::QueryBuf
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQuerySrv::QueryBuf()
    {
    TDnsQuery query( *iTarget, KDnsRRTypeSRV );
    TDnsQueryBuf querybuf( query );
    return iQueryBuf = querybuf;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::QueryResultBuf
// ----------------------------------------------------------------------------
//
TDes8& CNATFWUNSAFQuerySrv::QueryResultBuf()
    {
    return iResult;
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::QueryResult
// ----------------------------------------------------------------------------
//
TDnsRespSRV CNATFWUNSAFQuerySrv::QueryResult()
    {
    return iResult();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::Query
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQuerySrv::Query( MNATFWUNSAFHostResolver& aResolver )
    {
    aResolver.Resolver().Query( QueryBuf(),
                                QueryResultBuf(),
                                aResolver.RequestStatus() );
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::HandleQueryResultL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQuerySrv::HandleQueryResultL(
    MNATFWUNSAFHostResolver& aResolver )
    {
    if ( QueryResult().Target().CompareF( KDot8 ) != KErrNone )
        {
        AddL( QueryResult() );
        }

    while ( aResolver.Resolver().QueryGetNext( QueryResultBuf() )
                                                == KErrNone )
        {
        if ( QueryResult().Target().CompareF( KDot8 ) != KErrNone )
            {
            //Records are sorted in right order in inserting phase
            AddL( QueryResult() );
            }
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::ResultTargetL
// ----------------------------------------------------------------------------
//
const TDesC8& CNATFWUNSAFQuerySrv::ResultTargetL()
    {
    return SRVRecordL( 0 ).Target();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::ResultPortL
// ----------------------------------------------------------------------------
//
TUint CNATFWUNSAFQuerySrv::ResultPortL()
    {
    return SRVRecordL( 0 ).Port();
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::RemoveElementL
// ----------------------------------------------------------------------------
//
void CNATFWUNSAFQuerySrv::RemoveElementL( TInt aIndex )
    {
    if ( aIndex < iRecordArray.Count() && aIndex >= 0 )
        {
        iRecordArray.Remove( aIndex );
        }
    }

// ----------------------------------------------------------------------------
// CNATFWUNSAFQuerySrv::ArrayCountL
// ----------------------------------------------------------------------------
//
TUint CNATFWUNSAFQuerySrv::ArrayCountL()
    {
    return iRecordArray.Count();
    }

