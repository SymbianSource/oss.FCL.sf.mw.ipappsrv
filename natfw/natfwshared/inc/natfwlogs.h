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




#ifndef NATFWLOGS_H
#define NATFWLOGS_H

#include <utf.h>
#include <e32debug.h>
#include <in_sock.h>


_LIT( KNATFWWarning, "NATFW: Text contains not allowed characters, log ignored" );

class TNATFWLog
    {
    public:

    inline static void Print( const TDesC16& aName, const TDesC16& aStr ) 
        {
        TChar notAllowed('%');
        if ( aStr.Locate( notAllowed ) == KErrNotFound )
            {
            TBuf<256> str;
            _LIT( KNATFW, "%S %S" );
            str.Format( KNATFW(), &aName, &aStr );
            RDebug::Print(str);
            }
        else
            {
            RDebug::Print( KNATFWWarning );
            }    
        }    

    inline static void Print( 
        const TDesC16& aName, 
        const TDesC16& aStr, 
        TInt           aValue ) 
        {
        TChar notAllowed('%');
        if ( aStr.Locate( notAllowed ) == KErrNotFound )
            {
            TBuf<256> str;
            _LIT( KNATFW, "%S %S %d" );
            str.Format( KNATFW(), &aName, &aStr, aValue );
            RDebug::Print(str);                
            }
        else
            {
            RDebug::Print( KNATFWWarning );                            
            }    
        }
        
    inline static void Print(
        const TDesC16& aName, 
        const TDesC16& aStr1, 
        TInt           aValue1, 
        const TDesC16& aStr2, 
        TInt           aValue2) 
        {
        TChar notAllowed('%');
        if ( aStr1.Locate( notAllowed ) == KErrNotFound &&
             aStr2.Locate( notAllowed ) == KErrNotFound)
            {
            TBuf<256> str;
            _LIT( KNATFW, "%S %S %d, %S %d" );
            str.Format( KNATFW(), &aName, &aStr1, aValue1, 
                        &aStr2, aValue2 );
            RDebug::Print(str);                
            }
        else
            {
            RDebug::Print( KNATFWWarning );                            
            }
        }
        
    inline static void Print(
        const TDesC16& aName, 
        const TDesC16& aStr1, 
        TInt           aValue1, 
        const TDesC16& aStr2, 
        TInt           aValue2, 
        const TDesC16& aStr3, 
        TInt           aValue3 ) 
        {
        TChar notAllowed('%');
        if ( aStr1.Locate( notAllowed ) == KErrNotFound &&
             aStr2.Locate( notAllowed ) == KErrNotFound &&
             aStr3.Locate( notAllowed ) == KErrNotFound )
            {
            TBuf<256> str;
            _LIT( KNATFW, "%S %S %d, %S %d, %S %d" );
            str.Format( KNATFW(), &aName, &aStr1, aValue1, 
                        &aStr2, aValue2, &aStr3, aValue3 );
            RDebug::Print( str );
            }
        else
            {
            RDebug::Print( KNATFWWarning );                            
            }
        }
    
    inline static void Print(
        const TDesC16& aName, 
        const TDesC16& aStr1, 
        TInt           aValue1, 
        const TDesC16& aStr2, 
        TInt           aValue2, 
        const TDesC16& aStr3, 
        TInt           aValue3, 
        const TDesC16& aStr4, 
        TInt           aValue4) 
        {
        TChar notAllowed('%');
        if ( aStr1.Locate( notAllowed ) == KErrNotFound &&
             aStr2.Locate( notAllowed ) == KErrNotFound &&
             aStr3.Locate( notAllowed ) == KErrNotFound &&
             aStr4.Locate( notAllowed ) == KErrNotFound)
            {
            TBuf<256> str;
            _LIT( KNATFW, "%S %S %d, %S %d, %S %d, %S %d" );
            str.Format( KNATFW(), &aName, &aStr1, aValue1, 
                        &aStr2, aValue2, &aStr3, aValue3, &aStr4, aValue4 );
            RDebug::Print(str);                
            }
        else
            {
            RDebug::Print( KNATFWWarning );                            
            }
        }    

    inline static void Print(
        const TDesC16& aName, 
        const TDesC16& aStr1, 
        TInt           aValue1, 
        TInt           aValue2) 
        {
        TChar notAllowed('%');
        if ( aStr1.Locate( notAllowed ) == KErrNotFound )
            {
            TBuf<256> str;
            _LIT( KNATFW, "%S %S %d, %d" );
            str.Format( KNATFW(), &aName, &aStr1, aValue1, 
                        aValue2 );
            RDebug::Print(str);                
            }
        else
            {
            RDebug::Print( KNATFWWarning );                            
            }
        }

    inline static void Print(
        const TDesC16& aName,
        const TDesC16& aStrA,
        const TDesC8& aStrB) 
        {
        TChar notAllowed('%');
        if ( aName.Locate( notAllowed ) == KErrNotFound  &&
             aStrB.Locate( notAllowed ) == KErrNotFound )
            {
            TBuf<100> str2;
            CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aStrB);

            TBuf<256> str;
            _LIT(KNATFW, "%S, %S %S");
            str.Format(KNATFW(), &aName, &aStrA, &str2);
            RDebug::Print(str);
            }
        else
            {
            RDebug::Print( KNATFWWarning );                        
            }    
        }
    
    inline static void Print( const TDesC16& aStr,
                              const TInetAddr& aAddr )
        {
        _LIT(KLogFormat, "NATFWMultiplexer Protocols: %S: %S port %d \n");
        const TInt KIPv6AddrMaxLen = 39;
        TBuf<KIPv6AddrMaxLen> addrBuf;
        aAddr.Output(addrBuf);
        TBuf<256> str;
        str.Format(KLogFormat, &aStr, &addrBuf, aAddr.Port());

        RDebug::RawPrint(str);
        }
    
    inline static void Print( const TDesC16& aModuleName, 
                              const TDesC16& aTraceStr,
                              const TInetAddr& aAddr )
        {
        const TInt KIPv6AddrMaxLen = 39;
        TBuf<KIPv6AddrMaxLen> addrBuf;
        aAddr.Output( addrBuf );
        
        TBuf<256> str;
        _LIT( KLogFormat, "%S %S %S port %d \n" );
        str.Format( KLogFormat, &aModuleName, &aTraceStr, &addrBuf, aAddr.Port() );

        RDebug::RawPrint( str );
        }
    }; 

#endif // end of class TNATFWLog

// End of File
