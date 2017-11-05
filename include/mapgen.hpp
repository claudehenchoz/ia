#ifndef MAPBUILD_HPP
#define MAPBUILD_HPP

#include <vector>

#include "map.hpp"
#include "map_templates.hpp"

class Room;

struct Region
{
public:
    Region(const R& r) :
        main_room   (nullptr),
        r           (r),
        is_free     (true) {}

    Region() :
        main_room   (nullptr),
        r           (),
        is_free     (true) {}

    R rnd_room_rect() const;

    Room* main_room;
    R r;
    bool is_free;
};

namespace mapgen
{

// This variable is checked at certain points to see if the current map
// has been flagged as "failed". Setting is_map_valid to false will generally
// stop map generation, discard the map, and trigger generation of a new map.
extern bool is_map_valid;

// All cells marked as true in this array will be considered for door placement
extern bool door_proposals[map_w][map_h];

//------------------------------------------------------------------------------
// Generate maps
//------------------------------------------------------------------------------
// Standard dungeon level
bool mk_std_lvl();

// Hand crafted levels
bool mk_intro_lvl();
bool mk_egypt_lvl();
bool mk_leng_lvl();
bool mk_rat_cave_level();
bool mk_trapez_lvl();
bool mk_boss_lvl();

//------------------------------------------------------------------------------
// Map generation steps (in no particular order)
//------------------------------------------------------------------------------
void merge_regions(Region regions[3][3]);

void mk_aux_rooms(Region regions[3][3]);

void reserve_river(Region regions[3][3]);

void mk_sub_rooms();

void decorate();

void mk_doors();

void mk_metal_doors_and_levers();

void mk_monoliths();

void mk_pylons_and_levers();

//------------------------------------------------------------------------------
// Room reshaping utils (called by the room objects)
//------------------------------------------------------------------------------
// NOTE: Some reshape functions below will not change the boundaries of the
//       room, but may affect which cells belong to the room. This only affects
//       the room map (in the "map" namespace), so the room parameter should be
//       a const reference. For other reshape functions, the room may expand
//       beyond its initial rectangle, so in those cases the functions need to
//       modify the data of the room object.
void cut_room_corners(const Room& room);
void mk_pillars_in_room(const Room& room);
void cavify_room(Room& room);

//------------------------------------------------------------------------------
// Room creation
//------------------------------------------------------------------------------
// NOTE: All "mk_room..." functions are "complete", i.e. they handle all the
//       necessary steps such as creating floor on the map, creating room
//       objects and registering them, et c. No such actions are needed by the
//       clients.
Room* mk_room(Region& region);

Room* mk_room(const R& r, const IsSubRoom is_sub_room);

// Low level functions related to room creation - these are only necessary when
// creating rooms by other methods than the "mk_room" functions above.
void register_room(Room& room);

void mk_floor(const Room& room);

//------------------------------------------------------------------------------
// Misc utils
//------------------------------------------------------------------------------
void valid_corridor_entries(const Room& room,
                            std::vector<P>& out);

bool is_choke_point(const P& p,
                    const bool blocked[map_w][map_h],
                    ChokePointData* out);

void mk_pathfind_corridor(Room& r0,
                          Room& r1,
                          bool door_proposals[map_w][map_h] = nullptr);

void rnd_walk(const P& p0,
              int len,
              std::vector<P>& pos_list_ref,
              const bool allow_diagonal = true,
              R area = R(1, 1, map_w - 2, map_h - 2));

void pathfinder_walk(const P& p0,
                     const P& p1,
                     std::vector<P>& pos_list_ref,
                     const bool is_smooth);

// Generates a map of spawn chance weights, with emphasis on hidden, optional,
// or hard to reach areas - this can be used e.g. to place items or levers.
void mk_explore_spawn_weights(const bool blocked[map_w][map_h],
                              std::vector<P>& positions_out,
                              std::vector<int>& weights_out);

} // mapgen

#endif // MAPBUILD_HPP
