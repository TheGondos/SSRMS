#ifndef STATEVECTORPROPAGATOR_128F62F4_83F4_4BDD_967B_7E2CD4016D3F
#define STATEVECTORPROPAGATOR_128F62F4_83F4_4BDD_967B_7E2CD4016D3F
#pragma once

#include "OrbiterAPI.h"
#include "../KOST/kost.h"
#include <map>

class PropagatorPerturbation
{
public:
	virtual VECTOR3 GetAcceleration(double MET, const VECTOR3& equPos, const VECTOR3& equVel) = 0;
};

class StateVectorPropagator
{
	typedef std::map<double, kostStateVector> StateVectorMap;
	typedef StateVectorMap::iterator MapIterator;
	typedef StateVectorMap::const_iterator MapConstIterator;

	//std::map<double, kostStateVector> stateVectors;
	StateVectorMap stateVectors;

	double stepLength;
	int iterationsPerStep;
	double maxPropagationTime; // total time since last state vector update we will propagate vector
	//double currentTimestamp;
	double lastSaveMET;
	double lastStateVectorUpdateMET;
	//double maxTimestamp;
	double maxPropMET;

	VECTOR3 propPos, propVel;
	double propMET;

	double mu, GM, planetRadius, J2;
	PropagatorPerturbation* perturbFunction;
	//double vesselMass;
public:
	//StateVectorPropagator(const VECTOR3& pos, const VECTOR3& vel, OBJHANDLE hPlanet, double _vesselMass, double currentMET);
	StateVectorPropagator(double propagationStepLength, int _iterationsPerStep, double _maxPropagationTime);
	~StateVectorPropagator();

	void SetParameters(double vesselMass, double planetMass, double _planetRadius, double J2Coeff);
	/**
	 * Sets functor used to calculate perturbations (drag, thrust, etc.)
	 * Caller is responsible for ensuring object is not deleted while propagator is in use
	 * \param pertubations pointer to functor which will return perturbation acceleration for each timestep - NULL if no perturbations
	 */
	void DefinePerturbations(PropagatorPerturbation* pertubations);
	void UpdateVesselMass(double vesselMass);
	/**
	 * Updates state vector if last set of state vectors has been propagated to limit or difference between new and existing state vectors exceeds limit
	 * \param forceUpdate Forces state vectors to be updated.
	 * Returns true if error between new and existing vectors exceeded limit; false otherwise (if forceUpdate is true, function always returns false)
	 */
	bool UpdateStateVector(const VECTOR3& equPos, const VECTOR3& equVel, double met, bool forceUpdate=false);

	//void SetVesselMass(double newMass);

	//void GetState(VECTOR3& pos, VECTOR3& vel) const;
	void Step(double currentMET, double DeltaT);

	double GetElements(double MET, ELEMENTS& el, kostOrbitParam* oparam=NULL) const;
	void GetStateVectors(double MET, VECTOR3& pos, VECTOR3& vel) const;

	//double GetLastUpdateTime() const;

	//void GetApogeeData(double& ApD, double& METAtApogee) const;
	//void GetPerigeeData(double& PeD, double& METAtPerigee) const;
	void GetApogeeData(double currentMET, double& ApD, double& METAtApogee) const;
	void GetPerigeeData(double currentMET, double& PeD, double& METAtPerigee) const;
	/**
	 * Returns MET at which the vessel will reach a given altitude.
	 * \altitude altitude (relative to Earth's surface) in meters
	 */
	double GetMETAtAltitude(double currentMET, double altitude) const;
private:
	void Propagate(unsigned int iterationCount, double DeltaT);
	void CalculateAccelerationVector(VECTOR3& acc) const;
};

/**
 * Used with StateVectorPropagator to correct for OMS burn
 * Currently assumes constant acceleration (not quite correct, but should be good approximation)
 */
class OMSBurnPropagator : public PropagatorPerturbation
{
	VECTOR3 equBurnDirection;
	double deltaV;
	double TIG;
	double acc;
	bool burnInProgress, burnCompleted;
	double VGO; // used to track DeltaV applied so far
	double lastMET; // used for calculating interval between calls

	VECTOR3 cutoffPos, cutoffVel;
	VECTOR3 tigPos, tigVel;
public:
	OMSBurnPropagator();
	~OMSBurnPropagator();

	void SetBurnData(double TIG, const VECTOR3 &equDeltaV, double acceleration);
	void GetTIGStateVector(VECTOR3& pos, VECTOR3& vel) const;
	void GetCutoffStateVector(VECTOR3& pos, VECTOR3& vel) const;
	
	VECTOR3 GetAcceleration(double MET, const VECTOR3& equPos, const VECTOR3& equVel);
};

/**
 * Returns time (from epoch of elements passed) when spacecraft will next pass through given altitude.
 * Assumes altitude is between apogee and perigee of elements.
 * \radius distance (from centre of Earth) in meters
 */
double GetTimeToRadius(double radius, const ELEMENTS& el, double mu);

#endif //STATEVECTORPROPAGATOR_128F62F4_83F4_4BDD_967B_7E2CD4016D3F
