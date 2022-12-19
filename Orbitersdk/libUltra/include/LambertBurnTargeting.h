#ifndef LAMBERTBURNTARGETING_52AFC39B_152A_415C_9029_F5345F6B6930
#define LAMBERTBURNTARGETING_52AFC39B_152A_415C_9029_F5345F6B6930
#pragma once
#include "OrbiterAPI.h"
#include "StateVectorPropagator.h"

/**
 * Solves Lambert (or Gauss) problem iteratively.
 * Does not handle case where transfer angle is ~180 degrees (transfer orbit plane is undefined)
 * \param initialPos Initial position of shuttle (input)
 * \param finalPos Final position of shuttle (input)
 * \param transferTime Time for shuttle to travel from inital to final position (input)
 * \param orbitPeriod Period of current orbit (used to determine if transfer angle is greater or less than 180 degrees; does not need to be exact). (input)
 * \param mu G*(M+m) (input)
 * \param initialVelocity Initial velocity of shuttle on transfer orbit (i.e. after burn) (output)
 * \param finalVelcity Velocity of shuttle at final position (output)
 * \returns true if converged, false if solution could not be found.
 */
bool SolveLambertProblem(const VECTOR3& initialPos, const VECTOR3& finalPos, double transferTime, double orbitPeriod, double mu, VECTOR3& initialVelocity, VECTOR3& finalVelocity);

class LambertBurnTargeting
{
public:
	enum RESULT {RUNNING, CONVERGED, ERR};

	LambertBurnTargeting();
	~LambertBurnTargeting();

	void SetPlanetParameters(double _planetMass, double _planetRadius, double J2Coeff);

	void SetTargetingData(const VECTOR3& _initialPos, const VECTOR3& _finalPos, double _transferTime, double orbitPeriod, double vesselMass);
	void Step();
	RESULT CurrentState() const;
	void GetData(VECTOR3& _initialVelocity, VECTOR3& _finalVelocity) const;
private:
	/**
	 * Runs Lambert solver and updates propagator
	 */
	void PerformTargetingIteration();

	StateVectorPropagator propagator;
	double planetMass;
	// Lambert data
	int iterationCount;
	RESULT currentState;
	int propagatorStepsRemaining;
	VECTOR3 initialPos, finalPos;
	VECTOR3 initialVelocity, finalVelocity;
	VECTOR3 totalMissOffset;
	double transferTime, period, mu;
};

#endif // LAMBERTBURNTARGETING_52AFC39B_152A_415C_9029_F5345F6B6930
