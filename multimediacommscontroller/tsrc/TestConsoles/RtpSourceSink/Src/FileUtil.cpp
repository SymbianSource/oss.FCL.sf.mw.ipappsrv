/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Part of TestConsole application.
**              Methods for the class TFileUtil
**
*/




#include <e32std.h>
#include "FileUtil.h"
#include "TestConsoleDef.h"


//**********************************************************************
// Method    : TFileUtil::WriteLogFile()
// Parameters:
// Function:
// Returns:
//**********************************************************************

TInt TFileUtil::WriteLogFile(const TDesC &name, const TDesC &p)
    {
	HBufC *msg = NULL;
	TRAPD(msgError, msg = HBufC::NewL(p.Length() + 50));

	if(msgError != KErrNone)
        {
		return 1;
        }

	TPtr lineBuf(msg->Des());
	lineBuf.Fill('\0', lineBuf.Length());

	// get the current time and date
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime() ;

	// add the current time and date 
	lineBuf.Format(_L("%02d/%02d/%04d\t%02d:%02d:%02d:%02d\t"),
		dateTime.Day()+1,
		dateTime.Month()+1,
		dateTime.Year(),
		dateTime.Hour(),
		dateTime.Minute(),
		dateTime.Second(),
		(dateTime.MicroSecond() / 1000)); 

	// followed by the formatted data
	// lineBuf.AppendFormatList(aFmt, aList, &overflow16
	lineBuf.Append(p);
	lineBuf.Append(_L(""));
	lineBuf.Append(KTxtLineBreak);
	lineBuf.ZeroTerminate();

	TFileUtil::LogMessage(name, lineBuf);
    delete msg;
	return KErrNone;
    }


//**********************************************************************
// Method    : TFileUtil::LogMessage()
// Parameters:
// Function:
// Returns:
//**********************************************************************

TInt TFileUtil::LogMessage(const TFileName& name, const TDesC& mess )
    {
	// convert from unicode to 8 bit
	TBuf8<KMaxLogLineLength> lineBuf8;
	const TText* msgBuf = mess.Ptr();
	for (TInt i = 0; i < mess.Length() ;i++)
        {
		lineBuf8.Append(STATIC_CAST(TText, msgBuf[i]));
        }

    RFs fs;
    RFile file;
    TFileName nameWithPath;

    nameWithPath = KDefaultPath;
    nameWithPath.Append( name );
    nameWithPath.ZeroTerminate( );
    TInt err = fs.Connect( );

    if(err!=KErrNone)
        {
        return err;
        }

    err=file.Open(fs, nameWithPath, EFileStreamText|EFileWrite|EFileShareExclusive);
    if(err==KErrNotFound)
        {
        err=file.Create(fs, nameWithPath, EFileStreamText|EFileWrite|EFileShareExclusive);
        }
    if(err!=KErrNone)
        {
        return err;
        }

    TInt off(0);
    err=file.Seek(ESeekEnd, off);
    if(err!=KErrNone)
        {
        return err;
        }

	file.Write(lineBuf8);
    file.Flush();
    file.Close();
    fs.Close();

    return KErrNone;
    }


//**********************************************************************
// Method    : TFileUtil::WriteLogFile()
// Parameters:
// Function:
// Returns:
//**********************************************************************

TInt TFileUtil::WriteLogFile(const TDesC& name, const TDesC8& p)
    {
    HBufC8* msg = NULL;
    TRAPD(msgError, msg = HBufC8::NewL(p.Length() + 50));
    if(msgError != KErrNone) 
        {
        return 1;
        }
	
    TPtr8 b( msg->Des() );
	b.Fill('\0',b.Length());
	TTime now;
	now.HomeTime();
	TDateTime dateTime = now.DateTime() ;

	// add the current time and date 
	b.AppendFormat(_L8("%02d/%02d/%04d\t%02d:%02d:%02d:%02d\t"),
		dateTime.Day()+1,
		dateTime.Month()+1,
		dateTime.Year(),
		dateTime.Hour(),
		dateTime.Minute(),
		dateTime.Second(),
		(dateTime.MicroSecond() / 1000)); 

    b.Append(p);
    b.Append(_L8("\n"));
	b.ZeroTerminate();

    TFileUtil::LogMessage(name, b);

    delete msg;
    return KErrNone;
    }


//**********************************************************************
// Method    : TFileUtil::LogMessage()
// Parameters:
// Function:
// Returns:
//**********************************************************************
TInt TFileUtil::LogMessage(const TFileName& name, const TDesC8& mess )
    {
    RFs fs;
    RFile file;
    TFileName nameWithPath;

    nameWithPath = KDefaultPath;
    nameWithPath.Append( name );
    nameWithPath.ZeroTerminate( );
    TInt err = fs.Connect( );

    if(err!=KErrNone)
        {
        return err;
        }

    err=file.Open(fs, nameWithPath, EFileStreamText|EFileWrite|EFileShareExclusive);
    if(err==KErrNotFound)
        {
        err=file.Create(fs, nameWithPath, EFileStreamText|EFileWrite|EFileShareExclusive);
        }

    if(err!=KErrNone)
        {
        return err;
        }

    TInt off(0);
    err=file.Seek(ESeekEnd, off);
    if(err!=KErrNone)
        {
        return err;
        }

    file.Write(mess);
    file.Flush();
    file.Close();
    fs.Close();

    return KErrNone;
    }

//**********************************************************************
// Method    : TFileUtil::InitLogFile()
// Parameters:
// Function:
// Returns:
//**********************************************************************
TInt TFileUtil::InitLogFile( const TFileName& name )
    {
    RFs fs;
    RFile file;
    TFileName nameWithPath;

    nameWithPath = KDefaultPath;
    nameWithPath.Append( name );
    nameWithPath.ZeroTerminate( );
    TInt err = fs.Connect( );

    if( err != KErrNone )
        {
        return err;
        }

    err = file.Replace( fs, nameWithPath, /*EFileStreamText|*/EFileWrite|EFileShareExclusive );
    if(err!=KErrNone)
        {
        return err;
        }

    file.Close();
    fs.Close();

    return KErrNone;
    }


//**********************************************************************
// Method    : TFileUtil::LogToFile()
// Parameters:
// Function:
// Returns:
//**********************************************************************

void TFileUtil::LogToFile( const TDesC& aName, TRefByValue<const TDesC> aFmt, ... )
    {
	VA_LIST aList;
	VA_START(aList, aFmt);

	// decode formatted data for display on console
	TBuf<KMaxLogLineLength> lineBuf;
	lineBuf.AppendFormatList(aFmt, aList, NULL);// &iOverflow16);

	// write to log file
	WriteLogFile(aName, lineBuf);

	VA_END(aList);
    }

//**********************************************************************
// Method    : TFileUtil::LogMsg()
// Parameters:
// Function:
// Returns:
//**********************************************************************
void TFileUtil::LogMsg( TDes& aDes, TRefByValue<const TDesC> aFmt, ...)
    {
	VA_LIST aList;
	VA_START(aList, aFmt);

	// Decode formatted data for display on console
	aDes.AppendFormatList(aFmt, aList, NULL);

	VA_END(aList);
    }

