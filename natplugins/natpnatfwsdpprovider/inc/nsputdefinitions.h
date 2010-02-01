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
* Description:  Project wide unit testing definitions.
*
*/

#ifndef NSPUTDEFINITIONS_H
#define NSPUTDEFINITIONS_H

#ifdef TEST_EUNIT
#define NSP_UT_DEFINITIONS \
    friend class UT_CNSPController;\
    friend class UT_CNSPContentParser;\
    friend class UT_CNSPSession;\
    friend class UT_CNSPUseCases;\
    friend class UT_CNSPUseCases_fqdn;\
    friend class UT_NSPUtility;\
    friend class UT_CNSPMediaStreamContainer;\
    friend class UT_CNSPMediaStream;\
    friend class UT_CNSPMediaStreamComponent;\
    friend class UT_CNSPInterface;
#else // _DEBUG
#define NSP_UT_DEFINITIONS
#endif // _DEBUG

#endif // NSPUTDEFINITIONS_H

// end of file
