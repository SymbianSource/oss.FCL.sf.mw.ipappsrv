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
* Description:    
*
*/




// UID of this interface
const TUid KTestInterfaceUid = {0x01700000};

inline CTestInterface::CTestInterface()
	{
	}

inline CTestInterface::~CTestInterface()
	{
	// Destroy any instance variables and then
	// inform the framework that this specific 
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}



inline CTestInterface* CTestInterface::NewL(const TUid& aImplUid, TestInterfaceInitParams& aParams)
	{
	// The CreateImplementationL method will return
	// the created item.
	return REINTERPRET_CAST( CTestInterface*, 
	    REComSession::CreateImplementationL( aImplUid,
										     _FOFF( CTestInterface, iDtor_ID_Key ),
											 &aParams ) );
	}

