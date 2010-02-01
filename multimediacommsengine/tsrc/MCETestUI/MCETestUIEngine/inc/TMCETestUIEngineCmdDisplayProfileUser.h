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




#ifndef TMCETESTUIENGINECMDDISPLAYPROFILEUSER_H
#define TMCETESTUIENGINECMDDISPLAYPROFILEUSER_H

#include "TMCETestUIEngineCmdBase.h"

class CMCETestUIEngineProfile;

class TMCETestUIEngineCmdDisplayProfileUser : public TMCETestUIEngineCmdBase
{
  public:
    TMCETestUIEngineCmdDisplayProfileUser( 
	    CMCETestUIEngine& aEngine,
		  CMCETestUIEngineProfile& aProfile );
		  
	public: // Implementation for base class abstract methods

		/// Executes a sequence of operations.
		void ExecuteL();
		
		/// Returns short textual representation for behavior of command
		const TDesC& Caption() const;		
		
	private:
	  CMCETestUIEngineProfile& iProfile; // not owned
};

#endif
