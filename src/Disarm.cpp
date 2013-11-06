#include "Disarm.h"

#include "Engine.h"

#include "GameTime.h"
#include "Log.h"
#include "Renderer.h"
#include "ActorPlayer.h"
#include "Query.h"
#include "Map.h"
#include "FeatureTrap.h"

void Disarm::playerDisarm() const {
  eng->log->addMsg("Which direction? | space/esc to cancel", clrWhiteHigh);
  eng->renderer->drawMapAndInterface();
  const Pos disarmInPos(eng->player->pos + eng->query->direction());
  eng->log->clearLog();

  bool isDisarmableFound = false;

  Feature* const f = eng->map->featuresStatic[disarmInPos.x][disarmInPos.y];

  if(f->getId() == feature_trap) {
    Trap* const trap = dynamic_cast<Trap*>(f);
    if(trap->isHidden() == false) {
      isDisarmableFound = true;
      trap->trigger(eng->player);
      eng->gameTime->endTurnOfCurrentActor();
    }
  }

  if(isDisarmableFound == false) {
    eng->log->addMsg("I see nothing there that I can disarm.");
  }
}
