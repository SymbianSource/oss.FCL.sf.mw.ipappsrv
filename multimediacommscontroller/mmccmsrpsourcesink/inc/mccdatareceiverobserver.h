/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides a comfort noise generator class
*
*/



#ifndef MCCDATARECEIVEROBSERVER_H
#define MCCDATARECEIVEROBSERVER_H

#include <E32Base.h>

// CLASS DECLARATION

/**
*  CMccMsrpDataReceiver class declaration
*/
class MMccDataReceiverObserver
	{
    public:
    
		virtual void DataReceivedL( const TDesC8& aData ) = 0;

    };


#endif
