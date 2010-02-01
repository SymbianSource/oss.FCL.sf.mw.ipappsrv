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
* Description:    Candidate store item
*
*/




#include "natcandidatestoreitem.h"
 
#include "natfwcandidate.h"


// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::CNATCandidateStoreItem
// ---------------------------------------------------------------------------
//
CNATCandidateStoreItem::CNATCandidateStoreItem()
    {
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::ConstructL
// ---------------------------------------------------------------------------
//
void CNATCandidateStoreItem::ConstructL()
    {
    iCandidate = CNATFWCandidate::NewL();
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::NewL
// ---------------------------------------------------------------------------
//
CNATCandidateStoreItem* CNATCandidateStoreItem::NewL()
    {
    CNATCandidateStoreItem* self = CNATCandidateStoreItem::NewLC();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::NewLC
// ---------------------------------------------------------------------------
//
CNATCandidateStoreItem* CNATCandidateStoreItem::NewLC()
    {
    CNATCandidateStoreItem* self = new( ELeave ) CNATCandidateStoreItem();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::CNATCandidateStoreItem
// ---------------------------------------------------------------------------
//
CNATCandidateStoreItem::~CNATCandidateStoreItem()
    {
    delete iCandidate;
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::SetNATCandidate
// ---------------------------------------------------------------------------
//
void CNATCandidateStoreItem::SetNATCandidate( const CNATFWCandidate& aCandidate )
    {
    if ( iCandidate )
        {
        delete iCandidate;
        iCandidate = NULL;
        }
    iCandidate = CNATFWCandidate::NewL( aCandidate );
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::NATCandidate
// ---------------------------------------------------------------------------
//
const CNATFWCandidate& CNATCandidateStoreItem::NATCandidate() const
    {
    return *iCandidate;
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::ExternalizeL
// ---------------------------------------------------------------------------
//
void CNATCandidateStoreItem::ExternalizeL( RWriteStream& aStream ) const
    {
    TBuf16<64> tempBuf;
    
    aStream.WriteInt32L( iCandidate->SessionId() );
    aStream.WriteInt32L( iCandidate->StreamId() );
    aStream.WriteInt32L( iCandidate->StreamCollectionId() );  
    aStream.WriteInt32L( iCandidate->ComponentId() ); 
    aStream.WriteInt32L( iCandidate->Type() );
    aStream.WriteInt32L( iCandidate->Priority() );
    
    // Candidate Transport address
    iCandidate->TransportAddr().Output( tempBuf );
    aStream << tempBuf;
    aStream.WriteInt32L( iCandidate->TransportAddr().Port() );
    
    aStream.WriteInt32L( iCandidate->TransportProtocol() );
    aStream << iCandidate->Foundation();
    
    //Base of the candidate
    iCandidate->Base().Output( tempBuf );
    aStream << tempBuf;
    aStream.WriteInt32L( iCandidate->Base().Port() );
    }

// ---------------------------------------------------------------------------
// CNATCandidateStoreItem::InternalizeL
// ---------------------------------------------------------------------------
//
void CNATCandidateStoreItem::InternalizeL( RReadStream& aStream )
    {
    TBuf16<64> tempBuf;
    TBuf8<32> tempBuf8;
    TInetAddr address;

    iCandidate->SetSessionId( aStream.ReadInt32L() );
    iCandidate->SetStreamId( aStream.ReadInt32L() );
    iCandidate->SetStreamCollectionId( aStream.ReadInt32L() );
    iCandidate->SetComponentId( aStream.ReadInt32L() );
    iCandidate->SetType( 
        ( CNATFWCandidate::TCandidateType )aStream.ReadInt32L() );
    iCandidate->SetPriority( aStream.ReadInt32L() );
    
    // Candidate Transport address
    aStream >> tempBuf;
    User::LeaveIfError( address.Input( tempBuf ) ); 
    address.SetPort( aStream.ReadInt32L() );
    iCandidate->SetTransportAddrL( address );
    
    iCandidate->SetTransportProtocol( ( TUint )aStream.ReadInt32L() );
    
    //Foundation
    aStream >> tempBuf;
    __ASSERT_ALWAYS( tempBuf.Length() < tempBuf8.MaxLength(), User::Leave( KErrOverflow ) );
    tempBuf8.Copy( tempBuf );
    iCandidate->SetFoundationL( tempBuf8 );
    
     //Base of the candidate
    aStream >> tempBuf;
    User::LeaveIfError( address.Input( tempBuf ) ); 
    address.SetPort( aStream.ReadInt32L() );
    iCandidate->SetBase( address );
    }
