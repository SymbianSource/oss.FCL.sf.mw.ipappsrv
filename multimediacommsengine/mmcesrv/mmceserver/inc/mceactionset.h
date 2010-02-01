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




#ifndef MCEACTIONSET_H
#define MCEACTIONSET_H

#include <e32std.h>
#include "mcesrv.h"
#include "mcesession.h"
#include "mcestate.h"
#include "mcesipsession.h"
#include "mcedefs.h"


// FORWARD DECLARATION
class CMceSipSession;
class CSIPServerTransaction;
class CSIPClientTransaction;
class CMceMsgBase;
class CMceMediaManager;
class CSIPHeaderBase;
class CSdpDocument;
class CSIPMessageElements;
class CMceComSession;
class CSIPResponseElements;
class CMceMsgSIPReply;
class CMceMsgSIPData;
class TMceMccComEvent;
class TMceStateTransitionEvent;


const TInt KMceAutoReasonPhrase = KErrNotFound;

// CLASS DECLARATION

/**
*  MCE SIP session.
*
*  @lib 
*  @since
*/
class TMceActionSet 
	{
	
public: // Constructors and destructor

	/**
	* C++ constructor
	* @param aSession, reference to session.
	*/
	TMceActionSet( CMceSipSession& aSession );

public://state changing related

	/**
	* Changes server state
	* @param aNewState new server state
	*/
    void StateChanged( TMceStateIndex aNewState );

	/**
	* Changes client state synchronously as a response 
	* to client's ITC call
	* @param aIds set of ids defining ITC call context
	* @param aNewState new client state
	* @param aStatus status
	*/
    void ClientStateChanged( TMceIds& aIds, 
                             CMceSession::TState aNewState,
                             TInt aStatus = KErrNone );
                             
	/**
	* Changes client state. 
	* This is called when there is no SIP data for client
	* explaining the reason of state change. This happens,
	* for example, when client is establishing session and
	* there is no response, but media manager fails in offer
	* creation.
	* @param aNewState new client state
	* @param aReason a reason of state change
	*/
    void ClientStateChangedL( CMceSession::TState aNewState, 
                              TInt aReason = KErrNone );
        
	/**
	* Changes client state. 
	* This is called when there is SIP response explaining
	* the reason of state change. If handling of response fails
	* (error) reason is used for identifying the problem.
	* @param aNewState new client state
	* @param aResponse SIP response
	* @param aReason a reason of state change
	*/
    void ClientStateChangedL( CMceSession::TState aNewState, 
                              CSIPClientTransaction& aResponse,
                              TInt aReason = KErrNone );
                              
                              
	/**
	* Changes client state. 
	* This is called when there is SIP request explaining
	* the reason of state change. Only BYE is supported.
	* This method should not be used when handling INVITE request,
	* becauseINVITE causes always new incoming session creation and thus
	* state change is always EIncoming. 
	* @param aNewState new client state
	* @param aRequest SIP request
	* @param aReason a reason of state change
	*/
    void ClientStateChangedL( CMceSession::TState aNewState, 
                              CSIPServerTransaction& aRequest );


	/**
	* Changes client state. Used in session update.
	* This is called when there is SIP response explaining
	* the reason of state change and client session (body) must be 
	* updated because of it. If handling of response has been 
	* failed (error) reason is used for identifying the problem.
	* If reason is other than KErrNone, EMceItcUpdateFailed is 
	* used in callback.
	* @param aNewState new client state
	* @param aBody client session
	* @param aResponse SIP response
	* @param aReason a reason of state change
	*/
    void ClientStateChangedL( CMceSession::TState aNewState, 
                              CMceComSession& aBody,
                              CSIPClientTransaction& aResponse,
                              TInt aReason = KErrNone );

	/**
	* Changes client state. Used in session update.
	* This is called when client session (body) must be 
	* updated. This happens, for example, when client is updating
	* session, but media manager fails in update creation and 
	* rollback is needed.
	* EMceItcUpdateFailed call back is used always
	* used in callback.
	* @param aNewState new client state
	* @param aBody client session
	* @param aReason a reason of state change
	*/
    void ClientStateChangedL( CMceSession::TState aNewState, 
                              CMceComSession& aBody, 
                              TInt aReason );

    
    
	/**
	* informs client of occured severe error
	* @param aError a reason of error
	*/
    void ClientErrorOccured( TInt aError );
    
    
public://basic session establisment and update
    
    /**
    * Encode body to SDP. Encoded body can me obtained with 
    * Offer() method.
    */
    void EncodeL();
    
    /**
    * Decode body from SDP, which is created with
    * method CreateSDP() and stored as session offer.
    * @return warning code
    */
    TMceSipWarningCode DecodeL();
    
    /**
    * Decode body from SDP, which is created with
    * method CreateSDP() and stored as session offer.
    * @return warning code
    */
    TMceSipWarningCode Decode();

    /**
    * Update media manager according to body
    * @return return status:
    *       -System error < 0: error situation
    *       -KMceReady: update was synchronous
    *       -KMceAsync: updating asynchronously, wait EMEMediaUpdated event. 
    */
    TMceReturnStatus UpdateL();
    
    /**
    * Update media manager according to body
    * @return return status:
    *       -System error < 0: error situation
    *       -KMceReady: update was synchronous
    *       -KMceAsync: updating asynchronously, wait EMEMediaUpdated event. 
    */
    TMceReturnStatus Update();

    /**
    * Reserve resources
    * @return return status:
    *       -System error < 0: error situation
    *       -KMceReady: reservation was synchronous or not needed
    *       -KMceAsync: reserving asynchronously, wait EMEMediaReserved event. 
    */                  
    TMceReturnStatus ReserveL();
    
    /**
    * Update clients parameters in body
    * @param aIds set of ids defining ITC call context
    * @param aBody client session
    */
    void UpdateBodyL( TMceIds& aIds, CMceMsgBase& aBody );
    
    /**
    * Create body candidate for session update.
    * @param aType the direction of the body candidate
    */
    void CreateBodyCandidateL( CMceComSession::TType aType );
    
    /**
    * Returns truth value, reservation needed.
    * @return ETrue, if reservation is needed
    */
    TBool ReservationNeeded();
    
    /**
	* Sends INVITE with SDP
	*/
    void SendInviteL();
    
	/**
	* Informs client about new incoming session
	* @param aInvite server transaction
	*/
    void ClientUpdateIncomingSessionL( CSIPServerTransaction& aInvite );
    
	/**
	* Informs client about new incoming session update
	* @param aInvite server transaction
	* @param aCallback callback for client
	*/
    void ClientUpdateSessionL( CSIPServerTransaction& aInvite, 
                               TUint32 aCallback = EMceItcIncomingUpdate );
    
public://requests

	/**
	* Sends INVITE
	* @param aData session body for creation of SDP content
	*/
    void SendInviteL( CMceComSession& aData );
    
	/**
	* Sends ACK to response (2XX)
	* @param aResponse client transaction
	*/
	void SendACKL( CSIPClientTransaction& aResponse );
	
	/**
	* Sends BYE with default headers
	*/
    void SendBYEL();
    
	/**
	* Sends BYE with client defined headers and content
	* This is called when client terminates (EMceItcTerminateSession)
	* @param aData data containing headers
	*/
    void SendBYEL( CMceMsgBase& aData );

	/**
	* Sends CANCEL
    * @return ETrue, if CANCEL was sent
	*/
    TBool SendCancelL();
    
    /**
	* Sends UPDATE
    * @return ETrue, if UPDATE was sent
	*/
    void SendUpdateRequestL();
    
    /**
	* Sends PRACK
	* @param aContent if ETrue, use content in PRACK
    * @return ETrue, if PRACK was sent
	*/
    void SendPrackL( TBool aContent = ETrue );
    
	/**
	* Sends Extension Request 
	* @param  aData SIP data 
	* @param aIds set of ids defining ITC call context
   	*/
    void SendExtensionRequestL( CMceMsgBase& aData, TMceIds& aIds );
    
    
    /**
	* Receive Response to the Extension Requests sent.
	* @param aEvent, event
	*/
    void ReceiveExtensionRequestL( TMceStateTransitionEvent& aEvent );
    
    /**
	* Receive Error for an Extension Request sent. 
	* Error Can be anything like ICMP, SIP Timeout.
	* @param aRequest, request for which error is received.
	*/
    void ReceiveExtensionError( CSIPTransactionBase& aTransaction, TInt aError );

                 
public://responses


	/**
	* Sends answer to offer carried by INVITE
	* This is called when client accepts (EMceItcAcceptSession)
	* @param aRequest server transaction
	* @param aReply user defined response (CMceMsgSIPReply)
	*/
    void SendAnswerL( CSIPServerTransaction& aRequest, 
                      CMceMsgBase& aReply );
                      
	/**
	* Sends answer to offer carried by re-INVITE
	* @param aRequest the re-invite
	*/
    void SendAnswerL( CSIPServerTransaction& aRequest );
    
	/**
	* Sends rejection 488 Not Acceptable Here to offer with warning,
	* if warning is assigned to one of 3xx warnings. If warning is assigned
	* to 4xx, 5xx or 6xx it is been interpretet as response code.
	* @param aReason reason why 488 or response code is sent
	* @param aInvite server transaction
	*/
    void SendRejectOfferWithWarningL( TMceSipWarningCode aReason, 
                                      CSIPServerTransaction& aInvite );
	/**
	* Sends 200OK reponse (to BYE)
	* @param aRequest server transaction
	*/
    void Send200OKL( CSIPServerTransaction& aRequest );
    
	/**
	* Sends 603 Decline or user response to request (INVITE)
	* @param aRequest server transaction
	* @param aReply user defined response (CMceMsgSIPReply)
	*/
    void SendDeclineL( CSIPServerTransaction& aRequest, 
                       CMceMsgBase& aReply );
    
	/**
	* Sends 180 Ringing to request (INVITE)
	* @param aRequest server transaction
	* @param aReply user defined response (CMceMsgSIPReply)
	* @return ETrue, if response was sent reliably
	*/
    TBool SendRingL( CSIPServerTransaction& aRequest,
                    CMceMsgBase& aReply );

    /**
	* Sends 182 SipQueued message to request (INVITE)
	* @param aRequest server transaction
	* @param aReply user defined response (CMceMsgSIPReply)
	* @return ETrue, if response was sent reliably
	*/

	
    TBool SendSipQueuedL( CSIPServerTransaction& aRequest,
                          CMceMsgBase& aReply );
	
                        
    /**
	* Sends a response to request (INVITE). 
	* For responses sent automatically by state machine, 
	* not for those coming from client.
	* @param aRequest server transaction
	* @param aCode the reason code
	* @param aReason the reasn phrase as string pool index
	*/
    void SendResponseL( CSIPServerTransaction& aRequest,
                        TUint32 aCode,
                        TInt aReason );

    /**
    * Seize re-sending UPDATE/PRACK. 
    * @return ETrue if interrupt succeeded.
    */                           
    TBool InterruptConfirming();
	
   /**
	* Receive Responses to the Extnesion request sent.
	* @param aResponse, response need to be sent to the client
	* @param aBody session body
	*/
    
    void ReceiveExtensionResponseL( CSIPClientTransaction& aResponse, 
    						CMceComSession& aBody );
    

	/**
	* Sends a response to the extension requests received
	* @param  aData SIP data 
	* @param aIds set of ids defining ITC call context
	*/
    void SendExtensionResponseL( CMceMsgBase& aData, TMceIds& aIds );
       
    
public://new functions

	/**
	* Called when update has been succeed
	*/
    void UpdateSucceed();
    
	/**
	* Called when update has been failed
	*/
    void UpdateFailed();
    
	/**
	* Encodes session body
	* @return aEncodedBody place holder to encode session body
	*/
    HBufC8* BodyToTextL();

	/**
	* Creates SDP document based on client/server transaction
	* @param aSDPContainer transaction 
	* containing SDP document in its content
	* @return system wide error / SDP error
	*/
    TInt CreateSDP( CSIPTransactionBase& aSDPContainer );
    
    
   /**
    * Process the ReliableResponse 
    * @return EFalse if response contains RSeq value that is not one higher than the last one.
    */
    TBool ProcessReliableResponse( const CSIPResponseElements& aResponseElements );
    
    
    /**
    * Handle Reliable Response 
    * @return EFalse if response contains RSeq value that is not one higher than the last one.
    */
	TBool TMceActionSet::NeedToProcessL( TMceStateTransitionEvent& aEvent );
    
    
    /**
    * Check if session response contact header has sips uri or tls parameters
    * @param aTransaction sip transaction msgs
    * @return void
    */
    void CheckContactIsSecureL( CSIPTransactionBase& aTransaction );
    
    
   /**
    * Finds if the Request is Extenssion Request
    * @return ETrue if the Request is Extension Request
    */

    TBool IsExtensionRequest( RStringF method );

    
public://Session timer related
	
	/**
	* Process session timer data in received INVITE
	* @param aRequest the request
	*/
	void ProcessSessionTimerServerL( CSIPServerTransaction& aRequest );

	/**
	* Process session timer data in received response
	* @param aResponse the response
	*/
	void ProcessSessionTimerClientL( CSIPClientTransaction& aResponse );
	
	/**
	* Resets session timer
	*/
    void ResetSessionTimerL();

    /**
	* Stop pending timer
	*/
    void StopPendingTimer();

	/**
	* Sends re-invite indicating session refresh
	*/
    void SendSessionRefreshL();
    
public://Media related

	/**
	* Handles ITC function, which controls media
	* @param aIds set of ids defining ITC call context
	* @param aAction ITC function defining the action
	* @return aReturnMessage message to return
	*/
    HBufC8* ControlMediaL( TMceIds& aIds, TMceItcFunctions aAction );

	/**
	* Handles ITC function, which controls media
	* @param aIds set of ids defining ITC call context
	* @param aAction ITC function defining the action
	* @param aMessage message
	*/
    void ControlMediaL( TMceIds& aIds, TMceItcFunctions aAction,
                        const TDesC8& aMessage );

	/**
	* Handles ITC function, which controls media
	* @param aIds set of ids defining ITC call context
	* @param aAction ITC function defining the action
	* @param aMessage message
	*/
    void ControlMediaL( TMceIds& aIds, TMceItcFunctions aAction,
                        TPtr8& aMessage );

    
	/**
	* Stops all media
	*/
    void StopMedia();
    
    /*
    * stop the ongoing updating media
    */
    void StopUpdatingMedia();


	/**
	* Informs client about new incoming session update
	* @param aInvite server transaction
	*/
    void ClientMediaStateChangedL( TMceMccComEvent* aMccEvent, 
                                  TMceMediaEventCode aEvent = EMceMediaEventReceived );
    

protected://methods


	/**
	* Converts SDP document to text
	* @param aDocument SDP document
	* @return SDP document as text
	*/
	HBufC8* CreateSDPContentLC( CSdpDocument& aDocument );
		
	/**
	* Creates multipart-mixed content
	* @param aDocument SDP document
	* @param aUserContentType user defined content type to be mixed in
	* @param aUserContent user content to be mixed in
	* @return multipart-mixed content
	*/
    HBufC8* CreateMultipartContentLC( CSdpDocument& aDocument,
        	                          const TDesC8& aUserContentType, 
        	                          const TDesC8& aUserContent );

	/**
	* Process session timer data in received INVITE
	* @param aElements message elements
	*/
    void ProcessSessionTimerServerL( const CSIPMessageElements& aElements );
    
	/**
	* Adds session timer headers, when session is acting as server, 
	* to response
	* @param aSipHeaders SIP headers container
	*/
	void AddSessionTimerFieldsServerL( 
	                                  RPointerArray<CSIPHeaderBase>& aHeaders );
	/**
	* Adds session timer headers, when session is acting as client, 
	* to request
	* @param aSipHeaders SIP headers container
	*/
	void AddSessionTimerFieldsClientL( 
	                            RPointerArray<CSIPHeaderBase>& aSipHeaders );

	/**
	* Sends response to request.
	* @param aRequest the request
	* @param aResponse the response from client
	* @return ETrue, if response was sent reliably
	*/
    TBool SendResponseL( CSIPServerTransaction& aRequest, 
                        CMceMsgSIPReply& aResponse );
                        
	/**
	* Sends warning response to request.
	* @param aResponseCode the response code
	* @param aWarnCode the warning code
	* @param aRequest the request
	*/
    void SendWarningResponseL( const TUint aResponseCode,
                               const TUint aWarnCode,
                               CSIPServerTransaction& aRequest );

	/**
	* Processes invite
	* @param aBody session body
	* @param aInvite the invite
	*/
    const CSIPMessageElements* ProcessInviteL( 
                                    CMceComSession& aBody,
                                    CSIPServerTransaction& aInvite );
                                               
	/**
	* Sends session body via callback mechanism to client (session)
	* @param aIds set of ids defining ITC callback context
	* @param aBody session body
	* @param aContent content of message
	*/
    void SendBodyToClientL( TMceIds& aIds,
                            CMceComSession& aBody, 
                            HBufC8* aContent );

    
	/**
	* Sends SIP data via callback mechanism to client (session)
	* @param aIds set of ids defining ITC callback context
	* @param aData SIP data
	* @param aContent content of message
	*/
    void SendSIPDataToClientL( TMceIds& aIds,
                               CMceMsgBase& aData, 
                               HBufC8* aContent );
                                

	/**
	* Fills client reply from SIP response
	* @param aReply client reply
	* @param aResponse SIP response
	* @return message elements
	*/
    const CSIPMessageElements* ResponseDataL( 
                                        CMceMsgSIPReply& aReply, 
                                        CSIPClientTransaction& aResponse,
                                        CMceComSession& aBody );
                                              
	/**
	* Fills session body from SIP request
	* @param aContainer session body
	* @param aRequest SIP request
	* @return message elements
	*/
    const CSIPMessageElements* RequestDataL( CMceComSession& aContainer, 
                                             CSIPServerTransaction& aRequest );
                                              
	/**
	* Fills client (request) data from SIP request
	* @param aContainer client (request) data
	* @param aRequest SIP request
	* @return message elements
	*/
    const CSIPMessageElements* RequestDataL( CMceMsgSIPData& aContainer, 
                                             CSIPServerTransaction& aRequest );

	/**
	* Adds SIP headers to SIP request/response. If no headers, adds
	* default headers
	* @param aMessageElements message elements of SIP request/response
	* @param aPrimaryHeaders headers
	* @param aMethodInd if request, indicates method, whose default headers
	* @param aContact if ETrue, the Contact header is created and added
	* will be added
	*/
    void AddHeadersL( CSIPMessageElements& aMessageElements,
                      CDesC8Array* aPrimaryHeaders,
                      TInt aMethodInd = KErrNotFound );

	/**
	* Adds SIP headers to SIP request/response. If no headers, adds
	* default headers
	* @param aHeaders SIP headers container
	* @param aPrimaryHeaders headers
	* @param aMethodInd if request, indicates method, whose default headers
	* @param aContact if ETrue, the Contact header is created and added
	* will be added
	*/
    void AddHeadersL( RPointerArray<CSIPHeaderBase>& aHeaders,
                      CDesC8Array* aPrimaryHeaders,
                      TInt aMethodInd = KErrNotFound );
    
    /**
    * Adds the SIP extension headers according to support of the extensions.
    * @param aHeaders SIP headers container
    * @param aTransactionType the transaction type as SIP string constant
    * @param aIsBody ETrue, if request/response will contain a body
    * @param aIsResponse ETrue, if the headers are for response 
    * @param aResponseCode the response code
    * @return ETrue if this yelds to a relibly sent provisional response.
    */
    TBool AddExtensionHeadersL( RPointerArray<CSIPHeaderBase>& aHeaders,
                                TInt aTransactionType,
                                TBool aIsBody,
                                TBool aIsResponse = EFalse,
                                TUint aResponseCode = KErrNone );

    /**
    * Sends specified request. Supports INVITE, PRACK and UPDATE.
    * @param the request method as SIP string
    * @param client data
	* @param aContent if ETrue, use content in request
    */
    void SendRequestL( TInt aMethod, CMceComSession* aData = NULL, 
                       TBool aContent = ETrue );
                           
    /**
    * Find active body for sip session.
    * @return active body
    */
    CMceComSession& ActiveBody();
    
    
private://data
    
    //SIP session instance
    CMceSipSession& iSession;
    
	//definitions for unit testing
	MCESRV_UT_DEFINITIONS

	};


/**
 * implementaion of event handler interface
 *
 */
class TMceComEventHandler : public  MMceEventHandler
    {
    
public:

     TMceComEventHandler( CMceMediaManager& aMediaManager );
    
     TMceComEventHandler( CMceMediaManager& aMediaManager,
                          TPtr8& aWriteData );

     TMceComEventHandler( CMceMediaManager& aMediaManager,
                          const TDesC8& aReadData );

     TMceComEventHandler( CMceMediaManager& aMediaManager,
                          HBufC8*& aWriteData );

    /**
     * Handles event for sink
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComMediaSink& aTarget, TMceComEvent& aEvent );
     
    /**
     * Handles event for source
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComMediaSource& aTarget, TMceComEvent& aEvent );
     
    /**
     * Handles event for rtp sink
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComRtpSink& aTarget, TMceComEvent& aEvent );
     
    /**
     * Handles event for rtp source
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComRtpSource& aTarget, TMceComEvent& aEvent );
     
     
    /**
     * Handles event for speaker sink
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComSpeakerSink& aTarget, TMceComEvent& aEvent );
     
    /**
     * Handles event for display sink
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComDisplaySink& aTarget, TMceComEvent& aEvent );
    
    /**
     * Handles event for mic source
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComMicSource& aTarget, TMceComEvent& aEvent );

    /**
     * Handles event for camera source
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComCameraSource& aTarget, TMceComEvent& aEvent );
    
    /**
     * Handles event for file source
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComFileSource& aTarget, TMceComEvent& aEvent );

    /**
     * Handles event for file sink
     * @param aTarget target
     * @param aEvent event
     */
     TInt HandleL( CMceComFileSink& aTarget, TMceComEvent& aEvent );

     
private://NOT owned data
    
     //media manager instance
     CMceMediaManager& iMediaManager;

     TPtr8* iWriteData;
     const TDesC8* iReadData;
     
     HBufC8** iReturnData;
     
     };


#endif //MCEACTIONSET_H


// End of File
