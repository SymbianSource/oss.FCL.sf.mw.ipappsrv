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




#ifndef _MCCLOGS_H
#define _MCCLOGS_H

#include <utf.h>
#include <e32debug.h>
_LIT( KMCCWarning, "MCC: Text contains not allowed characters, log ignored" );

class TMCCLog
	{
	public:

    inline static void Print( const TDesC16& aName, const TDesC16& aStr ) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<256> str;
			_LIT( KMCC, "%S %S" );
			str.Format( KMCC(), &aName, &aStr );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMCCWarning );							
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
			_LIT( KMCC, "%S %S %d" );
			str.Format( KMCC(), &aName, &aStr, aValue );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMCCWarning );							
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
			_LIT( KMCC, "%S %S %d, %S %d" );
			str.Format( KMCC(), &aName, &aStr1, aValue1, 
			            &aStr2, aValue2 );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMCCWarning );							
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
			_LIT( KMCC, "%S %S %d, %d" );
			str.Format( KMCC(), &aName, &aStr1, aValue1, 
			            aValue2 );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMCCWarning );							
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
			_LIT(KMCC, "%S, %S %S");
			str.Format(KMCC(), &aName, &aStrA, &str2);
			RDebug::Print(str);
			}
		else
			{
			RDebug::Print( KMCCWarning );							
			}	
		}

    }; 

#endif // end of class TMCCLog

// End of File
