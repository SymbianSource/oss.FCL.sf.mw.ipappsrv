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




#ifndef TMCETRANSACTIONDATACONTAINER_H
#define TMCETRANSACTIONDATACONTAINER_H

//  INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <mcedefs.h>

// CLASS DECLARATION

/**
* Container class to hold detailed data of a transaction.
*
* It is assumed, that user has only one instance of this class.
* The same instance is passed between MCE and user, but the data
* inside of this container is just updated for each event.
*
* The data inside the container is valid only during the execution
* of the callback function. After this the remaining data in container
* is deleted.
*
* @lib mceclient.lib
*/
class TMceTransactionDataContainer
    {
    public:
     
		/**
		* Default constructor
		*/
     	IMPORT_C TMceTransactionDataContainer();
     	
		/**
		* Gets status code
		* @returns status code of transaction, if any
		*/
     	IMPORT_C TInt GetStatusCode();
     	
		/**
		* Gets reason phrase.
		* @returns reason phrase of transaction, if any; 
		*          ownership is transferred
		*/
     	IMPORT_C HBufC8* GetReasonPhrase();
     	
		/**
		* Gets headers.
		* @returns headers of transaction, if any;
		*          ownership is transferred
		*/
     	IMPORT_C CDesC8Array* GetHeaders();
     	
		/**
		* Gets content type.
		* @returns content type of transaction, if any;
		*          ownership is transferred
		*/
     	IMPORT_C HBufC8* GetContentType();
     	
		/**
		* Gets content headers.
		* @returns headers of content, if any;
		*          ownership is transferred
		*/
     	IMPORT_C CDesC8Array* GetContentHeaders();
     	
		/**
		* Gets content.
		* @returns content of transaction, if any;
		*          ownership is transferred
		*/
     	IMPORT_C HBufC8* GetContent();
     	
    public:
     
     	void SetStatusCode( TInt aStatusCode );
     	
     	void SetReasonPhrase( HBufC8* aReasonPhrase );
     	
     	void SetHeaders( CDesC8Array* aHeaders );
     	
     	void SetContentType( HBufC8* aContentType );
     	
     	void SetContent( HBufC8* aContent );
     	
     	void SetContentHeaders( CDesC8Array* aHeaders );
     	
     	void Clear();
     	
    private:
    
		TUint32 iStatusCode;
		
		HBufC8* iReasonPhrase;
		
		CDesC8Array* iHeaders;
		
        HBufC8* iContentType;
        
		CDesC8Array* iContentHeaders;
		
		HBufC8* iContent;
		
    private: // Reserved for future use
    
        TAny* iReserved;       

		MCE_UNIT_TEST_DEFS
    };

#endif
