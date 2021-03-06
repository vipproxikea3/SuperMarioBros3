#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "PlayScene.h"
#include "Point.h"
#include "TailAttackEffect.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT object = coObjects->at(i);
		CScene* s = CGame::GetInstance()->GetCurrentScene();
		if (dynamic_cast<CPlayScene*>(s)) {
			if (!this->IsInCamera())
				continue;
		}

		LPCOLLISIONEVENT e = SweptAABBEx(object);

		if (e->t > 0 && e->t <= 1.0f)
		{
			float mleft, mtop, mright, mbottom;
			GetBoundingBox(mleft, mtop, mright, mbottom);
			float oleft, otop, obottom, oright;
			e->obj->GetBoundingBox(oleft, otop, oright, obottom);
			if (e->nx != 0)
			{
				if (ceil(mbottom) == otop)
				{
					continue;
				}
				if (ceil(mtop) == obottom)
				{
					continue;
				}
			}
			if (e->ny != 0)
			{
				if (ceil(mleft) == oright || floor(mright) == oleft)
					continue;
			}
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny > 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);

	float min_ty0 = min_ty;
	min_ty = 1.0f;
	min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_ty && c->ny < 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}
	if (min_ty == 1.0f)
		min_ty = min_ty0;
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::ShowPoint() {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(17);
	CPoint* point = new CPoint(1);
	point->SetPosition(x, y - 16.0f);
	point->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(point);
}

void CGameObject::ShowTailAttackEffect() {
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(27);
	CTailAttackEffect* effect = new CTailAttackEffect();
	effect->SetPosition(x, y);
	effect->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->PushBackObj(effect);
}

bool CGameObject::BeAttackByTail() {
	float ml, mt, mr, mb;
	float ol, ot, or , ob;

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario) return false;
	if (!mario->IsSpinning()) return false;

	if (mario->IsTailAttacked())
		return false;

	mario->GetBoundingBox(ml, mt, mr, mb);
	this->GetBoundingBox(ol, ot, or, ob);
	mt += 14.0f;
	if (mario->nx > 0)
		mr += 8.0f;
	else
		ml -= 8.0f;

	if (ol > mr || or < ml || ot > mb || ob < mt)
		return false;
	mario->SetTailAttacked(true);
	return true;
}

bool CGameObject::BeThrownAwayByBreakBlock(vector<LPGAMEOBJECT>* coObjects) {
	float kl, kt, kr, kb;
	GetBoundingBox(kl, kt, kr, kb);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT object = coObjects->at(i);
		if (!object->IsInCamera())
			continue;

		if (!dynamic_cast<CBreakBlock*>(object))
			continue;

		if (!dynamic_cast<CBreakBlock*>(object)->IsDamaging())
			continue;

		float ol, ot, or , ob;
		object->GetBoundingBox(ol, ot, or , ob);

		if (kb > ot - 2 && kb < ot)
		{
			if ((kl >= ol && kl <= or ) || (kr >= ol && kr <= or))
				return true;
		}
	}
	return false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(x, y, bbox, rect.left, rect.top, rect.right, rect.bottom, 32);
}


CGameObject::~CGameObject()
{

}