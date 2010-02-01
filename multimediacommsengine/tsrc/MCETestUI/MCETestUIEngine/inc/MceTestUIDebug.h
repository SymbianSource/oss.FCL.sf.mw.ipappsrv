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






#ifndef MCETESTUILOGS_H
#define MCETESTUILOGS_H

#include <e32std.h>

// INCLUDES

#include <e32base.h>
#include <e32svr.h>
#include <utf.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * Class provides logging through RDebug in debug builds
 * '***MCETestUI***: ' added automatically in fron of every logged line
 */ 
class TMceTestUIDebug 
	{
	public:

#ifdef _DEBUG

	inline static void Print(const TDesC16& aStr) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "***MCETestUI***: %S");

			TBuf<256> str;
			str.Format(KMceFormat, &aStr);
			RDebug::Print(str);				
			}
		else
			{
			_LIT(KMceWarning, "***MCETestUI***: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr1, const TDesC16& aStr2) 
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound  &&
		     aStr2.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "***MCETestUI***: %S, %S");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, &aStr2);
			RDebug::Print(str);
			}
		else
			{
			_LIT(KMceWarning, "***MCETestUI***:: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr, TUint32 aValue) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "***MCETestUI***: %S, %d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr, aValue);
			RDebug::Print(str);
			}
		else
			{
			_LIT(KMceWarning, "***MCETestUI***: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr, TUint32 aValue1, TUint32 aValue2) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "***MCETestUI***: %S, %d, %d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr, aValue1, aValue2);
			RDebug::Print(str);
			}
		else
			{
			_LIT(KMceWarning, "MCE: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr, TUint32 aValue1, 
		TUint32 aValue2, TUint32 aValue3) 
		{
		TChar notAllowed('%');
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			_LIT(KMceFormat, "***MCETestUI***: %S, %d, %d, %d");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr, aValue1, aValue2, aValue3);
			RDebug::Print(str);
			}
		else
			{
			_LIT(KMceWarning, "***MCETestUI***: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

	inline static void Print(const TDesC16& aStr1, const TDesC8& aStr2) 
		{
		TChar notAllowed('%');
		if ( aStr1.Locate( notAllowed ) == KErrNotFound  &&
		     aStr2.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<100> str2;
			CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aStr2);

			_LIT(KMceFormat, "***MCETestUI***: %S, %S");
			TBuf<256> str;
			str.Format(KMceFormat, &aStr1, &str2);
			RDebug::Print(str);
			}
		else
			{
			_LIT(KMceWarning, "***MCETestUI***: Text contains not allowed characters, log ignored");
			RDebug::Print( KMceWarning );							
			}	
		}

#else

	inline static void Print(const TDesC16& /*aStr*/) 
		{
		}

	inline static void Print(const TDesC16& /*aStr1*/, const TDesC16& /*aStr1*/) 
		{
		}

	inline static void Print(const TDesC16& /*aStr*/, TUint32 /*aValue*/) 
		{
		}

	inline static void Print(const TDesC16& /*aStr*/, TUint32 /*aValue1*/, TUint32 /*aValue2*/) 
		{
		}

	inline static void Print(const TDesC16& /*aStr*/, TUint32 /*aValue1*/,
					  TUint32 /*aValue2*/, TUint32 /*aValue3*/) 
		{
		}

	inline static void Print(const TDesC16& /*aStr1*/, const TDesC8& /*aStr2*/) 
		{
		}

#endif

	};

/**
 * Macro for logging single string
 * MCE_DEBUG1("Hello")
 * Note, max length of printed line 256 characters
 */ 
#define MCE_TESTUI_DEBUG1( a ) { TMceTestUIDebug::Print( _L( a ) ); }

/**
 * Macro for logging two strings
 * MCE_DEBUG2("Hello", "Back")
 * Note, max length of printed line 256 characters
 */ 
#define MCE_TESTUI_DEBUG2( a, b ) { TMceTestUIDebug::Print( _L( a ), _L( b ) ); }

/**
 * Macro for logging a string and integer
 * MCE_DEBUG3("Hello", 1)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_TESTUI_DEBUG3( a, b ) { TMceTestUIDebug::Print( _L( a ), b ); }

/**
 * Macro for logging a string and 2 integers
 * MCE_DEBUG4("Hello", 1, 2)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_TESTUI_DEBUG4( a, b, c ) { TMceTestUIDebug::Print( _L( a ), b , c); }

 /**
 * Macro for logging a string and 3 integers
 * MCE_DEBUG5("Hello", 1, 2, 3)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_TESTUI_DEBUG5( a, b, c, d ) { TMceTestUIDebug::Print( _L( a ), b , c, d); }

 /**
 * Macro for logging a string and variable string
 * MCE_DEBUG6("Hello", string)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_TESTUI_DEBUG6( a, b) { TMceTestUIDebug::Print( _L( a ), b ); }

#endif