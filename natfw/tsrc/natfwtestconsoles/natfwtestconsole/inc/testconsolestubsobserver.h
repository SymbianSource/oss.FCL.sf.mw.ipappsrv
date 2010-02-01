/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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




#ifndef TESTCONSOLESTUBSOBSERVER_H
#define TESTCONSOLESTUBSOBSERVER_H

class CNATFWCandidate;
class CNATFWCandidatePair;

class MTestConsoleStubsObserver
    {
public:
    virtual void LocalCandidateFound( CNATFWCandidate& aLocalCandidate ) = 0;
    
    //virtual void NewCandidatePairFound( CNATFWCandidatePair* aCandidatePair ) = 0;
    };

#endif // TESTCONSOLESTUBSOBSERVER_H