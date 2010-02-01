/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Provides services for candidate prioritization.
*
*/





#ifndef CICECANDIDATEPRIORITIZER_H
#define CICECANDIDATEPRIORITIZER_H

#include <e32base.h>
#include "natfwinternaldefs.h"    //UNIT_TEST

class CNATFWCandidate;
class CNATFWNatSettingsApi;

/**
 *  Provides services for candidate prioritization.
 *
 *  @lib icecandidatehandler.dll
 *  @since S60 v3.2
 */
class CIceCandidatePrioritizer : public CBase
    {

    UNIT_TEST( UT_CIceCandidatePrioritizer )
    
public:

    /**
     * Two-phased constructor.
     * @param   aDomain     Domain to be used in preference settings query
     */
    static CIceCandidatePrioritizer* NewL( const TDesC8& aDomain );

    /**
     * Two-phased constructor.
     * @param   aDomain     Domain to be used in preference settings query
     */
    static CIceCandidatePrioritizer* NewLC( const TDesC8& aDomain );

    /**
     * Destructor.
     */
    virtual ~CIceCandidatePrioritizer();
    
    /**
     * Calculates priority for candidate.
     *
     * @since   S60 v3.2
     * @param   aCandidate  Candidate to prioritize
     */
    void PrioritizeL( CNATFWCandidate& aCandidate ) const;

private:

    CIceCandidatePrioritizer();
    
    void ConstructL( const TDesC8& aDomain );

private:
    
    /**
     * Used to read prioritization preferences from settings.
     * Own.
     */
    CNATFWNatSettingsApi* iNatSettings;
    };


#endif // CICECANDIDATEPRIORITIZER_H
