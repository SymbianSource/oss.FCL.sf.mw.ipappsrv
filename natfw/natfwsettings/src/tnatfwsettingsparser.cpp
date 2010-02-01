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




#include "tnatfwsettingsparser.h"
#include <badesca.h>

const TUint8 KCharToSeparateAddressAndPort = ':';
const TUint8 KCharToSeparateEComPlugInData = ',';
const TUint KDefaultGranularity = 1;

// ---------------------------------------------------------------------------
// TNATFWSettingsParser::ParseDataToArrayL
// ---------------------------------------------------------------------------
//
CDesC8Array* TNATFWSettingsParser::ParseDataToArrayL(
    const TDesC8& aData )
    {
    TInt error = KErrNone;
    TPtrC8 dataPointer;
    TUint nextDataStartIndex( 0 );
    TUint arrayLength = aData.Length();
    
    CDesC8ArrayFlat* descArray =
        new( ELeave ) CDesC8ArrayFlat( KDefaultGranularity );
    CleanupStack::PushL( descArray );
    
    for ( TUint i( 0 ); i < arrayLength; i++ )
        {
        if (  KCharToSeparateEComPlugInData == aData[i] )
            {
            dataPointer.Set( aData.Mid( nextDataStartIndex, i - nextDataStartIndex ) );
            if( !dataPointer.Length() )
                {
                error = KErrCorrupt;
                break;
                }
            descArray->AppendL( dataPointer );
            nextDataStartIndex = i + 1;
            }
        if ( i + 1 == arrayLength )
            {
            dataPointer.Set( aData.Mid( nextDataStartIndex, ( i - nextDataStartIndex ) + 1 ) );
            if( !dataPointer.Length() )
                {
                error = KErrCorrupt;
                break;
                }
            descArray->AppendL( dataPointer );
            }
        }
    if ( error )
        {
        descArray->Reset();
        }
    CleanupStack::Pop( descArray );
    return descArray;
    }


// ---------------------------------------------------------------------------
// TNATFWSettingsParser::ParseAddressL
// ---------------------------------------------------------------------------
//
HBufC8* TNATFWSettingsParser::ParseAddressL( const TDesC8& aData,
                                          TInt& aPort )
    {
    HBufC8* buffer = ParseAddressLC( aData, aPort );
    CleanupStack::Pop( buffer );
    return buffer;
    }


// ---------------------------------------------------------------------------
// TNATFWSettingsParser::ParseAddressLC
// ---------------------------------------------------------------------------
//
HBufC8* TNATFWSettingsParser::ParseAddressLC( const TDesC8& aData,
                                          TInt& aPort )
    {
    HBufC8* buffer;
    TInt result( aData.LocateReverse(
        TChar( KCharToSeparateAddressAndPort ) ) );
    if ( result != KErrNotFound )
        { 
        // result + 1, because we don't care separator character
        if (  KErrNone == TLex8( aData.Mid( result + 1 ) ).Val( aPort ) )
            {
            buffer = aData.Left( result ).AllocLC();
            }
        else
            {
            aPort = 0;
            buffer = aData.AllocLC();
            }
        }
    else
        {
        aPort = 0;
        buffer = aData.AllocLC();
        }
    return buffer;
    }

//End of file
