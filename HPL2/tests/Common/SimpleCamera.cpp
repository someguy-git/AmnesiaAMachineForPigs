/*
 * Copyright © 2011-2020 Frictional Games
 *
 * This file is part of Amnesia: A Machine For Pigs.
 *
 * Amnesia: A Machine For Pigs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Amnesia: A Machine For Pigs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Amnesia: A Machine For Pigs.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "SimpleCamera.h"

#include "input/Input.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "gui/GuiSet.h"
#include "gui/GuiSkin.h"
#include "scene/Camera.h"
#include "scene/Viewport.h"
#include "resources/FontManager.h"

using namespace hpl;

//-----------------------------------------------------------------------

cSimpleCamera::cSimpleCamera(const tString& asName, cEngine* apEngine, cWorld* apWorld,
							 float afMoveSpeed, const cVector3f& avStartPos, bool abMouseMode)
	: iUpdateable(asName)
{
	mpEngine = apEngine;
	mpWorld = apWorld;
	mfMoveSpeed = afMoveSpeed;
	mbMouseMode = false;

	////////////////////////////////
	// Camera
	mpCamera = mpEngine->GetScene()->CreateCamera(eCameraMoveMode_Fly);
	mpCamera->SetPosition(avStartPos);
	mpCamera->SetPitchLimits(-1.5f, 1.5f);
	mpCamera->SetYaw(0);
	mpCamera->SetPitch(0);

	////////////////////////////////
	// Viewport
	mpViewport = mpEngine->GetScene()->CreateViewport(mpCamera, mpWorld);
	mpViewport->SetSize(mpEngine->GetGraphics()->GetLowLevel()->GetScreenSizeInt());
	mpViewport->SetActive(true);
	mpViewport->SetVisible(true);

	////////////////////////////////
	// Gui
	mpSkin = mpEngine->GetGui()->CreateSkin("gui_default.skin");
	mpGuiSet = NULL;

	if(mpSkin)
	{
		mpGuiSet = mpEngine->GetGui()->CreateSet(asName + "_Set", mpSkin);
		mpGuiSet->SetDrawMouse(false);
		mpViewport->AddGuiSet(mpGuiSet);
	}
	else
	{
		Error("cSimpleCamera: could not load gui skin 'gui_default.skin', gui disabled.\n");
	}

	mpFont = mpEngine->GetResources()->GetFontManager()->CreateFontData("game_default.fnt");

	////////////////////////////////
	// Mouse
	SetMouseMode(abMouseMode);
}

//-----------------------------------------------------------------------

cSimpleCamera::~cSimpleCamera()
{
	if(mpCamera) mpCamera->SetPitchLimits(0, 0);
}

//-----------------------------------------------------------------------

void cSimpleCamera::SetMouseMode(bool abX)
{
	mbMouseMode = abX;

	if(mpEngine && mpEngine->GetInput())
	{
		mpEngine->GetInput()->GetLowLevel()->LockInput(abX);
	}

	if(mpGuiSet)
	{
		mpGuiSet->SetDrawMouse(!abX);
	}
}

//-----------------------------------------------------------------------

void cSimpleCamera::Update(float afTimeStep)
{
	if(mpCamera == NULL || mpEngine == NULL) return;

	////////////////////////////////
	// Mouse look
	if(mbMouseMode)
	{
		iMouse* pMouse = mpEngine->GetInput()->GetMouse();
		if(pMouse)
		{
			cVector2l vRel = pMouse->GetRelPosition();

			if(vRel.x != 0) mpCamera->AddYaw(-(float)vRel.x * 0.0025f);
			if(vRel.y != 0) mpCamera->AddPitch(-(float)vRel.y * 0.0025f);
		}
	}

	////////////////////////////////
	// Keyboard movement
	iKeyboard* pKeyboard = mpEngine->GetInput()->GetKeyboard();
	if(pKeyboard == NULL) return;

	float fSpeed = mfMoveSpeed * afTimeStep;
	if(pKeyboard->KeyIsDown(eKey_LeftShift)) fSpeed *= 3.0f;

	if(pKeyboard->KeyIsDown(eKey_W)) mpCamera->MoveForward(fSpeed);
	if(pKeyboard->KeyIsDown(eKey_S)) mpCamera->MoveForward(-fSpeed);

	if(pKeyboard->KeyIsDown(eKey_A)) mpCamera->MoveRight(-fSpeed);
	if(pKeyboard->KeyIsDown(eKey_D)) mpCamera->MoveRight(fSpeed);

	if(pKeyboard->KeyIsDown(eKey_Q)) mpCamera->MoveUp(-fSpeed);
	if(pKeyboard->KeyIsDown(eKey_E)) mpCamera->MoveUp(fSpeed);
}

//-----------------------------------------------------------------------

void cSimpleCamera::ListContainerNodeData(iRenderableContainerNode* apNode, int alLevel)
{
	if(apNode == NULL) return;

	tString sTab = "";
	for(int i=0; i<alLevel; ++i) sTab += "  ";

	int lObjectNum = apNode->HasObjects() ? (int)apNode->GetObjectList()->size() : 0;

	Log("%s-- Node %p (level %d, objects %d) --\n", sTab.c_str(), apNode, alLevel, lObjectNum);

	if(apNode->HasObjects())
	{
		tRenderableListIt it = apNode->GetObjectList()->begin();
		for(; it != apNode->GetObjectList()->end(); ++it)
		{
			iRenderable* pObject = *it;
			Log("%s   '%s' (%s)\n", sTab.c_str(), pObject->GetName().c_str(), pObject->GetEntityType().c_str());
		}
	}

	if(apNode->HasChildNodes())
	{
		tRenderableContainerNodeListIt childIt = apNode->GetChildNodeList()->begin();
		for(; childIt != apNode->GetChildNodeList()->end(); ++childIt)
		{
			ListContainerNodeData(*childIt, alLevel + 1);
		}
	}
}

//-----------------------------------------------------------------------
