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




#include "ttransactioncleanup.h"
#include "stunassert.h"
#include "cstuntransaction.h"

// -----------------------------------------------------------------------------
// TTransactionCleanup::TTransactionCleanup
// -----------------------------------------------------------------------------
//
TTransactionCleanup::TTransactionCleanup( CSTUNTransaction& aTransaction ) :
    iTransaction( aTransaction )
    {
    }

// -----------------------------------------------------------------------------
// TTransactionCleanup::CleanupItem
// -----------------------------------------------------------------------------
//
TCleanupItem TTransactionCleanup::CleanupItem()
    {
    return TCleanupItem( TerminateTransaction, this );
    }

// -----------------------------------------------------------------------------
// TTransactionCleanup::TerminateTransaction
// -----------------------------------------------------------------------------
//
void TTransactionCleanup::TerminateTransaction( TAny* aCleanup )
    {
    __STUN_ASSERT_RETURN( aCleanup, KErrArgument );

    reinterpret_cast<TTransactionCleanup*>
           ( aCleanup )->iTransaction.Terminate( KErrGeneral );
    }
