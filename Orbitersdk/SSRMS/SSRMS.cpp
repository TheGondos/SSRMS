#define STRICT 1
#define ORBITER_MODULE
#include <cstring>
#include "SSRMS.h"
//#include "OrbiterSoundSDK40.h"
#include "UltraMath.h"
#include <stdio.h>
#include <fstream>
#include "imgui.h"
#include <functional>

MODULEHANDLE hDLL;

// ==============================================================
// Local prototypes
//BOOL CALLBACK SSRMS_DlgProc (HWND, UINT, WPARAM, LPARAM);

void Button(const char *lbl, std::function<void()> cb) {
	const ImVec2 button_sz(ImVec2(30, 20));
	ImGui::Button(lbl, button_sz);
	if(ImGui::IsItemActive()) {
		cb();
	}

}

void DrawSSRMSControls(SSRMS *ssrms) {
//	ImGui::BeginGroupPanel("Rotation", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0));
	static double t0;

	if(t0 == 0.0) {
		t0 =oapiGetSimTime();
		return;
	}

	double t1 = oapiGetSimTime();
	double dt = t1 - t0;
	if(dt == 0.0) return;
	t0 = t1;
	ImGui::BeginGroupPanel("Frame", ImVec2(0, 0));
    ImGui::RadioButton("Shoulder", (int *)&ssrms->RefFrame, (int)ssrms->BASE_FRAME);
	ImGui::SameLine();
    ImGui::RadioButton("EE", (int *)&ssrms->RefFrame, (int)ssrms->EE_FRAME);
	ImGui::EndGroupPanel();
	ImGui::SameLine();
	ImGui::BeginGroupPanel("LEE", ImVec2(0, 0));
	if(ImGui::Button("Switch")) {
		ssrms->ChangeActiveLEE();
	}
	ImGui::SameLine();
	if(ImGui::Button("Light")) {
		// makes more sense to light up the other end of the arm?
		if(!ssrms->LEELight[1-ssrms->activeLEE]->IsActive())
			ssrms->LEELight[1-ssrms->activeLEE]->Activate(true);
		else
			ssrms->LEELight[1-ssrms->activeLEE]->Activate(false);
		ssrms->LightChange=true;
	}
	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("Rotation", ImVec2(0, 0));
		Button("+R", [ssrms, dt](){
			ssrms->DialogInput2.z=1.0;
			ssrms->DiagPushed2=true;
			ssrms->SimDT2=dt;
		});
		ImGui::SameLine();
		Button("-R", [ssrms, dt](){
			ssrms->DialogInput2.z=-1.0;
			ssrms->DiagPushed2=true;
			ssrms->SimDT2=dt;
		});
		Button("+Y", [ssrms, dt](){
			ssrms->DialogInput2.y=1.0;
			ssrms->DiagPushed2=true;
			ssrms->SimDT2=dt;
		});
		ImGui::SameLine();
		Button("-Y", [ssrms, dt](){
			ssrms->DialogInput2.y=-1.0;
			ssrms->DiagPushed2=true;
			ssrms->SimDT2=dt;
		});
		Button("+P", [ssrms, dt](){
			ssrms->DialogInput2.x=1.0;
			ssrms->DiagPushed2=true;
			ssrms->SimDT2=dt;
		});
		ImGui::SameLine();
		Button("-P", [ssrms, dt](){
			ssrms->DialogInput2.x=-1.0;
			ssrms->DiagPushed2=true;
			ssrms->SimDT2=dt;
		});
	ImGui::EndGroupPanel();
	ImGui::SameLine();
	ImGui::BeginGroupPanel("Translation", ImVec2(0, 0));
		Button("+X", [ssrms, dt](){
			ssrms->DialogInput.x=1.0;
			ssrms->DiagPushed=true;
			ssrms->SimDT2=dt;
		});
		ImGui::SameLine();
		Button("-X", [ssrms, dt](){
			ssrms->DialogInput.x=-1.0;
			ssrms->DiagPushed=true;
			ssrms->SimDT2=dt;
		});
		Button("+Y", [ssrms, dt](){
			ssrms->DialogInput.y=1.0;
			ssrms->DiagPushed=true;
			ssrms->SimDT2=dt;
		});
		ImGui::SameLine();
		Button("-Y", [ssrms, dt](){
			ssrms->DialogInput.y=-1.0;
			ssrms->DiagPushed=true;
			ssrms->SimDT2=dt;
		});
		Button("+Z", [ssrms, dt](){
			ssrms->DialogInput.z=1.0;
			ssrms->DiagPushed=true;
			ssrms->SimDT2=dt;
		});
		ImGui::SameLine();
		Button("-Z", [ssrms, dt](){
			ssrms->DialogInput.z=-1.0;
			ssrms->DiagPushed=true;
			ssrms->SimDT2=dt;
		});
	ImGui::EndGroupPanel();
	ImGui::SameLine();
	ImGui::BeginGroupPanel("Wrist", ImVec2(0, 0));
		Button("+R", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->WRIST_ROLL, ssrms->joint_angle[6]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-R", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->WRIST_ROLL, ssrms->joint_angle[6]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});
		Button("+Y", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->WRIST_YAW, ssrms->joint_angle[5]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-Y", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->WRIST_YAW, ssrms->joint_angle[5]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});
		Button("+P", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->WRIST_PITCH, ssrms->joint_angle[4]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-P", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->WRIST_PITCH, ssrms->joint_angle[4]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});
	ImGui::EndGroupPanel();
	ImGui::SameLine();
	ImGui::BeginGroupPanel("Shoulder", ImVec2(0, 0));
		Button("+R", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->SHOULDER_ROLL, ssrms->joint_angle[0]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-R", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->SHOULDER_ROLL, ssrms->joint_angle[0]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});
		Button("+Y", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->SHOULDER_YAW, ssrms->joint_angle[1]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-Y", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->SHOULDER_YAW, ssrms->joint_angle[1]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});
		Button("+P", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->SHOULDER_PITCH, ssrms->joint_angle[2]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-P", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->SHOULDER_PITCH, ssrms->joint_angle[2]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});	ImGui::EndGroupPanel();

	ImGui::BeginGroupPanel("Elbow", ImVec2(0, 0));
		Button("+P", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->ELBOW_PITCH, ssrms->joint_angle[3]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*1.0);
			ssrms->update_vectors=true;
		});
		ImGui::SameLine();
		Button("-P", [ssrms, dt](){
			ssrms->SetJointAngle(ssrms->ELBOW_PITCH, ssrms->joint_angle[3]+dt*JOINT_ROTATION_SPEED*ssrms->SpeedFactor*-1.0);
			ssrms->update_vectors=true;
		});
	ImGui::EndGroupPanel();

	ImGui::SameLine();

	ImGui::BeginGroupPanel("Speed", ImVec2(0, 0));
	if(ImGui::DragInt("##Speed", &ssrms->SpeedFactor, 1.0, 1, 10)) {
		ssrms->SpeedChange=true;
	}
	ImGui::EndGroupPanel();



	ImGui::BeginGroupPanel("Save", ImVec2(0, 0));
		if(ImGui::Button("1")) {
			ssrms->SAVEA();
		}
		ImGui::SameLine();
		if(ImGui::Button("2")) {
			ssrms->SAVEB();
		}
		ImGui::SameLine();
		if(ImGui::Button("3")) {
			ssrms->SAVEC();
		}
		ImGui::SameLine();
		if(ImGui::Button("4")) {
			ssrms->SAVED();
		}
	ImGui::EndGroupPanel();
	ImGui::SameLine();
	ImGui::BeginGroupPanel("Grapple", ImVec2(0, 0));
	if(ssrms->pLEE[ssrms->activeLEE]->Grappled()) {
		if(ImGui::Button("Release")) {
			ssrms->pLEE[ssrms->activeLEE]->Ungrapple();
		}
	} else {
		VESSEL* pVessel=NULL;
		bool CanGrapple=false;
		ATTACHMENTHANDLE hAtt=ssrms->pLEE[ssrms->activeLEE]->FindPayload(false, &pVessel);
		if(hAtt) {
			CanGrapple=true;
		}
		else { // check for base PDGF
			hAtt=ssrms->pLEE[ssrms->activeLEE]->FindPayload(true, &pVessel);		
			if(hAtt) {
				CanGrapple=true;
			}
		}
		if(!CanGrapple) ImGui::BeginDisabled();
		if(ImGui::Button("Grapple")) {
			ssrms->pLEE[ssrms->activeLEE]->Grapple();
		}
		if(!CanGrapple) ImGui::EndDisabled();
	}
	ImGui::SameLine();
	if(ImGui::Button("Toggle grapple points")) {
		if(!ssrms->ShowAttachmentPoints) {
			ssrms->ShowAttachmentPoints=true;
		}else{ 
			ssrms->ShowAttachmentPoints=false;
		}
		oapiSetShowGrapplePoints(ssrms->ShowAttachmentPoints);
	}
	ImGui::EndGroupPanel();
	
	ImGui::BeginGroupPanel("Load", ImVec2(0, 0));
		if(!ssrms->arm1set) ImGui::BeginDisabled();
		if(ImGui::Button("1")) {
			ssrms->LOADA();
		}
		if(!ssrms->arm1set) ImGui::EndDisabled();
		ImGui::SameLine();
		if(!ssrms->arm2set) ImGui::BeginDisabled();
		if(ImGui::Button("2")) {
			ssrms->LOADB();
		}
		if(!ssrms->arm2set) ImGui::EndDisabled();
		ImGui::SameLine();
		if(!ssrms->arm3set) ImGui::BeginDisabled();
		if(ImGui::Button("3")) {
			ssrms->LOADC();
		}
		if(!ssrms->arm3set) ImGui::EndDisabled();
		ImGui::SameLine();
		if(!ssrms->arm4set) ImGui::BeginDisabled();
		if(ImGui::Button("4")) {
			ssrms->LOADD();
		}
		if(!ssrms->arm4set) ImGui::EndDisabled();
	ImGui::EndGroupPanel();
	ImGui::SameLine();
	ImGui::BeginGroupPanel("Joints", ImVec2(0, 0));
		if(ImGui::Button("Sequence"))
			ssrms->ARMSEQ();
		ImGui::SameLine();
		if(!ssrms->center_arm) ImGui::BeginDisabled();
		if(ImGui::Button("HALT"))
			ssrms->StopMotion();
		if(!ssrms->center_arm) ImGui::EndDisabled();
		ImGui::SameLine();
		if(ImGui::Button("Select"))
			ssrms->ARMINPUT();
		ImGui::SameLine();
		if(ImGui::Button("All"))
			ssrms->FULLINPUT();
	ImGui::EndGroupPanel();

}

const std::string DlgSSRMSControls::etype = "DlgSSRMSControls";

DlgSSRMSControls::DlgSSRMSControls(const std::string &name, SSRMS *ssrms) : GUIElement(name, "DlgSSRMSControls") {
    show = false;
	m_ssrms = ssrms;
}

void DlgSSRMSControls::Show() {
	if(show) {
		m_ssrms->dialog_open = true;
		ImGui::Begin(name.c_str(), &show);

		DrawSSRMSControls(m_ssrms);

		ImGui::End();
	} else {
		m_ssrms->dialog_open = false;
	}
}


// --------------------------------------------------------------
// Utility functions
// --------------------------------------------------------------
double ResolveToNearestAngle(double degAngle, double degRef)
{
	double newAngle = degAngle;
	while(newAngle < (degRef-180.0)) newAngle+=360.0;
	while(newAngle > (degRef+180.0)) newAngle-=360.0;
	return newAngle;
}

SSRMS::SSRMS(OBJHANDLE hObj, int fmodel)
:VESSEL3(hObj, fmodel)
{
	SetEmptyMass(100);
	SetSize(10);
	SetPMI(_V(18.0, 18.0, 0.1));
	SetCrossSections(_V(4.7, 4.4, 0.4));
	m_DlgSSRMSControls = std::make_unique<DlgSSRMSControls>("SSRMS Controls", this);

	pSubsystemDirector = new SSRMSSubsystemDirector(this);
	pSubsystemDirector->AddSubsystem(pLEE[0] = new LEESystem(pSubsystemDirector, "LEE1", ATTACH_ID));
	pSubsystemDirector->AddSubsystem(pLEE[1] = new LEESystem(pSubsystemDirector, "LEE2", ATTACH_ID));

	//load mesh
	hSSRMSMesh = oapiLoadMeshGlobal("SSRMSF");
	mesh_ssrms = AddMesh(hSSRMSMesh);
	SetMeshVisibilityMode(mesh_ssrms, MESHVIS_ALWAYS/*|MESHVIS_EXTPASS*/);

	//initialize RMS
	for(int i=0;i<7;i++) {
		joint_angle[i]=0.0;
		joint_motion[i]=0;
	}
	arm_ee_pos = _V(15.90, 0.7, 0.59);
	arm_ee_dir = _V(1, 0, 0);
	arm_ee_rot = _V(0, 1, 0);
	arm_tip[0] = LEE_POS;
	arm_tip[1] = LEE_POS + _V(0, 0, 1);
	arm_tip[2] = LEE_POS + _V(0, 1, 0);
	arm_tip[3] = LEE2_CAM_POS;
	arm_tip[4] = LEE2_LIGHT_POS;
	mesh_center = _V(0, 0, 0);

	// create lights
	const COLOUR4 diff = {0.949f, 0.988f, 1.0f, 0.0f};
	const COLOUR4 amb = {0.0, 0.0, 0};
	const COLOUR4 spec = {0.0f, 0.0f, 0.0f,0};
	LEELight[0] = AddSpotLight(LEE1_LIGHT_POS, _V(0, 0, -1), 20.0, 0.5, 1.25, 0.1, 52.0*RAD, 52.0*1.2*RAD, diff, spec, amb);
	LEELight[1] = AddSpotLight(LEE2_LIGHT_POS, _V(0, 0,  1), 20.0, 0.5, 1.25, 0.1, 52.0*RAD, 52.0*1.2*RAD, diff, spec, amb);
	
	cameraA[0] = _V(0.645, 0.081, -2.948);
	cameraA[1] = cameraA[0] + _V(0, 0, -1);
	cameraA[2] = cameraA[0] + _V(1, 0, 0); 
	cameraB[0] = _V(-0.645, 0.125, 2.953);
	cameraB[1] = cameraB[0] + _V(0, 0, 1);
	cameraB[2] = cameraB[0] + _V(-1, 0, 0);

	camAPan = 0.0;
	camBPan = 0.0;
	camATilt = 0.0;
	camBTilt = 0.0;
	bTiltUp = false;
	bTiltDown = false;
	bPanLeft = false;
	bPanRight = false;
	
	nhCameraLabel = oapiCreateAnnotation(true, 1.0, _V(1.0, 1.0, 1.0));
	oapiAnnotationSetPos(nhCameraLabel, 0.5, 0.05, 0.6, 0.15);
	annotationDisplayTime = 0.0;
	
	foldState.Set(AnimState::OPEN, 1.0);

	update_vectors=true;
	arm_moved=false;
	update_camera = false;

	SpeedFactor=1;
	RefFrame=EE_FRAME;
	activeLEE=1;
	passiveLEE=0;

	cameraView = ACTIVE_LEE;

	AltKybdInput = _V(0, 0, 0);
	DialogInput = _V(0, 0, 0);

	DiagPushed = false;
	DiagPushed2 = false;

	ShowAttachmentPoints=false;

	//SetCameraOffset(arm_tip[2]);
	SetCameraDefaultDirection(arm_tip[1]-arm_tip[0]);

	OrbiterSoundHandle = -1;

	DefineAnimations();
	DefineThrusters();
//
// modifications DLH
//
	bFirstpass = false;
	bDiag = false;
	dialog_open = false;
	SpeedChange = true;
	LightChange = true;

	arm1_Br =arm2_Br =arm3_Br =arm4_Br = arm6_Br =0.0;
	arm1_Bp =arm2_Bp =arm3_Bp =arm4_Bp = arm6_Bp =0.0;
	arm1_By =arm2_By =arm3_By =arm4_By = arm6_By =0.0;
	arm1_ep =arm2_ep =arm3_ep =arm4_ep = arm6_ep =0.0;
	arm1_Ar =arm2_Ar =arm3_Ar =arm4_Ar = arm6_Ar =0.0;
	arm1_Ap =arm2_Ap =arm3_Ap =arm4_Ap = arm6_Ap =0.0;
	arm1_Ay =arm2_Ay =arm3_Ay =arm4_Ay = arm6_Ay =0.0;

	arm1set = arm2set = arm3set = arm4set = false;
	center_arm = false;
	Seqindex = 0;


}

SSRMS::~SSRMS()
{
	oapiCloseDialog(m_DlgSSRMSControls.get());
	delete pSubsystemDirector;
}

void SSRMS::DefineAnimations()
{
	ANIMATIONCOMPONENT_HANDLE parent;

	static UINT ShoulderRollGrp[1] = {21};
	static MGROUP_ROTATE sr_anim (mesh_ssrms, ShoulderRollGrp, 1,
		SR_JOINT, _V(0, 0, -1), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][SHOULDER_ROLL] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][SHOULDER_ROLL], 0, 1, &sr_anim);

	static UINT ShoulderYawGrp[1] = {22};
	static MGROUP_ROTATE sy_anim (mesh_ssrms, ShoulderYawGrp, 1,
		SY_JOINT, _V(0, 1, 0), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][SHOULDER_YAW] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][SHOULDER_YAW], 0, 1, &sy_anim, parent);

	static UINT ShoulderPitchGrp[5] = {23, 24, 25, 29, 30};
	static MGROUP_ROTATE sp_anim (mesh_ssrms, ShoulderPitchGrp, 5,
		SP_JOINT, _V(-1, 0, 0), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][SHOULDER_PITCH] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][SHOULDER_PITCH], 0, 1, &sp_anim, parent);

	static UINT ShoulderFoldGrp[3] = {28,31,32};
	static MGROUP_ROTATE shoulder_fold_anim(mesh_ssrms, ShoulderFoldGrp, 3, _V(0, 0.245, -3.65), _V(1, 0, 0), static_cast<float>(180.0*RAD));
	anim_fold = CreateAnimation(1.0);
	ANIMATIONCOMPONENT_HANDLE parent_cam = AddAnimationComponent(anim_fold, 0, 1, &shoulder_fold_anim, parent);

	static UINT BoomACamPitchGrp[1] = {26};
	static MGROUP_ROTATE boomA_cam_pitch_anim(mesh_ssrms, BoomACamPitchGrp, 1, _V(0.705, -0.024, -2.835), _V(1, 0, 0), static_cast<float>((CAM_PAN_MAX-CAM_PAN_MIN)*RAD));
	anim_CamAPan = CreateAnimation(0.5);
	parent_cam = AddAnimationComponent(anim_CamAPan, 0, 1, &boomA_cam_pitch_anim, parent_cam);	
	
	static UINT BoomACamYawGrp[1] = {27};
	static MGROUP_ROTATE boomA_cam_yaw_anim(mesh_ssrms, BoomACamYawGrp, 1, _V(0.677, 0.027, -2.835), _V(0, -1, 0), static_cast<float>((CAM_TILT_MAX-CAM_TILT_MIN)*RAD));
	anim_CamATilt = CreateAnimation(0.5);
	parent_cam = AddAnimationComponent(anim_CamATilt, 0, 1, &boomA_cam_yaw_anim, parent_cam);
	
	static MGROUP_ROTATE boomA_vec_anim(LOCALVERTEXLIST, MAKEGROUPARRAY(cameraA), 3, _V(0.705, -0.024, -2.835), _V(0, 0, -1), 0.0);
	UINT anim_temp = CreateAnimation(0.5);
	AddAnimationComponent(anim_temp, 0, 1, &boomA_vec_anim, parent_cam);

	static UINT ElbowPitchGrp[4] = {34, 39, 40, 41};
	static MGROUP_ROTATE ep_anim(mesh_ssrms, ElbowPitchGrp, 4,
		EP_JOINT, _V(-1, 0, 0), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][ELBOW_PITCH] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][ELBOW_PITCH], 0, 1, &ep_anim, parent);

	static UINT WristFoldGrp[3] = {33,35,38};
	static MGROUP_ROTATE wrist_fold_anim(mesh_ssrms, WristFoldGrp, 3, _V(0, -0.245, 3.65), _V(1, 0, 0), static_cast<float>(180.0*RAD));
	parent_cam = AddAnimationComponent(anim_fold, 0, 1, &wrist_fold_anim, parent);
	
	static UINT BoomBCamPitchGrp[1] = {36};
	static MGROUP_ROTATE boomB_cam_pitch_anim(mesh_ssrms, BoomBCamPitchGrp, 1, _V(-0.709, 0.02, 2.84), _V(-1, 0, 0), static_cast<float>((CAM_PAN_MAX-CAM_PAN_MIN)*RAD));
	anim_CamBPan = CreateAnimation(0.5);
	parent_cam = AddAnimationComponent(anim_CamBPan, 0, 1, &boomB_cam_pitch_anim, parent_cam);
	
	static UINT BoomBCamYawGrp[1] = {37};
	static MGROUP_ROTATE boomB_cam_yaw_anim(mesh_ssrms, BoomBCamYawGrp, 1, _V(-0.681, 0.071, 2.84), _V(0, -1, 0), static_cast<float>((CAM_TILT_MAX-CAM_TILT_MIN)*RAD));
	anim_CamBTilt = CreateAnimation(0.5);
	parent_cam = AddAnimationComponent(anim_CamBTilt, 0, 1, &boomB_cam_yaw_anim, parent_cam);
	
	static MGROUP_ROTATE boomB_vec_anim(LOCALVERTEXLIST, MAKEGROUPARRAY(cameraB), 3, _V(-0.709, 0.02, 2.84), _V(0, 0, -1), 0.0);
	anim_temp = CreateAnimation(0.5);
	AddAnimationComponent(anim_temp, 0, 1, &boomB_vec_anim, parent_cam);

	static UINT WristPitchGrp[1] = {42};
	static MGROUP_ROTATE wp_anim (mesh_ssrms, WristPitchGrp, 1,
		WP_JOINT, _V(-1, 0, 0), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][WRIST_PITCH] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][WRIST_PITCH], 0, 1, &wp_anim, parent);

	static UINT WristYawGrp[1] = {43};
	static MGROUP_ROTATE wy_anim(mesh_ssrms, WristYawGrp, 1,
		WY_JOINT, _V(0, 1, 0), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][WRIST_YAW] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][WRIST_YAW], 0, 1, &wy_anim, parent);

	static UINT WristRollGrp[21] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};
	static MGROUP_ROTATE wr_anim(mesh_ssrms, WristRollGrp, 21,
		LEE_POS, _V(0, 0, -1), static_cast<float>((JOINT_LIMITS[1]-JOINT_LIMITS[0])*RAD));
	anim_joint[1][WRIST_ROLL] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][WRIST_ROLL], 0, 1, &wr_anim, parent);

	static MGROUP_ROTATE lee_anim(LOCALVERTEXLIST, MAKEGROUPARRAY(arm_tip), 5,
		LEE_POS, _V(0, 0, -1), 0.0);
	anim_lee = CreateAnimation(0.5);
	AddAnimationComponent(anim_lee, 0, 1, &lee_anim, parent);

	for(int i=0;i<7;i++) anim_joint[0][i]=anim_joint[1][6-i];
}

void SSRMS::DefineThrusters()
{
	ph_null = CreatePropellantResource(1.0, 1.0);

	for(int i=0;i<12;i++) rms_control[i] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);

	CreateThrusterGroup(rms_control, 1, THGROUP_ATT_UP);
	CreateThrusterGroup(rms_control+1, 1, THGROUP_ATT_DOWN);
	CreateThrusterGroup(rms_control+2, 1, THGROUP_ATT_LEFT);
	CreateThrusterGroup(rms_control+3, 1, THGROUP_ATT_RIGHT);
	CreateThrusterGroup(rms_control+4, 1, THGROUP_ATT_FORWARD);
	CreateThrusterGroup(rms_control+5, 1, THGROUP_ATT_BACK);
	CreateThrusterGroup(rms_control+6, 1, THGROUP_ATT_PITCHUP);
	CreateThrusterGroup(rms_control+7, 1, THGROUP_ATT_PITCHDOWN);
	CreateThrusterGroup(rms_control+8, 1, THGROUP_ATT_YAWLEFT);
	CreateThrusterGroup(rms_control+9, 1, THGROUP_ATT_YAWRIGHT);
	CreateThrusterGroup(rms_control+10, 1, THGROUP_ATT_BANKLEFT);
	CreateThrusterGroup(rms_control+11, 1, THGROUP_ATT_BANKRIGHT);
}

bool SSRMS::MoveEE(const VECTOR3 &newPos, const VECTOR3 &newDir, const VECTOR3 &newRot, double DeltaT)
{
	double new_joint_angles[7]; // angles in degrees

	const VECTOR3 IK_REFERENCE = _V(SY_JOINT.x, SP_JOINT.y, SY_JOINT.z);
	const VECTOR3 LEE_OFFSET_VECTOR = _V(SR_JOINT.z-IK_REFERENCE.z, (SR_JOINT.x-IK_REFERENCE.x), -(SR_JOINT.y-IK_REFERENCE.y));
	// pretend shoulder roll is set to 0
	// newPos argument is position vector with base LEE at origin
	// pos vector uses IK origin (lower section of SY joint)
	VECTOR3 pos = RotateVectorX(newPos, joint_angle[SHOULDER_ROLL])+LEE_OFFSET_VECTOR;
	//sprintf_s(oapiDebugString(), 255, "IK Pos: %f %f %f", pos.x, pos.y, pos.z);
	// correct for shoulder roll
	VECTOR3 dir = RotateVectorX(newDir, joint_angle[SHOULDER_ROLL]);
	VECTOR3 rot = RotateVectorX(newRot, joint_angle[SHOULDER_ROLL]);

	VECTOR3 arm_wy_pos = pos-dir*WY_EE_DIST;

	//sprintf_s(oapiDebugString(), 255, "arm_wy_pos: %f %f %f", arm_wy_pos.x, arm_wy_pos.y, arm_wy_pos.z);

	// unlike the SRMS, where all the joints are in the same plane, the SSRMS has a horizontal offset between the pitch joints
	// treat this offset as a shoulder yaw bias, and calculate the corresponding angle
	VECTOR3 corrected_wy_pos = _V(arm_wy_pos.x, arm_wy_pos.y, 0.0);
	double c_length = length(corrected_wy_pos);
	double offset_angle = asin(LEE_OFFSET/c_length);
	//if(arm_wy_pos.x<0) offset_angle = -offset_angle;

	double sy_angle_r = atan2(arm_wy_pos.y, arm_wy_pos.x)+offset_angle;
	double sy_angle_noError = ResolveToNearestAngle(sy_angle_r*DEG, joint_angle[SHOULDER_YAW]);
	double sy_angle_180Error = ResolveToNearestAngle((sy_angle_r + RAD*180.0 - 2*offset_angle)*DEG, joint_angle[SHOULDER_YAW]);
	bool yaw180Error = false;
	if(abs(joint_angle[SHOULDER_YAW]-sy_angle_noError) > abs(joint_angle[SHOULDER_YAW]-sy_angle_180Error)) {
		//sprintf_s(oapiDebugString(), 255, "MoveEE: 180-degree yaw error: %f %f", abs(joint_angle[SHOULDER_YAW]-sy_angle_noError), offset_angle*DEG);
		yaw180Error = true;
		sy_angle_r = sy_angle_r + RAD*180.0 - 2*offset_angle;
		new_joint_angles[SHOULDER_YAW] = sy_angle_180Error;

		//arm_wy_pos = -arm_wy_pos;
		//dir = -dir;
		//rot = -rot;
	}
	else {
		new_joint_angles[SHOULDER_YAW] = sy_angle_noError;
		//sprintf_s(oapiDebugString(), 255, "MoveEE: no yaw error %f %f", abs(joint_angle[SHOULDER_YAW]-sy_angle_noError), offset_angle*DEG);
	}
	// calculate vector parallel to horizontal offset between booms
	// offset_vector is perpendicular to boom plane
	VECTOR3 offset_vector = RotateVectorZ(_V(0, 1, 0), sy_angle_r*DEG);
	// wp_dir vector is vector in boom plane parallel to WY joint rotation axis
	// if WR is 0, wp_dir is parallel to rot vector
	VECTOR3 wp_dir = crossp(dir, offset_vector);
	wp_dir/=length(wp_dir); // normalize vector
	// calculate WP joint position after compensating for horizontal and vertical joint offsets
	// we can now treat SSRMS as a single boom with multiple joints (like SRMS)
	VECTOR3 offset_wp_pos = arm_wy_pos+wp_dir*WP_WY_DIST+offset_vector*LEE_OFFSET;
	/*if(yaw180Error) {
		offset_wp_pos = -offset_wp_pos;
		wp_dir = -wp_dir;
		dir = -dir;
	}*/
	//sprintf_s(oapiDebugString(), 255, "Offset Pos: %f %f %f", offset_wp_pos.x, offset_wp_pos.y, offset_wp_pos.z);
	double r = length(offset_wp_pos);

	//sprintf_s(oapiDebugString(), 255, "%s offset_pos: %f %f %f", oapiDebugString(), offset_wp_pos.x, offset_wp_pos.y, offset_wp_pos.z);

	//sprintf_s(oapiDebugString(), 255, "offset_angle: %f pos_angle: %f", offset_angle*DEG, atan2(arm_wy_pos.y, arm_wy_pos.x)*DEG);

	//sprintf_s(oapiDebugString(), 255, "offset_vector: %f %f %f newDir: %f %f %f",
		//offset_vector.x, offset_vector.y, offset_vector.z, newDir.x, newDir.y, newDir.z);
	//sprintf_s(oapiDebugString(), 255, "%s offset_vector: %f %f %f wp_dir: %f %f %f", oapiDebugString(),
		//offset_vector.x, offset_vector.y, offset_vector.z, wp_dir.x, wp_dir.y, wp_dir.z);
	//sprintf_s(oapiDebugString(), 255, "wp_dir: %f %f %f newDir: %f %f %f",
		//wp_dir.x, wp_dir.y, wp_dir.z, newDir.x, newDir.y, newDir.z);

	// use cosine law to calculate SP and EP angles
	double rho = sqrt(offset_wp_pos.x*offset_wp_pos.x+offset_wp_pos.y*offset_wp_pos.y);
	double cos_phibar_e=(r*r-SP_EP_DIST*SP_EP_DIST-EP_WP_DIST*EP_WP_DIST)/(-2*SP_EP_DIST*EP_WP_DIST);
	if (fabs(cos_phibar_e)>1) {
		sprintf_s(oapiDebugString(), 255, "MoveEE: Can't reach with elbow");
		return false;//Can't reach new point with the elbow
	}
	new_joint_angles[ELBOW_PITCH]=DEG*acos(cos_phibar_e)-180.0;
	double cos_phi_s2=(EP_WP_DIST*EP_WP_DIST-SP_EP_DIST*SP_EP_DIST-r*r)/(-2*SP_EP_DIST*r);
	if(fabs(cos_phi_s2)>1) {
		sprintf_s(oapiDebugString(), 255, "MoveEE: Can't reach with shoulder");
		return false; //Can't reach with shoulder
	}
	new_joint_angles[SHOULDER_PITCH]=DEG*(-atan2(offset_wp_pos.z,rho)+acos(cos_phi_s2));
	// 2 possible solutions for pitch angles; pick one closest to current state
	double curElbowAngle = ResolveToNearestAngle(joint_angle[ELBOW_PITCH], 0.0); // in case actual elbow angle exceeds +/- 180 degrees
	if(!Eq(sign(new_joint_angles[ELBOW_PITCH]), sign(curElbowAngle), 0.01)) {
		//sprintf_s(oapiDebugString(), 255, "MoveEE: reversing elbow sign");
		new_joint_angles[ELBOW_PITCH] = -new_joint_angles[ELBOW_PITCH];
		new_joint_angles[SHOULDER_PITCH]=DEG*(-atan2(offset_wp_pos.z,rho)-acos(cos_phi_s2));
		//sp_angle_d = -sp_angle_d;
	}
	if(yaw180Error) {
		new_joint_angles[SHOULDER_PITCH] += 180.0+2.0*DEG*atan2(offset_wp_pos.z,rho);
		//sp_angle_d += 180.0+acos(cos_phi_s2);
		//sp_angle_d = 180.0 - sp_angle_d - 2.0*ep_angle_d;
	}
	//double sp_angle_d=DEG*(acos(cos_phi_s2));

	double phi=-DEG*acos(wp_dir.z);
	//double phi=DEG*acos(wp_dir.z);
	//double phi = DEG*atan2(sqrt(wp_dir.x*wp_dir.x + wp_dir.y*wp_dir.y), wp_dir.z);
	//if((wp_dir.x>0.0 && corrected_wy_pos.x>=0.0) || (wp_dir.x<0.0 && corrected_wy_pos.x<0.0)) phi=-phi;
	if(wp_dir.x<0.0) phi=-phi;
	if(offset_vector.y>0.0) phi=-phi;
	new_joint_angles[WRIST_PITCH]=phi-new_joint_angles[SHOULDER_PITCH]-new_joint_angles[ELBOW_PITCH];

	//double wy_angle_d;
	/*if((offset_vector.z>0.0 && newDir.z<0.0) || (offset_vector.z<0.0 && newDir.z>0.0)) 
		wy_angle_d=90.0+DEG*acos(dotp(offset_vector, newDir));
	else*/
	new_joint_angles[WRIST_YAW]=90.0-DEG*acos(dotp(offset_vector, dir));
	//new_joint_angles[WRIST_ROLL] = DEG*acos(dotp(wp_dir, rot));
	//if(dotp(wp_dir, crossp(rot, dir)) < 0) new_joint_angles[WRIST_ROLL] = -new_joint_angles[WRIST_ROLL];
	new_joint_angles[WRIST_ROLL] = DEG*SignedAngle(wp_dir, rot, dir);

	//if(yaw180Error) sp_angle_d+=180.0;

	// increment angles by 360 degrees to get them as close as possible to existing values
	// this was already done for SY joint
	for(int joint=SHOULDER_PITCH;joint<=WRIST_ROLL;joint++) new_joint_angles[joint] = ResolveToNearestAngle(new_joint_angles[joint], joint_angle[joint]);

	// make sure angles are within limits
	for(int joint=SHOULDER_YAW;joint<=WRIST_ROLL;joint++) {
		if(new_joint_angles[joint]<JOINT_SOFTSTOPS[0] || new_joint_angles[joint]>JOINT_SOFTSTOPS[1]) {
			sprintf_s(oapiDebugString(), 255, "Error: joint %d reached angle limit %f", joint, new_joint_angles[joint]);
			return false;
		}
		/*double speed = abs(new_joint_angles[joint]-joint_angle[joint])/DeltaT;
		if(speed > JOINT_SPEED_LIMIT*SpeedFactor) { // it probably isn't realistic to scale by speed factor here
			sprintf_s(oapiDebugString(), 255, "Error: joint %d reached speed limit %f", joint, speed);
			return false;
		}*/
	}
	
	for(int joint=SHOULDER_YAW;joint<=WRIST_ROLL;joint++) SetJointAngle(static_cast<SSRMS_JOINT>(joint), new_joint_angles[joint]); // leave roll 

	arm_ee_pos = newPos;
	arm_ee_dir = newDir;
	arm_ee_rot = newRot;
	
	return true;
}

bool SSRMS::SetJointAngle(SSRMS::SSRMS_JOINT joint, double angle)
{
	if(angle>=JOINT_SOFTSTOPS[0] && angle<=JOINT_SOFTSTOPS[1]) {
		double pos=linterp(JOINT_LIMITS[0], 0.0, JOINT_LIMITS[1], 1.0, angle);
		if(activeLEE == 0 && (joint == SHOULDER_YAW || joint == WRIST_YAW)) pos = 1.0-pos; // yaw joints rotate in different directions depending on which LEE is active
		SetAnimation(anim_joint[activeLEE][joint], pos);
		joint_angle[joint]=angle;
		arm_moved=true;
		update_camera = true;
		return true;
	}
	else return false;
}

bool SSRMS::ChangeActiveLEE()
{
	if(!pLEE[0]->GrappledToBase() || !pLEE[1]->GrappledToBase()) return false;

	if(pLEE[passiveLEE]->GrappledToBase()) pLEE[passiveLEE]->DetachFromBase();
	if(pLEE[activeLEE]->GrappledToBase()) pLEE[activeLEE]->AttachToBase();
	
	// change light settings
	if(LEELight[activeLEE]->IsActive()) {
		LEELight[activeLEE]->Activate(false);
		LEELight[passiveLEE]->Activate(true);
	}

	activeLEE=passiveLEE;
	passiveLEE=1-activeLEE;

	// swap joint angles
	for(unsigned short i=0;i<ELBOW_PITCH;i++) {
		double temp=joint_angle[i];
		joint_angle[i]=joint_angle[6-i];
		joint_angle[6-i]=temp;
	}
	// yaw joints rotate in different directions depending on which LEE is active
	joint_angle[SHOULDER_YAW] = -joint_angle[SHOULDER_YAW];
	joint_angle[WRIST_YAW] = -joint_angle[WRIST_YAW];
	CalculateVectors();

	UpdateMeshPosition();

	update_camera = true;

	return true;
}

void SSRMS::UpdateMeshPosition()
{
	// shift meshes to align active LEE with centre of external cam view
	if(activeLEE == 0) {
		ShiftMeshes(mesh_center-SR_JOINT);
		mesh_center = SR_JOINT;
	}
	else {
		ShiftMeshes(mesh_center-arm_tip[0]);
		mesh_center = arm_tip[0];
	}
	// update attachment points
	VECTOR3 ofs;
	GetMeshOffset(mesh_ssrms, ofs);
	pLEE[1]->SetAttachmentParams(arm_tip[0]+ofs, arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);
	pLEE[0]->SetAttachmentParams(SR_JOINT+ofs, _V(0, 0, -1), _V(0, 1, 0));
	// update light positions/direction
	LEELight[0]->SetPosition(LEE1_LIGHT_POS+ofs);
	LEELight[1]->SetPosition(arm_tip[4]+ofs);
	LEELight[1]->SetDirection(arm_tip[1]-arm_tip[0]);
}

void SSRMS::UpdateCameraView()
{
	if(oapiCameraInternal()) {
		VECTOR3 ofs;
		GetMeshOffset(mesh_ssrms, ofs);

		switch(cameraView) {
		case ACTIVE_LEE:
			if(activeLEE == 1) {
				// calculate rotation angle for EE cam
				VECTOR3 dir = arm_tip[1]-arm_tip[0];
				double angle = CalculateCameraRotationAngle(dir, arm_tip[2]-arm_tip[0]);

				SetCameraOffset(arm_tip[3]+ofs);
				SetCameraDefaultDirection(dir, angle);
			}
			else {
				SetCameraOffset(LEE1_CAM_POS+ofs);
				SetCameraDefaultDirection(_V(0, 0, -1), 0.0);
			}
			break;
		case BOOM_A:
		{
			VECTOR3 dir = cameraA[1]-cameraA[0];
			double angle = CalculateCameraRotationAngle(dir, cameraA[2]-cameraA[0]);
			SetCameraOffset(cameraA[0]+ofs);
			SetCameraDefaultDirection(dir, angle);
			break;
		}
		case BOOM_B:
		{
			VECTOR3 dir = cameraB[1]-cameraB[0];
			double angle = CalculateCameraRotationAngle(dir, cameraB[2]-cameraB[0]);
			SetCameraOffset(cameraB[0]+ofs);
			SetCameraDefaultDirection(dir, angle);
			break;
		}
		}
		
		oapiCameraSetCockpitDir(0.0, 0.0);
		SetCameraRotationRange(0, 0, 0, 0);
		update_camera = false;
	}
}

void SSRMS::UpdateCameraAnimations()
{
	SetAnimation(anim_CamAPan, (camAPan-CAM_PAN_MIN)/(CAM_PAN_MAX-CAM_PAN_MIN));
	SetAnimation(anim_CamATilt, (camATilt-CAM_TILT_MIN)/(CAM_TILT_MAX-CAM_TILT_MIN));
	SetAnimation(anim_CamBPan, (camBPan-CAM_PAN_MIN)/(CAM_PAN_MAX-CAM_PAN_MIN));
	SetAnimation(anim_CamBTilt, (camBTilt-CAM_TILT_MIN)/(CAM_TILT_MAX-CAM_TILT_MIN));
}

void SSRMS::CalculateVectors()
{
	// work forward through joints and update vectors
	arm_ee_dir = _V(1, 0, 0);
	arm_ee_rot = RotateVectorX(_V(0, 0, 1), -joint_angle[SHOULDER_ROLL]); // should probably be -joint_angle[SR]
	//arm_ee_rot = _V(0, 0, 1);
	arm_ee_pos = _V(SR_SY_DIST, 0, 0) + arm_ee_rot*SY_SP_VERT_DIST;
	// handle SY joint; get new direction, than translate EE pos to compensate for horizontal offset between booms
	arm_ee_dir = RotateVector(arm_ee_rot, RAD*joint_angle[SHOULDER_YAW], arm_ee_dir);
	VECTOR3 rot_cross_dir = crossp(arm_ee_rot, arm_ee_dir); // pitch joints rotate around this vector
	arm_ee_pos -= rot_cross_dir*LEE_OFFSET;
	// handle 3 pitch joints
	arm_ee_dir = RotateVector(rot_cross_dir, RAD*joint_angle[SHOULDER_PITCH], arm_ee_dir);
	arm_ee_pos += arm_ee_dir*SP_EP_DIST;
	arm_ee_dir = RotateVector(rot_cross_dir, RAD*joint_angle[ELBOW_PITCH], arm_ee_dir);
	arm_ee_pos += arm_ee_dir*EP_WP_DIST;
	arm_ee_dir = RotateVector(rot_cross_dir, RAD*joint_angle[WRIST_PITCH], arm_ee_dir);
	arm_ee_rot = RotateVector(rot_cross_dir, RAD*(joint_angle[SHOULDER_PITCH]+joint_angle[ELBOW_PITCH]+joint_angle[WRIST_PITCH]), arm_ee_rot);
	arm_ee_pos -= arm_ee_rot*WP_WY_DIST;
	// wrist yaw
	arm_ee_dir = RotateVector(arm_ee_rot, RAD*joint_angle[WRIST_YAW], arm_ee_dir);
	arm_ee_pos += arm_ee_dir*WY_EE_DIST;
	// wrist roll
	arm_ee_rot = RotateVector(arm_ee_dir, RAD*joint_angle[WRIST_ROLL], arm_ee_rot);

	VECTOR3 old_arm_ee_pos=arm_tip[0]-SR_JOINT;
	old_arm_ee_pos=_V(old_arm_ee_pos.z, old_arm_ee_pos.x, -old_arm_ee_pos.y);
	/*sprintf_s(oapiDebugString(), 255, "FK pos: %f %f %f Orbiter pos: %f %f %f arm_tip: %f %f %f",
		arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z,
		old_arm_ee_pos.x, old_arm_ee_pos.y, old_arm_ee_pos.z,
		arm_tip[0].x, arm_tip[0].y, arm_tip[0].z);*/
	/*sprintf_s(oapiDebugString(), 255, "Pos: %f %f %f Dir: %f %f %f Rot: %f %f %f",
		arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z,
		arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z,
		arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);*/
}

void SSRMS::ShowCameraViewLabel()
{
	switch(cameraView) {
	case ACTIVE_LEE:
		oapiAnnotationSetText(nhCameraLabel, "LEE camera");
		break;
	case BOOM_A:
		oapiAnnotationSetText(nhCameraLabel, "Boom A camera");
		break;
	case BOOM_B:
		oapiAnnotationSetText(nhCameraLabel, "Boom B camera");
		break;
	}
	annotationDisplayTime = 5.0;
}

void SSRMS::clbkLoadStateEx(FILEHANDLE scn, void *vs)
{
	char *line;
	bool bLightOn = false;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "ARM_STATUS", 10)) {
			sscanf(line+10, "%lf%lf%lf%lf%lf%lf%lf", &joint_angle[SHOULDER_ROLL], &joint_angle[SHOULDER_YAW], &joint_angle[SHOULDER_PITCH],
				&joint_angle[ELBOW_PITCH], &joint_angle[WRIST_PITCH], &joint_angle[WRIST_YAW], &joint_angle[WRIST_ROLL]);
			arm_moved=true;
			update_vectors=true;
			update_camera = true;
		}
		else if(!_strnicmp(line, "ARM_SET1", 8)) {
			sscanf(line+8, "%lf%lf%lf%lf%lf%lf%lf", &arm1_Br, &arm1_By, &arm1_Bp,
				&arm1_ep, &arm1_Ap, &arm1_Ay, &arm1_Ar);
			arm1set=true;
		}
		else if(!_strnicmp(line, "ARM_SET2", 8)) {
			sscanf(line+8, "%lf%lf%lf%lf%lf%lf%lf", &arm2_Br, &arm2_By, &arm2_Bp,
				&arm2_ep, &arm2_Ap, &arm2_Ay, &arm2_Ar);
			arm2set=true;
		}
		else if(!_strnicmp(line, "ARM_SET3", 8)) {
			sscanf(line+8, "%lf%lf%lf%lf%lf%lf%lf", &arm3_Br, &arm3_By, &arm3_Bp,
				&arm3_ep, &arm3_Ap, &arm3_Ay, &arm3_Ar);
			arm3set=true;
		}
		else if(!_strnicmp(line, "ARM_SET4", 8)) {
			sscanf(line+8, "%lf%lf%lf%lf%lf%lf%lf", &arm4_Br, &arm4_By, &arm4_Bp,
				&arm4_ep, &arm4_Ap, &arm4_Ay, &arm4_Ar);
			arm4set=true;
		}
		else if(!_strnicmp(line, "ACTIVE_LEE", 10)) {
			sscanf(line+10, "%d", &activeLEE);
			passiveLEE=1-activeLEE;
		}
		else if(!_strnicmp(line, "FOLDED", 6)) {
			sscan_state(line+6, foldState);
			SetAnimation(anim_fold, foldState.pos);
		}
		else if(!_strnicmp(line, "LIGHT", 5)) {
			bLightOn = true;
		}
		else if(!_strnicmp(line, "ACTIVE_CAMERA", 13)) {
			int temp;
			sscanf(line+13, "%d", &temp);
			cameraView = static_cast<CAMERA_VIEW>(temp);
			update_camera = true;
		}
		else if(!_strnicmp(line, "CAM_STATUS", 10)) {
			sscanf(line+10, "%lf%lf%lf%lf", &camAPan, &camATilt, &camBPan, &camBTilt);
			update_camera = true;
		}
		else if (!_strnicmp (line, "DIALOG",6)) {
			bDiag = true;
		}
		else if(!pSubsystemDirector->ParseScenarioLine(scn, line)) {
			ParseScenarioLineEx(line, vs);
		}
	}
	// do this at end of scenario loading, once we know which LEE is active
	LEELight[activeLEE]->Activate(bLightOn);
	LEELight[passiveLEE]->Activate(false);

	pSubsystemDirector->RealizeAll();// must be here so attachments are created in time or else CTD
}

void SSRMS::clbkSaveState(FILEHANDLE scn)
{
	VESSEL3::clbkSaveState(scn);

	char cbuf[255];
	sprintf(cbuf, "%f %f %f %f %f %f %f", joint_angle[SHOULDER_ROLL], joint_angle[SHOULDER_YAW], joint_angle[SHOULDER_PITCH],
				joint_angle[ELBOW_PITCH], joint_angle[WRIST_PITCH], joint_angle[WRIST_YAW], joint_angle[WRIST_ROLL]);
	oapiWriteScenario_string(scn, "ARM_STATUS", cbuf);
	if (arm1set){
			sprintf(cbuf, "%f %f %f %f %f %f %f", arm1_Br, arm1_By, arm1_Bp,
				arm1_ep, arm1_Ap, arm1_Ay, arm1_Ar);
	oapiWriteScenario_string(scn, "ARM_SET1", cbuf);
	}
	if (arm2set){
			sprintf(cbuf, "%f %f %f %f %f %f %f", arm2_Br, arm2_By, arm2_Bp,
				arm2_ep, arm2_Ap, arm2_Ay, arm2_Ar);
	oapiWriteScenario_string(scn, "ARM_SET2", cbuf);
	}
	if (arm3set){
			sprintf(cbuf, "%f %f %f %f %f %f %f", arm3_Br, arm3_By, arm3_Bp,
				arm3_ep, arm3_Ap, arm3_Ay, arm3_Ar);
	oapiWriteScenario_string(scn, "ARM_SET3", cbuf);
	}
	if (arm4set){
			sprintf(cbuf, "%f %f %f %f %f %f %f", arm4_Br, arm4_By, arm4_Bp,
				arm4_ep, arm4_Ap, arm4_Ay, arm4_Ar);
	oapiWriteScenario_string(scn, "ARM_SET4", cbuf);
	}
	oapiWriteScenario_int(scn, "ACTIVE_LEE", activeLEE);
	WriteScenario_state(scn, "FOLDED", foldState);
	if(LEELight[activeLEE]->IsActive()) oapiWriteLine(scn, "  LIGHT");
		strcpy (cbuf, "");
	if (dialog_open) oapiWriteLine(scn, "  DIALOG");
	oapiWriteScenario_int(scn, "ACTIVE_CAMERA", static_cast<int>(cameraView));
	sprintf(cbuf, "%f %f %f %f", camAPan, camATilt, camBPan, camBTilt);
	oapiWriteScenario_string(scn, "CAM_STATUS", cbuf);
	pSubsystemDirector->SaveState(scn);
}

void SSRMS::clbkPreStep(double SimT, double SimDT, double mjd)
{
	pSubsystemDirector->PreStep(SimT, SimDT, mjd);

	// disable all Orbitersim autopilots
	for(int i=NAVMODE_KILLROT;i<=NAVMODE_HOLDALT;i++) DeactivateNavmode(i);
	
	if(foldState.Moving()) {
		foldState.Move(SimDT*SSRMS_UNFOLD_SPEED);
		SetAnimation(anim_fold, foldState.pos);
	}
	
	// if arm is still folded, arm is not allowed to move
	if(!foldState.Open()) return;
	
	// pan/tilt cameras
	if(bPanLeft || bPanRight || bTiltDown || bTiltUp) {
		if(cameraView == BOOM_A) {
			if(bPanLeft) camAPan = range(CAM_PAN_MIN, camAPan - SimDT*CAM_PAN_TILT_RATE, CAM_PAN_MAX);
			else if(bPanRight) camAPan = range(CAM_PAN_MIN, camAPan + SimDT*CAM_PAN_TILT_RATE, CAM_PAN_MAX);

			if(bTiltDown) camATilt = range(CAM_TILT_MIN, camATilt - SimDT*CAM_PAN_TILT_RATE, CAM_TILT_MAX);
			else if(bTiltUp) camATilt = range(CAM_TILT_MIN, camATilt + SimDT*CAM_PAN_TILT_RATE, CAM_TILT_MAX);
		}
		else if(cameraView == BOOM_B) {
			if(bPanLeft) camBPan = range(CAM_PAN_MIN, camBPan - SimDT*CAM_PAN_TILT_RATE, CAM_PAN_MAX);
			else if(bPanRight) camBPan = range(CAM_PAN_MIN, camBPan + SimDT*CAM_PAN_TILT_RATE, CAM_PAN_MAX);

			if(bTiltDown) camBTilt = range(CAM_TILT_MIN, camBTilt - SimDT*CAM_PAN_TILT_RATE, CAM_TILT_MAX);
			else if(bTiltUp) camBTilt = range(CAM_TILT_MIN, camBTilt + SimDT*CAM_PAN_TILT_RATE, CAM_TILT_MAX);
		}
		UpdateCameraAnimations();
		update_camera = true;
	}
	// hide camera label after time limit expires
	if(annotationDisplayTime > 0) {
		annotationDisplayTime -= SimDT;
		if(annotationDisplayTime <= 0) oapiAnnotationSetText(nhCameraLabel, NULL);
	}

	// if one LEE is free, allow arm to move
	if(!pLEE[0]->GrappledToBase() || !pLEE[1]->GrappledToBase()) {
		for(unsigned short i=0;i<7;i++) {
			if(joint_motion[i]!=0) {
				SetJointAngle((SSRMS_JOINT)i, joint_angle[i]+SimDT*JOINT_ROTATION_SPEED*SpeedFactor*joint_motion[i]);
				update_vectors=true;
			}
		}

		VECTOR3 RHCInput=_V(0.0, 0.0, 0.0);
		VECTOR3 THCInput=_V(0.0, 0.0, 0.0);
		//VECTOR3 EETrans;

		if(GetAttitudeMode()==RCS_ROT) {
			THCInput = AltKybdInput;
		}
		else {
			THCInput.x=GetThrusterGroupLevel(THGROUP_ATT_FORWARD)-GetThrusterGroupLevel(THGROUP_ATT_BACK);
			THCInput.y=GetThrusterGroupLevel(THGROUP_ATT_RIGHT)-GetThrusterGroupLevel(THGROUP_ATT_LEFT);
			THCInput.z=GetThrusterGroupLevel(THGROUP_ATT_DOWN)-GetThrusterGroupLevel(THGROUP_ATT_UP);
		}

		THCInput*=EE_TRANSLATION_SPEED*SimDT*SpeedFactor;

		if (DiagPushed==true) {
			THCInput = DialogInput;
			DialogInput=_V(0, 0, 0);
			DiagPushed=false;
			THCInput*=EE_TRANSLATION_SPEED*SimDT2*SpeedFactor;
		}

		RHCInput.data[PITCH] = GetThrusterGroupLevel(THGROUP_ATT_PITCHUP)-GetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN);
		RHCInput.data[YAW] = GetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT)-GetThrusterGroupLevel(THGROUP_ATT_YAWLEFT);
		RHCInput.data[ROLL] = GetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT)-GetThrusterGroupLevel(THGROUP_ATT_BANKLEFT);
		//sprintf_s(oapiDebugString(), 255, "R: %f P: %f Y: %f", RHCInput.data[ROLL], RHCInput.data[PITCH], RHCInput.data[YAW]);

		RHCInput*=EE_ROTATION_SPEED*SimDT*SpeedFactor;
			
		if (DiagPushed2==true) {
			RHCInput = DialogInput2;
			DialogInput2=_V(0, 0, 0);
			DiagPushed2=false;
			RHCInput*=EE_ROTATION_SPEED*SimDT2*SpeedFactor;
		}

		//sprintf_s(oapiDebugString(), 255, "EETrans: %f %f %f", EETrans.x/length(EETrans), EETrans.y/length(EETrans), EETrans.z/length(EETrans));
		if(!Eq(THCInput, _V(0.0, 0.0, 0.0), 0.0001) || !Eq(RHCInput, _V(0.0, 0.0, 0.0))) {
			VECTOR3 EETrans, newDir, newRot;
			if(RefFrame==BASE_FRAME) {
				EETrans=THCInput;
				RotateVector(arm_ee_dir, _V(RHCInput.data[ROLL], RHCInput.data[PITCH], RHCInput.data[YAW]), newDir);
				RotateVector(arm_ee_rot,  _V(RHCInput.data[ROLL], RHCInput.data[PITCH], RHCInput.data[YAW]), newRot);
			}
			else if(RefFrame==EE_FRAME) {
				//EETrans = _V(THCInput.x*arm_ee_dir.x, THCInput.y*arm_ee_dir.y, THCInput.z*arm_ee_dir.z);
				EETrans = arm_ee_dir*THCInput.x + arm_ee_rot*THCInput.z + crossp(arm_ee_rot, arm_ee_dir)*THCInput.y;

				VECTOR3 y_axis = crossp(arm_ee_rot, arm_ee_dir);
				MATRIX3 RotMatrix = _M(arm_ee_dir.x, y_axis.x, arm_ee_rot.x,
									   arm_ee_dir.y, y_axis.y, arm_ee_rot.y,
									   arm_ee_dir.z, y_axis.z, arm_ee_rot.z);
				//MATRIX3 RotMatrix = RotationMatrix(arm_ee_dir, , arm_ee_rot);
				MATRIX3 RotMatrixRoll, RotMatrixPitch, RotMatrixYaw;
				GetRotMatrixX(RHCInput.data[ROLL], RotMatrixRoll);
				GetRotMatrixY(RHCInput.data[PITCH], RotMatrixPitch);
				GetRotMatrixZ(RHCInput.data[YAW], RotMatrixYaw);
				RotMatrix = mul(RotMatrix, RotMatrixPitch);
				RotMatrix = mul(RotMatrix, RotMatrixYaw);
				RotMatrix = mul(RotMatrix, RotMatrixRoll);

				newDir = _V(RotMatrix.m11, RotMatrix.m21, RotMatrix.m31);
				newRot = _V(RotMatrix.m13, RotMatrix.m23, RotMatrix.m33);
			}
			MoveEE(arm_ee_pos+EETrans, newDir, newRot, SimDT);
		}
		//if(!Eq(THCInput, _V(0.0, 0.0, 0.0), 0.01)) MoveEE(arm_ee_pos+RotateVectorZ(EETrans, joint_angle[SHOULDER_ROLL]), arm_ee_dir, arm_ee_rot);
	}

	//VECTOR3 dir = arm_tip[1]-arm_tip[0];
	//sprintf_s(oapiDebugString(), 255, "DIR: %f %f %f", dir.x, dir.y, dir.z);
}

void SSRMS::clbkPostStep(double SimT, double SimDT, double MJD)
{
	if (center_arm) SeqSSRMS();

	pSubsystemDirector->PostStep(SimT, SimDT, MJD);

	CheckFlags();

	if(arm_moved) {
		// calculate attitude
		VECTOR3 ee_frame_x, ee_frame_y, ee_frame_z; // reference frame defined by EE direction
		ee_frame_x=ConvertVectorToSSRMSFrame(arm_tip[1]-arm_tip[0]);
		ee_frame_z=ConvertVectorToSSRMSFrame(arm_tip[0]-arm_tip[2]);
		ee_frame_y=crossp(ee_frame_z, ee_frame_x);
		// this rotation matrix is correct regardless of which LEE is active
		MATRIX3 ee_frame_mat = _M(ee_frame_x.x, ee_frame_y.x, ee_frame_z.x,
								  ee_frame_x.y, ee_frame_y.y, ee_frame_z.y,
								  ee_frame_x.z, ee_frame_y.z, ee_frame_z.z);
		ee_angles_output = GetYZX_PYRAnglesFromMatrix(ee_frame_mat)*DEG;
		if(activeLEE == 0) ee_angles_output.data[YAW] = -ee_angles_output.data[YAW];

		// calculate position
		// at the moment, this is just distance (in inches) from base LEE to active LEE (in selected frame)
		// not sure how realistic this is
		ee_pos_output = arm_ee_pos*12.0/fps_to_ms;
		//ee_pos_output = _V(ee_pos_output.z, ee_pos_output.x, -ee_pos_output.y); // shoulder frame
		if(RefFrame == EE_FRAME) ee_pos_output = mul(ee_frame_mat, ee_pos_output);

		UpdateMeshPosition();

		arm_moved=false;
	}
	if(update_vectors) {
		CalculateVectors();

		update_vectors=false;
	}
	if(update_camera) UpdateCameraView();
}

bool SSRMS::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad* skp)
{
	char cbuf[255];

	//VESSEL3::clbkDrawHUD(mode, hps, hDC);

	//display joint angles
	sprintf(cbuf, "   SR      SY      SP      EP      WP      WY      WR");
	skp->Text(hps->W/3, hps->H/10, cbuf, strlen(cbuf));
	sprintf(cbuf, "%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f    (current)", joint_angle[SHOULDER_ROLL], joint_angle[SHOULDER_YAW], joint_angle[SHOULDER_PITCH],
		joint_angle[ELBOW_PITCH], joint_angle[WRIST_PITCH], joint_angle[WRIST_YAW], joint_angle[WRIST_ROLL]);
	skp->Text(hps->W/3, (hps->H/10)+(hps->Markersize/2), cbuf, strlen(cbuf));

	sprintf(cbuf, "%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f    TGT", arm6_Br,arm6_By,arm6_Bp,arm6_ep,arm6_Ap,arm6_Ay,arm6_Ar);
	skp->Text(hps->W/3, (hps->H/10)+(hps->Markersize), cbuf, strlen(cbuf));

	sprintf(cbuf, "%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f    ERR", joint_angle[SHOULDER_ROLL]-arm6_Br, joint_angle[SHOULDER_YAW]-arm6_By, joint_angle[SHOULDER_PITCH]-arm6_Bp,
		joint_angle[ELBOW_PITCH]-arm6_ep, joint_angle[WRIST_PITCH]-arm6_Ap, joint_angle[WRIST_YAW]-arm6_Ay, joint_angle[WRIST_ROLL]-arm6_Ar);
	skp->Text(hps->W/3, (hps->H/10)+(1.5*hps->Markersize), cbuf, strlen(cbuf));

	// position/attitude
	sprintf(cbuf, "   X     Y     Z      Pitch   Yaw    Roll");
	skp->Text(hps->W/3, hps->H/10 + (2.5*hps->Markersize), cbuf, strlen(cbuf));
	sprintf(cbuf, "%+5.0f %+5.0f %+5.0f    %+5.1f  %+4.1f  %+5.1f", ee_pos_output.x, ee_pos_output.y, ee_pos_output.z, ee_angles_output.data[PITCH], ee_angles_output.data[YAW], ee_angles_output.data[ROLL]);
	skp->Text(hps->W/3, hps->H/10 + (3.0*hps->Markersize), cbuf, strlen(cbuf));

	//rotation/translation speed
	sprintf(cbuf, "Rotation speed: %d deg/sec", SpeedFactor);
	skp->Text(hps->W/3, (hps->H/10)+(3.5*hps->Markersize), cbuf, strlen(cbuf));
	sprintf(cbuf, "Translation speed: %d ft/sec", SpeedFactor);
	skp->Text(hps->W/3, (hps->H/10)+(4.0*hps->Markersize), cbuf, strlen(cbuf));

	if(RefFrame==EE_FRAME)
		skp->Text(hps->W/3, (hps->H/10)+(4.5*hps->Markersize), "Using EE frame", 14);
	else skp->Text(hps->W/3, (hps->H/10)+(4.5*hps->Markersize), "Using shoulder frame", 20);

	if(pLEE[activeLEE]->Grappled()) sprintf(cbuf, "LEE%d - Latched", activeLEE+1);
	else sprintf(cbuf, "LEE%d - Open", activeLEE+1);
	skp->Text(hps->W/3, (hps->H/10)+(5.0*hps->Markersize), cbuf, strlen(cbuf));
	sprintf(cbuf, "LEE%d - BASE", (1-activeLEE)+1);
	skp->Text(hps->W/3, (hps->H/10)+(5.5*hps->Markersize), cbuf, strlen(cbuf));
	
	/*sprintf(cbuf, "SR: %.2f", sr_angle);
	TextOut(hDC, hps->W/3, hps->H/10, cbuf, strlen(cbuf));
	sprintf(cbuf, "SY: %.2f", sy_angle);
	TextOut(hDC, hps->W/3, (hps->H/10)+hps->Markersize, cbuf, strlen(cbuf));*/
	
	// draw LEE capture overlay
	if(cameraView == ACTIVE_LEE) {
		// draw crosshairs
		// grapple target pin has diameter of 0.375 inches and is 4 inches long
		const double GRAPPLE_PIN_RADIUS = (0.5*0.375/12.0)/MPS2FPS;
		const double DIST_TO_PIN = (LEE_POS.z-LEE2_CAM_POS.z) - (4.0/12.0)/MPS2FPS; // distance from camera to grapple target pin
		int pinSize = round(hps->Scale*DEG*atan(GRAPPLE_PIN_RADIUS/DIST_TO_PIN)); // angular size of grapple target pin when EE is aligned with grapple fixture
		skp->Line(hps->CX-pinSize, hps->CY, hps->CX-pinSize-hps->Markersize, hps->CY);
		skp->Line(hps->CX+pinSize, hps->CY, hps->CX+pinSize+hps->Markersize, hps->CY);
		skp->Line(hps->CX, hps->CY-pinSize, hps->CX, hps->CY-pinSize-hps->Markersize);
		skp->Line(hps->CX, hps->CY+pinSize, hps->CX, hps->CY+pinSize+hps->Markersize);
	
		// draw red lines
		// used to indicate roll angle and distance error
		const double MAX_ANGLE_ERROR = 15.0*RAD; // angular error represented by red lines
		const double MAX_DISTANCE_ERROR = (LEE_POS.z-LEE2_CAM_POS.z) + 0.2; // distance to grapple pin represented by outer red line
		const double MIN_DISTANCE_ERROR = (LEE_POS.z-LEE2_CAM_POS.z) + 0.1; // distance to grapple pin represented by inner red line
		const double GRAPPLE_TARGET_SIZE = (14.0/12.0)/MPS2FPS; // length of white line on grapple target
		int innerXOffset = round(hps->Scale*DEG*atan(0.5*GRAPPLE_TARGET_SIZE/MIN_DISTANCE_ERROR));
		int outerXOffset = round(hps->Scale*DEG*atan(0.5*GRAPPLE_TARGET_SIZE/MAX_DISTANCE_ERROR));
		int innerLength = round(innerXOffset*tan(MAX_ANGLE_ERROR));
		int outerLength = round(outerXOffset*tan(MAX_ANGLE_ERROR));
		oapi::Pen* redPen = oapiCreatePen(1, 3, 0xff000000/*RGB(255, 0, 0)*/);
		oapi::Pen* oldPen = skp->SetPen(redPen);
		skp->Line(hps->CX + innerXOffset, hps->CY - innerLength, hps->CX + innerXOffset, hps->CY + innerLength);
		skp->Line(hps->CX - innerXOffset, hps->CY - innerLength, hps->CX - innerXOffset, hps->CY + innerLength);
		skp->Line(hps->CX + outerXOffset, hps->CY - outerLength, hps->CX + outerXOffset, hps->CY + outerLength);
		skp->Line(hps->CX - outerXOffset, hps->CY - outerLength, hps->CX - outerXOffset, hps->CY + outerLength);
		skp->SetPen(oldPen);
		oapiReleasePen(redPen);
	}
	
	return true;
}

void SSRMS::clbkRenderHUD(int mode, const HUDPAINTSPEC* hps, SURFHANDLE hDefaultTex)
{
	// overridden to disable Orbitersim default HUD
}

void SSRMS::clbkPostCreation()
{
	//turn off attitude control sound
	//OrbiterSoundHandle = ConnectToOrbiterSoundDLL(GetHandle());
	//if(OrbiterSoundHandle!=-1)SoundOptionOnOff(OrbiterSoundHandle, PLAYATTITUDETHRUST, FALSE);

	SetPropellantMass(ph_null, 1.0);

	if(arm_moved) {
		for(int i=0;i<7;i++) SetJointAngle((SSRMS_JOINT)i, joint_angle[i]);
	}

	UpdateMeshPosition();
	
	UpdateCameraAnimations();

	VESSEL3::clbkPostCreation();
}

void SSRMS::clbkVisualCreated( VISHANDLE _vis, int refcount )
{
	UpdateMeshPosition();
	return;
}

int SSRMS::clbkConsumeBufferedKey(int key, bool down, char *kstate)
{
	if(KEYMOD_CONTROL(kstate)) {
		switch(key) {
			case OAPI_KEY_1:
				if(down) {
					joint_motion[SHOULDER_ROLL]=-1;
				}
				else {
					joint_motion[SHOULDER_ROLL]=0;
				}
				return 1;
			case OAPI_KEY_2:
				if(down) joint_motion[SHOULDER_YAW]=-1;
				else joint_motion[SHOULDER_YAW]=0;
				return 1;
			case OAPI_KEY_3:
				if(down) joint_motion[SHOULDER_PITCH]=-1;
				else joint_motion[SHOULDER_PITCH]=0;
				return 1;
			case OAPI_KEY_4:
				if(down) joint_motion[ELBOW_PITCH]=-1;
				else joint_motion[ELBOW_PITCH]=0;
				return 1;
			case OAPI_KEY_5:
				if(down) joint_motion[WRIST_PITCH]=-1;
				else joint_motion[WRIST_PITCH]=0;
				return 1;
			case OAPI_KEY_6:
				if(down) joint_motion[WRIST_YAW]=-1;
				else joint_motion[WRIST_YAW]=0;
				return 1;
			case OAPI_KEY_7:
				if(down) joint_motion[WRIST_ROLL]=-1;
				else joint_motion[WRIST_ROLL]=0;
				return 1;
			case OAPI_KEY_G:
				//if(!down) ToggleGrapple();
				if(!down) {
					//oapiWriteLog("Toggling Grapple");
					if(pLEE[activeLEE]->Grappled()) pLEE[activeLEE]->Ungrapple();
					else pLEE[activeLEE]->Grapple();
					bGrappled=true;
				}
				return 1;
			case OAPI_KEY_F:
				if(!down) {
					if(RefFrame==EE_FRAME) RefFrame=(FRAME)(RefFrame+1);
					else RefFrame=EE_FRAME;
				}
				return 1;
			case OAPI_KEY_A: // switch active LEE
				if(!down) {
					ChangeActiveLEE();
				}
				return 1;
			case OAPI_KEY_L:
				if(!down) {
					LightChange=true;
					if(!LEELight[activeLEE]->IsActive()) LEELight[activeLEE]->Activate(true);
					else LEELight[activeLEE]->Activate(false);
				}
				return 1;
			case OAPI_KEY_SPACE: // open SSRMS control dialog
				if(down) {
					oapiOpenDialog(m_DlgSSRMSControls.get());
					//oapiOpenDialogEx (hDLL, IDD_SSRMS, SSRMS_DlgProc, DLG_CAPTIONCLOSE, this);
				}
				return 1;
			case OAPI_KEY_I: 
				if(down) {
					FULLINPUT();
				}
				return 1;
			case OAPI_KEY_UP:
				if(!down) {
					if(cameraView < BOOM_B) cameraView = static_cast<CAMERA_VIEW>(cameraView+1);
					else cameraView = ACTIVE_LEE;
					update_camera = true;
					ShowCameraViewLabel();
				}
				return 1;
			case OAPI_KEY_DOWN:
				if(!down) {
					if(cameraView > ACTIVE_LEE) cameraView = static_cast<CAMERA_VIEW>(cameraView-1);
					else cameraView = BOOM_B;
					update_camera = true;
					ShowCameraViewLabel();
				}
				return 1;
		}
	}
	else if(KEYMOD_ALT(kstate)) {
		// rotate boom cameras
		if(cameraView == BOOM_A || cameraView == BOOM_B) {
			switch(key) {
			case OAPI_KEY_UP:
				bTiltUp = down;
				return 1;
			case OAPI_KEY_DOWN:
				bTiltDown = down;
				return 1;
			case OAPI_KEY_LEFT:
				bPanLeft = down;
				return 1;
			case OAPI_KEY_RIGHT:
				bPanRight = down;
				return 1;
			}
		}
	}
	else if(!KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) {
		switch(key) {
			case OAPI_KEY_1:
				if(down) joint_motion[SHOULDER_ROLL]=1;
				else joint_motion[SHOULDER_ROLL]=0;
				return 1;
			case OAPI_KEY_2:
				if(down) joint_motion[SHOULDER_YAW]=1;
				else joint_motion[SHOULDER_YAW]=0;
				return 1;
			case OAPI_KEY_3:
				if(down) joint_motion[SHOULDER_PITCH]=1;
				else joint_motion[SHOULDER_PITCH]=0;
				return 1;
			case OAPI_KEY_4:
				if(down) joint_motion[ELBOW_PITCH]=1;
				else joint_motion[ELBOW_PITCH]=0;
				return 1;
			case OAPI_KEY_5:
				if(down) joint_motion[WRIST_PITCH]=1;
				else joint_motion[WRIST_PITCH]=0;
				return 1;
			case OAPI_KEY_6:
				if(down) joint_motion[WRIST_YAW]=1;
				else joint_motion[WRIST_YAW]=0;
				return 1;
			case OAPI_KEY_7:
				if(down) joint_motion[WRIST_ROLL]=1;
				else joint_motion[WRIST_ROLL]=0;
				return 1;
			case OAPI_KEY_K: // unfold arm
				if(!down) {
					if(foldState.Open() || foldState.Opening()) {
						if(Eq(joint_angle[ELBOW_PITCH], -180.0, 0.1) || Eq(joint_angle[ELBOW_PITCH], 180.0, 0.1)) { // only allow closing if elbow joint is in correct position (otherwise animation doesn't work)
							foldState.action = AnimState::CLOSING;
						}
					}
					else {
						foldState.action = AnimState::OPENING;
					}
				}
				return 1;
			case OAPI_KEY_G:
				if(!down) {
					if(!ShowAttachmentPoints) ShowAttachmentPoints=true;
					else ShowAttachmentPoints=false;
					oapiSetShowGrapplePoints(ShowAttachmentPoints);
				}
				return 1;
			case OAPI_KEY_S:
				if(!down) {
					SpeedChange=true;
					if(SpeedFactor==1) SpeedFactor=2;
					else if(SpeedFactor==2) SpeedFactor=5;
					else if(SpeedFactor==5) SpeedFactor=10;
					else SpeedFactor=1;
				}
				return 1;
		case OAPI_KEY_LEFT:
			if(down) AltKybdInput.y=-1.0;
			else AltKybdInput.y=0.0;
			return 1;
		case OAPI_KEY_RIGHT:
			if(down) AltKybdInput.y=1.0;
			else AltKybdInput.y=0.0;
			return 1;
		case OAPI_KEY_INSERT:
			if(down) AltKybdInput.x=-1.0;
			else AltKybdInput.x=0.0;
			return 1;
		case OAPI_KEY_DELETE:
			if(down) AltKybdInput.x=1.0;
			else AltKybdInput.x=0.0;
			return 1;
		case OAPI_KEY_UP:
			if(down) AltKybdInput.z=1.0;
			else AltKybdInput.z=0.0;
			return 1;
		case OAPI_KEY_DOWN:
			if(down) AltKybdInput.z=-1.0;
			else AltKybdInput.z=0.0;
			return 1;
		}
	}

	return 0;
}

// --------------------------------------------------------------
// Module initialisation
// --------------------------------------------------------------
DLLCLBK void InitModule (MODULEHANDLE hModule)
{
	hDLL = hModule;

}

DLLCLBK void ExitModule (MODULEHANDLE hModule)
{
}

// --------------------------------------------------------------
// Vessel initialisation
// --------------------------------------------------------------
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
	return new SSRMS(hvessel, flightmodel);
}

// --------------------------------------------------------------
// Vessel cleanup
// --------------------------------------------------------------
DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete static_cast<SSRMS*>(vessel);
}
// ==============================================================
//
// modifications DLH
//
//============================================================================================================================
void SSRMS::LOADA ()
{
			arm6_Br = arm1_Br;
			arm6_By = arm1_By;
			arm6_Bp = arm1_Bp;
			arm6_ep = arm1_ep;
			arm6_Ap = arm1_Ap;
			arm6_Ay = arm1_Ay;
			arm6_Ar = arm1_Ar;

}
void SSRMS::LOADB()
{
			arm6_Br = arm2_Br;
			arm6_By = arm2_By;
			arm6_Bp = arm2_Bp;
			arm6_ep = arm2_ep;
			arm6_Ap = arm2_Ap;
			arm6_Ay = arm2_Ay;
			arm6_Ar = arm2_Ar;

}
void SSRMS::LOADC()
{
			arm6_Br = arm3_Br;
			arm6_By = arm3_By;
			arm6_Bp = arm3_Bp;
			arm6_ep = arm3_ep;
			arm6_Ap = arm3_Ap;
			arm6_Ay = arm3_Ay;
			arm6_Ar = arm3_Ar;

}
void SSRMS::LOADD()
{
			arm6_Br = arm4_Br;
			arm6_By = arm4_By;
			arm6_Bp = arm4_Bp;
			arm6_ep = arm4_ep;
			arm6_Ap = arm4_Ap;
			arm6_Ay = arm4_Ay;
			arm6_Ar = arm4_Ar;

}
void SSRMS::SAVEA ()
{
			arm1_Br = joint_angle[SHOULDER_ROLL];
			arm1_By = joint_angle[SHOULDER_YAW];
			arm1_Bp = joint_angle[SHOULDER_PITCH];
			arm1_ep = joint_angle[ELBOW_PITCH];
			arm1_Ap = joint_angle[WRIST_PITCH];
			arm1_Ay = joint_angle[WRIST_YAW];
			arm1_Ar = joint_angle[WRIST_ROLL];
			arm1set = true;

}
void SSRMS::SAVEB()
{
			arm2_Br = joint_angle[SHOULDER_ROLL];
			arm2_By = joint_angle[SHOULDER_YAW];
			arm2_Bp = joint_angle[SHOULDER_PITCH];
			arm2_ep = joint_angle[ELBOW_PITCH];
			arm2_Ap = joint_angle[WRIST_PITCH];
			arm2_Ay = joint_angle[WRIST_YAW];
			arm2_Ar = joint_angle[WRIST_ROLL];
			arm2set = true;

}
void SSRMS::SAVEC()
{
			arm3_Br = joint_angle[SHOULDER_ROLL];
			arm3_By = joint_angle[SHOULDER_YAW];
			arm3_Bp = joint_angle[SHOULDER_PITCH];
			arm3_ep = joint_angle[ELBOW_PITCH];
			arm3_Ap = joint_angle[WRIST_PITCH];
			arm3_Ay = joint_angle[WRIST_YAW];
			arm3_Ar = joint_angle[WRIST_ROLL];
			arm3set = true;

}
void SSRMS::SAVED()
{
			arm4_Br = joint_angle[SHOULDER_ROLL];
			arm4_By = joint_angle[SHOULDER_YAW];
			arm4_Bp = joint_angle[SHOULDER_PITCH];
			arm4_ep = joint_angle[ELBOW_PITCH];
			arm4_Ap = joint_angle[WRIST_PITCH];
			arm4_Ay = joint_angle[WRIST_YAW];
			arm4_Ar = joint_angle[WRIST_ROLL];
			arm4set = true;

}
// ==============================================================
bool SSRMS::ItemChange (const char *rstr)
{
	int ItemValue;
	sscanf (rstr, "%d", &ItemValue);
	DetermineArmSeq(ItemValue);
	center_arm = true;
	Seqindex = 0;
	Seqfinished = false;

	return true;
}
// ==============================================================
void SSRMS::DetermineArmSeq (int ItemValue)
{
	armseq[0] = int(ItemValue/1e6);
	ItemValue = ItemValue - (armseq[0]*1e6);
	armseq[1] = int(ItemValue/1e5);
	ItemValue = ItemValue - (armseq[1]*1e5);
	armseq[2] = int(ItemValue/1e4);
	ItemValue = ItemValue - (armseq[2]*1e4);
	armseq[3] = int(ItemValue/1e3);
	ItemValue = ItemValue - (armseq[3]*1e3);
	armseq[4] = int(ItemValue/1e2);
	ItemValue = ItemValue - (armseq[4]*1e2);
	armseq[5] = int(ItemValue/1e1);
	ItemValue = ItemValue - (armseq[5]*1e1);
	armseq[6] = int(ItemValue);
}
// ==============================================================
void SSRMS::SeqSSRMS()
{
	if (Seqfinished == true){
		Seqfinished = false;
		armseq[Seqindex] = 0;
		Seqindex +=1;
	}
	if (Seqindex > 6) center_arm = false;
	if (center_arm) PosSSRMS(Seqindex);
}
// ==============================================================
void SSRMS::PosSSRMS (int i)
{

	double dt = oapiGetSimStep();       // time step
	double da = JOINT_ROTATION_SPEED*dt*SpeedFactor;  // total rotation angle

	switch (armseq[i])
	{
	case 1:
		if ((joint_angle[0] != armM_Br)) {    // shoulder roll
			if (da >= fabs(joint_angle[0]-armM_Br)) // finished
				SetJointAngle(SHOULDER_ROLL, armM_Br), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(SHOULDER_ROLL,joint_angle[0]+((joint_angle[0] > armM_Br ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 2:
		if ((joint_angle[1] != armM_By)) {    // shoulder yaw
			if (da >= fabs(joint_angle[1]-armM_By)) // finished
				SetJointAngle(SHOULDER_YAW, armM_By), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(SHOULDER_YAW,joint_angle[1]+((joint_angle[1] > armM_By ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 3:
		if ((joint_angle[2] != armM_Bp)) {    // shoulder pitch
			if (da >= fabs(joint_angle[2]-armM_Bp)) // finished
				SetJointAngle(SHOULDER_PITCH, armM_Bp), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(SHOULDER_PITCH,joint_angle[2]+((joint_angle[2] > armM_Bp ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 4:
		if ((joint_angle[3] != armM_ep)) {    // elbow pitch
			if (da >= fabs(joint_angle[3]-armM_ep)) // finished
				SetJointAngle(ELBOW_PITCH, armM_ep), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(ELBOW_PITCH,joint_angle[3]+((joint_angle[3] > armM_ep ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 5:
		if ((joint_angle[4] != armM_Ap)) {    // wrist pitch
			if (da >= fabs(joint_angle[4]-armM_Ap)) // finished
				SetJointAngle(WRIST_PITCH, armM_Ap), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(WRIST_PITCH,joint_angle[4]+((joint_angle[4] > armM_Ap ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 6:
		if ((joint_angle[5] != armM_Ay)) {    // wrist yaw
			if (da >= fabs(joint_angle[5]-armM_Ay)) // finished
				SetJointAngle(WRIST_YAW, armM_Ay), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(WRIST_YAW,joint_angle[5]+((joint_angle[5] > armM_Ay ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 7:
		if ((joint_angle[6] != armM_Ar)) {    // wrist roll
			if (da >= fabs(joint_angle[6]-armM_Ar)) // finished
				SetJointAngle(WRIST_ROLL, armM_Ar), update_vectors=true, Seqfinished = true;
			else 
				SetJointAngle(WRIST_ROLL,joint_angle[6]+((joint_angle[6] > armM_Ar ? -da: da))), update_vectors=true;
		}else{
			Seqfinished = true;
		}
		break;
	case 8:
		if ((joint_angle[0] != armM_Br)||(joint_angle[1] != armM_By)||(joint_angle[2] != armM_Bp)||(joint_angle[3] != armM_ep)||(joint_angle[4] != armM_Ap)||(joint_angle[5] != armM_Ay)||(joint_angle[6] != armM_Ar)){
			if (da >= fabs(joint_angle[0]-armM_Br)) // finished
				SetJointAngle(SHOULDER_ROLL, armM_Br),update_vectors=true;
			else 
				SetJointAngle(SHOULDER_ROLL,joint_angle[0]+((joint_angle[0] > armM_Br ? -da: da))),update_vectors=true;


			if (da >= fabs(joint_angle[1]-armM_By)) // finished
				SetJointAngle(SHOULDER_YAW, armM_By),update_vectors=true;
			else 
				SetJointAngle(SHOULDER_YAW,joint_angle[1]+((joint_angle[1] > armM_By ? -da: da))),update_vectors=true;


			if (da >= fabs(joint_angle[2]-armM_Bp)) // finished
				SetJointAngle(SHOULDER_PITCH, armM_Bp),update_vectors=true;
			else 
				SetJointAngle(SHOULDER_PITCH,joint_angle[2]+((joint_angle[2] > armM_Bp ? -da: da))),update_vectors=true;


			if (da >= fabs(joint_angle[3]-armM_ep)) // finished
				SetJointAngle(ELBOW_PITCH, armM_ep),update_vectors=true;
			else 
				SetJointAngle(ELBOW_PITCH,joint_angle[3]+((joint_angle[3] > armM_ep ? -da: da))),update_vectors=true;


			if (da >= fabs(joint_angle[4]-armM_Ap)) // finished
				SetJointAngle(WRIST_PITCH, armM_Ap),update_vectors=true;
			else 
				SetJointAngle(WRIST_PITCH,joint_angle[4]+((joint_angle[4] > armM_Ap ? -da: da))),update_vectors=true;


			if (da >= fabs(joint_angle[5]-armM_Ay)) // finished
				SetJointAngle(WRIST_YAW, armM_Ay),update_vectors=true;
			else 
				SetJointAngle(WRIST_YAW,joint_angle[5]+((joint_angle[5] > armM_Ay ? -da: da))),update_vectors=true;


			if (da >= fabs(joint_angle[6]-armM_Ar)) // finished
				SetJointAngle(WRIST_ROLL, armM_Ar), update_vectors=true;
			else 
				SetJointAngle(WRIST_ROLL,joint_angle[6]+((joint_angle[6] > armM_Ar ? -da: da))), update_vectors=true;
		}else{
			update_vectors=true;
			Seqfinished = true;
		}

		
		break;
	case 0:
		Seqfinished = true;
		break;
	}
}
bool IteminputJoint(void *id, const char *str, void *data)
{	
		return ((SSRMS*)data)->ItemChangeJoint (str);
}
// ==============================================================
bool SSRMS::ItemChangeJoint (const char *rstr)
{
	int JointValue;
	double JointAmt;
	sscanf (rstr, "%d%lf", &JointValue, &JointAmt);
	if (JointAmt > 270.0) JointAmt = 270.0;
	if (JointAmt < -270.0) JointAmt = -270.0;
	switch (JointValue){
	case 1:
		arm6_Br = JointAmt;
		break;
	case 2:
		arm6_By = JointAmt;
		break;
	case 3:
		arm6_Bp = JointAmt;
		break;
	case 4:
		arm6_ep = JointAmt;
		break;
	case 5:
		arm6_Ap = JointAmt;
		break;
	case 6:
		arm6_Ay = JointAmt;
		break;
	case 7:
		arm6_Ar = JointAmt;
		break;
	}
return true;
}
void SSRMS::ARMINPUT()
{
			oapiOpenInputBox ("Joint (1-7) and Value (degrees)", IteminputJoint, 0, 20, (void*)this);
}
bool FullinputJoint(void *id, const char *str, void *data)
{	
		return ((SSRMS*)data)->FullChangeJoint (str);
}
// ==============================================================
bool SSRMS::FullChangeJoint (const char *rstr)
{
	sscanf (rstr, "%lf%lf%lf%lf%lf%lf%lf", &arm6_Br, &arm6_By, &arm6_Bp, &arm6_ep, &arm6_Ap, &arm6_Ay, &arm6_Ar);

return true;
}
void SSRMS::FULLINPUT()
{
			oapiOpenInputBox ("SR    SY    SP    EP    WP    WY    WR", FullinputJoint, 0, 60, (void*)this);
}
bool Iteminput(void *id, const char *str, void *data)
{	
		return ((SSRMS*)data)->ItemChange (str);
}
void SSRMS::ARMSEQ()
{
			armM_Br = arm6_Br;
			armM_Bp = arm6_Bp;
			armM_By = arm6_By;
			armM_ep = arm6_ep;
			armM_Ap = arm6_Ap;
			armM_Ay = arm6_Ay;
			armM_Ar = arm6_Ar;
			oapiOpenInputBox ("ARM SEQ", Iteminput, 0, 20, (void*)this);		

}
/*
BOOL CALLBACK SSRMS_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		SetWindowText (GetDlgItem (hWnd, IDC_GRAPPLE), sts->pLEE[sts->activeLEE]->Grappled() ? "Release" : "Grapple");
		SetTimer (hWnd, 1, 50, NULL);
		t0 = oapiGetSimTime();
		sts->dialog_open = true;
		return FALSE;
	case WM_CLOSE:
		sts->dialog_open = false;
		return 0;
}
*/
// ==============================================================
void SSRMS::StopMotion ()
{
	//int i;
//	for (i = 0; i < 15; i++) armmotion[i] = false;
	center_arm = false;
}
// ==============================================================
void SSRMS::CheckFlags()
{
	if (arm1set && arm1_Br==0.0 && arm1_By == 0.0 && arm1_Bp == 0.0
		&& arm1_ep == 0.0 && arm1_Ar == 0.0 && arm1_Ay == 0.0 
		&& arm1_Ap == 0.0) arm1set = false;
	if (arm2set && arm2_Br==0.0 && arm2_By == 0.0 && arm2_Bp == 0.0
		&& arm2_ep == 0.0 && arm2_Ar == 0.0 && arm2_Ay == 0.0 
		&& arm2_Ap == 0.0) arm2set = false;
	if (arm3set && arm3_Br==0.0 && arm3_By == 0.0 && arm3_Bp == 0.0
		&& arm3_ep == 0.0 && arm3_Ar == 0.0 && arm3_Ay == 0.0 
		&& arm3_Ap == 0.0) arm3set = false;
	if (arm4set && arm4_Br==0.0 && arm4_By == 0.0 && arm4_Bp == 0.0
		&& arm4_ep == 0.0 && arm4_Ar == 0.0 && arm4_Ay == 0.0 
		&& arm4_Ap == 0.0) arm4set = false;
	if (arm5set && arm5_Br==0.0 && arm5_By == 0.0 && arm5_Bp == 0.0
		&& arm5_ep == 0.0 && arm5_Ar == 0.0 && arm5_Ay == 0.0 
		&& arm5_Ap == 0.0) arm5set = false;
}
//============================================================================================================================
