#include "stdafx.h"
#include "igcWrapper.h"
#include "Enums.h"
#include "shipIGC.h"

using namespace System;

namespace AllegianceInterop
{

	GeoWrapper::GeoWrapper(::Geo * instance)
	{
		m_instance = instance;
	}
	//MissionParamsWrapper::MissionParamsWrapper(::MissionParams * instance)
	//		{
	//			m_instance = instance;
	//		}
	ImapMakerIGCWrapper::ImapMakerIGCWrapper(::ImapMakerIGC * instance)
	{
		m_instance = instance;
	}/*
	 String ^ ImapMakerIGCWrapper::IsValid(MissionParamsWrapper ^ pmp)
	 {
	 return gcnew String(m_instance->IsValid(pmp->m_instance));
	 }*/
	MapMakerID  ImapMakerIGCWrapper::GetMapMakerID()
	{
		return m_instance->GetMapMakerID();
	}
	VOID        ImapMakerIGCWrapper::SetMapMakerID(MapMakerID mbID)
	{
		return m_instance->SetMapMakerID(mbID);
	}
	String ^ ImapMakerIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	void        ImapMakerIGCWrapper::SetName(const char* newVal)
	{
		return m_instance->SetName(newVal);
	}
	MissionParamsWrapper::MissionParamsWrapper(::MissionParams * instance)
	{
		m_instance = instance;
	}
	void    MissionParamsWrapper::Reset()
	{
		return m_instance->Reset();
	}
	String ^ MissionParamsWrapper::Invalid(bool bFromClient)
	{
		return gcnew String(m_instance->Invalid(bFromClient));
	}
	bool MissionParamsWrapper::IsConquestGame()
	{
		return m_instance->IsConquestGame();
	}
	bool MissionParamsWrapper::IsTerritoryGame()
	{
		return m_instance->IsTerritoryGame();
	}
	bool MissionParamsWrapper::IsDeathMatchGame()
	{
		return m_instance->IsDeathMatchGame();
	}
	bool MissionParamsWrapper::IsCountdownGame()
	{
		return m_instance->IsCountdownGame();
	}
	bool MissionParamsWrapper::IsProsperityGame()
	{
		return m_instance->IsProsperityGame();
	}
	bool MissionParamsWrapper::IsArtifactsGame()
	{
		return m_instance->IsArtifactsGame();
	}
	bool MissionParamsWrapper::IsFlagsGame()
	{
		return m_instance->IsFlagsGame();
	}
	bool MissionParamsWrapper::IsNoWinningConditionsGame()
	{
		return m_instance->IsNoWinningConditionsGame();
	}
	float MissionParamsWrapper::GetCountDownTime()
	{
		return m_instance->GetCountDownTime();
	}
	int MissionParamsWrapper::GetConquestPercentage()
	{
		return m_instance->GetConquestPercentage();
	}
	int MissionParamsWrapper::GetTerritoryPercentage()
	{
		return m_instance->GetTerritoryPercentage();
	}
	short MissionParamsWrapper::GetDeathMatchKillLimit()
	{
		return m_instance->GetDeathMatchKillLimit();
	}
	int MissionParamsWrapper::GetArtifactsPercentage()
	{
		return m_instance->GetArtifactsPercentage();
	}
	ShipLoadoutWrapper::ShipLoadoutWrapper(::ShipLoadout * instance)
	{
		m_instance = instance;
	}
	ExpandedPartData*   ShipLoadoutWrapper::PartData0()
	{
		return m_instance->PartData0();
	}
	ExpandedPartData*   ShipLoadoutWrapper::PartDataN(short cb)
	{
		return m_instance->PartDataN(cb);
	}
	CompactOrientationWrapper::CompactOrientationWrapper(::CompactOrientation * instance)
	{
		m_instance = instance;
	}
	void    CompactOrientationWrapper::Set(const Orientation& orientation)
	{
		return m_instance->Set(orientation);
	}
	void    CompactOrientationWrapper::Export(Orientation* pOrientation)
	{
		return m_instance->Export(pOrientation);
	}
	void    CompactOrientationWrapper::Validate()
	{
		return m_instance->Validate();
	}
	CompactTimeWrapper::CompactTimeWrapper(::CompactTime * instance)
	{
		m_instance = instance;
	}
	void        CompactTimeWrapper::Set(Time timeBase, Time timeNow)
	{
		return m_instance->Set(timeBase, timeNow);
	}
	void        CompactTimeWrapper::Export(Time timeBase, Time* timeNow)
	{
		return m_instance->Export(timeBase, timeNow);
	}
	CompactPositionWrapper::CompactPositionWrapper(::CompactPosition * instance)
	{
		m_instance = instance;
	}/*
	 static  float   CompactPositionWrapper::Displacement(short delta)
	 {
	 return m_instance->Displacement(delta);
	 }*//*
	 static short    CompactPositionWrapper::Delta(float displacement)
	 {
	 return m_instance->Delta(displacement);
	 }*/
	void        CompactPositionWrapper::Set(const VectorWrapper ^ pReference, const VectorWrapper ^ p)
	{
		return m_instance->Set(*pReference->m_instance, *p->m_instance);
	}
	void        CompactPositionWrapper::Export(const VectorWrapper ^ pReference, Vector* p)
	{
		return m_instance->Export(*pReference->m_instance, p);
	}
	CompactStateWrapper::CompactStateWrapper(::CompactState * instance)
	{
		m_instance = instance;
	}
	void        CompactStateWrapper::Set(int stateM)
	{
		return m_instance->Set(stateM);
	}
	void        CompactStateWrapper::Export(int* pStateM)
	{
		return m_instance->Export(pStateM);
	}
	CompactVelocityWrapper::CompactVelocityWrapper(::CompactVelocity * instance)
	{
		m_instance = instance;
	}
	void            CompactVelocityWrapper::Set(const VectorWrapper ^ v)
	{
		return m_instance->Set(*v->m_instance);
	}
	void            CompactVelocityWrapper::Export(Vector* pVelocity)
	{
		return m_instance->Export(pVelocity);
	}
	CompactTurnRatesWrapper::CompactTurnRatesWrapper(::CompactTurnRates * instance)
	{
		m_instance = instance;
	}
	void        CompactTurnRatesWrapper::Set(const float turnRates[3])
	{
		return m_instance->Set(turnRates);
	}
	void            CompactTurnRatesWrapper::Export(float turnRates[3])
	{
		return m_instance->Export(turnRates);
	}
	CompactControlDataWrapper::CompactControlDataWrapper(::CompactControlData * instance)
	{
		m_instance = instance;
	}
	void        CompactControlDataWrapper::Set(const ControlData& cd)
	{
		return m_instance->Set(cd);
	}
	void            CompactControlDataWrapper::Export(ControlData* cd)
	{
		return m_instance->Export(cd);
	}
	CompactShipFractionsWrapper::CompactShipFractionsWrapper(::CompactShipFractions * instance)
	{
		m_instance = instance;
	}
	void        CompactShipFractionsWrapper::SetHullFraction(float hf)
	{
		return m_instance->SetHullFraction(hf);
	}
	float        CompactShipFractionsWrapper::GetHullFraction()
	{
		return m_instance->GetHullFraction();
	}
	void        CompactShipFractionsWrapper::SetShieldFraction(float sf)
	{
		return m_instance->SetShieldFraction(sf);
	}
	float        CompactShipFractionsWrapper::GetShieldFraction()
	{
		return m_instance->GetShieldFraction();
	}
	void        CompactShipFractionsWrapper::SetFuel(float maxFuel, float fuel)
	{
		return m_instance->SetFuel(maxFuel, fuel);
	}
	float       CompactShipFractionsWrapper::GetFuel(float maxFuel)
	{
		return m_instance->GetFuel(maxFuel);
	}
	void        CompactShipFractionsWrapper::SetAmmo(short maxAmmo, short ammo)
	{
		return m_instance->SetAmmo(maxAmmo, ammo);
	}
	short       CompactShipFractionsWrapper::GetAmmo(short maxAmmo)
	{
		return m_instance->GetAmmo(maxAmmo);
	}
	void        CompactShipFractionsWrapper::SetEnergy(float maxEnergy, float energy)
	{
		return m_instance->SetEnergy(maxEnergy, energy);
	}
	float        CompactShipFractionsWrapper::GetEnergy(float maxEnergy)
	{
		return m_instance->GetEnergy(maxEnergy);
	}
	float        CompactShipFractionsWrapper::GetOre(float maxOre)
	{
		return m_instance->GetOre(maxOre);
	}
	void        CompactShipFractionsWrapper::SetOre(float maxOre, float ore)
	{
		return m_instance->SetOre(maxOre, ore);
	}
	ServerLightShipUpdateWrapper::ServerLightShipUpdateWrapper(::ServerLightShipUpdate * instance)
	{
		m_instance = instance;
	}
	ServerHeavyShipUpdateWrapper::ServerHeavyShipUpdateWrapper(::ServerHeavyShipUpdate * instance) : ServerLightShipUpdateWrapper(instance)
	{
		m_instance = instance;
	}
	ServerSingleShipUpdateWrapper::ServerSingleShipUpdateWrapper(::ServerSingleShipUpdate * instance) : ServerLightShipUpdateWrapper(instance)
	{
		m_instance = instance;
	}
	ClientShipUpdateWrapper::ClientShipUpdateWrapper(::ClientShipUpdate * instance)
	{
		m_instance = instance;
	}
	ClientActiveTurretUpdateWrapper::ClientActiveTurretUpdateWrapper(::ClientActiveTurretUpdate * instance)
	{
		m_instance = instance;
	}
	ServerActiveTurretUpdateWrapper::ServerActiveTurretUpdateWrapper(::ServerActiveTurretUpdate * instance)
	{
		m_instance = instance;
	}
	IstaticIGCWrapper::IstaticIGCWrapper(::IstaticIGC * instance)
	{
		m_instance = instance;
	}

	
	void                            IstaticIGCWrapper::Initialize()
	{
		return m_instance->Initialize();
	}
	void                            IstaticIGCWrapper::Terminate()
	{
		return m_instance->Terminate();
	}
	float                           IstaticIGCWrapper::GetFloatConstant(FloatConstantID fcid)
	{
		return m_instance->GetFloatConstant(fcid);
	}
	void                            IstaticIGCWrapper::SetFloatConstant(FloatConstantID fcid, float f)
	{
		return m_instance->SetFloatConstant(fcid, f);
	}
	float                           IstaticIGCWrapper::GetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid)
	{
		return m_instance->GetDamageConstant(dmgid, defid);
	}
	void                            IstaticIGCWrapper::SetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid, float f)
	{
		return m_instance->SetDamageConstant(dmgid, defid, f);
	}
	const void*                     IstaticIGCWrapper::GetConstants()
	{
		return m_instance->GetConstants();
	}
	void                            IstaticIGCWrapper::SetConstants(const void* pf)
	{
		return m_instance->SetConstants(pf);
	}
	int                             IstaticIGCWrapper::GetSizeOfConstants()
	{
		return m_instance->GetSizeOfConstants();
	}
	void                            IstaticIGCWrapper::AddDroneType(IdroneTypeIGCWrapper^ dt)
	{
		return m_instance->AddDroneType(dt->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteDroneType(IdroneTypeIGCWrapper^ dt)
	{
		return m_instance->DeleteDroneType(dt->m_instance);
	}
	IdroneTypeIGCWrapper ^ IstaticIGCWrapper::GetDroneType(DroneTypeID id)
	{
		return gcnew IdroneTypeIGCWrapper((::IdroneTypeIGC *) m_instance->GetDroneType(id));
	}
	const DroneTypeListIGC*         IstaticIGCWrapper::GetDroneTypes()
	{
		return m_instance->GetDroneTypes();
	}
	void                            IstaticIGCWrapper::AddDevelopment(IdevelopmentIGCWrapper^ d)
	{
		return m_instance->AddDevelopment(d->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteDevelopment(IdevelopmentIGCWrapper^ d)
	{
		return m_instance->DeleteDevelopment(d->m_instance);
	}
	IdevelopmentIGCWrapper ^ IstaticIGCWrapper::GetDevelopment(DevelopmentID id)
	{
		return gcnew IdevelopmentIGCWrapper((::IdevelopmentIGC *) m_instance->GetDevelopment(id));
	}
	const DevelopmentListIGC*       IstaticIGCWrapper::GetDevelopments()
	{
		return m_instance->GetDevelopments();
	}
	void                            IstaticIGCWrapper::AddStationType(IstationTypeIGCWrapper^ st)
	{
		return m_instance->AddStationType(st->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteStationType(IstationTypeIGCWrapper^ st)
	{
		return m_instance->DeleteStationType(st->m_instance);
	}
	IstationTypeIGCWrapper ^ IstaticIGCWrapper::GetStationType(StationTypeID id)
	{
		return gcnew IstationTypeIGCWrapper((::IstationTypeIGC *) m_instance->GetStationType(id));
	}
	const StationTypeListIGC*       IstaticIGCWrapper::GetStationTypes()
	{
		return m_instance->GetStationTypes();
	}
	void                            IstaticIGCWrapper::AddCivilization(IcivilizationIGCWrapper^ c)
	{
		return m_instance->AddCivilization(c->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteCivilization(IcivilizationIGCWrapper^ c)
	{
		return m_instance->DeleteCivilization(c->m_instance);
	}
	IcivilizationIGCWrapper ^ IstaticIGCWrapper::GetCivilization(CivID id)
	{
		return gcnew IcivilizationIGCWrapper((::IcivilizationIGC *) m_instance->GetCivilization(id));
	}
	const CivilizationListIGC*      IstaticIGCWrapper::GetCivilizations()
	{
		return m_instance->GetCivilizations();
	}
	void                            IstaticIGCWrapper::AddHullType(IhullTypeIGCWrapper^ ht)
	{
		return m_instance->AddHullType(ht->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteHullType(IhullTypeIGCWrapper^ ht)
	{
		return m_instance->DeleteHullType(ht->m_instance);
	}
	IhullTypeIGCWrapper ^ IstaticIGCWrapper::GetHullType(HullID id)
	{
		return gcnew IhullTypeIGCWrapper((::IhullTypeIGC *) m_instance->GetHullType(id));
	}
	System::Collections::Generic::List<IhullTypeIGCWrapper ^> ^ IstaticIGCWrapper::GetHullTypes()
	{
		System::Collections::Generic::List<IhullTypeIGCWrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<IhullTypeIGCWrapper ^>();

		for (HullTypeLinkIGC* htl = m_instance->GetHullTypes()->first(); (htl != NULL); htl = htl->next())
		{
			returnValue->Add(gcnew IhullTypeIGCWrapper(htl->data()));
		}

		return returnValue;
	}
	void                            IstaticIGCWrapper::AddExpendableType(IexpendableTypeIGCWrapper^ mt)
	{
		return m_instance->AddExpendableType(mt->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteExpendableType(IexpendableTypeIGCWrapper^ mt)
	{
		return m_instance->DeleteExpendableType(mt->m_instance);
	}
	IexpendableTypeIGCWrapper ^ IstaticIGCWrapper::GetExpendableType(ExpendableTypeID id)
	{
		return gcnew IexpendableTypeIGCWrapper((::IexpendableTypeIGC *) m_instance->GetExpendableType(id));
	}
	const ExpendableTypeListIGC*    IstaticIGCWrapper::GetExpendableTypes()
	{
		return m_instance->GetExpendableTypes();
	}
	void                            IstaticIGCWrapper::AddPartType(IpartTypeIGCWrapper^ pt)
	{
		return m_instance->AddPartType(pt->m_instance);
	}
	void                            IstaticIGCWrapper::DeletePartType(IpartTypeIGCWrapper^ pt)
	{
		return m_instance->DeletePartType(pt->m_instance);
	}
	IpartTypeIGCWrapper ^ IstaticIGCWrapper::GetPartType(PartID id)
	{
		return gcnew IpartTypeIGCWrapper((::IpartTypeIGC *) m_instance->GetPartType(id));
	}
	System::Collections::Generic::List<IpartTypeIGCWrapper ^> ^          IstaticIGCWrapper::GetPartTypes()
	{
		System::Collections::Generic::List<IpartTypeIGCWrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<IpartTypeIGCWrapper ^>();

		for (PartTypeLinkIGC* htl = m_instance->GetPartTypes()->first(); (htl != NULL); htl = htl->next())
		{
			returnValue->Add(gcnew IpartTypeIGCWrapper(htl->data()));
		}

		return returnValue;
	}
	void                            IstaticIGCWrapper::AddProjectileType(IprojectileTypeIGCWrapper^ pt)
	{
		return m_instance->AddProjectileType(pt->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteProjectileType(IprojectileTypeIGCWrapper^ pt)
	{
		return m_instance->DeleteProjectileType(pt->m_instance);
	}
	IprojectileTypeIGCWrapper ^ IstaticIGCWrapper::GetProjectileType(ProjectileTypeID id)
	{
		return gcnew IprojectileTypeIGCWrapper((::IprojectileTypeIGC *) m_instance->GetProjectileType(id));
	}
	const ProjectileTypeListIGC*    IstaticIGCWrapper::GetProjectileTypes()
	{
		return m_instance->GetProjectileTypes();
	}
	void                            IstaticIGCWrapper::AddTreasureSet(ItreasureSetIGCWrapper^ pt)
	{
		return m_instance->AddTreasureSet(pt->m_instance);
	}
	void                            IstaticIGCWrapper::DeleteTreasureSet(ItreasureSetIGCWrapper^ pt)
	{
		return m_instance->DeleteTreasureSet(pt->m_instance);
	}
	ItreasureSetIGCWrapper ^ IstaticIGCWrapper::GetTreasureSet(TreasureSetID id)
	{
		return gcnew ItreasureSetIGCWrapper((::ItreasureSetIGC *) m_instance->GetTreasureSet(id));
	}
	const TreasureSetListIGC*       IstaticIGCWrapper::GetTreasureSets()
	{
		return m_instance->GetTreasureSets();
	}
	IpartTypeIGCWrapper ^ IstaticIGCWrapper::GetAmmoPack()
	{
		return gcnew IpartTypeIGCWrapper((::IpartTypeIGC *) m_instance->GetAmmoPack());
	}
	IpartTypeIGCWrapper ^ IstaticIGCWrapper::GetFuelPack()
	{
		return gcnew IpartTypeIGCWrapper((::IpartTypeIGC *) m_instance->GetFuelPack());
	}

	


	ImissionIGCWrapper::ImissionIGCWrapper(::ImissionIGC * instance) : IstaticIGCWrapper(instance)
	{
		m_instance = instance;
	}
	ZString				 ImissionIGCWrapper::BitsToTechsList(TechTreeBitMask & ttbm)
	{
		return m_instance->BitsToTechsList(ttbm);
	}
	void				 ImissionIGCWrapper::TechsListToBits(const char * szTechs, TechTreeBitMask & ttbm)
	{
		return m_instance->TechsListToBits(szTechs, ttbm);
	}
	ZString				 ImissionIGCWrapper::BitsToPartsList(PartMask & pm, EquipmentType et)
	{
		return m_instance->BitsToPartsList(pm, et);
	}
	PartMask			 ImissionIGCWrapper::PartMaskFromToken(const char * szToken, EquipmentType et)
	{
		return m_instance->PartMaskFromToken(szToken, et);
	}
	PartMask			 ImissionIGCWrapper::PartsListToMask(const char * szParts, EquipmentType et)
	{
		return m_instance->PartsListToMask(szParts, et);
	}
	bool				 ImissionIGCWrapper::LoadTechBitsList()
	{
		return m_instance->LoadTechBitsList();
	}
	bool				 ImissionIGCWrapper::LoadPartsBitsList()
	{
		return m_instance->LoadPartsBitsList();
	}
	void				 ImissionIGCWrapper::ExportStaticIGCObjs()
	{
		return m_instance->ExportStaticIGCObjs();
	}
	void				 ImissionIGCWrapper::ImportStaticIGCObjs()
	{
		return m_instance->ImportStaticIGCObjs();
	}
	MissionID               ImissionIGCWrapper::GetMissionID()
	{
		return m_instance->GetMissionID();
	}
	void                    ImissionIGCWrapper::SetMissionID(MissionID mid)
	{
		return m_instance->SetMissionID(mid);
	}
	void                    ImissionIGCWrapper::Initialize(Time now, IIgcSiteWrapper^ pIgcSite)
	{
		return m_instance->Initialize(now, pIgcSite->m_instance);
	}
	void                    ImissionIGCWrapper::Terminate()
	{
		return m_instance->Terminate();
	}
	void                    ImissionIGCWrapper::Update(Time now)
	{
		return m_instance->Update(now);
	}
	IIgcSiteWrapper ^ ImissionIGCWrapper::GetIgcSite()
	{
		::IIgcSite * unmanagedValue = (::IIgcSite *) m_instance->GetIgcSite();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IIgcSiteWrapper(unmanagedValue);
	}
	ImodelIGCWrapper ^ ImissionIGCWrapper::GetModel(ObjectType type, ObjectID id)
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->GetModel(type, id);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	IbaseIGCWrapper ^ ImissionIGCWrapper::GetBase(ObjectType type, ObjectID id)
	{
		::IbaseIGC * unmanagedValue = (::IbaseIGC *) m_instance->GetBase(type, id);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbaseIGCWrapper(unmanagedValue);
	}
	DamageTrackWrapper ^ ImissionIGCWrapper::CreateDamageTrack()
	{
		::DamageTrack * unmanagedValue = (::DamageTrack *) m_instance->CreateDamageTrack();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew DamageTrackWrapper(unmanagedValue);
	}
	void                    ImissionIGCWrapper::AddSide(IsideIGCWrapper^ s)
	{
		return m_instance->AddSide(s->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteSide(IsideIGCWrapper^ s)
	{
		return m_instance->DeleteSide(s->m_instance);
	}
	IsideIGCWrapper ^ ImissionIGCWrapper::GetSide(SideID sideID)
	{
		::IsideIGC * unmanagedValue = (::IsideIGC *) m_instance->GetSide(sideID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IsideIGCWrapper(unmanagedValue);
	}
	const SideListIGC*      ImissionIGCWrapper::GetSides()
	{
		return m_instance->GetSides();
	}/*
	 void                    ImissionIGCWrapper::GetSeenSides(ImodelIGCWrapper ^ pmodel, bool (& bseensides)
	 {
	 return m_instance->GetSeenSides(pmodel->m_instance, bseensides);
	 }*/
	void                    ImissionIGCWrapper::AddCluster(IclusterIGCWrapper^ c)
	{
		return m_instance->AddCluster(c->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteCluster(IclusterIGCWrapper^ c)
	{
		return m_instance->DeleteCluster(c->m_instance);
	}
	IclusterIGCWrapper ^ ImissionIGCWrapper::GetCluster(SectorID clusterID)
	{
		::IclusterIGC * unmanagedValue = (::IclusterIGC *) m_instance->GetCluster(clusterID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IclusterIGCWrapper(unmanagedValue);
	}

	System::Collections::Generic::List<IclusterIGCWrapper ^> ^   ImissionIGCWrapper::GetClusters()
	{
		ConvertSList(IclusterIGC, ClusterLinkIGC, m_instance->GetClusters())

		/*System::Collections::Generic::List<IclusterIGCWrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<IclusterIGCWrapper ^>();

		for (ClusterLinkIGC* pbl = m_instance->GetClusters()->last(); (pbl != NULL); pbl = pbl->txen())
		{
			IclusterIGC*     item = pbl->data();
			returnValue->Add(gcnew IclusterIGCWrapper(item));
		}
		return returnValue;*/
	}

	void                    ImissionIGCWrapper::AddShip(IshipIGCWrapper^ s)
	{
		return m_instance->AddShip(s->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteShip(IshipIGCWrapper^ s)
	{
		return m_instance->DeleteShip(s->m_instance);
	}
	IshipIGCWrapper ^ ImissionIGCWrapper::GetShip(ShipID shipID)
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetShip(shipID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	GenericList(IshipIGCWrapper) ImissionIGCWrapper::GetShips()
	{
		ConvertSList(IshipIGC, ShipLinkIGC, m_instance->GetShips())
	}
	void                    ImissionIGCWrapper::AddStation(IstationIGCWrapper^ s)
	{
		return m_instance->AddStation(s->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteStation(IstationIGCWrapper^ s)
	{
		return m_instance->DeleteStation(s->m_instance);
	}
	IstationIGCWrapper ^ ImissionIGCWrapper::GetStation(StationID stationID)
	{
		::IstationIGC * unmanagedValue = (::IstationIGC *) m_instance->GetStation(stationID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationIGCWrapper(unmanagedValue);
	}
	GenericList(IstationIGCWrapper) ImissionIGCWrapper::GetStations()
	{
		ConvertSList(IstationIGC, StationLinkIGC, m_instance->GetStations())
	}
	void                    ImissionIGCWrapper::AddAsteroid(IasteroidIGCWrapper^ p)
	{
		return m_instance->AddAsteroid(p->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteAsteroid(IasteroidIGCWrapper^ p)
	{
		return m_instance->DeleteAsteroid(p->m_instance);
	}
	IasteroidIGCWrapper ^ ImissionIGCWrapper::GetAsteroid(AsteroidID asteroidID)
	{
		::IasteroidIGC * unmanagedValue = (::IasteroidIGC *) m_instance->GetAsteroid(asteroidID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IasteroidIGCWrapper(unmanagedValue);
	}
	const AsteroidListIGC*  ImissionIGCWrapper::GetAsteroids()
	{
		return m_instance->GetAsteroids();
	}
	void                    ImissionIGCWrapper::AddWarp(IwarpIGCWrapper^ w)
	{
		return m_instance->AddWarp(w->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteWarp(IwarpIGCWrapper^ w)
	{
		return m_instance->DeleteWarp(w->m_instance);
	}
	IwarpIGCWrapper ^ ImissionIGCWrapper::GetWarp(WarpID warpID)
	{
		::IwarpIGC * unmanagedValue = (::IwarpIGC *) m_instance->GetWarp(warpID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IwarpIGCWrapper(unmanagedValue);
	}
	System::Collections::Generic::List<IwarpIGCWrapper ^> ^      ImissionIGCWrapper::GetWarps()
	{
		ConvertSList(IwarpIGC, WarpLinkIGC, m_instance->GetWarps())
		
	}
	void                    ImissionIGCWrapper::AddBuoy(IbuoyIGCWrapper^ t)
	{
		return m_instance->AddBuoy(t->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteBuoy(IbuoyIGCWrapper^ t)
	{
		return m_instance->DeleteBuoy(t->m_instance);
	}
	IbuoyIGCWrapper ^ ImissionIGCWrapper::GetBuoy(BuoyID buoyID)
	{
		::IbuoyIGC * unmanagedValue = (::IbuoyIGC *) m_instance->GetBuoy(buoyID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbuoyIGCWrapper(unmanagedValue);
	}
	const BuoyListIGC*      ImissionIGCWrapper::GetBuoys()
	{
		return m_instance->GetBuoys();
	}
	void                    ImissionIGCWrapper::AddTreasure(ItreasureIGCWrapper^ t)
	{
		return m_instance->AddTreasure(t->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteTreasure(ItreasureIGCWrapper^ t)
	{
		return m_instance->DeleteTreasure(t->m_instance);
	}
	ItreasureIGCWrapper ^ ImissionIGCWrapper::GetTreasure(TreasureID treasureID)
	{
		::ItreasureIGC * unmanagedValue = (::ItreasureIGC *) m_instance->GetTreasure(treasureID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ItreasureIGCWrapper(unmanagedValue);
	}
	const TreasureListIGC*  ImissionIGCWrapper::GetTreasures()
	{
		return m_instance->GetTreasures();
	}
	void                    ImissionIGCWrapper::AddMine(ImineIGCWrapper^ ms)
	{
		return m_instance->AddMine(ms->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteMine(ImineIGCWrapper^ m)
	{
		return m_instance->DeleteMine(m->m_instance);
	}
	ImineIGCWrapper ^ ImissionIGCWrapper::GetMine(MineID mineID)
	{
		::ImineIGC * unmanagedValue = (::ImineIGC *) m_instance->GetMine(mineID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImineIGCWrapper(unmanagedValue);
	}
	const MineListIGC*      ImissionIGCWrapper::GetMines()
	{
		return m_instance->GetMines();
	}
	void                    ImissionIGCWrapper::AddProbe(IprobeIGCWrapper^ ms)
	{
		return m_instance->AddProbe(ms->m_instance);
	}
	void                    ImissionIGCWrapper::DeleteProbe(IprobeIGCWrapper^ m)
	{
		return m_instance->DeleteProbe(m->m_instance);
	}
	IprobeIGCWrapper ^ ImissionIGCWrapper::GetProbe(ProbeID probeID)
	{
		::IprobeIGC * unmanagedValue = (::IprobeIGC *) m_instance->GetProbe(probeID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprobeIGCWrapper(unmanagedValue);
	}
	const ProbeListIGC*     ImissionIGCWrapper::GetProbes()
	{
		return m_instance->GetProbes();
	}
	BuoyID                  ImissionIGCWrapper::GenerateNewBuoyID()
	{
		return m_instance->GenerateNewBuoyID();
	}
	ShipID                  ImissionIGCWrapper::GenerateNewShipID()
	{
		return m_instance->GenerateNewShipID();
	}
	AsteroidID              ImissionIGCWrapper::GenerateNewAsteroidID()
	{
		return m_instance->GenerateNewAsteroidID();
	}
	StationID               ImissionIGCWrapper::GenerateNewStationID()
	{
		return m_instance->GenerateNewStationID();
	}
	TreasureID              ImissionIGCWrapper::GenerateNewTreasureID()
	{
		return m_instance->GenerateNewTreasureID();
	}
	MineID                  ImissionIGCWrapper::GenerateNewMineID()
	{
		return m_instance->GenerateNewMineID();
	}
	ProbeID                 ImissionIGCWrapper::GenerateNewProbeID()
	{
		return m_instance->GenerateNewProbeID();
	}
	MissileID               ImissionIGCWrapper::GenerateNewMissileID()
	{
		return m_instance->GenerateNewMissileID();
	}
	void                    ImissionIGCWrapper::SetMissionStage(STAGE st)
	{
		return m_instance->SetMissionStage(st);
	}
	AllegianceInterop::MissionStage ImissionIGCWrapper::GetMissionStage()
	{
		return (AllegianceInterop::MissionStage) m_instance->GetMissionStage();
	}
	void                    ImissionIGCWrapper::EnterGame()
	{
		return m_instance->EnterGame();
	}
	void                    ImissionIGCWrapper::ResetMission()
	{
		return m_instance->ResetMission();
	}
	void                    ImissionIGCWrapper::SetPrivateData(DWORD dwPrivate)
	{
		return m_instance->SetPrivateData(dwPrivate);
	}
	DWORD                   ImissionIGCWrapper::GetPrivateData()
	{
		return m_instance->GetPrivateData();
	}
	Time                    ImissionIGCWrapper::GetLastUpdate()
	{
		return m_instance->GetLastUpdate();
	}
	MissionParamsWrapper ^ ImissionIGCWrapper::GetMissionParams()
	{
		::MissionParams * unmanagedValue = (::MissionParams *) m_instance->GetMissionParams();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew MissionParamsWrapper(unmanagedValue);
	}
	void                    ImissionIGCWrapper::SetMissionParams(MissionParamsWrapper^ pmp)
	{
		return m_instance->SetMissionParams(pmp->m_instance);
	}
	void                    ImissionIGCWrapper::SetStartTime(Time timeStart)
	{
		return m_instance->SetStartTime(timeStart);
	}
	CstaticIGC*             ImissionIGCWrapper::GetStaticCore()
	{
		return m_instance->GetStaticCore();
	}
	void                    ImissionIGCWrapper::SetStaticCore(CstaticIGC* pStatic)
	{
		return m_instance->SetStaticCore(pStatic);
	}
	short                   ImissionIGCWrapper::GetReplayCount()
	{
		return m_instance->GetReplayCount();
	}
	String ^ ImissionIGCWrapper::GetContextName()
	{
		return gcnew String(m_instance->GetContextName());
	}
	void                    ImissionIGCWrapper::UpdateAllies(const char Allies[c_cSidesMax])
	{
		return m_instance->UpdateAllies(Allies);
	}
	IbaseIGCWrapper::IbaseIGCWrapper(::IbaseIGC * instance)
	{
		m_instance = instance;
	}
	HRESULT         IbaseIGCWrapper::Initialize(ImissionIGCWrapper^ pMission, Time now, const void* data, int length)
	{
		return m_instance->Initialize(pMission->m_instance, now, data, length);
	}
	void            IbaseIGCWrapper::Terminate()
	{
		return m_instance->Terminate();
	}
	void            IbaseIGCWrapper::Update(Time now)
	{
		return m_instance->Update(now);
	}
	int             IbaseIGCWrapper::Export(void* data)
	{
		return m_instance->Export(data);
	}
	int             IbaseIGCWrapper::GetUniqueID()
	{
		return m_instance->GetUniqueID();
	}
	ObjectType      IbaseIGCWrapper::GetObjectType()
	{
		return m_instance->GetObjectType();
	}
	ObjectID        IbaseIGCWrapper::GetObjectID()
	{
		return m_instance->GetObjectID();
	}
	ImissionIGCWrapper ^ IbaseIGCWrapper::GetMission()
	{
		::ImissionIGC * unmanagedValue = (::ImissionIGC *) m_instance->GetMission();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImissionIGCWrapper(unmanagedValue);
	}
	ItypeIGCWrapper::ItypeIGCWrapper(::ItypeIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	const void*     ItypeIGCWrapper::GetData()
	{
		return m_instance->GetData();
	}
	AttachSiteWrapper::AttachSiteWrapper(::AttachSite * instance)
	{
		m_instance = instance;
	}
	void AttachSiteWrapper::Terminate()
	{
		return m_instance->Terminate();
	}
	void AttachSiteWrapper::AddExplosion(ImodelIGCWrapper^ pmodel, int type)
	{
		return m_instance->AddExplosion(pmodel->m_instance, type);
	}
	void AttachSiteWrapper::AddExplosion(const VectorWrapper ^ vecPosition, float scale, int type)
	{
		return m_instance->AddExplosion(*vecPosition->m_instance, scale, type);
	}
	void AttachSiteWrapper::AddThingSite(ThingSiteWrapper^ thing)
	{
		return m_instance->AddThingSite(thing->m_instance);
	}
	void AttachSiteWrapper::DeleteThingSite(ThingSiteWrapper^ thing)
	{
		return m_instance->DeleteThingSite(thing->m_instance);
	}
	ThingSiteWrapper::ThingSiteWrapper(::ThingSite * instance) : AttachSiteWrapper(instance)
	{
		m_instance = instance;
	}
	void        ThingSiteWrapper::Purge()
	{
		return m_instance->Purge();
	}
	Vector      ThingSiteWrapper::GetChildModelOffset(const ZString& strFrame)
	{
		return m_instance->GetChildModelOffset(strFrame);
	}
	Vector      ThingSiteWrapper::GetChildOffset(const ZString& strFrame)
	{
		return m_instance->GetChildOffset(strFrame);
	}
	void        ThingSiteWrapper::AddHullHit(const VectorWrapper ^ vecPosition, const VectorWrapper ^ vecNormal)
	{
		return m_instance->AddHullHit(*vecPosition->m_instance, *vecNormal->m_instance);
	}
	void        ThingSiteWrapper::AddFlare(Time ptime, const VectorWrapper ^ vecPosition, int id, const Vector* ellipseEquation)
	{
		return m_instance->AddFlare(ptime, *vecPosition->m_instance, id, ellipseEquation);
	}
	void        ThingSiteWrapper::AddMuzzleFlare(const VectorWrapper ^ vecEmissionPoint, float duration)
	{
		return m_instance->AddMuzzleFlare(*vecEmissionPoint->m_instance, duration);
	}
	void        ThingSiteWrapper::SetVisible(unsigned char render)
	{
		return m_instance->SetVisible(render);
	}
	float           ThingSiteWrapper::GetDistanceToEdge()
	{
		return m_instance->GetDistanceToEdge();
	}
	float           ThingSiteWrapper::GetScreenRadius()
	{
		return m_instance->GetScreenRadius();
	}
	unsigned char   ThingSiteWrapper::GetRadarState()
	{
		return m_instance->GetRadarState();
	}
	ThingGeo*   ThingSiteWrapper::GetThingGeo()
	{
		return m_instance->GetThingGeo();
	}
	GeoWrapper ^ ThingSiteWrapper::GetGeo()
	{
		::Geo * unmanagedValue = (::Geo *) m_instance->GetGeo();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew GeoWrapper(unmanagedValue);
	}
	void        ThingSiteWrapper::SetPosition(const VectorWrapper ^ position)
	{
		return m_instance->SetPosition(*position->m_instance);
	}
	float       ThingSiteWrapper::GetRadius()
	{
		return m_instance->GetRadius();
	}
	void        ThingSiteWrapper::SetRadius(float r)
	{
		return m_instance->SetRadius(r);
	}
	void        ThingSiteWrapper::SetOrientation(const Orientation& orientation)
	{
		return m_instance->SetOrientation(orientation);
	}
	void        ThingSiteWrapper::Spin(float r)
	{
		return m_instance->Spin(r);
	}
	void        ThingSiteWrapper::SetTexture(const char* pszTextureName)
	{
		return m_instance->SetTexture(pszTextureName);
	}
	void        ThingSiteWrapper::SetTrailColor(const Color& c)
	{
		return m_instance->SetTrailColor(c);
	}
	HRESULT     ThingSiteWrapper::LoadDecal(const char* textureName, bool bDirectional, float width)
	{
		return m_instance->LoadDecal(textureName, bDirectional, width);
	}
	HRESULT     ThingSiteWrapper::LoadModel(int options, const char* modelName, const char* textureName)
	{
		return m_instance->LoadModel(options, modelName, textureName);
	}
	HRESULT     ThingSiteWrapper::LoadAleph(const char* textureName)
	{
		return m_instance->LoadAleph(textureName);
	}
	HRESULT     ThingSiteWrapper::LoadEffect(const Color& color)
	{
		return m_instance->LoadEffect(color);
	}
	void        ThingSiteWrapper::SetColors(float aInner, float fInner, float fOuter)
	{
		return m_instance->SetColors(aInner, fInner, fOuter);
	}
	HRESULT     ThingSiteWrapper::LoadMine(const char* textureName, float strength, float radius)
	{
		return m_instance->LoadMine(textureName, strength, radius);
	}
	void        ThingSiteWrapper::SetMineStrength(float strength)
	{
		return m_instance->SetMineStrength(strength);
	}
	ImodelIGCWrapper::ImodelIGCWrapper(::ImodelIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	Time                 ImodelIGCWrapper::GetLastUpdate()
	{
		return m_instance->GetLastUpdate();
	}
	void                 ImodelIGCWrapper::SetLastUpdate(Time now)
	{
		return m_instance->SetLastUpdate(now);
	}
	void                 ImodelIGCWrapper::SetMission(ImissionIGCWrapper^ pmission)
	{
		return m_instance->SetMission(pmission->m_instance);
	}
	bool                 ImodelIGCWrapper::GetVisibleF()
	{
		return m_instance->GetVisibleF();
	}
	void                 ImodelIGCWrapper::SetVisibleF(bool visibleF)
	{
		return m_instance->SetVisibleF(visibleF);
	}
	void                 ImodelIGCWrapper::SetRender(unsigned char render)
	{
		return m_instance->SetRender(render);
	}
	bool                 ImodelIGCWrapper::SeenBySide(IsideIGCWrapper^ side)
	{
		return m_instance->SeenBySide(side->m_instance);
	}
	bool                 ImodelIGCWrapper::GetCurrentEye(IsideIGCWrapper^ side)
	{
		return m_instance->GetCurrentEye(side->m_instance);
	}
	void                 ImodelIGCWrapper::UpdateSeenBySide()
	{
		return m_instance->UpdateSeenBySide();
	}
	void                 ImodelIGCWrapper::SetSideVisibility(IsideIGCWrapper^ side, bool fVisible)
	{
		return m_instance->SetSideVisibility(side->m_instance, fVisible);
	}
	ThingSiteWrapper ^ ImodelIGCWrapper::GetThingSite()
	{
		::ThingSite * unmanagedValue = (::ThingSite *) m_instance->GetThingSite();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ThingSiteWrapper(unmanagedValue);
	}
	void                 ImodelIGCWrapper::FreeThingSite()
	{
		return m_instance->FreeThingSite();
	}
	void                 ImodelIGCWrapper::SetPosition(const VectorWrapper ^ newVal)
	{
		return m_instance->SetPosition(*newVal->m_instance);
	}
	void                 ImodelIGCWrapper::SetVelocity(const VectorWrapper ^ newVal)
	{
		return m_instance->SetVelocity(*newVal->m_instance);
	}
	void                 ImodelIGCWrapper::SetOrientation(const Orientation& newVal)
	{
		return m_instance->SetOrientation(newVal);
	}
	void                 ImodelIGCWrapper::SetRotation(const Rotation& newVal)
	{
		return m_instance->SetRotation(newVal);
	}
	float                ImodelIGCWrapper::GetRadius()
	{
		return m_instance->GetRadius();
	}
	void                 ImodelIGCWrapper::SetRadius(float newVal)
	{
		return m_instance->SetRadius(newVal);
	}
	ModelAttributes      ImodelIGCWrapper::GetAttributes()
	{
		return m_instance->GetAttributes();
	}
	IsideIGCWrapper ^ ImodelIGCWrapper::GetSide()
	{
		::IsideIGC * unmanagedValue = (::IsideIGC *) m_instance->GetSide();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IsideIGCWrapper(unmanagedValue);
	}
	void                 ImodelIGCWrapper::SetSide(IsideIGCWrapper^ newVal)
	{
		return m_instance->SetSide(newVal->m_instance);
	}
	float                ImodelIGCWrapper::GetMass()
	{
		return m_instance->GetMass();
	}
	void                 ImodelIGCWrapper::SetMass(float newVal)
	{
		return m_instance->SetMass(newVal);
	}
	IclusterIGCWrapper ^ ImodelIGCWrapper::GetCluster()
	{
		::IclusterIGC * unmanagedValue = (::IclusterIGC *) m_instance->GetCluster();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IclusterIGCWrapper(unmanagedValue);
	}
	void                 ImodelIGCWrapper::SetCluster(IclusterIGCWrapper^ pVal)
	{
		if (pVal == nullptr)
			return m_instance->SetCluster(nullptr);
		else
			return m_instance->SetCluster(pVal->m_instance);
	}
	String ^ ImodelIGCWrapper::GetName()
	{
		const char * name = m_instance->GetName();

		if (name[0] == '\0')
			return GetSecondaryName();

		return gcnew String(name);
	}

	String ^ ImodelIGCWrapper::GetSecondaryName()
	{
		const char * name = m_instance->GetName();

		return gcnew String(name + strlen(name) + 1); // This is how common asteroid names are stored... I guess they were optimizing for dialup??

		//if (m_instance->GetName()[0] == '\0')
		//	return gcnew String(m_instance->GetName() + 1); // This is how common asteroid names are stored... I guess they were optimizing for dialup??
		//else
		//	return nullptr;
	}

	void                 ImodelIGCWrapper::SetName(const char* newVal)
	{
		return m_instance->SetName(newVal);
	}
	void                 ImodelIGCWrapper::SetSecondaryName(const char* newVal)
	{
		return m_instance->SetSecondaryName(newVal);
	}
	float                ImodelIGCWrapper::GetSignature()
	{
		return m_instance->GetSignature();
	}
	void                 ImodelIGCWrapper::SetSignature(float newVal)
	{
		return m_instance->SetSignature(newVal);
	}
	void                 ImodelIGCWrapper::ChangeSignature(float delta)
	{
		return m_instance->ChangeSignature(delta);
	}
	void                 ImodelIGCWrapper::SetTexture(const char* pszTextureName)
	{
		return m_instance->SetTexture(pszTextureName);
	}
	void                 ImodelIGCWrapper::Move(float t)
	{
		return m_instance->Move(t);
	}
	void                 ImodelIGCWrapper::Move()
	{
		return m_instance->Move();
	}
	void                 ImodelIGCWrapper::SetBB(Time tStart, Time tStop, float dt)
	{
		return m_instance->SetBB(tStart, tStop, dt);
	}
	HitTest*             ImodelIGCWrapper::GetHitTest()
	{
		return m_instance->GetHitTest();
	}
	IObject*             ImodelIGCWrapper::GetIcon()
	{
		return m_instance->GetIcon();
	}
	void                 ImodelIGCWrapper::SetIcon(IObject* picon)
	{
		return m_instance->SetIcon(picon);
	}
	SideID               ImodelIGCWrapper::GetFlag()
	{
		return m_instance->GetFlag();
	}
	IdamageIGCWrapper::IdamageIGCWrapper(::IdamageIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float           IdamageIGCWrapper::GetFraction()
	{
		return m_instance->GetFraction();
	}
	void            IdamageIGCWrapper::SetFraction(float newVal)
	{
		return m_instance->SetFraction(newVal);
	}
	float           IdamageIGCWrapper::GetHitPoints()
	{
		return m_instance->GetHitPoints();
	}
	IscannerIGCWrapper::IscannerIGCWrapper(::IscannerIGC * instance) : IdamageIGCWrapper(instance)
	{
		m_instance = instance;
	}
	bool                 IscannerIGCWrapper::InScannerRange(ImodelIGCWrapper^ pModel)
	{
		return m_instance->InScannerRange(pModel->m_instance);
	}
	bool                 IscannerIGCWrapper::CanSee(ImodelIGCWrapper^ pModel)
	{
		return m_instance->CanSee(pModel->m_instance);
	}
	IshipIGCWrapper::IshipIGCWrapper(::IshipIGC * instance) : IscannerIGCWrapper(instance)
	{
		m_instance = instance;
	}
	void                 IshipIGCWrapper::SetObjectID(ObjectID id)
	{
		return m_instance->SetObjectID(id);
	}
	Money                IshipIGCWrapper::GetValue()
	{
		return m_instance->GetValue();
	}
	IstationIGCWrapper ^ IshipIGCWrapper::GetStation()
	{
		::IstationIGC * unmanagedValue = (::IstationIGC *) m_instance->GetStation();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationIGCWrapper(unmanagedValue);
	}
	void                 IshipIGCWrapper::SetStation(IstationIGCWrapper^ s)
	{
		if (s == nullptr)
			return m_instance->SetStation(nullptr);
		else 
			return m_instance->SetStation(s->m_instance);
	}
	void                 IshipIGCWrapper::Reset(bool bFull)
	{
		return m_instance->Reset(bFull);
	}
	float                IshipIGCWrapper::GetTorqueMultiplier()
	{
		return m_instance->GetTorqueMultiplier();
	}
	float                IshipIGCWrapper::GetCurrentTurnRate(Axis axis)
	{
		return m_instance->GetCurrentTurnRate(axis);
	}
	void                 IshipIGCWrapper::SetCurrentTurnRate(Axis axis, float newVal)
	{
		return m_instance->SetCurrentTurnRate(axis, newVal);
	}
	IhullTypeIGCWrapper ^ IshipIGCWrapper::GetHullType()
	{
		if (m_instance->HasValidHullType() == false)
			return nullptr;

		::IhullTypeIGC * unmanagedValue = (::IhullTypeIGC *) m_instance->GetHullType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IhullTypeIGCWrapper(unmanagedValue);
	}
	IhullTypeIGCWrapper ^ IshipIGCWrapper::GetBaseHullType()
	{
		::IhullTypeIGC * unmanagedValue = (::IhullTypeIGC *) m_instance->GetBaseHullType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IhullTypeIGCWrapper(unmanagedValue);
	}
	void                 IshipIGCWrapper::SetBaseHullType(IhullTypeIGCWrapper^ newVal)
	{
		if (newVal == nullptr)
			return m_instance->SetBaseHullType(nullptr);
		else
			return m_instance->SetBaseHullType(newVal->m_instance);
	}
	void                 IshipIGCWrapper::SetMission(ImissionIGCWrapper^ pMission)
	{
		return m_instance->SetMission(pMission->m_instance);
	}
	void                 IshipIGCWrapper::AddPart(IpartIGCWrapper^ part)
	{
		return m_instance->AddPart(part->m_instance);
	}
	void                 IshipIGCWrapper::DeletePart(IpartIGCWrapper^ part)
	{
		return m_instance->DeletePart(part->m_instance);
	}
	GenericList(IpartIGCWrapper) IshipIGCWrapper::GetParts()
	{
		ConvertSList(IpartIGC, PartLinkIGC, m_instance->GetParts())
	}
	IpartIGCWrapper ^ IshipIGCWrapper::GetMountedPart(EquipmentType type, Mount mountID)
	{
		::IpartIGC * unmanagedValue = (::IpartIGC *) m_instance->GetMountedPart(type, mountID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IpartIGCWrapper(unmanagedValue);
	}
	void                 IshipIGCWrapper::MountPart(IpartIGCWrapper^ p, Mount mountNew, Mount* pmountOld)
	{
		return m_instance->MountPart(p->m_instance, mountNew, pmountOld);
	}
	short                IshipIGCWrapper::GetAmmo()
	{
		return m_instance->GetAmmo();
	}
	void                 IshipIGCWrapper::SetAmmo(short amount)
	{
		return m_instance->SetAmmo(amount);
	}
	float                IshipIGCWrapper::GetFuel()
	{
		return m_instance->GetFuel();
	}
	void                 IshipIGCWrapper::SetFuel(float newVal)
	{
		return m_instance->SetFuel(newVal);
	}
	float                IshipIGCWrapper::GetEnergy()
	{
		return m_instance->GetEnergy();
	}
	void                 IshipIGCWrapper::SetEnergy(float newVal)
	{
		return m_instance->SetEnergy(newVal);
	}
	float                IshipIGCWrapper::GetCloaking()
	{
		return m_instance->GetCloaking();
	}
	void                 IshipIGCWrapper::SetCloaking(float newVal)
	{
		return m_instance->SetCloaking(newVal);
	}
	bool                 IshipIGCWrapper::GetVectorLock()
	{
		return m_instance->GetVectorLock();
	}
	void                 IshipIGCWrapper::SetVectorLock(bool bVectorLock)
	{
		return m_instance->SetVectorLock(bVectorLock);
	}
	void                 IshipIGCWrapper::SetControls(const ControlData& newVal)
	{
		return m_instance->SetControls(newVal);
	}
	bool                 IshipIGCWrapper::fRipcordActive()
	{
		return m_instance->fRipcordActive();
	}
	float                IshipIGCWrapper::GetRipcordTimeLeft()
	{
		return m_instance->GetRipcordTimeLeft();
	}
	void                 IshipIGCWrapper::ResetRipcordTimeLeft()
	{
		return m_instance->ResetRipcordTimeLeft();
	}
	int                  IshipIGCWrapper::GetStateM()
	{
		return m_instance->GetStateM();
	}
	void                 IshipIGCWrapper::SetStateM(int newVal)
	{
		return m_instance->SetStateM(newVal);
	}
	void                 IshipIGCWrapper::SetStateBits(int mask, int newBits)
	{
		return m_instance->SetStateBits(mask, newBits);
	}
	ImodelIGCWrapper ^ IshipIGCWrapper::GetCommandTarget(Command i)
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->GetCommandTarget(i);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	CommandID            IshipIGCWrapper::GetCommandID(Command i)
	{
		return m_instance->GetCommandID(i);
	}
	void                 IshipIGCWrapper::SetCommand(Command i, ImodelIGCWrapper^ target, CommandID cid)
	{
		if(target == nullptr)
			return m_instance->SetCommand(i, nullptr, cid);
		else
			return m_instance->SetCommand(i, target->m_instance, cid);
	}
	void                 IshipIGCWrapper::PreplotShipMove(Time timeStop)
	{
		return m_instance->PreplotShipMove(timeStop);
	}
	void                 IshipIGCWrapper::PlotShipMove(Time timeStop)
	{
		return m_instance->PlotShipMove(timeStop);
	}
	void                 IshipIGCWrapper::ExecuteShipMove(Time timeStop)
	{
		return m_instance->ExecuteShipMove(timeStop);
	}
	bool                IshipIGCWrapper::EquivalentShip(IshipIGCWrapper^ pship)
	{
		return m_instance->EquivalentShip(pship->m_instance);
	}
	short               IshipIGCWrapper::ExportShipLoadout(ShipLoadoutWrapper^ ploadout)
	{
		return m_instance->ExportShipLoadout(ploadout->m_instance);
	}
	void                IshipIGCWrapper::ExportFractions(CompactShipFractionsWrapper^ pfractions)
	{
		return m_instance->ExportFractions(pfractions->m_instance);
	}
	void                IshipIGCWrapper::ExportShipUpdate(ServerLightShipUpdateWrapper^ pshipupdate)
	{
		return m_instance->ExportShipUpdate(pshipupdate->m_instance);
	}
	void                IshipIGCWrapper::ExportShipUpdate(ServerSingleShipUpdateWrapper^ pshipupdate)
	{
		return m_instance->ExportShipUpdate(pshipupdate->m_instance);
	}
	void                IshipIGCWrapper::ExportShipUpdate(ClientShipUpdateWrapper^ pshipupdate)
	{
		return m_instance->ExportShipUpdate(pshipupdate->m_instance);
	}
	void                IshipIGCWrapper::ExportShipUpdate(ClientActiveTurretUpdateWrapper^ patu)
	{
		return m_instance->ExportShipUpdate(patu->m_instance);
	}
	void                IshipIGCWrapper::ProcessFractions(const CompactShipFractions& fractions)
	{
		return m_instance->ProcessFractions(fractions);
	}
	ShipUpdateStatus    IshipIGCWrapper::ProcessShipUpdate(const ServerLightShipUpdate& shipupdate)
	{
		return m_instance->ProcessShipUpdate(shipupdate);
	}
	ShipUpdateStatus    IshipIGCWrapper::ProcessShipUpdate(const ClientShipUpdate& shipupdate)
	{
		return m_instance->ProcessShipUpdate(shipupdate);
	}
	ShipUpdateStatus    IshipIGCWrapper::ProcessShipUpdate(const ClientActiveTurretUpdate& shipupdate)
	{
		return m_instance->ProcessShipUpdate(shipupdate);
	}
	void                IshipIGCWrapper::SetPrivateData(DWORD dwPrivate)
	{
		return m_instance->SetPrivateData(dwPrivate);
	}
	DWORD               IshipIGCWrapper::GetPrivateData()
	{
		return m_instance->GetPrivateData();
	}
	void                IshipIGCWrapper::ReInitialize(DataShipIGC * dataShip, Time now)
	{
		return m_instance->ReInitialize(dataShip, now);
	}
	Mount               IshipIGCWrapper::HitTreasure(TreasureCode treasureCode, ObjectID objectID, short amount)
	{
		return m_instance->HitTreasure(treasureCode, objectID, amount);
	}
	ImissileIGCWrapper ^ IshipIGCWrapper::GetLastMissileFired()
	{
		::ImissileIGC * unmanagedValue = (::ImissileIGC *) m_instance->GetLastMissileFired();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImissileIGCWrapper(unmanagedValue);
	}
	void                IshipIGCWrapper::SetLastMissileFired(ImissileIGCWrapper^ pmissile)
	{
		return m_instance->SetLastMissileFired(pmissile->m_instance);
	}
	Time                IshipIGCWrapper::GetLastTimeLaunched()
	{
		return m_instance->GetLastTimeLaunched();
	}
	void                IshipIGCWrapper::SetLastTimeLaunched(Time timeLastLaunch)
	{
		return m_instance->SetLastTimeLaunched(timeLastLaunch);
	}
	void                IshipIGCWrapper::SetLastTimeDocked(Time timeLastDock)
	{
		return m_instance->SetLastTimeDocked(timeLastDock);
	}
	Time                IshipIGCWrapper::GetLastTimeDocked()
	{
		return m_instance->GetLastTimeDocked();
	}
	void                IshipIGCWrapper::Promote()
	{
		return m_instance->Promote();
	}
	void                IshipIGCWrapper::SetParentShip(IshipIGCWrapper^ pship)
	{
		return m_instance->SetParentShip(pship->m_instance);
	}
	IshipIGCWrapper ^ IshipIGCWrapper::GetParentShip()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetParentShip();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	Mount               IshipIGCWrapper::GetTurretID()
	{
		return m_instance->GetTurretID();
	}
	void                IshipIGCWrapper::SetTurretID(Mount turretID)
	{
		return m_instance->SetTurretID(turretID);
	}
	IshipIGCWrapper ^ IshipIGCWrapper::GetGunner(Mount turretID)
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetGunner(turretID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	const ShipListIGC*  IshipIGCWrapper::GetChildShips()
	{
		return m_instance->GetChildShips();
	}
	void    IshipIGCWrapper::Complain(SoundID sid, const char* pszMsg)
	{
		return m_instance->Complain(sid, pszMsg);
	}
	short               IshipIGCWrapper::GetKills()
	{
		return m_instance->GetKills();
	}
	void                IshipIGCWrapper::SetKills(short n)
	{
		return m_instance->SetKills(n);
	}
	void                IshipIGCWrapper::AddKill()
	{
		return m_instance->AddKill();
	}
	short               IshipIGCWrapper::GetDeaths()
	{
		return m_instance->GetDeaths();
	}
	void                IshipIGCWrapper::SetDeaths(short n)
	{
		return m_instance->SetDeaths(n);
	}
	void                IshipIGCWrapper::AddDeath()
	{
		return m_instance->AddDeath();
	}
	short               IshipIGCWrapper::GetEjections()
	{
		return m_instance->GetEjections();
	}
	void                IshipIGCWrapper::SetEjections(short n)
	{
		return m_instance->SetEjections(n);
	}
	void                IshipIGCWrapper::AddEjection()
	{
		return m_instance->AddEjection();
	}
	float               IshipIGCWrapper::GetExperienceMultiplier()
	{
		return m_instance->GetExperienceMultiplier();
	}
	float               IshipIGCWrapper::GetExperience()
	{
		return m_instance->GetExperience();
	}
	void                IshipIGCWrapper::SetExperience(float experiance)
	{
		return m_instance->SetExperience(experiance);
	}
	void                IshipIGCWrapper::AddExperience()
	{
		return m_instance->AddExperience();
	}
	void                IshipIGCWrapper::AddChildShip(IshipIGCWrapper^ pship)
	{
		return m_instance->AddChildShip(pship->m_instance);
	}
	void                IshipIGCWrapper::DeleteChildShip(IshipIGCWrapper^ pship)
	{
		return m_instance->DeleteChildShip(pship->m_instance);
	}
	IshipIGCWrapper ^ IshipIGCWrapper::GetSourceShip()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetSourceShip();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	IpartIGCWrapper ^ IshipIGCWrapper::CreateAndAddPart(const PartData* ppd)
	{
		::IpartIGC * unmanagedValue = (::IpartIGC *) m_instance->CreateAndAddPart(ppd);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IpartIGCWrapper(unmanagedValue);
	}
	IpartIGCWrapper ^ IshipIGCWrapper::CreateAndAddPart(IpartTypeIGCWrapper^ ppt, Mount mount, short amount)
	{
		::IpartIGC * unmanagedValue = (::IpartIGC *) m_instance->CreateAndAddPart(ppt->m_instance, mount, amount);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IpartIGCWrapper(unmanagedValue);
	}
	WingID              IshipIGCWrapper::GetWingID()
	{
		return m_instance->GetWingID();
	}
	void                IshipIGCWrapper::SetWingID(WingID wid)
	{
		return m_instance->SetWingID(wid);
	}
	PilotType           IshipIGCWrapper::GetPilotType()
	{
		return m_instance->GetPilotType();
	}
	AbilityBitMask      IshipIGCWrapper::GetOrdersABM()
	{
		return m_instance->GetOrdersABM();
	}
	bool                IshipIGCWrapper::GetAutopilot()
	{
		return m_instance->GetAutopilot();
	}
	void                IshipIGCWrapper::SetAutopilot(bool bAutopilot)
	{
		return m_instance->SetAutopilot(bAutopilot);
	}
	bool                IshipIGCWrapper::LegalCommand(CommandID cid)
	{
		return m_instance->LegalCommand(cid);
	}
	/*void                IshipIGCWrapper::SetRunawayCheckCooldown(float dtRunAway)
	{
		return m_instance->SetRunawayCheckCooldown(dtRunAway);
	}*/
	IshipIGCWrapper ^ IshipIGCWrapper::GetAutoDonate()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetAutoDonate();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	void                IshipIGCWrapper::SetAutoDonate(IshipIGCWrapper^ pship)
	{
		return m_instance->SetAutoDonate(pship->m_instance);
	}
	ImodelIGCWrapper ^ IshipIGCWrapper::GetRipcordModel()
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->GetRipcordModel();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	void                IshipIGCWrapper::SetRipcordModel(ImodelIGCWrapper^ pmodel)
	{
		return m_instance->SetRipcordModel(pmodel->m_instance);
	}
	ImodelIGCWrapper ^ IshipIGCWrapper::FindRipcordModel(IclusterIGCWrapper^ pcluster)
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->FindRipcordModel(pcluster->m_instance);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	float               IshipIGCWrapper::GetRipcordDebt()
	{
		return m_instance->GetRipcordDebt();
	}
	void                IshipIGCWrapper::AdjustRipcordDebt(float delta)
	{
		return m_instance->AdjustRipcordDebt(delta);
	}
	void			 IshipIGCWrapper::SetStayDocked(bool stayDock)
	{
		return m_instance->SetStayDocked(stayDock);
	}
	bool			 IshipIGCWrapper::GetStayDocked()
	{
		return m_instance->GetStayDocked();
	}
	void                IshipIGCWrapper::SetGettingAmmo(bool gettingAmmo)
	{
		return m_instance->SetGettingAmmo(gettingAmmo);
	}
	void                IshipIGCWrapper::SetWingmanBehaviour(WingmanBehaviourBitMask wingmanBehaviour)
	{
		return m_instance->SetWingmanBehaviour(wingmanBehaviour);
	}
	WingmanBehaviourBitMask IshipIGCWrapper::GetWingmanBehaviour()
	{
		return m_instance->GetWingmanBehaviour();
	}
	void			 IshipIGCWrapper::AddRepair(float repair)
	{
		return m_instance->AddRepair(repair);
	}
	float			 IshipIGCWrapper::GetRepair()
	{
		return m_instance->GetRepair();
	}
	void			 IshipIGCWrapper::SetAchievementMask(AchievementMask am)
	{
		return m_instance->SetAchievementMask(am);
	}
	void			 IshipIGCWrapper::ClearAchievementMask()
	{
		return m_instance->ClearAchievementMask();
	}
	AchievementMask	 IshipIGCWrapper::GetAchievementMask()
	{
		return m_instance->GetAchievementMask();
	}
	void			 IshipIGCWrapper::MarkPreviouslySpotted()
	{
		return m_instance->MarkPreviouslySpotted();
	}
	/*bool			 IshipIGCWrapper::RecentlySpotted()
	{
		return m_instance->RecentlySpotted();
	}*/
	DamageTrackWrapper ^ IshipIGCWrapper::GetDamageTrack()
	{
		::DamageTrack * unmanagedValue = (::DamageTrack *) m_instance->GetDamageTrack();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew DamageTrackWrapper(unmanagedValue);
	}
	void                IshipIGCWrapper::CreateDamageTrack()
	{
		return m_instance->CreateDamageTrack();
	}
	void                IshipIGCWrapper::DeleteDamageTrack()
	{
		return m_instance->DeleteDamageTrack();
	}
	void                IshipIGCWrapper::ResetDamageTrack()
	{
		return m_instance->ResetDamageTrack();
	}
	void                IshipIGCWrapper::ApplyMineDamage()
	{
		return m_instance->ApplyMineDamage();
	}
	CommandID           IshipIGCWrapper::GetDefaultOrder(ImodelIGCWrapper^ pmodel)
	{
		return m_instance->GetDefaultOrder(pmodel->m_instance);
	}
	bool                IshipIGCWrapper::OkToLaunch(Time now)
	{
		return m_instance->OkToLaunch(now);
	}
	bool                IshipIGCWrapper::PickDefaultOrder(IclusterIGCWrapper^ pcluster, const VectorWrapper ^ position, bool bDocked)
	{
		return m_instance->PickDefaultOrder(pcluster->m_instance, *position->m_instance, bDocked);
	}
	bool                IshipIGCWrapper::IsGhost()
	{
		return m_instance->IsGhost();
	}
	float               IshipIGCWrapper::GetEndurance()
	{
		return m_instance->GetEndurance();
	}
	bool                IshipIGCWrapper::InGarage(IshipIGCWrapper^ pship, float tCollision)
	{
		return m_instance->InGarage(pship->m_instance, tCollision);
	}
	bool                IshipIGCWrapper::IsUsingAreaOfEffectWeapon()
	{
		return m_instance->IsUsingAreaOfEffectWeapon();
	}
	short               IshipIGCWrapper::GetLaunchSlot()
	{
		return m_instance->GetLaunchSlot();
	}
	WarningMask         IshipIGCWrapper::GetWarningMask()
	{
		return m_instance->GetWarningMask();
	}
	void                IshipIGCWrapper::SetWarningMaskBit(WarningMask wm)
	{
		return m_instance->SetWarningMaskBit(wm);
	}
	void                IshipIGCWrapper::ClearWarningMaskBit(WarningMask wm)
	{
		return m_instance->ClearWarningMaskBit(wm);
	}
	void                IshipIGCWrapper::SetBaseData(IbaseIGCWrapper^ pbase)
	{
		return m_instance->SetBaseData(pbase->m_instance);
	}
	IbaseIGCWrapper ^ IshipIGCWrapper::GetBaseData()
	{
		::IbaseIGC * unmanagedValue = (::IbaseIGC *) m_instance->GetBaseData();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbaseIGCWrapper(unmanagedValue);
	}
	float               IshipIGCWrapper::GetOre()
	{
		return m_instance->GetOre();
	}
	void                IshipIGCWrapper::SetSkills(float fShoot, float fTurn, float fGoto)
	{
		return m_instance->SetSkills(fShoot, fTurn, fGoto);
	}
	void			 IshipIGCWrapper::SetWantBoost(bool bOn)
	{
		return m_instance->SetWantBoost(bOn);
	}
	bool 			 IshipIGCWrapper::GetWantBoost()
	{
		return m_instance->GetWantBoost();
	}

	IstationTypeIGCWrapper ^ IshipIGCWrapper::GetStationType() 
	{ 
		return gcnew IstationTypeIGCWrapper((IstationTypeIGC*)(IbaseIGC*)m_instance->GetBaseData()); 
	}

	//void				IshipIGCWrapper::SetCluster(IclusterIGCWrapper ^ cluster) { m_instance->SetCluster(cluster->m_instance); }

	PlayerInfoWrapper ^ IshipIGCWrapper::GetPlayerInfo()
	{
		return gcnew PlayerInfoWrapper((PlayerInfo *) m_instance->GetPrivateData());
	}


	IbuoyIGCWrapper::IbuoyIGCWrapper(::IbuoyIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	void                 IbuoyIGCWrapper::AddConsumer()
	{
		return m_instance->AddConsumer();
	}
	void                 IbuoyIGCWrapper::ReleaseConsumer()
	{
		return m_instance->ReleaseConsumer();
	}
	BuoyType             IbuoyIGCWrapper::GetBuoyType()
	{
		return m_instance->GetBuoyType();
	}
	IchaffIGCWrapper::IchaffIGCWrapper(::IchaffIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IprojectileIGCWrapper::IprojectileIGCWrapper(::IprojectileIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IprojectileTypeIGCWrapper ^ IprojectileIGCWrapper::GetProjectileType()
	{
		::IprojectileTypeIGC * unmanagedValue = (::IprojectileTypeIGC *) m_instance->GetProjectileType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprojectileTypeIGCWrapper(unmanagedValue);
	}
	ImodelIGCWrapper ^ IprojectileIGCWrapper::GetLauncher()
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->GetLauncher();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	void                 IprojectileIGCWrapper::SetLauncher(ImodelIGCWrapper^ newVal)
	{
		return m_instance->SetLauncher(newVal->m_instance);
	}
	void                 IprojectileIGCWrapper::SetGunner(IshipIGCWrapper^ pship)
	{
		return m_instance->SetGunner(pship->m_instance);
	}
	ImissileIGCWrapper::ImissileIGCWrapper(::ImissileIGC * instance) : IdamageIGCWrapper(instance)
	{
		m_instance = instance;
	}
	ImissileTypeIGCWrapper ^ ImissileIGCWrapper::GetMissileType()
	{
		::ImissileTypeIGC * unmanagedValue = (::ImissileTypeIGC *) m_instance->GetMissileType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImissileTypeIGCWrapper(unmanagedValue);
	}
	IshipIGCWrapper ^ ImissileIGCWrapper::GetLauncher()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetLauncher();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	ImodelIGCWrapper ^ ImissileIGCWrapper::GetTarget()
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->GetTarget();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	void                ImissileIGCWrapper::SetTarget(ImodelIGCWrapper^ newVal)
	{
		return m_instance->SetTarget(newVal->m_instance);
	}
	float               ImissileIGCWrapper::GetLock()
	{
		return m_instance->GetLock();
	}
	void                ImissileIGCWrapper::Explode(const VectorWrapper ^ position)
	{
		return m_instance->Explode(*position->m_instance);
	}
	void                ImissileIGCWrapper::Disarm()
	{
		return m_instance->Disarm();
	}
	ImineIGCWrapper::ImineIGCWrapper(::ImineIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	ImineTypeIGCWrapper ^ ImineIGCWrapper::GetMineType()
	{
		::ImineTypeIGC * unmanagedValue = (::ImineTypeIGC *) m_instance->GetMineType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImineTypeIGCWrapper(unmanagedValue);
	}
	IshipIGCWrapper ^ ImineIGCWrapper::GetLauncher()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetLauncher();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	float                ImineIGCWrapper::GetStrength()
	{
		return m_instance->GetStrength();
	}
	void                 ImineIGCWrapper::ReduceStrength(float amount)
	{
		return m_instance->ReduceStrength(amount);
	}
	float                ImineIGCWrapper::GetTimeFraction()
	{
		return m_instance->GetTimeFraction();
	}
	IbuildingEffectIGCWrapper::IbuildingEffectIGCWrapper(::IbuildingEffectIGC * instance) : IdamageIGCWrapper(instance)
	{
		m_instance = instance;
	}
	void                IbuildingEffectIGCWrapper::BuilderTerminated()
	{
		return m_instance->BuilderTerminated();
	}
	IasteroidIGCWrapper ^ IbuildingEffectIGCWrapper::GetAsteroid()
	{
		::IasteroidIGC * unmanagedValue = (::IasteroidIGC *) m_instance->GetAsteroid();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IasteroidIGCWrapper(unmanagedValue);
	}
	void                IbuildingEffectIGCWrapper::MakeUnhitable()
	{
		return m_instance->MakeUnhitable();
	}
	void                IbuildingEffectIGCWrapper::AsteroidUpdate(Time now)
	{
		return m_instance->AsteroidUpdate(now);
	}
	IprobeIGCWrapper::IprobeIGCWrapper(::IprobeIGC * instance) : IscannerIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IprobeTypeIGCWrapper ^ IprobeIGCWrapper::GetProbeType()
	{
		::IprobeTypeIGC * unmanagedValue = (::IprobeTypeIGC *) m_instance->GetProbeType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprobeTypeIGCWrapper(unmanagedValue);
	}
	IprojectileTypeIGCWrapper ^ IprobeIGCWrapper::GetProjectileType()
	{
		::IprojectileTypeIGC * unmanagedValue = (::IprojectileTypeIGC *) m_instance->GetProjectileType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprojectileTypeIGCWrapper(unmanagedValue);
	}
	float                IprobeIGCWrapper::GetProjectileLifespan()
	{
		return m_instance->GetProjectileLifespan();
	}
	float                IprobeIGCWrapper::GetLifespan()
	{
		return m_instance->GetLifespan();
	}
	float                IprobeIGCWrapper::GetDtBurst()
	{
		return m_instance->GetDtBurst();
	}
	float                IprobeIGCWrapper::GetAccuracy()
	{
		return m_instance->GetAccuracy();
	}
	bool                 IprobeIGCWrapper::GetCanRipcord(float ripcordspeed)
	{
		return m_instance->GetCanRipcord();
	}
	float                IprobeIGCWrapper::GetRipcordDelay()
	{
		return m_instance->GetRipcordDelay();
	}
	SoundID              IprobeIGCWrapper::GetAmbientSound()
	{
		return m_instance->GetAmbientSound();
	}
	float                IprobeIGCWrapper::GetTimeFraction()
	{
		return m_instance->GetTimeFraction();
	}
	void			 IprobeIGCWrapper::SetExpiration(Time time)
	{
		return m_instance->SetExpiration(time);
	}
	IshipIGCWrapper ^ IprobeIGCWrapper::GetProbeLauncherShip()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetProbeLauncherShip();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	IstationIGCWrapper::IstationIGCWrapper(::IstationIGC * instance) : IscannerIGCWrapper(instance)
	{
		m_instance = instance;
	}
	void                    IstationIGCWrapper::AddShip(IshipIGCWrapper^ s)
	{
		return m_instance->AddShip(s->m_instance);
	}
	void                    IstationIGCWrapper::DeleteShip(IshipIGCWrapper^ s)
	{
		return m_instance->DeleteShip(s->m_instance);
	}
	IshipIGCWrapper ^ IstationIGCWrapper::GetShip(ShipID shipID)
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetShip(shipID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	GenericList(IshipIGCWrapper) IstationIGCWrapper::GetShips()
	{
		ConvertSList(IshipIGC, ShipLinkIGC, m_instance->GetShips())
	}
	void                    IstationIGCWrapper::RepairAndRefuel(IshipIGCWrapper^ pship)
	{
		return m_instance->RepairAndRefuel(pship->m_instance);
	}
	void                    IstationIGCWrapper::Launch(IshipIGCWrapper^ pship)
	{
		return m_instance->Launch(pship->m_instance);
	}
	bool                    IstationIGCWrapper::InGarage(IshipIGCWrapper^ pship, const VectorWrapper ^ position)
	{
		return m_instance->InGarage(pship->m_instance, *position->m_instance);
	}
	ObjectID			 IstationIGCWrapper::GetRoidID()
	{
		return m_instance->GetRoidID();
	}
	void IstationIGCWrapper::SetRoidID(ObjectID id)
	{
		return m_instance->SetRoidID(id);
	}
	Vector				 IstationIGCWrapper::GetRoidPos()
	{
		return m_instance->GetRoidPos();
	}
	void IstationIGCWrapper::SetRoidPos(Vector pos)
	{
		return m_instance->SetRoidPos(pos);
	}
	float				 IstationIGCWrapper::GetRoidSig()
	{
		return m_instance->GetRoidSig();
	}
	void IstationIGCWrapper::SetRoidSig(float Sig)
	{
		return m_instance->SetRoidSig(Sig);
	}
	float				 IstationIGCWrapper::GetRoidRadius()
	{
		return m_instance->GetRoidRadius();
	}
	void IstationIGCWrapper::SetRoidRadius(float Radius)
	{
		return m_instance->SetRoidRadius(Radius);
	}
	AsteroidAbilityBitMask IstationIGCWrapper::GetRoidCaps()
	{
		return m_instance->GetRoidCaps();
	}
	void IstationIGCWrapper::SetRoidCaps(AsteroidAbilityBitMask aabm)
	{
		return m_instance->SetRoidCaps(aabm);
	}
	void IstationIGCWrapper::SetRoidSide(SideID sid, bool bset)
	{
		return m_instance->SetRoidSide(sid, bset);
	}
	bool IstationIGCWrapper::GetRoidSide(SideID sid)
	{
		return m_instance->GetRoidSide(sid);
	}
	float                   IstationIGCWrapper::GetShieldFraction()
	{
		return m_instance->GetShieldFraction();
	}
	void                    IstationIGCWrapper::SetShieldFraction(float sf)
	{
		return m_instance->SetShieldFraction(sf);
	}
	IstationTypeIGCWrapper ^ IstationIGCWrapper::GetStationType()
	{
		::IstationTypeIGC * unmanagedValue = (::IstationTypeIGC *) m_instance->GetStationType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationTypeIGCWrapper(unmanagedValue);
	}
	IstationTypeIGCWrapper ^ IstationIGCWrapper::GetBaseStationType()
	{
		::IstationTypeIGC * unmanagedValue = (::IstationTypeIGC *) m_instance->GetBaseStationType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationTypeIGCWrapper(unmanagedValue);
	}
	void                    IstationIGCWrapper::SetBaseStationType(IstationTypeIGCWrapper^ pst)
	{
		return m_instance->SetBaseStationType(pst->m_instance);
	}
	bool                    IstationIGCWrapper::CanBuy(IbuyableIGCWrapper^ b)
	{
		return m_instance->CanBuy(b->m_instance);
	}
	bool                    IstationIGCWrapper::IsObsolete(IbuyableIGCWrapper^ b)
	{
		return m_instance->IsObsolete(b->m_instance);
	}
	IbuyableIGCWrapper ^ IstationIGCWrapper::GetSuccessor(IbuyableIGCWrapper^ b)
	{
		::IbuyableIGC * unmanagedValue = (::IbuyableIGC *) m_instance->GetSuccessor(b->m_instance);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbuyableIGCWrapper(unmanagedValue);
	}
	IpartTypeIGCWrapper ^ IstationIGCWrapper::GetSimilarPart(IpartTypeIGCWrapper^ ppt)
	{
		::IpartTypeIGC * unmanagedValue = (::IpartTypeIGC *) m_instance->GetSimilarPart(ppt->m_instance);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IpartTypeIGCWrapper(unmanagedValue);
	}
	void                    IstationIGCWrapper::SetLastDamageReport(Time timeLastDamage)
	{
		return m_instance->SetLastDamageReport(timeLastDamage);
	}
	Time                    IstationIGCWrapper::GetLastDamageReport()
	{
		return m_instance->GetLastDamageReport();
	}
	SoundID                 IstationIGCWrapper::GetInteriorSound()
	{
		return m_instance->GetInteriorSound();
	}
	SoundID                 IstationIGCWrapper::GetExteriorSound()
	{
		return m_instance->GetExteriorSound();
	}
	IbuyableIGCWrapper::IbuyableIGCWrapper(::IbuyableIGC * instance) : ItypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	String ^ IbuyableIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	String ^ IbuyableIGCWrapper::GetIconName()
	{
		return gcnew String(m_instance->GetIconName());
	}
	String ^ IbuyableIGCWrapper::GetDescription()
	{
		return gcnew String(m_instance->GetDescription());
	}
	String ^ IbuyableIGCWrapper::GetModelName()
	{
		return gcnew String(m_instance->GetModelName());
	}
	Money                   IbuyableIGCWrapper::GetPrice()
	{
		return m_instance->GetPrice();
	}
	DWORD                   IbuyableIGCWrapper::GetTimeToBuild()
	{
		return m_instance->GetTimeToBuild();
	}
	BuyableGroupID          IbuyableIGCWrapper::GetGroupID()
	{
		return m_instance->GetGroupID();
	}
	IdevelopmentIGCWrapper::IdevelopmentIGCWrapper(::IdevelopmentIGC * instance) : IbuyableIGCWrapper(instance)
	{
		m_instance = instance;
	}
	bool                        IdevelopmentIGCWrapper::GetTechOnly()
	{
		return m_instance->GetTechOnly();
	}
	bool                        IdevelopmentIGCWrapper::IsObsolete(const TechTreeBitMask& ttbm)
	{
		return m_instance->IsObsolete(ttbm);
	}
	SoundID                     IdevelopmentIGCWrapper::GetCompletionSound()
	{
		return m_instance->GetCompletionSound();
	}
	IdroneTypeIGCWrapper::IdroneTypeIGCWrapper(::IdroneTypeIGC * instance) : IbuyableIGCWrapper(instance)
	{
		m_instance = instance;
	}
	HullID                  IdroneTypeIGCWrapper::GetHullTypeID()
	{
		return m_instance->GetHullTypeID();
	}
	PilotType               IdroneTypeIGCWrapper::GetPilotType()
	{
		return m_instance->GetPilotType();
	}
	float                   IdroneTypeIGCWrapper::GetShootSkill()
	{
		return m_instance->GetShootSkill();
	}
	float                   IdroneTypeIGCWrapper::GetMoveSkill()
	{
		return m_instance->GetMoveSkill();
	}
	float                   IdroneTypeIGCWrapper::GetBravery()
	{
		return m_instance->GetBravery();
	}
	IexpendableTypeIGCWrapper ^ IdroneTypeIGCWrapper::GetLaidExpendable()
	{
		::IexpendableTypeIGC * unmanagedValue = (::IexpendableTypeIGC *) m_instance->GetLaidExpendable();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IexpendableTypeIGCWrapper(unmanagedValue);
	}
	IstationTypeIGCWrapper::IstationTypeIGCWrapper(::IstationTypeIGC * instance) : IbuyableIGCWrapper(instance)
	{
		m_instance = instance;
	}
	HitPoints               IstationTypeIGCWrapper::GetMaxArmorHitPoints()
	{
		return m_instance->GetMaxArmorHitPoints();
	}
	DefenseTypeID           IstationTypeIGCWrapper::GetArmorDefenseType()
	{
		return m_instance->GetArmorDefenseType();
	}
	HitPoints               IstationTypeIGCWrapper::GetMaxShieldHitPoints()
	{
		return m_instance->GetMaxShieldHitPoints();
	}
	DefenseTypeID           IstationTypeIGCWrapper::GetShieldDefenseType()
	{
		return m_instance->GetShieldDefenseType();
	}
	float                   IstationTypeIGCWrapper::GetArmorRegeneration()
	{
		return m_instance->GetArmorRegeneration();
	}
	float                   IstationTypeIGCWrapper::GetShieldRegeneration()
	{
		return m_instance->GetShieldRegeneration();
	}
	float                   IstationTypeIGCWrapper::GetSignature()
	{
		return m_instance->GetSignature();
	}
	float                   IstationTypeIGCWrapper::GetRadius()
	{
		return m_instance->GetRadius();
	}
	float                   IstationTypeIGCWrapper::GetScannerRange()
	{
		return m_instance->GetScannerRange();
	}
	Money                   IstationTypeIGCWrapper::GetIncome()
	{
		return m_instance->GetIncome();
	}
	StationAbilityBitMask   IstationTypeIGCWrapper::GetCapabilities()
	{
		return m_instance->GetCapabilities();
	}
	bool                    IstationTypeIGCWrapper::HasCapability(StationAbilityBitMask habm)
	{
		return m_instance->HasCapability(habm);
	}
	String ^ IstationTypeIGCWrapper::GetTextureName()
	{
		return gcnew String(m_instance->GetTextureName());
	}
	String ^ IstationTypeIGCWrapper::GetBuilderName()
	{
		return gcnew String(m_instance->GetBuilderName());
	}
	IstationTypeIGCWrapper ^ IstationTypeIGCWrapper::GetSuccessorStationType(IsideIGCWrapper^ pside)
	{
		::IstationTypeIGC * unmanagedValue = (::IstationTypeIGC *) m_instance->GetSuccessorStationType(pside->m_instance);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationTypeIGCWrapper(unmanagedValue);
	}
	IstationTypeIGCWrapper ^ IstationTypeIGCWrapper::GetDirectSuccessorStationType()
	{
		::IstationTypeIGC * unmanagedValue = (::IstationTypeIGC *) m_instance->GetDirectSuccessorStationType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationTypeIGCWrapper(unmanagedValue);
	}
	AsteroidAbilityBitMask  IstationTypeIGCWrapper::GetBuildAABM()
	{
		return m_instance->GetBuildAABM();
	}
	int                     IstationTypeIGCWrapper::GetLaunchSlots()
	{
		return m_instance->GetLaunchSlots();
	}
	int                     IstationTypeIGCWrapper::GetLandSlots()
	{
		return m_instance->GetLandSlots();
	}
	int                     IstationTypeIGCWrapper::GetCapLandSlots()
	{
		return m_instance->GetCapLandSlots();
	}
	int                     IstationTypeIGCWrapper::GetLandPlanes(int slotID)
	{
		return m_instance->GetLandPlanes(slotID);
	}
	SoundID                 IstationTypeIGCWrapper::GetInteriorSound()
	{
		return m_instance->GetInteriorSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetInteriorAlertSound()
	{
		return m_instance->GetInteriorAlertSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetExteriorSound()
	{
		return m_instance->GetExteriorSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetConstructorNeedRockSound()
	{
		return m_instance->GetConstructorNeedRockSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetConstructorUnderAttackSound()
	{
		return m_instance->GetConstructorUnderAttackSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetConstructorDestroyedSound()
	{
		return m_instance->GetConstructorDestroyedSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetCompletionSound()
	{
		return m_instance->GetCompletionSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetUnderAttackSound()
	{
		return m_instance->GetUnderAttackSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetCriticalSound()
	{
		return m_instance->GetCriticalSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetDestroyedSound()
	{
		return m_instance->GetDestroyedSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetCapturedSound()
	{
		return m_instance->GetCapturedSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetEnemyCapturedSound()
	{
		return m_instance->GetEnemyCapturedSound();
	}
	SoundID                 IstationTypeIGCWrapper::GetEnemyDestroyedSound()
	{
		return m_instance->GetEnemyDestroyedSound();
	}
	StationClassID          IstationTypeIGCWrapper::GetClassID()
	{
		return m_instance->GetClassID();
	}
	IdroneTypeIGCWrapper ^ IstationTypeIGCWrapper::GetConstructionDroneType()
	{
		::IdroneTypeIGC * unmanagedValue = (::IdroneTypeIGC *) m_instance->GetConstructionDroneType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IdroneTypeIGCWrapper(unmanagedValue);
	}
	IbucketIGCWrapper::IbucketIGCWrapper(::IbucketIGC * instance) : IbuyableIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IbuyableIGCWrapper ^ IbucketIGCWrapper::GetBuyable()
	{
		::IbuyableIGC * unmanagedValue = (::IbuyableIGC *) m_instance->GetBuyable();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbuyableIGCWrapper(unmanagedValue);
	}
	IsideIGCWrapper ^ IbucketIGCWrapper::GetSide()
	{
		::IsideIGC * unmanagedValue = (::IsideIGC *) m_instance->GetSide();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IsideIGCWrapper(unmanagedValue);
	}
	void                    IbucketIGCWrapper::SetPrice(Money m)
	{
		return m_instance->SetPrice(m);
	}
	int                     IbucketIGCWrapper::GetPercentBought()
	{
		return m_instance->GetPercentBought();
	}
	int                     IbucketIGCWrapper::GetPercentComplete()
	{
		return m_instance->GetPercentComplete();
	}
	bool                    IbucketIGCWrapper::GetCompleteF()
	{
		return m_instance->GetCompleteF();
	}
	void                    IbucketIGCWrapper::ForceComplete(Time now)
	{
		return m_instance->ForceComplete(now);
	}
	DWORD                   IbucketIGCWrapper::GetTime()
	{
		return m_instance->GetTime();
	}
	Money                   IbucketIGCWrapper::GetMoney()
	{
		return m_instance->GetMoney();
	}
	void                    IbucketIGCWrapper::SetTimeAndMoney(DWORD dwTime, Money money)
	{
		return m_instance->SetTimeAndMoney(dwTime, money);
	}
	Money                   IbucketIGCWrapper::AddMoney(Money m)
	{
		return m_instance->AddMoney(m);
	}
	void                    IbucketIGCWrapper::SetEmpty()
	{
		return m_instance->SetEmpty();
	}
	ObjectType              IbucketIGCWrapper::GetBucketType()
	{
		return m_instance->GetBucketType();
	}
	IbucketIGCWrapper ^ IbucketIGCWrapper::GetPredecessor()
	{
		::IbucketIGC * unmanagedValue = (::IbucketIGC *) m_instance->GetPredecessor();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbucketIGCWrapper(unmanagedValue);
	}
	IprojectileTypeIGCWrapper::IprojectileTypeIGCWrapper(::IprojectileTypeIGC * instance) : ItypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float            IprojectileTypeIGCWrapper::GetPower()
	{
		return m_instance->GetPower();
	}
	float            IprojectileTypeIGCWrapper::GetBlastPower()
	{
		return m_instance->GetBlastPower();
	}
	float            IprojectileTypeIGCWrapper::GetBlastRadius()
	{
		return m_instance->GetBlastRadius();
	}
	float            IprojectileTypeIGCWrapper::GetSpeed()
	{
		return m_instance->GetSpeed();
	}
	bool             IprojectileTypeIGCWrapper::GetAbsoluteF()
	{
		return m_instance->GetAbsoluteF();
	}
	String ^ IprojectileTypeIGCWrapper::GetModelName()
	{
		return gcnew String(m_instance->GetModelName());
	}
	String ^ IprojectileTypeIGCWrapper::GetModelTexture()
	{
		return gcnew String(m_instance->GetModelTexture());
	}
	float            IprojectileTypeIGCWrapper::GetLifespan()
	{
		return m_instance->GetLifespan();
	}
	float            IprojectileTypeIGCWrapper::GetRadius()
	{
		return m_instance->GetRadius();
	}
	float            IprojectileTypeIGCWrapper::GetRotation()
	{
		return m_instance->GetRotation();
	}
	COLORVALUE       IprojectileTypeIGCWrapper::GetColor()
	{
		return m_instance->GetColor();
	}
	DamageTypeID     IprojectileTypeIGCWrapper::GetDamageType()
	{
		return m_instance->GetDamageType();
	}
	SoundID          IprojectileTypeIGCWrapper::GetAmbientSound()
	{
		return m_instance->GetAmbientSound();
	}
	IexpendableTypeIGCWrapper::IexpendableTypeIGCWrapper(::IexpendableTypeIGC * instance) : ItypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	const LauncherDef*          IexpendableTypeIGCWrapper::GetLauncherDef()
	{
		return m_instance->GetLauncherDef();
	}
	EquipmentType               IexpendableTypeIGCWrapper::GetEquipmentType()
	{
		return m_instance->GetEquipmentType();
	}
	float                       IexpendableTypeIGCWrapper::GetLoadTime()
	{
		return m_instance->GetLoadTime();
	}
	float                       IexpendableTypeIGCWrapper::GetMass()
	{
		return m_instance->GetMass();
	}
	float                       IexpendableTypeIGCWrapper::GetSignature()
	{
		return m_instance->GetSignature();
	}
	HitPoints                   IexpendableTypeIGCWrapper::GetHitPoints()
	{
		return m_instance->GetHitPoints();
	}
	DefenseTypeID               IexpendableTypeIGCWrapper::GetDefenseType()
	{
		return m_instance->GetDefenseType();
	}
	float                       IexpendableTypeIGCWrapper::GetLifespan()
	{
		return m_instance->GetLifespan();
	}
	String ^ IexpendableTypeIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	String ^ IexpendableTypeIGCWrapper::GetModelName()
	{
		return gcnew String(m_instance->GetModelName());
	}
	String ^ IexpendableTypeIGCWrapper::GetModelTexture()
	{
		return gcnew String(m_instance->GetModelTexture());
	}
	float                       IexpendableTypeIGCWrapper::GetRadius()
	{
		return m_instance->GetRadius();
	}
	ExpendableAbilityBitMask    IexpendableTypeIGCWrapper::GetCapabilities()
	{
		return m_instance->GetCapabilities();
	}
	bool                        IexpendableTypeIGCWrapper::HasCapability(ExpendableAbilityBitMask habm)
	{
		return m_instance->HasCapability(habm);
	}
	ImissileTypeIGCWrapper::ImissileTypeIGCWrapper(::ImissileTypeIGC * instance) : IexpendableTypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float           ImissileTypeIGCWrapper::GetAcceleration()
	{
		return m_instance->GetAcceleration();
	}
	float           ImissileTypeIGCWrapper::GetTurnRate()
	{
		return m_instance->GetTurnRate();
	}
	float           ImissileTypeIGCWrapper::GetInitialSpeed()
	{
		return m_instance->GetInitialSpeed();
	}
	float           ImissileTypeIGCWrapper::GetMaxLock()
	{
		return m_instance->GetMaxLock();
	}
	float           ImissileTypeIGCWrapper::GetChaffResistance()
	{
		return m_instance->GetChaffResistance();
	}
	float           ImissileTypeIGCWrapper::GetLockTime()
	{
		return m_instance->GetLockTime();
	}
	float           ImissileTypeIGCWrapper::GetReadyTime()
	{
		return m_instance->GetReadyTime();
	}
	float           ImissileTypeIGCWrapper::GetDispersion()
	{
		return m_instance->GetDispersion();
	}
	float           ImissileTypeIGCWrapper::GetLockAngle()
	{
		return m_instance->GetLockAngle();
	}
	float           ImissileTypeIGCWrapper::GetPower()
	{
		return m_instance->GetPower();
	}
	float           ImissileTypeIGCWrapper::GetBlastPower()
	{
		return m_instance->GetBlastPower();
	}
	float           ImissileTypeIGCWrapper::GetBlastRadius()
	{
		return m_instance->GetBlastRadius();
	}
	DamageTypeID    ImissileTypeIGCWrapper::GetDamageType()
	{
		return m_instance->GetDamageType();
	}
	SoundID         ImissileTypeIGCWrapper::GetLaunchSound()
	{
		return m_instance->GetLaunchSound();
	}
	SoundID         ImissileTypeIGCWrapper::GetAmbientSound()
	{
		return m_instance->GetAmbientSound();
	}
	ImineTypeIGCWrapper::ImineTypeIGCWrapper(::ImineTypeIGC * instance) : IexpendableTypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float           ImineTypeIGCWrapper::GetPower()
	{
		return m_instance->GetPower();
	}
	float           ImineTypeIGCWrapper::GetEndurance()
	{
		return m_instance->GetEndurance();
	}
	DamageTypeID    ImineTypeIGCWrapper::GetDamageType()
	{
		return m_instance->GetDamageType();
	}
	IchaffTypeIGCWrapper::IchaffTypeIGCWrapper(::IchaffTypeIGC * instance) : IexpendableTypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float               IchaffTypeIGCWrapper::GetChaffStrength()
	{
		return m_instance->GetChaffStrength();
	}
	IprobeTypeIGCWrapper::IprobeTypeIGCWrapper(::IprobeTypeIGC * instance) : IexpendableTypeIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float               IprobeTypeIGCWrapper::GetScannerRange()
	{
		return m_instance->GetScannerRange();
	}
	float               IprobeTypeIGCWrapper::GetDtBurst()
	{
		return m_instance->GetDtBurst();
	}
	float               IprobeTypeIGCWrapper::GetDispersion()
	{
		return m_instance->GetDispersion();
	}
	float               IprobeTypeIGCWrapper::GetAccuracy()
	{
		return m_instance->GetAccuracy();
	}
	short               IprobeTypeIGCWrapper::GetAmmo()
	{
		return m_instance->GetAmmo();
	}
	IprojectileTypeIGCWrapper ^ IprobeTypeIGCWrapper::GetProjectileType()
	{
		::IprojectileTypeIGC * unmanagedValue = (::IprojectileTypeIGC *) m_instance->GetProjectileType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprojectileTypeIGCWrapper(unmanagedValue);
	}
	float               IprobeTypeIGCWrapper::GetRipcordDelay()
	{
		return m_instance->GetRipcordDelay();
	}
	SoundID             IprobeTypeIGCWrapper::GetAmbientSound()
	{
		return m_instance->GetAmbientSound();
	}
	IhullTypeIGCWrapper::IhullTypeIGCWrapper(::IhullTypeIGC * instance) : IbuyableIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float                IhullTypeIGCWrapper::GetLength()
	{
		return m_instance->GetLength();
	}
	float                IhullTypeIGCWrapper::GetMaxSpeed()
	{
		return m_instance->GetMaxSpeed();
	}
	float                IhullTypeIGCWrapper::GetMaxTurnRate(Axis axis)
	{
		return m_instance->GetMaxTurnRate(axis);
	}
	float                IhullTypeIGCWrapper::GetTurnTorque(Axis axis)
	{
		return m_instance->GetTurnTorque(axis);
	}
	float                IhullTypeIGCWrapper::GetThrust()
	{
		return m_instance->GetThrust();
	}
	float                IhullTypeIGCWrapper::GetSideMultiplier()
	{
		return m_instance->GetSideMultiplier();
	}
	float                IhullTypeIGCWrapper::GetBackMultiplier()
	{
		return m_instance->GetBackMultiplier();
	}
	float                IhullTypeIGCWrapper::GetScannerRange()
	{
		return m_instance->GetScannerRange();
	}
	float                IhullTypeIGCWrapper::GetMaxEnergy()
	{
		return m_instance->GetMaxEnergy();
	}
	float                IhullTypeIGCWrapper::GetRechargeRate()
	{
		return m_instance->GetRechargeRate();
	}
	HitPoints            IhullTypeIGCWrapper::GetHitPoints()
	{
		return m_instance->GetHitPoints();
	}
	DefenseTypeID        IhullTypeIGCWrapper::GetDefenseType()
	{
		return m_instance->GetDefenseType();
	}
	PartMask             IhullTypeIGCWrapper::GetPartMask(EquipmentType et, Mount mountID)
	{
		return m_instance->GetPartMask(et, mountID);
	}
	short                IhullTypeIGCWrapper::GetCapacity(EquipmentType et)
	{
		return m_instance->GetCapacity(et);
	}
	Mount                IhullTypeIGCWrapper::GetMaxWeapons()
	{
		return m_instance->GetMaxWeapons();
	}
	Mount                IhullTypeIGCWrapper::GetMaxFixedWeapons()
	{
		return m_instance->GetMaxFixedWeapons();
	}
	bool                 IhullTypeIGCWrapper::CanMount(IpartTypeIGCWrapper^ ppt, Mount mountID)
	{
		return m_instance->CanMount(ppt->m_instance, mountID);
	}
	String ^ IhullTypeIGCWrapper::GetTextureName()
	{
		return gcnew String(m_instance->GetTextureName());
	}
	float                IhullTypeIGCWrapper::GetMass()
	{
		return m_instance->GetMass();
	}
	float                IhullTypeIGCWrapper::GetSignature()
	{
		return m_instance->GetSignature();
	}
	HullAbilityBitMask   IhullTypeIGCWrapper::GetCapabilities()
	{
		return m_instance->GetCapabilities();
	}
	bool                 IhullTypeIGCWrapper::HasCapability(HullAbilityBitMask habm)
	{
		return m_instance->HasCapability(habm);
	}
	float                IhullTypeIGCWrapper::GetScale()
	{
		return m_instance->GetScale();
	}
	short                IhullTypeIGCWrapper::GetMaxAmmo()
	{
		return m_instance->GetMaxAmmo();
	}
	float                IhullTypeIGCWrapper::GetMaxFuel()
	{
		return m_instance->GetMaxFuel();
	}
	float                IhullTypeIGCWrapper::GetECM()
	{
		return m_instance->GetECM();
	}
	float                IhullTypeIGCWrapper::GetRipcordSpeed()
	{
		return m_instance->GetRipcordSpeed();
	}
	float                IhullTypeIGCWrapper::GetRipcordCost()
	{
		return m_instance->GetRipcordCost();
	}
	IhullTypeIGCWrapper ^ IhullTypeIGCWrapper::GetSuccessorHullType()
	{
		::IhullTypeIGC * unmanagedValue = (::IhullTypeIGC *) m_instance->GetSuccessorHullType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IhullTypeIGCWrapper(unmanagedValue);
	}
	SoundID              IhullTypeIGCWrapper::GetInteriorSound()
	{
		return m_instance->GetInteriorSound();
	}
	SoundID              IhullTypeIGCWrapper::GetExteriorSound()
	{
		return m_instance->GetExteriorSound();
	}
	SoundID              IhullTypeIGCWrapper::GetMainThrusterInteriorSound()
	{
		return m_instance->GetMainThrusterInteriorSound();
	}
	SoundID              IhullTypeIGCWrapper::GetMainThrusterExteriorSound()
	{
		return m_instance->GetMainThrusterExteriorSound();
	}
	SoundID              IhullTypeIGCWrapper::GetManuveringThrusterInteriorSound()
	{
		return m_instance->GetManuveringThrusterInteriorSound();
	}
	SoundID              IhullTypeIGCWrapper::GetManuveringThrusterExteriorSound()
	{
		return m_instance->GetManuveringThrusterExteriorSound();
	}
	const PartTypeListIGC*      IhullTypeIGCWrapper::GetPreferredPartTypes()
	{
		return m_instance->GetPreferredPartTypes();
	}
	IObject*             IhullTypeIGCWrapper::GetIcon()
	{
		return m_instance->GetIcon();
	}
	int                     IhullTypeIGCWrapper::GetLaunchSlots()
	{
		return m_instance->GetLaunchSlots();
	}
	int                     IhullTypeIGCWrapper::GetLandSlots()
	{
		return m_instance->GetLandSlots();
	}
	int                     IhullTypeIGCWrapper::GetLandPlanes(int slotID)
	{
		return m_instance->GetLandPlanes(slotID);
	}
	IpartTypeIGCWrapper::IpartTypeIGCWrapper(::IpartTypeIGC * instance) : IbuyableIGCWrapper(instance)
	{
		m_instance = instance;
	}
	EquipmentType   IpartTypeIGCWrapper::GetEquipmentType()
	{
		return m_instance->GetEquipmentType();
	}
	PartMask        IpartTypeIGCWrapper::GetPartMask()
	{
		return m_instance->GetPartMask();
	}
	float           IpartTypeIGCWrapper::GetMass()
	{
		return m_instance->GetMass();
	}
	float           IpartTypeIGCWrapper::GetSignature()
	{
		return m_instance->GetSignature();
	}
	IpartTypeIGCWrapper ^ IpartTypeIGCWrapper::GetSuccessorPartType()
	{
		::IpartTypeIGC * unmanagedValue = (::IpartTypeIGC *) m_instance->GetSuccessorPartType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IpartTypeIGCWrapper(unmanagedValue);
	}
	String ^ IpartTypeIGCWrapper::GetInventoryLineMDLName()
	{
		return gcnew String(m_instance->GetInventoryLineMDLName());
	}/*
	 String ^ IpartTypeIGCWrapper::GetEquipmentTypeName(EquipmentType et)
	 {
	 return gcnew String(m_instance->GetEquipmentTypeName(et));
	 }*/
	short           IpartTypeIGCWrapper::GetAmount(IshipIGCWrapper^ pship)
	{
		return m_instance->GetAmount(pship->m_instance);
	}
	IlauncherTypeIGCWrapper::IlauncherTypeIGCWrapper(::IlauncherTypeIGC * instance) : IpartTypeIGCWrapper(instance)
	{
		m_instance = instance;
	}/*
	 static  bool                IlauncherTypeIGCWrapper::IsLauncherType(EquipmentType et)
	 {
	 return m_instance->IsLauncherType(et);
	 }*/
	IexpendableTypeIGCWrapper ^ IlauncherTypeIGCWrapper::GetExpendableType()
	{
		::IexpendableTypeIGC * unmanagedValue = (::IexpendableTypeIGC *) m_instance->GetExpendableType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IexpendableTypeIGCWrapper(unmanagedValue);
	}
	IpartIGCWrapper::IpartIGCWrapper(::IpartIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	EquipmentType    IpartIGCWrapper::GetEquipmentType()
	{
		return m_instance->GetEquipmentType();
	}
	IpartTypeIGCWrapper ^ IpartIGCWrapper::GetPartType()
	{
		::IpartTypeIGC * unmanagedValue = (::IpartTypeIGC *) m_instance->GetPartType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IpartTypeIGCWrapper(unmanagedValue);
	}
	IshipIGCWrapper ^ IpartIGCWrapper::GetShip()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetShip();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	void             IpartIGCWrapper::SetShip(IshipIGCWrapper^ newVal, Mount mount)
	{
		return m_instance->SetShip(newVal->m_instance, mount);
	}
	Mount            IpartIGCWrapper::GetMountID()
	{
		return m_instance->GetMountID();
	}
	void             IpartIGCWrapper::SetMountID(Mount newVal)
	{
		return m_instance->SetMountID(newVal);
	}
	bool             IpartIGCWrapper::fActive()
	{
		return m_instance->fActive();
	}
	void             IpartIGCWrapper::Activate()
	{
		return m_instance->Activate();
	}
	void             IpartIGCWrapper::Deactivate()
	{
		return m_instance->Deactivate();
	}
	float            IpartIGCWrapper::GetMass()
	{
		return m_instance->GetMass();
	}
	Money            IpartIGCWrapper::GetPrice()
	{
		return m_instance->GetPrice();
	}
	float            IpartIGCWrapper::GetMountedFraction()
	{
		return m_instance->GetMountedFraction();
	}
	void             IpartIGCWrapper::SetMountedFraction(float f)
	{
		return m_instance->SetMountedFraction(f);
	}
	void             IpartIGCWrapper::Arm()
	{
		return m_instance->Arm();
	}
	short            IpartIGCWrapper::GetAmount()
	{
		return m_instance->GetAmount();
	}
	void             IpartIGCWrapper::SetAmount(short a)
	{
		return m_instance->SetAmount(a);
	}
	IweaponIGCWrapper::IweaponIGCWrapper(::IweaponIGC * instance) : IpartIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IprojectileTypeIGCWrapper ^ IweaponIGCWrapper::GetProjectileType()
	{
		::IprojectileTypeIGC * unmanagedValue = (::IprojectileTypeIGC *) m_instance->GetProjectileType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprojectileTypeIGCWrapper(unmanagedValue);
	}
	short                IweaponIGCWrapper::GetAmmoPerShot()
	{
		return m_instance->GetAmmoPerShot();
	}
	float                IweaponIGCWrapper::GetLifespan()
	{
		return m_instance->GetLifespan();
	}
	float                IweaponIGCWrapper::GetDtBurst()
	{
		return m_instance->GetDtBurst();
	}
	IshipIGCWrapper ^ IweaponIGCWrapper::GetGunner()
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetGunner();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	void                 IweaponIGCWrapper::SetGunner(IshipIGCWrapper^ newVal)
	{
		return m_instance->SetGunner(newVal->m_instance);
	}
	bool                 IweaponIGCWrapper::fFiringShot()
	{
		return m_instance->fFiringShot();
	}
	bool                 IweaponIGCWrapper::fFiringBurst()
	{
		return m_instance->fFiringBurst();
	}
	SoundID              IweaponIGCWrapper::GetActivateSound()
	{
		return m_instance->GetActivateSound();
	}
	SoundID              IweaponIGCWrapper::GetSingleShotSound()
	{
		return m_instance->GetSingleShotSound();
	}
	SoundID              IweaponIGCWrapper::GetBurstSound()
	{
		return m_instance->GetBurstSound();
	}
	IshieldIGCWrapper::IshieldIGCWrapper(::IshieldIGC * instance) : IpartIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float           IshieldIGCWrapper::GetRegeneration()
	{
		return m_instance->GetRegeneration();
	}
	float           IshieldIGCWrapper::GetMaxStrength()
	{
		return m_instance->GetMaxStrength();
	}
	DefenseTypeID   IshieldIGCWrapper::GetDefenseType()
	{
		return m_instance->GetDefenseType();
	}
	float           IshieldIGCWrapper::GetFraction()
	{
		return m_instance->GetFraction();
	}
	void            IshieldIGCWrapper::SetFraction(float newVal)
	{
		return m_instance->SetFraction(newVal);
	}
	IcloakIGCWrapper::IcloakIGCWrapper(::IcloakIGC * instance) : IpartIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float    IcloakIGCWrapper::GetEnergyConsumption()
	{
		return m_instance->GetEnergyConsumption();
	}
	float    IcloakIGCWrapper::GetMaxCloaking()
	{
		return m_instance->GetMaxCloaking();
	}
	float    IcloakIGCWrapper::GetOnRate()
	{
		return m_instance->GetOnRate();
	}
	float    IcloakIGCWrapper::GetOffRate()
	{
		return m_instance->GetOffRate();
	}
	SoundID  IcloakIGCWrapper::GetEngageSound()
	{
		return m_instance->GetEngageSound();
	}
	SoundID  IcloakIGCWrapper::GetDisengageSound()
	{
		return m_instance->GetDisengageSound();
	}
	IafterburnerIGCWrapper::IafterburnerIGCWrapper(::IafterburnerIGC * instance) : IpartIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float    IafterburnerIGCWrapper::GetFuelConsumption()
	{
		return m_instance->GetFuelConsumption();
	}
	float    IafterburnerIGCWrapper::GetMaxThrustWithGA()
	{
		return m_instance->GetMaxThrustWithGA();
	}
	float    IafterburnerIGCWrapper::GetMaxThrust()
	{
		return m_instance->GetMaxThrust();
	}
	float    IafterburnerIGCWrapper::GetOnRate()
	{
		return m_instance->GetOnRate();
	}
	float    IafterburnerIGCWrapper::GetOffRate()
	{
		return m_instance->GetOffRate();
	}
	void     IafterburnerIGCWrapper::IncrementalUpdate(Time lastUpdate, Time now, bool bUseFuel)
	{
		return m_instance->IncrementalUpdate(lastUpdate, now, bUseFuel);
	}
	float    IafterburnerIGCWrapper::GetPower()
	{
		return m_instance->GetPower();
	}
	void     IafterburnerIGCWrapper::SetPower(float p)
	{
		return m_instance->SetPower(p);
	}
	SoundID  IafterburnerIGCWrapper::GetInteriorSound()
	{
		return m_instance->GetInteriorSound();
	}
	SoundID  IafterburnerIGCWrapper::GetExteriorSound()
	{
		return m_instance->GetExteriorSound();
	}
	IpackIGCWrapper::IpackIGCWrapper(::IpackIGC * instance) : IpartIGCWrapper(instance)
	{
		m_instance = instance;
	}
	PackType IpackIGCWrapper::GetPackType()
	{
		return m_instance->GetPackType();
	}
	IlauncherIGCWrapper::IlauncherIGCWrapper(::IlauncherIGC * instance) : IpartIGCWrapper(instance)
	{
		m_instance = instance;
	}/*
	 static bool                 IlauncherIGCWrapper::IsLauncher(ObjectType type)
	 {
	 return m_instance->IsLauncher(type);
	 }*/
	void                IlauncherIGCWrapper::SetTimeFired(Time now)
	{
		return m_instance->SetTimeFired(now);
	}
	Time                IlauncherIGCWrapper::GetTimeLoaded()
	{
		return m_instance->GetTimeLoaded();
	}
	void                IlauncherIGCWrapper::SetTimeLoaded(Time tl)
	{
		return m_instance->SetTimeLoaded(tl);
	}
	void                IlauncherIGCWrapper::ResetTimeLoaded()
	{
		return m_instance->ResetTimeLoaded();
	}
	float               IlauncherIGCWrapper::GetArmedFraction()
	{
		return m_instance->GetArmedFraction();
	}
	ImagazineIGCWrapper::ImagazineIGCWrapper(::ImagazineIGC * instance) : IlauncherIGCWrapper(instance)
	{
		m_instance = instance;
	}
	ImissileTypeIGCWrapper ^ ImagazineIGCWrapper::GetMissileType()
	{
		::ImissileTypeIGC * unmanagedValue = (::ImissileTypeIGC *) m_instance->GetMissileType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImissileTypeIGCWrapper(unmanagedValue);
	}
	float               ImagazineIGCWrapper::GetLock()
	{
		return m_instance->GetLock();
	}
	void                ImagazineIGCWrapper::SetLock(float l)
	{
		return m_instance->SetLock(l);
	}
	IdispenserIGCWrapper::IdispenserIGCWrapper(::IdispenserIGC * instance) : IlauncherIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IexpendableTypeIGCWrapper ^ IdispenserIGCWrapper::GetExpendableType()
	{
		::IexpendableTypeIGC * unmanagedValue = (::IexpendableTypeIGC *) m_instance->GetExpendableType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IexpendableTypeIGCWrapper(unmanagedValue);
	}
	IclusterIGCWrapper::IclusterIGCWrapper(::IclusterIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	String ^ IclusterIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	Time                    IclusterIGCWrapper::GetLastUpdate()
	{
		return m_instance->GetLastUpdate();
	}
	void                    IclusterIGCWrapper::SetLastUpdate(Time now)
	{
		return m_instance->SetLastUpdate(now);
	}
	void                    IclusterIGCWrapper::AddModel(ImodelIGCWrapper^ modelNew)
	{
		return m_instance->AddModel(modelNew->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteModel(ImodelIGCWrapper^ modelOld)
	{
		return m_instance->DeleteModel(modelOld->m_instance);
	}
	ImodelIGCWrapper ^ IclusterIGCWrapper::GetModel(const char* name)
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->GetModel(name);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	const ModelListIGC*     IclusterIGCWrapper::GetModels()
	{
		return m_instance->GetModels();
	}
	const ModelListIGC*     IclusterIGCWrapper::GetPickableModels()
	{
		return m_instance->GetPickableModels();
	}
	void                    IclusterIGCWrapper::AddWarp(IwarpIGCWrapper^ warpNew)
	{
		return m_instance->AddWarp(warpNew->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteWarp(IwarpIGCWrapper^ warpOld)
	{
		return m_instance->DeleteWarp(warpOld->m_instance);
	}
	System::Collections::Generic::List<IwarpIGCWrapper^> ^      IclusterIGCWrapper::GetWarps()
	{
		System::Collections::Generic::List<IwarpIGCWrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<IwarpIGCWrapper ^>();

		for (WarpLinkIGC* pbl = m_instance->GetWarps()->last(); (pbl != NULL); pbl = pbl->txen())
		{
			IwarpIGC*     item = pbl->data();
			returnValue->Add(gcnew IwarpIGCWrapper(item));
		}
		return returnValue;
	}
	void                    IclusterIGCWrapper::AddMine(ImineIGCWrapper^ ms)
	{
		return m_instance->AddMine(ms->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteMine(ImineIGCWrapper^ m)
	{
		return m_instance->DeleteMine(m->m_instance);
	}
	ImineIGCWrapper ^ IclusterIGCWrapper::GetMine(MineID mineID)
	{
		::ImineIGC * unmanagedValue = (::ImineIGC *) m_instance->GetMine(mineID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImineIGCWrapper(unmanagedValue);
	}
	const MineListIGC*      IclusterIGCWrapper::GetMines()
	{
		return m_instance->GetMines();
	}
	void                    IclusterIGCWrapper::AddMissile(ImissileIGCWrapper^ ms)
	{
		return m_instance->AddMissile(ms->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteMissile(ImissileIGCWrapper^ m)
	{
		return m_instance->DeleteMissile(m->m_instance);
	}
	ImissileIGCWrapper ^ IclusterIGCWrapper::GetMissile(MissileID missileID)
	{
		::ImissileIGC * unmanagedValue = (::ImissileIGC *) m_instance->GetMissile(missileID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImissileIGCWrapper(unmanagedValue);
	}
	const MissileListIGC*   IclusterIGCWrapper::GetMissiles()
	{
		return m_instance->GetMissiles();
	}
	void                    IclusterIGCWrapper::AddProbe(IprobeIGCWrapper^ ms)
	{
		return m_instance->AddProbe(ms->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteProbe(IprobeIGCWrapper^ m)
	{
		return m_instance->DeleteProbe(m->m_instance);
	}
	IprobeIGCWrapper ^ IclusterIGCWrapper::GetProbe(ProbeID probeID)
	{
		::IprobeIGC * unmanagedValue = (::IprobeIGC *) m_instance->GetProbe(probeID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IprobeIGCWrapper(unmanagedValue);
	}
	const ProbeListIGC*     IclusterIGCWrapper::GetProbes()
	{
		return m_instance->GetProbes();
	}
	float                   IclusterIGCWrapper::GetScreenX()
	{
		return m_instance->GetScreenX();
	}
	float                   IclusterIGCWrapper::GetScreenY()
	{
		return m_instance->GetScreenY();
	}
	void                    IclusterIGCWrapper::SetActive(bool bActive)
	{
		return m_instance->SetActive(bActive);
	}
	void                    IclusterIGCWrapper::AddStation(IstationIGCWrapper^ stationNew)
	{
		return m_instance->AddStation(stationNew->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteStation(IstationIGCWrapper^ stationOld)
	{
		return m_instance->DeleteStation(stationOld->m_instance);
	}
	IstationIGCWrapper ^ IclusterIGCWrapper::GetStation(StationID stationID)
	{
		::IstationIGC * unmanagedValue = (::IstationIGC *) m_instance->GetStation(stationID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationIGCWrapper(unmanagedValue);
	}
	GenericList(IstationIGCWrapper)   IclusterIGCWrapper::GetStations()
	{
		ConvertSList(IstationIGC, StationLinkIGC, m_instance->GetStations())
	}
	void                    IclusterIGCWrapper::AddShip(IshipIGCWrapper^ shipNew)
	{
		return m_instance->AddShip(shipNew->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteShip(IshipIGCWrapper^ shipOld)
	{
		return m_instance->DeleteShip(shipOld->m_instance);
	}
	IshipIGCWrapper ^ IclusterIGCWrapper::GetShip(ShipID shipID)
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetShip(shipID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	GenericList(IshipIGCWrapper)      IclusterIGCWrapper::GetShips()
	{
		ConvertSList(IshipIGC, ShipLinkIGC, m_instance->GetShips())
	}
	void                    IclusterIGCWrapper::AddAsteroid(IasteroidIGCWrapper^ asteroidNew)
	{
		return m_instance->AddAsteroid(asteroidNew->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteAsteroid(IasteroidIGCWrapper^ asteroidOld)
	{
		return m_instance->DeleteAsteroid(asteroidOld->m_instance);
	}
	IasteroidIGCWrapper ^ IclusterIGCWrapper::GetAsteroid(AsteroidID asteroidID)
	{
		::IasteroidIGC * unmanagedValue = (::IasteroidIGC *) m_instance->GetAsteroid(asteroidID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IasteroidIGCWrapper(unmanagedValue);
	}
	GenericList(IasteroidIGCWrapper)  IclusterIGCWrapper::GetAsteroids()
	{
		ConvertSList(IasteroidIGC, AsteroidLinkIGC, m_instance->GetAsteroids());
	}
	void                    IclusterIGCWrapper::AddTreasure(ItreasureIGCWrapper^ treasureNew)
	{
		return m_instance->AddTreasure(treasureNew->m_instance);
	}
	void                    IclusterIGCWrapper::DeleteTreasure(ItreasureIGCWrapper^ treasureOld)
	{
		return m_instance->DeleteTreasure(treasureOld->m_instance);
	}
	ItreasureIGCWrapper ^ IclusterIGCWrapper::GetTreasure(TreasureID treasureID)
	{
		::ItreasureIGC * unmanagedValue = (::ItreasureIGC *) m_instance->GetTreasure(treasureID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ItreasureIGCWrapper(unmanagedValue);
	}
	const TreasureListIGC*  IclusterIGCWrapper::GetTreasures()
	{
		return m_instance->GetTreasures();
	}
	ClusterSiteWrapper ^ IclusterIGCWrapper::GetClusterSite()
	{
		::ClusterSite * unmanagedValue = (::ClusterSite *) m_instance->GetClusterSite();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ClusterSiteWrapper(unmanagedValue);
	}
	short                   IclusterIGCWrapper::GetNstars()
	{
		return m_instance->GetNstars();
	}
	short                   IclusterIGCWrapper::GetNdebris()
	{
		return m_instance->GetNdebris();
	}
	unsigned int            IclusterIGCWrapper::GetStarSeed()
	{
		return m_instance->GetStarSeed();
	}
	Color                   IclusterIGCWrapper::GetLightColor()
	{
		return m_instance->GetLightColor();
	}
	Color                   IclusterIGCWrapper::GetLightColorAlt()
	{
		return m_instance->GetLightColorAlt();
	}
	float                   IclusterIGCWrapper::GetBidirectionalAmbientLevel()
	{
		return m_instance->GetBidirectionalAmbientLevel();
	}
	float                   IclusterIGCWrapper::GetAmbientLevel()
	{
		return m_instance->GetAmbientLevel();
	}
	String ^ IclusterIGCWrapper::GetPosterName()
	{
		return gcnew String(m_instance->GetPosterName());
	}
	void             IclusterIGCWrapper::SetPrivateData(DWORD dwPrivate)
	{
		return m_instance->SetPrivateData(dwPrivate);
	}
	DWORD            IclusterIGCWrapper::GetPrivateData()
	{
		return m_instance->GetPrivateData();
	}
	void             IclusterIGCWrapper::FlushCollisionQueue()
	{
		return m_instance->FlushCollisionQueue();
	}
	void             IclusterIGCWrapper::MakeModelStatic(ImodelIGCWrapper^ pmodel)
	{
		return m_instance->MakeModelStatic(pmodel->m_instance);
	}
	void             IclusterIGCWrapper::MakeModelUnhitable(ImodelIGCWrapper^ pmodel)
	{
		return m_instance->MakeModelUnhitable(pmodel->m_instance);
	}
	void             IclusterIGCWrapper::FreeThingSite(ModelAttributes mt, ImodelIGCWrapper^ pmodel, HitTest* pht)
	{
		return m_instance->FreeThingSite(mt, pmodel->m_instance, pht);
	}
	bool             IclusterIGCWrapper::GetHomeSector()
	{
		return m_instance->GetHomeSector();
	}
	float            IclusterIGCWrapper::GetPendingTreasures()
	{
		return m_instance->GetPendingTreasures();
	}
	void             IclusterIGCWrapper::SetPendingTreasures(float fpt)
	{
		return m_instance->SetPendingTreasures(fpt);
	}
	float            IclusterIGCWrapper::GetCost()
	{
		return m_instance->GetCost();
	}
	void			 IclusterIGCWrapper::SetHighlight(SideID sid, ClusterHighlight ch)
	{
		return m_instance->SetHighlight(sid, ch);
	}
	ClusterHighlight IclusterIGCWrapper::GetHighlight(SideID sid)
	{
		return m_instance->GetHighlight(sid);
	}
	IasteroidIGCWrapper::IasteroidIGCWrapper(::IasteroidIGC * instance) : IdamageIGCWrapper(instance)
	{
		m_instance = instance;
	}
	float                   IasteroidIGCWrapper::GetOre()
	{
		return m_instance->GetOre();
	}
	void                    IasteroidIGCWrapper::SetOre(float newVal)
	{
		return m_instance->SetOre(newVal);
	}
	float                   IasteroidIGCWrapper::MineOre(float dt)
	{
		return m_instance->MineOre(dt);
	}
	AsteroidAbilityBitMask  IasteroidIGCWrapper::GetCapabilities()
	{
		return m_instance->GetCapabilities();
	}
	bool                    IasteroidIGCWrapper::HasCapability(AsteroidAbilityBitMask aabm)
	{
		return m_instance->HasCapability(aabm);
	}
	IbuildingEffectIGCWrapper ^ IasteroidIGCWrapper::GetBuildingEffect()
	{
		::IbuildingEffectIGC * unmanagedValue = (::IbuildingEffectIGC *) m_instance->GetBuildingEffect();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbuildingEffectIGCWrapper(unmanagedValue);
	}
	void                    IasteroidIGCWrapper::SetBuildingEffect(IbuildingEffectIGCWrapper^ pbe)
	{
		return m_instance->SetBuildingEffect(pbe->m_instance);
	}/*
	 String ^ IasteroidIGCWrapper::GetTypeName(AsteroidAbilityBitMask aabm)
	 {
	 return gcnew String(m_instance->GetTypeName(aabm));
	 }*//*
	 String ^ IasteroidIGCWrapper::GetTypePrefix(int index)
	 {
	 return gcnew String(m_instance->GetTypePrefix(index));
	 }*//*
	 static int                      IasteroidIGCWrapper::NumberSpecialAsteroids(MissionParamsWrapper^ pmp)
	 {
	 return m_instance->NumberSpecialAsteroids(pmp->m_instance);
	 }*//*
	 static int                      IasteroidIGCWrapper::GetSpecialAsterioid(MissionParamsWrapper^ pmp, int index)
	 {
	 return m_instance->GetSpecialAsterioid(pmp->m_instance, index);
	 }*//*
	 static int                      IasteroidIGCWrapper::GetRandomType(AsteroidAbilityBitMask aabm)
	 {
	 return m_instance->GetRandomType(aabm);
	 }*/
	float IasteroidIGCWrapper::GetOreSeenBySide(IsideIGCWrapper ^ side1)
	{
		return m_instance->GetOreSeenBySide(side1->m_instance);
	}
	bool IasteroidIGCWrapper::GetAsteroidCurrentEye(IsideIGCWrapper ^ side1)
	{
		return m_instance->GetAsteroidCurrentEye(side1->m_instance);
	}
	void IasteroidIGCWrapper::SetOreWithFraction(float oreFraction, bool clientUpdate)
	{
		return m_instance->SetOreWithFraction(oreFraction, clientUpdate);
	}
	float IasteroidIGCWrapper::GetOreFraction()
	{
		return m_instance->GetOreFraction();
	}
	void IasteroidIGCWrapper::SetBuilderSeenSide(ObjectID oid)
	{
		return m_instance->SetBuilderSeenSide(oid);
	}
	bool IasteroidIGCWrapper::GetBuilderSeenSide(ObjectID oid)
	{
		return m_instance->GetBuilderSeenSide(oid);
	}
	void IasteroidIGCWrapper::SetInhibitUpdate(bool inhib)
	{
		return m_instance->SetInhibitUpdate(inhib);
	}
	IwarpIGCWrapper::IwarpIGCWrapper(::IwarpIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IwarpIGCWrapper ^ IwarpIGCWrapper::GetDestination()
	{
		::IwarpIGC * unmanagedValue = (::IwarpIGC *) m_instance->GetDestination();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IwarpIGCWrapper(unmanagedValue);
	}
	const WarpBombList* IwarpIGCWrapper::GetBombs()
	{
		return m_instance->GetBombs();
	}
	bool                IwarpIGCWrapper::IsFixedPosition()
	{
		return m_instance->IsFixedPosition();
	}
	double			 IwarpIGCWrapper::MassLimit()
	{
		return m_instance->MassLimit();
	}
	ItreasureIGCWrapper::ItreasureIGCWrapper(::ItreasureIGC * instance) : ImodelIGCWrapper(instance)
	{
		m_instance = instance;
	}
	TreasureCode        ItreasureIGCWrapper::GetTreasureCode()
	{
		return m_instance->GetTreasureCode();
	}
	IbuyableIGCWrapper ^ ItreasureIGCWrapper::GetBuyable()
	{
		::IbuyableIGC * unmanagedValue = (::IbuyableIGC *) m_instance->GetBuyable();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbuyableIGCWrapper(unmanagedValue);
	}
	ObjectID            ItreasureIGCWrapper::GetTreasureID()
	{
		return m_instance->GetTreasureID();
	}
	void                ItreasureIGCWrapper::SetTreasureID(ObjectID newVal)
	{
		return m_instance->SetTreasureID(newVal);
	}
	short               ItreasureIGCWrapper::GetAmount()
	{
		return m_instance->GetAmount();
	}
	void                ItreasureIGCWrapper::SetAmount(short a)
	{
		return m_instance->SetAmount(a);
	}
	void                ItreasureIGCWrapper::ResetExpiration(Time now)
	{
		return m_instance->ResetExpiration(now);
	}
	IsideIGCWrapper::IsideIGCWrapper(::IsideIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	IcivilizationIGCWrapper ^ IsideIGCWrapper::GetCivilization()
	{
		::IcivilizationIGC * unmanagedValue = (::IcivilizationIGC *) m_instance->GetCivilization();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IcivilizationIGCWrapper(unmanagedValue);
	}
	void                        IsideIGCWrapper::SetCivilization(IcivilizationIGCWrapper^ pciv)
	{
		return m_instance->SetCivilization(pciv->m_instance);
	}
	void                        IsideIGCWrapper::DestroyBuckets()
	{
		return m_instance->DestroyBuckets();
	}
	void                        IsideIGCWrapper::CreateBuckets()
	{
		return m_instance->CreateBuckets();
	}
	void                        IsideIGCWrapper::SetName(const char* newVal)
	{
		return m_instance->SetName(newVal);
	}
	String ^ IsideIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	SquadID                     IsideIGCWrapper::GetSquadID()
	{
		return m_instance->GetSquadID();
	}
	void                        IsideIGCWrapper::SetSquadID(SquadID squadID)
	{
		return m_instance->SetSquadID(squadID);
	}
	const TechTreeBitMask       IsideIGCWrapper::GetTechs()
	{
		return m_instance->GetTechs();
	}
	void                        IsideIGCWrapper::ResetBuildingTechs()
	{
		return m_instance->ResetBuildingTechs();
	}
	void                        IsideIGCWrapper::SetBuildingTechs(const TechTreeBitMask& ttbm)
	{
		return m_instance->SetBuildingTechs(ttbm);
	}
	void                        IsideIGCWrapper::SetDevelopmentTechs(const TechTreeBitMask& ttbm)
	{
		return m_instance->SetDevelopmentTechs(ttbm);
	}
	bool					 IsideIGCWrapper::IsNewDevelopmentTechs(const TechTreeBitMask& ttbm)
	{
		return m_instance->IsNewDevelopmentTechs(ttbm);
	}
	bool                        IsideIGCWrapper::ApplyDevelopmentTechs(const TechTreeBitMask& ttbm)
	{
		return m_instance->ApplyDevelopmentTechs(ttbm);
	}
	void                        IsideIGCWrapper::SetInitialTechs(const TechTreeBitMask& ttbm)
	{
		return m_instance->SetInitialTechs(ttbm);
	}
	void                        IsideIGCWrapper::UpdateInitialTechs()
	{
		return m_instance->UpdateInitialTechs();
	}
	void                        IsideIGCWrapper::AddStation(IstationIGCWrapper^ s)
	{
		return m_instance->AddStation(s->m_instance);
	}
	void                        IsideIGCWrapper::DeleteStation(IstationIGCWrapper^ s)
	{
		return m_instance->DeleteStation(s->m_instance);
	}
	IstationIGCWrapper ^ IsideIGCWrapper::GetStation(StationID id)
	{
		::IstationIGC * unmanagedValue = (::IstationIGC *) m_instance->GetStation(id);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationIGCWrapper(unmanagedValue);
	}
	GenericList(IstationIGCWrapper) IsideIGCWrapper::GetStations()
	{
		ConvertSList(IstationIGC, StationLinkIGC, m_instance->GetStations())
	}
	void                        IsideIGCWrapper::AddShip(IshipIGCWrapper^ s)
	{
		return m_instance->AddShip(s->m_instance);
	}
	void                        IsideIGCWrapper::DeleteShip(IshipIGCWrapper^ s)
	{
		return m_instance->DeleteShip(s->m_instance);
	}
	IshipIGCWrapper ^ IsideIGCWrapper::GetShip(ShipID id)
	{
		::IshipIGC * unmanagedValue = (::IshipIGC *) m_instance->GetShip(id);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IshipIGCWrapper(unmanagedValue);
	}
	GenericList(IshipIGCWrapper) IsideIGCWrapper::GetShips()
	{
		ConvertSList(IshipIGC, ShipLinkIGC, m_instance->GetShips())
	}
	void                        IsideIGCWrapper::AddBucket(IbucketIGCWrapper^ s)
	{
		return m_instance->AddBucket(s->m_instance);
	}
	void                        IsideIGCWrapper::DeleteBucket(IbucketIGCWrapper^ s)
	{
		return m_instance->DeleteBucket(s->m_instance);
	}
	IbucketIGCWrapper ^ IsideIGCWrapper::GetBucket(BucketID bucketID)
	{
		::IbucketIGC * unmanagedValue = (::IbucketIGC *) m_instance->GetBucket(bucketID);

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IbucketIGCWrapper(unmanagedValue);
	}
	System::Collections::Generic::List<IbucketIGCWrapper ^> ^        IsideIGCWrapper::GetBuckets()
	{
		System::Collections::Generic::List<IbucketIGCWrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<IbucketIGCWrapper ^>();

		for (BucketLinkIGC* pbl = m_instance->GetBuckets()->last(); (pbl != NULL); pbl = pbl->txen())
		{
			IbucketIGC*     pbucket = pbl->data();
			returnValue->Add(gcnew IbucketIGCWrapper(pbucket));
		}
		return returnValue;
	}
	bool                        IsideIGCWrapper::CanBuy(IbuyableIGCWrapper^ b)
	{
		return m_instance->CanBuy(b->m_instance);
	}
	void                        IsideIGCWrapper::SetGlobalAttributeSet(const GlobalAttributeSet& gas)
	{
		return m_instance->SetGlobalAttributeSet(gas);
	}
	void                        IsideIGCWrapper::ApplyGlobalAttributeSet(const GlobalAttributeSet& gas)
	{
		return m_instance->ApplyGlobalAttributeSet(gas);
	}
	void                        IsideIGCWrapper::ResetGlobalAttributeSet()
	{
		return m_instance->ResetGlobalAttributeSet();
	}
	bool                        IsideIGCWrapper::GetActiveF()
	{
		return m_instance->GetActiveF();
	}
	void                        IsideIGCWrapper::SetActiveF(bool activeF)
	{
		return m_instance->SetActiveF(activeF);
	}
	void                        IsideIGCWrapper::SetPrivateData(DWORD dwPrivate)
	{
		return m_instance->SetPrivateData(dwPrivate);
	}
	DWORD                       IsideIGCWrapper::GetPrivateData()
	{
		return m_instance->GetPrivateData();
	}
	short                       IsideIGCWrapper::GetKills()
	{
		return m_instance->GetKills();
	}
	void                        IsideIGCWrapper::AddKill()
	{
		return m_instance->AddKill();
	}
	short                       IsideIGCWrapper::GetDeaths()
	{
		return m_instance->GetDeaths();
	}
	void                        IsideIGCWrapper::AddDeath()
	{
		return m_instance->AddDeath();
	}
	short                       IsideIGCWrapper::GetEjections()
	{
		return m_instance->GetEjections();
	}
	void                        IsideIGCWrapper::AddEjection()
	{
		return m_instance->AddEjection();
	}
	short                       IsideIGCWrapper::GetBaseKills()
	{
		return m_instance->GetBaseKills();
	}
	void                        IsideIGCWrapper::AddBaseKill()
	{
		return m_instance->AddBaseKill();
	}
	short                       IsideIGCWrapper::GetBaseCaptures()
	{
		return m_instance->GetBaseCaptures();
	}
	void                        IsideIGCWrapper::AddBaseCapture()
	{
		return m_instance->AddBaseCapture();
	}
	void                        IsideIGCWrapper::Reset()
	{
		return m_instance->Reset();
	}
	void                        IsideIGCWrapper::AddToStockpile(IbuyableIGCWrapper^ b, short count)
	{
		return m_instance->AddToStockpile(b->m_instance, count);
	}
	short                       IsideIGCWrapper::RemoveFromStockpile(IbuyableIGCWrapper^ b, short count)
	{
		return m_instance->RemoveFromStockpile(b->m_instance, count);
	}
	short                       IsideIGCWrapper::GetStockpile(IbuyableIGCWrapper^ b)
	{
		return m_instance->GetStockpile(b->m_instance);
	}
	const StockpileList*        IsideIGCWrapper::GetStockpile()
	{
		return m_instance->GetStockpile();
	}
	unsigned char IsideIGCWrapper::GetConquestPercent()
	{
		return m_instance->GetConquestPercent();
	}
	void          IsideIGCWrapper::SetConquestPercent(unsigned char newVal)
	{
		return m_instance->SetConquestPercent(newVal);
	}
	unsigned char IsideIGCWrapper::GetTerritoryCount()
	{
		return m_instance->GetTerritoryCount();
	}
	void          IsideIGCWrapper::SetTerritoryCount(unsigned char newVal)
	{
		return m_instance->SetTerritoryCount(newVal);
	}
	short         IsideIGCWrapper::GetFlags()
	{
		return m_instance->GetFlags();
	}
	void          IsideIGCWrapper::SetFlags(short newVal)
	{
		return m_instance->SetFlags(newVal);
	}
	short         IsideIGCWrapper::GetArtifacts()
	{
		return m_instance->GetArtifacts();
	}
	void          IsideIGCWrapper::SetArtifacts(short newVal)
	{
		return m_instance->SetArtifacts(newVal);
	}
	float         IsideIGCWrapper::GetTimeEndured()
	{
		return m_instance->GetTimeEndured();
	}
	void          IsideIGCWrapper::SetTimeEndured(float fSeconds)
	{
		return m_instance->SetTimeEndured(fSeconds);
	}
	long          IsideIGCWrapper::GetProsperityPercentBought()
	{
		return m_instance->GetProsperityPercentBought();
	}
	long          IsideIGCWrapper::GetProsperityPercentComplete()
	{
		return m_instance->GetProsperityPercentComplete();
	}
	void		  IsideIGCWrapper::SetAllies(char allies)
	{
		return m_instance->SetAllies(allies);
	}
	char          IsideIGCWrapper::GetAllies()
	{
		return m_instance->GetAllies();
	}/*
	 static bool           IsideIGCWrapper::AlliedSides(IsideIGCWrapper ^ side1, IsideIGCWrapper ^ side2)
	 {
	 return m_instance->AlliedSides(side1->m_instance, side2->m_instance);
	 }*/
	bool IsideIGCWrapper::GetRandomCivilization()
	{
		return m_instance->GetRandomCivilization();
	}
	void IsideIGCWrapper::SetRandomCivilization(bool rand)
	{
		return m_instance->SetRandomCivilization(rand);
	}
	int IsideIGCWrapper::GetNumPlayersOnSide()
	{
		return m_instance->GetNumPlayersOnSide();
	}
	void IsideIGCWrapper::HandleNewEnemyCluster(IclusterIGCWrapper^ pcluster)
	{
		return m_instance->HandleNewEnemyCluster(pcluster->m_instance);
	}
	void IsideIGCWrapper::UpdateTerritory()
	{
		return m_instance->UpdateTerritory();
	}
	ClusterListIGC IsideIGCWrapper::GetTerritory()
	{
		return m_instance->GetTerritory();
	}
	bool IsideIGCWrapper::IsTerritory(IclusterIGCWrapper^ pcluster)
	{
		return m_instance->IsTerritory(pcluster->m_instance);
	}
	IcivilizationIGCWrapper::IcivilizationIGCWrapper(::IcivilizationIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	String ^ IcivilizationIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	String ^ IcivilizationIGCWrapper::GetIconName()
	{
		return gcnew String(m_instance->GetIconName());
	}
	String ^ IcivilizationIGCWrapper::GetHUDName()
	{
		return gcnew String(m_instance->GetHUDName());
	}
	IhullTypeIGCWrapper ^ IcivilizationIGCWrapper::GetLifepod()
	{
		::IhullTypeIGC * unmanagedValue = (::IhullTypeIGC *) m_instance->GetLifepod();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IhullTypeIGCWrapper(unmanagedValue);
	}
	IstationTypeIGCWrapper ^ IcivilizationIGCWrapper::GetInitialStationType()
	{
		::IstationTypeIGC * unmanagedValue = (::IstationTypeIGC *) m_instance->GetInitialStationType();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IstationTypeIGCWrapper(unmanagedValue);
	}
	float                       IcivilizationIGCWrapper::GetBonusMoney()
	{
		return m_instance->GetBonusMoney();
	}
	float                       IcivilizationIGCWrapper::GetIncomeMoney()
	{
		return m_instance->GetIncomeMoney();
	}
	ItreasureSetIGCWrapper::ItreasureSetIGCWrapper(::ItreasureSetIGC * instance) : IbaseIGCWrapper(instance)
	{
		m_instance = instance;
	}
	String ^ ItreasureSetIGCWrapper::GetName()
	{
		return gcnew String(m_instance->GetName());
	}
	bool                        ItreasureSetIGCWrapper::GetZoneOnly()
	{
		return m_instance->GetZoneOnly();
	}
	short					 ItreasureSetIGCWrapper::GetSize()
	{
		return m_instance->GetSize();
	}
	void                        ItreasureSetIGCWrapper::AddTreasureData(TreasureCode tc, ObjectID oid, unsigned char chance)
	{
		return m_instance->AddTreasureData(tc, oid, chance);
	}
	ShipStatusWrapper::ShipStatusWrapper(::ShipStatus * instance)
	{
		m_instance = instance;
	}
	void    ShipStatusWrapper::Reset()
	{
		return m_instance->Reset();
	}
	HullID      ShipStatusWrapper::GetHullID()
	{
		return m_instance->GetHullID();
	}
	void        ShipStatusWrapper::SetHullID(HullID newVal)
	{
		return m_instance->SetHullID(newVal);
	}
	SectorID    ShipStatusWrapper::GetSectorID()
	{
		return m_instance->GetSectorID();
	}
	void        ShipStatusWrapper::SetSectorID(SectorID newVal)
	{
		return m_instance->SetSectorID(newVal);
	}
	StationID   ShipStatusWrapper::GetStationID()
	{
		return m_instance->GetStationID();
	}
	void        ShipStatusWrapper::SetStationID(StationID stationID)
	{
		return m_instance->SetStationID(stationID);
	}
	StationID   ShipStatusWrapper::GetParentID()
	{
		return m_instance->GetParentID();
	}
	void        ShipStatusWrapper::SetParentID(ShipID shipID)
	{
		return m_instance->SetParentID(shipID);
	}
	ShipState   ShipStatusWrapper::GetState()
	{
		return m_instance->GetState();
	}
	void        ShipStatusWrapper::SetState(ShipState newVal)
	{
		return m_instance->SetState(newVal);
	}
	bool        ShipStatusWrapper::GetUnknown()
	{
		return m_instance->GetUnknown();
	}
	void        ShipStatusWrapper::SetUnknown()
	{
		return m_instance->SetUnknown();
	}
	bool        ShipStatusWrapper::GetDetected()
	{
		return m_instance->GetDetected();
	}
	void        ShipStatusWrapper::SetDetected(bool bDetected)
	{
		return m_instance->SetDetected(bDetected);
	}
	DWORD        ShipStatusWrapper::GetStateTime()
	{
		return m_instance->GetStateTime();
	}
	void        ShipStatusWrapper::SetStateTime(DWORD dTime)
	{
		return m_instance->SetStateTime(dTime);
	}
	ClusterSiteWrapper::ClusterSiteWrapper(::ClusterSite * instance) : AttachSiteWrapper(instance)
	{
		m_instance = instance;
	}
	HRESULT         ClusterSiteWrapper::GetEnvironmentGeo(const char* pszName)
	{
		return m_instance->GetEnvironmentGeo(pszName);
	}
	GeoWrapper ^ ClusterSiteWrapper::GetEnvironmentGeo()
	{
		::Geo * unmanagedValue = (::Geo *) m_instance->GetEnvironmentGeo();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew GeoWrapper(unmanagedValue);
	}
	GroupGeo*       ClusterSiteWrapper::GetGroupScene()
	{
		return m_instance->GetGroupScene();
	}
	PosterImage*    ClusterSiteWrapper::GetPosterImage()
	{
		return m_instance->GetPosterImage();
	}
	void            ClusterSiteWrapper::SetEnvironmentGeo(const char* pszName)
	{
		return m_instance->SetEnvironmentGeo(pszName);
	}
	float           ClusterSiteWrapper::GetRotation()
	{
		return m_instance->GetRotation();
	}
	void            ClusterSiteWrapper::AddRotation(float da)
	{
		return m_instance->AddRotation(da);
	}
	void                    ClusterSiteWrapper::AddScanner(SideID sid, IscannerIGCWrapper^ scannerNew)
	{
		return m_instance->AddScanner(sid, scannerNew->m_instance);
	}
	void                    ClusterSiteWrapper::DeleteScanner(SideID sid, IscannerIGCWrapper^ scannerOld)
	{
		return m_instance->DeleteScanner(sid, scannerOld->m_instance);
	}
	const ScannerListIGC*   ClusterSiteWrapper::GetScanners(SideID sid)
	{
		return m_instance->GetScanners(sid);
	}
	AssetMask               ClusterSiteWrapper::GetClusterAssetMask()
	{
		return m_instance->GetClusterAssetMask();
	}
	void                    ClusterSiteWrapper::SetClusterAssetMask(AssetMask am)
	{
		return m_instance->SetClusterAssetMask(am);
	}
	void                    ClusterSiteWrapper::MoveShip()
	{
		return m_instance->MoveShip();
	}
	SideVisibilityWrapper::SideVisibilityWrapper(::SideVisibility * instance)
	{
		m_instance = instance;
	}
	bool    SideVisibilityWrapper::fVisible()
	{
		return m_instance->fVisible();
	}
	IscannerIGCWrapper ^ SideVisibilityWrapper::pLastSpotter()
	{
		::IscannerIGC * unmanagedValue = (::IscannerIGC *) m_instance->pLastSpotter();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew IscannerIGCWrapper(unmanagedValue);
	}
	void        SideVisibilityWrapper::fVisible(bool v)
	{
		return m_instance->fVisible(v);
	}
	void        SideVisibilityWrapper::pLastSpotter(IscannerIGCWrapper^ s)
	{
		return m_instance->pLastSpotter(s->m_instance);
	}
	void SideVisibilityWrapper::CurrentEyed(bool v)
	{
		return m_instance->CurrentEyed(v);
	}
	bool SideVisibilityWrapper::CurrentEyed()
	{
		return m_instance->CurrentEyed();
	}
	GameSiteWrapper::GameSiteWrapper(::GameSite * instance)
	{
		m_instance = instance;
	}/*
	 static int GameSiteWrapper::MessageBox(const ZString& strText, const ZString& strCaption, UINT nType)
	 {
	 return m_instance->MessageBox(strText, strCaption, nType);
	 }*/
	IIgcSiteWrapper::IIgcSiteWrapper(::IIgcSite * instance)
	{
		m_instance = instance;
	}
	Time IIgcSiteWrapper::ServerTimeFromClientTime(Time timeClient)
	{
		return m_instance->ServerTimeFromClientTime(timeClient);
	}
	Time IIgcSiteWrapper::ClientTimeFromServerTime(Time timeServer)
	{
		return m_instance->ClientTimeFromServerTime(timeServer);
	}
	void    IIgcSiteWrapper::DevelopmentCompleted(IbucketIGCWrapper^ b, IdevelopmentIGCWrapper^ d, Time now)
	{
		return m_instance->DevelopmentCompleted(b->m_instance, d->m_instance, now);
	}
	void    IIgcSiteWrapper::StationTypeCompleted(IbucketIGCWrapper^ b, IstationIGCWrapper^ pstation, IstationTypeIGCWrapper^ st, Time now)
	{
		return m_instance->StationTypeCompleted(b->m_instance, pstation->m_instance, st->m_instance, now);
	}
	void    IIgcSiteWrapper::HullTypeCompleted(IsideIGCWrapper^ pside, IhullTypeIGCWrapper^ pht)
	{
		return m_instance->HullTypeCompleted(pside->m_instance, pht->m_instance);
	}
	void    IIgcSiteWrapper::PartTypeCompleted(IsideIGCWrapper^ pside, IpartTypeIGCWrapper^ ppt)
	{
		return m_instance->PartTypeCompleted(pside->m_instance, ppt->m_instance);
	}
	void    IIgcSiteWrapper::DroneTypeCompleted(IbucketIGCWrapper^ b, IstationIGCWrapper^ pstation, IdroneTypeIGCWrapper^ dt, Time now)
	{
		return m_instance->DroneTypeCompleted(b->m_instance, pstation->m_instance, dt->m_instance, now);
	}
	void    IIgcSiteWrapper::BucketChangeEvent(BucketChange bc, IbucketIGCWrapper^ b)
	{
		return m_instance->BucketChangeEvent(bc, b->m_instance);
	}
	void    IIgcSiteWrapper::SideBuildingTechChange(IsideIGCWrapper^ s)
	{
		return m_instance->SideBuildingTechChange(s->m_instance);
	}
	void    IIgcSiteWrapper::SideDevelopmentTechChange(IsideIGCWrapper^ s)
	{
		return m_instance->SideDevelopmentTechChange(s->m_instance);
	}
	void    IIgcSiteWrapper::SideGlobalAttributeChange(IsideIGCWrapper^ s)
	{
		return m_instance->SideGlobalAttributeChange(s->m_instance);
	}
	void    IIgcSiteWrapper::StationTypeChange(IstationIGCWrapper^ s)
	{
		return m_instance->StationTypeChange(s->m_instance);
	}
	void    IIgcSiteWrapper::ClusterUpdateEvent(IclusterIGCWrapper^ c)
	{
		return m_instance->ClusterUpdateEvent(c->m_instance);
	}
	void IIgcSiteWrapper::GetMoneyRequest(IshipIGCWrapper^ pshipSender, Money amount, HullID hidFor)
	{
		return m_instance->GetMoneyRequest(pshipSender->m_instance, amount, hidFor);
	}
	void IIgcSiteWrapper::PlaySoundEffect(SoundID soundID, ImodelIGCWrapper^ model)
	{
		return m_instance->PlaySoundEffect(soundID, model->m_instance);
	}
	void IIgcSiteWrapper::PlaySoundEffect(SoundID soundID, ImodelIGCWrapper^ model, const VectorWrapper ^ vectOffset)
	{
		return m_instance->PlaySoundEffect(soundID, model->m_instance, *vectOffset->m_instance);
	}
	void IIgcSiteWrapper::PlayNotificationSound(SoundID soundID, ImodelIGCWrapper^ model)
	{
		return m_instance->PlayNotificationSound(soundID, model->m_instance);
	}
	void IIgcSiteWrapper::PlayFFEffect(ForceEffectID effectID, ImodelIGCWrapper^ model, LONG lDirection)
	{
		return m_instance->PlayFFEffect(effectID, model->m_instance, lDirection);
	}
	void IIgcSiteWrapper::PlayVisualEffect(VisualEffectID effectID, ImodelIGCWrapper^ model, float fIntensity)
	{
		return m_instance->PlayVisualEffect(effectID, model->m_instance, fIntensity);
	}
	DamageTrackWrapper::DamageTrackWrapper(::DamageTrack * instance)
	{
		m_instance = instance;
	}
	void    DamageTrackWrapper::SwitchSlots()
	{
		return m_instance->SwitchSlots();
	}
	void    DamageTrackWrapper::Reset()
	{
		return m_instance->Reset();
	}
	void    DamageTrackWrapper::AddDamageBucket(DamageBucketWrapper^ pdb)
	{
		return m_instance->AddDamageBucket(pdb->m_instance);
	}
	void    DamageTrackWrapper::DeleteDamageBucket(DamageBucketWrapper^ pdb)
	{
		return m_instance->DeleteDamageBucket(pdb->m_instance);
	}
	DamageBucketList*    DamageTrackWrapper::GetDamageBuckets()
	{
		return m_instance->GetDamageBuckets();
	}/*
	 static void DamageTrackWrapper::sort(DamageBucketList* pListBuckets)
	 {
	 return m_instance->sort(pListBuckets);
	 }*/
	DamageBucketWrapper::DamageBucketWrapper(::DamageBucket * instance)
	{
		m_instance = instance;
	}
	void    DamageBucketWrapper::Initialize(Time timeNow, ImodelIGCWrapper^ pmodel)
	{
		return m_instance->Initialize(timeNow, pmodel->m_instance);
	}
	ImodelIGCWrapper ^ DamageBucketWrapper::model()
	{
		::ImodelIGC * unmanagedValue = (::ImodelIGC *) m_instance->model();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew ImodelIGCWrapper(unmanagedValue);
	}
	float       DamageBucketWrapper::totalDamage()
	{
		return m_instance->totalDamage();
	}
	bool        DamageBucketWrapper::flash(Time timeNow)
	{
		return m_instance->flash(timeNow);
	}
	void        DamageBucketWrapper::SwitchSlots(int idSlot)
	{
		return m_instance->SwitchSlots(idSlot);
	}
	DamageTrackSetWrapper::DamageTrackSetWrapper(::DamageTrackSet * instance)
	{
		m_instance = instance;
	}
	void DamageTrackSetWrapper::Initialize(Time now)
	{
		return m_instance->Initialize(now);
	}
	void DamageTrackSetWrapper::Update(Time now)
	{
		return m_instance->Update(now);
	}
	DamageTrackWrapper ^ DamageTrackSetWrapper::Create()
	{
		::DamageTrack * unmanagedValue = (::DamageTrack *) m_instance->Create();

		if (unmanagedValue == nullptr)
			return nullptr;
		else
			return gcnew DamageTrackWrapper(unmanagedValue);
	}
	//void            DamageTrackSetWrapper::AddTrack(DamageTrackWrapper^ pdt)
	//		{
	//			return m_instance->AddTrack(pdt->m_instance);
	//		}
	//void            DamageTrackSetWrapper::DeleteTrack(DamageTrackWrapper^ pdt)
	//		{
	//			return m_instance->DeleteTrack(pdt->m_instance);
	//		}
	WaypointWrapper::WaypointWrapper(::Waypoint * instance)
	{
		m_instance = instance;
	}
	GotoPlanWrapper::GotoPlanWrapper(::GotoPlan * instance)
	{
		m_instance = instance;
	}
	PersistPlayerScoreObjectWrapper::PersistPlayerScoreObjectWrapper(::PersistPlayerScoreObject * instance)
	{
		m_instance = instance;
	}
	void    PersistPlayerScoreObjectWrapper::Reset()
	{
		return m_instance->Reset();
	}
	short   PersistPlayerScoreObjectWrapper::GetRank()
	{
		return m_instance->GetRank();
	}
	float   PersistPlayerScoreObjectWrapper::GetScore()
	{
		return m_instance->GetScore();
	}
	void  PersistPlayerScoreObjectWrapper::SetCivID(CivID civID)
	{
		return m_instance->SetCivID(civID);
	}
	CivID PersistPlayerScoreObjectWrapper::GetCivID()
	{
		return m_instance->GetCivID();
	}
	float   PersistPlayerScoreObjectWrapper::GetCombatRating()
	{
		return m_instance->GetCombatRating();
	}
	void    PersistPlayerScoreObjectWrapper::SetCombatRating(float cr)
	{
		return m_instance->SetCombatRating(cr);
	}
	PlayerScoreObjectWrapper::PlayerScoreObjectWrapper(::PlayerScoreObject * instance)
	{
		m_instance = instance;
	}
	void    PlayerScoreObjectWrapper::CalculateScore(ImissionIGCWrapper^ pmission)
	{
		return m_instance->CalculateScore(pmission->m_instance);
	}
	void    PlayerScoreObjectWrapper::SetScore(float fNewScore)
	{
		return m_instance->SetScore(fNewScore);
	}
	void    PlayerScoreObjectWrapper::SetCommanderScore(float fCommanderScore)
	{
		return m_instance->SetCommanderScore(fCommanderScore);
	}
	float   PlayerScoreObjectWrapper::GetScore()
	{
		return m_instance->GetScore();
	}
	void    PlayerScoreObjectWrapper::Reset(bool bFull)
	{
		return m_instance->Reset(bFull);
	}
	void PlayerScoreObjectWrapper::SetRankRatio(float rankRatio)
	{
		return m_instance->SetRankRatio(rankRatio);
	}
	bool    PlayerScoreObjectWrapper::Connected()
	{
		return m_instance->Connected();
	}
	bool    PlayerScoreObjectWrapper::Commanding()
	{
		return m_instance->Commanding();
	}
	void    PlayerScoreObjectWrapper::Connect(Time t)
	{
		return m_instance->Connect(t);
	}
	void    PlayerScoreObjectWrapper::Disconnect(Time t)
	{
		return m_instance->Disconnect(t);
	}
	void    PlayerScoreObjectWrapper::StartCommand(Time t)
	{
		return m_instance->StartCommand(t);
	}
	void    PlayerScoreObjectWrapper::StopCommand(Time t)
	{
		return m_instance->StopCommand(t);
	}
	void    PlayerScoreObjectWrapper::SpotWarp()
	{
		return m_instance->SpotWarp();
	}
	void    PlayerScoreObjectWrapper::SpotSpecialAsteroid()
	{
		return m_instance->SpotSpecialAsteroid();
	}
	/*void PlayerScoreObjectWrapper::AddTargetSpot()
	{
		return m_instance->AddTargetSpot();
	}
	short   PlayerScoreObjectWrapper::GetTargetsSpotted()
	{
		return m_instance->GetTargetsSpotted();
	}*/
	void PlayerScoreObjectWrapper::SetRepair(float repair)
	{
		return m_instance->SetRepair(repair);
	}
	float PlayerScoreObjectWrapper::GetRepair()
	{
		return m_instance->GetRepair();
	}
	void    PlayerScoreObjectWrapper::KillBase(bool bPilot)
	{
		return m_instance->KillBase(bPilot);
	}
	void    PlayerScoreObjectWrapper::CaptureBase(bool bPilot)
	{
		return m_instance->CaptureBase(bPilot);
	}
	short   PlayerScoreObjectWrapper::GetPilotBaseKills()
	{
		return m_instance->GetPilotBaseKills();
	}
	short   PlayerScoreObjectWrapper::GetPilotBaseCaptures()
	{
		return m_instance->GetPilotBaseCaptures();
	}
	float   PlayerScoreObjectWrapper::GetWarpsSpotted()
	{
		return m_instance->GetWarpsSpotted();
	}
	float   PlayerScoreObjectWrapper::GetAsteroidsSpotted()
	{
		return m_instance->GetAsteroidsSpotted();
	}
	float   PlayerScoreObjectWrapper::GetMinerKills()
	{
		return m_instance->GetMinerKills();
	}
	float   PlayerScoreObjectWrapper::GetBuilderKills()
	{
		return m_instance->GetBuilderKills();
	}
	float   PlayerScoreObjectWrapper::GetLayerKills()
	{
		return m_instance->GetLayerKills();
	}
	float   PlayerScoreObjectWrapper::GetCarrierKills()
	{
		return m_instance->GetCarrierKills();
	}
	float   PlayerScoreObjectWrapper::GetPlayerKills()
	{
		return m_instance->GetPlayerKills();
	}
	float   PlayerScoreObjectWrapper::GetBaseKills()
	{
		return m_instance->GetBaseKills();
	}
	float   PlayerScoreObjectWrapper::GetBaseCaptures()
	{
		return m_instance->GetBaseCaptures();
	}
	short   PlayerScoreObjectWrapper::GetTechsRecovered()
	{
		return m_instance->GetTechsRecovered();
	}
	void    PlayerScoreObjectWrapper::AddTechsRecovered()
	{
		return m_instance->AddTechsRecovered();
	}
	short   PlayerScoreObjectWrapper::GetFlags()
	{
		return m_instance->GetFlags();
	}
	void    PlayerScoreObjectWrapper::AddFlag()
	{
		return m_instance->AddFlag();
	}
	short   PlayerScoreObjectWrapper::GetArtifacts()
	{
		return m_instance->GetArtifacts();
	}
	void    PlayerScoreObjectWrapper::AddArtifact()
	{
		return m_instance->AddArtifact();
	}
	void    PlayerScoreObjectWrapper::AddRescue()
	{
		return m_instance->AddRescue();
	}
	short   PlayerScoreObjectWrapper::GetRescues()
	{
		return m_instance->GetRescues();
	}
	short   PlayerScoreObjectWrapper::GetKills()
	{
		return m_instance->GetKills();
	}
	void    PlayerScoreObjectWrapper::AddKill()
	{
		return m_instance->AddKill();
	}
	short   PlayerScoreObjectWrapper::GetAssists()
	{
		return m_instance->GetAssists();
	}
	void    PlayerScoreObjectWrapper::AddAssist()
	{
		return m_instance->AddAssist();
	}
	short   PlayerScoreObjectWrapper::GetDeaths()
	{
		return m_instance->GetDeaths();
	}
	void    PlayerScoreObjectWrapper::AddDeath()
	{
		return m_instance->AddDeath();
	}
	void    PlayerScoreObjectWrapper::SetDeaths(short d)
	{
		return m_instance->SetDeaths(d);
	}
	short   PlayerScoreObjectWrapper::GetEjections()
	{
		return m_instance->GetEjections();
	}
	void    PlayerScoreObjectWrapper::AddEjection()
	{
		return m_instance->AddEjection();
	}
	void    PlayerScoreObjectWrapper::SetEjections(short e)
	{
		return m_instance->SetEjections(e);
	}
	float   PlayerScoreObjectWrapper::GetCombatRating()
	{
		return m_instance->GetCombatRating();
	}
	void    PlayerScoreObjectWrapper::SetCombatRating(float cr)
	{
		return m_instance->SetCombatRating(cr);
	}
	void                       PlayerScoreObjectWrapper::SetPersist(PersistPlayerScoreObjectWrapper ^ p)
	{
		return m_instance->SetPersist(p->m_instance);
	}
	bool                        PlayerScoreObjectWrapper::GetWinner()
	{
		return m_instance->GetWinner();
	}
	bool                        PlayerScoreObjectWrapper::GetLoser()
	{
		return m_instance->GetLoser();
	}
	bool                        PlayerScoreObjectWrapper::GetCommandWinner()
	{
		return m_instance->GetCommandWinner();
	}
	bool                        PlayerScoreObjectWrapper::GetCommandLoser()
	{
		return m_instance->GetCommandLoser();
	}
	float PlayerScoreObjectWrapper::GetTimePlayed()
	{
		return m_instance->GetTimePlayed();
	}
	float PlayerScoreObjectWrapper::GetTimeCommanded()
	{
		return m_instance->GetTimeCommanded();
	}
	bool    PlayerScoreObjectWrapper::GetCommandCredit()
	{
		return m_instance->GetCommandCredit();
	}
	GameOverScoreObjectWrapper::GameOverScoreObjectWrapper(::GameOverScoreObject * instance)
	{
		m_instance = instance;
	}
	void    GameOverScoreObjectWrapper::Set(PlayerScoreObjectWrapper^ ppso)
	{
		return m_instance->Set(ppso->m_instance);
	}
	short   GameOverScoreObjectWrapper::GetRank()
	{
		return m_instance->GetRank();
	}
	//float   GameOverScoreObjectWrapper::GetScore()
	//		{
	//			return m_instance->GetScore();
	//		}
	float   GameOverScoreObjectWrapper::GetRecentTimePlayed()
	{
		return m_instance->GetRecentTimePlayed();
	}
	short   GameOverScoreObjectWrapper::GetRecentBaseKills()
	{
		return m_instance->GetRecentBaseKills();
	}
	short   GameOverScoreObjectWrapper::GetRecentBaseCaptures()
	{
		return m_instance->GetRecentBaseCaptures();
	}
	short   GameOverScoreObjectWrapper::GetRecentKills()
	{
		return m_instance->GetRecentKills();
	}
	short   GameOverScoreObjectWrapper::GetRecentDeaths()
	{
		return m_instance->GetRecentDeaths();
	}
	short   GameOverScoreObjectWrapper::GetRecentEjections()
	{
		return m_instance->GetRecentEjections();
	}
	//short   GameOverScoreObjectWrapper::GetTotalBaseKills()
	//		{
	//			return m_instance->GetTotalBaseKills();
	//		}
	//short   GameOverScoreObjectWrapper::GetTotalBaseCaptures()
	//		{
	//			return m_instance->GetTotalBaseCaptures();
	//		}
	//short   GameOverScoreObjectWrapper::GetFlags()
	//		{
	//			return m_instance->GetFlags();
	//		}
	//short   GameOverScoreObjectWrapper::GetArtifacts()
	//		{
	//			return m_instance->GetArtifacts();
	//		}
	//short   GameOverScoreObjectWrapper::GetRescues()
	//		{
	//			return m_instance->GetRescues();
	//		}
	//short   GameOverScoreObjectWrapper::GetTotalKills()
	//		{
	//			return m_instance->GetTotalKills();
	//		}
	//short   GameOverScoreObjectWrapper::GetTotalDeaths()
	//		{
	//			return m_instance->GetTotalDeaths();
	//		}
	//short   GameOverScoreObjectWrapper::GetTotalEjections()
	//		{
	//			return m_instance->GetTotalEjections();
	//		}
	//short   GameOverScoreObjectWrapper::GetWins()
	//		{
	//			return m_instance->GetWins();
	//		}
	//short   GameOverScoreObjectWrapper::GetLosses()
	//		{
	//			return m_instance->GetLosses();
	//		}
	float   GameOverScoreObjectWrapper::GetRecentScore()
	{
		return m_instance->GetRecentScore();
	}
	unsigned short   GameOverScoreObjectWrapper::GetRecentAssists()
	{
		return m_instance->GetRecentAssists();
	}
	//float   GameOverScoreObjectWrapper::GetCombatRating()
	//		{
	//			return m_instance->GetCombatRating();
	//		}
	//bool    GameOverScoreObjectWrapper::GetWinner()
	//		{
	//			return m_instance->GetWinner();
	//		}
	//bool    GameOverScoreObjectWrapper::GetLoser()
	//		{
	//			return m_instance->GetLoser();
	//		}
	bool        GameOverScoreObjectWrapper::GetCommander()
	{
		return m_instance->GetCommander();
	}
	bool        GameOverScoreObjectWrapper::GetGameCounted()
	{
		return m_instance->GetGameCounted();
	}
}
