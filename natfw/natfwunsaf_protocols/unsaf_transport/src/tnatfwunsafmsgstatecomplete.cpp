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




#include "tnatfwunsafmsgstatecomplete.h"
#include "mnatfwunsafmsgassemblerctx.h"
#include "mnatfwunsafmsgassemblerobserver.h"
#include "natfwunsafmessagefactory.h"

// ----------------------------------------------------------------------------
// TNATFWUNSAFMsgStateComplete::DataReceivedL
// ----------------------------------------------------------------------------
//
TBool TNATFWUNSAFMsgStateComplete::DataReceivedL(
    TPtr8 /*aData*/,
    TUint& /*aNextLength*/ )
    {
    __ASSERT_ALWAYS( iMsgAssembler.Message() != 0,
                     User::Leave( KErrNotFound ) );

    // MsgAssembler can detach the message
    CNATFWUNSAFMessage* tmpMsg = iMsgAssembler.Message();
    iMsgAssembler.DetachMessage();

    // Reset the state to msg init, there might be still some data
    // in aData but it will be discarded.
    iMsgAssembler.ChangeState( TNATFWUNSAFMsgStateBase::EMsgInit );

    CleanupStack::PushL( tmpMsg );
    iMsgAssembler.MsgObserver().NATFWUNSAFMsgCompleteL( tmpMsg,
                                                   KErrNone );

    CleanupStack::Pop( tmpMsg ); // ownership was transferred

    // Indicate completion
    return ETrue;
    }

// End of File

