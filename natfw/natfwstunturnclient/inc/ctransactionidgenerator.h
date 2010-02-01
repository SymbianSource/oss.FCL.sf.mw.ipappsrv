/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef C_CTRANSACTIONIDGENERATOR_H
#define C_CTRANSACTIONIDGENERATOR_H

// INCLUDES
#include <e32base.h>
#include "natfwunsaftransactionid.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  This class provides unique UNSAF TransactionID values. 
 */
class CTransactionIDGenerator : public CBase
    {
    public: // Constructors and destructor

        /**
        * Creates a new instance of CTransactionIDGenerator.        
        * @return A new instance of CTransactionIDGenerator, ownership is
        *    transferred.
        */
        static CTransactionIDGenerator* NewL();

        ~CTransactionIDGenerator();
    
    public: // New functions

        /**
        * Creates a unique UNSAF TransactionID and writes it into
        * aTransactionID.
        *
        * @param aObject IN: Pointer to an object, whose state will be used as a
        *         random input for creating the UNSAF TransactionID.
        *         Ownership is not transferred.
        * @param aObjectSize Size in bytes, of the object pointed by aObject.
        * @param aTransactionID OUT: UNSAF TransactionID
        */
        void GetIDL( TAny* aObject,
                     TInt aObjectSize,
                     TNATFWUNSAFTransactionID& aTransactionID );

    private:
    
        CTransactionIDGenerator();

        CTransactionIDGenerator(
            const CTransactionIDGenerator& aTransactionIDGenerator );

        HBufC8* BuildInputDataLC( TAny* aObject, TInt aObjectSize );
        
        void AddClockInfo( TDes8& aBuf ) const;
        
        void AddSystemInfo( TDes8& aBuf ) const;
        
        void ComputeChecksum( TDes8& aBuf,
                              const TAny* aPtr,
                              TInt aLength ) const;

    private: // Data

        //Seed value for Math::Rand()
        TInt64 iSeed;

        TUint32 iCounter;

    private: // For testing purposes

#ifdef TEST_EUNIT
        friend class CSTUNClientImplementationTest;
#endif
        __DECLARE_TEST;
    };

#endif // C_CTRANSACTIONIDGENERATOR_H
