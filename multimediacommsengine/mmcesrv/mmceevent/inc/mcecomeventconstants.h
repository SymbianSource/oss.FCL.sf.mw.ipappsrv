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
* Description:    Constants needed by SSC Server
*
*/





#ifndef CMCECOMEVENTCONSTANTS_H
#define CMCECOMEVENTCONSTANTS_H

// CONSTANTS
const TInt KMceSrvEventStateArraySize = 5;
typedef TInt TMceEventStateIndex;

// State indices. The order is like this, because
// they are created in this order.
const TMceEventStateIndex KMceIdleEventStateIndex = 2;
const TMceEventStateIndex KMceEstablishingEventStateIndex = 3;
const TMceEventStateIndex KMceEstablishedEventStateIndex = 4;
const TMceEventStateIndex KMceTerminatingEventStateIndex = 0;
const TMceEventStateIndex KMceTerminatedEventStateIndex = 1;

const TInt KMceEventStateError = KErrTotalLossOfPrecision;

#endif      // CMCECOMEVENTCONSTANTS_H   
            
// End of File



        
