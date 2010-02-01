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
* Description:    Macros for logging
*
*/




#ifndef QOSCONTROLLERLOG_H
#define QOSCONTROLLERLOG_H

// INCLUDES

#include <e32base.h>
#include <e32svr.h>
#include <utf.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * Class provides logging through RDebug in debug builds
 * 'QosController: ' added automatically in fron of every logged line
 */ 
class TQosControllerDebug 
	{
	public:

#ifdef _DEBUG

	inline static void Print(const TDesC16& aStr) 
		{
		_LIT(KQosControllerFormat, "QosController: %S");

		TBuf<256> str;
		str.Format(KQosControllerFormat, &aStr);
		RDebug::Print(str);
		}

	inline static void Print(const TDesC16& aStr1, const TDesC16& aStr2) 
		{
		_LIT(KQosControllerFormat, "QosController: %S, %S");
		TBuf<256> str;
		str.Format(KQosControllerFormat, &aStr1, &aStr2);
		RDebug::Print(str);
		}

	inline static void Print(const TDesC16& aStr, TUint32 aValue) 
		{
		_LIT(KQosControllerFormat, "QosController: %S, %d");
		TBuf<256> str;
		str.Format(KQosControllerFormat, &aStr, aValue);
		RDebug::Print(str);
		}

	inline static void Print(const TDesC16& aStr, TUint32 aValue1, TUint32 aValue2) 
		{
		_LIT(KQosControllerFormat, "QosController: %S, %d, %d");
		TBuf<256> str;
		str.Format(KQosControllerFormat, &aStr, aValue1, aValue2);
		RDebug::Print(str);
		}

	inline static void Print(const TDesC16& aStr, TUint32 aValue1, 
		TUint32 aValue2, TUint32 aValue3) 
		{
		_LIT(KQosControllerFormat, "QosController: %S, %d, %d, %d");
		TBuf<256> str;
		str.Format(KQosControllerFormat, &aStr, aValue1, aValue2, aValue3);
		RDebug::Print(str);
		}

	inline static void Print(const TDesC16& aStr1, const TDesC8& aStr2) 
		{
		TBuf<100> str2;
		CnvUtfConverter::ConvertToUnicodeFromUtf8(str2, aStr2);

		_LIT(KQosControllerFormat, "QosController: %S, %S");
		TBuf<256> str;
		str.Format(KQosControllerFormat, &aStr1, &str2);
		RDebug::Print(str);
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
 * __TRACE_MCC_QOS_CONTROLLER1("Hello")
 * Note, max length of printed line 256 characters
 */ 
#define __TRACE_MCC_QOS_CONTROLLER1( a ) { TQosControllerDebug::Print( _L( a ) ); }

/**
 * Macro for logging two strings
 * __TRACE_MCC_QOS_CONTROLLER2("Hello", "Back")
 * Note, max length of printed line 256 characters
 */ 
#define __TRACE_MCC_QOS_CONTROLLER2( a, b ) { TQosControllerDebug::Print( _L( a ), _L( b ) ); }

/**
 * Macro for logging a string and integer
 * __TRACE_MCC_QOS_CONTROLLER3("Hello", 1)
 * Note, max length of printed line 256 characters
 */ 
#define __TRACE_MCC_QOS_CONTROLLER3( a, b ) { TQosControllerDebug::Print( _L( a ), b ); }

/**
 * Macro for logging a string and 2 integers
 * __TRACE_MCC_QOS_CONTROLLER4("Hello", 1, 2)
 * Note, max length of printed line 256 characters
 */ 
#define __TRACE_MCC_QOS_CONTROLLER4( a, b, c ) { TQosControllerDebug::Print( _L( a ), b , c); }

 /**
 * Macro for logging a string and 3 integers
 * __TRACE_MCC_QOS_CONTROLLER5("Hello", 1, 2, 3)
 * Note, max length of printed line 256 characters
 */ 
#define __TRACE_MCC_QOS_CONTROLLER5( a, b, c, d ) { TQosControllerDebug::Print( _L( a ), b , c, d); }

 /**
 * Macro for logging a string and variable string
 * __TRACE_MCC_QOS_CONTROLLER6("Hello", string)
 * Note, max length of printed line 256 characters
 */ 
#define __TRACE_MCC_QOS_CONTROLLER6( a, b) { TQosControllerDebug::Print( _L( a ), b ); }

#endif
