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




#ifndef UT_CNATFWUNSAFSERVERRESOLVEROBSERVERSTUB_H
#define UT_CNATFWUNSAFSERVERRESOLVEROBSERVERSTUB_H

//  EXTERNAL INCLUDES
#include <e32base.h>
#include <in_sock.h>

//  INTERNAL INCLUDES
#include "natfwunsafserverresolverobserver.h"

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class.
 *
 */
NONSHARABLE_CLASS( UT_CNATFWUNSAFServerResolverObserverStub ) : public CBase,
    public MNATFWUNSAFServerResolverObserver
        {
    public:

        static UT_CNATFWUNSAFServerResolverObserverStub* NewL();

        static UT_CNATFWUNSAFServerResolverObserverStub* NewLC();

        void CompletedL();
        void ErrorOccured( TInt aError );

        /**
         * Destructor.
         */
        ~UT_CNATFWUNSAFServerResolverObserverStub();

            //data
        RArray<TInetAddr>* iResultArray;
        TInt iError;
    private:

        UT_CNATFWUNSAFServerResolverObserverStub();

        void ConstructL();

        TUint iPort;
        
        // Unit test class declared as friend
        //    friend class CHostResolverTest;
    };
#endif // end of UT_CNATFWUNSAFSERVERRESOLVEROBSERVERSTUB_H

// End of File
