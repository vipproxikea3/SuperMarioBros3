#include "MarioWorldMap.h"
#include "Station.h"
#include "Utils.h"

void CMarioWorldMap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Calculate dx, dy 
	CGameObject::Update(dt);

	x += dx;
	y += dy;

	if (state != MARIOWORLDMAP_STATE_IDLE) {
		switch (state)
		{
		case MARIOWORLDMAP_STATE_WALKING_LEFT:
			if (x <= lLeft) {
				x = lLeft;
				this->SetState(MARIOWORLDMAP_STATE_IDLE);
			}
			break;
		case MARIOWORLDMAP_STATE_WALKING_RIGHT:
			if (x >= lRight) {
				x = lRight;
				this->SetState(MARIOWORLDMAP_STATE_IDLE);
			}
			break;
		case MARIOWORLDMAP_STATE_WALKING_TOP:
			if (y <= lTop) {
				y = lTop;
				this->SetState(MARIOWORLDMAP_STATE_IDLE);
			}
			break;
		case MARIOWORLDMAP_STATE_WALKING_BOTTOM:
			if (y >= lBottom) {
				y = lBottom;
				this->SetState(MARIOWORLDMAP_STATE_IDLE);
			}
			break;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
	}
	else
	{
		DebugOut(L"[INFO]Collision\n");

		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		float x0 = x;
		float y0 = y;

		for (UINT i = 0; i < coEventsResult.size(); i++) {
			LPCOLLISIONEVENT e = coEventsResult[i];

			//STATION
			if (dynamic_cast<CStation*>(e->obj)) {
				CStation* station = dynamic_cast<CStation*>(e->obj);
				switch (this->GetState())
				{
				case MARIOWORLDMAP_STATE_WALKING_LEFT:
					this->SetLimit(station->x_start, -1000, 1000, 1000);
					this->SetControl(station->canLeft, station->canUp, station->canRight, station->canDown);
					break;
				case MARIOWORLDMAP_STATE_WALKING_RIGHT:
					this->SetLimit(-1000, -1000, station->x_start, 1000);
					this->SetControl(station->canLeft, station->canUp, station->canRight, station->canDown);
					break;
				case MARIOWORLDMAP_STATE_WALKING_TOP:
					this->SetLimit(-1000, station->y_start, 1000, 1000);
					this->SetControl(station->canLeft, station->canUp, station->canRight, station->canDown);
					break;
				case MARIOWORLDMAP_STATE_WALKING_BOTTOM:
					this->SetLimit(-1000, -1000, 1000, station->y_start);
					this->SetControl(station->canLeft, station->canUp, station->canRight, station->canDown);
					break;
				}

				sceneId = station->GetTargetScene();
				DebugOut(L"[INFO] Scene %d! \n", sceneId);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMarioWorldMap::Render()
{
	animation_set->at(MARIOWORLDMAP_ANI_IDLE)->Render(x, y, 255);
}

void CMarioWorldMap::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIOWORLDMAP_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case MARIOWORLDMAP_STATE_WALKING_LEFT:
		vx = -MARIOWORLDMAP_WALKING_SPEED;
		vy = 0;
		break;
	case MARIOWORLDMAP_STATE_WALKING_RIGHT:
		vx = MARIOWORLDMAP_WALKING_SPEED;
		vy = 0;
		break;
	case MARIOWORLDMAP_STATE_WALKING_TOP:
		vx = 0;
		vy = -MARIOWORLDMAP_WALKING_SPEED;
		break;
	case MARIOWORLDMAP_STATE_WALKING_BOTTOM:
		vx = 0;
		vy = MARIOWORLDMAP_WALKING_SPEED;
		break;
	}
}

void CMarioWorldMap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MARIOWORLDMAP_BBOX_WIDTH;
	bottom = y + MARIOWORLDMAP_BBOX_HEIGHT;
}