#ifndef LEESYSTEM_H_B30596F2_ADAD_4294_B92E_58D7AD978722
#define LEESYSTEM_H_B30596F2_ADAD_4294_B92E_58D7AD978722
#pragma once

#include "Subsystem.h"

class SSRMS;
typedef SubsystemDirector<SSRMS> SSRMSSubsystemDirector;
typedef Subsystem<SSRMS> SSRMSSubsystem;

const double MAX_GRAPPLING_DIST = 0.5;
// max distance between RMS tip and grappling point for successful grappling
//const double MAX_GRAPPLING_ANGLE = 0.087266;
//const double MAX_GRAPPLING_ANGLE = 0.2;
const double MAX_GRAPPLING_ANGLE_COS_ERR = 0.02; // max angle between EE and grapple for successful grappling (cosine of angle)

/**
 * Simulates LEE.
 * Manages 2 attachment handles; one parent (acting a base LEE) and one child (acting as end effector).
 */
class LEESystem : public SSRMSSubsystem
{
public:
	LEESystem(SSRMSSubsystemDirector* _director, const string& _ident, const std::string& _attachID);
	virtual ~LEESystem();

	/**
	 * Sets attachment controlled by class
	 * @param active bool indicating if this LEE is active (can grapple payload) or passive (grappled to base PDGF)
	 */
	//void SetAttachment(ATTACHMENTHANDLE ah, bool active);
	void SetAttachmentParams(const VECTOR3& pos, const VECTOR3& dir, const VECTOR3& rot) const;

	/**
	 * Called when 'grapple' command is sent to LEE.
	 * /returns handle to vessel grappled; NULL if nothing in range
	 */
	OBJHANDLE Grapple();
	/**
	 * Called when 'ungrapple command is sent to LEE.
	 */
	void Ungrapple();
	bool Grappled() const {return (hPayloadAttachment!=NULL);};
	bool GrappledToBase() const {return (Grappled() && isGrappledToBase);};

	void AttachToBase();
	void DetachFromBase();
	ATTACHMENTHANDLE FindPayload(bool checkBaseAttachment, VESSEL** pVessel=NULL) const;

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* keyword, const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;
protected:
	void CheckForAttachedObjects();
	bool CanAttach(VESSEL* v, ATTACHMENTHANDLE ah, const VECTOR3& glatchpos, const VECTOR3& glatchdir, const VECTOR3& glatchrot) const;

	void AttachPayload(VESSEL* vessel, ATTACHMENTHANDLE attachment);
	void DetachPayload();

	//virtual void OnAttach() = 0;
	//virtual void OnDetach() = 0;

//	ATTACHMENTHANDLE FindPayload(bool checkBaseAttachment, VESSEL** pVessel=NULL) const;
	bool PayloadIsFree() const;
	
	VESSEL* attachedPayload;
	ATTACHMENTHANDLE hPayloadAttachment;
	ATTACHMENTHANDLE hBaseAttach, hEEAttach;
	
	//bool isActive;
	bool isGrappledToBase;

	std::string AttachID;

	bool firstStep;

	// used only for loading attachments from scenario
	std::string payloadName;
	int attachmentIndex;
};

#endif
