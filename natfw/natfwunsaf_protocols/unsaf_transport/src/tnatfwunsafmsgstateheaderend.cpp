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




#include "tnatfwunsafmsgstateheaderend.h"
#include "mnatfwunsafmsgassemblerctx.h"
#include "natfwunsafmessagefactory.h"

// ----------------------------------------------------------------------------
// TNATFWUNSAFMsgStateHeaderEnd::DataReceivedL
// ----------------------------------------------------------------------------
//
TBool TNATFWUNSAFMsgStateHeaderEnd::DataReceivedL( TPtr8 aData,
    TUint& aNextLength )
    {
    __ASSERT_ALWAYS( aData.Length() >= iMsgAssembler.MessageLength(),
                     User::Leave( KErrUnderflow ) );

    TPtrC8 headerDescriptor = aData.Left( iMsgAssembler.MessageLength() );

    CNATFWUNSAFMessage* message =
        iMsgAssembler.MsgFactory().DecodeL( headerDescriptor );

    iMsgAssembler.SetMessage( message );
    message = 0;

    // delete the header part from the received data,
    aData.Delete( 0, iMsgAssembler.MessageLength() );

    iMsgAssembler.ChangeState( TNATFWUNSAFMsgStateBase::EMsgComplete );
    return iMsgAssembler.CurrentState().DataReceivedL( aData, aNextLength );
    }

// End of File

