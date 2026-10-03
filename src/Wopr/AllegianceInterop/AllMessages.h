
#pragma once
#include "ManagedObject.h"
#include "igcWrapper.h"

using namespace System;
using namespace System::Collections::Generic;

namespace AllegianceInterop
{
	ref class ClusterInfo;
	ref class WarpInfo;


    [Serializable] public ref class FMD_C_LOGONREQ : public ManagedObject<::FMD_C_LOGONREQ>
	{
	public:
		
        FMD_C_LOGONREQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_LOGONREQ(

                        String ^ CharacterName,
                        String ^ ZoneTicket,
                        String ^ CDKey,
                        String ^ MissionPassword,
                        USHORT fedsrvVer,
                        UINT32 time,
                        DWORD dwCookie,
                        int crcFileList,
                        String ^ steamAuthTicket /* Originally: int8_t steamAuthTicket[1024] */,
                        uint32_t steamAuthTicketLength,
                        uint64_t steamID,
                        String ^ drmHash /* Originally: char drmHash[50] */
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, LOGONREQ)
                FM_VAR_PARM(StringToCharArray(CharacterName), CB_ZTS)
                FM_VAR_PARM(StringToCharArray(ZoneTicket), CB_ZTS)
                FM_VAR_PARM(StringToCharArray(CDKey), CB_ZTS)
                FM_VAR_PARM(StringToCharArray(MissionPassword), CB_ZTS)
            END_PFM_CREATE

             pfm->fedsrvVer = fedsrvVer;
             pfm->time = time;
             pfm->dwCookie = dwCookie;
             pfm->crcFileList = crcFileList;
             strcpy((char *) pfm->steamAuthTicket, StringToCharArray(steamAuthTicket)); /* Originally: pfm->steamAuthTicket[1024] = steamAuthTicket[1024]; */
             pfm->steamAuthTicketLength = steamAuthTicketLength;
             pfm->steamID = steamID;
             strcpy(pfm->drmHash, StringToCharArray(drmHash)); /* Originally: pfm->drmHash[50] = drmHash[50]; */

			 this->m_Instance = pfm;
        }



        property String ^ CharacterName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, CharacterName)); 
			}
		}




        property String ^ ZoneTicket
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, ZoneTicket)); 
			}
		}




        property String ^ CDKey
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, CDKey)); 
			}
		}




        property String ^ MissionPassword
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, MissionPassword)); 
			}
		}



        property USHORT fedsrvVer
		{
		public:
			USHORT get()
			{
				return m_Instance->fedsrvVer; 
			}
		}

        property Time time
		{
		public:
			Time get()
			{
				return m_Instance->time; 
			}
		}

        property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie; 
			}
		}

        property int crcFileList
		{
		public:
			int get()
			{
				return m_Instance->crcFileList; 
			}
		}

        property String ^ steamAuthTicket /* Originally: int8_t steamAuthTicket[1024] */
		{
		public:
			String ^ get()
			{
				return gcnew String((char *) m_Instance->steamAuthTicket); 
			}
		}

        property uint32_t steamAuthTicketLength
		{
		public:
			uint32_t get()
			{
				return m_Instance->steamAuthTicketLength; 
			}
		}

        property uint64_t steamID
		{
		public:
			uint64_t get()
			{
				return m_Instance->steamID; 
			}
		}

        property String ^ drmHash /* Originally: char drmHash[50] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->drmHash); 
			}
		}

	};


    [Serializable] public ref class FMD_S_LOGONACK : public ManagedObject<::FMD_S_LOGONACK>
	{
	public:
		
        FMD_S_LOGONACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_LOGONACK(

                        String ^ CharName_OR_FailureReason,
                        Time timeServer,
                        ShipID shipID,
                        bool fValidated,
                        bool fRetry,
                        Cookie cookie
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LOGONACK)
                FM_VAR_PARM(StringToCharArray(CharName_OR_FailureReason), CB_ZTS)
            END_PFM_CREATE

             pfm->timeServer = timeServer;
             pfm->shipID = shipID;
             pfm->fValidated = fValidated;
             pfm->fRetry = fRetry;
             pfm->cookie = cookie;

			 this->m_Instance = pfm;
        }



        property String ^ CharName_OR_FailureReason
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, CharName_OR_FailureReason)); 
			}
		}



        property Time timeServer
		{
		public:
			Time get()
			{
				return m_Instance->timeServer; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property bool fValidated
		{
		public:
			bool get()
			{
				return m_Instance->fValidated; 
			}
		}

        property bool fRetry
		{
		public:
			bool get()
			{
				return m_Instance->fRetry; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_LOGOFF : public ManagedObject<::FMD_CS_LOGOFF>
	{
	public:
		
        FMD_CS_LOGOFF(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_LOGOFF(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, LOGOFF)
            END_PFM_CREATE

				this->m_Instance = pfm;
        }


	};


    [Serializable] public ref class FMD_C_BUY_LOADOUT : public ManagedObject<::FMD_C_BUY_LOADOUT>
	{
	public:
		
        FMD_C_BUY_LOADOUT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_BUY_LOADOUT(

                        String ^ loadout,
                        bool fLaunch
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, BUY_LOADOUT)
                FM_VAR_PARM(StringToCharArray(loadout), CB_ZTS)
            END_PFM_CREATE

             pfm->fLaunch = fLaunch;

			this->m_Instance = pfm;
        }



        property String ^ loadout
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, loadout)); 
			}
		}



        property bool fLaunch
		{
		public:
			bool get()
			{
				return m_Instance->fLaunch; 
			}
		}

	};


    [Serializable] public ref class FMD_S_BUY_LOADOUT_ACK : public ManagedObject<::FMD_S_BUY_LOADOUT_ACK>
	{
	public:
		
        FMD_S_BUY_LOADOUT_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_BUY_LOADOUT_ACK(

                        String ^ loadout,
                        bool fBoughtEverything,
                        bool fLaunch
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, BUY_LOADOUT_ACK)
                FM_VAR_PARM(StringToCharArray(loadout), CB_ZTS)
            END_PFM_CREATE

             pfm->fBoughtEverything = fBoughtEverything;
             pfm->fLaunch = fLaunch;

			 this->m_Instance = pfm;
        }



        property String ^ loadout
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, loadout)); 
			}
		}



        property bool fBoughtEverything
		{
		public:
			bool get()
			{
				return m_Instance->fBoughtEverything; 
			}
		}

        property bool fLaunch
		{
		public:
			bool get()
			{
				return m_Instance->fLaunch; 
			}
		}

	};


    [Serializable] public ref class FMD_S_OBJECT_SPOTTED : public ManagedObject<::FMD_S_OBJECT_SPOTTED>
	{
	public:
		
        FMD_S_OBJECT_SPOTTED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_OBJECT_SPOTTED(

                        ObjectType otObject,
                        ObjectID oidObject,
                        ObjectType otSpotter,
                        ObjectID oidSpotter
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, OBJECT_SPOTTED)
            END_PFM_CREATE

             pfm->otObject = otObject;
             pfm->oidObject = oidObject;
             pfm->otSpotter = otSpotter;
             pfm->oidSpotter = oidSpotter;

			 this->m_Instance = pfm;
        }


        property ObjectType otObject
		{
		public:
			ObjectType get()
			{
				return m_Instance->otObject; 
			}
		}

        property ObjectID oidObject
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidObject; 
			}
		}

        property ObjectType otSpotter
		{
		public:
			ObjectType get()
			{
				return m_Instance->otSpotter; 
			}
		}

        property ObjectID oidSpotter
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidSpotter; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SET_CLUSTER : public ManagedObject<::FMD_S_SET_CLUSTER>
	{
	public:
		
        FMD_S_SET_CLUSTER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SET_CLUSTER(

                        SectorID sectorID,
                        ServerSingleShipUpdate shipupdate,
                        Cookie cookie
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SET_CLUSTER)
            END_PFM_CREATE

             pfm->sectorID = sectorID;
             pfm->shipupdate = shipupdate;
             pfm->cookie = cookie;

			 this->m_Instance = pfm;
        }


        property SectorID sectorID
		{
		public:
			SectorID get()
			{
				return m_Instance->sectorID; 
			}
		}

        property ServerSingleShipUpdate shipupdate
		{
		public:
			ServerSingleShipUpdate get()
			{
				return m_Instance->shipupdate; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

	};


    [Serializable] public ref class FMD_S_DEV_COMPLETED : public ManagedObject<::FMD_S_DEV_COMPLETED>
	{
	public:
		
        FMD_S_DEV_COMPLETED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_DEV_COMPLETED(

                        DevelopmentID devId
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, DEV_COMPLETED)
            END_PFM_CREATE

             pfm->devId = devId;

			this->m_Instance = pfm;
        }


        property DevelopmentID devId
		{
		public:
			DevelopmentID get()
			{
				return m_Instance->devId; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SET_BRIEFING_TEXT : public ManagedObject<::FMD_S_SET_BRIEFING_TEXT>
	{
	public:
		
        FMD_S_SET_BRIEFING_TEXT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SET_BRIEFING_TEXT(

                        String ^ text,
                        bool fGenerateCivBriefing
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SET_BRIEFING_TEXT)
                FM_VAR_PARM(StringToCharArray(text), CB_ZTS)
            END_PFM_CREATE

             pfm->fGenerateCivBriefing = fGenerateCivBriefing;

			this->m_Instance = pfm;
        }



        property String ^ text
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, text)); 
			}
		}



        property bool fGenerateCivBriefing
		{
		public:
			bool get()
			{
				return m_Instance->fGenerateCivBriefing; 
			}
		}

	};


    [Serializable] public ref class FMD_S_PLAYERINFO : public ManagedObject<::FMD_S_PLAYERINFO>
	{
	public:
		
        FMD_S_PLAYERINFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PLAYERINFO(

						PersistPlayerScoreObject rgPersistPlayerScores,
                        //String ^ PersistPlayerScoreObject rgPersistPlayerScores /* Originally: // PersistPlayerScoreObject rgPersistPlayerScores[pfm->cbrgPersistPlayerScores / sizeof(PersistPlayerScoreObject)] = FM_VAR_REF(pfm, rgPersistPlayerScores) */,
                        String ^ CharacterName /* Originally: char CharacterName[c_cbName] */,
                        Money money,
                        bool fReady,
                        bool fTeamLeader,
                        bool fMissionOwner,
                        short nKills,
                        short nEjections,
                        short nDeaths,
                        float fExperience,
                        SideID iSide,
                        ShipID shipID,
                        RankID rankID,
                        //WingID wingID,
                        DroneTypeID dtidDrone,
                        PilotType pilotType,
                        AbilityBitMask abmOrders,
                        ObjectID baseObjectID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PLAYERINFO)
				//FM_VAR_PARM(StringToCharArray(rgPersistPlayerScores), CB_ZTS)
				END_PFM_CREATE

				//pfm->rgPersistPlayerScores = rgPersistPlayerScores; /* Originally: pfm->PersistPlayerScoreObject rgPersistPlayerScores[pfm->cbrgPersistPlayerScores / sizeof(PersistPlayerScoreObject)] = FM_VAR_REF(pfm, rgPersistPlayerScores) = PersistPlayerScoreObject rgPersistPlayerScores[pfm->cbrgPersistPlayerScores / sizeof(PersistPlayerScoreObject)] = FM_VAR_REF(pfm, rgPersistPlayerScores); */

				// TODO: Handle rgPersistPlayerScores objects.

             strcpy(pfm->CharacterName, StringToCharArray(CharacterName)); /* Originally: pfm->CharacterName[c_cbName] = CharacterName[c_cbName]; */
             pfm->money = money;
             pfm->fReady = fReady;
             pfm->fTeamLeader = fTeamLeader;
             pfm->fMissionOwner = fMissionOwner;
             pfm->nKills = nKills;
             pfm->nEjections = nEjections;
             pfm->nDeaths = nDeaths;
             pfm->fExperience = fExperience;
             pfm->iSide = iSide;
             pfm->shipID = shipID;
             pfm->rankID = rankID;
             //pfm->wingID = wingID;
             pfm->dtidDrone = dtidDrone;
             pfm->pilotType = pilotType;
             pfm->abmOrders = abmOrders;
             pfm->baseObjectID = baseObjectID;

			 this->m_Instance = pfm;
        }

		/*array<unsigned char> ^ ToManagedByteArray()
		{
			return 
		}*/

  //      property String ^ rgPersistPlayerScores
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(FM_VAR_REF(m_Instance, rgPersistPlayerScores)); 
		//	}
		//}


		// TODO: Handle PersistPlayerScoreObject objects.
  //      property  PersistPlayerScoreObject  rgPersistPlayerScores /* Originally: // PersistPlayerScoreObject rgPersistPlayerScores[pfm->cbrgPersistPlayerScores / sizeof(PersistPlayerScoreObject)] = FM_VAR_REF(pfm, rgPersistPlayerScores) */
		//{
		//public:
		//	PersistPlayerScoreObject get()
		//	{
		//		return m_Instance->rgPersistPlayerScores;
		//	}
		//}

        property String ^ CharacterName /* Originally: char CharacterName[c_cbName] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->CharacterName); 
			}
		}

        property Money money
		{
		public:
			Money get()
			{
				return m_Instance->money; 
			}
		}

        property bool fReady
		{
		public:
			bool get()
			{
				return m_Instance->fReady; 
			}
		}

        property bool fTeamLeader
		{
		public:
			bool get()
			{
				return m_Instance->fTeamLeader; 
			}
		}

        property bool fMissionOwner
		{
		public:
			bool get()
			{
				return m_Instance->fMissionOwner; 
			}
		}

        property short nKills
		{
		public:
			short get()
			{
				return m_Instance->nKills; 
			}
		}

        property short nEjections
		{
		public:
			short get()
			{
				return m_Instance->nEjections; 
			}
		}

        property short nDeaths
		{
		public:
			short get()
			{
				return m_Instance->nDeaths; 
			}
		}

        property float fExperience
		{
		public:
			float get()
			{
				return m_Instance->fExperience; 
			}
		}

        property SideID iSide
		{
		public:
			SideID get()
			{
				return m_Instance->iSide; 
			}

			void set(short iSide)
			{
				m_Instance->iSide = iSide;
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property RankID rankID
		{
		public:
			RankID get()
			{
				return m_Instance->rankID; 
			}
		}

  //      property //WingID wingID
		//{
		//public:
		//	//WingID get()
		//	{
		//		return m_Instance->wingID; 
		//	}
		//}

        property DroneTypeID dtidDrone
		{
		public:
			DroneTypeID get()
			{
				return m_Instance->dtidDrone; 
			}
		}

        property PilotType pilotType
		{
		public:
			PilotType get()
			{
				return m_Instance->pilotType; 
			}
		}

        property AbilityBitMask abmOrders
		{
		public:
			AbilityBitMask get()
			{
				return m_Instance->abmOrders; 
			}
		}

        property ObjectID baseObjectID
		{
		public:
			ObjectID get()
			{
				return m_Instance->baseObjectID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_STATIC_MAP_INFO : public ManagedObject<::FMD_S_STATIC_MAP_INFO>
	{
	public:
		
        FMD_S_STATIC_MAP_INFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_STATIC_MAP_INFO(

                        String ^ maps
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, STATIC_MAP_INFO)
                FM_VAR_PARM(StringToCharArray(maps), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ maps
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, maps)); 
			}
		}



	};


    [Serializable] public ref class FMD_CS_PROJECTILE_INSTANCE : public ManagedObject<::FMD_CS_PROJECTILE_INSTANCE>
	{
	public:
		
        FMD_CS_PROJECTILE_INSTANCE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_PROJECTILE_INSTANCE(

                        Time timeCreate,
                        DataProjectileIGC data,
                        Vector position
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, PROJECTILE_INSTANCE)
            END_PFM_CREATE

             pfm->timeCreate = timeCreate;
             pfm->data = data;
             pfm->position = position;

			 this->m_Instance = pfm;
        }


        property Time timeCreate
		{
		public:
			Time get()
			{
				return m_Instance->timeCreate; 
			}
		}

        property DataProjectileIGC data
		{
		public:
			DataProjectileIGC get()
			{
				return m_Instance->data; 
			}
		}

        property Vector position
		{
		public:
			Vector get()
			{
				return m_Instance->position; 
			}
		}

	};


    [Serializable] public ref class FMD_S_RANK_INFO : public ManagedObject<::FMD_S_RANK_INFO>
	{
	public:
		
        FMD_S_RANK_INFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_RANK_INFO(

                        String ^ ranks,
                        short cRanks
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, RANK_INFO)
                FM_VAR_PARM(StringToCharArray(ranks), CB_ZTS)
            END_PFM_CREATE

             pfm->cRanks = cRanks;

			this->m_Instance = pfm;
        }



        property String ^ ranks
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, ranks)); 
			}
		}



        property short cRanks
		{
		public:
			short get()
			{
				return m_Instance->cRanks; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SET_START_TIME : public ManagedObject<::FMD_S_SET_START_TIME>
	{
	public:
		
        FMD_S_SET_START_TIME(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SET_START_TIME(

                        Time timeStart
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SET_START_TIME)
            END_PFM_CREATE

             pfm->timeStart = timeStart;

			this->m_Instance = pfm;
        }


        property Time timeStart
		{
		public:
			Time get()
			{
				return m_Instance->timeStart; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SET_TEAM_INFO : public ManagedObject<::FMD_CS_SET_TEAM_INFO>
	{
	public:
		
        FMD_CS_SET_TEAM_INFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SET_TEAM_INFO(

                        SquadID squadID,
                        SideID sideID,
                        String ^ SideName /* Originally: char SideName[c_cbName] */
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SET_TEAM_INFO)
            END_PFM_CREATE

             pfm->squadID = squadID;
             pfm->sideID = sideID;
             strcpy(pfm->SideName, StringToCharArray(SideName)); /* Originally: pfm->SideName[c_cbName] = SideName[c_cbName]; */

			 this->m_Instance = pfm;
        }


        property SquadID squadID
		{
		public:
			SquadID get()
			{
				return m_Instance->squadID; 
			}
		}

        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property String ^ SideName /* Originally: char SideName[c_cbName] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->SideName); 
			}
		}

	};


    [Serializable] public ref class FMD_S_PLAYER_RESCUED : public ManagedObject<::FMD_S_PLAYER_RESCUED>
	{
	public:
		
        FMD_S_PLAYER_RESCUED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PLAYER_RESCUED(

                        ShipID shipIDRescuer,
                        ShipID shipIDRescuee
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PLAYER_RESCUED)
            END_PFM_CREATE

             pfm->shipIDRescuer = shipIDRescuer;
             pfm->shipIDRescuee = shipIDRescuee;

			 this->m_Instance = pfm;
        }


        property ShipID shipIDRescuer
		{
		public:
			ShipID get()
			{
				return m_Instance->shipIDRescuer; 
			}
		}

        property ShipID shipIDRescuee
		{
		public:
			ShipID get()
			{
				return m_Instance->shipIDRescuee; 
			}
		}

	};


    [Serializable] public ref class FMD_S_BALLOT : public ManagedObject<::FMD_S_BALLOT>
	{
	public:
		
        FMD_S_BALLOT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_BALLOT(

                        String ^ BallotText,
                        ObjectType otInitiator,
                        ObjectID oidInitiator,
                        BallotID ballotID,
                        Time timeExpiration,
                        bool bHideToLeader
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, BALLOT)
                FM_VAR_PARM(StringToCharArray(BallotText), CB_ZTS)
            END_PFM_CREATE

             pfm->otInitiator = otInitiator;
             pfm->oidInitiator = oidInitiator;
             pfm->ballotID = ballotID;
             pfm->timeExpiration = timeExpiration;
             pfm->bHideToLeader = bHideToLeader;

			 this->m_Instance = pfm;
        }



        property String ^ BallotText
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, BallotText)); 
			}
		}



        property ObjectType otInitiator
		{
		public:
			ObjectType get()
			{
				return m_Instance->otInitiator; 
			}
		}

        property ObjectID oidInitiator
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidInitiator; 
			}
		}

        property BallotID ballotID
		{
		public:
			BallotID get()
			{
				return m_Instance->ballotID; 
			}
		}

        property Time timeExpiration
		{
		public:
			Time get()
			{
				return m_Instance->timeExpiration; 
			}
		}

        property bool bHideToLeader
		{
		public:
			bool get()
			{
				return m_Instance->bHideToLeader; 
			}
		}

	};


    [Serializable] public ref class FMD_S_CANCEL_BALLOT : public ManagedObject<::FMD_S_CANCEL_BALLOT>
	{
	public:
		
        FMD_S_CANCEL_BALLOT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_CANCEL_BALLOT(

                        BallotID ballotID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, CANCEL_BALLOT)
            END_PFM_CREATE

             pfm->ballotID = ballotID;

			this->m_Instance = pfm;
        }


        property BallotID ballotID
		{
		public:
			BallotID get()
			{
				return m_Instance->ballotID; 
			}
		}

	};


    [Serializable] public ref class FMD_C_VOTE : public ManagedObject<::FMD_C_VOTE>
	{
	public:
		
        FMD_C_VOTE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_VOTE(

                        BallotID ballotID,
                        bool bAgree
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, VOTE)
            END_PFM_CREATE

             pfm->ballotID = ballotID;
             pfm->bAgree = bAgree;

			 this->m_Instance = pfm;
        }


        property BallotID ballotID
		{
		public:
			BallotID get()
			{
				return m_Instance->ballotID; 
			}
		}

        property bool bAgree
		{
		public:
			bool get()
			{
				return m_Instance->bAgree; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SHIP_DELETE : public ManagedObject<::FMD_S_SHIP_DELETE>
	{
	public:
		
        FMD_S_SHIP_DELETE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SHIP_DELETE(

                        ShipID shipID,
                        ShipDeleteReason sdr
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SHIP_DELETE)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->sdr = sdr;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property ShipDeleteReason sdr
		{
		public:
			ShipDeleteReason get()
			{
				return m_Instance->sdr; 
			}
		}

	};


    [Serializable] public ref class FMD_C_RANDOMIZE_TEAMS : public ManagedObject<::FMD_C_RANDOMIZE_TEAMS>
	{
	public:
		
        FMD_C_RANDOMIZE_TEAMS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_RANDOMIZE_TEAMS(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, RANDOMIZE_TEAMS)
            END_PFM_CREATE

				this->m_Instance = pfm;
        }


	};

	public ref class  ChatDataWrapper
	{
	private:
		ChatData * m_instance;

	public:
		ChatDataWrapper(ChatData * instance)
		{
			m_instance = instance;
		}

		property ShipID sidSender
		{
		public:
			ShipID get()
			{
				return m_instance->sidSender;
			}
		}

		property ChatTarget chatTarget
		{
		public:
			ChatTarget get()
			{
				return (ChatTarget) m_instance->chatTarget;
			}
		}

		property ObjectID oidRecipient
		{
		public:
			ObjectID get()
			{
				return m_instance->oidRecipient;
			}
		}
		
		property SoundID voiceOver
		{
		public:
			SoundID get()
			{
				return m_instance->voiceOver;
			}
		}

		property CommandID commandID
		{
		public:
			CommandID get()
			{
				return m_instance->commandID;
			}
		}

		property bool bObjectModel // was chat object model generated?
		{
		public:
			bool get()
			{
				return m_instance->bObjectModel;
			}
		}
	};


    [Serializable] public ref class FMD_CS_CHATMESSAGE : public ManagedObject<::FMD_CS_CHATMESSAGE>
	{
	public:
		
        FMD_CS_CHATMESSAGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_CHATMESSAGE(

                        String ^ Message,
                        ChatData cd,
                        ObjectID oidTarget,
                        ObjectType otTarget
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, CHATMESSAGE)
                FM_VAR_PARM(StringToCharArray(Message), CB_ZTS)
            END_PFM_CREATE

             pfm->cd = cd;
             pfm->oidTarget = oidTarget;
             pfm->otTarget = otTarget;

			 this->m_Instance = pfm;
        }



        property String ^ Message
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Message)); 
			}
		}



        property ChatDataWrapper ^ cd
		{
		public:
			ChatDataWrapper ^ get()
			{
				return gcnew ChatDataWrapper(&m_Instance->cd);
			}
		}

        property ObjectID oidTarget
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidTarget; 
			}
		}

        property ObjectType otTarget
		{
		public:
			ObjectType get()
			{
				return m_Instance->otTarget; 
			}
		}

	};


    [Serializable] public ref class FMD_C_SHIP_UPDATE : public ManagedObject<::FMD_C_SHIP_UPDATE>
	{
	public:
		
        FMD_C_SHIP_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_SHIP_UPDATE(

                        Time timeUpdate,
                        Cookie cookie,
                        ClientShipUpdate shipupdate
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, SHIP_UPDATE)
            END_PFM_CREATE

             pfm->timeUpdate = timeUpdate;
             pfm->cookie = cookie;
             pfm->shipupdate = shipupdate;

			 this->m_Instance = pfm;
        }


        property Time timeUpdate
		{
		public:
			Time get()
			{
				return m_Instance->timeUpdate; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

        property ClientShipUpdate shipupdate
		{
		public:
			ClientShipUpdate get()
			{
				return m_Instance->shipupdate; 
			}
		}

	};


    [Serializable] public ref class FMD_S_EXPORT : public ManagedObject<::FMD_S_EXPORT>
	{
	public:
		
        FMD_S_EXPORT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_EXPORT(

                        String ^ exportData,
                        ObjectType objecttype
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, EXPORT)
                FM_VAR_PARM(StringToCharArray(exportData), CB_ZTS)
            END_PFM_CREATE

             pfm->objecttype = objecttype;

			this->m_Instance = pfm;
        }



        property String ^ exportData
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, exportData)); 
			}
		}



        property ObjectType objecttype
		{
		public:
			ObjectType get()
			{
				return m_Instance->objecttype; 
			}
		}

	};


    [Serializable] public ref class FMD_S_POSTER : public ManagedObject<::FMD_S_POSTER>
	{
	public:
		
        FMD_S_POSTER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_POSTER(

                        String ^ textureName /* Originally: char textureName[c_cbFileName] */,
                        float latitude,
                        float longitude,
                        float radius,
                        SectorID sectorID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, POSTER)
            END_PFM_CREATE

             strcpy(pfm->textureName, StringToCharArray(textureName)); /* Originally: pfm->textureName[c_cbFileName] = textureName[c_cbFileName]; */
             pfm->latitude = latitude;
             pfm->longitude = longitude;
             pfm->radius = radius;
             pfm->sectorID = sectorID;

			 this->m_Instance = pfm;
        }


        property String ^ textureName /* Originally: char textureName[c_cbFileName] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->textureName); 
			}
		}

        property float latitude
		{
		public:
			float get()
			{
				return m_Instance->latitude; 
			}
		}

        property float longitude
		{
		public:
			float get()
			{
				return m_Instance->longitude; 
			}
		}

        property float radius
		{
		public:
			float get()
			{
				return m_Instance->radius; 
			}
		}

        property SectorID sectorID
		{
		public:
			SectorID get()
			{
				return m_Instance->sectorID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_TREASURE : public ManagedObject<::FMD_S_TREASURE>
	{
	public:
		
        FMD_S_TREASURE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_TREASURE(

                        DataTreasureIGC dataTreasure
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, TREASURE)
            END_PFM_CREATE

             pfm->dataTreasure = dataTreasure;

			this->m_Instance = pfm;
        }


        property DataTreasureIGC dataTreasure
		{
		public:
			DataTreasureIGC get()
			{
				return m_Instance->dataTreasure; 
			}
		}

	};


    [Serializable] public ref class FMD_S_DESTROY_TREASURE : public ManagedObject<::FMD_S_DESTROY_TREASURE>
	{
	public:
		
        FMD_S_DESTROY_TREASURE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_DESTROY_TREASURE(

                        TreasureID treasureID,
                        SectorID sectorID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, DESTROY_TREASURE)
            END_PFM_CREATE

             pfm->treasureID = treasureID;
             pfm->sectorID = sectorID;

			 this->m_Instance = pfm;
        }


        property TreasureID treasureID
		{
		public:
			TreasureID get()
			{
				return m_Instance->treasureID; 
			}
		}

        property SectorID sectorID
		{
		public:
			SectorID get()
			{
				return m_Instance->sectorID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_NEW_SHIP_INSTANCE : public ManagedObject<::FMD_CS_NEW_SHIP_INSTANCE>
	{
	public:
		
        FMD_CS_NEW_SHIP_INSTANCE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_NEW_SHIP_INSTANCE(

                        String ^ exportData
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, NEW_SHIP_INSTANCE)
                FM_VAR_PARM(StringToCharArray(exportData), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ exportData
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, exportData)); 
			}
		}



	};


    [Serializable] public ref class FMD_CS_DROP_PART : public ManagedObject<::FMD_CS_DROP_PART>
	{
	public:
		
        FMD_CS_DROP_PART(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_DROP_PART(

                        ShipID shipID,
                        EquipmentType et,
                        Mount mount
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, DROP_PART)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->et = et;
             pfm->mount = mount;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property EquipmentType et
		{
		public:
			EquipmentType get()
			{
				return m_Instance->et; 
			}
		}

        property Mount mount
		{
		public:
			Mount get()
			{
				return m_Instance->mount; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SWAP_PART : public ManagedObject<::FMD_CS_SWAP_PART>
	{
	public:
		
        FMD_CS_SWAP_PART(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SWAP_PART(

                        ShipID shipID,
                        EquipmentType etOld,
                        Mount mountOld,
                        Mount mountNew
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SWAP_PART)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->etOld = etOld;
             pfm->mountOld = mountOld;
             pfm->mountNew = mountNew;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property EquipmentType etOld
		{
		public:
			EquipmentType get()
			{
				return m_Instance->etOld; 
			}
		}

        property Mount mountOld
		{
		public:
			Mount get()
			{
				return m_Instance->mountOld; 
			}
		}

        property Mount mountNew
		{
		public:
			Mount get()
			{
				return m_Instance->mountNew; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_PING : public ManagedObject<::FMD_CS_PING>
	{
	public:
		
        FMD_CS_PING(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_PING(

                        int timeClient,
                        int timeServer,
                        FMGuaranteed fmg
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, PING)
            END_PFM_CREATE

             pfm->timeClient = timeClient;
             pfm->timeServer = timeServer;
             pfm->fmg = fmg;

			 this->m_Instance = pfm;
        }


        property Time timeClient
		{
		public:
			Time get()
			{
				return m_Instance->timeClient; 
			}
		}

        property Time timeServer
		{
		public:
			Time get()
			{
				return m_Instance->timeServer; 
			}
		}

        property FMGuaranteed fmg
		{
		public:
			FMGuaranteed get()
			{
				return m_Instance->fmg; 
			}
		}

	};


    [Serializable] public ref class FMD_S_URLROOT : public ManagedObject<::FMD_S_URLROOT>
	{
	public:
		
        FMD_S_URLROOT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_URLROOT(

                        String ^ UrlRoot
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, URLROOT)
                FM_VAR_PARM(StringToCharArray(UrlRoot), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ UrlRoot
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, UrlRoot)); 
			}
		}



	};


    [Serializable] public ref class FMD_S_MISSION_STAGE : public ManagedObject<::FMD_S_MISSION_STAGE>
	{
	public:
		
        FMD_S_MISSION_STAGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_MISSION_STAGE(

                        STAGE stage
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, MISSION_STAGE)
            END_PFM_CREATE

             pfm->stage = stage;

			this->m_Instance = pfm;
        }


        property STAGE stage
		{
		public:
			STAGE get()
			{
				return m_Instance->stage; 
			}
		}

	};


    [Serializable] public ref class FMD_S_MISSIONDEF : public ManagedObject<::FMD_S_MISSIONDEF>
	{
	public:
		
        FMD_S_MISSIONDEF(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_MISSIONDEF(

                        MissionParams misparms,
                        String ^ szDescription /* Originally: char szDescription[c_cbDescription] */,
                        DWORD dwCookie,
                        SideID iSideMissionOwner,
                        bool fAutoAcceptLeaders,
                        bool fInProgress,
                        STAGE stage,
                        String ^ szServerName /* Originally: // KGJV #114 - added server name & addr char      szServerName[c_cbName] */,
                        String ^ szServerAddr /* Originally: char szServerAddr[64] */,
                        String ^ rgszName         /* Originally: // $CRC: should probably just make a struct for the per team stuff and have an array of the struct char      rgszName        [c_cSidesMax][c_cbSideName] */,
                        String ^ rgCivID          /* Originally: //CivID rgCivID         [c_cSidesMax] */,
                        String ^ rgStationID      /* Originally: StationID rgStationID     [c_cSidesMax] */,
                        String ^ rgShipIDLeaders  /* Originally: ShipID rgShipIDLeaders [c_cSidesMax] */,
                        String ^ rgcPlayers       /* Originally: char rgcPlayers      [c_cSidesMax] */,
                        String ^ rgfAutoAccept    /* Originally: char rgfAutoAccept   [c_cSidesMax] */,
                        String ^ rgfReady         /* Originally: char rgfReady        [c_cSidesMax] */,
                        String ^ rgfForceReady    /* Originally: char rgfForceReady   [c_cSidesMax] */,
                        String ^ rgfActive        /* Originally: char rgfActive       [c_cSidesMax] */,
                        String ^ rgfAllies		 /* Originally: char rgfAllies		[c_cSidesMax] */
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, MISSIONDEF)
            END_PFM_CREATE

             pfm->misparms = misparms;
             strcpy(pfm->szDescription, StringToCharArray(szDescription)); /* Originally: pfm->szDescription[c_cbDescription] = szDescription[c_cbDescription]; */
             pfm->dwCookie = dwCookie;
             pfm->iSideMissionOwner = iSideMissionOwner;
             pfm->fAutoAcceptLeaders = fAutoAcceptLeaders;
             pfm->fInProgress = fInProgress;
             pfm->stage = stage;
             strcpy(pfm->szServerName, StringToCharArray(szServerName)); /* Originally: pfm->char      szServerName[c_cbName] = char      szServerName[c_cbName]; */
             strcpy(pfm->szServerAddr, StringToCharArray(szServerAddr)); /* Originally: pfm->szServerAddr[64] = szServerAddr[64]; */
             
			 //TODO: Handle complex array
			 //strcpy(pfm->rgszName        , StringToCharArray(rgszName        )); /* Originally: pfm->char      rgszName        [c_cSidesMax][c_cbSideName] = char      rgszName        [c_cSidesMax][c_cbSideName]; */
			 //strcpy(pfm->rgCivID         , StringToCharArray(rgCivID         )); /* Originally: pfm->rgCivID         [c_cSidesMax] = rgCivID         [c_cSidesMax]; */
			 //strcpy(pfm->rgStationID     , StringToCharArray(rgStationID     )); /* Originally: pfm->rgStationID     [c_cSidesMax] = rgStationID     [c_cSidesMax]; */
             //strcpy(pfm->rgShipIDLeaders , StringToCharArray(rgShipIDLeaders )); /* Originally: pfm->rgShipIDLeaders [c_cSidesMax] = rgShipIDLeaders [c_cSidesMax]; */
             strcpy(pfm->rgcPlayers      , StringToCharArray(rgcPlayers      )); /* Originally: pfm->rgcPlayers      [c_cSidesMax] = rgcPlayers      [c_cSidesMax]; */
             strcpy(pfm->rgfAutoAccept   , StringToCharArray(rgfAutoAccept   )); /* Originally: pfm->rgfAutoAccept   [c_cSidesMax] = rgfAutoAccept   [c_cSidesMax]; */
             strcpy(pfm->rgfReady        , StringToCharArray(rgfReady        )); /* Originally: pfm->rgfReady        [c_cSidesMax] = rgfReady        [c_cSidesMax]; */
             strcpy(pfm->rgfForceReady   , StringToCharArray(rgfForceReady   )); /* Originally: pfm->rgfForceReady   [c_cSidesMax] = rgfForceReady   [c_cSidesMax]; */
             strcpy(pfm->rgfActive       , StringToCharArray(rgfActive       )); /* Originally: pfm->rgfActive       [c_cSidesMax] = rgfActive       [c_cSidesMax]; */
             strcpy(pfm->rgfAllies		, StringToCharArray(rgfAllies		)); /* Originally: pfm->rgfAllies		[c_cSidesMax] = rgfAllies		[c_cSidesMax]; */

			 this->m_Instance = pfm;
        }


        property MissionParams misparms
		{
		public:
			MissionParams get()
			{
				return m_Instance->misparms; 
			}
		}

        property String ^ szDescription /* Originally: char szDescription[c_cbDescription] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->szDescription); 
			}
		}

        property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie; 
			}
		}

        property SideID iSideMissionOwner
		{
		public:
			SideID get()
			{
				return m_Instance->iSideMissionOwner; 
			}
		}

        property bool fAutoAcceptLeaders
		{
		public:
			bool get()
			{
				return m_Instance->fAutoAcceptLeaders; 
			}
		}

        property bool fInProgress
		{
		public:
			bool get()
			{
				return m_Instance->fInProgress; 
			}
		}

        property STAGE stage
		{
		public:
			STAGE get()
			{
				return m_Instance->stage; 
			}
		}

        property String ^ szServerName /* Originally: // KGJV #114 - added server name & addr char      szServerName[c_cbName] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->szServerName); 
			}
		}

        property String ^ szServerAddr /* Originally: char szServerAddr[64] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->szServerAddr); 
			}
		}

  //      property String ^ rgszName         /* Originally: // $CRC: should probably just make a struct for the per team stuff and have an array of the struct char      rgszName        [c_cSidesMax][c_cbSideName] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgszName        ); 
		//	}
		//}

  //      property String ^ rgCivID          /* Originally: //CivID rgCivID         [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgCivID         ); 
		//	}
		//}

  //      property String ^ rgStationID      /* Originally: StationID rgStationID     [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgStationID     ); 
		//	}
		//}

  //      property String ^ rgShipIDLeaders  /* Originally: ShipID rgShipIDLeaders [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgShipIDLeaders ); 
		//	}
		//}

  //      property String ^ rgcPlayers       /* Originally: char rgcPlayers      [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgcPlayers      ); 
		//	}
		//}

  //      property String ^ rgfAutoAccept    /* Originally: char rgfAutoAccept   [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgfAutoAccept   ); 
		//	}
		//}

  //      property String ^ rgfReady         /* Originally: char rgfReady        [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgfReady        ); 
		//	}
		//}

  //      property String ^ rgfForceReady    /* Originally: char rgfForceReady   [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgfForceReady   ); 
		//	}
		//}

  //      property String ^ rgfActive        /* Originally: char rgfActive       [c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgfActive       ); 
		//	}
		//}

  //      property String ^ rgfAllies		 /* Originally: char rgfAllies		[c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgfAllies		); 
		//	}
		//}

	};


    [Serializable] public ref class FMD_C_POSITIONREQ : public ManagedObject<::FMD_C_POSITIONREQ>
	{
	public:
		
        FMD_C_POSITIONREQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_POSITIONREQ(

                   SideID iSide     // Client can issue multiple of these SideID  iSide
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, POSITIONREQ)
            END_PFM_CREATE

             pfm->iSide = iSide;

			this->m_Instance = pfm;
        }


        property SideID  iSide // Client can issue multiple of these SideID  iSide
		{
		public:
			SideID get() // Client can issue multiple of these get()
			{
				return m_Instance->iSide; 
			}
		}

	};


    [Serializable] public ref class FMD_S_POSITIONREQ : public ManagedObject<::FMD_S_POSITIONREQ>
	{
	public:
		
        FMD_S_POSITIONREQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_POSITIONREQ(

                        ShipID shipID,
                        SideID iSide
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, POSITIONREQ)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->iSide = iSide;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property SideID iSide
		{
		public:
			SideID get()
			{
				return m_Instance->iSide; 
			}
		}

	};


    [Serializable] public ref class FMD_C_POSITIONACK : public ManagedObject<::FMD_C_POSITIONACK>
	{
	public:
		
        FMD_C_POSITIONACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_POSITIONACK(

						ShipID shipID,   // everyone gets either a PLAYER_SIDE_CHANGE or a DELPOSITIONREQ ShipID        shipID,
                        SideID iSide,
                        bool fAccepted
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, POSITIONACK)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->iSide = iSide;
             pfm->fAccepted = fAccepted;

			 this->m_Instance = pfm;
        }


        property ShipID shipID // everyone gets either a PLAYER_SIDE_CHANGE or a DELPOSITIONREQ ShipID        shipID
		{
		public:
			ShipID get() // everyone gets either a PLAYER_SIDE_CHANGE or a DELPOSITIONREQ get()
			{
				return m_Instance->shipID; 
			}
		}

        property SideID iSide
		{
		public:
			SideID get()
			{
				return m_Instance->iSide; 
			}
		}

        property bool fAccepted
		{
		public:
			bool get()
			{
				return m_Instance->fAccepted; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_DELPOSITIONREQ : public ManagedObject<::FMD_CS_DELPOSITIONREQ>
	{
	public:
		
        FMD_CS_DELPOSITIONREQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_DELPOSITIONREQ(

                        ShipID shipID,
                        SideID iSide,
                        DelPositionReqReason reason
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, DELPOSITIONREQ)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->iSide = iSide;
             pfm->reason = reason;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property SideID iSide
		{
		public:
			SideID get()
			{
				return m_Instance->iSide; 
			}
		}

        property DelPositionReqReason reason
		{
		public:
			DelPositionReqReason get()
			{
				return m_Instance->reason; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_LOCK_LOBBY : public ManagedObject<::FMD_CS_LOCK_LOBBY>
	{
	public:
		
        FMD_CS_LOCK_LOBBY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_LOCK_LOBBY(

                        bool fLock
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, LOCK_LOBBY)
            END_PFM_CREATE

             pfm->fLock = fLock;
			this->m_Instance = pfm;

        }


        property bool fLock
		{
		public:
			bool get()
			{
				return m_Instance->fLock; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_LOCK_SIDES : public ManagedObject<::FMD_CS_LOCK_SIDES>
	{
	public:
		
        FMD_CS_LOCK_SIDES(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_LOCK_SIDES(

                        bool fLock
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, LOCK_SIDES)
            END_PFM_CREATE

             pfm->fLock = fLock;

			this->m_Instance = pfm;
        }


        property bool fLock
		{
		public:
			bool get()
			{
				return m_Instance->fLock; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_PLAYER_READY : public ManagedObject<::FMD_CS_PLAYER_READY>
	{
	public:
		
        FMD_CS_PLAYER_READY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_PLAYER_READY(

						bool fReady, // Sent to everyone on team bool fReady,
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, PLAYER_READY)
            END_PFM_CREATE

             pfm->fReady = fReady;
             pfm->shipID = shipID;

			 this->m_Instance = pfm;
        }


        property bool fReady // Sent to everyone on team bool fReady
		{
		public:
			bool get() // Sent to everyone on team get()
			{
				return m_Instance->fReady; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_TEAM_READY : public ManagedObject<::FMD_S_TEAM_READY>
	{
	public:
		
        FMD_S_TEAM_READY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_TEAM_READY(

			SideID    iSide,// Server-generated based on whether everyone on team is ready, or leader has forced ready SideID    iSide,
                        bool fReady
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, TEAM_READY)
            END_PFM_CREATE

             pfm->iSide = iSide;
             pfm->fReady = fReady;
			 this->m_Instance = pfm;
        }


        property SideID iSide  // Server-generated based on whether everyone on team is ready, or leader has forced ready SideID    iSide
		{
		public:
			SideID get() // Server-generated based on whether everyone on team is ready, or leader has forced ready get()
			{
				return m_Instance->iSide; 
			}
		}

        property bool fReady
		{
		public:
			bool get()
			{
				return m_Instance->fReady; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_FORCE_TEAM_READY : public ManagedObject<::FMD_CS_FORCE_TEAM_READY>
	{
	public:
		
        FMD_CS_FORCE_TEAM_READY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_FORCE_TEAM_READY(

			SideID    iSide, // if fForceReady==true, causes TEAM_READY to also be sent SideID    iSide,
                        bool fForceReady
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, FORCE_TEAM_READY)
            END_PFM_CREATE

             pfm->iSide = iSide;
             pfm->fForceReady = fForceReady;
			 this->m_Instance = pfm;
        }


        property SideID    iSide // if fForceReady==true, causes TEAM_READY to also be sent SideID    iSide
		{
		public:
			SideID get() // if fForceReady==true, causes TEAM_READY to also be sent get()
			{
				return m_Instance->iSide; 
			}
		}

        property bool fForceReady
		{
		public:
			bool get()
			{
				return m_Instance->fForceReady; 
			}
		}

	};


    [Serializable] public ref class FMD_C_DOCKED : public ManagedObject<::FMD_C_DOCKED>
	{
	public:
		
        FMD_C_DOCKED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_DOCKED(

                        // This is only used for undocking StationID stationID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, DOCKED)
            END_PFM_CREATE

             pfm->stationID = stationID;
			this->m_Instance = pfm;
        }


        property StationID stationID // This is only used for undocking StationID stationID
		{
		public:
			StationID get() // This is only used for undocking get()
			{
				return m_Instance->stationID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_CHANGE_TEAM_CIV : public ManagedObject<::FMD_CS_CHANGE_TEAM_CIV>
	{
	public:
		
        FMD_CS_CHANGE_TEAM_CIV(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_CHANGE_TEAM_CIV(

                        SideID iSide,
                        CivID civID,
                        bool random
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, CHANGE_TEAM_CIV)
            END_PFM_CREATE

             pfm->iSide = iSide;
             pfm->civID = civID;
             pfm->random = random;

			 this->m_Instance = pfm;
        }


        property SideID iSide
		{
		public:
			SideID get()
			{
				return m_Instance->iSide; 
			}
		}

        property CivID civID
		{
		public:
			CivID get()
			{
				return m_Instance->civID; 
			}
		}

        property bool random
		{
		public:
			bool get()
			{
				return m_Instance->random; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_AUTO_ACCEPT : public ManagedObject<::FMD_CS_AUTO_ACCEPT>
	{
	public:
		
        FMD_CS_AUTO_ACCEPT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_AUTO_ACCEPT(

                        SideID iSide,
                        bool fAutoAccept
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, AUTO_ACCEPT)
            END_PFM_CREATE

             pfm->iSide = iSide;
             pfm->fAutoAccept = fAutoAccept;

			 this->m_Instance = pfm;
        }


        property SideID iSide
		{
		public:
			SideID get()
			{
				return m_Instance->iSide; 
			}
		}

        property bool fAutoAccept
		{
		public:
			bool get()
			{
				return m_Instance->fAutoAccept; 
			}
		}

	};


    [Serializable] public ref class FMD_S_STATIONS_UPDATE : public ManagedObject<::FMD_S_STATIONS_UPDATE>
	{
	public:
		
        FMD_S_STATIONS_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_STATIONS_UPDATE(

                        String ^ rgStationStates
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, STATIONS_UPDATE)
                FM_VAR_PARM(StringToCharArray(rgStationStates), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ rgStationStates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgStationStates)); 
			}
		}



	};


    [Serializable] public ref class FMD_S_STATION_CAPTURE : public ManagedObject<::FMD_S_STATION_CAPTURE>
	{
	public:
		
        FMD_S_STATION_CAPTURE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_STATION_CAPTURE(

                        StationID stationID,
                        SideID sidOld,
                        SideID sidNew,
                        ShipID shipIDCredit
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, STATION_CAPTURE)
            END_PFM_CREATE

             pfm->stationID = stationID;
             pfm->sidOld = sidOld;
             pfm->sidNew = sidNew;
             pfm->shipIDCredit = shipIDCredit;

			 this->m_Instance = pfm;
        }


        property StationID stationID
		{
		public:
			StationID get()
			{
				return m_Instance->stationID; 
			}
		}

        property SideID sidOld
		{
		public:
			SideID get()
			{
				return m_Instance->sidOld; 
			}
		}

        property SideID sidNew
		{
		public:
			SideID get()
			{
				return m_Instance->sidNew; 
			}
		}

        property ShipID shipIDCredit
		{
		public:
			ShipID get()
			{
				return m_Instance->shipIDCredit; 
			}
		}

	};


    [Serializable] public ref class FMD_S_STATION_DESTROYED : public ManagedObject<::FMD_S_STATION_DESTROYED>
	{
	public:
		
        FMD_S_STATION_DESTROYED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_STATION_DESTROYED(

                        StationID stationID,
                        ShipID launcher
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, STATION_DESTROYED)
            END_PFM_CREATE

             pfm->stationID = stationID;
             pfm->launcher = launcher;

			 this->m_Instance = pfm;
        }


        property StationID stationID
		{
		public:
			StationID get()
			{
				return m_Instance->stationID; 
			}
		}

        property ShipID launcher
		{
		public:
			ShipID get()
			{
				return m_Instance->launcher; 
			}
		}

	};


    [Serializable] public ref class FMD_S_MONEY_CHANGE : public ManagedObject<::FMD_S_MONEY_CHANGE>
	{
	public:
		
        FMD_S_MONEY_CHANGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_MONEY_CHANGE(

                        Money dMoney,
                        ShipID sidTo,
                        ShipID sidFrom
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, MONEY_CHANGE)
            END_PFM_CREATE

             pfm->dMoney = dMoney;
             pfm->sidTo = sidTo;
             pfm->sidFrom = sidFrom;

			 this->m_Instance = pfm;
        }


        property Money dMoney
		{
		public:
			Money get()
			{
				return m_Instance->dMoney; 
			}
		}

        property ShipID sidTo
		{
		public:
			ShipID get()
			{
				return m_Instance->sidTo; 
			}
		}

        property ShipID sidFrom
		{
		public:
			ShipID get()
			{
				return m_Instance->sidFrom; 
			}
		}

	};


    [Serializable] public ref class FMD_S_DOCKED : public ManagedObject<::FMD_S_DOCKED>
	{
	public:
		
        FMD_S_DOCKED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_DOCKED(

                        StationID stationID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, DOCKED)
            END_PFM_CREATE

             pfm->stationID = stationID;

			this->m_Instance = pfm;
        }


        property StationID stationID
		{
		public:
			StationID get()
			{
				return m_Instance->stationID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_GAME_OVER : public ManagedObject<::FMD_S_GAME_OVER>
	{
	public:
		
        FMD_S_GAME_OVER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_GAME_OVER(

                        String ^ szGameoverMessage,
                        //String ^ rgSides /* Originally: SideEndgameInfo rgSides[c_cSidesMax] */,
                        SideID iSideWinner,
                        char nNumSides,
                        bool bEjectPods
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, GAME_OVER)
                FM_VAR_PARM(StringToCharArray(szGameoverMessage), CB_ZTS)
            END_PFM_CREATE

				// TODO: Handle array
             //strcpy(pfm->rgSides, StringToCharArray(rgSides)); /* Originally: pfm->rgSides[c_cSidesMax] = rgSides[c_cSidesMax]; */
             pfm->iSideWinner = iSideWinner;
             pfm->nNumSides = nNumSides;
             pfm->bEjectPods = bEjectPods;

			 this->m_Instance = pfm;
        }



        property String ^ szGameoverMessage
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szGameoverMessage)); 
			}
		}



  //      property String ^ rgSides /* Originally: SideEndgameInfo rgSides[c_cSidesMax] */
		//{
		//public:
		//	String ^ get()
		//	{
		//		return gcnew String(m_Instance->rgSides); 
		//	}
		//}

        property SideID iSideWinner
		{
		public:
			SideID get()
			{
				return m_Instance->iSideWinner; 
			}
		}

        property char nNumSides
		{
		public:
			char get()
			{
				return m_Instance->nNumSides; 
			}
		}

        property bool bEjectPods
		{
		public:
			bool get()
			{
				return m_Instance->bEjectPods; 
			}
		}

	};


    [Serializable] public ref class FMD_S_GAME_OVER_PLAYERS : public ManagedObject<::FMD_S_GAME_OVER_PLAYERS>
	{
	public:
		
        FMD_S_GAME_OVER_PLAYERS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_GAME_OVER_PLAYERS(

                        String ^ rgPlayerInfo
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, GAME_OVER_PLAYERS)
                FM_VAR_PARM(StringToCharArray(rgPlayerInfo), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ rgPlayerInfo
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgPlayerInfo)); 
			}
		}



	};


    [Serializable] public ref class FMD_C_BUCKET_DONATE : public ManagedObject<::FMD_C_BUCKET_DONATE>
	{
	public:
		
        FMD_C_BUCKET_DONATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_BUCKET_DONATE(

                        Money moneyGiven,
                        BucketID iBucket
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, BUCKET_DONATE)
            END_PFM_CREATE

             pfm->moneyGiven = moneyGiven;
             pfm->iBucket = iBucket;

			 this->m_Instance = pfm;
        }


        property Money moneyGiven
		{
		public:
			Money get()
			{
				return m_Instance->moneyGiven; 
			}
		}

        property BucketID iBucket
		{
		public:
			BucketID get()
			{
				return m_Instance->iBucket; 
			}
		}

	};


    [Serializable] public ref class FMD_S_BUCKET_STATUS : public ManagedObject<::FMD_S_BUCKET_STATUS>
	{
	public:
		
        FMD_S_BUCKET_STATUS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_BUCKET_STATUS(

                        DWORD timeTotal,
                        SideID sideID,
                        Money moneyTotal,
                        BucketID iBucket
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, BUCKET_STATUS)
            END_PFM_CREATE

             pfm->timeTotal = timeTotal;
             pfm->sideID = sideID;
             pfm->moneyTotal = moneyTotal;
             pfm->iBucket = iBucket;


			 this->m_Instance = pfm;
        }


        property DWORD timeTotal
		{
		public:
			DWORD get()
			{
				return m_Instance->timeTotal; 
			}
		}

        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property Money moneyTotal
		{
		public:
			Money get()
			{
				return m_Instance->moneyTotal; 
			}
		}

        property BucketID iBucket
		{
		public:
			BucketID get()
			{
				return m_Instance->iBucket; 
			}
		}

	};


    [Serializable] public ref class FMD_C_PLAYER_DONATE : public ManagedObject<::FMD_C_PLAYER_DONATE>
	{
	public:
		
        FMD_C_PLAYER_DONATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_PLAYER_DONATE(

                        Money moneyGiven,
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, PLAYER_DONATE)
            END_PFM_CREATE

             pfm->moneyGiven = moneyGiven;
             pfm->shipID = shipID;

			 this->m_Instance = pfm;
        }


        property Money moneyGiven
		{
		public:
			Money get()
			{
				return m_Instance->moneyGiven; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SIDE_INACTIVE : public ManagedObject<::FMD_CS_SIDE_INACTIVE>
	{
	public:
		
        FMD_CS_SIDE_INACTIVE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SIDE_INACTIVE(

                        SideID sideID,
                        bool bActive,
                        bool bChangeAET,
                        bool bAET
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SIDE_INACTIVE)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->bActive = bActive;
             pfm->bChangeAET = bChangeAET;
             pfm->bAET = bAET;

			 this->m_Instance = pfm;
        }


        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property bool bActive
		{
		public:
			bool get()
			{
				return m_Instance->bActive; 
			}
		}

        property bool bChangeAET
		{
		public:
			bool get()
			{
				return m_Instance->bChangeAET; 
			}
		}

        property bool bAET
		{
		public:
			bool get()
			{
				return m_Instance->bAET; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_FIRE_MISSILE : public ManagedObject<::FMD_CS_FIRE_MISSILE>
	{
	public:
		
        FMD_CS_FIRE_MISSILE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_FIRE_MISSILE(

                        String ^ missileLaunchData,
                        float lock,
                        Time timeFired,
                        ExpendableTypeID missiletypeID,
                        ShipID launcherID,
                        SectorID clusterID,
                        ObjectType targetType,
                        ObjectID targetID,
                        bool bDud
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, FIRE_MISSILE)
                FM_VAR_PARM(StringToCharArray(missileLaunchData), CB_ZTS)
            END_PFM_CREATE

             pfm->lock = lock;
             pfm->timeFired = timeFired;
             pfm->missiletypeID = missiletypeID;
             pfm->launcherID = launcherID;
             pfm->clusterID = clusterID;
             pfm->targetType = targetType;
             pfm->targetID = targetID;
             pfm->bDud = bDud;

			 this->m_Instance = pfm;
        }



        property String ^ missileLaunchData
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, missileLaunchData)); 
			}
		}



        property float lock
		{
		public:
			float get()
			{
				return m_Instance->lock; 
			}
		}

        property Time timeFired
		{
		public:
			Time get()
			{
				return m_Instance->timeFired; 
			}
		}

        property ExpendableTypeID missiletypeID
		{
		public:
			ExpendableTypeID get()
			{
				return m_Instance->missiletypeID; 
			}
		}

        property ShipID launcherID
		{
		public:
			ShipID get()
			{
				return m_Instance->launcherID; 
			}
		}

        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property ObjectType targetType
		{
		public:
			ObjectType get()
			{
				return m_Instance->targetType; 
			}
		}

        property ObjectID targetID
		{
		public:
			ObjectID get()
			{
				return m_Instance->targetID; 
			}
		}

        property bool bDud
		{
		public:
			bool get()
			{
				return m_Instance->bDud; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SIDE_TECH_CHANGE : public ManagedObject<::FMD_S_SIDE_TECH_CHANGE>
	{
	public:
		
        FMD_S_SIDE_TECH_CHANGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SIDE_TECH_CHANGE(

                        SideID sideID,
                        TechTreeBitMask ttbmDevelopments
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SIDE_TECH_CHANGE)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->ttbmDevelopments = ttbmDevelopments;

			 this->m_Instance = pfm;
        }


        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property TechTreeBitMask ttbmDevelopments
		{
		public:
			TechTreeBitMask get()
			{
				return m_Instance->ttbmDevelopments; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SIDE_ATTRIBUTE_CHANGE : public ManagedObject<::FMD_S_SIDE_ATTRIBUTE_CHANGE>
	{
	public:
		
        FMD_S_SIDE_ATTRIBUTE_CHANGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SIDE_ATTRIBUTE_CHANGE(

                        SideID sideID,
                        GlobalAttributeSet gasAttributes
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SIDE_ATTRIBUTE_CHANGE)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->gasAttributes = gasAttributes;

			 this->m_Instance = pfm;
        }


        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property GlobalAttributeSet gasAttributes
		{
		public:
			GlobalAttributeSet get()
			{
				return m_Instance->gasAttributes; 
			}
		}

	};


    [Serializable] public ref class FMD_S_EJECT : public ManagedObject<::FMD_S_EJECT>
	{
	public:
		
        FMD_S_EJECT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_EJECT(

                        ShipID shipID,
                        Vector position,
                        Vector velocity,
                        Vector forward,
                        Cookie cookie
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, EJECT)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->position = position;
             pfm->velocity = velocity;
             pfm->forward = forward;
             pfm->cookie = cookie;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property Vector position
		{
		public:
			Vector get()
			{
				return m_Instance->position; 
			}
		}

        property Vector velocity
		{
		public:
			Vector get()
			{
				return m_Instance->velocity; 
			}
		}

        property Vector forward
		{
		public:
			Vector get()
			{
				return m_Instance->forward; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SINGLE_SHIP_UPDATE : public ManagedObject<::FMD_S_SINGLE_SHIP_UPDATE>
	{
	public:
		
        FMD_S_SINGLE_SHIP_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SINGLE_SHIP_UPDATE(

                        ObjectType otTarget,
                        ObjectID oidTarget,
                        ObjectType otAccepted,
                        ObjectID oidAccepted,
                        CommandID cidAccepted,
                        ObjectType otPlan,
                        ObjectID oidPlan,
                        CommandID cidPlan,
                        ObjectID oidWaypointWarp,
                        ServerSingleShipUpdate shipupdate,
                        bool bIsRipcording,
                        ObjectType otRipcord,
                        ObjectID oidRipcord
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SINGLE_SHIP_UPDATE)
            END_PFM_CREATE

             pfm->otTarget = otTarget;
             pfm->oidTarget = oidTarget;
             pfm->otAccepted = otAccepted;
             pfm->oidAccepted = oidAccepted;
             pfm->cidAccepted = cidAccepted;
             pfm->otPlan = otPlan;
             pfm->oidPlan = oidPlan;
             pfm->cidPlan = cidPlan;
             pfm->oidWaypointWarp = oidWaypointWarp;
             pfm->shipupdate = shipupdate;
             pfm->bIsRipcording = bIsRipcording;
             pfm->otRipcord = otRipcord;
             pfm->oidRipcord = oidRipcord;

			 this->m_Instance = pfm;
        }


        property ObjectType otTarget
		{
		public:
			ObjectType get()
			{
				return m_Instance->otTarget; 
			}
		}

        property ObjectID oidTarget
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidTarget; 
			}
		}

        property ObjectType otAccepted
		{
		public:
			ObjectType get()
			{
				return m_Instance->otAccepted; 
			}
		}

        property ObjectID oidAccepted
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidAccepted; 
			}
		}

        property CommandID cidAccepted
		{
		public:
			CommandID get()
			{
				return m_Instance->cidAccepted; 
			}
		}

        property ObjectType otPlan
		{
		public:
			ObjectType get()
			{
				return m_Instance->otPlan; 
			}
		}

        property ObjectID oidPlan
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidPlan; 
			}
		}

        property CommandID cidPlan
		{
		public:
			CommandID get()
			{
				return m_Instance->cidPlan; 
			}
		}

        property ObjectID oidWaypointWarp
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidWaypointWarp; 
			}
		}

        property ServerSingleShipUpdate shipupdate
		{
		public:
			ServerSingleShipUpdate get()
			{
				return m_Instance->shipupdate; 
			}
		}

        property bool bIsRipcording
		{
		public:
			bool get()
			{
				return m_Instance->bIsRipcording; 
			}
		}

        property ObjectType otRipcord
		{
		public:
			ObjectType get()
			{
				return m_Instance->otRipcord; 
			}
		}

        property ObjectID oidRipcord
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidRipcord; 
			}
		}

	};


    [Serializable] public ref class FMD_C_SUICIDE : public ManagedObject<::FMD_C_SUICIDE>
	{
	public:
		
        FMD_C_SUICIDE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_SUICIDE(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, SUICIDE)
            END_PFM_CREATE

				this->m_Instance = pfm;
        }


	};


    [Serializable] public ref class FMD_C_FIRE_EXPENDABLE : public ManagedObject<::FMD_C_FIRE_EXPENDABLE>
	{
	public:
		
        FMD_C_FIRE_EXPENDABLE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_FIRE_EXPENDABLE(

                        EquipmentType et
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, FIRE_EXPENDABLE)
            END_PFM_CREATE

             pfm->et = et;

			this->m_Instance = pfm;
        }


        property EquipmentType et
		{
		public:
			EquipmentType get()
			{
				return m_Instance->et; 
			}
		}

	};


    [Serializable] public ref class FMD_S_MISSILE_DESTROYED : public ManagedObject<::FMD_S_MISSILE_DESTROYED>
	{
	public:
		
        FMD_S_MISSILE_DESTROYED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_MISSILE_DESTROYED(

                        Vector position,
                        SectorID clusterID,
                        MissileID missileID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, MISSILE_DESTROYED)
            END_PFM_CREATE

             pfm->position = position;
             pfm->clusterID = clusterID;
             pfm->missileID = missileID;

			 this->m_Instance = pfm;
        }


        property Vector position
		{
		public:
			Vector get()
			{
				return m_Instance->position; 
			}
		}

        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property MissileID missileID
		{
		public:
			MissileID get()
			{
				return m_Instance->missileID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_MINE_DESTROYED : public ManagedObject<::FMD_S_MINE_DESTROYED>
	{
	public:
		
        FMD_S_MINE_DESTROYED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_MINE_DESTROYED(

                        SectorID clusterID,
                        MineID mineID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, MINE_DESTROYED)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->mineID = mineID;

			 this->m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property MineID mineID
		{
		public:
			MineID get()
			{
				return m_Instance->mineID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_PROBE_DESTROYED : public ManagedObject<::FMD_S_PROBE_DESTROYED>
	{
	public:
		
        FMD_S_PROBE_DESTROYED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PROBE_DESTROYED(

                        SectorID clusterID,
                        ProbeID probeID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PROBE_DESTROYED)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->probeID = probeID;

			 this->m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property ProbeID probeID
		{
		public:
			ProbeID get()
			{
				return m_Instance->probeID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_ASTEROID_DESTROYED : public ManagedObject<::FMD_S_ASTEROID_DESTROYED>
	{
	public:
		
        FMD_S_ASTEROID_DESTROYED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ASTEROID_DESTROYED(

                        SectorID clusterID,
                        AsteroidID asteroidID,
                        bool explodeF
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ASTEROID_DESTROYED)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->asteroidID = asteroidID;
             pfm->explodeF = explodeF;

			 this->m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property AsteroidID asteroidID
		{
		public:
			AsteroidID get()
			{
				return m_Instance->asteroidID; 
			}
		}

        property bool explodeF
		{
		public:
			bool get()
			{
				return m_Instance->explodeF; 
			}
		}

	};


    [Serializable] public ref class FMD_S_FIRE_EXPENDABLE : public ManagedObject<::FMD_S_FIRE_EXPENDABLE>
	{
	public:
		
        FMD_S_FIRE_EXPENDABLE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_FIRE_EXPENDABLE(

                        ShipID launcherID,
                        EquipmentType equipmentType
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, FIRE_EXPENDABLE)
            END_PFM_CREATE

             pfm->launcherID = launcherID;
             pfm->equipmentType = equipmentType;

			 this->m_Instance = pfm;
        }


        property ShipID launcherID
		{
		public:
			ShipID get()
			{
				return m_Instance->launcherID; 
			}
		}

        property EquipmentType equipmentType
		{
		public:
			EquipmentType get()
			{
				return m_Instance->equipmentType; 
			}
		}

	};


    [Serializable] public ref class FMD_S_TREASURE_SETS : public ManagedObject<::FMD_S_TREASURE_SETS>
	{
	public:
		
        FMD_S_TREASURE_SETS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_TREASURE_SETS(

                        String ^ treasureSet,
                        short cTreasureSets
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, TREASURE_SETS)
                FM_VAR_PARM(StringToCharArray(treasureSet), CB_ZTS)
            END_PFM_CREATE

             pfm->cTreasureSets = cTreasureSets;

			this->m_Instance = pfm;
        }



        property String ^ treasureSet
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, treasureSet)); 
			}
		}

        property short cTreasureSets
		{
		public:
			short get()
			{
				return m_Instance->cTreasureSets; 
			}
		}

	};

	/*public ref class ShipStatusWrapper
	{
	public:
		ShipStatus * m_instance;

		ShipStatusWrapper(ShipStatus * shipStatus)
		{
			m_instance = shipStatus;
		}

		HullID      GetHullID(void) 
		{
			return m_instance->GetHullID();
		}
		void        SetHullID(HullID    newVal)
		{
			m_instance->SetHullID(newVal);
		}

		SectorID    GetSectorID(void) 
		{
			return m_instance->GetSectorID();
		}
		void        SetSectorID(SectorID    newVal)
		{
			m_instance->SetSectorID(newVal);
		}

		StationID   GetStationID(void) 
		{
			return m_instance->GetStationID();
		}
		void        SetStationID(StationID stationID)
		{
			m_instance->SetStationID(stationID);
		}

		StationID   GetParentID(void)
		{
			return m_instance->GetParentID();
		}
		void        SetParentID(ShipID shipID)
		{
			m_instance->SetParentID(shipID);
		}

		ShipState   GetState(void) 
		{
			return m_instance->GetState();
		}
		void        SetState(ShipState    newVal)
		{
			m_instance->SetState(newVal);
		}

		bool        GetUnknown(void) 
		{
			return m_instance->GetUnknown();
		}
		void        SetUnknown(void)
		{
			m_instance->SetUnknown();
		}

		bool        GetDetected(void)
		{
			return m_instance->GetDetected();
		}
		void        SetDetected(bool    bDetected)
		{
			m_instance->SetDetected(bDetected);
		}
		DWORD        GetStateTime(void) 
		{
			return m_instance->GetStateTime();
		}
		void        SetStateTime(DWORD    dTime)
		{
			m_instance->SetStateTime(dTime);
		}
	};*/

    [Serializable] public ref class FMD_S_SHIP_STATUS : public ManagedObject<::FMD_S_SHIP_STATUS>
	{
	public:
		
        FMD_S_SHIP_STATUS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SHIP_STATUS(

                        ShipID shipID,
                        ShipStatus status
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SHIP_STATUS)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->status = status;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property ShipStatusWrapper ^ status
		{
		public:
			ShipStatusWrapper ^ get()
			{
				if (m_Instance == nullptr)
					return nullptr;

				return gcnew ShipStatusWrapper(&m_Instance->status);
			}
		}

	};


    [Serializable] public ref class FMD_S_PROBES_UPDATE : public ManagedObject<::FMD_S_PROBES_UPDATE>
	{
	public:
		
        FMD_S_PROBES_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PROBES_UPDATE(

                        String ^ rgProbeStates
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PROBES_UPDATE)
                FM_VAR_PARM(StringToCharArray(rgProbeStates), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ rgProbeStates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgProbeStates)); 
			}
		}



	};


    [Serializable] public ref class FMD_CS_ORDER_CHANGE : public ManagedObject<::FMD_CS_ORDER_CHANGE>
	{
	public:
		
        FMD_CS_ORDER_CHANGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_ORDER_CHANGE(

                        ShipID shipID,
                        ObjectID objectID,
                        ObjectType objectType,
                        CommandID commandID,
                        Command command
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, ORDER_CHANGE)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->objectID = objectID;
             pfm->objectType = objectType;
             pfm->commandID = commandID;
             pfm->command = command;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property ObjectID objectID
		{
		public:
			ObjectID get()
			{
				return m_Instance->objectID; 
			}
		}

        property ObjectType objectType
		{
		public:
			ObjectType get()
			{
				return m_Instance->objectType; 
			}
		}

        property CommandID commandID
		{
		public:
			CommandID get()
			{
				return m_Instance->commandID; 
			}
		}

        property Command command
		{
		public:
			Command get()
			{
				return m_Instance->command; 
			}
		}

	};


    [Serializable] public ref class FMD_S_LEAVE_SHIP : public ManagedObject<::FMD_S_LEAVE_SHIP>
	{
	public:
		
        FMD_S_LEAVE_SHIP(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_LEAVE_SHIP(

                        ShipID sidChild
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LEAVE_SHIP)
            END_PFM_CREATE

             pfm->sidChild = sidChild;

			this->m_Instance = pfm;
        }


        property ShipID sidChild
		{
		public:
			ShipID get()
			{
				return m_Instance->sidChild; 
			}
		}

	};


    [Serializable] public ref class FMD_S_JOINED_MISSION : public ManagedObject<::FMD_S_JOINED_MISSION>
	{
	public:
		
        FMD_S_JOINED_MISSION(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_JOINED_MISSION(

                        ShipID shipID,
                        float dtime,
                        Cookie cookie,
                        DWORD dwCookie
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, JOINED_MISSION)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->dtime = dtime;
             pfm->cookie = cookie;
             pfm->dwCookie = dwCookie;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property float dtime
		{
		public:
			float get()
			{
				return m_Instance->dtime; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

        property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie; 
			}
		}

	};


    [Serializable] public ref class FMD_S_LOADOUT_CHANGE : public ManagedObject<::FMD_S_LOADOUT_CHANGE>
	{
	public:
		
        FMD_S_LOADOUT_CHANGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_LOADOUT_CHANGE(

                        String ^ loadout,
                        String ^ rgPassengers,
                        ShipID sidShip
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LOADOUT_CHANGE)
                FM_VAR_PARM(StringToCharArray(loadout), CB_ZTS)
                FM_VAR_PARM(StringToCharArray(rgPassengers), CB_ZTS)
            END_PFM_CREATE

             pfm->sidShip = sidShip;

			this->m_Instance = pfm;
        }



        property String ^ loadout
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, loadout)); 
			}
		}




        property String ^ rgPassengers
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgPassengers)); 
			}
		}



        property ShipID sidShip
		{
		public:
			ShipID get()
			{
				return m_Instance->sidShip; 
			}
		}

	};


    [Serializable] public ref class FMD_C_ACTIVE_TURRET_UPDATE : public ManagedObject<::FMD_C_ACTIVE_TURRET_UPDATE>
	{
	public:
		
        FMD_C_ACTIVE_TURRET_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_ACTIVE_TURRET_UPDATE(

                        Time timeUpdate,
                        ClientActiveTurretUpdate atu
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, ACTIVE_TURRET_UPDATE)
            END_PFM_CREATE

             pfm->timeUpdate = timeUpdate;
             pfm->atu = atu;

			 this->m_Instance = pfm;
        }


        property Time timeUpdate
		{
		public:
			Time get()
			{
				return m_Instance->timeUpdate; 
			}
		}

        property ClientActiveTurretUpdate atu
		{
		public:
			ClientActiveTurretUpdate get()
			{
				return m_Instance->atu; 
			}
		}

	};


    [Serializable] public ref class FMD_C_INACTIVE_TURRET_UPDATE : public ManagedObject<::FMD_C_INACTIVE_TURRET_UPDATE>
	{
	public:
		
        FMD_C_INACTIVE_TURRET_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_INACTIVE_TURRET_UPDATE(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, INACTIVE_TURRET_UPDATE)
            END_PFM_CREATE

				this->m_Instance = pfm;
        }


	};


    [Serializable] public ref class FMD_S_TELEPORT_ACK : public ManagedObject<::FMD_S_TELEPORT_ACK>
	{
	public:
		
        FMD_S_TELEPORT_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_TELEPORT_ACK(

                        StationID stationID,
                        bool bNewHull
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, TELEPORT_ACK)
            END_PFM_CREATE

             pfm->stationID = stationID;
             pfm->bNewHull = bNewHull;

			 this->m_Instance = pfm;
        }


        property StationID stationID
		{
		public:
			StationID get()
			{
				return m_Instance->stationID; 
			}
		}

        property bool bNewHull
		{
		public:
			bool get()
			{
				return m_Instance->bNewHull; 
			}
		}

	};


    [Serializable] public ref class FMD_C_BOARD_SHIP : public ManagedObject<::FMD_C_BOARD_SHIP>
	{
	public:
		
        FMD_C_BOARD_SHIP(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_BOARD_SHIP(

                        ShipID sidParent
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, BOARD_SHIP)
            END_PFM_CREATE

             pfm->sidParent = sidParent;

			this->m_Instance = pfm;
        }


        property ShipID sidParent
		{
		public:
			ShipID get()
			{
				return m_Instance->sidParent; 
			}
		}

	};


    [Serializable] public ref class FMD_C_VIEW_CLUSTER : public ManagedObject<::FMD_C_VIEW_CLUSTER>
	{
	public:
		
        FMD_C_VIEW_CLUSTER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_VIEW_CLUSTER(

                        SectorID clusterID,
                        ObjectType otTarget,
                        ObjectID oidTarget
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, VIEW_CLUSTER)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->otTarget = otTarget;
             pfm->oidTarget = oidTarget;

			 this->m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property ObjectType otTarget
		{
		public:
			ObjectType get()
			{
				return m_Instance->otTarget; 
			}
		}

        property ObjectID oidTarget
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidTarget; 
			}
		}

	};


    [Serializable] public ref class FMD_S_KILL_SHIP : public ManagedObject<::FMD_S_KILL_SHIP>
	{
	public:
		
        FMD_S_KILL_SHIP(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_KILL_SHIP(

                        ShipID shipID,
                        ObjectType typeCredit,
                        ObjectID idCredit,
                        SideID sideidKiller,
                        bool bKillCredit,
                        bool bDeathCredit
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, KILL_SHIP)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->typeCredit = typeCredit;
             pfm->idCredit = idCredit;
             pfm->sideidKiller = sideidKiller;
             pfm->bKillCredit = bKillCredit;
             pfm->bDeathCredit = bDeathCredit;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property ObjectType typeCredit
		{
		public:
			ObjectType get()
			{
				return m_Instance->typeCredit; 
			}
		}

        property ObjectID idCredit
		{
		public:
			ObjectID get()
			{
				return m_Instance->idCredit; 
			}
		}

        property SideID sideidKiller
		{
		public:
			SideID get()
			{
				return m_Instance->sideidKiller; 
			}
		}

        property bool bKillCredit
		{
		public:
			bool get()
			{
				return m_Instance->bKillCredit; 
			}
		}

        property bool bDeathCredit
		{
		public:
			bool get()
			{
				return m_Instance->bDeathCredit; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SET_WINGID : public ManagedObject<::FMD_CS_SET_WINGID>
	{
	public:
		
        FMD_CS_SET_WINGID(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SET_WINGID(

                        ShipID shipID,
                        WingID wingID,
                        bool bCommanded
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SET_WINGID)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->wingID = wingID;
             pfm->bCommanded = bCommanded;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property WingID wingID
		{
		public:
			WingID get()
			{
				return m_Instance->wingID; 
			}
		}

        property bool bCommanded
		{
		public:
			bool get()
			{
				return m_Instance->bCommanded; 
			}
		}

	};


    [Serializable] public ref class FMD_S_ICQ_CHAT_ACK : public ManagedObject<::FMD_S_ICQ_CHAT_ACK>
	{
	public:
		
        FMD_S_ICQ_CHAT_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ICQ_CHAT_ACK(

                        String ^ Message,
                        int icqid
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ICQ_CHAT_ACK)
                FM_VAR_PARM(StringToCharArray(Message), CB_ZTS)
            END_PFM_CREATE

             pfm->icqid = icqid;

			this->m_Instance = pfm;
        }



        property String ^ Message
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Message)); 
			}
		}



        property int icqid
		{
		public:
			int get()
			{
				return m_Instance->icqid; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_CHATBUOY : public ManagedObject<::FMD_CS_CHATBUOY>
	{
	public:
		
        FMD_CS_CHATBUOY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_CHATBUOY(

                        String ^ Message,
                        ChatData cd,
                        DataBuoyIGC db
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, CHATBUOY)
                FM_VAR_PARM(StringToCharArray(Message), CB_ZTS)
            END_PFM_CREATE

             pfm->cd = cd;
             pfm->db = db;

			 this->m_Instance = pfm;
        }



        property String ^ Message
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Message)); 
			}
		}



        property ChatData cd
		{
		public:
			ChatData get()
			{
				return m_Instance->cd; 
			}
		}

        property DataBuoyIGC db
		{
		public:
			DataBuoyIGC get()
			{
				return m_Instance->db; 
			}
		}

	};


    [Serializable] public ref class FMD_S_CREATE_CHAFF : public ManagedObject<::FMD_S_CREATE_CHAFF>
	{
	public:
		
        FMD_S_CREATE_CHAFF(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_CREATE_CHAFF(

                        Vector p0,
                        Vector v0,
                        Time time0,
                        ExpendableTypeID etid
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, CREATE_CHAFF)
            END_PFM_CREATE

             pfm->p0 = p0;
             pfm->v0 = v0;
             pfm->time0 = time0;
             pfm->etid = etid;

			 this->m_Instance = pfm;
        }


        property Vector p0
		{
		public:
			Vector get()
			{
				return m_Instance->p0; 
			}
		}

        property Vector v0
		{
		public:
			Vector get()
			{
				return m_Instance->v0; 
			}
		}

        property Time time0
		{
		public:
			Time get()
			{
				return m_Instance->time0; 
			}
		}

        property ExpendableTypeID etid
		{
		public:
			ExpendableTypeID get()
			{
				return m_Instance->etid; 
			}
		}

	};


    [Serializable] public ref class FMD_S_MISSILE_SPOOFED : public ManagedObject<::FMD_S_MISSILE_SPOOFED>
	{
	public:
		
        FMD_S_MISSILE_SPOOFED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_MISSILE_SPOOFED(

                        MissileID missileID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, MISSILE_SPOOFED)
            END_PFM_CREATE

             pfm->missileID = missileID;

			this->m_Instance = pfm;
        }


        property MissileID missileID
		{
		public:
			MissileID get()
			{
				return m_Instance->missileID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_END_SPOOFING : public ManagedObject<::FMD_S_END_SPOOFING>
	{
	public:
		
        FMD_S_END_SPOOFING(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_END_SPOOFING(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, END_SPOOFING)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }


	};


    [Serializable] public ref class FMD_C_AUTODONATE : public ManagedObject<::FMD_C_AUTODONATE>
	{
	public:
		
        FMD_C_AUTODONATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_AUTODONATE(

                        ShipID sidDonateTo,
                        Money amount
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, AUTODONATE)
            END_PFM_CREATE

             pfm->sidDonateTo = sidDonateTo;
             pfm->amount = amount;

			 this->m_Instance = pfm;
        }


        property ShipID sidDonateTo
		{
		public:
			ShipID get()
			{
				return m_Instance->sidDonateTo; 
			}
		}

        property Money amount
		{
		public:
			Money get()
			{
				return m_Instance->amount; 
			}
		}

	};

	// Must be kept in sync with MapMakerID in igc.h... :( 
	public enum class MapType
	{
		c_mmSingleRing = 0,
		c_mmDoubleRing = 1,
		c_mmPinWheel = 2,
		c_mmDiamondRing = 3,
		c_mmSnowFlake = 4,
		c_mmSplitBase = 5,
		c_mmBrawl = 6,
		c_mmBigRing = 7,
		c_mmHiLo = 8,
		c_mmHiHigher = 9,
		c_mmStar = 10,
		c_mmInsideOut = 11,
		c_mmGrid = 12,
		c_mmEastWest = 13,
		c_mmLargeSplit = 14,
		c_mmMax = 15
	};
	
	// Really limited right now, but we can add more parameters as required. Really just focusing on setting the map to HiHigher for the bots. 
    [Serializable] public ref class FMD_CS_MISSIONPARAMS : public ManagedObject<::FMD_CS_MISSIONPARAMS>
	{
	public:
		
        FMD_CS_MISSIONPARAMS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_MISSIONPARAMS(
			String ^ gameName,
			MapType mapID,
			int nTotalMaxPlayersPerGame,
			int nMaxPlayersPerTeam
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, MISSIONPARAMS)
				END_PFM_CREATE

			MissionParams missionParams;

			strcpy(missionParams.strGameName, StringToCharArray(gameName));
			missionParams.mmMapType = (MapMakerID)mapID;
			missionParams.nTotalMaxPlayersPerGame = nTotalMaxPlayersPerGame;
			missionParams.nMaxPlayersPerTeam = nMaxPlayersPerTeam;

			pfm->missionparams = missionParams;

			this->m_Instance = pfm;
		}


        /*property MissionParams missionparams
		{
		public:
			MissionParams get()
			{
				return m_Instance->missionparams; 
			}
		}*/

	};


    [Serializable] public ref class FMD_S_PAYDAY : public ManagedObject<::FMD_S_PAYDAY>
	{
	public:
		
        FMD_S_PAYDAY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PAYDAY(

                        Money dMoney
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PAYDAY)
            END_PFM_CREATE

             pfm->dMoney = dMoney;

			this->m_Instance = pfm;
        }


        property Money dMoney
		{
		public:
			Money get()
			{
				return m_Instance->dMoney; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SET_MONEY : public ManagedObject<::FMD_S_SET_MONEY>
	{
	public:
		
        FMD_S_SET_MONEY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SET_MONEY(

                        Money money,
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SET_MONEY)
            END_PFM_CREATE

             pfm->money = money;
             pfm->shipID = shipID;

			 this->m_Instance = pfm;
        }


        property Money money
		{
		public:
			Money get()
			{
				return m_Instance->money; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_AUTODONATE : public ManagedObject<::FMD_S_AUTODONATE>
	{
	public:
		
        FMD_S_AUTODONATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_AUTODONATE(

                        ShipID sidDonateBy,
                        ShipID sidDonateTo,
                        Money amount
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, AUTODONATE)
            END_PFM_CREATE

             pfm->sidDonateBy = sidDonateBy;
             pfm->sidDonateTo = sidDonateTo;
             pfm->amount = amount;

			 this->m_Instance = pfm;
        }


        property ShipID sidDonateBy
		{
		public:
			ShipID get()
			{
				return m_Instance->sidDonateBy; 
			}
		}

        property ShipID sidDonateTo
		{
		public:
			ShipID get()
			{
				return m_Instance->sidDonateTo; 
			}
		}

        property Money amount
		{
		public:
			Money get()
			{
				return m_Instance->amount; 
			}
		}

	};


    [Serializable] public ref class FMD_C_MUTE : public ManagedObject<::FMD_C_MUTE>
	{
	public:
		
        FMD_C_MUTE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_MUTE(

                        ShipID shipID,
                        bool bMute
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, MUTE)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->bMute = bMute;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property bool bMute
		{
		public:
			bool get()
			{
				return m_Instance->bMute; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SET_TEAM_LEADER : public ManagedObject<::FMD_CS_SET_TEAM_LEADER>
	{
	public:
		
        FMD_CS_SET_TEAM_LEADER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SET_TEAM_LEADER(

						SideID    sideID,// server to notify everyone of a new team leader 
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SET_TEAM_LEADER)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->shipID = shipID;
			 this->m_Instance = pfm;

        }


        property SideID    sideID // server to notify everyone of a new team leader 
		{
		public:
			SideID get() // server to notify everyone of a new team leader get()
			{
				return m_Instance->sideID; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SET_TEAM_INVESTOR : public ManagedObject<::FMD_CS_SET_TEAM_INVESTOR>
	{
	public:
		
        FMD_CS_SET_TEAM_INVESTOR(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SET_TEAM_INVESTOR(

			SideID    sideID, // server to notify everyone of a new team investor 
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SET_TEAM_INVESTOR)
            END_PFM_CREATE

             pfm-> sideID =   sideID;
             pfm->shipID = shipID;

			 this->m_Instance = pfm;
        }


        property SideID    sideID // server to notify everyone of a new team investor 
		{
		public:
			SideID get() // server to notify everyone of a new team investor get()
			{
				return m_Instance->sideID; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_SET_MISSION_OWNER : public ManagedObject<::FMD_CS_SET_MISSION_OWNER>
	{
	public:
		
        FMD_CS_SET_MISSION_OWNER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_SET_MISSION_OWNER(

			SideID    sideID, // sent when someone is promoted to be mission owner 
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, SET_MISSION_OWNER)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->shipID = shipID;

			 this->m_Instance = pfm;
        }


        property  SideID    sideID // sent when someone is promoted to be mission owner
		{
		public:
			SideID get() // sent when someone is promoted to be mission owner get()
			{
				return m_Instance->sideID; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_QUIT_MISSION : public ManagedObject<::FMD_CS_QUIT_MISSION>
	{
	public:
		
        FMD_CS_QUIT_MISSION(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_QUIT_MISSION(
			String ^ szMessageParam,
                        // a player on their team, or by any player to boot themselves FM_VAR_ITEM(szMessageParam),
                        ShipID shipID,
                        QuitSideReason reason
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, QUIT_MISSION)
				FM_VAR_PARM(StringToCharArray(szMessageParam), CB_ZTS)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->reason = reason;

			 this->m_Instance = pfm;
        }


        property String ^ szMessageParam // a player on their team, or by any player to boot themselves FM_VAR_ITEM(szMessageParam)
		{
		public:
			String ^ get()// a player on their team, or by any player to boot themselves get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szMessageParam));
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property QuitSideReason reason
		{
		public:
			QuitSideReason get()
			{
				return m_Instance->reason; 
			}
		}

	};


    [Serializable] public ref class FMD_S_JOIN_SIDE : public ManagedObject<::FMD_S_JOIN_SIDE>
	{
	public:
		
        FMD_S_JOIN_SIDE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_JOIN_SIDE(

                        SideID sideID,
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, JOIN_SIDE)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->shipID = shipID;

			 this->m_Instance = pfm;
        }


        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_QUIT_SIDE : public ManagedObject<::FMD_CS_QUIT_SIDE>
	{
	public:
		
        FMD_CS_QUIT_SIDE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_QUIT_SIDE(

                        String ^ szMessageParam,
                        ShipID shipID,
                        int reason // was QuitSideReason
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, QUIT_SIDE)
                FM_VAR_PARM(StringToCharArray(szMessageParam), CB_ZTS)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->reason = (QuitSideReason) reason;

			 this->m_Instance = pfm;
        }



        property String ^ szMessageParam
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szMessageParam)); 
			}
		}



        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property QuitSideReason reason
		{
		public:
			QuitSideReason get()
			{
				return m_Instance->reason; 
			}
		}

	};


    [Serializable] public ref class FMD_S_ENTER_GAME : public ManagedObject<::FMD_S_ENTER_GAME>
	{
	public:
		
        FMD_S_ENTER_GAME(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ENTER_GAME(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ENTER_GAME)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }


	};


    [Serializable] public ref class FMD_CS_RELOAD : public ManagedObject<::FMD_CS_RELOAD>
	{
	public:
		
        FMD_CS_RELOAD(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_RELOAD(

                        String ^ rgReloads,
                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, RELOAD)
                FM_VAR_PARM(StringToCharArray(rgReloads), CB_ZTS)
            END_PFM_CREATE

             pfm->shipID = shipID;

			this->m_Instance = pfm;
        }



        property String ^ rgReloads
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgReloads)); 
			}
		}



        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


   /* [Serializable] public ref class FMD_S_THREAT : public ManagedObject<::FMD_S_THREAT>
	{
	public:
		
        FMD_S_THREAT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_THREAT(

                        SectorID sectorID,
                        ClusterWarning warning
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, THREAT)
            END_PFM_CREATE

             pfm->sectorID = sectorID;
             pfm->warning = warning;

			 this->m_Instance = pfm;
        }


        property SectorID sectorID
		{
		public:
			SectorID get()
			{
				return m_Instance->sectorID; 
			}
		}

        property ClusterWarning warning
		{
		public:
			ClusterWarning get()
			{
				return m_Instance->warning; 
			}
		}

	};*/

	// TODO: Handle all arrays in this message
 //   [Serializable] public ref class FMD_S_GAME_STATE : public ManagedObject<::FMD_S_GAME_STATE>
	//{
	//public:
	//	
 //       FMD_S_GAME_STATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
 //       {
 //       }

	//	FMD_S_GAME_STATE(

 //                       String ^ char conquest /* Originally: unsigned char conquest[c_cSidesMax] */,
 //                       String ^ char territory /* Originally: unsigned char territory[c_cSidesMax] */,
 //                       String ^ char nFlags /* Originally: unsigned char nFlags[c_cSidesMax] */,
 //                       String ^ char nArtifacts /* Originally: unsigned char nArtifacts[c_cSidesMax] */
 //           ) 
	//	{
 //           SimpleClient sc;
	//		FedMessaging fm(&sc);

	//		BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, GAME_STATE)
 //           END_PFM_CREATE

 //            strcpy(pfm->char conquest, StringToCharArray(char conquest)); /* Originally: pfm->char conquest[c_cSidesMax] = char conquest[c_cSidesMax]; */
 //            strcpy(pfm->char territory, StringToCharArray(char territory)); /* Originally: pfm->char territory[c_cSidesMax] = char territory[c_cSidesMax]; */
 //            strcpy(pfm->char nFlags, StringToCharArray(char nFlags)); /* Originally: pfm->char nFlags[c_cSidesMax] = char nFlags[c_cSidesMax]; */
 //            strcpy(pfm->char nArtifacts, StringToCharArray(char nArtifacts)); /* Originally: pfm->char nArtifacts[c_cSidesMax] = char nArtifacts[c_cSidesMax]; */

//				this->m_Instance = pfm;
 //       }


 //       property String ^ char conquest /* Originally: unsigned char conquest[c_cSidesMax] */
	//	{
	//	public:
	//		String ^ get()
	//		{
	//			return gcnew String(m_Instance->char conquest); 
	//		}
	//	}

 //       property String ^ char territory /* Originally: unsigned char territory[c_cSidesMax] */
	//	{
	//	public:
	//		String ^ get()
	//		{
	//			return gcnew String(m_Instance->char territory); 
	//		}
	//	}

 //       property String ^ char nFlags /* Originally: unsigned char nFlags[c_cSidesMax] */
	//	{
	//	public:
	//		String ^ get()
	//		{
	//			return gcnew String(m_Instance->char nFlags); 
	//		}
	//	}

 //       property String ^ char nArtifacts /* Originally: unsigned char nArtifacts[c_cSidesMax] */
	//	{
	//	public:
	//		String ^ get()
	//		{
	//			return gcnew String(m_Instance->char nArtifacts); 
	//		}
	//	}

	//};


    [Serializable] public ref class FMD_S_GAIN_FLAG : public ManagedObject<::FMD_S_GAIN_FLAG>
	{
	public:
		
        FMD_S_GAIN_FLAG(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_GAIN_FLAG(

                        SideID sideidFlag,
                        ShipID shipidRecipient,
                        bool bIsTreasureDocked,
                        PartID parttypeidDocked
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, GAIN_FLAG)
            END_PFM_CREATE

             pfm->sideidFlag = sideidFlag;
             pfm->shipidRecipient = shipidRecipient;
             pfm->bIsTreasureDocked = bIsTreasureDocked;
             pfm->parttypeidDocked = parttypeidDocked;

			 this->m_Instance = pfm;
        }


        property SideID sideidFlag
		{
		public:
			SideID get()
			{
				return m_Instance->sideidFlag; 
			}
		}

        property ShipID shipidRecipient
		{
		public:
			ShipID get()
			{
				return m_Instance->shipidRecipient; 
			}
		}

        property bool bIsTreasureDocked
		{
		public:
			bool get()
			{
				return m_Instance->bIsTreasureDocked; 
			}
		}

        property PartID parttypeidDocked
		{
		public:
			PartID get()
			{
				return m_Instance->parttypeidDocked; 
			}
		}

	};


    [Serializable] public ref class FMD_C_START_GAME : public ManagedObject<::FMD_C_START_GAME>
	{
	public:
		
        FMD_C_START_GAME(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_START_GAME(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, START_GAME)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }


	};


    [Serializable] public ref class FMD_S_ACQUIRE_TREASURE : public ManagedObject<::FMD_S_ACQUIRE_TREASURE>
	{
	public:
		
        FMD_S_ACQUIRE_TREASURE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ACQUIRE_TREASURE(

                        TreasureCode treasureCode,
                        ObjectID treasureID,
                        short amount
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ACQUIRE_TREASURE)
            END_PFM_CREATE

             pfm->treasureCode = treasureCode;
             pfm->treasureID = treasureID;
             pfm->amount = amount;

			 this->m_Instance = pfm;
        }


        property TreasureCode treasureCode
		{
		public:
			TreasureCode get()
			{
				return m_Instance->treasureCode; 
			}
		}

        property ObjectID treasureID
		{
		public:
			ObjectID get()
			{
				return m_Instance->treasureID; 
			}
		}

        property short amount
		{
		public:
			short get()
			{
				return m_Instance->amount; 
			}
		}

	};


    [Serializable] public ref class FMD_C_TREASURE_ACK : public ManagedObject<::FMD_C_TREASURE_ACK>
	{
	public:
		
        FMD_C_TREASURE_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_TREASURE_ACK(

                        Mount mountID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, TREASURE_ACK)
            END_PFM_CREATE

             pfm->mountID = mountID;

			this->m_Instance = pfm;
        }


        property Mount mountID
		{
		public:
			Mount get()
			{
				return m_Instance->mountID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_ADD_PART : public ManagedObject<::FMD_S_ADD_PART>
	{
	public:
		
        FMD_S_ADD_PART(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ADD_PART(

                        ShipID shipID,
                        PartData newPartData
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ADD_PART)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->newPartData = newPartData;

			 this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property PartData newPartData
		{
		public:
			PartData get()
			{
				return m_Instance->newPartData; 
			}
		}

	};


    [Serializable] public ref class FMD_S_ENTER_LIFEPOD : public ManagedObject<::FMD_S_ENTER_LIFEPOD>
	{
	public:
		
        FMD_S_ENTER_LIFEPOD(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ENTER_LIFEPOD(

                        ShipID shipID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ENTER_LIFEPOD)
            END_PFM_CREATE

             pfm->shipID = shipID;

			this->m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_LIGHT_SHIPS_UPDATE : public ManagedObject<::FMD_S_LIGHT_SHIPS_UPDATE>
	{
	public:
		
        FMD_S_LIGHT_SHIPS_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_LIGHT_SHIPS_UPDATE(

                        String ^ rgInactiveTurretUpdates,
                        String ^ rgLightShipUpdates
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LIGHT_SHIPS_UPDATE)
                FM_VAR_PARM(StringToCharArray(rgInactiveTurretUpdates), CB_ZTS)
                FM_VAR_PARM(StringToCharArray(rgLightShipUpdates), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ rgInactiveTurretUpdates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgInactiveTurretUpdates)); 
			}
		}




        property String ^ rgLightShipUpdates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgLightShipUpdates)); 
			}
		}



	};


    [Serializable] public ref class FMD_S_HEAVY_SHIPS_UPDATE : public ManagedObject<::FMD_S_HEAVY_SHIPS_UPDATE>
	{
	public:
		
        FMD_S_HEAVY_SHIPS_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_HEAVY_SHIPS_UPDATE(

                        String ^ rgActiveTurretUpdates,
                        String ^ rgHeavyShipUpdates,
                        Time timeReference,
                        Vector positionReference,
                        BytePercentage bpTargetHull,
                        BytePercentage bpTargetShield,
                        CompactShipFractions fractions
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, HEAVY_SHIPS_UPDATE)
                FM_VAR_PARM(StringToCharArray(rgActiveTurretUpdates), CB_ZTS)
                FM_VAR_PARM(StringToCharArray(rgHeavyShipUpdates), CB_ZTS)
            END_PFM_CREATE

             pfm->timeReference = timeReference;
             pfm->positionReference = positionReference;
             pfm->bpTargetHull = bpTargetHull;
             pfm->bpTargetShield = bpTargetShield;
             pfm->fractions = fractions;

			 this->m_Instance = pfm;
        }



        property String ^ rgActiveTurretUpdates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgActiveTurretUpdates)); 
			}
		}




        property String ^ rgHeavyShipUpdates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgHeavyShipUpdates)); 
			}
		}



        property Time timeReference
		{
		public:
			Time get()
			{
				return m_Instance->timeReference; 
			}
		}

        property Vector positionReference
		{
		public:
			Vector get()
			{
				return m_Instance->positionReference; 
			}
		}

        property BytePercentage bpTargetHull
		{
		public:
			BytePercentage get()
			{
				return m_Instance->bpTargetHull; 
			}
		}

        property BytePercentage bpTargetShield
		{
		public:
			BytePercentage get()
			{
				return m_Instance->bpTargetShield; 
			}
		}

        property CompactShipFractions fractions
		{
		public:
			CompactShipFractions get()
			{
				return m_Instance->fractions; 
			}
		}

	};


    [Serializable] public ref class FMD_S_VIEW_CLUSTER : public ManagedObject<::FMD_S_VIEW_CLUSTER>
	{
	public:
		
        FMD_S_VIEW_CLUSTER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_VIEW_CLUSTER(

                        Vector position,
                        SectorID clusterID,
                        bool bUsePosition
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, VIEW_CLUSTER)
            END_PFM_CREATE

             pfm->position = position;
             pfm->clusterID = clusterID;
             pfm->bUsePosition = bUsePosition;

			 this->m_Instance = pfm;
        }


        property Vector position
		{
		public:
			Vector get()
			{
				return m_Instance->position; 
			}
		}

        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property bool bUsePosition
		{
		public:
			bool get()
			{
				return m_Instance->bUsePosition; 
			}
		}

	};


    [Serializable] public ref class FMD_S_BOARD_NACK : public ManagedObject<::FMD_S_BOARD_NACK>
	{
	public:
		
        FMD_S_BOARD_NACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_BOARD_NACK(

                        ShipID sidRequestedParent
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, BOARD_NACK)
            END_PFM_CREATE

             pfm->sidRequestedParent = sidRequestedParent;

			this->m_Instance = pfm;
        }


        property ShipID sidRequestedParent
		{
		public:
			ShipID get()
			{
				return m_Instance->sidRequestedParent; 
			}
		}

	};


    [Serializable] public ref class FMD_S_ASTEROIDS_UPDATE : public ManagedObject<::FMD_S_ASTEROIDS_UPDATE>
	{
	public:
		
        FMD_S_ASTEROIDS_UPDATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ASTEROIDS_UPDATE(

                        String ^ rgAsteroidStates
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ASTEROIDS_UPDATE)
                FM_VAR_PARM(StringToCharArray(rgAsteroidStates), CB_ZTS)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }



        property String ^ rgAsteroidStates
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgAsteroidStates)); 
			}
		}



	};


    [Serializable] public ref class FMD_S_ASTEROID_DRAINED : public ManagedObject<::FMD_S_ASTEROID_DRAINED>
	{
	public:
		
        FMD_S_ASTEROID_DRAINED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ASTEROID_DRAINED(

                        SectorID clusterID,
                        AsteroidID asteroidID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ASTEROID_DRAINED)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->asteroidID = asteroidID;

			 this->m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property AsteroidID asteroidID
		{
		public:
			AsteroidID get()
			{
				return m_Instance->asteroidID; 
			}
		}

	};


    [Serializable] public ref class FMD_S_BUILDINGEFFECT_DESTROYED : public ManagedObject<::FMD_S_BUILDINGEFFECT_DESTROYED>
	{
	public:
		
        FMD_S_BUILDINGEFFECT_DESTROYED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_BUILDINGEFFECT_DESTROYED(

                        AsteroidID asteroidID
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, BUILDINGEFFECT_DESTROYED)
            END_PFM_CREATE

             pfm->asteroidID = asteroidID;

			this->m_Instance = pfm;
        }


        property AsteroidID asteroidID
		{
		public:
			AsteroidID get()
			{
				return m_Instance->asteroidID; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_REQUEST_MONEY : public ManagedObject<::FMD_CS_REQUEST_MONEY>
	{
	public:
		
        FMD_CS_REQUEST_MONEY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_REQUEST_MONEY(

                        Money amount,
                        ShipID shipidRequest,
                        HullID hidFor
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, REQUEST_MONEY)
            END_PFM_CREATE

             pfm->amount = amount;
             pfm->shipidRequest = shipidRequest;
             pfm->hidFor = hidFor;

			 this->m_Instance = pfm;
        }


        property Money amount
		{
		public:
			Money get()
			{
				return m_Instance->amount; 
			}
		}

        property ShipID shipidRequest
		{
		public:
			ShipID get()
			{
				return m_Instance->shipidRequest; 
			}
		}

        property HullID hidFor
		{
		public:
			HullID get()
			{
				return m_Instance->hidFor; 
			}
		}

	};


    [Serializable] public ref class FMD_S_SHIP_RESET : public ManagedObject<::FMD_S_SHIP_RESET>
	{
	public:
		
        FMD_S_SHIP_RESET(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_SHIP_RESET(

                        ServerSingleShipUpdate shipupdate,
                        Cookie cookie
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, SHIP_RESET)
            END_PFM_CREATE

             pfm->shipupdate = shipupdate;
             pfm->cookie = cookie;

			 this->m_Instance = pfm;
        }


        property ServerSingleShipUpdate shipupdate
		{
		public:
			ServerSingleShipUpdate get()
			{
				return m_Instance->shipupdate; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

	};


    [Serializable] public ref class FMD_C_RIPCORD_REQUEST : public ManagedObject<::FMD_C_RIPCORD_REQUEST>
	{
	public:
		
        FMD_C_RIPCORD_REQUEST(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_RIPCORD_REQUEST(

                        SectorID sidRipcord,
                        bool bHasGoal,
                        Vector positionGoal
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, RIPCORD_REQUEST)
            END_PFM_CREATE

             pfm->sidRipcord = sidRipcord;
             pfm->bHasGoal = bHasGoal;
             pfm->positionGoal = positionGoal;

			this->m_Instance = pfm;
        }


        property SectorID sidRipcord
		{
		public:
			SectorID get()
			{
				return m_Instance->sidRipcord; 
			}
		}

        property bool bHasGoal
		{
		public:
			bool get()
			{
				return m_Instance->bHasGoal; 
			}
		}

        property Vector positionGoal
		{
		public:
			Vector get()
			{
				return m_Instance->positionGoal; 
			}
		}

	};


    [Serializable] public ref class FMD_S_RIPCORD_ACTIVATE : public ManagedObject<::FMD_S_RIPCORD_ACTIVATE>
	{
	public:
		
        FMD_S_RIPCORD_ACTIVATE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_RIPCORD_ACTIVATE(

                        ShipID shipidRipcord,
                        ObjectType otRipcord,
                        ObjectID oidRipcord,
                        SectorID sidRipcord
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, RIPCORD_ACTIVATE)
            END_PFM_CREATE

             pfm->shipidRipcord = shipidRipcord;
             pfm->otRipcord = otRipcord;
             pfm->oidRipcord = oidRipcord;
             pfm->sidRipcord = sidRipcord;

			 this->m_Instance = pfm;
        }


        property ShipID shipidRipcord
		{
		public:
			ShipID get()
			{
				return m_Instance->shipidRipcord; 
			}
		}

        property ObjectType otRipcord
		{
		public:
			ObjectType get()
			{
				return m_Instance->otRipcord; 
			}
		}

        property ObjectID oidRipcord
		{
		public:
			ObjectID get()
			{
				return m_Instance->oidRipcord; 
			}
		}

        property SectorID sidRipcord
		{
		public:
			SectorID get()
			{
				return m_Instance->sidRipcord; 
			}
		}

	};


    [Serializable] public ref class FMD_S_RIPCORD_DENIED : public ManagedObject<::FMD_S_RIPCORD_DENIED>
	{
	public:
		
        FMD_S_RIPCORD_DENIED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_RIPCORD_DENIED(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, RIPCORD_DENIED)
            END_PFM_CREATE

				this->m_Instance = pfm;

        }


	};


    [Serializable] public ref class FMD_S_RIPCORD_ABORTED : public ManagedObject<::FMD_S_RIPCORD_ABORTED>
	{
	public:
		
        FMD_S_RIPCORD_ABORTED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_RIPCORD_ABORTED(

                        ShipID shipidRipcord,
                        bool bLanded
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, RIPCORD_ABORTED)
            END_PFM_CREATE

             pfm->shipidRipcord = shipidRipcord;
             pfm->bLanded = bLanded;

			this->m_Instance = pfm;
        }


        property ShipID shipidRipcord
		{
		public:
			ShipID get()
			{
				return m_Instance->shipidRipcord; 
			}
		}

        property bool bLanded
		{
		public:
			bool get()
			{
				return m_Instance->bLanded; 
			}
		}

	};


    [Serializable] public ref class FMD_S_WARP_BOMB : public ManagedObject<::FMD_S_WARP_BOMB>
	{
	public:
		
        FMD_S_WARP_BOMB(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_WARP_BOMB(

                        int timeExplosion,
                        short warpidBombed,
                        short expendableidMissile
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, WARP_BOMB)
            END_PFM_CREATE

             pfm->timeExplosion = timeExplosion;
             pfm->warpidBombed = warpidBombed;
             pfm->expendableidMissile = expendableidMissile;

			 this->m_Instance = pfm;
        }


        property Time timeExplosion
		{
		public:
			Time get()
			{
				return m_Instance->timeExplosion; 
			}
		}

        property WarpID warpidBombed
		{
		public:
			WarpID get()
			{
				return m_Instance->warpidBombed; 
			}
		}

        property ExpendableTypeID expendableidMissile
		{
		public:
			ExpendableTypeID get()
			{
				return m_Instance->expendableidMissile; 
			}
		}

	};


    [Serializable] public ref class FMD_S_PROMOTE : public ManagedObject<::FMD_S_PROMOTE>
	{
	public:
		
        FMD_S_PROMOTE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PROMOTE(

                        ShipID shipidPromoted
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PROMOTE)
            END_PFM_CREATE

             pfm->shipidPromoted = shipidPromoted;

			m_Instance = pfm;
        }


        property ShipID shipidPromoted
		{
		public:
			ShipID get()
			{
				return m_Instance->shipidPromoted; 
			}
		}

	};


    [Serializable] public ref class FMD_C_PROMOTE : public ManagedObject<::FMD_C_PROMOTE>
	{
	public:
		
        FMD_C_PROMOTE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_PROMOTE(

                        Mount mountidPromoted
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, PROMOTE)
            END_PFM_CREATE

             pfm->mountidPromoted = mountidPromoted;

			m_Instance = pfm;
        }


        property Mount mountidPromoted
		{
		public:
			Mount get()
			{
				return m_Instance->mountidPromoted; 
			}
		}

	};


    [Serializable] public ref class FMD_S_CREATE_BUCKETS : public ManagedObject<::FMD_S_CREATE_BUCKETS>
	{
	public:
		
        FMD_S_CREATE_BUCKETS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_CREATE_BUCKETS(

                        TechTreeBitMask ttbmDevelopments,
                        TechTreeBitMask ttbmInitial
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, CREATE_BUCKETS)
            END_PFM_CREATE

             pfm->ttbmDevelopments = ttbmDevelopments;
             pfm->ttbmInitial = ttbmInitial;

			 m_Instance = pfm;
        }


        property TechTreeBitMask ttbmDevelopments
		{
		public:
			TechTreeBitMask get()
			{
				return m_Instance->ttbmDevelopments; 
			}
		}

        property TechTreeBitMask ttbmInitial
		{
		public:
			TechTreeBitMask get()
			{
				return m_Instance->ttbmInitial; 
			}
		}

	};


    [Serializable] public ref class FMD_S_RELAUNCH_SHIP : public ManagedObject<::FMD_S_RELAUNCH_SHIP>
	{
	public:
		
        FMD_S_RELAUNCH_SHIP(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_RELAUNCH_SHIP(

                        String ^ loadout,
                        ShipID shipID,
                        ShipID carrierID,
                        Vector position,
                        Vector velocity,
                        CompactOrientation orientation,
                        Cookie cookie
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, RELAUNCH_SHIP)
                FM_VAR_PARM(StringToCharArray(loadout), CB_ZTS)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->carrierID = carrierID;
             pfm->position = position;
             pfm->velocity = velocity;
             pfm->orientation = orientation;
             pfm->cookie = cookie;

			 m_Instance = pfm;
        }



        property String ^ loadout
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, loadout)); 
			}
		}



        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property ShipID carrierID
		{
		public:
			ShipID get()
			{
				return m_Instance->carrierID; 
			}
		}

        property Vector position
		{
		public:
			Vector get()
			{
				return m_Instance->position; 
			}
		}

        property Vector velocity
		{
		public:
			Vector get()
			{
				return m_Instance->velocity; 
			}
		}

        property CompactOrientation orientation
		{
		public:
			CompactOrientation get()
			{
				return m_Instance->orientation; 
			}
		}

        property Cookie cookie
		{
		public:
			Cookie get()
			{
				return m_Instance->cookie; 
			}
		}

	};


    /*[Serializable] public ref class FMD_C_BANDWIDTH : public ManagedObject<::FMD_C_BANDWIDTH>
	{
	public:
		
        FMD_C_BANDWIDTH(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_BANDWIDTH(

                        unsigned int          value
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, BANDWIDTH)
            END_PFM_CREATE

             pfm->int          value = int          value;

			 m_Instance = pfm;
        }


        property unsigned int          value
		{
		public:
			unsigned get()
			{
				return m_Instance->int          value; 
			}
		}

	};*/


    [Serializable] public ref class FMD_S_PINGDATA : public ManagedObject<::FMD_S_PINGDATA>
	{
	public:
		
        FMD_S_PINGDATA(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_PINGDATA(

                        ShipID shipID,
                        unsigned short	  ping,
                        unsigned short      loss
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PINGDATA)
            END_PFM_CREATE

             pfm->shipID = shipID;
             pfm->ping =  ping;
             pfm->loss = loss;

			 m_Instance = pfm;
        }


        property ShipID shipID
		{
		public:
			ShipID get()
			{
				return m_Instance->shipID; 
			}
		}

        property unsigned short	  ping
		{
		public:
			unsigned short get()
			{
				return m_Instance->ping; 
			}
		}

        property unsigned short      loss
		{
		public:
			unsigned short get()
			{
				return m_Instance->loss; 
			}
		}

	};


    [Serializable] public ref class FMD_C_REQPINGDATA : public ManagedObject<::FMD_C_REQPINGDATA>
	{
	public:
		
        FMD_C_REQPINGDATA(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_REQPINGDATA(

            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, REQPINGDATA)
            END_PFM_CREATE

				m_Instance = pfm;
        }


	};


    [Serializable] public ref class FMD_C_CHANGE_ALLIANCE : public ManagedObject<::FMD_C_CHANGE_ALLIANCE>
	{
	public:
		
        FMD_C_CHANGE_ALLIANCE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_C_CHANGE_ALLIANCE(

                        SideID sideID,
                        SideID sideAlly
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, CHANGE_ALLIANCE)
            END_PFM_CREATE

             pfm->sideID = sideID;
             pfm->sideAlly = sideAlly;

			 m_Instance = pfm;
        }


        property SideID sideID
		{
		public:
			SideID get()
			{
				return m_Instance->sideID; 
			}
		}

        property SideID sideAlly
		{
		public:
			SideID get()
			{
				return m_Instance->sideAlly; 
			}
		}

	};


    [Serializable] public ref class FMD_S_CHANGE_ALLIANCES : public ManagedObject<::FMD_S_CHANGE_ALLIANCES>
	{
	public:
		
        FMD_S_CHANGE_ALLIANCES(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_CHANGE_ALLIANCES(

                        String ^ Allies /* Originally: char Allies[c_cSidesMax] */
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, CHANGE_ALLIANCES)
            END_PFM_CREATE

             strcpy(pfm->Allies, StringToCharArray(Allies)); /* Originally: pfm->Allies[c_cSidesMax] = Allies[c_cSidesMax]; */

			m_Instance = pfm;
        }


        property String ^ Allies /* Originally: char Allies[c_cSidesMax] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->Allies); 
			}
		}

	};


    [Serializable] public ref class FMD_S_ASTEROID_MINED : public ManagedObject<::FMD_S_ASTEROID_MINED>
	{
	public:
		
        FMD_S_ASTEROID_MINED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_S_ASTEROID_MINED(

                        SectorID clusterID,
                        AsteroidID asteroidID,
                        BytePercentage bpOreFraction
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, ASTEROID_MINED)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->asteroidID = asteroidID;
             pfm->bpOreFraction = bpOreFraction;

			 m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property AsteroidID asteroidID
		{
		public:
			AsteroidID get()
			{
				return m_Instance->asteroidID; 
			}
		}

        property BytePercentage bpOreFraction
		{
		public:
			BytePercentage get()
			{
				return m_Instance->bpOreFraction; 
			}
		}

	};


    [Serializable] public ref class FMD_CS_HIGHLIGHT_CLUSTER : public ManagedObject<::FMD_CS_HIGHLIGHT_CLUSTER>
	{
	public:
		
        FMD_CS_HIGHLIGHT_CLUSTER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
        {
        }

		FMD_CS_HIGHLIGHT_CLUSTER(

                        SectorID clusterID,
                        ClusterHighlight highlight
            ) 
		{
            SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, CS, HIGHLIGHT_CLUSTER)
            END_PFM_CREATE

             pfm->clusterID = clusterID;
             pfm->highlight = highlight;

			 m_Instance = pfm;
        }


        property SectorID clusterID
		{
		public:
			SectorID get()
			{
				return m_Instance->clusterID; 
			}
		}

        property ClusterHighlight highlight
		{
		public:
			ClusterHighlight get()
			{
				return m_Instance->highlight; 
			}
		}

	};

	[Serializable] public ref class FMD_C_LOGON_LOBBY_OLD : public ManagedObject<::FMD_C_LOGON_LOBBY_OLD>
	{
	public:

		FMD_C_LOGON_LOBBY_OLD(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_LOGON_LOBBY_OLD(

			String ^ ZoneTicket,
			short verLobby,
			int crcFileList,
			DWORD dwTime
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, LOGON_LOBBY_OLD)
				FM_VAR_PARM(StringToCharArray(ZoneTicket), CB_ZTS)
				END_PFM_CREATE

				pfm->verLobby = verLobby;
			pfm->crcFileList = crcFileList;
			pfm->dwTime = dwTime;

			m_Instance = pfm;
		}



		property String ^ ZoneTicket
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, ZoneTicket));
			}
		}



		property short verLobby
		{
		public:
			short get()
			{
				return m_Instance->verLobby;
			}
		}

		property int crcFileList
		{
		public:
			int get()
			{
				return m_Instance->crcFileList;
			}
		}

		property DWORD dwTime
		{
		public:
			DWORD get()
			{
				return m_Instance->dwTime;
			}
		}

	};


	[Serializable] public ref class FMD_C_LOGOFF_LOBBY : public ManagedObject<::FMD_C_LOGOFF_LOBBY>
	{
	public:

		FMD_C_LOGOFF_LOBBY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_LOGOFF_LOBBY(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, LOGOFF_LOBBY)
				END_PFM_CREATE

				m_Instance = pfm;

		}


	};


	[Serializable] public ref class FMD_L_AUTO_UPDATE_INFO : public ManagedObject<::FMD_L_AUTO_UPDATE_INFO>
	{
	public:

		FMD_L_AUTO_UPDATE_INFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_AUTO_UPDATE_INFO(

			String ^ FTPSite,
			String ^ FTPInitialDirectory,
			String ^ FTPAccount,
			String ^ FTPPassword,
			int crcFileList,
			unsigned nFileListSize
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, AUTO_UPDATE_INFO)
				FM_VAR_PARM(StringToCharArray(FTPSite), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(FTPInitialDirectory), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(FTPAccount), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(FTPPassword), CB_ZTS)
				END_PFM_CREATE

				pfm->crcFileList = crcFileList;
			pfm->nFileListSize = nFileListSize;

			m_Instance = pfm;
		}



		property String ^ FTPSite
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, FTPSite));
			}
		}




		property String ^ FTPInitialDirectory
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, FTPInitialDirectory));
			}
		}




		property String ^ FTPAccount
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, FTPAccount));
			}
		}




		property String ^ FTPPassword
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, FTPPassword));
			}
		}



		property int crcFileList
		{
		public:
			int get()
			{
				return m_Instance->crcFileList;
			}
		}

		property unsigned nFileListSize
		{
		public:
			unsigned get()
			{
				return m_Instance->nFileListSize;
			}
		}

	};


	[Serializable] public ref class FMD_C_CREATE_MISSION_REQ : public ManagedObject<::FMD_C_CREATE_MISSION_REQ>
	{
	public:

		FMD_C_CREATE_MISSION_REQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_CREATE_MISSION_REQ(

			String ^ Server,
			String ^ Address,
			String ^ IGCStaticFile,
			String ^ GameName
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, CREATE_MISSION_REQ)
				FM_VAR_PARM(StringToCharArray(Server), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(Address), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(IGCStaticFile), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(GameName), CB_ZTS)
				END_PFM_CREATE

				m_Instance = pfm;

		}



		property String ^ Server
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Server));
			}
		}




		property String ^ Address
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Address));
			}
		}




		property String ^ IGCStaticFile
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, IGCStaticFile));
			}
		}




		property String ^ GameName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, GameName));
			}
		}



	};


	[Serializable] public ref class FMD_L_CREATE_MISSION_ACK : public ManagedObject<::FMD_L_CREATE_MISSION_ACK>
	{
	public:

		FMD_L_CREATE_MISSION_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_CREATE_MISSION_ACK(

			DWORD dwCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, CREATE_MISSION_ACK)
				END_PFM_CREATE

				pfm->dwCookie = dwCookie;

			m_Instance = pfm;
		}


		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

	};


	[Serializable] public ref class FMD_L_CREATE_MISSION_NACK : public ManagedObject<::FMD_L_CREATE_MISSION_NACK>
	{
	public:

		FMD_L_CREATE_MISSION_NACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_CREATE_MISSION_NACK(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, CREATE_MISSION_NACK)
				END_PFM_CREATE

				m_Instance = pfm;
		}


	};


	[Serializable] public ref class FMD_C_JOIN_GAME_REQ : public ManagedObject<::FMD_C_JOIN_GAME_REQ>
	{
	public:

		FMD_C_JOIN_GAME_REQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_JOIN_GAME_REQ(

			DWORD dwCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, JOIN_GAME_REQ)
				END_PFM_CREATE

				pfm->dwCookie = dwCookie;

			m_Instance = pfm;
		}


		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

	};


	[Serializable] public ref class FMD_L_JOIN_GAME_NACK : public ManagedObject<::FMD_L_JOIN_GAME_NACK>
	{
	public:

		FMD_L_JOIN_GAME_NACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_JOIN_GAME_NACK(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, JOIN_GAME_NACK)
				END_PFM_CREATE

				m_Instance = pfm;

		}


	};


	[Serializable] public ref class FMD_L_JOIN_MISSION : public ManagedObject<::FMD_L_JOIN_MISSION>
	{
	public:

		FMD_L_JOIN_MISSION(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_JOIN_MISSION(

			DWORD dwCookie,
			String ^ szServer /* Originally: char szServer[64] */,
			DWORD dwPort,
			GUID guidInstance
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, JOIN_MISSION)
				END_PFM_CREATE

				pfm->dwCookie = dwCookie;
			strcpy(pfm->szServer, StringToCharArray(szServer)); /* Originally: pfm->szServer[64] = szServer[64]; */
			pfm->dwPort = dwPort;
			pfm->guidInstance = guidInstance;

			m_Instance = pfm;
		}


		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

		property String ^ szServer /* Originally: char szServer[64] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->szServer);
			}
		}

		property DWORD dwPort
		{
		public:
			DWORD get()
			{
				return m_Instance->dwPort;
			}
		}

		property GUID guidInstance
		{
		public:
			GUID get()
			{
				return m_Instance->guidInstance;
			}
		}

	};

	
	[Serializable] public ref class FMD_L_LOGON_ACK : public ManagedObject<::FMD_L_LOGON_ACK>
	{
	public:

		FMD_L_LOGON_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_LOGON_ACK(

			DWORD dwTimeOffset
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, LOGON_ACK)
				END_PFM_CREATE

				pfm->dwTimeOffset = dwTimeOffset;

			m_Instance = pfm;
		}


		property DWORD dwTimeOffset
		{
		public:
			DWORD get()
			{
				return m_Instance->dwTimeOffset;
			}
		}

	};


	[Serializable] public ref class FMD_L_LOGON_NACK : public ManagedObject<::FMD_L_LOGON_NACK>
	{
	public:

		FMD_L_LOGON_NACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_LOGON_NACK(

			String ^ Reason,
			bool fRetry
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, LOGON_NACK)
				FM_VAR_PARM(StringToCharArray(Reason), CB_ZTS)
				END_PFM_CREATE

				pfm->fRetry = fRetry;

			m_Instance = pfm;
		}



		property String ^ Reason
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Reason));
			}
		}



		property bool fRetry
		{
		public:
			bool get()
			{
				return m_Instance->fRetry;
			}
		}

	};


	[Serializable] public ref class FMD_C_FIND_PLAYER : public ManagedObject<::FMD_C_FIND_PLAYER>
	{
	public:

		FMD_C_FIND_PLAYER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_FIND_PLAYER(

			String ^ szCharacterName
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, FIND_PLAYER)
				FM_VAR_PARM(StringToCharArray(szCharacterName), CB_ZTS)
				END_PFM_CREATE


				m_Instance = pfm;

		}



		property String ^ szCharacterName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szCharacterName));
			}
		}



	};


	[Serializable] public ref class FMD_L_FOUND_PLAYER : public ManagedObject<::FMD_L_FOUND_PLAYER>
	{
	public:

		FMD_L_FOUND_PLAYER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_FOUND_PLAYER(

			DWORD dwCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, FOUND_PLAYER)
				END_PFM_CREATE

				pfm->dwCookie = dwCookie;

			m_Instance = pfm;
		}


		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

	};


	[Serializable] public ref class FMD_C_LOGON_LOBBY : public ManagedObject<::FMD_C_LOGON_LOBBY>
	{
	public:

		FMD_C_LOGON_LOBBY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_LOGON_LOBBY(

			String ^ ZoneTicket,
			String ^ ASGS_Ticket,
			short verLobby,
			int crcFileList,
			DWORD dwTime,
			String ^ szName /* Originally: char szName[c_cbName] */,
			String ^ steamAuthTicket /* Originally: int8_t steamAuthTicket[1024] */,
			uint32_t steamAuthTicketLength,
			uint64_t steamID
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, LOGON_LOBBY)
				FM_VAR_PARM(StringToCharArray(ZoneTicket), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(ASGS_Ticket), CB_ZTS)
				END_PFM_CREATE

			pfm->verLobby = verLobby;
			pfm->crcFileList = crcFileList;
			pfm->dwTime = dwTime;
			strcpy(pfm->szName, StringToCharArray(szName)); /* Originally: pfm->szName[c_cbName] = szName[c_cbName]; */
			strcpy((char *) pfm->steamAuthTicket, StringToCharArray(steamAuthTicket)); /* Originally: pfm->steamAuthTicket[1024] = steamAuthTicket[1024]; */
			pfm->steamAuthTicketLength = steamAuthTicketLength;
			pfm->steamID = steamID;

			m_Instance = pfm;
		}



		property String ^ ZoneTicket
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, ZoneTicket));
			}
		}




		property String ^ ASGS_Ticket
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, ASGS_Ticket));
			}
		}



		property short verLobby
		{
		public:
			short get()
			{
				return m_Instance->verLobby;
			}
		}

		property int crcFileList
		{
		public:
			int get()
			{
				return m_Instance->crcFileList;
			}
		}

		property DWORD dwTime
		{
		public:
			DWORD get()
			{
				return m_Instance->dwTime;
			}
		}

		property String ^ szName /* Originally: char szName[c_cbName] */
		{
		public:
			String ^ get()
			{
				return gcnew String(m_Instance->szName);
			}
		}

		property String ^ steamAuthTicket /* Originally: int8_t steamAuthTicket[1024] */
		{
		public:
			String ^ get()
			{
				return gcnew String((char *) m_Instance->steamAuthTicket);
			}
		}

		property uint32_t steamAuthTicketLength
		{
		public:
			uint32_t get()
			{
				return m_Instance->steamAuthTicketLength;
			}
		}

		property uint64_t steamID
		{
		public:
			uint64_t get()
			{
				return m_Instance->steamID;
			}
		}

	};


	[Serializable] public ref class FMD_C_GET_SERVERS_REQ : public ManagedObject<::FMD_C_GET_SERVERS_REQ>
	{
	public:

		FMD_C_GET_SERVERS_REQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_C_GET_SERVERS_REQ(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, C, GET_SERVERS_REQ)
				END_PFM_CREATE


				m_Instance = pfm;
		}


	};

	[Serializable] public ref struct ServerCoreInfo_Managed
	{
		String ^ szName;
		String ^  szRemoteAddress;
		String ^  szLocation; //it's not a filename but we want it short- keep in sync with CFLServer::m_szLocation
		int  iCurGames;
		int  iMaxGames;
		DWORD dwCoreMask; // 32 bits mask (so max is 32 cores)
	};

	[Serializable] public ref struct StaticCoreInfo_Managed
	{
		String ^ cbIGCFile;
	};

	[Serializable] public ref class FMD_L_SERVERS_LIST : public ManagedObject<::FMD_L_SERVERS_LIST>
	{
	private:
		System::Collections::Generic::List<StaticCoreInfo_Managed ^> ^ m_cores;
		System::Collections::Generic::List<ServerCoreInfo_Managed ^> ^ m_servers;

	public:

		FMD_L_SERVERS_LIST(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
			StaticCoreInfo * cores = (StaticCoreInfo *) FM_VAR_REF(m_Instance, Cores);
			ServerCoreInfo * servers = (ServerCoreInfo *) FM_VAR_REF(m_Instance, Servers);

			// Convert cores to managed.
			m_cores = gcnew System::Collections::Generic::List<StaticCoreInfo_Managed ^>();

			for (int i = 0; i < m_Instance->cCores; i++)
			{
				StaticCoreInfo_Managed ^ core = gcnew StaticCoreInfo_Managed();
				core->cbIGCFile = gcnew String(cores[i].cbIGCFile);
				m_cores->Add(core);
			}
			
			// Convert servers to managed.
			m_servers = gcnew System::Collections::Generic::List<ServerCoreInfo_Managed ^>();

			for (int i = 0; i < m_Instance->cServers; i++)
			{
				ServerCoreInfo_Managed ^ server = gcnew ServerCoreInfo_Managed();

				server->dwCoreMask = servers[i].dwCoreMask;
				server->iCurGames = servers[i].iCurGames;
				server->iMaxGames = servers[i].iMaxGames;
				server->szLocation = gcnew String(servers[i].szLocation);
				server->szName = gcnew String(servers[i].szName);
				server->szRemoteAddress = gcnew String(servers[i].szRemoteAddress);

				m_servers->Add(server);
			}
		}

		FMD_L_SERVERS_LIST(

			String ^ Cores,
			String ^ Servers,
			int cCores,
			int cServers
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, SERVERS_LIST)
				FM_VAR_PARM(StringToCharArray(Cores), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(Servers), CB_ZTS)
				END_PFM_CREATE

				pfm->cCores = cCores;
			pfm->cServers = cServers;

			m_Instance = pfm;
		}


		property System::Collections::Generic::List<StaticCoreInfo_Managed ^> ^Cores
		{
		public:
			System::Collections::Generic::List<StaticCoreInfo_Managed ^> ^ get()
			{
				return m_cores;
			}
		}

		property System::Collections::Generic::List<ServerCoreInfo_Managed ^> ^ Servers
		{
		public:
			System::Collections::Generic::List<ServerCoreInfo_Managed ^> ^ get()
			{
				return m_servers;
			}
		}

		property int cCores
		{
		public:
			int get()
			{
				return m_Instance->cCores;
			}
		}

		property int cServers
		{
		public:
			int get()
			{
				return m_Instance->cServers;
			}
		}

	};


	///////////////////////////////////////////////////////////////////////////////////////////
	// lobby\messagesall.h
	///////////////////////////////////////////////////////////////////////////////////////////


	[Serializable] public ref class FMD_LS_LOBBYMISSIONINFO : public ManagedObject<::FMD_LS_LOBBYMISSIONINFO>
	{
	public:

		FMD_LS_LOBBYMISSIONINFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_LS_LOBBYMISSIONINFO(

			String ^ szGameName,
			String ^ rgSquadIDs,
			String ^ szGameDetailsFiles,
			String ^ szIGCStaticFile,
			String ^ szServerName,
			String ^ szServerAddr,
			String ^ szPrivilegedUsers,
			String ^ szServerVersion,
			DWORD dwPort,
			DWORD dwCookie,
			unsigned dwStartTime,
			short nMinRank,
			short nMaxRank,
			unsigned nNumPlayers,
			unsigned nNumNoatPlayers,
			unsigned nMaxPlayersPerGame,
			unsigned nMinPlayersPerTeam,
			unsigned nMaxPlayersPerTeam,
			unsigned nTeams,
			bool fCountdownStarted,
			bool fInProgress,
			bool fMSArena,
			bool fScoresCount,
			bool fInvulnerableStations,
			bool fAllowDevelopments,
			bool fLimitedLives,
			bool fConquest,
			bool fDeathMatch,
			bool fCountdown,
			bool fProsperity,
			bool fArtifacts,
			bool fFlags,
			bool fTerritorial,
			bool fGuaranteedSlotsAvailable,
			bool fAnySlotsAvailable,
			bool fSquadGame,
			bool fEjectPods
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, LS, LOBBYMISSIONINFO)
				FM_VAR_PARM(StringToCharArray(szGameName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(rgSquadIDs), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szGameDetailsFiles), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szIGCStaticFile), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szServerName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szServerAddr), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szPrivilegedUsers), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szServerVersion), CB_ZTS)
				END_PFM_CREATE

				pfm->dwPort = dwPort;
			pfm->dwCookie = dwCookie;
			pfm->dwStartTime = dwStartTime;
			pfm->nMinRank = nMinRank;
			pfm->nMaxRank = nMaxRank;
			pfm->nNumPlayers = nNumPlayers;
			pfm->nNumNoatPlayers = nNumNoatPlayers;
			pfm->nMaxPlayersPerGame = nMaxPlayersPerGame;
			pfm->nMinPlayersPerTeam = nMinPlayersPerTeam;
			pfm->nMaxPlayersPerTeam = nMaxPlayersPerTeam;
			pfm->nTeams = nTeams;
			pfm->fCountdownStarted = fCountdownStarted;
			pfm->fInProgress = fInProgress;
			pfm->fMSArena = fMSArena;
			pfm->fScoresCount = fScoresCount;
			pfm->fInvulnerableStations = fInvulnerableStations;
			pfm->fAllowDevelopments = fAllowDevelopments;
			pfm->fLimitedLives = fLimitedLives;
			pfm->fConquest = fConquest;
			pfm->fDeathMatch = fDeathMatch;
			pfm->fCountdown = fCountdown;
			pfm->fProsperity = fProsperity;
			pfm->fArtifacts = fArtifacts;
			pfm->fFlags = fFlags;
			pfm->fTerritorial = fTerritorial;
			pfm->fGuaranteedSlotsAvailable = fGuaranteedSlotsAvailable;
			pfm->fAnySlotsAvailable = fAnySlotsAvailable;
			pfm->fSquadGame = fSquadGame;
			pfm->fEjectPods = fEjectPods;

			m_Instance = pfm;
		}



		property String ^ szGameName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szGameName));
			}
		}




		property String ^ rgSquadIDs
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, rgSquadIDs));
			}
		}




		property String ^ szGameDetailsFiles
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szGameDetailsFiles));
			}
		}




		property String ^ szIGCStaticFile
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szIGCStaticFile));
			}
		}




		property String ^ szServerName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szServerName));
			}
		}




		property String ^ szServerAddr
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szServerAddr));
			}
		}




		property String ^ szPrivilegedUsers
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szPrivilegedUsers));
			}
		}




		property String ^ szServerVersion
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szServerVersion));
			}
		}



		property DWORD dwPort
		{
		public:
			DWORD get()
			{
				return m_Instance->dwPort;
			}
		}

		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

		property unsigned dwStartTime
		{
		public:
			unsigned get()
			{
				return m_Instance->dwStartTime;
			}
		}

		property short nMinRank
		{
		public:
			short get()
			{
				return m_Instance->nMinRank;
			}
		}

		property short nMaxRank
		{
		public:
			short get()
			{
				return m_Instance->nMaxRank;
			}
		}

		property unsigned nNumPlayers
		{
		public:
			unsigned get()
			{
				return m_Instance->nNumPlayers;
			}
		}

		property unsigned nNumNoatPlayers
		{
		public:
			unsigned get()
			{
				return m_Instance->nNumNoatPlayers;
			}
		}

		property unsigned nMaxPlayersPerGame
		{
		public:
			unsigned get()
			{
				return m_Instance->nMaxPlayersPerGame;
			}
		}

		property unsigned nMinPlayersPerTeam
		{
		public:
			unsigned get()
			{
				return m_Instance->nMinPlayersPerTeam;
			}
		}

		property unsigned nMaxPlayersPerTeam
		{
		public:
			unsigned get()
			{
				return m_Instance->nMaxPlayersPerTeam;
			}
		}

		property unsigned nTeams
		{
		public:
			unsigned get()
			{
				return m_Instance->nTeams;
			}
		}

		property bool fCountdownStarted
		{
		public:
			bool get()
			{
				return m_Instance->fCountdownStarted;
			}
		}

		property bool fInProgress
		{
		public:
			bool get()
			{
				return m_Instance->fInProgress;
			}
		}

		property bool fMSArena
		{
		public:
			bool get()
			{
				return m_Instance->fMSArena;
			}
		}

		property bool fScoresCount
		{
		public:
			bool get()
			{
				return m_Instance->fScoresCount;
			}
		}

		property bool fInvulnerableStations
		{
		public:
			bool get()
			{
				return m_Instance->fInvulnerableStations;
			}
		}

		property bool fAllowDevelopments
		{
		public:
			bool get()
			{
				return m_Instance->fAllowDevelopments;
			}
		}

		property bool fLimitedLives
		{
		public:
			bool get()
			{
				return m_Instance->fLimitedLives;
			}
		}

		property bool fConquest
		{
		public:
			bool get()
			{
				return m_Instance->fConquest;
			}
		}

		property bool fDeathMatch
		{
		public:
			bool get()
			{
				return m_Instance->fDeathMatch;
			}
		}

		property bool fCountdown
		{
		public:
			bool get()
			{
				return m_Instance->fCountdown;
			}
		}

		property bool fProsperity
		{
		public:
			bool get()
			{
				return m_Instance->fProsperity;
			}
		}

		property bool fArtifacts
		{
		public:
			bool get()
			{
				return m_Instance->fArtifacts;
			}
		}

		property bool fFlags
		{
		public:
			bool get()
			{
				return m_Instance->fFlags;
			}
		}

		property bool fTerritorial
		{
		public:
			bool get()
			{
				return m_Instance->fTerritorial;
			}
		}

		property bool fGuaranteedSlotsAvailable
		{
		public:
			bool get()
			{
				return m_Instance->fGuaranteedSlotsAvailable;
			}
		}

		property bool fAnySlotsAvailable
		{
		public:
			bool get()
			{
				return m_Instance->fAnySlotsAvailable;
			}
		}

		property bool fSquadGame
		{
		public:
			bool get()
			{
				return m_Instance->fSquadGame;
			}
		}

		property bool fEjectPods
		{
		public:
			bool get()
			{
				return m_Instance->fEjectPods;
			}
		}

	};


	[Serializable] public ref class FMD_LS_MISSION_GONE : public ManagedObject<::FMD_LS_MISSION_GONE>
	{
	public:

		FMD_LS_MISSION_GONE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_LS_MISSION_GONE(

			DWORD dwCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, LS, MISSION_GONE)
				END_PFM_CREATE

				pfm->dwCookie = dwCookie;

			m_Instance = pfm;
		}


		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

	};


	[Serializable] public ref class FMD_LS_SQUAD_MEMBERSHIPS : public ManagedObject<::FMD_LS_SQUAD_MEMBERSHIPS>
	{
	public:

		FMD_LS_SQUAD_MEMBERSHIPS(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_LS_SQUAD_MEMBERSHIPS(

			String ^ squadMemberships,
			int cSquadMemberships
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, LS, SQUAD_MEMBERSHIPS)
				FM_VAR_PARM(StringToCharArray(squadMemberships), CB_ZTS)
				END_PFM_CREATE

				pfm->cSquadMemberships = cSquadMemberships;

			m_Instance = pfm;
		}



		property String ^ squadMemberships
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, squadMemberships));
			}
		}



		property int cSquadMemberships
		{
		public:
			int get()
			{
				return m_Instance->cSquadMemberships;
			}
		}

	};


	// From: messagesls.h

	[Serializable] public ref class FMD_S_LOGON_LOBBY : public ManagedObject<::FMD_S_LOGON_LOBBY>
	{
	public:

		FMD_S_LOGON_LOBBY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_LOGON_LOBBY(

			String ^ vStaticCoreInfo,
			String ^ szLocation,
			String ^ szPrivilegedUsers,
			String ^ szServerVersion,
			int cStaticCoreInfo,
			int MaxGames,
			short verLobby,
			DWORD dwPort,
			int CurGames
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LOGON_LOBBY)
				FM_VAR_PARM(StringToCharArray(vStaticCoreInfo), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szLocation), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szPrivilegedUsers), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szServerVersion), CB_ZTS)
				END_PFM_CREATE

				pfm->cStaticCoreInfo = cStaticCoreInfo;
			pfm->MaxGames = MaxGames;
			pfm->verLobby = verLobby;
			pfm->dwPort = dwPort;
			pfm->CurGames = CurGames;

			this->m_Instance = pfm;
		}



		property String ^ vStaticCoreInfo
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, vStaticCoreInfo));
			}
		}




		property String ^ szLocation
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szLocation));
			}
		}




		property String ^ szPrivilegedUsers
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szPrivilegedUsers));
			}
		}




		property String ^ szServerVersion
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szServerVersion));
			}
		}



		property int cStaticCoreInfo
		{
		public:
			int get()
			{
				return m_Instance->cStaticCoreInfo;
			}
		}

		property int MaxGames
		{
		public:
			int get()
			{
				return m_Instance->MaxGames;
			}
		}

		property short verLobby
		{
		public:
			short get()
			{
				return m_Instance->verLobby;
			}
		}

		property DWORD dwPort
		{
		public:
			DWORD get()
			{
				return m_Instance->dwPort;
			}
		}

		property int CurGames
		{
		public:
			int get()
			{
				return m_Instance->CurGames;
			}
		}

	};


	[Serializable] public ref class FMD_S_LOGOFF_LOBBY : public ManagedObject<::FMD_S_LOGOFF_LOBBY>
	{
	public:

		FMD_S_LOGOFF_LOBBY(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_LOGOFF_LOBBY(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LOGOFF_LOBBY)
				END_PFM_CREATE


				this->m_Instance = pfm;
		}


	};


	[Serializable] public ref class FMD_L_CREATE_MISSION_REQ : public ManagedObject<::FMD_L_CREATE_MISSION_REQ>
	{
	public:

		FMD_L_CREATE_MISSION_REQ(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_CREATE_MISSION_REQ(

			String ^ GameName,
			String ^ IGCStaticFile,
			DWORD dwCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, CREATE_MISSION_REQ)
				FM_VAR_PARM(StringToCharArray(GameName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(IGCStaticFile), CB_ZTS)
				END_PFM_CREATE

				pfm->dwCookie = dwCookie;

			this->m_Instance = pfm;
		}



		property String ^ GameName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, GameName));
			}
		}




		property String ^ IGCStaticFile
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, IGCStaticFile));
			}
		}



		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

	};

	// TODO: Sort this one out if needed.
	//[Serializable] public ref class FMD_S_NEW_MISSION : public ManagedObject<::FMD_S_NEW_MISSION>
	//{
	//public:

	//	FMD_S_NEW_MISSION(array<System::Byte> ^ bytes) : ManagedObject(bytes)
	//	{
	//	}

	//	FMD_S_NEW_MISSION(

	//		// We need to get this cookie translated into a lobby cookie--see below DWORD dwIGCMissionID
	//	)
	//	{
	//		SimpleClient sc;
	//		FedMessaging fm(&sc);

	//		BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, NEW_MISSION)
	//			END_PFM_CREATE

	//			pfm->DWORD dwIGCMissionID = DWORD dwIGCMissionID;

	//		this->m_Instance = pfm;
	//	}


	//	property // We need to get this cookie translated into a lobby cookie--see below DWORD dwIGCMissionID
	//	{
	//	public:
	//		// We need to get this cookie translated into a lobby cookie--see below get()
	//	{
	//		return m_Instance->DWORD dwIGCMissionID;
	//	}
	//	}

	//};


	[Serializable] public ref class FMD_L_NEW_MISSION_ACK : public ManagedObject<::FMD_L_NEW_MISSION_ACK>
	{
	public:

		FMD_L_NEW_MISSION_ACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_NEW_MISSION_ACK(

			// E.g., object model-created. no one will be able to join until the server gets this DWORD dwIGCMissionID,
			DWORD dwCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, NEW_MISSION_ACK)
				END_PFM_CREATE

				pfm->dwIGCMissionID = dwIGCMissionID;
			pfm->dwCookie = dwCookie;

			this->m_Instance = pfm;
		}


		property DWORD dwIGCMissionID // E.g., object model-created. no one will be able to join until the server gets this DWORD dwIGCMissionID
		{
		public:
			DWORD  get() // E.g., object model-created. no one will be able to join until the server gets this get()
		{
			return m_Instance->dwIGCMissionID;
		}
		}

		property DWORD dwCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwCookie;
			}
		}

	};


	[Serializable] public ref class FMD_S_HEARTBEAT : public ManagedObject<::FMD_S_HEARTBEAT>
	{
	public:

		FMD_S_HEARTBEAT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_HEARTBEAT(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, HEARTBEAT)
				END_PFM_CREATE


				this->m_Instance = pfm;
		}


	};


	[Serializable] public ref class FMD_L_QUIT : public ManagedObject<::FMD_L_QUIT>
	{
	public:

		FMD_L_QUIT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_QUIT(

		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, QUIT)
				END_PFM_CREATE


				this->m_Instance = pfm;
		}


	};


	[Serializable] public ref class FMD_L_TOKEN : public ManagedObject<::FMD_L_TOKEN>
	{
	public:

		FMD_L_TOKEN(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_TOKEN(

			String ^ Token
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, TOKEN)
				FM_VAR_PARM(StringToCharArray(Token), CB_ZTS)
				END_PFM_CREATE


				this->m_Instance = pfm;
		}



		property String ^ Token
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Token));
			}
		}



	};


	[Serializable] public ref class FMD_S_PLAYER_JOINED : public ManagedObject<::FMD_S_PLAYER_JOINED>
	{
	public:

		FMD_S_PLAYER_JOINED(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_PLAYER_JOINED(

			String ^ szCharacterName,
			String ^ szCDKey,
			String ^ szAddress,
			DWORD dwMissionCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PLAYER_JOINED)
				FM_VAR_PARM(StringToCharArray(szCharacterName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szCDKey), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szAddress), CB_ZTS)
				END_PFM_CREATE

				pfm->dwMissionCookie = dwMissionCookie;

			this->m_Instance = pfm;
		}



		property String ^ szCharacterName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szCharacterName));
			}
		}




		property String ^ szCDKey
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szCDKey));
			}
		}




		property String ^ szAddress
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szAddress));
			}
		}



		property DWORD dwMissionCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwMissionCookie;
			}
		}

	};


	[Serializable] public ref class FMD_S_PLAYER_QUIT : public ManagedObject<::FMD_S_PLAYER_QUIT>
	{
	public:

		FMD_S_PLAYER_QUIT(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_PLAYER_QUIT(

			String ^ szCharacterName,
			DWORD dwMissionCookie
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PLAYER_QUIT)
				FM_VAR_PARM(StringToCharArray(szCharacterName), CB_ZTS)
				END_PFM_CREATE

				pfm->dwMissionCookie = dwMissionCookie;

			this->m_Instance = pfm;
		}



		property String ^ szCharacterName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szCharacterName));
			}
		}



		property DWORD dwMissionCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwMissionCookie;
			}
		}

	};


	[Serializable] public ref class FMD_L_REMOVE_PLAYER : public ManagedObject<::FMD_L_REMOVE_PLAYER>
	{
	public:

		FMD_L_REMOVE_PLAYER(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_REMOVE_PLAYER(

			String ^ szCharacterName,
			String ^ szMessageParam,
			DWORD dwMissionCookie,
			RemovePlayerReason reason
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, REMOVE_PLAYER)
				FM_VAR_PARM(StringToCharArray(szCharacterName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(szMessageParam), CB_ZTS)
				END_PFM_CREATE

				pfm->dwMissionCookie = dwMissionCookie;
			pfm->reason = reason;

			this->m_Instance = pfm;
		}



		property String ^ szCharacterName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szCharacterName));
			}
		}




		property String ^ szMessageParam
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, szMessageParam));
			}
		}



		property DWORD dwMissionCookie
		{
		public:
			DWORD get()
			{
				return m_Instance->dwMissionCookie;
			}
		}

		property RemovePlayerReason reason
		{
		public:
			RemovePlayerReason get()
			{
				return m_Instance->reason;
			}
		}

	};


	[Serializable] public ref class FMD_S_PAUSE : public ManagedObject<::FMD_S_PAUSE>
	{
	public:

		FMD_S_PAUSE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_PAUSE(

			bool fPause
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, PAUSE)
				END_PFM_CREATE

				pfm->fPause = fPause;

			this->m_Instance = pfm;
		}


		property bool fPause
		{
		public:
			bool get()
			{
				return m_Instance->fPause;
			}
		}

	};


	[Serializable] public ref class FMD_L_LOGON_SERVER_NACK : public ManagedObject<::FMD_L_LOGON_SERVER_NACK>
	{
	public:

		FMD_L_LOGON_SERVER_NACK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_LOGON_SERVER_NACK(

			String ^ Reason
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, LOGON_SERVER_NACK)
				FM_VAR_PARM(StringToCharArray(Reason), CB_ZTS)
				END_PFM_CREATE


				this->m_Instance = pfm;
		}



		property String ^ Reason
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Reason));
			}
		}



	};


	[Serializable] public ref class FMD_LS_PLAYER_RANK : public ManagedObject<::FMD_LS_PLAYER_RANK>
	{
	public:

		FMD_LS_PLAYER_RANK(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_LS_PLAYER_RANK(

			// These items will also be returned from the lobby server. int rank
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, LS, PLAYER_RANK)
				END_PFM_CREATE

				pfm->rank = rank;

			this->m_Instance = pfm;
		}


		property int rank // These items will also be returned from the lobby server. int rank
		{
		public:
			int get() // These items will also be returned from the lobby server. get()
		{
			return m_Instance->rank;
		}
		}

	};


	[Serializable] public ref class FMD_L_UPDATE_DRM_HASHES : public ManagedObject<::FMD_L_UPDATE_DRM_HASHES>
	{
	public:

		FMD_L_UPDATE_DRM_HASHES(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_L_UPDATE_DRM_HASHES(

			String ^ DrmDownloadUrl
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, L, UPDATE_DRM_HASHES)
				FM_VAR_PARM(StringToCharArray(DrmDownloadUrl), CB_ZTS)
				END_PFM_CREATE


				this->m_Instance = pfm;
		}



		property String ^ DrmDownloadUrl
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, DrmDownloadUrl));
			}
		}



	};


	[Serializable] public ref class FMD_S_LOG_CHAT_MESSAGE : public ManagedObject<::FMD_S_LOG_CHAT_MESSAGE>
	{
	public:

		FMD_S_LOG_CHAT_MESSAGE(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
		}

		FMD_S_LOG_CHAT_MESSAGE(

			String ^ MissionName,
			String ^ SourceName,
			String ^ TargetName,
			String ^ Message,
			String ^ SourceIP,
			uint64_t sourceSteamID,
			uint64_t targetSteamID
		)
		{
			SimpleClient sc;
			FedMessaging fm(&sc);

			BEGIN_PFM_CREATE_ALLOC(fm, pfm, S, LOG_CHAT_MESSAGE)
				FM_VAR_PARM(StringToCharArray(MissionName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(SourceName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(TargetName), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(Message), CB_ZTS)
				FM_VAR_PARM(StringToCharArray(SourceIP), CB_ZTS)
				END_PFM_CREATE

				pfm->sourceSteamID = sourceSteamID;
			pfm->targetSteamID = targetSteamID;

			this->m_Instance = pfm;
		}



		property String ^ MissionName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, MissionName));
			}
		}




		property String ^ SourceName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, SourceName));
			}
		}




		property String ^ TargetName
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, TargetName));
			}
		}




		property String ^ Message
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, Message));
			}
		}




		property String ^ SourceIP
		{
		public:
			String ^ get()
			{
				return gcnew String(FM_VAR_REF(m_Instance, SourceIP));
			}
		}



		property uint64_t sourceSteamID
		{
		public:
			uint64_t get()
			{
				return m_Instance->sourceSteamID;
			}
		}

		property uint64_t targetSteamID
		{
		public:
			uint64_t get()
			{
				return m_Instance->targetSteamID;
			}
		}

	};


	[Serializable] public ref class WarpInfo
	{
	private:
		ClusterInfo ^ m_fromCluster;
		ClusterInfo ^ m_toCluster;

	public:
		WarpInfo(ClusterInfo ^ fromCluster, ClusterInfo ^ toCluster)
		{
			m_fromCluster = fromCluster;
			m_toCluster = toCluster;
		}

		property ClusterInfo ^ FromCluster
		{
		public:
			ClusterInfo ^ get()
			{
				return m_fromCluster;
			}
			void set(ClusterInfo ^ value)
			{
				m_fromCluster = value;
			}
		}

		property ClusterInfo ^ ToCluster
		{
		public:
			ClusterInfo ^ get()
			{
				return m_toCluster;
			}
			void set(ClusterInfo ^ value)
			{
				m_toCluster = value;
			}
		}


		ClusterInfo ^ GetCluster() { return m_fromCluster; }
		ClusterInfo ^ GetDestinationCluster() { return m_toCluster; }
	};


	[Serializable] public ref class ClusterInfo
	{
	private: 
		SectorID m_clusterID;
		bool m_isHomeSector;
		System::Collections::Generic::List<WarpInfo ^> ^ m_warps = gcnew System::Collections::Generic::List<WarpInfo ^>();

	public:
		ClusterInfo(SectorID clusterID, bool isHomeSector)
		{
			m_clusterID = clusterID;
			m_isHomeSector = isHomeSector;
		}

		property System::Collections::Generic::List<WarpInfo ^> ^ Warps
		{
		public:
			System::Collections::Generic::List<WarpInfo ^> ^ get()
			{
				return m_warps;
			}
			void set(System::Collections::Generic::List<WarpInfo ^> ^ value)
			{
				m_warps = value;
			}
		}

		property SectorID ClusterID
		{
		public:
			SectorID get()
			{
				return m_clusterID;
			}
			void set(SectorID value)
			{
				m_clusterID = value;
			}
		}

		property bool IsHomeSector
		{
		public:
			bool get()
			{
				return m_isHomeSector;
			}
			void set(bool value)
			{
				m_isHomeSector = value;
			}
		}

		SectorID GetObjectID() { return m_clusterID; }
		bool GetHomeSector() { return m_isHomeSector; }
		System::Collections::Generic::List<WarpInfo ^> ^ GetWarps() { return m_warps; }
	};



	[Serializable] public ref class FMD_S_CLUSTERINFO : public ManagedObject<::FMD_S_CLUSTERINFO>
	{
	private: 
		System::Collections::Generic::List<ClusterInfo ^> ^ m_clusterInfos;

	public:

		FMD_S_CLUSTERINFO(array<System::Byte> ^ bytes) : ManagedObject(bytes)
		{
			m_clusterInfos = gcnew System::Collections::Generic::List<ClusterInfo ^>();

			for (int i = 0; i < sizeof(m_Instance->clusterIDs) / sizeof(SectorID) && m_Instance->clusterIDs[i] != -1; i++)
			{
				m_clusterInfos->Add(gcnew ClusterInfo(m_Instance->clusterIDs[i], m_Instance->homeSectors[i]));
			}

			for (int sourceClusterIndex = 0; sourceClusterIndex < m_clusterInfos->Count; sourceClusterIndex++)
			{
				for (int warpIndex = 0; warpIndex < sizeof(m_Instance->warpFromClusterIDs) / sizeof(SectorID) && m_Instance->warpFromClusterIDs[warpIndex] != -1; warpIndex++)
				{
					if (m_Instance->warpFromClusterIDs[warpIndex] == m_clusterInfos[sourceClusterIndex]->GetObjectID())
					{
						for (int destinationClusterIndex = 0; destinationClusterIndex < m_clusterInfos->Count; destinationClusterIndex++)
						{
							if (m_clusterInfos[destinationClusterIndex]->GetObjectID() == m_Instance->warpToClusterIDs[warpIndex])
							{
								m_clusterInfos[sourceClusterIndex]->GetWarps()->Add(gcnew WarpInfo(m_clusterInfos[sourceClusterIndex], m_clusterInfos[destinationClusterIndex]));
								break;
							}
						}
					}
				}
			}
		}

		property System::Collections::Generic::List<ClusterInfo ^> ^ Clusters
		{
		public:
			System::Collections::Generic::List<ClusterInfo ^> ^ get()
			{
				return m_clusterInfos;
			}
		}

	};


	
}
