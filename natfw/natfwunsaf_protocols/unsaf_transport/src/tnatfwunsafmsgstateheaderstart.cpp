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




#include "tnatfwunsafmsgstateheaderstart.h"
#include "mnatfwunsafmsgassemblerctx.h"

// ----------------------------------------------------------------------------
// TNATFWUNSAFMsgStateHeaderStart::DataReceivedL
// ----------------------------------------------------------------------------
//
TBool TNATFWUNSAFMsgStateHeaderStart::DataReceivedL(
    TPtr8 aData,
    TUint& aNextLength )
    {
    TInt messageLength( iMsgAssembler.MessageLength() );
    __ASSERT_ALWAYS( messageLength > 0,
                     User::Leave( KErrNotReady ) );

    TBool retVal( EFalse );

    if ( iMsgAssembler.MsgBuffer().Size() > 0 )
        {
        // If there's existing data in msg buffer insert the received data
        // to msg buffer.
        iMsgAssembler.MsgBuffer().InsertL( iMsgAssembler.MsgBuffer().Size(),
                                           aData );
        iMsgAssembler.MsgBuffer().Compress();

        // delete content of received data.
        aData.Delete( 0, aData.Length() );
        TPtr8 bufPtr = iMsgAssembler.MsgBuffer().Ptr( 0 );

        if ( bufPtr.Length() >= messageLength )
            {
            // There's enough data for complete NATFWUNSAF msg

            HBufC8* newData = HBufC8::NewLC( bufPtr.Length() );

            // copy the msg buffer data to new data
            TPtr8 newDataPtr = newData->Des();
            newDataPtr.Append( bufPtr );

            // clean the msg buffer
            iMsgAssembler.MsgBuffer().Reset();
            iMsgAssembler.MsgBuffer().Compress();

            iMsgAssembler.ChangeState(
                TNATFWUNSAFMsgStateBase::EMsgHeaderEnd );
            retVal =
                iMsgAssembler.CurrentState().DataReceivedL( newDataPtr,
                                                            aNextLength );
            CleanupStack::PopAndDestroy( newData );
            }
        }
    else if ( aData.Length() >= messageLength )
        {
        // There's enough data for complete NATFWUNSAF msg

        iMsgAssembler.ChangeState( TNATFWUNSAFMsgStateBase::EMsgHeaderEnd );
        retVal =
            iMsgAssembler.CurrentState().DataReceivedL( aData, aNextLength );
        }
    else
        {
        // There's not enough data for complete UNSAF message,
        // wait for more incoming data
        iMsgAssembler.MsgBuffer().InsertL( iMsgAssembler.MsgBuffer().Size(),
                                           aData );

        iMsgAssembler.MsgBuffer().Compress();

        aNextLength =
            messageLength - iMsgAssembler.MsgBuffer().Ptr( 0 ).Length();
        }

    return retVal;
    }

// End of File

