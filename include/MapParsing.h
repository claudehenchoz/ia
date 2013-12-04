#ifndef MAP_PARSING_H
#define MAP_PARSING_H

#include <vector>

#include "CommonTypes.h"
#include "Config.h"

class Engine;
class Cell;
class FeatureMob;
class Actor;

enum CellPredCheckEntity_t {
  cellPredCheckEntityActor,
  cellPredCheckEntityFeatureStatic,
  cellPredCheckEntityFeatureMob
};

class CellPred {
public:
  virtual bool isCheckingCells()          const = 0;
  virtual bool isCheckingMobFeatures()    const = 0;
  virtual bool isCheckingActors()         const = 0;
  virtual bool check(const Cell& c)       const = 0;
  virtual bool check(const FeatureMob& f) const = 0;
  virtual bool check(const Actor& a)      const = 0;
  const Engine* const eng;
protected:
  CellPred(Engine* engine) : eng(engine) {}
};

class CellPredBlocksVision : public CellPred {
public:
  CellPredBlocksVision(Engine* engine) : CellPred(engine) {}
  bool isCheckingCells()          const {return true;}
  bool isCheckingMobFeatures()    const {return true;}
  bool isCheckingActors()         const {return false;}
  bool check(const Cell& c)       const;
  bool check(const FeatureMob& f) const;
  bool check(const Actor& a)      const {(void)a; return false;}
};

class CellPredBlocksBodyType : public CellPred {
public:
  CellPredBlocksBodyType(BodyType_t bodyType, bool isActorsBlocking,
                         Engine* engine) :
    CellPred(engine), bodyType_(bodyType),
    IS_ACTORS_BLOCKING_(isActorsBlocking) {}
  bool isCheckingCells()          const {return true;}
  bool isCheckingMobFeatures()    const {return true;}
  bool isCheckingActors()         const {return IS_ACTORS_BLOCKING_;}
  bool check(const Cell& c)       const;
  bool check(const FeatureMob& f) const;
  bool check(const Actor& a)      const;
  const BodyType_t bodyType_;
  const bool IS_ACTORS_BLOCKING_;
};

class CellPredBlocksProjectiles : public CellPred {
public:
  CellPredBlocksProjectiles(Engine* engine) : CellPred(engine) {}
  bool isCheckingCells()          const {return true;}
  bool isCheckingMobFeatures()    const {return true;}
  bool isCheckingActors()         const {return false;}
  bool check(const Cell& c)       const;
  bool check(const FeatureMob& f) const;
  bool check(const Actor& a)      const {(void)a; return false;}
};

class CellPredLivingActorsAdjToPos : public CellPred {
public:
  CellPredLivingActorsAdjToPos(const Pos& pos, Engine* engine) :
    CellPred(engine), pos_(pos) {}
  bool isCheckingCells()          const {return false;}
  bool isCheckingMobFeatures()    const {return false;}
  bool isCheckingActors()         const {return true;}
  bool check(const Cell& c)       const {(void)c; return false;}
  bool check(const FeatureMob& f) const {(void)f; return false;}
  bool check(const Actor& a)      const;
  const Pos& pos_;
};

class CellPredBlocksItems : public CellPred {
public:
  CellPredBlocksItems(Engine* engine) : CellPred(engine) {}
  bool isCheckingCells()          const {return true;}
  bool isCheckingMobFeatures()    const {return true;}
  bool isCheckingActors()         const {return false;}
  bool check(const Cell& c)       const;
  bool check(const FeatureMob& f) const;
  bool check(const Actor& a)      const {(void)a; return false;}
};

enum MapParseWriteRule {
  mapParseWriteAlways,
  mapParseWriteOnlyFalse,
  mapParseWriteOnlyTrue
};

class MapParser {
public:
  MapParser() {};

  static void parse(const CellPred& predicate,
                    bool arrayOut[MAP_X_CELLS][MAP_Y_CELLS],
                    const MapParseWriteRule writeRule = mapParseWriteAlways);
};

//Function object for sorting stl containers by distance to origin
struct IsCloserToOrigin {
public:
  IsCloserToOrigin(const Pos& c, const Engine* engine) :
    c_(c), eng(engine) {
  }
  bool operator()(const Pos& c1, const Pos& c2);
  Pos c_;
  const Engine* eng;
};

class FloodFill {
public:
  FloodFill(Engine* engine) : eng(engine) {}

  void run(
    const Pos& origin, bool blockers[MAP_X_CELLS][MAP_Y_CELLS],
    int values[MAP_X_CELLS][MAP_Y_CELLS], int travelLimit,
    const Pos& target);

private:
  Engine* const eng;
};

class PathFinder {
public:
  PathFinder(Engine* engine) : eng(engine) {}

  void run(const Pos& origin, const Pos& target,
           bool blockers[MAP_X_CELLS][MAP_Y_CELLS],
           vector<Pos>& vectorToFill) const;
private:
  Engine* eng;
};


//  bool isCellNextToPlayer(const Pos& pos,
//                          const bool COUNT_SAME_CELL_AS_NEIGHBOUR) const;
//
//
//
//
//
//private:
//  Engine* eng;
//};

#endif
