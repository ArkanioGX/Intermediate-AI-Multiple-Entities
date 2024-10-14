#include "GridActor.h"
#include "GridComponent.h"
#include "GridRendererComponent.h"

GridActor::GridActor()
{
	gc = new GridComponent(this);
	gr = new GridRendererComponent(this, gc);
}
