#include "LEESystem.h"
#include "SSRMS.h"
#include "UltraMath.h"
#include <cassert>

LEESystem::LEESystem(SSRMSSubsystemDirector* _director, const std::string &_ident, const std::string &_attachID)
: SSRMSSubsystem(_director, _ident), isGrappledToBase(false), firstStep(true)
{
	attachedPayload=NULL;
	hPayloadAttachment=NULL;
	hBaseAttach=NULL;
	hEEAttach=NULL;

	AttachID=_attachID;

	payloadName="";
	attachmentIndex=-1;
}

LEESystem::~LEESystem()
{
}

/*void LatchSubsystem::SetAttachment(ATTACHMENTHANDLE ah, bool active)
{
	if(ah) {
		hAttach = ah;
		isActive = active;
	}
}*/

void LEESystem::SetAttachmentParams(const VECTOR3& pos, const VECTOR3& dir, const VECTOR3& rot) const
{
	V()->SetAttachmentParams(hBaseAttach, pos, dir, rot);
	V()->SetAttachmentParams(hEEAttach, pos, dir, rot);
}

OBJHANDLE LEESystem::Grapple()
{
	VESSEL* pVessel=NULL;

	// check child attach points first
	ATTACHMENTHANDLE hAtt=FindPayload(false, &pVessel);
	if(hAtt) {
		AttachPayload(pVessel, hAtt);
		isGrappledToBase = false;
	}
	else { // check for base PDGF
		hAtt=FindPayload(true, &pVessel);		
		if(hAtt) {
			AttachPayload(pVessel, hAtt);
			isGrappledToBase = true;
			oapiWriteLog("Grappled base PDGF");
		}
	}

	if(pVessel) return pVessel->GetHandle();
	return NULL;
}

void LEESystem::Ungrapple()
{
	DetachPayload();
}

void LEESystem::AttachToBase()
{
	assert(isGrappledToBase);

	attachedPayload->AttachChild(V()->GetHandle(), hPayloadAttachment, hBaseAttach);
}

void LEESystem::DetachFromBase()
{
	assert(isGrappledToBase);

	attachedPayload->DetachChild(hPayloadAttachment);
}

void LEESystem::Realize()
{
	hBaseAttach = V()->CreateAttachment(true, SR_JOINT, _V(0, 0, -1), _V(0, 1, 0), ATTACH_ID);
	hEEAttach = V()->CreateAttachment(false, SR_JOINT, _V(0, 0, -1), _V(0, 1, 0), ATTACH_ID);
}

void LEESystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(firstStep) {
		CheckForAttachedObjects();
		firstStep=false;
	}

	if(!isGrappledToBase) {
		if(attachedPayload!=NULL && !V()->GetAttachmentStatus(hEEAttach)) {
			if(PayloadIsFree()) {
				V()->AttachChild(attachedPayload->GetHandle(), hEEAttach, hPayloadAttachment);
				//double mass=GetSSRMS()->GetEmptyMass()+attachedPayload->GetMass();
				//GetSSRMS()->SetEmptyMass(mass);
			}
		}
		/*else if(!isActive && isGrappledToBase) { // force parent vessel to attach
			attachedPayload->AttachChild(GetSSRMS()->GetHandle(), hPayloadAttachment, hAttach);
		}*/
	}
}

bool LEESystem::OnParseLine(const char* keyword, const char* line)
{
	//if(!_strnicmp(line, label.c_str(), label.length())) {
	if(!_strnicmp(keyword, "ATTACHED_PAYLOAD", 16)) {
		char cbuf[255];
		sscanf(line+1, "%s %d", cbuf, &attachmentIndex); // first char is space, so skip it
		payloadName = cbuf;
		return true;
	}
	else if(!_strnicmp(keyword, "BASE", 4)) {
		isGrappledToBase = true;
		return true;
	}
	return false;
}

void LEESystem::OnSaveState(FILEHANDLE scn) const
{
	SSRMSSubsystem::OnSaveState(scn);

	if(hPayloadAttachment) {
		char pData[55], pIdent[55];
		sprintf_s(pData, 55, "%s %d", attachedPayload->GetName(), attachedPayload->GetAttachmentIndex(hPayloadAttachment));
		//sprintf_s(pIdent, 55, "%s_ATTACHED_PAYLOAD", GetIdentifier().c_str());
		//oapiWriteScenario_string(scn, pIdent, pData);
		oapiWriteScenario_string(scn, "ATTACHED_PAYLOAD", pData);

		if(isGrappledToBase) oapiWriteScenario_string(scn, "BASE", "");
	}
}

void LEESystem::AttachPayload(VESSEL* vessel, ATTACHMENTHANDLE attachment)
{
	//for the moment, assume attachment passed is completely valid
	hPayloadAttachment=attachment;
	attachedPayload=vessel;

	//detached=false;

	//OnAttach();
}

void LEESystem::DetachPayload()
{
	/*if(attachedPayload) {
		// remove mass of released payload
		double mass=GetSSRMS()->GetEmptyMass()-attachedPayload->GetMass();
		GetSSRMS()->SetEmptyMass(mass);
	}*/
	if(!isGrappledToBase) {
		V()->DetachChild(hEEAttach);
	}
	else {
		if(attachedPayload) {
			attachedPayload->DetachChild(hPayloadAttachment);
		}
	}
	hPayloadAttachment=NULL;
	attachedPayload=NULL;

	//OnDetach();
}

ATTACHMENTHANDLE LEESystem::FindPayload(bool checkBaseAttachment, VESSEL** pVessel) const
{
	oapiWriteLog("Looking for payload");
	VECTOR3 gpos, gdir, grms, pos, dir, rot, grmsdir, grmsrot;
	V()->GetAttachmentParams(hEEAttach, pos, dir, rot); // both attach points are at the same position; we can use either one
	V()->Local2Global (pos, grms);  // global position of RMS tip
	V()->GlobalRot(dir, grmsdir); // direction of LEE attachment point in global frame
	V()->GlobalRot(rot, grmsrot);

	// Search the complete vessel list for a grappling candidate.
	// Not very scalable ...
	for (int i = 0; i < oapiGetVesselCount(); i++) {
		OBJHANDLE hV = oapiGetVesselByIndex (i);
		if (hV == V()->GetHandle()) continue; // we don't want to grapple ourselves ...
		oapiGetGlobalPos (hV, &gpos);
		if (dist (gpos, grms) < 10*oapiGetSize (hV)) { // in range
			VESSEL *v = oapiGetVesselInterface (hV);

			int nAttach = v->AttachmentCount (!checkBaseAttachment);
			for (int j = 0; j < nAttach; j++) { // now scan all attachment points of the candidate
				ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (!checkBaseAttachment, j);
				if(CanAttach(v, hAtt, grms, grmsdir, grmsrot)) {
					if(pVessel) *pVessel=v;
					return hAtt;
				}
			}
		}
	}
	if(pVessel) *pVessel=NULL;
	return NULL;
}

bool LEESystem::CanAttach(VESSEL* v, ATTACHMENTHANDLE ah, const VECTOR3& glatchpos, const VECTOR3& glatchdir, const VECTOR3& glatchrot) const
{
	//const char *id = v->GetAttachmentId (ah);
	//oapiWriteLog(const_cast<char*>(id));
	//if (strncmp (id, AttachID.c_str(), AttachID.length())) 
		//return false; // attachment point not compatible

	VECTOR3 pos, dir, rot;
	VECTOR3 gpos, gdir, grot;
	v->GetAttachmentParams (ah, pos, dir, rot);
	v->Local2Global (pos, gpos);
	if (dist (gpos, glatchpos) < MAX_GRAPPLING_DIST) {
		v->GlobalRot(dir, gdir);
		if(Eq(-1.0, dotp(gdir, glatchdir), MAX_GRAPPLING_ANGLE_COS_ERR)) {
			v->GlobalRot(rot, grot);
			if(Eq(1.0, dotp(grot, glatchrot), MAX_GRAPPLING_ANGLE_COS_ERR)) return true;
			else sprintf_s(oapiDebugString(), 255, "Unable to grapple: Rotation error: %f", DEG*acos(dotp(grot, glatchrot)));
		}
		else {
			sprintf_s(oapiDebugString(), 255, "Unable to grapple: Angle error: %f", DEG*acos(dotp(gdir, glatchdir)));
		}
	}
	return false;
}

bool LEESystem::PayloadIsFree() const
{
	if(attachedPayload) {
		//if we are attached to payload, it must be 'free'
		if(V()->GetAttachmentStatus(hEEAttach)) return true;
		//otherwise, loop through all attachment points on payload and check if any of them are in use
		int count=attachedPayload->AttachmentCount(true);
		for(int i=0;i<count;i++) {
			ATTACHMENTHANDLE att=attachedPayload->GetAttachmentHandle(true, i);
			if(attachedPayload->GetAttachmentStatus(att)) return false;
		}
	}
	return true;
}

void LEESystem::CheckForAttachedObjects()
{
	OBJHANDLE hV = NULL;
	if(V()->GetAttachmentStatus(hBaseAttach)) {
		hV = V()->GetAttachmentStatus(hBaseAttach);
		isGrappledToBase = true;
	}
	else if(V()->GetAttachmentStatus(hEEAttach)) {
		hV = V()->GetAttachmentStatus(hEEAttach);
		isGrappledToBase = false;
	}
	if(hV) {
		attachedPayload=oapiGetVesselInterface(hV);
		//double mass=GetSSRMS()->GetEmptyMass()+attachedPayload->GetMass();
		//GetSSRMS()->SetEmptyMass(mass);
		// find handle of attachment point on payload
		for(int i=0;i<attachedPayload->AttachmentCount(!isGrappledToBase);i++) {
			ATTACHMENTHANDLE hAtt=attachedPayload->GetAttachmentHandle(!isGrappledToBase, i);
			if(attachedPayload->GetAttachmentStatus(hAtt)==V()->GetHandle()) {
				hPayloadAttachment=hAtt;
				//OnAttach();
				oapiWriteLog("Found grappled object");
				oapiWriteLog((char*)GetIdentifier().c_str());
				return;
			}
		}
	}
	else { // check data loaded from scenario
		if(attachmentIndex!=-1 && !payloadName.empty()) {
			hV=oapiGetVesselByName((char*)payloadName.c_str());
			if(hV) {
				VESSEL* v=oapiGetVesselInterface(hV);
				ATTACHMENTHANDLE attach=v->GetAttachmentHandle(!isGrappledToBase, attachmentIndex);
				AttachPayload(v, attach);
			}
			else {
				oapiWriteLog("Could not find payload:");
				oapiWriteLog((char*)payloadName.c_str());
			}
		}
	}
}
