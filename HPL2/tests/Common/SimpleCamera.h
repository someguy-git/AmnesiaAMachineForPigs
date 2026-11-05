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

#ifndef HPL_SIMPLE_CAMERA_H
#define HPL_SIMPLE_CAMERA_H

#include "hpl.h"

namespace hpl {

	//----------------------------------------------

	/**
	 * Free-flying camera used by the standalone tools (MapView, ModelView).
	 *
	 * Creates its own camera, viewport and gui set, and moves the camera
	 * from the keyboard (WASD + QE, shift for speed) and the mouse while
	 * mouse mode is active.
	 */
	class cSimpleCamera : public iUpdateable
	{
	public:
		cSimpleCamera(const tString& asName, cEngine* apEngine, cWorld* apWorld,
					  float afMoveSpeed, const cVector3f& avStartPos, bool abMouseMode);
		~cSimpleCamera();

		void Update(float afTimeStep);

		cCamera* GetCamera() { return mpCamera; }
		cViewport* GetViewport() { return mpViewport; }
		cGuiSet* GetSet() { return mpGuiSet; }
		iFontData* GetFont() { return mpFont; }

		void SetMouseMode(bool abX);
		bool GetMouseMode() { return mbMouseMode; }

		/** Dumps the contents of a renderable container tree to the log. */
		void ListContainerNodeData(iRenderableContainerNode* apNode, int alLevel);

	private:
		cEngine* mpEngine;

		cCamera* mpCamera;
		cViewport* mpViewport;
		cWorld* mpWorld;

		cGuiSkin* mpSkin;
		cGuiSet* mpGuiSet;
		iFontData* mpFont;

		float mfMoveSpeed;
		bool mbMouseMode;
	};

	//----------------------------------------------

};
#endif // HPL_SIMPLE_CAMERA_H
