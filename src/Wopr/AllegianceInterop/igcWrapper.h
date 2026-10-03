
#pragma once

#include "igc.h"
#include "clintlib.h"
#include "Enums.h"

// Use this to convert the unmanaged pointer into a wrapped object. In the unmanaged pointer is nullptr, then 
// a null object will be returned instead of a wrapper on a null object.
#define ConvertTo(ManagedType, UnmanagedObject) \
	if(UnmanagedObject == nullptr) return nullptr; ManagedType ^ value = gcnew ManagedType(UnmanagedObject); if(value->m_instance == nullptr) return nullptr; else return value;

#define ConvertSList(IGC_OBJECT, IGC_LINK, NATIVE_CALL) \
	System::Collections::Generic::List<##IGC_OBJECT##Wrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<##IGC_OBJECT##Wrapper ^>(); \
\
	if(NATIVE_CALL != nullptr) \
	{ \
		for (IGC_LINK* pbl = NATIVE_CALL->last(); (pbl != NULL); pbl = pbl->txen())  \
		{  \
			IGC_OBJECT*     item = pbl->data();  \
			returnValue->Add(gcnew IGC_OBJECT##Wrapper(item));  \
		}  \
	} \
	return returnValue;\

#define GenericList(ManagedType) System::Collections::Generic::List<ManagedType ^> ^ 

#define ConvertToGenericList(IGC_OBJECT, METHOD_NAME, IGC_LINK) \
	System::Collections::Generic::List<IGC_OBJECT##Wrapper ^> ^ METHOD_NAME##() \
	{ \
		System::Collections::Generic::List<##IGC_OBJECT##Wrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<AllegianceInterop::##IGC_OBJECT##Wrapper ^>(); \
	\
		for (IGC_LINK* pbl = m_instance->##METHOD_NAME##()->last(); (pbl != NULL); pbl = pbl->txen())  \
		{  \
			IGC_OBJECT*     item = pbl->data();  \
			returnValue->Add(gcnew IGC_OBJECT##Wrapper(item));  \
		}  \
		return returnValue;\
	}\


using namespace System;

namespace AllegianceInterop
{


    ref class GeoWrapper;
    ref class MissionParamsWrapper;
    ref class ImapMakerIGCWrapper;
    ref class MissionParamsWrapper;
    ref class ShipLoadoutWrapper;
    ref class CompactOrientationWrapper;
    ref class CompactTimeWrapper;
    ref class CompactPositionWrapper;
    ref class CompactStateWrapper;
    ref class CompactVelocityWrapper;
    ref class CompactTurnRatesWrapper;
    ref class CompactControlDataWrapper;
    ref class CompactShipFractionsWrapper;
    ref class ServerLightShipUpdateWrapper;
    ref class ServerHeavyShipUpdateWrapper;
    ref class ServerSingleShipUpdateWrapper;
    ref class ClientShipUpdateWrapper;
    ref class ClientActiveTurretUpdateWrapper;
    ref class ServerActiveTurretUpdateWrapper;
    ref class IstaticIGCWrapper;
    ref class ImissionIGCWrapper;
    ref class IbaseIGCWrapper;
    ref class ItypeIGCWrapper;
    ref class AttachSiteWrapper;
    ref class ThingSiteWrapper;
    ref class ImodelIGCWrapper;
    ref class IdamageIGCWrapper;
    ref class IscannerIGCWrapper;
    ref class IshipIGCWrapper;
    ref class IbuoyIGCWrapper;
    ref class IchaffIGCWrapper;
    ref class IprojectileIGCWrapper;
    ref class ImissileIGCWrapper;
    ref class ImineIGCWrapper;
    ref class IbuildingEffectIGCWrapper;
    ref class IprobeIGCWrapper;
    ref class IstationIGCWrapper;
    ref class IbuyableIGCWrapper;
    ref class IdevelopmentIGCWrapper;
    ref class IdroneTypeIGCWrapper;
    ref class IstationTypeIGCWrapper;
    ref class IbucketIGCWrapper;
    ref class IprojectileTypeIGCWrapper;
    ref class IexpendableTypeIGCWrapper;
    ref class ImissileTypeIGCWrapper;
    ref class ImineTypeIGCWrapper;
    ref class IchaffTypeIGCWrapper;
    ref class IprobeTypeIGCWrapper;
    ref class IhullTypeIGCWrapper;
    ref class IpartTypeIGCWrapper;
    ref class IlauncherTypeIGCWrapper;
    ref class IpartIGCWrapper;
    ref class IweaponIGCWrapper;
    ref class IshieldIGCWrapper;
    ref class IcloakIGCWrapper;
    ref class IafterburnerIGCWrapper;
    ref class IpackIGCWrapper;
    ref class IlauncherIGCWrapper;
    ref class ImagazineIGCWrapper;
    ref class IdispenserIGCWrapper;
    ref class IclusterIGCWrapper;
    ref class IasteroidIGCWrapper;
    ref class IwarpIGCWrapper;
    ref class ItreasureIGCWrapper;
    ref class IsideIGCWrapper;
    ref class IcivilizationIGCWrapper;
    ref class ItreasureSetIGCWrapper;
    ref class ShipStatusWrapper;
    ref class ClusterSiteWrapper;
    ref class SideVisibilityWrapper;
    ref class GameSiteWrapper;
    ref class IIgcSiteWrapper;
    ref class DamageTrackWrapper;
    ref class DamageBucketWrapper;
    ref class DamageTrackSetWrapper;
    ref class WaypointWrapper;
    ref class GotoPlanWrapper;
    ref class PersistPlayerScoreObjectWrapper;
    ref class PlayerScoreObjectWrapper;
    ref class GameOverScoreObjectWrapper;
	
	public ref class PlayerInfoWrapper
	{
	public:
		PlayerInfo * m_instance = nullptr;

		PlayerInfoWrapper(PlayerInfo * instance)
		{
			m_instance = instance;
		}

		ShipID LastSeenParent() { return m_instance->LastSeenParent(); }
		SectorID LastSeenSector() { return m_instance->LastSeenSector(); }
		HullID LastSeenShipType() { return m_instance->LastSeenShipType(); }
		ShipState LastSeenState() { return m_instance->LastSeenState(); }
		StationID LastSeenStation() { return m_instance->LastSeenStation(); }
		DWORD LastStateChange() { return m_instance->LastStateChange(); }
		Money GetMoney() { return m_instance->GetMoney(); }
	};

	public ref class VectorWrapper
	{
	public:
		::Vector * m_instance = nullptr;

		VectorWrapper(::Vector instance)
		{
			m_instance = new Vector();
			*m_instance = instance;
		}

		~VectorWrapper()
		{
			if (m_instance != nullptr)
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}

		!VectorWrapper()
		{
			if (m_instance != nullptr)
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}

		VectorWrapper(float x, float y, float z) : VectorWrapper(Vector(x, y, z))
		{
		}

		static const VectorWrapper ^ GetZero() { return gcnew VectorWrapper(::Vector::GetZero()); }

		float X() { return m_instance->X(); }
		float Y() { return m_instance->Y(); }
		float Z() { return m_instance->Z(); }

		VectorWrapper ^ SetX(float xArg) { return gcnew VectorWrapper(m_instance->SetX(xArg)); }
		VectorWrapper ^ SetY(float yArg) { return gcnew VectorWrapper(m_instance->SetY(yArg)); }
		VectorWrapper ^ SetZ(float zArg) { return gcnew VectorWrapper(m_instance->SetZ(zArg));  }

		bool IsZero() 
		{
			return m_instance->IsZero();
		}

		float LengthSquared() 
		{
			return m_instance->LengthSquared();
		}

		float Length()
		{
			return m_instance->Length();
		}

		double AngleInRadians(VectorWrapper ^ otherPoint)
		{
			if (X() == 0 && Y() == 0)
				return 0;

			return atan2(Y(), X());

			//return atan((double) (otherPoint->X() - X()), (double) (otherPoint->Y() - Y()));

			//return acos(DotProduct(otherPoint) / (this->Length() * otherPoint->Length()));
		}

		float DotProduct(VectorWrapper ^ otherPoint)
		{
			return (this->X() * otherPoint->X()) + (this->X() * otherPoint->Y()) + (this->X() * otherPoint->Z());
		}

		String ^ GetString()
		{
			return gcnew String((PCC) m_instance->GetString());
		}

		VectorWrapper ^ operator+=(VectorWrapper ^ v)
		{
			return gcnew VectorWrapper(*m_instance += *v->m_instance);
		}

		VectorWrapper ^ operator-=(VectorWrapper ^ v)
		{
			return gcnew VectorWrapper(*m_instance -= *v->m_instance);
		}

		VectorWrapper ^ operator*=(float s)
		{
			return gcnew VectorWrapper(*m_instance *= s);
		}

		VectorWrapper ^ operator/=(float s)
		{
			return gcnew VectorWrapper(*m_instance /= s);
		}

		float   operator [](int i)
		{
			return (*m_instance)[i];
		}

		VectorWrapper ^ Normalize() 
		{
			return gcnew VectorWrapper(m_instance->Normalize());
		}

		void SetNormalize()
		{
			m_instance->SetNormalize();
		}

		//
		// friend functions
		//

		static bool operator ==(VectorWrapper ^ v1, VectorWrapper ^ v2)
		{
			return v1->m_instance == v2->m_instance;
		}

		static bool operator!=(const VectorWrapper ^ v1, const VectorWrapper ^ v2)
		{
			return !(v1 == v2);
		}

		static VectorWrapper ^ operator-(const VectorWrapper ^ v)
		{
			return gcnew VectorWrapper(-(*v->m_instance));		}

		static VectorWrapper ^ operator+(const VectorWrapper ^ v1, const VectorWrapper ^ v2)
		{
			return gcnew VectorWrapper(*v1->m_instance + *v2->m_instance);
		}

		static VectorWrapper ^ operator-(const VectorWrapper ^ v1, const VectorWrapper ^ v2)
		{
			return gcnew VectorWrapper(*v1->m_instance - *v2->m_instance);
		}

		static VectorWrapper ^ operator*(const VectorWrapper ^ v1, float s)
		{
			return gcnew VectorWrapper(*v1->m_instance * s);
		}

		static VectorWrapper ^ operator*(float s, const VectorWrapper ^ v1)
		{
			return v1 * s;
		}

		static VectorWrapper ^ operator/(const VectorWrapper ^ v1, float s)
		{
			return gcnew VectorWrapper(*v1->m_instance / s);
		}

		static float operator*(const VectorWrapper ^ v1, const VectorWrapper ^ v2)
		{
			return *v1->m_instance * *v2->m_instance;
		}

		static VectorWrapper ^ CrossProduct(const VectorWrapper ^ v1, const VectorWrapper ^ v2)
		{
			return
				gcnew VectorWrapper(Vector(
				(v1->m_instance->Y() * v2->m_instance->Z()) - (v2->m_instance->Y() * v1->m_instance->Z()),
					(v1->m_instance->Z() * v2->m_instance->X()) - (v2->m_instance->Z() * v1->m_instance->X()),
					(v1->m_instance->X() * v2->m_instance->Y()) - (v2->m_instance->X() * v1->m_instance->Y())
				));
		}

		/*static GetOrthogonalVector() const;

		static static RandomDirection(void);
		static static RandomPosition(float radius);*/
	};


    public ref class GeoWrapper
	{
	public:
		::Geo * m_instance;

	public:
		
		GeoWrapper(::Geo * instance);

    };

 //   public ref class MissionParamsWrapper
	//{
	//public:
	//	::MissionParams * m_instance;

	//public:
	//	
	//	MissionParamsWrapper(::MissionParams * instance);

 //   };

    public ref class ImapMakerIGCWrapper
	{
	public:
		::ImapMakerIGC * m_instance;

	public:
		
		ImapMakerIGCWrapper(::ImapMakerIGC * instance);
/*
        String ^ IsValid(MissionParamsWrapper ^ pmp);*/
        MapMakerID  GetMapMakerID();
        VOID        SetMapMakerID(MapMakerID mbID);
        String ^ GetName();
        void        SetName(const char* newVal);
    };

    public ref class MissionParamsWrapper
	{
	public:
		::MissionParams * m_instance;

	public:
		
		MissionParamsWrapper(::MissionParams * instance);

        void    Reset();
        String ^ Invalid(bool bFromClient);
        bool IsConquestGame();
        bool IsTerritoryGame();
        bool IsDeathMatchGame();
        bool IsCountdownGame();
        bool IsProsperityGame();
        bool IsArtifactsGame();
        bool IsFlagsGame();
        bool IsNoWinningConditionsGame();
        float GetCountDownTime();
        int GetConquestPercentage();
        int GetTerritoryPercentage();
        short GetDeathMatchKillLimit();
        int GetArtifactsPercentage();
		int GetMaxDrones() { return m_instance->nMaxDronesPerTeam; }
    };

    public ref class ShipLoadoutWrapper
	{
	public:
		::ShipLoadout * m_instance;

	public:
		
		ShipLoadoutWrapper(::ShipLoadout * instance);

        ExpandedPartData*   PartData0();
        ExpandedPartData*   PartDataN(short cb);
    };

    public ref class CompactOrientationWrapper
	{
	public:
		::CompactOrientation * m_instance;

	public:
		
		CompactOrientationWrapper(::CompactOrientation * instance);

        void    Set(const Orientation& orientation);
        void    Export(Orientation* pOrientation);
        void    Validate();
    };

    public ref class CompactTimeWrapper
	{
	public:
		::CompactTime * m_instance;

	public:
		
		CompactTimeWrapper(::CompactTime * instance);

        void        Set(Time timeBase, Time timeNow);
        void        Export(Time timeBase, Time* timeNow);
    };

    public ref class CompactPositionWrapper
	{
	public:
		::CompactPosition * m_instance;

	public:
		
		CompactPositionWrapper(::CompactPosition * instance);
/*
        static  float   Displacement(short delta);*//*
        static short    Delta(float displacement);*/
        void        Set(const VectorWrapper ^ pReference, const VectorWrapper ^ p);
        void        Export(const VectorWrapper ^ pReference, Vector* p);
    };

    public ref class CompactStateWrapper
	{
	public:
		::CompactState * m_instance;

	public:
		
		CompactStateWrapper(::CompactState * instance);

        void        Set(int stateM);
        void        Export(int* pStateM);
    };

    public ref class CompactVelocityWrapper
	{
	public:
		::CompactVelocity * m_instance;

	public:
		
		CompactVelocityWrapper(::CompactVelocity * instance);

        void            Set(const VectorWrapper ^ v);
        void            Export(Vector* pVelocity);
    };

    public ref class CompactTurnRatesWrapper
	{
	public:
		::CompactTurnRates * m_instance;

	public:
		
		CompactTurnRatesWrapper(::CompactTurnRates * instance);

        void        Set(const float turnRates[3]);
        void            Export(float turnRates[3]);
    };

    public ref class CompactControlDataWrapper
	{
	public:
		::CompactControlData * m_instance;

	public:
		
		CompactControlDataWrapper(::CompactControlData * instance);

        void        Set(const ControlData& cd);
        void            Export(ControlData* cd);
    };

    public ref class CompactShipFractionsWrapper
	{
	public:
		::CompactShipFractions * m_instance;

	public:
		
		CompactShipFractionsWrapper(::CompactShipFractions * instance);

        void        SetHullFraction(float hf);
        float        GetHullFraction();
        void        SetShieldFraction(float sf);
        float        GetShieldFraction();
        void        SetFuel(float maxFuel, float fuel);
        float       GetFuel(float maxFuel);
        void        SetAmmo(short maxAmmo, short ammo);
        short       GetAmmo(short maxAmmo);
        void        SetEnergy(float maxEnergy, float energy);
        float        GetEnergy(float maxEnergy);
        float        GetOre(float maxOre);
        void        SetOre(float maxOre, float ore);
    };

    public ref class ServerLightShipUpdateWrapper
	{
	public:
		::ServerLightShipUpdate * m_instance;

	public:
		
		ServerLightShipUpdateWrapper(::ServerLightShipUpdate * instance);

    };

    public ref class ServerHeavyShipUpdateWrapper : ServerLightShipUpdateWrapper
	{
	public:
		::ServerHeavyShipUpdate * m_instance;

	public:
		
		ServerHeavyShipUpdateWrapper(::ServerHeavyShipUpdate * instance);

    };

    public ref class ServerSingleShipUpdateWrapper : ServerLightShipUpdateWrapper
	{
	public:
		::ServerSingleShipUpdate * m_instance;

	public:
		
		ServerSingleShipUpdateWrapper(::ServerSingleShipUpdate * instance);

    };

    public ref class ClientShipUpdateWrapper
	{
	public:
		::ClientShipUpdate * m_instance;

	public:
		
		ClientShipUpdateWrapper(::ClientShipUpdate * instance);

    };

    public ref class ClientActiveTurretUpdateWrapper
	{
	public:
		::ClientActiveTurretUpdate * m_instance;

	public:
		
		ClientActiveTurretUpdateWrapper(::ClientActiveTurretUpdate * instance);

    };

    public ref class ServerActiveTurretUpdateWrapper
	{
	public:
		::ServerActiveTurretUpdate * m_instance;

	public:
		
		ServerActiveTurretUpdateWrapper(::ServerActiveTurretUpdate * instance);

    };

	ref class IstaticIGCWrapper;
	public ref class IstaticIGCWrapper
	{
	public:
		::IstaticIGC * m_instance;

	public:

		IstaticIGCWrapper(::IstaticIGC * instance);

		void                            Initialize();
		void                            Terminate();
		float                           GetFloatConstant(FloatConstantID fcid);
		void                            SetFloatConstant(FloatConstantID fcid, float f);
		float                           GetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid);
		void                            SetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid, float f);
		const void*                     GetConstants();
		void                            SetConstants(const void* pf);
		int                             GetSizeOfConstants();
		void                            AddDroneType(IdroneTypeIGCWrapper^ dt);
		void                            DeleteDroneType(IdroneTypeIGCWrapper^ dt);
		IdroneTypeIGCWrapper ^ GetDroneType(DroneTypeID id);
		const DroneTypeListIGC*         GetDroneTypes();
		void                            AddDevelopment(IdevelopmentIGCWrapper^ d);
		void                            DeleteDevelopment(IdevelopmentIGCWrapper^ d);
		IdevelopmentIGCWrapper ^ GetDevelopment(DevelopmentID id);
		const DevelopmentListIGC*       GetDevelopments();
		void                            AddStationType(IstationTypeIGCWrapper^ st);
		void                            DeleteStationType(IstationTypeIGCWrapper^ st);
		IstationTypeIGCWrapper ^ GetStationType(StationTypeID id);
		const StationTypeListIGC*       GetStationTypes();
		void                            AddCivilization(IcivilizationIGCWrapper^ c);
		void                            DeleteCivilization(IcivilizationIGCWrapper^ c);
		IcivilizationIGCWrapper ^ GetCivilization(CivID id);
		const CivilizationListIGC*      GetCivilizations();
		void                            AddHullType(IhullTypeIGCWrapper^ ht);
		void                            DeleteHullType(IhullTypeIGCWrapper^ ht);
		IhullTypeIGCWrapper ^ GetHullType(HullID id);
		System::Collections::Generic::List<IhullTypeIGCWrapper ^> ^          GetHullTypes();
		void                            AddExpendableType(IexpendableTypeIGCWrapper^ mt);
		void                            DeleteExpendableType(IexpendableTypeIGCWrapper^ mt);
		IexpendableTypeIGCWrapper ^ GetExpendableType(ExpendableTypeID id);
		const ExpendableTypeListIGC*    GetExpendableTypes();
		void                            AddPartType(IpartTypeIGCWrapper^ pt);
		void                            DeletePartType(IpartTypeIGCWrapper^ pt);
		IpartTypeIGCWrapper ^ GetPartType(PartID id);
		System::Collections::Generic::List<IpartTypeIGCWrapper ^> ^          GetPartTypes();
		void                            AddProjectileType(IprojectileTypeIGCWrapper^ pt);
		void                            DeleteProjectileType(IprojectileTypeIGCWrapper^ pt);
		IprojectileTypeIGCWrapper ^ GetProjectileType(ProjectileTypeID id);
		const ProjectileTypeListIGC*    GetProjectileTypes();
		void                            AddTreasureSet(ItreasureSetIGCWrapper^ pt);
		void                            DeleteTreasureSet(ItreasureSetIGCWrapper^ pt);
		ItreasureSetIGCWrapper ^ GetTreasureSet(TreasureSetID id);
		const TreasureSetListIGC*       GetTreasureSets();
		IpartTypeIGCWrapper ^ GetAmmoPack();
		IpartTypeIGCWrapper ^ GetFuelPack();
	};

    public ref class ImissionIGCWrapper : IstaticIGCWrapper
	{
	public:
		::ImissionIGC * m_instance;

	public:
		
		ImissionIGCWrapper(::ImissionIGC * instance);

        ZString				 BitsToTechsList(TechTreeBitMask & ttbm);
        void				 TechsListToBits(const char * szTechs, TechTreeBitMask & ttbm);
        ZString				 BitsToPartsList(PartMask & pm, EquipmentType et);
        PartMask			 PartMaskFromToken(const char * szToken, EquipmentType et);
        PartMask			 PartsListToMask(const char * szParts, EquipmentType et);
        bool				 LoadTechBitsList();
        bool				 LoadPartsBitsList();
        void				 ExportStaticIGCObjs();
        void				 ImportStaticIGCObjs();
        MissionID               GetMissionID();
        void                    SetMissionID(MissionID mid);
        void                    Initialize(Time now, IIgcSiteWrapper^ pIgcSite);
        void                    Terminate();
        void                    Update(Time now);
        IIgcSiteWrapper ^ GetIgcSite();
        ImodelIGCWrapper ^ GetModel(ObjectType type, ObjectID id);
        IbaseIGCWrapper ^ GetBase(ObjectType type, ObjectID id);
        DamageTrackWrapper ^ CreateDamageTrack();
        void                    AddSide(IsideIGCWrapper^ s);
        void                    DeleteSide(IsideIGCWrapper^ s);
        IsideIGCWrapper ^ GetSide(SideID sideID);
        const SideListIGC*      GetSides();/*
        void                    GetSeenSides(ImodelIGCWrapper ^ pmodel, bool (& bseensides);*/
        void                    AddCluster(IclusterIGCWrapper^ c);
        void                    DeleteCluster(IclusterIGCWrapper^ c);
        IclusterIGCWrapper ^ GetCluster(SectorID clusterID);
        System::Collections::Generic::List<IclusterIGCWrapper ^> ^   GetClusters();
        void                    AddShip(IshipIGCWrapper^ s);
        void                    DeleteShip(IshipIGCWrapper^ s);
        IshipIGCWrapper ^ GetShip(ShipID shipID);
		GenericList(IshipIGCWrapper) GetShips();
        void                    AddStation(IstationIGCWrapper^ s);
        void                    DeleteStation(IstationIGCWrapper^ s);
        IstationIGCWrapper ^ GetStation(StationID stationID);
        

		GenericList(IstationIGCWrapper) GetStations();

        void                    AddAsteroid(IasteroidIGCWrapper^ p);
        void                    DeleteAsteroid(IasteroidIGCWrapper^ p);
        IasteroidIGCWrapper ^ GetAsteroid(AsteroidID asteroidID);
        const AsteroidListIGC*  GetAsteroids();
        void                    AddWarp(IwarpIGCWrapper^ w);
        void                    DeleteWarp(IwarpIGCWrapper^ w);
        IwarpIGCWrapper ^ GetWarp(WarpID warpID);
		System::Collections::Generic::List<IwarpIGCWrapper ^> ^      GetWarps();
        void                    AddBuoy(IbuoyIGCWrapper^ t);
        void                    DeleteBuoy(IbuoyIGCWrapper^ t);
        IbuoyIGCWrapper ^ GetBuoy(BuoyID buoyID);
        const BuoyListIGC*      GetBuoys();
        void                    AddTreasure(ItreasureIGCWrapper^ t);
        void                    DeleteTreasure(ItreasureIGCWrapper^ t);
        ItreasureIGCWrapper ^ GetTreasure(TreasureID treasureID);
        const TreasureListIGC*  GetTreasures();
        void                    AddMine(ImineIGCWrapper^ ms);
        void                    DeleteMine(ImineIGCWrapper^ m);
        ImineIGCWrapper ^ GetMine(MineID mineID);
        const MineListIGC*      GetMines();
        void                    AddProbe(IprobeIGCWrapper^ ms);
        void                    DeleteProbe(IprobeIGCWrapper^ m);
        IprobeIGCWrapper ^ GetProbe(ProbeID probeID);
        const ProbeListIGC*     GetProbes();
        BuoyID                  GenerateNewBuoyID();
        ShipID                  GenerateNewShipID();
        AsteroidID              GenerateNewAsteroidID();
        StationID               GenerateNewStationID();
        TreasureID              GenerateNewTreasureID();
        MineID                  GenerateNewMineID();
        ProbeID                 GenerateNewProbeID();
        MissileID               GenerateNewMissileID();
        void                    SetMissionStage(STAGE st);
		AllegianceInterop::MissionStage GetMissionStage();
        void                    EnterGame();
        void                    ResetMission();
        void                    SetPrivateData(DWORD dwPrivate);
        DWORD                   GetPrivateData();
        Time                    GetLastUpdate();
        MissionParamsWrapper ^ GetMissionParams();
        void                    SetMissionParams(MissionParamsWrapper^ pmp);
        void                    SetStartTime(Time timeStart);
        CstaticIGC*             GetStaticCore();
        void                    SetStaticCore(CstaticIGC* pStatic);
        short                   GetReplayCount();
        String ^ GetContextName();
        void                    UpdateAllies(const char Allies[c_cSidesMax]);
    };

    public ref class IbaseIGCWrapper
	{
	public:
		::IbaseIGC * m_instance;

	public:
		
		IbaseIGCWrapper(::IbaseIGC * instance);

        HRESULT         Initialize(ImissionIGCWrapper^ pMission, Time now, const void* data, int length);
        void            Terminate();
        void            Update(Time now);
        int             Export(void* data);
        int             GetUniqueID();
        ObjectType      GetObjectType();
        ObjectID        GetObjectID();
        ImissionIGCWrapper ^ GetMission();
    };

    public ref class ItypeIGCWrapper : IbaseIGCWrapper
	{
	public:
		::ItypeIGC * m_instance;

	public:
		
		ItypeIGCWrapper(::ItypeIGC * instance);

        const void*     GetData();
    };

    public ref class AttachSiteWrapper
	{
	public:
		::AttachSite * m_instance;

	public:
		
		AttachSiteWrapper(::AttachSite * instance);

        void Terminate();
        void AddExplosion(ImodelIGCWrapper^ pmodel, int type);
        void AddExplosion(const VectorWrapper ^ vecPosition, float scale, int type);
        void AddThingSite(ThingSiteWrapper^ thing);
        void DeleteThingSite(ThingSiteWrapper^ thing);
    };

    public ref class ThingSiteWrapper : AttachSiteWrapper
	{
	public:
		::ThingSite * m_instance;

	public:
		
		ThingSiteWrapper(::ThingSite * instance);

        void        Purge();
        Vector      GetChildModelOffset(const ZString& strFrame);
        Vector      GetChildOffset(const ZString& strFrame);
        void        AddHullHit(const VectorWrapper ^ vecPosition, const VectorWrapper ^ vecNormal);
        void        AddFlare(Time ptime, const VectorWrapper ^ vecPosition, int id, const Vector* ellipseEquation);
        void        AddMuzzleFlare(const VectorWrapper ^ vecEmissionPoint, float duration);
        void        SetVisible(unsigned char render);
        float           GetDistanceToEdge();
        float           GetScreenRadius();
        unsigned char   GetRadarState();
        ThingGeo*   GetThingGeo();
        GeoWrapper ^ GetGeo();
        void        SetPosition(const VectorWrapper ^ position);
        float       GetRadius();
        void        SetRadius(float r);
        void        SetOrientation(const Orientation& orientation);
        void        Spin(float r);
        void        SetTexture(const char* pszTextureName);
        void        SetTrailColor(const Color& c);
        HRESULT     LoadDecal(const char* textureName, bool bDirectional, float width);
        HRESULT     LoadModel(int options, const char* modelName, const char* textureName);
        HRESULT     LoadAleph(const char* textureName);
        HRESULT     LoadEffect(const Color& color);
        void        SetColors(float aInner, float fInner, float fOuter);
        HRESULT     LoadMine(const char* textureName, float strength, float radius);
        void        SetMineStrength(float strength);
    };

    public ref class ImodelIGCWrapper : IbaseIGCWrapper
	{
	public:
		::ImodelIGC * m_instance;

	public:
		
		ImodelIGCWrapper(::ImodelIGC * instance);

        Time                 GetLastUpdate();
        void                 SetLastUpdate(Time now);
        void                 SetMission(ImissionIGCWrapper^ pmission);
        bool                 GetVisibleF();
        void                 SetVisibleF(bool visibleF);
        void                 SetRender(unsigned char render);
        bool                 SeenBySide(IsideIGCWrapper^ side);
        bool                 GetCurrentEye(IsideIGCWrapper^ side);
        void                 UpdateSeenBySide();
        void                 SetSideVisibility(IsideIGCWrapper^ side, bool fVisible);
        ThingSiteWrapper ^ GetThingSite();
        void                 FreeThingSite();
        void                 SetPosition(const VectorWrapper ^ newVal);
		VectorWrapper ^      GetPosition() { return gcnew VectorWrapper(m_instance->GetPosition()); }
        void                 SetVelocity(const VectorWrapper ^ newVal);
		VectorWrapper ^		 GetVelocity() { return gcnew VectorWrapper(m_instance->GetVelocity()); }
        void                 SetOrientation(const Orientation& newVal);
        void                 SetRotation(const Rotation& newVal);
        float                GetRadius();
        void                 SetRadius(float newVal);
        ModelAttributes      GetAttributes();
        IsideIGCWrapper ^ GetSide();
        void                 SetSide(IsideIGCWrapper^ newVal);
        float                GetMass();
        void                 SetMass(float newVal);
        IclusterIGCWrapper ^ GetCluster();
        void                 SetCluster(IclusterIGCWrapper^ pVal);
        String ^ GetName();
		String ^ GetSecondaryName();
        void                 SetName(const char* newVal);
        void                 SetSecondaryName(const char* newVal);
		
        float                GetSignature();
        void                 SetSignature(float newVal);
        void                 ChangeSignature(float delta);
        void                 SetTexture(const char* pszTextureName);
        void                 Move(float t);
        void                 Move();
        void                 SetBB(Time tStart, Time tStop, float dt);
        HitTest*             GetHitTest();
        IObject*             GetIcon();
        void                 SetIcon(IObject* picon);
        SideID               GetFlag();

		virtual String^ ToString() override
		{
			return GetName();
		}
    };

    public ref class IdamageIGCWrapper : ImodelIGCWrapper
	{
	public:
		::IdamageIGC * m_instance;

	public:
		
		IdamageIGCWrapper(::IdamageIGC * instance);

        float           GetFraction();
        void            SetFraction(float newVal);
        float           GetHitPoints();
    };

    public ref class IscannerIGCWrapper : IdamageIGCWrapper
	{
	public:
		::IscannerIGC * m_instance;

	public:
		
		IscannerIGCWrapper(::IscannerIGC * instance);

        bool                 InScannerRange(ImodelIGCWrapper^ pModel);
        bool                 CanSee(ImodelIGCWrapper^ pModel);
    };

    public ref class IshipIGCWrapper : IscannerIGCWrapper
	{
	public:
		::IshipIGC * m_instance;

	public:
		
		IshipIGCWrapper(::IshipIGC * instance);

        void                 SetObjectID(ObjectID id);
        Money                GetValue();
        IstationIGCWrapper ^ GetStation();
        void                 SetStation(IstationIGCWrapper^ s);
        void                 Reset(bool bFull);
        float                GetTorqueMultiplier();
        float                GetCurrentTurnRate(Axis axis);
        void                 SetCurrentTurnRate(Axis axis, float newVal);
        IhullTypeIGCWrapper ^ GetHullType();
        IhullTypeIGCWrapper ^ GetBaseHullType();
        void                 SetBaseHullType(IhullTypeIGCWrapper^ newVal);
        void                 SetMission(ImissionIGCWrapper^ pMission);
        void                 AddPart(IpartIGCWrapper^ part);
        void                 DeletePart(IpartIGCWrapper^ part);
        GenericList(IpartIGCWrapper) GetParts();
        IpartIGCWrapper ^ GetMountedPart(EquipmentType type, Mount mountID);
        void                 MountPart(IpartIGCWrapper^ p, Mount mountNew, Mount* pmountOld);
        short                GetAmmo();
        void                 SetAmmo(short amount);
        float                GetFuel();
        void                 SetFuel(float newVal);
        float                GetEnergy();
        void                 SetEnergy(float newVal);
        float                GetCloaking();
        void                 SetCloaking(float newVal);
        bool                 GetVectorLock();
        void                 SetVectorLock(bool bVectorLock);
        void                 SetControls(const ControlData& newVal);
        bool                 fRipcordActive();
        float                GetRipcordTimeLeft();
        void                 ResetRipcordTimeLeft();
        int                  GetStateM();
        void                 SetStateM(int newVal);
        void                 SetStateBits(int mask, int newBits);
        ImodelIGCWrapper ^ GetCommandTarget(Command i);
        CommandID            GetCommandID(Command i);
        void                 SetCommand(Command i, ImodelIGCWrapper^ target, CommandID cid);
        void                 PreplotShipMove(Time timeStop);
        void                 PlotShipMove(Time timeStop);
        void                 ExecuteShipMove(Time timeStop);
        bool                EquivalentShip(IshipIGCWrapper^ pship);
        short               ExportShipLoadout(ShipLoadoutWrapper^ ploadout);
        void                ExportFractions(CompactShipFractionsWrapper^ pfractions);
        void                ExportShipUpdate(ServerLightShipUpdateWrapper^ pshipupdate);
        void                ExportShipUpdate(ServerSingleShipUpdateWrapper^ pshipupdate);
        void                ExportShipUpdate(ClientShipUpdateWrapper^ pshipupdate);
        void                ExportShipUpdate(ClientActiveTurretUpdateWrapper^ patu);
        void                ProcessFractions(const CompactShipFractions& fractions);
        ShipUpdateStatus    ProcessShipUpdate(const ServerLightShipUpdate& shipupdate);
        ShipUpdateStatus    ProcessShipUpdate(const ClientShipUpdate& shipupdate);
        ShipUpdateStatus    ProcessShipUpdate(const ClientActiveTurretUpdate& shipupdate);
        void                SetPrivateData(DWORD dwPrivate);
        DWORD               GetPrivateData();
        void                ReInitialize(DataShipIGC * dataShip, Time now);
        Mount               HitTreasure(TreasureCode treasureCode, ObjectID objectID, short amount);
        ImissileIGCWrapper ^ GetLastMissileFired();
        void                SetLastMissileFired(ImissileIGCWrapper^ pmissile);
        Time                GetLastTimeLaunched();
        void                SetLastTimeLaunched(Time timeLastLaunch);
        void                SetLastTimeDocked(Time timeLastDock);
        Time                GetLastTimeDocked();
        void                Promote();
        void                SetParentShip(IshipIGCWrapper^ pship);
        IshipIGCWrapper ^ GetParentShip();
        Mount               GetTurretID();
        void                SetTurretID(Mount turretID);
        IshipIGCWrapper ^ GetGunner(Mount turretID);
        const ShipListIGC*  GetChildShips();
        void    Complain(SoundID sid, const char* pszMsg);
        short               GetKills();
        void                SetKills(short n);
        void                AddKill();
        short               GetDeaths();
        void                SetDeaths(short n);
        void                AddDeath();
        short               GetEjections();
        void                SetEjections(short n);
        void                AddEjection();
        float               GetExperienceMultiplier();
        float               GetExperience();
        void                SetExperience(float experiance);
        void                AddExperience();
        void                AddChildShip(IshipIGCWrapper^ pship);
        void                DeleteChildShip(IshipIGCWrapper^ pship);
        IshipIGCWrapper ^ GetSourceShip();
        IpartIGCWrapper ^ CreateAndAddPart(const PartData* ppd);
        IpartIGCWrapper ^ CreateAndAddPart(IpartTypeIGCWrapper^ ppt, Mount mount, short amount);
        WingID              GetWingID();
        void                SetWingID(WingID wid);
        PilotType           GetPilotType();
        AbilityBitMask      GetOrdersABM();
        bool                GetAutopilot();
        void                SetAutopilot(bool bAutopilot);
        bool                LegalCommand(CommandID cid);
        //void                SetRunawayCheckCooldown(float dtRunAway);
        IshipIGCWrapper ^ GetAutoDonate();
        void                SetAutoDonate(IshipIGCWrapper^ pship);
        ImodelIGCWrapper ^ GetRipcordModel();
        void                SetRipcordModel(ImodelIGCWrapper^ pmodel);
        ImodelIGCWrapper ^ FindRipcordModel(IclusterIGCWrapper^ pcluster);
        float               GetRipcordDebt();
        void                AdjustRipcordDebt(float delta);
        void			 SetStayDocked(bool stayDock);
        bool			 GetStayDocked();
        void                SetGettingAmmo(bool gettingAmmo);
        void                SetWingmanBehaviour(WingmanBehaviourBitMask wingmanBehaviour);
        WingmanBehaviourBitMask GetWingmanBehaviour();
        void			 AddRepair(float repair);
        float			 GetRepair();
        void			 SetAchievementMask(AchievementMask am);
        void			 ClearAchievementMask();
        AchievementMask	 GetAchievementMask();
        void			 MarkPreviouslySpotted();
        //bool			 RecentlySpotted();
        DamageTrackWrapper ^ GetDamageTrack();
        void                CreateDamageTrack();
        void                DeleteDamageTrack();
        void                ResetDamageTrack();
        void                ApplyMineDamage();
        CommandID           GetDefaultOrder(ImodelIGCWrapper^ pmodel);
        bool                OkToLaunch(Time now);
        bool                PickDefaultOrder(IclusterIGCWrapper^ pcluster, const VectorWrapper ^ position, bool bDocked);
        bool                IsGhost();
        float               GetEndurance();
        bool                InGarage(IshipIGCWrapper^ pship, float tCollision);
        bool                IsUsingAreaOfEffectWeapon();
        short               GetLaunchSlot();
        WarningMask         GetWarningMask();
        void                SetWarningMaskBit(WarningMask wm);
        void                ClearWarningMaskBit(WarningMask wm);
        void                SetBaseData(IbaseIGCWrapper^ pbase);
        IbaseIGCWrapper ^ GetBaseData();
        float               GetOre();
        void                SetSkills(float fShoot, float fTurn, float fGoto);
        void			 SetWantBoost(bool bOn);
        bool 			 GetWantBoost();
		IstationTypeIGCWrapper ^ GetStationType();
		//void				SetCluster(IclusterIGCWrapper ^ cluster);
		PlayerInfoWrapper ^ GetPlayerInfo();
		
    };

    public ref class IbuoyIGCWrapper : ImodelIGCWrapper
	{
	public:
		::IbuoyIGC * m_instance;

	public:
		
		IbuoyIGCWrapper(::IbuoyIGC * instance);

        void                 AddConsumer();
        void                 ReleaseConsumer();
        BuoyType             GetBuoyType();
    };

    public ref class IchaffIGCWrapper : ImodelIGCWrapper
	{
	public:
		::IchaffIGC * m_instance;

	public:
		
		IchaffIGCWrapper(::IchaffIGC * instance);

    };

    public ref class IprojectileIGCWrapper : ImodelIGCWrapper
	{
	public:
		::IprojectileIGC * m_instance;

	public:
		
		IprojectileIGCWrapper(::IprojectileIGC * instance);

        IprojectileTypeIGCWrapper ^ GetProjectileType();
        ImodelIGCWrapper ^ GetLauncher();
        void                 SetLauncher(ImodelIGCWrapper^ newVal);
        void                 SetGunner(IshipIGCWrapper^ pship);
    };

    public ref class ImissileIGCWrapper : IdamageIGCWrapper
	{
	public:
		::ImissileIGC * m_instance;

	public:
		
		ImissileIGCWrapper(::ImissileIGC * instance);

        ImissileTypeIGCWrapper ^ GetMissileType();
        IshipIGCWrapper ^ GetLauncher();
        ImodelIGCWrapper ^ GetTarget();
        void                SetTarget(ImodelIGCWrapper^ newVal);
        float               GetLock();
        void                Explode(const VectorWrapper ^ position);
        void                Disarm();
    };

    public ref class ImineIGCWrapper : ImodelIGCWrapper
	{
	public:
		::ImineIGC * m_instance;

	public:
		
		ImineIGCWrapper(::ImineIGC * instance);

        ImineTypeIGCWrapper ^ GetMineType();
        IshipIGCWrapper ^ GetLauncher();
        float                GetStrength();
        void                 ReduceStrength(float amount);
        float                GetTimeFraction();
    };

    public ref class IbuildingEffectIGCWrapper : IdamageIGCWrapper
	{
	public:
		::IbuildingEffectIGC * m_instance;

	public:
		
		IbuildingEffectIGCWrapper(::IbuildingEffectIGC * instance);

        void                BuilderTerminated();
        IasteroidIGCWrapper ^ GetAsteroid();
        void                MakeUnhitable();
        void                AsteroidUpdate(Time now);
    };

    public ref class IprobeIGCWrapper : IscannerIGCWrapper
	{
	public:
		::IprobeIGC * m_instance;

	public:
		
		IprobeIGCWrapper(::IprobeIGC * instance);

        IprobeTypeIGCWrapper ^ GetProbeType();
        IprojectileTypeIGCWrapper ^ GetProjectileType();
        float                GetProjectileLifespan();
        float                GetLifespan();
        float                GetDtBurst();
        float                GetAccuracy();
        bool                 GetCanRipcord(float ripcordspeed);
        float                GetRipcordDelay();
        SoundID              GetAmbientSound();
        float                GetTimeFraction();
        void			 SetExpiration(Time time);
        IshipIGCWrapper ^ GetProbeLauncherShip();
    };

    public ref class IstationIGCWrapper : IscannerIGCWrapper
	{
	public:
		::IstationIGC * m_instance;

	public:
		
		IstationIGCWrapper(::IstationIGC * instance);

        void                    AddShip(IshipIGCWrapper^ s);
        void                    DeleteShip(IshipIGCWrapper^ s);
        IshipIGCWrapper ^ GetShip(ShipID shipID);
		GenericList(IshipIGCWrapper) GetShips();
        void                    RepairAndRefuel(IshipIGCWrapper^ pship);
        void                    Launch(IshipIGCWrapper^ pship);
        bool                    InGarage(IshipIGCWrapper^ pship, const VectorWrapper ^ position);
        ObjectID			 GetRoidID();
        void SetRoidID(ObjectID id);
        Vector				 GetRoidPos();
        void SetRoidPos(Vector pos);
        float				 GetRoidSig();
        void SetRoidSig(float Sig);
        float				 GetRoidRadius();
        void SetRoidRadius(float Radius);
        AsteroidAbilityBitMask GetRoidCaps();
        void SetRoidCaps(AsteroidAbilityBitMask aabm);
        void SetRoidSide(SideID sid, bool bset);
        bool GetRoidSide(SideID sid);
        float                   GetShieldFraction();
        void                    SetShieldFraction(float sf);
        IstationTypeIGCWrapper ^ GetStationType();
        IstationTypeIGCWrapper ^ GetBaseStationType();
        void                    SetBaseStationType(IstationTypeIGCWrapper^ pst);
        bool                    CanBuy(IbuyableIGCWrapper^ b);
        bool                    IsObsolete(IbuyableIGCWrapper^ b);
        IbuyableIGCWrapper ^ GetSuccessor(IbuyableIGCWrapper^ b);
        IpartTypeIGCWrapper ^ GetSimilarPart(IpartTypeIGCWrapper^ ppt);
        void                    SetLastDamageReport(Time timeLastDamage);
        Time                    GetLastDamageReport();
        SoundID                 GetInteriorSound();
        SoundID                 GetExteriorSound();
    };

    public ref class IbuyableIGCWrapper : ItypeIGCWrapper
	{
	public:
		::IbuyableIGC * m_instance;

	public:
		
		IbuyableIGCWrapper(::IbuyableIGC * instance);

        String ^ GetName();
        String ^ GetIconName();
        String ^ GetDescription();
        String ^ GetModelName();
        Money                   GetPrice();
        DWORD                   GetTimeToBuild();
        BuyableGroupID          GetGroupID();
    };

    public ref class IdevelopmentIGCWrapper : IbuyableIGCWrapper
	{
	public:
		::IdevelopmentIGC * m_instance;

	public:
		
		IdevelopmentIGCWrapper(::IdevelopmentIGC * instance);

        bool                        GetTechOnly();
        bool                        IsObsolete(const TechTreeBitMask& ttbm);
        SoundID                     GetCompletionSound();
    };

    public ref class IdroneTypeIGCWrapper : IbuyableIGCWrapper
	{
	public:
		::IdroneTypeIGC * m_instance;

	public:
		
		IdroneTypeIGCWrapper(::IdroneTypeIGC * instance);

        HullID                  GetHullTypeID();
        PilotType               GetPilotType();
        float                   GetShootSkill();
        float                   GetMoveSkill();
        float                   GetBravery();
        IexpendableTypeIGCWrapper ^ GetLaidExpendable();
    };

    public ref class IstationTypeIGCWrapper : IbuyableIGCWrapper
	{
	public:
		::IstationTypeIGC * m_instance;

	public:
		
		IstationTypeIGCWrapper(::IstationTypeIGC * instance);

        HitPoints               GetMaxArmorHitPoints();
        DefenseTypeID           GetArmorDefenseType();
        HitPoints               GetMaxShieldHitPoints();
        DefenseTypeID           GetShieldDefenseType();
        float                   GetArmorRegeneration();
        float                   GetShieldRegeneration();
        float                   GetSignature();
        float                   GetRadius();
        float                   GetScannerRange();
        Money                   GetIncome();
        StationAbilityBitMask   GetCapabilities();
        bool                    HasCapability(StationAbilityBitMask habm);
        String ^ GetTextureName();
        String ^ GetBuilderName();
        IstationTypeIGCWrapper ^ GetSuccessorStationType(IsideIGCWrapper^ pside);
        IstationTypeIGCWrapper ^ GetDirectSuccessorStationType();
        AsteroidAbilityBitMask  GetBuildAABM();
        int                     GetLaunchSlots();
        int                     GetLandSlots();
        int                     GetCapLandSlots();
        int                     GetLandPlanes(int slotID);
        SoundID                 GetInteriorSound();
        SoundID                 GetInteriorAlertSound();
        SoundID                 GetExteriorSound();
        SoundID                 GetConstructorNeedRockSound();
        SoundID                 GetConstructorUnderAttackSound();
        SoundID                 GetConstructorDestroyedSound();
        SoundID                 GetCompletionSound();
        SoundID                 GetUnderAttackSound();
        SoundID                 GetCriticalSound();
        SoundID                 GetDestroyedSound();
        SoundID                 GetCapturedSound();
        SoundID                 GetEnemyCapturedSound();
        SoundID                 GetEnemyDestroyedSound();
        StationClassID          GetClassID();
        IdroneTypeIGCWrapper ^ GetConstructionDroneType();
    };

    public ref class IbucketIGCWrapper : IbuyableIGCWrapper
	{
	public:
		::IbucketIGC * m_instance;

	public:
		
		IbucketIGCWrapper(::IbucketIGC * instance);

        IbuyableIGCWrapper ^ GetBuyable();
        IsideIGCWrapper ^ GetSide();
        void                    SetPrice(Money m);
        int                     GetPercentBought();
        int                     GetPercentComplete();
        bool                    GetCompleteF();
        void                    ForceComplete(Time now);
        DWORD                   GetTime();
        Money                   GetMoney();
        void                    SetTimeAndMoney(DWORD dwTime, Money money);
        Money                   AddMoney(Money m);
        void                    SetEmpty();
        ObjectType              GetBucketType();
        IbucketIGCWrapper ^ GetPredecessor();
    };

    public ref class IprojectileTypeIGCWrapper : ItypeIGCWrapper
	{
	public:
		::IprojectileTypeIGC * m_instance;

	public:
		
		IprojectileTypeIGCWrapper(::IprojectileTypeIGC * instance);

        float            GetPower();
        float            GetBlastPower();
        float            GetBlastRadius();
        float            GetSpeed();
        bool             GetAbsoluteF();
        String ^ GetModelName();
        String ^ GetModelTexture();
        float            GetLifespan();
        float            GetRadius();
        float            GetRotation();
        COLORVALUE       GetColor();
        DamageTypeID     GetDamageType();
        SoundID          GetAmbientSound();
    };

    public ref class IexpendableTypeIGCWrapper : ItypeIGCWrapper
	{
	public:
		::IexpendableTypeIGC * m_instance;

	public:
		
		IexpendableTypeIGCWrapper(::IexpendableTypeIGC * instance);

        const LauncherDef*          GetLauncherDef();
        EquipmentType               GetEquipmentType();
        float                       GetLoadTime();
        float                       GetMass();
        float                       GetSignature();
        HitPoints                   GetHitPoints();
        DefenseTypeID               GetDefenseType();
        float                       GetLifespan();
        String ^ GetName();
        String ^ GetModelName();
        String ^ GetModelTexture();
        float                       GetRadius();
        ExpendableAbilityBitMask    GetCapabilities();
        bool                        HasCapability(ExpendableAbilityBitMask habm);
    };

    public ref class ImissileTypeIGCWrapper : IexpendableTypeIGCWrapper
	{
	public:
		::ImissileTypeIGC * m_instance;

	public:
		
		ImissileTypeIGCWrapper(::ImissileTypeIGC * instance);

        float           GetAcceleration();
        float           GetTurnRate();
        float           GetInitialSpeed();
        float           GetMaxLock();
        float           GetChaffResistance();
        float           GetLockTime();
        float           GetReadyTime();
        float           GetDispersion();
        float           GetLockAngle();
        float           GetPower();
        float           GetBlastPower();
        float           GetBlastRadius();
        DamageTypeID    GetDamageType();
        SoundID         GetLaunchSound();
        SoundID         GetAmbientSound();
    };

    public ref class ImineTypeIGCWrapper : IexpendableTypeIGCWrapper
	{
	public:
		::ImineTypeIGC * m_instance;

	public:
		
		ImineTypeIGCWrapper(::ImineTypeIGC * instance);

        float           GetPower();
        float           GetEndurance();
        DamageTypeID    GetDamageType();
    };

    public ref class IchaffTypeIGCWrapper : IexpendableTypeIGCWrapper
	{
	public:
		::IchaffTypeIGC * m_instance;

	public:
		
		IchaffTypeIGCWrapper(::IchaffTypeIGC * instance);

        float               GetChaffStrength();
    };

    public ref class IprobeTypeIGCWrapper : IexpendableTypeIGCWrapper
	{
	public:
		::IprobeTypeIGC * m_instance;

	public:
		
		IprobeTypeIGCWrapper(::IprobeTypeIGC * instance);

        float               GetScannerRange();
        float               GetDtBurst();
        float               GetDispersion();
        float               GetAccuracy();
        short               GetAmmo();
        IprojectileTypeIGCWrapper ^ GetProjectileType();
        float               GetRipcordDelay();
        SoundID             GetAmbientSound();
    };

    public ref class IhullTypeIGCWrapper : IbuyableIGCWrapper
	{
	public:
		::IhullTypeIGC * m_instance;

	public:
		
		IhullTypeIGCWrapper(::IhullTypeIGC * instance);

        float                GetLength();
        float                GetMaxSpeed();
        float                GetMaxTurnRate(Axis axis);
        float                GetTurnTorque(Axis axis);
        float                GetThrust();
        float                GetSideMultiplier();
        float                GetBackMultiplier();
        float                GetScannerRange();
        float                GetMaxEnergy();
        float                GetRechargeRate();
        HitPoints            GetHitPoints();
        DefenseTypeID        GetDefenseType();
        PartMask             GetPartMask(EquipmentType et, Mount mountID);
        short                GetCapacity(EquipmentType et);
        Mount                GetMaxWeapons();
        Mount                GetMaxFixedWeapons();
        bool                 CanMount(IpartTypeIGCWrapper^ ppt, Mount mountID);
        String ^ GetTextureName();
        float                GetMass();
        float                GetSignature();
        HullAbilityBitMask   GetCapabilities();
        bool                 HasCapability(HullAbilityBitMask habm);
        float                GetScale();
        short                GetMaxAmmo();
        float                GetMaxFuel();
        float                GetECM();
        float                GetRipcordSpeed();
        float                GetRipcordCost();
        IhullTypeIGCWrapper ^ GetSuccessorHullType();
        SoundID              GetInteriorSound();
        SoundID              GetExteriorSound();
        SoundID              GetMainThrusterInteriorSound();
        SoundID              GetMainThrusterExteriorSound();
        SoundID              GetManuveringThrusterInteriorSound();
        SoundID              GetManuveringThrusterExteriorSound();
        const PartTypeListIGC*      GetPreferredPartTypes();
        IObject*             GetIcon();
        int                     GetLaunchSlots();
        int                     GetLandSlots();
        int                     GetLandPlanes(int slotID);
		//bool				HasValue() { return m_instance != nullptr; }
    };

    public ref class IpartTypeIGCWrapper : IbuyableIGCWrapper
	{
	public:
		::IpartTypeIGC * m_instance;

	public:
		
		IpartTypeIGCWrapper(::IpartTypeIGC * instance);

        EquipmentType   GetEquipmentType();
        PartMask        GetPartMask();
        float           GetMass();
        float           GetSignature();
        IpartTypeIGCWrapper ^ GetSuccessorPartType();
        String ^ GetInventoryLineMDLName();/*
        String ^ GetEquipmentTypeName(EquipmentType et);*/
        short           GetAmount(IshipIGCWrapper^ pship);
    };

    public ref class IlauncherTypeIGCWrapper : IpartTypeIGCWrapper
	{
	public:
		::IlauncherTypeIGC * m_instance;

	public:
		
		IlauncherTypeIGCWrapper(::IlauncherTypeIGC * instance);
/*
        static  bool                IsLauncherType(EquipmentType et);*/
        IexpendableTypeIGCWrapper ^ GetExpendableType();
    };

    public ref class IpartIGCWrapper : IbaseIGCWrapper
	{
	public:
		::IpartIGC * m_instance;

	public:
		
		IpartIGCWrapper(::IpartIGC * instance);

        EquipmentType    GetEquipmentType();
        IpartTypeIGCWrapper ^ GetPartType();
        IshipIGCWrapper ^ GetShip();
        void             SetShip(IshipIGCWrapper^ newVal, Mount mount);
        Mount            GetMountID();
        void             SetMountID(Mount newVal);
        bool             fActive();
        void             Activate();
        void             Deactivate();
        float            GetMass();
        Money            GetPrice();
        float            GetMountedFraction();
        void             SetMountedFraction(float f);
        void             Arm();
        short            GetAmount();
        void             SetAmount(short a);
		
    };

    public ref class IweaponIGCWrapper : IpartIGCWrapper
	{
	public:
		::IweaponIGC * m_instance;

	public:
		
		IweaponIGCWrapper(::IweaponIGC * instance);

        IprojectileTypeIGCWrapper ^ GetProjectileType();
        short                GetAmmoPerShot();
        float                GetLifespan();
        float                GetDtBurst();
        IshipIGCWrapper ^ GetGunner();
        void                 SetGunner(IshipIGCWrapper^ newVal);
        bool                 fFiringShot();
        bool                 fFiringBurst();
        SoundID              GetActivateSound();
        SoundID              GetSingleShotSound();
        SoundID              GetBurstSound();
    };

    public ref class IshieldIGCWrapper : IpartIGCWrapper
	{
	public:
		::IshieldIGC * m_instance;

	public:
		
		IshieldIGCWrapper(::IshieldIGC * instance);

        float           GetRegeneration();
        float           GetMaxStrength();
        DefenseTypeID   GetDefenseType();
        float           GetFraction();
        void            SetFraction(float newVal);
    };

    public ref class IcloakIGCWrapper : IpartIGCWrapper
	{
	public:
		::IcloakIGC * m_instance;

	public:
		
		IcloakIGCWrapper(::IcloakIGC * instance);

        float    GetEnergyConsumption();
        float    GetMaxCloaking();
        float    GetOnRate();
        float    GetOffRate();
        SoundID  GetEngageSound();
        SoundID  GetDisengageSound();
    };

    public ref class IafterburnerIGCWrapper : IpartIGCWrapper
	{
	public:
		::IafterburnerIGC * m_instance;

	public:
		
		IafterburnerIGCWrapper(::IafterburnerIGC * instance);

        float    GetFuelConsumption();
        float    GetMaxThrustWithGA();
        float    GetMaxThrust();
        float    GetOnRate();
        float    GetOffRate();
        void     IncrementalUpdate(Time lastUpdate, Time now, bool bUseFuel);
        float    GetPower();
        void     SetPower(float p);
        SoundID  GetInteriorSound();
        SoundID  GetExteriorSound();
    };

    public ref class IpackIGCWrapper : IpartIGCWrapper
	{
	public:
		::IpackIGC * m_instance;

	public:
		
		IpackIGCWrapper(::IpackIGC * instance);

        PackType GetPackType();
    };

    public ref class IlauncherIGCWrapper : IpartIGCWrapper
	{
	public:
		::IlauncherIGC * m_instance;

	public:
		
		IlauncherIGCWrapper(::IlauncherIGC * instance);
/*
        static bool                 IsLauncher(ObjectType type);*/
        void                SetTimeFired(Time now);
        Time                GetTimeLoaded();
        void                SetTimeLoaded(Time tl);
        void                ResetTimeLoaded();
        float               GetArmedFraction();
    };

    public ref class ImagazineIGCWrapper : IlauncherIGCWrapper
	{
	public:
		::ImagazineIGC * m_instance;

	public:
		
		ImagazineIGCWrapper(::ImagazineIGC * instance);

        ImissileTypeIGCWrapper ^ GetMissileType();
        float               GetLock();
        void                SetLock(float l);
    };

    public ref class IdispenserIGCWrapper : IlauncherIGCWrapper
	{
	public:
		::IdispenserIGC * m_instance;

	public:
		
		IdispenserIGCWrapper(::IdispenserIGC * instance);

        IexpendableTypeIGCWrapper ^ GetExpendableType();

		static IdispenserIGCWrapper ^ GetDispenserForPart(IpartIGCWrapper ^ part) { ConvertTo(IdispenserIGCWrapper, (IdispenserIGC *)part->m_instance); };
    };

    public ref class IclusterIGCWrapper : IbaseIGCWrapper
	{
	public:
		::IclusterIGC * m_instance;

	public:
		
		IclusterIGCWrapper(::IclusterIGC * instance);

        String ^ GetName();
        Time                    GetLastUpdate();
        void                    SetLastUpdate(Time now);
        void                    AddModel(ImodelIGCWrapper^ modelNew);
        void                    DeleteModel(ImodelIGCWrapper^ modelOld);
        ImodelIGCWrapper ^ GetModel(const char* name);
        const ModelListIGC*     GetModels();
        const ModelListIGC*     GetPickableModels();
        void                    AddWarp(IwarpIGCWrapper^ warpNew);
        void                    DeleteWarp(IwarpIGCWrapper^ warpOld);
        System::Collections::Generic::List<IwarpIGCWrapper^> ^    GetWarps();
        void                    AddMine(ImineIGCWrapper^ ms);
        void                    DeleteMine(ImineIGCWrapper^ m);
        ImineIGCWrapper ^ GetMine(MineID mineID);
        const MineListIGC*      GetMines();
        void                    AddMissile(ImissileIGCWrapper^ ms);
        void                    DeleteMissile(ImissileIGCWrapper^ m);
        ImissileIGCWrapper ^ GetMissile(MissileID missileID);
        const MissileListIGC*   GetMissiles();
        void                    AddProbe(IprobeIGCWrapper^ ms);
        void                    DeleteProbe(IprobeIGCWrapper^ m);
        IprobeIGCWrapper ^ GetProbe(ProbeID probeID);
        const ProbeListIGC*     GetProbes();
        float                   GetScreenX();
        float                   GetScreenY();
        void                    SetActive(bool bActive);
        void                    AddStation(IstationIGCWrapper^ stationNew);
        void                    DeleteStation(IstationIGCWrapper^ stationOld);
        IstationIGCWrapper ^ GetStation(StationID stationID);
		GenericList(IstationIGCWrapper)   GetStations();
        void                    AddShip(IshipIGCWrapper^ shipNew);
        void                    DeleteShip(IshipIGCWrapper^ shipOld);
        IshipIGCWrapper ^ GetShip(ShipID shipID);
		GenericList(IshipIGCWrapper) GetShips();
        void                    AddAsteroid(IasteroidIGCWrapper^ asteroidNew);
        void                    DeleteAsteroid(IasteroidIGCWrapper^ asteroidOld);
        IasteroidIGCWrapper ^ GetAsteroid(AsteroidID asteroidID);
		GenericList(IasteroidIGCWrapper)  GetAsteroids();
        void                    AddTreasure(ItreasureIGCWrapper^ treasureNew);
        void                    DeleteTreasure(ItreasureIGCWrapper^ treasureOld);
        ItreasureIGCWrapper ^ GetTreasure(TreasureID treasureID);
        const TreasureListIGC*  GetTreasures();
        ClusterSiteWrapper ^ GetClusterSite();
        short                   GetNstars();
        short                   GetNdebris();
        unsigned int            GetStarSeed();
        Color                   GetLightColor();
        Color                   GetLightColorAlt();
        float                   GetBidirectionalAmbientLevel();
        float                   GetAmbientLevel();
        String ^ GetPosterName();
        void             SetPrivateData(DWORD dwPrivate);
        DWORD            GetPrivateData();
        void             FlushCollisionQueue();
        void             MakeModelStatic(ImodelIGCWrapper^ pmodel);
        void             MakeModelUnhitable(ImodelIGCWrapper^ pmodel);
        void             FreeThingSite(ModelAttributes mt, ImodelIGCWrapper^ pmodel, HitTest* pht);
        bool             GetHomeSector();
        float            GetPendingTreasures();
        void             SetPendingTreasures(float fpt);
        float            GetCost();
        void			 SetHighlight(SideID sid, ClusterHighlight ch);
        ClusterHighlight GetHighlight(SideID sid);
    };

    public ref class IasteroidIGCWrapper : IdamageIGCWrapper
	{
	public:
		::IasteroidIGC * m_instance;

	public:
		
		IasteroidIGCWrapper(::IasteroidIGC * instance);

        float                   GetOre();
        void                    SetOre(float newVal);
        float                   MineOre(float dt);
        AsteroidAbilityBitMask  GetCapabilities();
        bool                    HasCapability(AsteroidAbilityBitMask aabm);
        IbuildingEffectIGCWrapper ^ GetBuildingEffect();
        void                    SetBuildingEffect(IbuildingEffectIGCWrapper^ pbe);/*
        String ^ GetTypeName(AsteroidAbilityBitMask aabm);*//*
        String ^ GetTypePrefix(int index);*//*
        static int                      NumberSpecialAsteroids(MissionParamsWrapper^ pmp);*//*
        static int                      GetSpecialAsterioid(MissionParamsWrapper^ pmp, int index);*//*
        static int                      GetRandomType(AsteroidAbilityBitMask aabm);*/
        float GetOreSeenBySide(IsideIGCWrapper ^ side1);
        bool GetAsteroidCurrentEye(IsideIGCWrapper ^ side1);
        void SetOreWithFraction(float oreFraction, bool clientUpdate);
        float GetOreFraction();
        void SetBuilderSeenSide(ObjectID oid);
        bool GetBuilderSeenSide(ObjectID oid);
        void SetInhibitUpdate(bool inhib);
    };

    public ref class IwarpIGCWrapper : ImodelIGCWrapper
	{
	public:
		::IwarpIGC * m_instance;

	public:
		
		IwarpIGCWrapper(::IwarpIGC * instance);

        IwarpIGCWrapper ^ GetDestination();
        const WarpBombList* GetBombs();
        bool                IsFixedPosition();
        double			 MassLimit();
    };

    public ref class ItreasureIGCWrapper : ImodelIGCWrapper
	{
	public:
		::ItreasureIGC * m_instance;

	public:
		
		ItreasureIGCWrapper(::ItreasureIGC * instance);

        TreasureCode        GetTreasureCode();
        IbuyableIGCWrapper ^ GetBuyable();
        ObjectID            GetTreasureID();
        void                SetTreasureID(ObjectID newVal);
        short               GetAmount();
        void                SetAmount(short a);
        void                ResetExpiration(Time now);
    };

    public ref class IsideIGCWrapper : IbaseIGCWrapper
	{
	public:
		::IsideIGC * m_instance;

	public:
		
		IsideIGCWrapper(::IsideIGC * instance);

        IcivilizationIGCWrapper ^ GetCivilization();
        void                        SetCivilization(IcivilizationIGCWrapper^ pciv);
        void                        DestroyBuckets();
        void                        CreateBuckets();
        void                        SetName(const char* newVal);
        String ^ GetName();
        SquadID                     GetSquadID();
        void                        SetSquadID(SquadID squadID);
        const TechTreeBitMask       GetTechs();
        void                        ResetBuildingTechs();
        void                        SetBuildingTechs(const TechTreeBitMask& ttbm);
        void                        SetDevelopmentTechs(const TechTreeBitMask& ttbm);
        bool					 IsNewDevelopmentTechs(const TechTreeBitMask& ttbm);
        bool                        ApplyDevelopmentTechs(const TechTreeBitMask& ttbm);
        void                        SetInitialTechs(const TechTreeBitMask& ttbm);
        void                        UpdateInitialTechs();
        void                        AddStation(IstationIGCWrapper^ s);
        void                        DeleteStation(IstationIGCWrapper^ s);
        IstationIGCWrapper ^ GetStation(StationID id);
		GenericList(IstationIGCWrapper) GetStations();
        void                        AddShip(IshipIGCWrapper^ s);
        void                        DeleteShip(IshipIGCWrapper^ s);
        IshipIGCWrapper ^ GetShip(ShipID id);
		GenericList(IshipIGCWrapper) GetShips();
        void                        AddBucket(IbucketIGCWrapper^ s);
        void                        DeleteBucket(IbucketIGCWrapper^ s);
        IbucketIGCWrapper ^ GetBucket(BucketID bucketID);
		System::Collections::Generic::List<IbucketIGCWrapper ^> ^          GetBuckets();
        bool                        CanBuy(IbuyableIGCWrapper^ b);
        void                        SetGlobalAttributeSet(const GlobalAttributeSet& gas);
        void                        ApplyGlobalAttributeSet(const GlobalAttributeSet& gas);
        void                        ResetGlobalAttributeSet();
        bool                        GetActiveF();
        void                        SetActiveF(bool activeF);
        void                        SetPrivateData(DWORD dwPrivate);
        DWORD                       GetPrivateData();
        short                       GetKills();
        void                        AddKill();
        short                       GetDeaths();
        void                        AddDeath();
        short                       GetEjections();
        void                        AddEjection();
        short                       GetBaseKills();
        void                        AddBaseKill();
        short                       GetBaseCaptures();
        void                        AddBaseCapture();
        void                        Reset();
        void                        AddToStockpile(IbuyableIGCWrapper^ b, short count);
        short                       RemoveFromStockpile(IbuyableIGCWrapper^ b, short count);
        short                       GetStockpile(IbuyableIGCWrapper^ b);
        const StockpileList*        GetStockpile();
        unsigned char GetConquestPercent();
        void          SetConquestPercent(unsigned char newVal);
        unsigned char GetTerritoryCount();
        void          SetTerritoryCount(unsigned char newVal);
        short         GetFlags();
        void          SetFlags(short newVal);
        short         GetArtifacts();
        void          SetArtifacts(short newVal);
        float         GetTimeEndured();
        void          SetTimeEndured(float fSeconds);
        long          GetProsperityPercentBought();
        long          GetProsperityPercentComplete();
        void		  SetAllies(char allies);
        char          GetAllies();/*
        static bool           AlliedSides(IsideIGCWrapper ^ side1, IsideIGCWrapper ^ side2);*/
        bool GetRandomCivilization();
        void SetRandomCivilization(bool rand);
        int GetNumPlayersOnSide();
        void HandleNewEnemyCluster(IclusterIGCWrapper^ pcluster);
        void UpdateTerritory();
        ClusterListIGC GetTerritory();
        bool IsTerritory(IclusterIGCWrapper^ pcluster);
    };

    public ref class IcivilizationIGCWrapper : IbaseIGCWrapper
	{
	public:
		::IcivilizationIGC * m_instance;

	public:
		
		IcivilizationIGCWrapper(::IcivilizationIGC * instance);

        String ^ GetName();
        String ^ GetIconName();
        String ^ GetHUDName();
        IhullTypeIGCWrapper ^ GetLifepod();
        IstationTypeIGCWrapper ^ GetInitialStationType();
        float                       GetBonusMoney();
        float                       GetIncomeMoney();
    };

    public ref class ItreasureSetIGCWrapper : IbaseIGCWrapper
	{
	public:
		::ItreasureSetIGC * m_instance;

	public:
		
		ItreasureSetIGCWrapper(::ItreasureSetIGC * instance);

        String ^ GetName();
        bool                        GetZoneOnly();
        short					 GetSize();
        void                        AddTreasureData(TreasureCode tc, ObjectID oid, unsigned char chance);
    };

    public ref class ShipStatusWrapper
	{
	public:
		::ShipStatus * m_instance;

	public:
		
		ShipStatusWrapper(::ShipStatus * instance);

        void    Reset();
        HullID      GetHullID();
        void        SetHullID(HullID newVal);
        SectorID    GetSectorID();
        void        SetSectorID(SectorID newVal);
        StationID   GetStationID();
        void        SetStationID(StationID stationID);
        StationID   GetParentID();
        void        SetParentID(ShipID shipID);
        ShipState   GetState();
        void        SetState(ShipState newVal);
        bool        GetUnknown();
        void        SetUnknown();
        bool        GetDetected();
        void        SetDetected(bool bDetected);
        DWORD        GetStateTime();
        void        SetStateTime(DWORD dTime);
    };

    public ref class ClusterSiteWrapper : AttachSiteWrapper
	{
	public:
		::ClusterSite * m_instance;

	public:
		
		ClusterSiteWrapper(::ClusterSite * instance);

        HRESULT         GetEnvironmentGeo(const char* pszName);
        GeoWrapper ^ GetEnvironmentGeo();
        GroupGeo*       GetGroupScene();
        PosterImage*    GetPosterImage();
        void            SetEnvironmentGeo(const char* pszName);
        float           GetRotation();
        void            AddRotation(float da);
        void                    AddScanner(SideID sid, IscannerIGCWrapper^ scannerNew);
        void                    DeleteScanner(SideID sid, IscannerIGCWrapper^ scannerOld);
        const ScannerListIGC*   GetScanners(SideID sid);
        AssetMask               GetClusterAssetMask();
        void                    SetClusterAssetMask(AssetMask am);
        void                    MoveShip();
    };

    public ref class SideVisibilityWrapper
	{
	public:
		::SideVisibility * m_instance;

	public:
		
		SideVisibilityWrapper(::SideVisibility * instance);

        bool    fVisible();
        IscannerIGCWrapper ^ pLastSpotter();
        void        fVisible(bool v);
        void        pLastSpotter(IscannerIGCWrapper^ s);
        void CurrentEyed(bool v);
        bool CurrentEyed();
    };

    public ref class GameSiteWrapper
	{
	public:
		::GameSite * m_instance;

	public:
		
		GameSiteWrapper(::GameSite * instance);
/*
        static int MessageBox(const ZString& strText, const ZString& strCaption, UINT nType);*/
    };

    public ref class IIgcSiteWrapper
	{
	public:
		::IIgcSite * m_instance;

	public:
		
		IIgcSiteWrapper(::IIgcSite * instance);

        Time ServerTimeFromClientTime(Time timeClient);
        Time ClientTimeFromServerTime(Time timeServer);
        void    DevelopmentCompleted(IbucketIGCWrapper^ b, IdevelopmentIGCWrapper^ d, Time now);
        void    StationTypeCompleted(IbucketIGCWrapper^ b, IstationIGCWrapper^ pstation, IstationTypeIGCWrapper^ st, Time now);
        void    HullTypeCompleted(IsideIGCWrapper^ pside, IhullTypeIGCWrapper^ pht);
        void    PartTypeCompleted(IsideIGCWrapper^ pside, IpartTypeIGCWrapper^ ppt);
        void    DroneTypeCompleted(IbucketIGCWrapper^ b, IstationIGCWrapper^ pstation, IdroneTypeIGCWrapper^ dt, Time now);
        void    BucketChangeEvent(BucketChange bc, IbucketIGCWrapper^ b);
        void    SideBuildingTechChange(IsideIGCWrapper^ s);
        void    SideDevelopmentTechChange(IsideIGCWrapper^ s);
        void    SideGlobalAttributeChange(IsideIGCWrapper^ s);
        void    StationTypeChange(IstationIGCWrapper^ s);
        void    ClusterUpdateEvent(IclusterIGCWrapper^ c);
        void GetMoneyRequest(IshipIGCWrapper^ pshipSender, Money amount, HullID hidFor);
        void PlaySoundEffect(SoundID soundID, ImodelIGCWrapper^ model);
        void PlaySoundEffect(SoundID soundID, ImodelIGCWrapper^ model, const VectorWrapper ^ vectOffset);
        void PlayNotificationSound(SoundID soundID, ImodelIGCWrapper^ model);
        void PlayFFEffect(ForceEffectID effectID, ImodelIGCWrapper^ model, LONG lDirection);
        void PlayVisualEffect(VisualEffectID effectID, ImodelIGCWrapper^ model, float fIntensity);
    };

    public ref class DamageTrackWrapper
	{
	public:
		::DamageTrack * m_instance;

	public:
		
		DamageTrackWrapper(::DamageTrack * instance);

        void    SwitchSlots();
        void    Reset();
        void    AddDamageBucket(DamageBucketWrapper^ pdb);
        void    DeleteDamageBucket(DamageBucketWrapper^ pdb);
        DamageBucketList*    GetDamageBuckets();/*
        static void sort(DamageBucketList* pListBuckets);*/
    };

    public ref class DamageBucketWrapper
	{
	public:
		::DamageBucket * m_instance;

	public:
		
		DamageBucketWrapper(::DamageBucket * instance);

        void    Initialize(Time timeNow, ImodelIGCWrapper^ pmodel);
        ImodelIGCWrapper ^ model();
        float       totalDamage();
        bool        flash(Time timeNow);
        void        SwitchSlots(int idSlot);
    };

    public ref class DamageTrackSetWrapper
	{
	public:
		::DamageTrackSet * m_instance;

	public:
		
		DamageTrackSetWrapper(::DamageTrackSet * instance);

        void Initialize(Time now);
        void Update(Time now);
        DamageTrackWrapper ^ Create();
        /*void            AddTrack(DamageTrackWrapper^ pdt);
        void            DeleteTrack(DamageTrackWrapper^ pdt);*/
    };

    public ref class WaypointWrapper
	{
	public:
		::Waypoint * m_instance;

	public:
		
		WaypointWrapper(::Waypoint * instance);

    };

    public ref class GotoPlanWrapper
	{
	public:
		::GotoPlan * m_instance;

	public:
		
		GotoPlanWrapper(::GotoPlan * instance);

    };

    public ref class PersistPlayerScoreObjectWrapper
	{
	public:
		::PersistPlayerScoreObject * m_instance;

	public:
		
		PersistPlayerScoreObjectWrapper(::PersistPlayerScoreObject * instance);

        void    Reset();
        short   GetRank();
        float   GetScore();
        void  SetCivID(CivID civID);
        CivID GetCivID();
        float   GetCombatRating();
        void    SetCombatRating(float cr);
    };

    public ref class PlayerScoreObjectWrapper
	{
	public:
		::PlayerScoreObject * m_instance;

	public:
		
		PlayerScoreObjectWrapper(::PlayerScoreObject * instance);

        void    CalculateScore(ImissionIGCWrapper^ pmission);
        void    SetScore(float fNewScore);
        void    SetCommanderScore(float fCommanderScore);
        float   GetScore();
        void    Reset(bool bFull);
        void SetRankRatio(float rankRatio);
        bool    Connected();
        bool    Commanding();
        void    Connect(Time t);
        void    Disconnect(Time t);
        void    StartCommand(Time t);
        void    StopCommand(Time t);
        void    SpotWarp();
        void    SpotSpecialAsteroid();
        //void AddTargetSpot();
        //short   GetTargetsSpotted();
        void SetRepair(float repair);
        float GetRepair();
        void    KillBase(bool bPilot);
        void    CaptureBase(bool bPilot);
        short   GetPilotBaseKills();
        short   GetPilotBaseCaptures();
        float   GetWarpsSpotted();
        float   GetAsteroidsSpotted();
        float   GetMinerKills();
        float   GetBuilderKills();
        float   GetLayerKills();
        float   GetCarrierKills();
        float   GetPlayerKills();
        float   GetBaseKills();
        float   GetBaseCaptures();
        short   GetTechsRecovered();
        void    AddTechsRecovered();
        short   GetFlags();
        void    AddFlag();
        short   GetArtifacts();
        void    AddArtifact();
        void    AddRescue();
        short   GetRescues();
        short   GetKills();
        void    AddKill();
        short   GetAssists();
        void    AddAssist();
        short   GetDeaths();
        void    AddDeath();
        void    SetDeaths(short d);
        short   GetEjections();
        void    AddEjection();
        void    SetEjections(short e);
        float   GetCombatRating();
        void    SetCombatRating(float cr);
        void                       SetPersist(PersistPlayerScoreObjectWrapper ^ p);
        bool                        GetWinner();
        bool                        GetLoser();
        bool                        GetCommandWinner();
        bool                        GetCommandLoser();
        float GetTimePlayed();
        float GetTimeCommanded();
        bool    GetCommandCredit();
    };

    public ref class GameOverScoreObjectWrapper
	{
	public:
		::GameOverScoreObject * m_instance;

	public:
		
		GameOverScoreObjectWrapper(::GameOverScoreObject * instance);

        void    Set(PlayerScoreObjectWrapper^ ppso);
        short   GetRank();
        //float   GetScore();
        float   GetRecentTimePlayed();
        short   GetRecentBaseKills();
        short   GetRecentBaseCaptures();
        short   GetRecentKills();
        short   GetRecentDeaths();
        short   GetRecentEjections();
        /*short   GetTotalBaseKills();
        short   GetTotalBaseCaptures();
        short   GetFlags();
        short   GetArtifacts();
        short   GetRescues();
        short   GetTotalKills();
        short   GetTotalDeaths();
        short   GetTotalEjections();
        short   GetWins();
        short   GetLosses();*/
        float   GetRecentScore();
        unsigned short   GetRecentAssists();
        /*float   GetCombatRating();
        bool    GetWinner();
        bool    GetLoser();*/
        bool        GetCommander();
        bool        GetGameCounted();
    };

}
