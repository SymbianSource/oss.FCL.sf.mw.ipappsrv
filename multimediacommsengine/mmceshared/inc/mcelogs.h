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






#ifndef MCELOGS_H
#define MCELOGS_H

#include <e32std.h>

// INCLUDES

#include <e32base.h>
#include <e32svr.h>
#include <utf.h>
#include <flogger.h>
#include <f32file.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

_LIT( KMceLogPath, "C:\\Logs\\Mce\\" );
_LIT( KMceLogDir, "Mce" );
_LIT( KMceLogFile, "MceLog.txt" );

/**
 * Class provides logging through RFileLogger or RDebug in debug builds
 * 'MCE: ' added automatically in fron of every logged line
 */ 
class TMceDebug 
	{
	public:

#ifdef _DEBUG

// To disable file logging totally (so that RFileLogger server is not used even
// for determining whether file logging is possible), comment line below
#define MCE_FILE_LOGGING_ENABLED
	    
#if ( defined __WINSCW__ ) || ( defined __WINS__ )
// No sense to have file logging enabled in wins
#undef MCE_FILE_LOGGING_ENABLED
#endif

	inline static void Print( const TDesC16& aStr, TBool aSafetyChecked = EFalse ) 
		{
		TChar notAllowed('%');
		TBuf<256> str;
		_LIT( KMceFormat, "MCE: %S" );
		_LIT( KMceWarning, "MCE: Text contains not allowed characters, log ignored");
		
		if ( aSafetyChecked || aStr.Locate( notAllowed ) == KErrNotFound )
			{
			str.Format( KMceFormat, &aStr );
			}
		else
			{
			str.Copy( KMceWarning );
			}	
		
        if ( LogToFile( KMceLogPath ) )
            {
            RFileLogger::Write( KMceLogDir, KMceLogFile,
                EFileLoggingModeAppend, str );
            }
        else
            {
            RDebug::Print( str );
            }
		}

	inline static void Print( const TDesC16& aStr1, const TDesC16& aStr2 ) 
		{
		TChar notAllowed('%');
		TBuf<256> str;
		_LIT( KMceFormat, "MCE: %S, %S" );
		_LIT( KMceWarning, "MCE: Text contains not allowed characters, log ignored" );
		
		if ( aStr1.Locate( notAllowed ) == KErrNotFound  &&
		     aStr2.Locate( notAllowed ) == KErrNotFound )
			{
			str.Format( KMceFormat, &aStr1, &aStr2 );
			}
		else
			{
			str.Copy( KMceWarning );
			}
		
        if ( LogToFile( KMceLogPath ) )
            {
            RFileLogger::Write( KMceLogDir, KMceLogFile,
                EFileLoggingModeAppend, str );
            }
        else
            {
            RDebug::Print( str );
            }
		}

	inline static void Print( const TDesC16& aStr, TUint32 aValue ) 
		{
		TChar notAllowed('%');
		TBuf<256> str;
		_LIT( KMceFormat, "MCE: %S, %d" );
		_LIT( KMceWarning, "MCE: Text contains not allowed characters, log ignored" );
		
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			str.Format( KMceFormat, &aStr, aValue );
			}
		else
			{
			str.Copy( KMceWarning );
			}
			
		if ( LogToFile( KMceLogPath ) )
            {
            RFileLogger::Write( KMceLogDir, KMceLogFile,
                EFileLoggingModeAppend, str );
            }
        else
            {
            RDebug::Print( str );
            }
		}

	inline static void Print( const TDesC16& aStr, TUint32 aValue1, TUint32 aValue2 ) 
		{
		TChar notAllowed('%');
		TBuf<256> str;
		_LIT( KMceFormat, "MCE: %S, %d, %d" );
		_LIT( KMceWarning, "MCE: Text contains not allowed characters, log ignored" );
		
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			str.Format( KMceFormat, &aStr, aValue1, aValue2 );
			}
		else
			{
			str.Copy( KMceWarning );
			}
		
        if ( LogToFile( KMceLogPath ) )
            {
            RFileLogger::Write( KMceLogDir, KMceLogFile,
                EFileLoggingModeAppend, str );
            }
        else
            {
            RDebug::Print( str );
            }
		}

	inline static void Print( const TDesC16& aStr, TUint32 aValue1, 
		TUint32 aValue2, TUint32 aValue3 ) 
		{
		TChar notAllowed('%');
		TBuf<256> str;
		_LIT( KMceFormat, "MCE: %S, %d, %d, %d" );
		_LIT( KMceWarning, "MCE: Text contains not allowed characters, log ignored" );
		
		if ( aStr.Locate( notAllowed ) == KErrNotFound )
			{
			str.Format( KMceFormat, &aStr, aValue1, aValue2, aValue3 );
			}
		else
			{
			str.Copy( KMceWarning );
			}
		
        if ( LogToFile( KMceLogPath ) )
            {
            RFileLogger::Write( KMceLogDir, KMceLogFile,
                EFileLoggingModeAppend, str );
            }
        else
            {
            RDebug::Print( str );
            }
		}

	inline static void Print( const TDesC16& aStr1, const TDesC8& aStr2 ) 
		{
		TChar notAllowed('%');
		TBuf<256> str;
		_LIT( KMceFormat, "MCE: %S, %S" );
		_LIT( KMceWarning, "MCE: Text contains not allowed characters, log ignored" );
		
		if ( aStr1.Locate( notAllowed ) == KErrNotFound  &&
		     aStr2.Locate( notAllowed ) == KErrNotFound )
			{
			TBuf<100> str2;
			CnvUtfConverter::ConvertToUnicodeFromUtf8( str2, aStr2 );

			str.Format( KMceFormat, &aStr1, &str2 );
			}
		else
			{
			str.Copy( KMceWarning );
			}
		
        if ( LogToFile( KMceLogPath ) )
            {
            RFileLogger::Write( KMceLogDir, KMceLogFile,
                EFileLoggingModeAppend, str );
            }
        else
            {
            RDebug::Print( str );
            }
		
		}
	
    private:
    
#ifdef MCE_FILE_LOGGING_ENABLED
        
    inline static TBool LogToFile( const TDesC& aFolderName )
        {
        TBool exists( EFalse );
        RFs fs;
        if ( KErrNone == fs.Connect() )
            {
            TEntry entry;
            exists = ( fs.Entry( aFolderName, entry ) == KErrNone );
            fs.Close();        
            }
        return exists;
        }
#else
 
    inline static TBool LogToFile( const TDesC& /*aFolderName*/ )
        {
        return EFalse;
        }
    
#endif
    
#else
	
	inline static void Print( const TDesC16& /*aStr*/, TBool /*aSafetyChecked*/ = EFalse ) 
		{
		}

	inline static void Print( const TDesC16& /*aStr1*/, const TDesC16& /*aStr1*/ ) 
		{
		}

	inline static void Print( const TDesC16& /*aStr*/, TUint32 /*aValue*/ ) 
		{
		}

	inline static void Print( const TDesC16& /*aStr*/, TUint32 /*aValue1*/, TUint32 /*aValue2*/ ) 
		{
		}

	inline static void Print( const TDesC16& /*aStr*/, TUint32 /*aValue1*/,
					  TUint32 /*aValue2*/, TUint32 /*aValue3*/ ) 
		{
		}

	inline static void Print( const TDesC16& /*aStr1*/, const TDesC8& /*aStr2*/ ) 
		{
		}

#endif

	};

/**
 * Macro for logging single string
 * MCE_DEBUG1("Hello")
 * Note, max length of printed line 256 characters
 */ 
#define MCE_DEBUG1( a ) { TMceDebug::Print( _L( a ) ); }

/**
 * Macro for logging two strings
 * MCE_DEBUG2("Hello", "Back")
 * Note, max length of printed line 256 characters
 */ 
#define MCE_DEBUG2( a, b ) { TMceDebug::Print( _L( a ), _L( b ) ); }

/**
 * Macro for logging a string and integer
 * MCE_DEBUG3("Hello", 1)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_DEBUG3( a, b ) { TMceDebug::Print( _L( a ), b ); }

/**
 * Macro for logging a string and 2 integers
 * MCE_DEBUG4("Hello", 1, 2)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_DEBUG4( a, b, c ) { TMceDebug::Print( _L( a ), b , c); }

 /**
 * Macro for logging a string and 3 integers
 * MCE_DEBUG5("Hello", 1, 2, 3)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_DEBUG5( a, b, c, d ) { TMceDebug::Print( _L( a ), b , c, d); }

 /**
 * Macro for logging a string and variable string
 * MCE_DEBUG6("Hello", string)
 * Note, max length of printed line 256 characters
 */ 
#define MCE_DEBUG6( a, b) { TMceDebug::Print( _L( a ), b ); }

#endif

// End of File
