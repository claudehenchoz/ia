#ifndef ACTOR_DATA_HPP
#define ACTOR_DATA_HPP

#include <string>
#include <vector>

#include "ability_values.hpp"
#include "global.hpp"
#include "properties.hpp"
#include "audio.hpp"
#include "room.hpp"
#include "spells.hpp"

enum class ActorId
{
    player,
    zombie,
    zombie_axe,
    bloated_zombie,
    crawling_intestines,
    crawling_hand,
    thing,
    floating_skull,
    cultist,
    cultist_priest,
    cultist_wizard,
    cultist_arch_wizard,
    bog_tcher,
    rat,
    rat_thing,
    green_spider,
    white_spider,
    red_spider,
    shadow_spider,
    leng_spider,
    pit_viper,
    spitting_cobra,
    black_mamba,
    mi_go,
    mi_go_commander,
    flying_polyp,
    greater_polyp,
    mind_leech,
    spirit_leech,
    life_leech,
    ghoul,
    shadow,
    invis_stalker,
    wolf,
    fire_hound,
    energy_hound,
    zuul,
    ghost,
    phantasm,
    wraith,
    void_traveler,
    elder_void_traveler,
    raven,
    giant_bat,
    vampire_bat,
    abaxu, // Unique bat
    byakhee,
    giant_mantis,
    locust,
    mummy,
    croc_head_mummy,
    khephren,
    nitokris,
    deep_one,
    ape,
    keziah_mason,
    brown_jenkin,
    leng_elder,
    major_clapham_lee,
    dean_halsey,
    worm_mass,
    mind_worms,
    dust_vortex,
    fire_vortex,
    energy_vortex,
    ooze_black,
    ooze_clear,
    ooze_putrid,
    ooze_poison,
    strange_color,
    chthonian,
    hunting_horror,
    sentry_drone,
    animated_wpn,
    mold,
    gas_spore,
    death_fiend,
    the_high_priest,
    high_priest_guard_war_vet,
    high_priest_guard_rogue,
    high_priest_guard_ghoul,

    END
};

enum class MonGroupSize
{
    alone,
    few,
    pack,
    swarm
};

// Each actor data entry has a list of this struct, this is used for choosing
// group sizes when spawning monsters. The size of the group spawned is
// determined by a weighted random choice (so that a certain monster could for
// example usually spawn alone, but on some rare occasions spawn in big groups).
struct MonGroupSpawnRule
{
    MonGroupSpawnRule() :
        group_size  (MonGroupSize::alone),
        weight      (1) {}

    MonGroupSpawnRule(MonGroupSize group_size, int weight) :
        group_size  (group_size),
        weight      (weight) {}

    MonGroupSize group_size;
    int weight;
};

enum class ActorSpeed
{
    sluggish    = 30,
    slow        = 80,
    normal      = 100,
    fast        = 150,
    fastest     = 300,
    END
};

enum class ActorErraticFreq
{
    never       = 0,
    rare        = 5,
    somewhat    = 15,
    very        = 35
};

enum class ActorSize
{
    undefined,
    floor,
    humanoid,
    giant
};

enum class AiId
{
    looks,
    makes_room_for_friend,
    attacks,
    paths_to_tgt_when_aware,
    moves_to_tgt_when_los,
    moves_to_lair,
    moves_to_leader,
    moves_to_random_when_unaware,
    END
};

struct ActorDataT
{
    ActorDataT()
    {
        reset();
    }

    void reset();

    ActorId id;
    std::string name_a;
    std::string name_the;
    std::string corpse_name_a;
    std::string corpse_name_the;
    TileId tile;
    char glyph;
    Clr color;
    std::vector<MonGroupSpawnRule> group_sizes;
    int hp, spi, dmg_melee, dmg_ranged;
    int speed_pct;
    AbilityVals ability_vals;
    bool natural_props[(size_t)PropId::END];
    int ranged_cooldown_turns;
    bool ai[(size_t)AiId::END];
    int nr_turns_aware;
    int spawn_min_dlvl, spawn_max_dlvl;
    ActorSize actor_size;
    bool is_humanoid;
    bool is_auto_descr_allowed;
    int nr_kills;
    bool has_player_seen;
    bool can_open_doors, can_bash_doors;
    //
    // NOTE: Knockback may be prevented even if this is false, e.g. if monstes
    //       is ethereal
    //
    bool prevent_knockback;
    int nr_left_allowed_to_spawn;
    bool is_unique;
    bool is_auto_spawn_allowed;
    std::string descr;
    std::string wary_msg;
    std::string aggro_msg_mon_seen;
    std::string aggro_msg_mon_hidden;
    SfxId aggro_sfx_mon_seen;
    SfxId aggro_sfx_mon_hidden;
    std::string spell_cast_msg;
    ActorErraticFreq erratic_move_pct;
    ShockLvl mon_shock_lvl;
    bool is_rat;
    bool is_canine;
    bool is_spider;
    bool is_undead;
    bool is_ghost;
    bool is_snake;
    bool is_reptile;
    bool is_amphibian;
    bool can_be_summoned_by_mon;
    bool can_bleed;
    bool can_leave_corpse;
    bool prio_corpse_bash;
    std::vector<RoomType> native_rooms;
};

namespace actor_data
{

extern ActorDataT data[(size_t)ActorId::END];

void init();

void save();
void load();

} // actor_data

#endif // ACTOR_DATA_HPP
