/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MCCFILESOURCETIMER_H
#define MCCFILESOURCETIMER_H

#include <e32base.h>

class MMccEofTimerObserver
{
    public:
        virtual void EofTimerExpiredL() = 0;
};

class CMccEofTimer : public CTimer
    {
    public:
        static CMccEofTimer* NewL(MMccEofTimerObserver* aOwner);
        ~CMccEofTimer();
        
        void RunL(); // from CTimer

    protected:
        CMccEofTimer(MMccEofTimerObserver* aOwner);
        
    private:
        MMccEofTimerObserver* iOwner; // not owned
        
    };
#endif