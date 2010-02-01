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




#ifndef _ICETESTSTUBS_H_
#define _ICETESTSTUBS_H_

#include "natfwconnectivityapidefs.h"

#include "natfwtestconsolestubs.h"


class MICETestStubsObserver;
class CNATFWCandidate;
class CConsoleBase;

class CICETestStubs :
    public CNATConnFWTestConsoleStubs
    {
    
public:
    static CICETestStubs* NewL( CConsoleBase& aConsole );
    
    virtual ~CICETestStubs();
    
public:

// from CNATConnFWTestConsoleStubs    
    void NewCandidatePairFound( CNATFWCandidatePair* aCandidatePair );
    
    void NewLocalCandidateFound( 
        CNATFWCandidate* aLocalCandidate );
    
    void SetObserver( MICETestStubsObserver* aObserver );

protected:
    
    CICETestStubs( CConsoleBase& aConsole );
    
private:

    MICETestStubsObserver* iObserver;
    };
    
#endif // _ICEUDBTESTSTUBS_H_