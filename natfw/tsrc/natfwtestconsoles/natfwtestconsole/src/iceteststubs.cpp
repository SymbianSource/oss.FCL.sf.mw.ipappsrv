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
* Description:    
*
*/



#include <e32base.h>
#include <e32debug.h>

#include "iceteststubs.h"
#include "natfwcandidate.h"
#include "natfwcandidatepair.h"
#include "iceteststubsobserver.h"


// ---------------------------------------------------------------------------
// CICETestStubs::NewL
// ---------------------------------------------------------------------------
//
CICETestStubs* CICETestStubs::NewL( CConsoleBase& aConsole )
    {
    CICETestStubs* self = new (ELeave) CICETestStubs( aConsole );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CICETestStubs::CICETestStubs
// ---------------------------------------------------------------------------
//    
CICETestStubs::CICETestStubs( CConsoleBase& aConsole ) :
    CNATConnFWTestConsoleStubs( aConsole )
    {
    }
    
    
// ---------------------------------------------------------------------------
// CICETestStubs::~CICETestStubs
// ---------------------------------------------------------------------------
//   
CICETestStubs::~CICETestStubs()
    {
    }


// ---------------------------------------------------------------------------
// CICETestStubs::SetObserver
// ---------------------------------------------------------------------------
//
void CICETestStubs::SetObserver( MICETestStubsObserver* aObserver )
    {
    iObserver = aObserver;
    }


// ---------------------------------------------------------------------------
// CICETestStubs::NewLocalCandidateFound
// ---------------------------------------------------------------------------
//
void CICETestStubs::NewLocalCandidateFound( 
        CNATFWCandidate* aLocalCandidate )
    {
    TInetAddr localAddr = aLocalCandidate->TransportAddr();
    
    TBuf<40> buffer;
    localAddr.Output( buffer );
    RDebug::Print( _L("CICETestStubs::NewLocalCandidateFound ADDRESS: %S "),
        &buffer );
    
    if( iObserver )
        {
        iObserver->LocalCandidateFound( *aLocalCandidate );
        }
    else
        {
        delete aLocalCandidate;
        }
    /*
    CActiveScheduler::Stop();
    iActive = EFalse;
    */
    }
    

// ---------------------------------------------------------------------------
// CICETestStubs::NewLocalCandidateFound
// ---------------------------------------------------------------------------
//    
void CICETestStubs::NewCandidatePairFound(
    CNATFWCandidatePair* aCandidatePair )
    {
    if( iObserver )
        {
        iObserver->NewCandidatePairFound( aCandidatePair );
        }
    else
        {
        delete aCandidatePair;
        }
    if ( iActive )
        {
        CActiveScheduler::Stop();
        iActive = EFalse;
        }
    }


