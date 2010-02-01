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
* Description:  See class definition below.
*
*/

#ifndef __CTCSERIALCONNECTION_H__
#define __CTCSERIALCONNECTION_H__

// INCLUDES
#include "CTcBaseConnection.h"

#include <c32comm.h>

// CLASS DEFINITION
/**
 * CTcSerialConnection implements Serial port specific connection
 * initialization on top of CTcBaseConnection.
 */
class CTcSerialConnection
	: public CTcBaseConnection
	{
	public:	// Constructors and destructor

		/// Default constructor
		CTcSerialConnection( RCommServ& aCommServ, 
                             const TDesC& aPortName, 
                             TUint aPortSpeed );

		/// Destructor
		~CTcSerialConnection();
		
	protected: // From CActive
	
	    void DoCancel();

	public: // From CTcBaseConnection
	
	    void ConnectL();

		void Close();

		void Send( const TDesC8& aDes );

		void Receive( TDes8& aDes );

		void ReceiveOneOrMore( TDes8& aDes );

		void SetupPortL();

    private:
    
        void SetupPortBaudrateValuesL();
        
    private:
    
        RComm iCommPort;
        
        RCommServ& iCommServ;
        
        const TDesC& iPortName;
        
	    TUint iPortSpeed;
	    
	    TBps iBaudrate;
	    
	    TUint iBaudrateCap;
	};

#endif // __CTCSERIALCONNECTION_H__
