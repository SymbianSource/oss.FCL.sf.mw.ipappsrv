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
* Description:    Candidate store
*
*/




#include "natcandidatestore.h"

#include "natfwcandidate.h"
#include <s32file.h> 

#include "natcandidatestoreitem.h"

_LIT( KFileName, "E:\\natfwcandidates.bin" );

// ---------------------------------------------------------------------------
// CNATCandidateStore::CNATCandidateStore
// ---------------------------------------------------------------------------
//
CNATCandidateStore::CNATCandidateStore()
    {
    }

// ---------------------------------------------------------------------------
// CNATCandidateStore::ConstructL
// ---------------------------------------------------------------------------
//
void CNATCandidateStore::ConstructL()
    {
    User::LeaveIfError( iFileServer.Connect() );
    }

// ---------------------------------------------------------------------------
// CNATCandidateStore::NewL
// ---------------------------------------------------------------------------
//
CNATCandidateStore* CNATCandidateStore::NewL()
    {
    CNATCandidateStore* self = CNATCandidateStore::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CNATCandidateStore::NewLC
// ---------------------------------------------------------------------------
//
CNATCandidateStore* CNATCandidateStore::NewLC()
    {
    CNATCandidateStore* self = new( ELeave ) CNATCandidateStore();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CNATCandidateStore::~CNATCandidateStore
// ---------------------------------------------------------------------------
//
CNATCandidateStore::~CNATCandidateStore()
    {
    iFileServer.Close();
    }

// ---------------------------------------------------------------------------
// CNATCandidateStore::WriteL
// ---------------------------------------------------------------------------
//
void CNATCandidateStore::WriteL( const RPointerArray<CNATFWCandidate>& aCandidates )
    {
    CNATCandidateStoreItem* item = CNATCandidateStoreItem::NewLC();
    
    RFileWriteStream outstream;
    outstream.PushL();
    
    User::LeaveIfError( outstream.Replace(
        iFileServer, KFileName, EFileStream | EFileWrite ) );

    TInt itemCount = aCandidates.Count();
    
    outstream.WriteInt32L( itemCount );
   
    for ( TInt i( 0 ); i < itemCount; i++ )
        {
        item->SetNATCandidate( *aCandidates[i] );
        outstream << *item;
        }
    outstream.CommitL();
    outstream.Pop();
    outstream.Close();
    CleanupStack::PopAndDestroy( item );
    }

// ---------------------------------------------------------------------------
// CNATCandidateStore::ReadL
// ---------------------------------------------------------------------------
//
void CNATCandidateStore::ReadL( RPointerArray<CNATFWCandidate>& aCandidates )
    {
    CNATCandidateStoreItem* item = CNATCandidateStoreItem::NewLC();
    
    RFileReadStream instream;
    instream.PushL();
    
    User::LeaveIfError( instream.Open(
        iFileServer, KFileName, EFileStream | EFileRead ) );
    
    TInt itemCount = instream.ReadInt32L();
    
    for ( TInt i ( 0 ); i < itemCount; i++ )
        {
        instream >> *item;
        aCandidates.AppendL( CNATFWCandidate::NewLC( item->NATCandidate() ) );
        CleanupStack::Pop( 1 );
        }
    instream.Pop();
    instream.Close();
    CleanupStack::PopAndDestroy( item );
    }
