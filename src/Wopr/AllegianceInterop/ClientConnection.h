#pragma once

#include <msclr/marshal.h>

#include "NativeClient.h"
#include "ISupportManagedByteArrays.h"
#include "Enums.h"
//#include "igc.h"
#include "igcWrapper.h"
#include "clintlibWrapper.h"


// For testing
#include "igc.h"
#include "missionigc.h"
#include "mapmakerigc.h"


using namespace System;
using namespace System::Threading;
using namespace System::Collections::Generic;

using namespace msclr::interop;

namespace AllegianceInterop
{
	/*public ref class TestObject
	{
	public:
		String ^ var1;
		int number2 = 0;
	};*/

	// Sillyness to let OnAppMessageDelegate use ClientConnection as an argument.
	ref class ClientConnection;

	[Serializable]
	delegate void OnAppMessageEventDelegate(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
	delegate void OnLogonLobbyAckEventDelegate(bool fValidated, bool bRetry, LPCSTR szFailureReason);
	delegate void OnLogonAckEventDelegate(bool fValidated, bool bRetry, LPCSTR szFailureReason);

	//public delegate void OnTestObjectChanged(TestObject ^ testObject);



	[Serializable]
	public ref class ClientConnection
	{
	private:
		//int m_counter = 0;

		[NonSerialized]
		System::Runtime::InteropServices::GCHandle m_gchOnAppMessageEventDelegate;
		[NonSerialized]
		System::Runtime::InteropServices::GCHandle m_gchOnLogonLobbyAckEventDelegate;
		[NonSerialized]
		System::Runtime::InteropServices::GCHandle m_gchOnLogonAckEventDelegate;

		// Becuase the client can be serialized between two app domains, only the original client actually needs to clean up the connection
		// when it's original creator hits finalize. All of the serialized copies do not need to do the delete. By marking this flag
		// as non-serialized we will ensure that any copies of this object will not attempt to delete the unmanaged pointer to the native client that
		// doesn't belong to them. 
		[NonSerialized]
		bool m_deleteNativeClientOnFinalize = false;

		NativeClient * m_nativeClient;

		void SendMessage(ConnectionType connectionType, ISupportManagedByteArrays ^ message);

		//OnTestObjectChanged ^ m_onTestObjectChangedDelegate = nullptr;

		//TestObject ^ m_testObject = nullptr;

		//TestObject ^ m_testObjectReference = nullptr;

	public:

		void TestMapMaker()
		{
			const MissionParams * mp = m_nativeClient->GetCore()->GetMissionParams();
			/*CmissionIGC mission;
			
			mission.SetMissionParams(mp);
			mission.GenerateMission(Time::Now(), mp, nullptr, nullptr);*/

			CmapMakerHiHigherIGC mm;
			mm.Create(Time::Now(), mp, m_nativeClient->GetCore());
			
		}

		/*void TestNativeStuff()
		{
			NativeClient * c = &*(NativeClient *) nullptr;
		}*/

		/*void AddGameInfoChangedCallback(OnTestObjectChanged ^ onTestObjectChangedDelegate)
		{
			m_testObject = gcnew TestObject();

			m_onTestObjectChangedDelegate = onTestObjectChangedDelegate;
		}

		void UpdateTestObject(TestObject ^ testObject)
		{
			m_testObject->var1 = testObject->var1;
			m_testObject->number2 = testObject->number2;

		}

		void SetTestObjectNumber(int number)
		{
			m_testObject->number2 = number;
			m_onTestObjectChangedDelegate(m_testObject);
		}

		int GetTestObjectNumber()
		{
			return m_testObject->number2;
		}

		void SetTestObjectRef(TestObject ^ testObjectReference)
		{
			m_testObjectReference = testObjectReference;
		}

		int GetTestObjectRefNumber()
		{
			return m_testObjectReference->number2;
		}*/

		delegate void OnAppMessageDelegate(ClientConnection ^ clientConnection, array<Byte> ^ bytes);
		event OnAppMessageDelegate ^ OnAppMessage;

		ClientConnection(String ^ artpath);
		~ClientConnection();
		!ClientConnection();

		void OnAppMessageEvent(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
		void OnLogonLobbyAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason);
		void OnLogonAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason);

		bool ConnectToLobby(CancellationTokenSource ^ cancellationTokenSource, String ^ lobbyAddress, String ^ characterName, String ^ cdKey);
		bool ConnectToServer(/*CancellationTokenSource ^ cancellationTokenSource, */String ^ serverAddress, int port, String ^ characterName, String ^ cdKey, int cookie);

		void DisconnectLobby()
		{
			m_nativeClient->DisconnectLobby();
		}

		void DisconnectServer()
		{
			m_nativeClient->Disconnect();
		}

		void SendAndReceiveUpdate();

		void SendMessageLobby(ISupportManagedByteArrays ^ message);
		void SendMessageServer(ISupportManagedByteArrays ^ message);

		//generic <class T> 
		//	T GetNullOrObject(void * unmanagedValue)
		//{
		//	if (unmanagedValue == nullptr)
		//		return nullptr;

		//	//return ICreate<T>::Create();
		//	return gcnew T(unmanagedValue);
		//}

		System::Collections::Generic::List<MissionInfoWrapper ^> ^ GetMissionList();
		void JoinMission(MissionInfoWrapper ^ missionInfo, String ^ password);
		IshipIGCWrapper ^ GetShip() { if (m_nativeClient == nullptr) return nullptr; ConvertTo(IshipIGCWrapper, m_nativeClient->GetShip()) };
		IsideIGCWrapper ^ GetSide() { if (m_nativeClient == nullptr) return nullptr; ConvertTo(IsideIGCWrapper, m_nativeClient->GetSide()) };
		void AddMoneyToBucket(IbucketIGCWrapper ^ bucket, Money money) { m_nativeClient->AddMoneyToBucket(bucket->m_instance, money); };
		Money GetMoney(void) { return m_nativeClient->GetMoney(); };
		ImissionIGCWrapper ^ GetCore() { ConvertTo(ImissionIGCWrapper, m_nativeClient->GetCore()) };
		void BuyLoadout(IshipIGCWrapper ^ ship, bool launch) { m_nativeClient->BuyLoadout(ship->m_instance, launch); };
		void BuyDefaultLoadout(IshipIGCWrapper ^ ship, IstationIGCWrapper ^ station, IhullTypeIGCWrapper ^ hullType, Money budget) { m_nativeClient->BuyDefaultLoadout(ship->m_instance, station->m_instance, hullType->m_instance, &budget); };
		IshipIGCWrapper ^ CreateEmptyShip() { ConvertTo(IshipIGCWrapper, m_nativeClient->CreateEmptyShip()) }
		void BoardShip(IshipIGCWrapper ^ ship) { m_nativeClient->BoardShip(ship->m_instance); };

		short BuyPartOnBudget(IshipIGCWrapper ^       pship, IpartTypeIGCWrapper ^   ppt, Mount           mount, Money          pbudget) {
			return m_nativeClient->BuyPartOnBudget(pship->m_instance, ppt->m_instance, mount, &pbudget);
		}

		/*void FireExpendable(IshipIGCWrapper ^ pShip,
			IdispenserIGCWrapper ^ pDispenser,
			uint32 timeFired) { m_nativeClient->FireExpendable(pShip->m_instance, pDispenser->m_instance, timeFired); };*/

		void FireDispenser(IshipIGCWrapper ^ ship)
		{
			IdispenserIGC * dispenser = (IdispenserIGC *)ship->m_instance->GetMountedPart(ET_Dispenser, 0);
			m_nativeClient->FireExpendable(ship->m_instance, dispenser, Time::Now());
		}

		void FireChaff(IshipIGCWrapper ^ ship)
		{
			IdispenserIGC * dispenser = (IdispenserIGC *)ship->m_instance->GetMountedPart(ET_ChaffLauncher, 0);
			m_nativeClient->FireExpendable(ship->m_instance, dispenser, Time::Now());
		}

		void FireMissile(IshipIGCWrapper ^ ship, ImodelIGCWrapper ^ target, float lock)
		{
			ImagazineIGC * magazine = (ImagazineIGC *)ship->m_instance->GetMountedPart(ET_Magazine, 0);
			m_nativeClient->FireMissile(ship->m_instance, magazine, Time::Now(), target->m_instance, lock);
		}

		IbuoyIGCWrapper ^ CreateBuoy(BuoyType buoyType, float x, float y, float z, int clusterID, bool visible)
		{
			DataBuoyIGC buoy;

			ImissionIGC * mission = m_nativeClient->GetSide()->GetMission();
			buoy.buoyID = mission->GenerateNewBuoyID();
			buoy.position = Vector(x, y, z);
			buoy.clusterID = clusterID;
			buoy.type = buoyType;
			buoy.visible = visible;

			IbuoyIGCWrapper ^ buoyWrapper = gcnew IbuoyIGCWrapper((IbuoyIGC *)mission->CreateObject(Time::Now(), OT_buoy, &buoy, sizeof(buoy)));
			return buoyWrapper;
		}

		ImodelIGCWrapper ^  FindTarget(
			IshipIGCWrapper ^       pship,
			int						ttMask,
			AbilityBitMask			abmAbilities);

		float GetDistanceSquared(ImodelIGCWrapper ^ model1, ImodelIGCWrapper ^ model2) { return (model1->m_instance->GetPosition() - model2->m_instance->GetPosition()).LengthSquared(); }

		void EndLockDown(AllegianceInterop::LockdownCriteria lockdownCriteria) { m_nativeClient->EndLockDown((int)lockdownCriteria); }


		[System::Runtime::ExceptionServices::HandleProcessCorruptedStateExceptions]
		bool SendChat(
			IshipIGCWrapper ^							pshipSender,
			AllegianceInterop::ChatTarget				ctRecipient,
			ObjectID									oidRecipient,
			SoundID										soundID,
			String ^									pszText,
			CommandID									cid,
			ObjectType									otTarget,
			ObjectID									oidTarget,
			ImodelIGCWrapper ^							pmodelTarget,
			bool										bObjectModel)
		{
			try
			{
				marshal_context^ context = gcnew marshal_context();

				if (pshipSender == nullptr)
					return false;

				if (m_nativeClient == nullptr)
					return false;

				m_nativeClient->SendChat(
					pshipSender->m_instance,
					(::ChatTarget)ctRecipient,
					oidRecipient,
					soundID,
					context->marshal_as<const char*>(pszText),
					cid,
					otTarget,
					oidTarget,
					pmodelTarget ? pmodelTarget->m_instance : nullptr,
					bObjectModel);

				return true;
			}
			catch (System::Exception ^ ex)
			{
				System::Console::WriteLine("Exception in SendChat: " + ex->ToString());
				return false;
			}
			catch (...)
			{
				System::Console::WriteLine("Native exception in SendChat");
				return false;
			}
		}

		ShipID SideLeaderShipID(SideID sideID) { return m_nativeClient->MyMission()->SideLeaderShipID(sideID); }

		void DonateMoney(PlayerInfoWrapper ^ playerInfo, Money amount) {
			m_nativeClient->DonateMoney(playerInfo->m_instance, amount);
		}

	};
}

