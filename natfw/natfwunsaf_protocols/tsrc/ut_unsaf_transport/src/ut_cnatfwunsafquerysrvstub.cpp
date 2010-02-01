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
* Description:  
*
*/




#include "ut_cnatfwunsafquerysrvstub.h"
#include "mnatfwunsafhostresolver.h"

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQuerySrvStub::NewL
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQuerySrvStub* UT_CNATFWUNSAFQuerySrvStub::NewL(const TDesC8& aTarget)
    {
    UT_CNATFWUNSAFQuerySrvStub* self = new (ELeave) UT_CNATFWUNSAFQuerySrvStub();
    CleanupStack::PushL(self);
    self->ConstructL(aTarget);
    CleanupStack::Pop();
    return self;
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQuerySrvStub::ConstructL
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFQuerySrvStub::ConstructL(const TDesC8& aTarget)
    {
    CNATFWUNSAFQuerySrv::ConstructL(aTarget);
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQuerySrvStub::UT_CNATFWUNSAFQuerySrvStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQuerySrvStub::UT_CNATFWUNSAFQuerySrvStub()
:CNATFWUNSAFQuerySrv()
    {
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQuerySrvStub::~UT_CNATFWUNSAFQuerySrvStub
// ----------------------------------------------------------------------------
//
UT_CNATFWUNSAFQuerySrvStub::~UT_CNATFWUNSAFQuerySrvStub()
    {
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQuerySrvStub::Query
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFQuerySrvStub::Query (
    MNATFWUNSAFHostResolver& aResolver )
    {
    aResolver.Resolver();
    iResult().SetPriority(10);
    iResult().SetWeight(5);
    iResult().SetPort(5093);
    iResult().SetTarget(_L8("SrvSecond.net"));
    }

// ----------------------------------------------------------------------------
// UT_CNATFWUNSAFQuerySrvStub::HandleQueryResultL
// ----------------------------------------------------------------------------
//
void UT_CNATFWUNSAFQuerySrvStub::HandleQueryResultL (
    MNATFWUNSAFHostResolver& aResolver )
    {
    TDnsRespSRV srv;
    srv.SetPort(iResult().Port());
    srv.SetTarget(iResult().Target());
    srv.SetWeight(iResult().Weight());
    srv.SetPriority(iResult().Priority());
    AddL(srv);
    aResolver.Resolver();

    iResult().SetPriority(10);
    iResult().SetWeight(6);
    iResult().SetPort(5093);
    iResult().SetTarget(_L8("SrvFirst.net"));

    TDnsRespSRV srv2;
    srv2.SetPort(iResult().Port());
    srv2.SetTarget(iResult().Target());
    srv2.SetWeight(iResult().Weight());
    srv2.SetPriority(iResult().Priority());
    AddL(srv2);

    iResult().SetPriority(12);
    iResult().SetWeight(3);
    iResult().SetPort(5093);
    iResult().SetTarget(_L8("SrvThird.net"));

    TDnsRespSRV srv3;
    srv3.SetPort(iResult().Port());
    srv3.SetTarget(iResult().Target());
    srv3.SetWeight(iResult().Weight());
    srv3.SetPriority(iResult().Priority());
    AddL(srv3);
    }
