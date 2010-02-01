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
* Description:    Definition and attributes for the class TFileUtil
**
*/




#ifndef __FILEUTIL_H
#define __FILEUTIL_H

#include <f32file.h> 
#include <e32base.h>
#include <e32cons.h>


class TFileUtil
    {
public:
    enum TDataType
        {
        EInteger, 
        EUInteger,
        EFloat,
        EDouble,
        ECharacter,
        EHexadecimal
        };

public:

    static 	TInt LogMessage(const TFileName&, const TDesC8&);
	
    static 	TInt WriteLogFile(const TDesC& name, const TDesC8& p);
    static 	TInt InitLogFile( const TFileName& name );

	static  TInt WriteLogFile(const TDesC& name, const TDesC& p);
	static  TInt LogMessage(const TFileName&, const TDesC&);

	static  void LogToFile(const TDesC& name, TRefByValue<const TDesC> aFmt, ...);
	static	void LogMsg(TDes& aDes, TRefByValue<const TDesC> aFmt, ...);
    };

#endif __FILEUTIL_H


