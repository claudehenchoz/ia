#include "sound.hpp"

#include <iostream>
#include <string>

#include "feature_rigid.hpp"
#include "map.hpp"
#include "actor_player.hpp"
#include "actor_mon.hpp"
#include "game_time.hpp"
#include "map_parsing.hpp"

// -----------------------------------------------------------------------------
// Sound
// -----------------------------------------------------------------------------
Snd::Snd(
    const std::string& msg,
    const SfxId sfx,
    const IgnoreMsgIfOriginSeen ignore_msg_if_origin_seen,
    const P& origin,
    Actor* const actor_who_made_sound,
    const SndVol vol,
    const AlertsMon alerting_mon,
    const MorePromptOnMsg add_more_prompt_on_msg,
    std::shared_ptr<SndHeardEffect> snd_heard_effect) :
    msg_(msg),
    sfx_(sfx),
    is_msg_ignored_if_origin_seen_(ignore_msg_if_origin_seen),
    origin_(origin),
    actor_who_made_sound_(actor_who_made_sound),
    vol_(vol),
    is_alerting_mon_(alerting_mon),
    add_more_prompt_on_msg_(add_more_prompt_on_msg),
    snd_heard_effect_(snd_heard_effect) {}

Snd::~Snd()
{

}

void Snd::run()
{
    snd_emit::run(*this);
}

void Snd::on_heard(Actor& actor) const
{
    if (snd_heard_effect_)
    {
        snd_heard_effect_->run(actor);
    }
}

// -----------------------------------------------------------------------------
// Sound emitting
// -----------------------------------------------------------------------------
namespace snd_emit
{

namespace
{

int nr_snd_msg_printed_current_turn_;

bool is_snd_heard_at_range(const int range, const Snd& snd)
{
    return range <= (snd.is_loud() ? snd_dist_loud : snd_dist_normal);
}

} // namespace

void reset_nr_snd_msg_printed_current_turn()
{
    nr_snd_msg_printed_current_turn_ = 0;
}

void run(Snd snd)
{
    ASSERT(snd.msg() != " ");

    bool blocked[map_w][map_h];

    map_parsers::BlocksSound()
        .run(blocked);

    const P& origin = snd.origin();

    // Never block the origin - we want to be able to run the sound from e.g. a
    // closing door, after it was closed (and we don't want this to depend on
    // the floodfill algorithm, so we explicitly set the origin to free here)
    blocked[origin.x][origin.y] = false;

    int flood[map_w][map_h];

    floodfill(origin,
              blocked,
              flood,
              999,
              P(-1, -1),
              true);

    flood[origin.x][origin.y] = 0;

    for (Actor* actor : game_time::actors)
    {
        const int flood_val_at_actor = flood[actor->pos.x][actor->pos.y];

        const bool is_origin_seen_by_player =
            map::cells[origin.x][origin.y].is_seen_by_player;

        if (!is_snd_heard_at_range(flood_val_at_actor, snd))
        {
            continue;
        }

        if (actor->is_player())
        {
            if (is_origin_seen_by_player &&
                snd.is_msg_ignored_if_origin_seen())
            {
                snd.clear_msg();
            }

            const P& player_pos = map::player->pos;

            if (!snd.msg().empty())
            {
                // Add a direction to the message (i.e. "(NW)", "(E)" , etc)
                if (player_pos != origin)
                {
                    std::string dir_str = "";

                    dir_utils::compass_dir_name(player_pos,
                                                origin,
                                                dir_str);

                    snd.add_string("(" + dir_str + ")");
                }
            }

            const int snd_max_dist =
                snd.is_loud() ?
                snd_dist_loud : snd_dist_normal;

            const int pct_dist = (flood_val_at_actor * 100) / snd_max_dist;

            const P offset = (origin - player_pos).signs();

            const Dir dir_to_origin = dir_utils::dir(offset);

            map::player->hear_sound(snd,
                                    is_origin_seen_by_player,
                                    dir_to_origin, pct_dist);
        }
        else // Not player
        {
            Mon* const mon = static_cast<Mon*>(actor);

            mon->hear_sound(snd);
        }
    }
}

} // snd_emit
