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




#ifndef MCCLOGS_H
#define MCCLOGS_H

#include <utf.h>
#include <e32debug.h>

_LIT( KMccWarning, "Mcc: Text contains not allowed characters, log ignored" );

class TMccLog
	{
	public:

    inline static void Print( const TDesC16& aName, const TDesC16& aStr ) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<128> str;
			_LIT( KMcc, "%S %S" );
			str.Format( KMcc(), &aName, &aStr );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccWarning );							
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
			TBuf<128> str;
			_LIT( KMcc, "%S %S %d" );
			str.Format( KMcc(), &aName, &aStr, aValue );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccWarning );							
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
			TBuf<200> str;
			_LIT( KMcc, "%S %S %d, %S %d" );
			str.Format( KMcc(), &aName, &aStr1, aValue1, 
			            &aStr2, aValue2 );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccWarning );							
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
		     aStr3.Locate( notAllowed ) == KErrNotFound)
			{
			TBuf<256> str;
			_LIT( KMcc, "%S %S %d, %S %d, %S %d, %S %d" );
			str.Format( KMcc(), &aName, &aStr1, aValue1, 
			            &aStr2, aValue2, &aStr3, aValue3, &aStr4, aValue4 );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccWarning );							
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
			TBuf<140> str;
			_LIT( KMcc, "%S %S %d, %d" );
			str.Format( KMcc(), &aName, &aStr1, aValue1, 
			            aValue2 );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccWarning );							
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
			TBuf<128> str;
			_LIT( KMcc, "%S, %S" );
			_LIT( KMcc2, "%S" );
			str.Format( KMcc(), &aName, &aStrA );
			RDebug::Print( str );
			
			const TInt KMccLogWriteBufSize = 50;
			TInt lastPos( aStrB.Length() - 1 );
			TInt prevPos = 0;
			TInt readLen = ( lastPos < KMccLogWriteBufSize ) ? ( lastPos + 1 ) : KMccLogWriteBufSize;
			while ( prevPos < lastPos )
			    {
			    TBuf<128> str2;
			    CnvUtfConverter::ConvertToUnicodeFromUtf8( str2, aStrB.Mid( prevPos, readLen ) );
			    str.Format( KMcc2(), &str2 );
		    	RDebug::Print( str );
			    prevPos += readLen;
			    readLen = ( prevPos + KMccLogWriteBufSize ) > lastPos ? 
			        ( lastPos - prevPos + 1 ) : KMccLogWriteBufSize;
			    }
			
			}
		else
			{
			RDebug::Print( KMccWarning );							
			}	
		}

    inline static void PrintReal( 
	    const TDesC16& aName, 
	    const TDesC16& aStr, 
	    TReal          aValue ) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<128> str;
			_LIT( KMcc, "%S %S %f" );
			str.Format( KMcc(), &aName, &aStr, aValue );
			RDebug::Print(str);				
			}
		else
			{
			RDebug::Print( KMccWarning );							
			}	
		}
		
    }; 

#endif // end of class TMccLog

// End of File
