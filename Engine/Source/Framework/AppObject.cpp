#include "AppObject.h"
#include "Application.h"

void AppObject::OnUpdate(const Tick& tick)
{
	Super::OnUpdate(tick);

	//set last tick
	SetLastTick(tick);
}

void AppObject::OnConstruct()
{
	Super::OnConstruct();

}
