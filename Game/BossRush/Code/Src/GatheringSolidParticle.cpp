#include "GatheringSolidParticle.hpp"
#include "Core/engine.hpp"
using namespace Solid;

GatheringSolidParticle::GatheringSolidParticle()
{
	
}

GatheringSolidParticle::~GatheringSolidParticle()
{
	
}

void GatheringSolidParticle::Init()
{
    if (isInit)
        return;
    isInit = true;
	Engine* engine = Engine::GetInstance();
	if (!engine->ecsManager.GotComponent<ParticleEffect>(gameObject->GetEntity()))
		effect = engine->ecsManager.AddComponent<ParticleEffect>(gameObject, ParticleEffect());
	else
		effect = &(engine->ecsManager.GetComponent<ParticleEffect>(gameObject->GetEntity()));
	effect->SetTex(engine->graphicsResourceMgr.GetTexture("Boss_042_basecolor.jpg"));
	effect->particlesSize = 1.5f;
	effect->SetEmitRate(0.f);
	effect->SetCount(300);
	effect->boxPosGen = nullptr;
	effect->spherePosGen = std::make_shared<SpherePosGen>();
	effect->spherePosGen->radX = 0.5f;
	effect->spherePosGen->radZ = 0.5f;
	effect->attractorUpdater = std::make_shared<AttractorUpdater>();
	effect->attractorUpdater->attractors.push_back({0.f,0.f,0.f,0.2f});
	effect->killerZoneUpdater = std::make_shared<KillerZoneUpdater>();
	effect->killerZoneUpdater->offset = {0.02f, 0.02f, 0.02f};
	effect->velGen->maxStartVel = {0.f,0.f,0.f,0.f};
	effect->velGen->minStartVel = {0.f,0.f,0.f,0.f};
    effect->colGen->minStartCol = {0.f,0.7f,0.7f,1.f};
    effect->colGen->maxStartCol = {0.f,1.0f,1.0f,1.f};
    effect->colGen->minEndCol = {1.f,1.0f,1.0f,0.f};
    effect->colGen->maxEndCol = {1.f,1.0f,1.0f,0.f};
	effect->UpdateEmitter();
	effect->UpdateSystem();
}

void GatheringSolidParticle::Update()
{
    if (!isInit)
        return;

    if (!gathering)
	{
	    autoDestroy += Time::DeltaTime();
        return;
    }

	if (gathering && currGather <= 0.f)
	{
		effect->SetEmitRate(170.f);
		effect->UpdateEmitter();
		currGather = 0.f;
	}

	currGather += Time::DeltaTime();
	if (currGather >= gatherTime)
	{
		effect->SetEmitRate(0.f);
		effect->UpdateEmitter();
		currGather = -1.f;
		gathering = false;
	}



};
void GatheringSolidParticle::FixedUpdate() {


};
void GatheringSolidParticle::LateUpdate() {


};

void GatheringSolidParticle::Destroy()
{
	Engine* engine = Engine::GetInstance();
	if (engine->ecsManager.GotComponent<ParticleEffect>(gameObject->GetEntity()))
		engine->ecsManager.RemoveComponent<ParticleEffect>(gameObject);
	isInit = false;
}