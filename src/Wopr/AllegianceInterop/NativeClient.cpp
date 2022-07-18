#include "stdafx.h"

#include "NativeClient.h"
#include "NativeClientClusterSite.h"
#include "NativeClientThingSite.h"
//#include "AllMessages.h"
#include "MessageTypeNative.h"


CriticalSectionManager AllegianceInterop::NativeClient::m_criticalSection = CriticalSectionManager();

namespace AllegianceInterop
{
	int NativeClient::ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		debugf("NativeClient::ExceptionFilter()");
		if (code == EXCEPTION_ACCESS_VIOLATION)
		{
			debugf("\tcaught AV as expected.");
			return EXCEPTION_EXECUTE_HANDLER;
		}
		else
		{
			debugf("\tdidn't catch AV, unexpected.");
			return EXCEPTION_CONTINUE_SEARCH;
		};
	}

	HRESULT NativeClient::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
	{
		
		__try
		{
			//if(m_counter < 400)
			OnAppMessageEvent(pthis, cnxnFrom, pfm);
		}
		__except (ExceptionFilter(GetExceptionCode(), GetExceptionInformation()))
		{
			debugf("Exception Hit!");
		}

		//m_counter++;

		return S_OK;
	}

	void NativeClient::OnLogonLobbyAck(bool fValidated, bool bRetry, LPCSTR szFailureReason)
	{
		OnLogonLobbyAckEvent(fValidated, bRetry, szFailureReason);
	}

	void NativeClient::OnLogonAck(bool fValidated, bool bRetry, LPCSTR szFailureReason)
	{
		OnLogonAckEvent(fValidated, bRetry, szFailureReason);
	}

	NativeClient::NativeClient(const char * artpath)
	{
		// Intercept underlying exceptions so that the application can't crash out. 
		/*typedef void(*SignalHandlerPointer)(int);
		SignalHandlerPointer previousHandler;
		previousHandler = signal(SIGSEGV, SignalHandler);*/

		// TODO: Read this from the registry or config.
		UTL::SetArtPath(artpath);

		// Ensure that cluster info isn't deleted when the server sends us FM_S_DOCKED
		m_deleteShipInfoOnDockMessage = false;
		m_allowClientToReceiveClusterUpdatesForAllClusters = true;
	}

	//void NativeClient::HookOnAppMessageEvent(void * function)
	//void NativeClient::HookOnAppMessageEvent(HRESULT(*function)(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm))
	void NativeClient::HookOnAppMessageEvent(OnAppMessageEventFunction function)
	{
		m_OnAppMessageDelegate = function;

		__hook(&NativeClient::OnAppMessageEvent, this, &NativeClient::OnAppMessageEventHandler);
	}

	//void NativeClient::HookOnLogonLobbyAckEvent(void(*function)(bool fValidated, bool bRetry, LPCSTR szFailureReason))
	void NativeClient::HookOnLogonLobbyAckEvent(OnLogonLobbyAckEventFunction function)
	{
		m_OnLogonLobbyAckDelegate = function;

		__hook(&NativeClient::OnLogonLobbyAckEvent, this, &NativeClient::OnLogonLobbyAckEventHandler);
	}


	//void NativeClient::HookOnLogonAckEvent(void(*function)(bool fValidated, bool bRetry, LPCSTR szFailureReason))
	void NativeClient::HookOnLogonAckEvent(OnLogonAckEventFunction function)
	{
		m_OnLogonAckDelegate = function;

		__hook(&NativeClient::OnLogonAckEvent, this, &NativeClient::OnLogonAckEventHandler);
	}

	// From CPig::ReceiveSendAndUpdate()
	void NativeClient::SendAndReceiveUpdate()
	{
		m_lastUpdate = m_timeLastUpdate;
		m_now = Time::Now();

		// Call the base class to receive messages and perform periodic updates
		HRESULT hr = BaseClient::ReceiveMessages(); // OnAppMessage will process all messages

													// Send updates and messages
		if (SUCCEEDED(hr) && BaseClient::GetNetwork()->IsConnected())
		{
			m_now = Time::Now();

			//debugf("NativeClient::SendAndReceiveUpdate() m_lastUpdate: %ld, m_now: %ld\n", m_lastUpdate.clock(), m_now.clock());

				// Update the ship
			{
				if (BaseClient::GetShip())
				{
					
					BaseClient::SendUpdate(m_lastUpdate);
				}
			}

			// Check for server lag
			BaseClient::CheckServerLag(Time::Now());

			// Send messages if still connected
			if (BaseClient::GetNetwork()->IsConnected())
				BaseClient::SendMessages();

			// Update damage
			//if (BaseClient::GetCore()->GetLastUpdate() <= m_now)
			//{
				// Update IGC
				//this->GetCore()->Update(m_now);
				BaseClient::GetCore()->Update(m_now);
			/*}
			else
			{
				printf("WTF?  GetLastUpdate(): %ld, m_now: %ld, m_timeLastUpdate: %ld,  Time::Now(): %ld \n",
					BaseClient::GetCore()->GetLastUpdate(), m_now, m_timeLastUpdate, Time::Now());
			}*/
		}

		// Reset the last update time
		m_timeLastUpdate = m_now;
	}


	void NativeClient::OnAppMessageEventHandler(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
	{
		//Time m_now = Time::Now();
		
		try
		{
			// Let the underlying client perform message housekeeping. 
			// if the message will be modifying the static code, then ensure that only one thread is updating the static core at a time
			// it appears that something in the core variables is actually static, but I haven't found it yet.
			// Pigs handles this by ensuring that all messages are processed serially, but this will cause lag in the bots. 
			// Hopefully we can work around it like this.
			if (pfm->fmid == AllegianceInterop::MessageTypeNative::FM_S_JOINED_MISSION
				|| pfm->fmid == AllegianceInterop::MessageTypeNative::FM_S_EXPORT
				|| pfm->fmid == AllegianceInterop::MessageTypeNative::FM_S_BUY_LOADOUT_ACK)
			{
				m_criticalSection.Lock();
				BaseClient::HandleMsg(pfm, m_lastUpdate, m_now);
				m_criticalSection.UnLock();
			}
			else
			{
				BaseClient::HandleMsg(pfm, m_lastUpdate, m_now);
			}
		}
		catch (char * exceptionText)
		{
			debugf("\n\n*** OnAppMessageEventHandler Exception: %s\n\n", exceptionText);
		}
		
		
		
		if (m_OnAppMessageDelegate != nullptr)
		{
			(m_OnAppMessageDelegate)(pthis, cnxnFrom, pfm);
		}
		
		//m_lastUpdate = m_now;
	}
	void NativeClient::OnLogonLobbyAckEventHandler(bool fValidated, bool bRetry, LPCSTR szFailureReason)
	{
		if (m_OnLogonLobbyAckDelegate != nullptr)
		{
			m_OnLogonLobbyAckDelegate(fValidated, bRetry, szFailureReason);
		}
	}
	void NativeClient::OnLogonAckEventHandler(bool fValidated, bool bRetry, LPCSTR szFailureReason)
	{
		if (m_OnLogonAckDelegate != nullptr)
		{
			m_OnLogonAckDelegate(fValidated, bRetry, szFailureReason);
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// IIgcSite Overrides

	TRef<ThingSite> NativeClient::CreateThingSite(ImodelIGC* pModel)
	{
		return new NativeClientThingSite(this->GetSide());
	}

	TRef<ClusterSite> NativeClient::CreateClusterSite(IclusterIGC* pCluster)
	{
		return new NativeClientClusterSite(pCluster);
	}

	void NativeClient::ChangeStation(IshipIGC* pship, IstationIGC* pstationOld,
		IstationIGC* pstationNew)
	{
		if (pship == BaseClient::GetShip())
		{
			if (pstationNew)
			{
				// We just moved to a new station.  Raise any events to handle that here.
			}
			else if (pstationOld)
			{
				// Reset the AutoPilot flag
				BaseClient::SetAutoPilot(false);

				// Get the source ship and base hull type
				ZAssert(pstationOld);
				IshipIGC* pshipSource = BaseClient::GetShip()->GetSourceShip();
				ZAssert(pshipSource);
				const IhullTypeIGC* pht = pshipSource->GetBaseHullType();
				ZAssert(pht);

				//If no weapon is selected, try to select a weapon
				if (pshipSource && pshipSource->GetObjectID() == BaseClient::GetShip()->GetObjectID()) //imago 10/14
				{
					if (BaseClient::m_selectedWeapon >= pht->GetMaxFixedWeapons()
						|| !BaseClient::GetWeapon())
						BaseClient::NextWeapon();
				}

				// Reset the Vector Lock bit
				GetShip()->SetStateBits(coastButtonIGC, 0);

				// Reset the controls
				ControlData cd;
				cd.Reset();
				BaseClient::GetShip()->SetControls(cd);
			}
		}

		// Perform default processing
		BaseClient::ChangeStation(pship, pstationOld, pstationNew);
	}

	void NativeClient::ChangeCluster(IshipIGC*  pship, IclusterIGC* pclusterOld,
		IclusterIGC* pclusterNew)
	{
		if (pship && pship->GetObjectID() == BaseClient::GetShip()->GetObjectID())
		{
			if (pclusterOld && BaseClient::GetNetwork()->IsConnected())
				pclusterOld->SetActive(false);

			// Perform default processing
			BaseClient::ChangeCluster(pship, pclusterOld, pclusterNew);

			if (pclusterNew && BaseClient::GetNetwork()->IsConnected())
				pclusterNew->SetActive(true);
		}
	}

}
