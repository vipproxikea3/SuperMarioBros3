#include <algorithm>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Block.h"
#include "Goomba.h"
#include "Coin.h"
#include "BrickReward.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "Koopa.h"
#include "Gate.h"
#include "PlayScene.h"
#include "Point.h"
#include "PiranhaPlant.h"
#include "VenusFireTrap.h"
#include "MarioTransformEffect.h"
#include "EndSceneTitle.h"

void CMario::CalVx(DWORD dt) {
	vx += ax * dt;
	if (this->GetState() == MARIO_STATE_RUN_RIGHT || this->GetState() == MARIO_STATE_RUN_LEFT) {
		if (abs(vx) > MARIO_RUN_SPEED)
			vx = nx * MARIO_RUN_SPEED;
	} else if (abs(vx) > MARIO_WALKING_SPEED)
		vx = nx * MARIO_WALKING_SPEED;

	if (state == MARIO_STATE_IDLE)
	{
		if (nx > 0)
		{
			if (vx < 0)
				vx = 0;
		}
		else
			if (vx > 0)
				vx = 0;
	}

	// Update run speed stack
	UpdateRunSpeedStack();
}

void CMario::UpdateRunSpeedStack() {
	runSpeedStack = 0;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_1)
		runSpeedStack = 1;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_2)
		runSpeedStack = 2;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_3)
		runSpeedStack = 3;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_4)
		runSpeedStack = 4;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_5)
		runSpeedStack = 5;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_6)
		runSpeedStack = 6;
	if (abs(vx) > MARRIO_RUN_SPEED_STACK_7)
		runSpeedStack = 7;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Update bullets
	for (int i = 0; i < bullets.size(); i++) {
		if (!bullets[i]->isDisable)
			bullets[i]->Update(dt, coObjects);
	}

	// clean bullets
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isDisable) {
			CGameObject* p = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete p;
		}
	}


	// Check Out Scene
	if (GetTickCount64() - start_out_scene >= MARIO_TIME_OUT_SCENE && readyOutScene) {
		outScene = true;
	}

	// Update hugging shell Position
	if (hugging)
		UpdateHuggingShellPosition();

	// Stop spingning
	if (GetTickCount64() - spin_start >= MARIO_SPIN_TIME && spinning) {
		spinning = false;
		spin_start = NULL;
		TailAttacked = false;
	}

	// Stop transform
	if (GetTickCount64() - start_transform >= MARIO_TRANSFORM_TIME && transforming)
		StopTransform();

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Check flying
	if (runSpeedStack != 7) {
		SetStopFly();
		fly_start = NULL;
	}

	if (GetTickCount64() - fly_start >= MARIO_FLY_TIME) {
		flyIng = false;
		fly_start = NULL;
	}

	// Simple fall down
	if (!flyIng && !fallIng) {
		vy += MARIO_GRAVITY * dt;
	}

	PipeWalking();

	// Calculate vx
	if (canControl)
		CalVx(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		isOnGround = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float x0 = x;
		float y0 = y;

		x = x0 + dx;
		y = y0 + dy;

		if (ny != 0) isOnGround = false;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->ny == -1 && !dynamic_cast<CCoin*>(e->obj) && !dynamic_cast<CSuperMushroom*>(e->obj) && !dynamic_cast<CSuperLeaf*>(e->obj)) {
				canJump = 1;
				fallIng = false;
				isOnGround = true;
			}

			// PIRANHA PLANT
			if (dynamic_cast<CPiranhaPlant*>(e->obj))
			{
				CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj);
				BasicCollision(min_tx, min_ty, nx, ny, x0, y0);

				if (e->ny != 0 && untouchable == 0) {
					if (this->GetState() != PIRANHAPLANT_STATE_DIE && untouchable == 0)
						lvlDown();
				}

				if (e->nx != 0) {
					// raccoon spin
					if (spinning) {
						if (e->nx * this->nx < 0) {
							vx = 0;
							plant->SetState(PIRANHAPLANT_STATE_DIE);
							ShowPoint();
						}
						else {
							if (this->GetState() != PIRANHAPLANT_STATE_DIE && untouchable == 0)
								lvlDown();
						}
					}
					else if (this->GetState() != PIRANHAPLANT_STATE_DIE && untouchable == 0)
					{
						lvlDown();
					}
				}
			}

			// VENUS FIRE TRAP
			if (dynamic_cast<CVenusFireTrap*>(e->obj))
			{
				CVenusFireTrap* trap = dynamic_cast<CVenusFireTrap*>(e->obj);
				BasicCollision(min_tx, min_ty, nx, ny, x0, y0);

				if (e->ny != 0 && untouchable == 0) {
					if (this->GetState() != VENUSFIRETRAP_STATE_DIE && untouchable == 0)
						lvlDown();
				}

				if (e->nx != 0) {
					if (this->GetState() != VENUSFIRETRAP_STATE_DIE && this->GetState() != VENUSFIRETRAP_STATE_IDLE && untouchable == 0)
					{
						lvlDown();
					}
				}
			}

			// GOOMBA
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				if (e->ny != 0) {
					if (e->ny < 0)
					{
						vy = 0;
						y = y0 + min_ty * dy + e->ny * 0.4f;
						if (goomba->GetState() != GOOMBA_STATE_DIE_Y) {
							goomba->SetState(GOOMBA_STATE_DIE_Y);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							ShowPoint();
						}
					}
					else {
						flyIng = false;
						if (untouchable == 0)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE_Y && goomba->GetState() != GOOMBA_STATE_DIE_X)
							{
								lvlDown();
							}
						}
					}
				}

				if (e->nx != 0) {
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE_Y && goomba->GetState() != GOOMBA_STATE_DIE_X)
						{
							lvlDown();
						}
					}
				}
			}

			// PARAGOOMBA
			if (dynamic_cast<CParaGoomba*>(e->obj))
			{
				CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);

				if (e->ny != 0) {
					vy = 0;
					if (e->ny < 0)
					{
						y = y0 + min_ty * dy + e->ny * 0.4f;
						if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_X && paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_Y)
						{
							if (paraGoomba->GetLevel() == PARAGOOMBA_LEVEL_WING) {
								paraGoomba->SetLevel(PARAGOOMBA_LEVEL_GOOMBA);
							}
							else {
								paraGoomba->SetState(PARAGOOMBA_STATE_DIE_Y);
							}
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							ShowPoint();
						}
					}
					else {
						flyIng = false;
						if (untouchable == 0)
						{
							if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_Y && paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_X)
							{
								lvlDown();
							}
						}
					}
				}

				if (e->nx != 0) {
					if (untouchable == 0)
					{
						if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_Y && paraGoomba->GetState() != PARAGOOMBA_STATE_DIE_X)
						{
							lvlDown();
						}
					}
				}
			}

			// KOOPA
			if (dynamic_cast<CKoopa*>(e->obj))
			{
				CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

				if (koopa->GetLevel() == KOOPA_LEVEL_KOOPA) {
					if (e->ny != 0) {
						if (e->ny > 0)
						{
							vy = 0;
							y = y0 + min_ty * dy + e->ny * 0.4f;
							flyIng = false;
							if (untouchable == 0 && !pipeWalking)
							{
								lvlDown();
							}
						}
						else if (e->ny < 0) {
							vy = 0;
							y = y0 + min_ty * dy + e->ny * 0.4f;
							koopa->lvlDown();
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							ShowPoint();
						}
					}

					if (e->nx != 0) {
						if (untouchable == 0 && !pipeWalking)
						{
							lvlDown();
						}
					}
				}
				else {
					switch (koopa->GetState()) {
					case SHELL_STATE_IDLE:
						if (e->nx != 0) {
							if (isReadyHug) {
								huggingShell = koopa;
								koopa->SetState(SHELL_STATE_BEHUG);
								hugging = true;
								isReadyHug = false;
							}
							else {
								vx = 0;
								x = x0 + min_tx * dx + e->nx * 0.4f;
								if (e->nx < 0) {
									koopa->SetState(SHELL_STATE_WALKING_RIGHT);
								}
								else {
									koopa->SetState(SHELL_STATE_WALKING_LEFT);
								}
							}
						}
						if (e->ny == 1) {
							vy = 0;
							y = y0;
							flyIng = false;
							if (untouchable == 0)
							{
								lvlDown();
							}							
						}
						else if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							if (this->x < koopa->x + (SHELL_BBOX_WIDTH / 2)) {
								koopa->SetState(SHELL_STATE_WALKING_RIGHT);
							}
							else {
								koopa->SetState(SHELL_STATE_WALKING_LEFT);
							}
						}
						break;
					case SHELL_STATE_OVERTURN:
						if (e->nx != 0) {
							if (isReadyHug) {
								huggingShell = koopa;
								koopa->SetState(SHELL_STATE_BEHUG);
								hugging = true;
								isReadyHug = false;
							}
							else {
								vx = 0;
								x = x0 + min_tx * dx + e->nx * 0.4f;
								if (e->nx < 0) {
									koopa->SetState(SHELL_STATE_WALKING_RIGHT);
								}
								else {
									koopa->SetState(SHELL_STATE_WALKING_LEFT);
								}
							}
						}
						if (e->ny == 1) {
							vy = 0;
							y = y0;
							flyIng = false;
							if (untouchable == 0)
							{
								lvlDown();
							}
						}
						else if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							if (this->x < koopa->x + (SHELL_BBOX_WIDTH / 2)) {
								koopa->SetState(SHELL_STATE_WALKING_RIGHT);
							}
							else {
								koopa->SetState(SHELL_STATE_WALKING_LEFT);
							}
						}
						break;
					case SHELL_STATE_WALKING_LEFT:
						if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							koopa->SetState(SHELL_STATE_IDLE);
						}
						else if (untouchable == 0)
						{
							lvlDown();
						}
						break;
					case SHELL_STATE_WALKING_RIGHT:
						if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							koopa->SetState(SHELL_STATE_IDLE);
						}
						else if (untouchable == 0)
						{
							lvlDown();
						}
						break;
					}
				}
			}

			// PARAKOOPA
			if (dynamic_cast<CParaKoopa*>(e->obj))
			{
				CParaKoopa* paraKoopa = dynamic_cast<CParaKoopa*>(e->obj);

				if (paraKoopa->GetLevel() == PARAKOOPA_LEVEL_KOOPA || paraKoopa->GetLevel() == PARAKOOPA_LEVEL_WING) {
					if (e->ny != 0) {
						vy = 0;
						if (e->ny < 0)
						{
							y = y0 + min_ty * dy + e->ny * 0.4f;
							paraKoopa->lvlDown();
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							ShowPoint();
						}
						else {
							flyIng = false;
							if (untouchable == 0)
							{
								lvlDown();
							}
						}
					}

					if (e->nx != 0) {
						if (untouchable == 0)
						{
							lvlDown();
						}
					}
				}
				else {
					switch (paraKoopa->GetState()) {
					case PARAKOOPA_SHELL_STATE_IDLE:
						if (e->nx != 0) {
							if (isReadyHug) {
								huggingShell = paraKoopa;
								paraKoopa->SetState(PARAKOOPA_SHELL_STATE_BEHUG);
								hugging = true;
								isReadyHug = false;
							}
							else {
								vx = 0;
								x = x0 + min_tx * dx + e->nx * 0.4f;
								if (e->nx < 0) {
									paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_RIGHT);
								}
								else {
									paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_LEFT);
								}
							}
						}
						if (e->ny == 1) {
							vy = 0;
							y = y0;
							flyIng = false;
							if (untouchable == 0)
							{
								lvlDown();
							}
						}
						else if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							if (this->x < paraKoopa->x + (PARAKOOPA_SHELL_BBOX_WIDTH / 2)) {
								paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_RIGHT);
							}
							else {
								paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_LEFT);
							}
						}
						break;
					case PARAKOOPA_SHELL_STATE_OVERTURN:
						if (e->nx != 0) {
							if (isReadyHug) {
								huggingShell = paraKoopa;
								paraKoopa->SetState(PARAKOOPA_SHELL_STATE_BEHUG);
								hugging = true;
								isReadyHug = false;
							}
							else {
								vx = 0;
								x = x0 + min_tx * dx + e->nx * 0.4f;
								if (e->nx < 0) {
									paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_RIGHT);
								}
								else {
									paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_LEFT);
								}
							}
						}
						if (e->ny == 1) {
							vy = 0;
							y = y0;
							flyIng = false;
							if (untouchable == 0)
							{
								lvlDown();
							}
						}
						else if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							if (this->x < paraKoopa->x + (PARAKOOPA_SHELL_BBOX_WIDTH / 2)) {
								paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_RIGHT);
							}
							else {
								paraKoopa->SetState(PARAKOOPA_SHELL_STATE_WALKING_LEFT);
							}
						}
						break;
					case PARAKOOPA_SHELL_STATE_WALKING_RIGHT:
						if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							paraKoopa->SetState(PARAKOOPA_SHELL_STATE_IDLE);
						}
						else if (untouchable == 0)
						{
							lvlDown();
						}
						break;
					case PARAKOOPA_SHELL_STATE_WALKING_LEFT:
						if (e->ny == -1) {
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							paraKoopa->SetState(PARAKOOPA_SHELL_STATE_IDLE);
						}
						else if (untouchable == 0)
						{
							lvlDown();
						}
						break;
					}
				}
			}

			// GATE
			if (dynamic_cast<CGate*>(e->obj)) {
				if (!pipeWalking && e->ny != 0) {
					CGate* gate = dynamic_cast<CGate*>(e->obj);
					targetX = gate->GetTargetX();
					targetY = gate->GetTargetY();
					targetZone = gate->GetTargetZone();
					typePipeWalking = gate->GetType();
					y_pipeWalking_start = gate->y;
					x_pipeWalking_start = gate->x;
					pipeWalking = true;
				}
			}

			// LOTTERY
			if (dynamic_cast<CLottery*>(e->obj)) {
				this->canControl = false;
				CLottery* lottery = dynamic_cast<CLottery*>(e->obj);
				lottery->StopChange();
				lottery->Fly();
				ShowEndSceneTitle(lottery->GetState());
				CBackup::GetInstance()->SetLottery(lottery->GetState() + 100);
				this->SetState(MARIO_STATE_IDLE);
				SetStopFly();
				SetStopFall();
				this->vx = 0;
				this->ax = 0;
				this->vy = 0;
			}

			// BRICKREWARD
			if (dynamic_cast<CBrickReward*>(e->obj)) {
				CBrickReward* brickReward = dynamic_cast<CBrickReward*>(e->obj);
				BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
				if (e->ny == 1 && brickReward->GetState() == BRICKREWARD_STATE_IDLE) {
					brickReward->SetState(BRICKREWARD_STATE_JUMP);
					flyIng = false;
				}
			}

			// BLOCK
			if (dynamic_cast<CBlock*>(e->obj)) {
				if (pipeWalking) return;
				CBlock* block = dynamic_cast<CBlock*>(e->obj);

				if (e->ny == -1 && block->isBlockTop()) {
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + ny * 0.4f;
				}

				if (e->ny == 1 && block->isBlockBottom()) {
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + ny * 0.4f;

					flyIng = false;
				}

				if (e->nx == -1 && block->isBlockLeft()) {
					this->vx = 0;
					this->x = x0 + min_tx * this->dx + nx * 0.4f;
				}

				if (e->nx == 1 && block->isBlockRight()) {
					this->vx = 0;
					this->x = x0 + min_tx * this->dx + nx * 0.4f;
				}
			}

			// BREAKBLOCK
			if (dynamic_cast<CBreakBlock*>(e->obj)) {
				CBreakBlock* breakBlock = dynamic_cast<CBreakBlock*>(e->obj);
				BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);

				if (e->ny == 1 && breakBlock->GetState() == BREAKBLOCK_STATE_IDLE) {
					breakBlock->ShowReward();
				}
			}

			// SWITCHBLOCK
			if (dynamic_cast<CSwitchBlock*>(e->obj)) {
				CSwitchBlock* switchBlock = dynamic_cast<CSwitchBlock*>(e->obj);
				BasicCollision(min_tx, min_ty, e->nx, e->ny, x0, y0);
				if (e->ny == -1 && switchBlock->GetState() == SWITCHBLOCK_STATE_IDLE) {
					switchBlock->Switch();
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}

			// FLY BLOCK
			if (dynamic_cast<CFlyBlock*>(e->obj)) {
				CFlyBlock* block = dynamic_cast<CFlyBlock*>(e->obj);
				if (e->ny == -1 && block->GetState() == FLYBLOCK_STATE_WALKING)
				{
					block->SetState(FLYBLOCK_STATE_FALL);
				}
				if (e->ny == -1) {
					this->vy = block->vy * 0.75;
					this->y = y0 + min_ty * this->dy + ny * 0.4f;
				}
				if (e->ny == 1) {
					this->vy = 0;
					this->y = y0 + min_ty * this->dy + ny * 2.5f;
				}
				if (e->nx != 0) {
					this->vx = 0;
					this->x = x0 + min_tx * this->dx + nx * 2.5f;
				}
			}
		}
	}

	// Check out zone
	if (x < zoneLeft) x = zoneLeft;
	if (x + MARIO_BIG_BBOX_WIDTH > zoneRight) x = zoneRight - MARIO_BIG_BBOX_WIDTH;
	if (y > zoneBottom) {
		if (this->GetState() != MARIO_STATE_DIE)
		{
			this->SetLevel(MARIO_LEVEL_SMALL);
			this->SetState(MARIO_STATE_DIE);
		}
	}

	// Check out Camera with CameraMan
	CCameraMan* cameraMan = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetCameraMan();
	if (cameraMan) {
		if (x < cameraMan->x)
			x = cameraMan->x;
		if (x + 16.0f > cameraMan->x + CGame::GetInstance()->GetScreenWidth())
			x = cameraMan->x + CGame::GetInstance()->GetScreenWidth() - 16.0f;
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::PipeWalking() {
	if (pipeWalking) {
		float height = MARIO_SMALL_BBOX_HEIGHT;
		if (level != MARIO_LEVEL_SMALL)
			height = MARIO_BIG_BBOX_HEIGHT;

		if (typePipeWalking == 1) {
			if (y <= y_pipeWalking_start) {
				this->SetState(MARIO_STATE_IDLE);
				this->x = x_pipeWalking_start;
				canControl = false;
				vx = 0;
				vy = MARIO_PIPE_WALKING_SPEED;
			}
			else {
				CCameraMan* cameraMan = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetCameraMan();
				if (cameraMan)
					cameraMan->x += targetX - this->x;
				this->SetPosition(targetX, targetY);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SwitchZone(targetZone);
				this->canControl = true;
				pipeWalking = false;
				vy = 0;
				vx = 0;
			}
		}
		else if (typePipeWalking == -1) {
			if (!teleported) {
				CCameraMan* cameraMan = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetCameraMan();
				if (cameraMan)
					cameraMan->x += targetX - this->x;
				this->SetPosition(targetX, targetY);
				((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SwitchZone(targetZone);
				teleported = true;
			}
			if (y + height >= targetY) {
				this->SetState(MARIO_STATE_IDLE);
				canControl = false;
				vx = 0;
				vy = -MARIO_PIPE_WALKING_SPEED;
			}
			else {
				this->canControl = true;
				pipeWalking = false;
				vy = 0;
				vx = 0;
				teleported = false;
			}
		}
	}
}

void CMario::ShowMarioTransformEffect() {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(28);
	CMarioTransformEffect* effect = new CMarioTransformEffect();
	effect->SetPosition(x, y);
	effect->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(effect);
}

void CMario::ShowEndSceneTitle(int type) {
	float cx, cy;
	float screenWidth, screenHeight;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);
	screenWidth = game->GetScreenWidth();
	screenHeight = game->GetScreenHeight();

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(29);
	CEndSceneTitle* title = new CEndSceneTitle(type);
	title->SetPosition(cx + (screenWidth - ENDSCENETITLE_BBOX_WIDTH) * 0.5, cy + (screenHeight - ENDSCENETITLE_BBOX_HEIGHT) * 0.25);
	title->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(title);
}

void CMario::UpdateHuggingShellPosition() {
	if (huggingShell == NULL) return;
	switch (this->level)
	{
	case MARIO_LEVEL_SMALL:
		if (nx > 0)
			huggingShell->SetPosition(this->x + 12.0f, this->y - 1.0f);
		else
			huggingShell->SetPosition(this->x - 12.0f, this->y - 1.0f);
		break;
	default:
		if (nx > 0)
			huggingShell->SetPosition(this->x + 12.0f, this->y + 5.0f);
		else
			huggingShell->SetPosition(this->x - 12.0f, this->y + 5.0f);
		break;
	}
}

void CMario::StopHug() {
	this->isReadyHug = false;
	if (!hugging) return;
	if (nx > 0) {
		huggingShell->SetState(SHELL_STATE_WALKING_RIGHT);
	}
	else {
		huggingShell->SetState(SHELL_STATE_WALKING_LEFT);
	}

	this->hugging = false;
	this->huggingShell = NULL;
}

void CMario::SitDown() {
	if (level == MARIO_LEVEL_SMALL)
		return;
	this->vx = 0;
	this->ax = 0;
	this->vy = 0;
	sitting = true;
	y += MARIO_BIG_BBOX_HEIGHT - MARIO_SITTING_BBOX_HEIGHT;
}

void CMario::StandUp() {
	if (sitting == false)
		return;
	if (level == MARIO_LEVEL_SMALL)
		return;
	sitting = false;
	y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SITTING_BBOX_HEIGHT;
	y -= 1.0f;
}

void CMario::LvlUp() {
	switch (this->level) {
	case MARIO_LEVEL_SMALL:
		this->SetLevel(MARIO_LEVEL_BIG);
		y = y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT);
		y -= 2.0f;
		break;
	case MARIO_LEVEL_BIG:
		this->SetLevel(MARIO_LEVEL_RACCOON);
		y -= 2.0f;
		break;
	case MARIO_LEVEL_RACCOON:
		this->SetLevel(MARIO_LEVEL_FIRE);
		y -= 2.0f;
		break;
	}
	CBackup::GetInstance()->SetMarioLevel(level);
	ShowMarioTransformEffect();
	StartTransform();
}

void CMario::lvlDown() {
	StandUp();
	switch (level) {
	case MARIO_LEVEL_SMALL:
	{
		this->SetState(MARIO_STATE_DIE);
	}
	case MARIO_LEVEL_BIG:
		SetLevel(MARIO_LEVEL_SMALL);
		StartUntouchable();
		ShowMarioTransformEffect();
		StartTransform();
		break;
	case MARIO_LEVEL_RACCOON:
		SetLevel(MARIO_LEVEL_BIG);
		StartUntouchable();
		ShowMarioTransformEffect();
		StartTransform();
		break;
	case MARIO_LEVEL_FIRE:
		SetLevel(MARIO_LEVEL_BIG);
		StartUntouchable();
		ShowMarioTransformEffect();
		StartTransform();
		break;
	}
	CBackup::GetInstance()->SetMarioLevel(level);
}

void CMario::Spin() {
	if (level == MARIO_LEVEL_RACCOON && GetTickCount64() - spin_start >= MARIO_SPIN_TIME + MARIO_SPIN_COOLDOWN_TIME) {
		spinning = true;
		spin_start = GetTickCount64();
	}
}

void CMario::Shot() {
	if (GetTickCount64() - lastShotTime >= MARIO_SHOT_COOLDOWN_TIME && this->level == MARIO_LEVEL_FIRE) {
		lastShotTime = GetTickCount64();
		bullet = new CMarioFireBullet();
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		bullet->SetAnimationSet(animation_sets->Get(MARIO_FIRE_BULLET_ANI_SET_ID));
		if (nx > 0) {
			bullet->SetPosition(this->x + 7.0f, this->y + 5.0f);
			bullet->SetState(BULLET_STATE_WALKING_RIGHT);
		}
		else {
			bullet->SetPosition(this->x - 7.0f, this->y + 5.0f);
			bullet->SetState(BULLET_STATE_WALKING_LEFT);
		}
		bullets.push_back(bullet);
	}
}

void CMario::Fly() {
	if (this->level == MARIO_LEVEL_RACCOON) {
		if (runSpeedStack == 7)
		{
			flyIng = true;
			canJump = false;
			fly_start = GetTickCount64();
			vy = -MARIO_FLY_SPEED;
		}
		else if (!isOnGround && !flyIng)
		{
			fallIng = true;
			vy = MARIO_FALL_SPEED;
		}
	}
}

void CMario::Jump() {
	isOnGround = false;
	if (level == MARIO_LEVEL_SMALL)
		vy = -MARIO_JUMP_SPEED_Y_WEAK;
	else
		vy = -MARIO_JUMP_SPEED_Y_STRONG;
}

void CMario::BasicCollision(float min_tx, float min_ty, float nx, float ny, float x0, float y0)
{
	if (nx != 0)
	{
		this->vx = 0;
		this->x = x0 + min_tx * this->dx + nx * 0.4f;
	}
	if (ny != 0)
	{
		this->vy = 0;
		this->y = y0 + min_ty * this->dy + ny * 0.4f;
	}
}

void CMario::Render()
{
	for (int i = 0; i < bullets.size(); i++) {
		if (!bullets[i]->isDisable)
			bullets[i]->Render();
	}

	if (transforming)
		return;

	int alpha = 255;
	if (untouchable) alpha = 128;

	int ani;
	if (state == MARIO_STATE_DIE) {
		ani = MARIO_ANI_DIE;
		animation_set->at(ani)->Render(x, y, alpha);
	}
	else
	{
		switch (level) {
		case MARIO_LEVEL_SMALL:
			if (pipeWalking) {
				ani = MARIO_ANI_SMALL_PIPE_WALKING;
				animation_set->at(ani)->Render(x, y, alpha);
			}
			else
			{
				if (vy < 0) {
					if (nx > 0)
						ani = MARIO_ANI_SMALL_JUMP_RIGHT;
					else
						ani = MARIO_ANI_SMALL_JUMP_LEFT;
				}
				else {
					if (vx == 0)
					{
						if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
						else ani = MARIO_ANI_SMALL_IDLE_LEFT;
					}
					else if (vx > 0)
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
					else ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
				animation_set->at(ani)->Render(x, y, alpha);
			}
			break;
		case MARIO_LEVEL_BIG:
			if (pipeWalking) {
				ani = MARIO_ANI_BIG_PIPE_WALKING;
				animation_set->at(ani)->Render(x, y, alpha);
			}
			else if (sitting) {
				if (nx > 0) {
					ani = MARIO_ANI_BIG_SIT_RIGHT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
				else {
					ani = MARIO_ANI_BIG_SIT_LEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
			}
			else
			{
				if (vy < 0) {
					if (nx > 0)
						ani = MARIO_ANI_BIG_JUMP_RIGHT;
					else
						ani = MARIO_ANI_BIG_JUMP_LEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
				else {
					if (vx == 0)
					{
						if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
						else ani = MARIO_ANI_BIG_IDLE_LEFT;
						animation_set->at(ani)->Render(x, y, alpha);
					}
					else if (vx > 0) {
						if (nx < 0) {
							ani = MARIO_ANI_BIG_DRIFF_RIGHT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							if (vx == MARIO_RUN_SPEED) {
								ani = MARIO_ANI_BIG_RUN_RIGHT;
								animation_set->at(ani)->Render(x - 3.0f, y, alpha);
							}
							else {
								ani = MARIO_ANI_BIG_WALKING_RIGHT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
						}
					}
					else {
						if (nx > 0) {
							ani = MARIO_ANI_BIG_DRIFF_LEFT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							if (vx == -MARIO_RUN_SPEED) {
								ani = MARIO_ANI_BIG_RUN_LEFT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
							else {
								ani = MARIO_ANI_BIG_WALKING_LEFT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
						}
					}
				}
			}
			break;
		case MARIO_LEVEL_RACCOON:
			if (pipeWalking) {
				ani = MARIO_ANI_RACCOON_PIPE_WALKING;
				animation_set->at(ani)->Render(x, y, alpha);
			}
			else if (sitting) {
				if (nx > 0) {
					ani = MARIO_ANI_RACCOON_SIT_RIGHT;
					animation_set->at(ani)->Render(x - 7.0f, y, alpha);
				}
				else {
					ani = MARIO_ANI_RACCOON_SIT_LEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
			}
			else if (flyIng)
			{
				if (nx > 0) {
					ani = MARIO_ANI_RACCOON_FLY_RIGHT;
					animation_set->at(ani)->Render(x - 8.0f, y, alpha);
				}					
				else {
					ani = MARIO_ANI_RACCOON_FLY_LEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
			}
			else if (spinning)
			{
				if (nx > 0) {
					ani = MARIO_ANI_RACCOON_SPIN_RIGHT;
					animation_set->at(ani)->Render(x - 7.0f, y, alpha);
				}
				else {
					ani = MARIO_ANI_RACCOON_SPIN_LEFT;
					animation_set->at(ani)->Render(x - 7.0f, y, alpha);
				}
			}
			else if (fallIng)
			{
				if (nx > 0) {
					ani = MARIO_ANI_RACCOON_FALL_RIGHT;
					animation_set->at(ani)->Render(x - 8.0f, y, alpha);
				}
				else {
					ani = MARIO_ANI_RACCOON_FALL_LEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
			}
			else
			{
				if (vy < 0)
				{
					if (nx > 0) {
						ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
						animation_set->at(ani)->Render(x - 7.0f, y, alpha);
					}
					else {
						ani = MARIO_ANI_RACCOON_JUMP_LEFT;
						animation_set->at(ani)->Render(x, y, alpha);
					}
				}
				else {
					if (vx == 0)
					{
						if (nx > 0) {
							ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
							animation_set->at(ani)->Render(x - 7.0f, y, alpha);
						}
						else {
							ani = MARIO_ANI_RACCOON_IDLE_LEFT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
					}
					else if (vx > 0) {
						if (nx < 0) {
							ani = MARIO_ANI_RACCOON_DRIFF_RIGHT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							if (runSpeedStack == 7) {
								ani = MARIO_ANI_RACCOON_RUN_RIGHT;
								animation_set->at(ani)->Render(x - 8.0f, y, alpha);
							}
							else {
								ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
								animation_set->at(ani)->Render(x - 7.0f, y, alpha);
							}

						}
					}
					else {
						if (nx > 0) {
							ani = MARIO_ANI_RACCOON_DRIFF_LEFT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							if (runSpeedStack == 7) {
								ani = MARIO_ANI_RACCOON_RUN_LEFT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
							else {
								ani = MARIO_ANI_RACCOON_WALKING_LEFT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
						}
					}
				}
			}
			break;
		case MARIO_LEVEL_FIRE:
			if (pipeWalking) {
				ani = MARIO_ANI_FIRE_PIPE_WALKING;
				animation_set->at(ani)->Render(x, y, alpha);
			}
			else if (sitting) {
				if (nx > 0) {
					ani = MARIO_ANI_FIRE_SIT_RIGHT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
				else {
					ani = MARIO_ANI_FIRE_SIT_LEFT;
					animation_set->at(ani)->Render(x, y, alpha);
				}
			}
			else
			{
				if (vy < 0) {
					if (nx > 0) {
						ani = MARIO_ANI_FIRE_JUMP_RIGHT;
						animation_set->at(ani)->Render(x, y, alpha);
					}
					else {
						ani = MARIO_ANI_FIRE_JUMP_LEFT;
						animation_set->at(ani)->Render(x, y, alpha);
					}
				}
				else {
					if (vx == 0)
					{
						if (nx > 0) {
							ani = MARIO_ANI_FIRE_IDLE_RIGHT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							ani = MARIO_ANI_FIRE_IDLE_LEFT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
					}
					else if (vx > 0) {
						if (nx < 0) {
							ani = MARIO_ANI_FIRE_DRIFF_RIGHT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							if (vx == MARIO_RUN_SPEED) {
								ani = MARIO_ANI_FIRE_RUN_RIGHT;
								animation_set->at(ani)->Render(x - 3.0f, y, alpha);
							}
							else {
								ani = MARIO_ANI_FIRE_WALKING_RIGHT;
								animation_set->at(ani)->Render(x, y, alpha);
							}

						}
					}
					else {
						if (nx > 0) {
							ani = MARIO_ANI_FIRE_DRIFF_LEFT;
							animation_set->at(ani)->Render(x, y, alpha);
						}
						else {
							if (vx == -MARIO_RUN_SPEED) {
								ani = MARIO_ANI_FIRE_RUN_LEFT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
							else {
								ani = MARIO_ANI_FIRE_WALKING_LEFT;
								animation_set->at(ani)->Render(x, y, alpha);
							}
						}
					}
				}
			}
			break;
		}
	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_RUN_RIGHT:
		ax = MARIO_ACCELERATION;
		if (walkingRight == false)
		{
			vx += MARIO_WALKING_SPEED_BASE;
			walkingRight = true;
			walkingLeft = false;
		}
		nx = 1;
		break;
	case MARIO_STATE_RUN_LEFT:
		ax = -MARIO_ACCELERATION;
		if (walkingLeft == false)
		{
			vx += -MARIO_WALKING_SPEED_BASE;
			walkingLeft = true;
			walkingRight = false;
		}
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		ax = MARIO_ACCELERATION;
		if (walkingRight == false)
		{
			vx += MARIO_WALKING_SPEED_BASE;
			walkingRight = true;
			walkingLeft = false;
		}
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		ax = -MARIO_ACCELERATION;
		if (walkingLeft == false)
		{
			vx += -MARIO_WALKING_SPEED_BASE;
			walkingLeft = true;
			walkingRight = false;
		}
		nx = -1;
		break;
	case MARIO_STATE_IDLE:
		walkingLeft = false;
		walkingRight = false;
		if (isOnGround == true)
			if (vx > 0)
				ax = -MARIO_WALKING_FRICTION;
			else if (vx < 0)
				ax = MARIO_WALKING_FRICTION;
			else ax = 0;
		break;
	case MARIO_STATE_DIE:
		this->SetLevel(MARIO_LEVEL_SMALL);
		CBackup::GetInstance()->PopLifeStack();
		CBackup* backup = CBackup::GetInstance();
		backup->SetMarioLevel(level);
		backup->SetCurPos(backup->GetmWlastPosX(), backup->GetmWlastPosY());
		backup->SetCurControl(backup->GetmWlastCanLeft(), backup->GetmWlastCanTop(), backup->GetmWlastCanRight(), backup->GetmWlastCanBot());
		vx = 0;
		ax = 0;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		flyIng = false;
		fallIng = false;
		ReadyOutScene();
		break;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	switch (level) {
	case MARIO_LEVEL_SMALL:
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		break;
	case MARIO_LEVEL_BIG:
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (sitting)
			bottom = y + MARIO_SITTING_BBOX_HEIGHT;
		break;
	case MARIO_LEVEL_RACCOON:
		right = x + MARIO_RACCOON_BBOX_WIDTH;
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
		if (sitting)
			bottom = y + MARIO_SITTING_BBOX_HEIGHT;
		break;
	case MARIO_LEVEL_FIRE:
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
		if (sitting)
			bottom = y + MARIO_SITTING_BBOX_HEIGHT;
		break;
	}
}

