#include "game.hpp"

#include <vector>
#include <memory>

#include "init.hpp"
#include "popup.hpp"
#include "io.hpp"
#include "highscore.hpp"
#include "text_format.hpp"
#include "query.hpp"
#include "actor_player.hpp"
#include "msg_log.hpp"
#include "sdl_base.hpp"
#include "map.hpp"
#include "create_character.hpp"
#include "actor_mon.hpp"
#include "saving.hpp"
#include "status_lines.hpp"
#include "feature_rigid.hpp"
#include "feature_mob.hpp"
#include "feature_door.hpp"
#include "reload.hpp"
#include "wham.hpp"
#include "close.hpp"
#include "disarm.hpp"
#include "pickup.hpp"
#include "look.hpp"
#include "attack.hpp"
#include "throwing.hpp"
#include "marker.hpp"
#include "explosion.hpp"
#include "item_factory.hpp"
#include "actor_factory.hpp"
#include "player_spells.hpp"
#include "map_travel.hpp"
#include "manual.hpp"
#include "postmortem.hpp"
#include "character_descr.hpp"
#include "mapgen.hpp"
#include "actor_data.hpp"

namespace game
{

CellRenderData render_array[map_w][map_h];
CellRenderData render_array_no_actors[map_w][map_h];

namespace
{

int clvl_ = 0;
int xp_pct_ = 0;
int xp_accum_ = 0;
TimeData start_time_;

std::vector<HistoryEvent> history_events_;

void query_quit()
{
    const auto quit_choices = std::vector<std::string>
    {
        "Yes",
        "No"
    };

    const int quit_choice =
        popup::show_menu_msg(
            "Save and highscore are not kept.",
            quit_choices,
            "Quit the current game?");

    if (quit_choice == 0)
    {
        states::pop();

        init::cleanup_session();
    }
}

int lifebar_length(const Actor& actor)
{
    const int actor_hp = std::max(0, actor.hp());

    const int actor_hp_max = actor.hp_max(true);

    if (actor_hp < actor_hp_max)
    {
        int hp_percent = (actor_hp * 100) / actor_hp_max;

        return ((config::cell_px_w() - 2) * hp_percent) / 100;
    }

    return -1;
}

void draw_life_bar(const P& pos, const int length)
{
    if (length < 0)
    {
        return;
    }

    const P cell_dims(config::cell_px_w(), config::cell_px_h());

    const int w_green = length;

    const int w_bar_tot = cell_dims.x - 2;

    const int w_red = w_bar_tot - w_green;

    const P px_pos =
        io::px_pos_for_cell_in_panel(Panel::map,
                                     pos + P(0, 1)) - P(0, 2);

    const int x0_green = px_pos.x + 1;

    const int x0_red = x0_green + w_green;

    if (w_green > 0)
    {
        io::draw_line_hor(P(x0_green, px_pos.y),
                          w_green,
                          clr_green_lgt);
    }

    if (w_red > 0)
    {
        io::draw_line_hor(P(x0_red, px_pos.y),
                          w_red,
                          clr_red_lgt);
    }
}


} // namespace

void init()
{
    clvl_ = 1;
    xp_pct_ = 0;
    xp_accum_ = 0;

    history_events_.clear();

    for (int x = 0; x < map_w; ++x)
    {
        for (int y = 0; y < map_h; ++y)
        {
            render_array[x][y] = CellRenderData();
            render_array_no_actors[x][y] = CellRenderData();
        }
    }
}

void save()
{
    saving::put_int(clvl_);
    saving::put_int(xp_pct_);
    saving::put_int(xp_accum_);
    saving::put_int(start_time_.year_);
    saving::put_int(start_time_.month_);
    saving::put_int(start_time_.day_);
    saving::put_int(start_time_.hour_);
    saving::put_int(start_time_.minute_);
    saving::put_int(start_time_.second_);

    saving::put_int(history_events_.size());

    for (const HistoryEvent& event : history_events_)
    {
        saving::put_str(event.msg);
        saving::put_int(event.turn);
    }
}

void load()
{
    clvl_ = saving::get_int();
    xp_pct_ = saving::get_int();
    xp_accum_ = saving::get_int();
    start_time_.year_ = saving::get_int();
    start_time_.month_ = saving::get_int();
    start_time_.day_ = saving::get_int();
    start_time_.hour_ = saving::get_int();
    start_time_.minute_ = saving::get_int();
    start_time_.second_ = saving::get_int();

    const int nr_events = saving::get_int();

    for (int i = 0; i < nr_events; ++i)
    {
        const std::string msg = saving::get_str();
        const int turn = saving::get_int();

        history_events_.push_back({msg, turn});
    }
}

void handle_player_input(const InputData& input)
{
    msg_log::clear();

    switch (input.key)
    {

    //
    // Movement
    //
    case SDLK_RIGHT:
    case '6':
    case 'l':
    {
        if (input.is_shift_held)
        {
            map::player->move(Dir::up_right);
        }
        else if (input.is_ctrl_held)
        {
            map::player->move(Dir::down_right);
        }
        else
        {
            map::player->move(Dir::right);
        }
    }
    break;

    case SDLK_DOWN:
    case '2':
    case 'j':
    {
        map::player->move(Dir::down);
        return;
    }
    break;

    case SDLK_LEFT:
    case '4':
    case 'h':
    {
        if (input.is_shift_held)
        {
            map::player->move(Dir::up_left);
        }
        else if (input.is_ctrl_held)
        {
            map::player->move(Dir::down_left);
        }
        else
        {
            map::player->move(Dir::left);
        }
    }
    break;

    case SDLK_UP:
    case '8':
    case 'k':
    {
        map::player->move(Dir::up);
    }
    break;

    case SDLK_PAGEUP:
    case '9':
    case 'u':
        {
            map::player->move(Dir::up_right);
        }
        break;

    case SDLK_PAGEDOWN:
    case '3':
    case 'n':
    {
        map::player->move(Dir::down_right);
    }
    break;

    case SDLK_END:
    case '1':
    case 'b':
    {
        map::player->move(Dir::down_left);
    }
    break;

    case SDLK_HOME:
    case '7':
    case 'y':
    {
        map::player->move(Dir::up_left);
    }
    break;

    case '5':
    case '.':
    {
        if (player_bon::traits[(size_t)Trait::steady_aimer])
        {
            PropHandler& prop_hlr = map::player->prop_handler();

            PropAiming* aiming = nullptr;

            if (player_bon::traits[(size_t)Trait::sharpshooter])
            {
                aiming = static_cast<PropAiming*>(
                    prop_hlr.prop(PropId::aiming));

                if (aiming)
                {
                    ++aiming->nr_turns_aiming_;

                    aiming->set_nr_turns_left(aiming->nr_turns_left() + 1);
                }
            }

            if (!aiming)
            {
                aiming = new PropAiming(PropTurns::specific, 1);

                prop_hlr.apply(aiming);
            }
        }

        map::player->move(Dir::center);
    }
    break;

    //
    // Wait
    //
    case 's':
    {
        if (map::player->is_seeing_burning_feature())
        {
            msg_log::add(msg_fire_prevent_cmd);
        }
        else if (!map::player->seen_foes().empty())
        {
            msg_log::add(msg_mon_prevent_cmd);
        }
        else // We are allowed to wait
        {
            //
            // NOTE: We should not print any "wait" message here, since it will
            //       look weird in some cases - e.g. when the waiting is
            //       immediately interrupted by a message from rearranging
            //       pistol magazines.
            //

            //
            // NOTE: Waiting with 's' merely performs "move" into the center
            //       position a number of turns (i.e. the same as pressing '5')
            //
            const int turns_to_apply = 5;

            map::player->wait_turns_left = turns_to_apply - 1;

            game_time::tick();
        }
    }
    break;

    //
    // Manual
    //
    case '?':
    case SDLK_F1:
    {
        std::unique_ptr<State> browse_manual_state(new BrowseManual);

        states::push(std::move(browse_manual_state));
    }
    break;

    //
    // Options
    //
    case '=':
    {
        std::unique_ptr<State> config_state(new ConfigState);

        states::push(std::move(config_state));
    }
    break;

    //
    // Reload
    //
    case 'r':
    {
        Item* const wpn = map::player->inv().item_in_slot(SlotId::wpn);

        reload::try_reload(*map::player, wpn);
    }
    break;

    //
    // Kick
    //
    case 'w':
    {
        wham::run();
    }
    break;

    //
    // Close
    //
    case 'c':
    {
        close_door::player_try_close_or_jam();
    }
    break;

    //
    // Disarm
    //
    case 'd':
    {
        disarm::player_disarm();
    }
    break;

    //
    // Unload
    //
    case 'G':
    {
        item_pickup::try_unload_wpn_or_pickup_ammo();
    }
    break;

    //
    // Aim/fire
    //
    case 'f':
    {
        const bool is_allowed =
            map::player->prop_handler().
            allow_attack_ranged(Verbosity::verbose);

        if (is_allowed)
        {
            auto* const item = map::player->inv().item_in_slot(SlotId::wpn);

            if (item)
            {
                const ItemDataT& item_data = item->data();

                if (item_data.ranged.is_ranged_wpn)
                {
                    auto* wpn = static_cast<Wpn*>(item);

                    if ((wpn->nr_ammo_loaded_ >= 1) ||
                        item_data.ranged.has_infinite_ammo)
                    {
                        // Not enough health for Mi-go gun?

                        //
                        // TODO: This doesn't belong here - refactor
                        //
                        if (wpn->data().id == ItemId::mi_go_gun)
                        {
                            if (map::player->hp() <= mi_go_gun_hp_drained)
                            {
                                msg_log::add(
                                    "I don't have enough health to fire it.");

                                return;
                            }
                        }

                        std::unique_ptr<State> aim_state(
                            new Aiming(map::player->pos, *wpn));

                        states::push(std::move(aim_state));
                    }
                    // Not enough ammo loaded - auto reload?
                    else if (config::is_ranged_wpn_auto_reload())
                    {
                        reload::try_reload(*map::player, item);
                    }
                    else // Not enough ammo loaded, and auto reloading disabled
                    {
                        msg_log::add("There is no ammo loaded.");
                    }
                }
                else // Wielded item is not a ranged weapon
                {
                    msg_log::add("I am not wielding a firearm.");
                }
            }
            else // Not wielding any item
            {
                msg_log::add("I am not wielding a weapon.");
            }
        }
    }
    break;

    //
    // Get
    //
    case 'g':
    {
        const P& p = map::player->pos;

        Item* const item_at_player = map::cells[p.x][p.y].item;

        if (item_at_player &&
            item_at_player->data().id == ItemId::trapez)
        {
            game::add_history_event("Beheld The Shining Trapezohedron!");

            states::pop();

            //
            // Show victory text
            //
            game::win_game();

            //
            // Go to postmortem state
            //
            std::unique_ptr<State> postmortem_state(
                new PostmortemMenu(IsWin::yes));

            states::push(std::move(postmortem_state));

            return;
        }

        item_pickup::try_pick();
    }
    break;

    //
    // Inventory screen
    //
    case 'i':
    {
        std::unique_ptr<State> browse_inv(new BrowseInv);

        states::push(std::move(browse_inv));
    }
    break;

    //
    // Apply item
    //
    case 'a':
    {
        std::unique_ptr<State> apply_state(new Apply);

        states::push(std::move(apply_state));
    }
    break;

    //
    // Swap to prepared weapon
    //
    case 'z':
    {
        Inventory& inv = map::player->inv();

        Item* const wielded = inv.item_in_slot(SlotId::wpn);

        Item* const alt = inv.item_in_slot(SlotId::wpn_alt);

        if (wielded || alt)
        {
            const std::string alt_name_a =
                alt ?
                alt->name(ItemRefType::a) :
                "";

            // War veteran swaps instantly
            const bool is_instant = player_bon::bg() == Bg::war_vet;

            const std::string swift_str =
                is_instant ?
                "swiftly " :
                "";

            if (wielded)
            {
                if (alt)
                {
                    msg_log::add("I " +
                                 swift_str +
                                 "swap to " +
                                 alt_name_a +
                                 ".");
                }
                else // No current alt weapon
                {
                    const std::string name = wielded->name(ItemRefType::plain);

                    msg_log::add("I " +
                                 swift_str +
                                 "put away my " +
                                 name +
                                 ".");
                }
            }
            else // No current wielded item
            {
                msg_log::add("I " +
                             swift_str +
                             "wield " +
                             alt_name_a +
                             ".");
            }

            inv.swap_wielded_and_prepared();

            if (!is_instant)
            {
                game_time::tick();
            }
        }
        else // No wielded weapon and no alt weapon
        {
            msg_log::add("I have neither a wielded nor a prepared weapon.");
        }
    }
    break;

    //
    // Auto-walk
    //
    case 'e':
    {
        if (map::player->is_seeing_burning_feature())
        {
            msg_log::add(msg_fire_prevent_cmd);
        }
        else if (!map::player->seen_foes().empty())
        {
            msg_log::add(msg_mon_prevent_cmd);
        }
        else if (!map::player->prop_handler().allow_see())
        {
            msg_log::add("Not while blind.");
        }
        else if (map::player->has_prop(PropId::poisoned))
        {
            msg_log::add("Not while poisoned.");
        }
        else if (map::player->has_prop(PropId::confused))
        {
            msg_log::add("Not while confused.");
        }
        else // We are allowed to use auto-walk
        {
            msg_log::add("Which direction?" + cancel_info_str,
                         clr_white_lgt);

            const Dir input_dir = query::dir(AllowCenter::no);

            msg_log::clear();

            if (input_dir == Dir::END || input_dir == Dir::center)
            {
                // Invalid direction
                io::update_screen();
            }
            else // Valid direction
            {
                map::player->set_quick_move(input_dir);
            }

        }
    }
    break;

    //
    // Throw
    //
    case 't':
    {
        const Item* explosive = map::player->active_explosive;

        if (explosive)
        {
            std::unique_ptr<State> throwing_explosive(
                new ThrowingExplosive(map::player->pos, *explosive));

            states::push(std::move(throwing_explosive));
        }
        else // Not holding explosive - run throwing attack instead
        {
            if (!map::player->thrown_item)
            {
                msg_log::add("No item selected for throwing (press [T]).");

                return;
            }

            const bool is_allowed =
                map::player->prop_handler().
                allow_attack_ranged(Verbosity::verbose);

            if (is_allowed)
            {
                std::unique_ptr<State> throwing(
                    new Throwing(map::player->pos,
                                 *map::player->thrown_item));

                states::push(std::move(throwing));
            }
        }
    }
    break;

    //
    // Select item for throwing
    //
    case 'T':
    {
        std::unique_ptr<State> select_throw(new SelectThrow);

        states::push(std::move(select_throw));
    }
    break;

    //
    // View
    //
    case 'v':
    {
        if (map::player->prop_handler().allow_see())
        {
            std::unique_ptr<State> view_state(
                new Viewing(map::player->pos));

            states::push(std::move(view_state));
        }
        else // Cannot see
        {
            msg_log::add("I cannot see.");
        }
    }
    break;

    //
    // Auto melee
    //
    case SDLK_TAB:
    {
        map::player->auto_melee();
    }
    break;

    //
    // Cast spell / use power
    //
    case 'x':
    {
        std::unique_ptr<State> browse_spell_state(new BrowseSpell);

        states::push(std::move(browse_spell_state));
    }
    break;

    //
    // Character info
    //
    case '@':
    {
        //
        // TODO: Push character description state
        //
        std::unique_ptr<State> char_descr_state(new CharacterDescr);

        states::push(std::move(char_descr_state));
    }
    break;

    case 'm':
    {
        std::unique_ptr<State> msg_history_state(new MsgHistoryState);

        states::push(std::move(msg_history_state));
    }
    break;

    //
    // Make noise
    //
    case 'N':
    {
        if (player_bon::bg() == Bg::ghoul)
        {
            msg_log::add("I let out a chilling howl.");
        }
        else // Not ghoul
        {
            msg_log::add("I make some noise.");
        }


        Snd snd("",
                SfxId::END,
                IgnoreMsgIfOriginSeen::yes,
                map::player->pos,
                map::player,
                SndVol::low,
                AlertsMon::yes);

        snd_emit::run(snd);

        game_time::tick();
    }
    break;

    //
    // Menu
    //
    case SDLK_ESCAPE:
    {
        const std::vector<std::string> choices
        {
            "Options",
            "Tome of Wisdom",
            "Quit",
            "Cancel"
        };

        const int choice =
            popup::show_menu_msg("", choices);

        if (choice == 0)
        {
            //
            // Options
            //
            std::unique_ptr<State> config_state(new ConfigState);

            states::push(std::move(config_state));
        }
        else if (choice == 1)
        {
            //
            // Manual
            //
            std::unique_ptr<State> browse_manual_state(new BrowseManual);

            states::push(std::move(browse_manual_state));
        }
        else if (choice == 2)
        {
            //
            // Quit
            //
            query_quit();
        }
    }
    break;

    //
    // Quit
    //
    case 'Q':
    {
        query_quit();
    }
    break;

    //
    // Some cheat commands enabled in debug builds
    //
#ifndef NDEBUG
    case SDLK_F2:
    {
        map_travel::go_to_nxt();
    }
    break;

    case SDLK_F3:
    {
        game::incr_player_xp(100, Verbosity::silent);
    }
    break;

    case SDLK_F4:
    {
        if (init::is_cheat_vision_enabled)
        {
            for (int x = 0; x < map_w; ++x)
            {
                for (int y = 0; y < map_h; ++y)
                {
                    map::cells[x][y].is_seen_by_player = false;
                    map::cells[x][y].is_explored = false;
                }
            }

            init::is_cheat_vision_enabled = false;
        }
        else // Cheat vision was not enabled
        {
            init::is_cheat_vision_enabled = true;
        }

        map::player->update_fov();
    }
    break;

    case SDLK_F5:
    {
        map::player->incr_shock(50, ShockSrc::misc);
    }
    break;

    case SDLK_F6:
    {
        item_factory::mk_item_on_floor(ItemId::gas_mask, map::player->pos);

        for (size_t i = 0; i < (size_t)ItemId::END; ++i)
        {
            const auto& item_data = item_data::data[i];

            if (item_data.value != ItemValue::normal && item_data.allow_spawn)
            {
                item_factory::mk_item_on_floor(ItemId(i), map::player->pos);
            }
        }
    }
    break;

    case SDLK_F7:
    {
        map::player->teleport();
    }
    break;

    case SDLK_F8:
    {
        map::player->prop_handler().apply(
            new PropInfected(PropTurns::std));
    }
    break;

    case SDLK_F9:
    {
        const std::string query_str = "Summon monster id:";

        io::draw_text(query_str, Panel::screen, P(0, 0), clr_yellow);

        const int idx = query::number(P(query_str.size(), 0),
                                      clr_white_lgt,
                                      0,
                                      (int)ActorId::END,
                                      0,
                                      false);

        const ActorId mon_id = ActorId(idx);

        actor_factory::spawn(map::player->pos, {mon_id}, MakeMonAware::no);
    }
    break;
#endif // NDEBUG

    //
    // Undefined commands
    //
    default:
    {
        if (input.key != -1 && input.key != SDLK_RETURN)
        {
            std::string cmd_tried = " ";

            cmd_tried[0] = input.key;

            if (cmd_tried != " ")
            {
                msg_log::add("Unknown command '" + cmd_tried + "'.");
            }
        }
    }

    } // key switch
}

int clvl()
{
    return clvl_;
}

int xp_pct()
{
    return xp_pct_;
}

int xp_accumulated()
{
    return xp_accum_;
}

TimeData start_time()
{
    return start_time_;
}

void incr_player_xp(const int xp_gained,
                    const Verbosity verbosity)
{
    if (!map::player->is_alive())
    {
        return;
    }

    if (verbosity == Verbosity::verbose)
    {
        msg_log::add("(+" + std::to_string(xp_gained) + "% XP)");
    }

    xp_pct_ += xp_gained;

    xp_accum_ += xp_gained;

    while (xp_pct_ >= 100)
    {
        if (clvl_ < player_max_clvl)
        {
            ++clvl_;

            msg_log::add("Welcome to level " + std::to_string(clvl_) + "!",
                         clr_green,
                         false,
                         MorePromptOnMsg::yes);

            msg_log::more_prompt();

            {
                const int hp_gained = 1;

                map::player->change_max_hp(hp_gained,
                                           Verbosity::silent);

                map::player->restore_hp(hp_gained,
                                        false,
                                        Verbosity::silent);
            }

            {
                const int spi_gained = 1;

                map::player->change_max_spi(spi_gained,
                                            Verbosity::silent);

                map::player->restore_spi(spi_gained,
                                         false,
                                         Verbosity::silent);
            }

            std::unique_ptr<State> trait_state(new PickTraitState);

            states::push(std::move(trait_state));
        }

        xp_pct_ -= 100;
    }
}

void incr_clvl()
{
    ++clvl_;
}

void win_game()
{
    io::cover_panel(Panel::screen);
    io::update_screen();

    const std::vector<std::string> win_msg =
    {
        "As I approach the crystal, an eerie glow illuminates the area. "
        "I notice a figure observing me from the edge of the light. "
        "There is no doubt concerning the nature of this entity; "
        "it is the Faceless God who dwells in the depths of the earth - "
        "Nyarlathotep!",

        "I panic. Why is it I find myself here, stumbling around in darkness? "
        "Is this all part of a plan? "
        "The being beckons me to gaze into the stone.",

        "In the radiance I see visions beyond eternity, visions of unreal "
        "reality, visions of the brightest light of day and the darkest night "
        "of madness. There is only onward now, I have to see, I have to KNOW.",

        "So I make a pact with the Fiend.",

        "I now harness the shadows that stride from world to world to sow "
        "death and madness. "
        "The destinies of all things on earth, living and dead, are mine."
    };

    const int padding = 9;

    const int x0 = padding;

    const int max_w = map_w - (padding * 2);

    const int line_delay = 50;

    int y = 2;

    for (const std::string& section_msg : win_msg)
    {
        const auto section_lines = text_format::split(section_msg, max_w);

        for (const std::string& line : section_lines)
        {
            io::draw_text(line,
                          Panel::screen,
                          P(x0, y),
                          clr_white,
                          clr_black);

            io::update_screen();

            sdl_base::sleep(line_delay);

            ++y;
        }
        ++y;
    }

    ++y;

    const std::string cmd_label = "[space/esc/enter] to continue";

    io::draw_text_center(
        "[space/esc/enter] to continue",
        Panel::screen,
        P(map_w_half, screen_h - 2),
        clr_menu_drk,
        clr_black,
        false);

    io::update_screen();

    query::wait_for_confirm();
}

void on_mon_seen(Actor& actor)
{
    auto& d = actor.data();

    if (!d.has_player_seen)
    {
        d.has_player_seen = true;

        //
        // Give XP based on monster shock rating
        //
        int xp_gained = 0;

        switch (d.mon_shock_lvl)
        {
        case ShockLvl::unsettling:
            xp_gained = 3;
            break;

        case ShockLvl::frightening:
            xp_gained = 5;
            break;

        case ShockLvl::terrifying:
            xp_gained = 10;
            break;

        case ShockLvl::mind_shattering:
            xp_gained = 20;
            break;

        case ShockLvl::none:
        case ShockLvl::END:
            break;
        }

        if (xp_gained > 0)
        {
            const std::string name = actor.name_a();

            msg_log::add("I have discovered " + name + "!");

            incr_player_xp(xp_gained);

            msg_log::more_prompt();

            add_history_event("Discovered " + name + ".");

            //
            // We also cause some shock the first time
            //
            map::player->incr_shock(d.mon_shock_lvl,
                                    ShockSrc::see_mon);
        }
    }
}

void on_mon_killed(Actor& actor)
{
    ActorDataT& d = actor.data();

    d.nr_kills += 1;

    const int min_hp_for_sadism_bon = 4;

    if (d.hp >= min_hp_for_sadism_bon &&
        insanity::has_sympt(InsSymptId::sadism))
    {
        map::player->shock_ = std::max(0.0, map::player->shock_ - 3.0);
    }

    if (d.is_unique)
    {
        const std::string name = actor.name_the();

        add_history_event("Defeated " + name + ".");
    }
}

void add_history_event(const std::string msg)
{
    const int turn_nr = game_time::turn_nr();

    history_events_.push_back({msg, turn_nr});
}

const std::vector<HistoryEvent>& history()
{
    return history_events_;
}

} // game

// -----------------------------------------------------------------------------
// Game state
// -----------------------------------------------------------------------------
StateId GameState::id()
{
    return StateId::game;
}

void GameState::on_start()
{
    if (entry_mode_ == GameEntryMode::new_game)
    {
        // Some backgrounds and traits may have affected maximum hp and spi
        // (either positively or negatively), so here we need to (re)set the
        // current hp and spi to the maximum values
        map::player->set_hp_and_spi_to_max();

        map::player->mk_start_items();

        game::add_history_event("Started journey");

        if (!config::is_intro_lvl_skipped())
        {
            io::clear_screen();

            const std::string msg =
                "I stand on a cobbled forest path, ahead lies a shunned and "
                "decrepit old church. I know of the things that dwell below, "
                "and of the Cult of Starry Wisdom and the monstrous sacrifices "
                "dedicated to their overlords. But now they are only deranged "
                "fanatics grasping at false promises. I will enter these "
                "sprawling catacombs and rob them of treasures and knowledge! "
                "At the depths of the abyss lies my true destiny, an artifact "
                "of non-human origin called \"The shining Trapezohedron\" - "
                "a window to all the secrets of the universe. I can feel it "
                "calling me.";

            popup::show_msg(msg,
                            "The story so far...",
                            SfxId::END,
                            1);
        }
    }

    if (config::is_intro_lvl_skipped() ||
        (entry_mode_ == GameEntryMode::load_game))
    {
        // Build first/next dungeon level
        map_travel::go_to_nxt();
    }
    else
    {
        // Build forest.
        mapgen::mk_intro_lvl();
    }

    map::player->update_fov();

    game::start_time_ = current_time();
}

void GameState::draw()
{
    draw_map();

    status_lines::draw();

    msg_log::draw();
}

void GameState::update()
{
    // To avoid redrawing the map for *each* actor, we instead run acting inside
    // a loop here. We exit the loop if the next actor to act is the player.
    // Then another state cycle will be executed, and rendering performed.
    while (true)
    {
        //
        // Let the current actor act
        //
        Actor* actor = game_time::current_actor();

        const bool allow_act = actor->prop_handler().allow_act();

        const bool is_gibbed = actor->state() == ActorState::destroyed;

        if (allow_act && !is_gibbed)
        {
            // Tell actor to "do something". If this is the player, input is
            // read from either the player or the bot. If it's a monster, the
            // AI handles it.
            actor->act();
        }
        else // Actor cannot act
        {
            if (actor->is_player())
            {
                sdl_base::sleep(ms_delay_player_unable_act);
            }

            game_time::tick();
        }

        //
        // NOTE: This state may have been popped at this point
        //

        // We have quit the current game, or the player is dead?
        if (!map::player ||
            !states::contains_state(StateId::game) ||
            (map::player->state() != ActorState::alive))
        {
            break;
        }

        // Exit the loop if the next actor is the player (to trigger rendering).
        const Actor* next_actor = game_time::current_actor();

        if (next_actor->is_player())
        {
            break;
        }
    }

    // Player is dead?
    if (map::player && (map::player->state() != ActorState::alive))
    {
        TRACE << "Player died" << std::endl;

        static_cast<Player*>(map::player)->wait_turns_left = -1;

        audio::play(SfxId::death);

        msg_log::add("-I AM DEAD!-",
                     clr_msg_bad,
                     false,
                     MorePromptOnMsg::yes);

        //
        // Go to postmortem menu
        //
        states::pop();

        std::unique_ptr<State> postmortem_state(
            new PostmortemMenu(IsWin::no));

        states::push(std::move(postmortem_state));

        return;
    }
}

void GameState::draw_map()
{
    CellRenderData* render_data = nullptr;

    const bool is_tile_mode = config::is_tiles_mode();

    //--------------------------------------------------------------------------
    // Insert rigids and blood
    //--------------------------------------------------------------------------
    for (int x = 0; x < map_w; ++x)
    {
        for (int y = 0; y < map_h; ++y)
        {
            // Reset render data at this position
            game::render_array[x][y] = CellRenderData();

            render_data = &game::render_array[x][y];

            auto& cell = map::cells[x][y];

            if (cell.is_seen_by_player)
            {
                const auto* const f = cell.rigid;

                TileId gore_tile = TileId::empty;

                char gore_glyph = 0;

                if (f->can_have_gore())
                {
                    gore_tile = f->gore_tile();
                    gore_glyph = f->gore_glyph();
                }

                if (gore_tile == TileId::empty)
                {
                    render_data->tile = f->tile();
                    render_data->glyph = f->glyph();
                    render_data->clr = f->clr();

                    const Clr& feature_clr_bg = f->clr_bg();

                    if (!is_clr_equal(feature_clr_bg, clr_black))
                    {
                        render_data->clr_bg = feature_clr_bg;
                    }
                }
                else // Has gore
                {
                    render_data->tile = gore_tile;
                    render_data->glyph = gore_glyph;
                    render_data->clr = clr_red;
                }

                if (f->is_los_passable() &&
                    !f->is_bottomless() &&
                    cell.is_lit)
                {
                    render_data->mark_lit = true;
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    // Insert dead actors
    //--------------------------------------------------------------------------
    for (Actor* actor : game_time::actors)
    {
        const P& p(actor->pos);

        if (actor->is_corpse() &&
            actor->data().glyph != 0 &&
            actor->data().glyph != ' ' &&
            actor->data().tile != TileId::empty &&
            map::cells[p.x][p.y].is_seen_by_player)
        {
            render_data = &game::render_array[p.x][p.y];

            render_data->clr = actor->clr();
            render_data->tile = actor->tile();
            render_data->glyph = actor->glyph();
        }
    }

    for (int x = 0; x < map_w; ++x)
    {
        for (int y = 0; y < map_h; ++y)
        {
            render_data = &game::render_array[x][y];

            if (map::cells[x][y].is_seen_by_player)
            {
                //--------------------------------------------------------------
                // Insert items
                //--------------------------------------------------------------
                const Item* const item = map::cells[x][y].item;

                if (item)
                {
                    render_data->clr = item->clr();
                    render_data->tile = item->tile();
                    render_data->glyph = item->glyph();
                }

                // -------------------------------------------------------------
                // Copy array to player memory (before living actors and mobile
                // features)
                // -------------------------------------------------------------
                auto& cell = map::cells[x][y];

                cell.player_visual_memory = *render_data;

                game::render_array_no_actors[x][y] = *render_data;

                // -------------------------------------------------------------
                // Color cells slightly yellow when marked as lit
                // -------------------------------------------------------------
                if (render_data->mark_lit)
                {
                    render_data->clr.r = std::min(255, render_data->clr.r + 40);
                    render_data->clr.g = std::min(255, render_data->clr.g + 40);

                    render_data->is_light_fade_allowed = false;
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    // Insert mobile features
    //--------------------------------------------------------------------------
    for (auto* mob : game_time::mobs)
    {
        const P& p = mob->pos();

        const TileId mob_tile = mob->tile();

        const char mob_glyph = mob->glyph();

        if (mob_tile != TileId::empty &&
            mob_glyph != 0 &&
            mob_glyph != ' ' &&
            map::cells[p.x][p.y].is_seen_by_player)
        {
            render_data = &game::render_array[p.x][p.y];
            render_data->clr = mob->clr();
            render_data->tile = mob_tile;
            render_data->glyph = mob_glyph;
        }
    }

    //--------------------------------------------------------------------------
    // Insert living actors
    //--------------------------------------------------------------------------
    for (auto* actor : game_time::actors)
    {
        if (!actor->is_player() &&
            actor->is_alive())
        {
            // There is a living monster here

            const P& p = actor->pos;

            render_data = &game::render_array[p.x][p.y];

            // Sanity check: There should NOT already be an actor here which is
            // seen, or that we are aware of
            ASSERT(!render_data->is_living_actor_seen_here);
            ASSERT(!render_data->is_aware_of_hostile_mon_here);
            ASSERT(!render_data->is_aware_of_allied_mon_here);

            const auto* const mon = static_cast<const Mon*>(actor);

            if (map::player->can_see_actor(*actor))
            {
                if (actor->tile() != TileId::empty &&
                    actor->glyph() != 0 &&
                    actor->glyph() != ' ')
                {
                    render_data->clr = actor->clr();
                    render_data->tile = actor->tile();
                    render_data->glyph = actor->glyph();

                    render_data->is_living_actor_seen_here = true;
                    render_data->is_light_fade_allowed = false;

                    render_data->lifebar_length = game::lifebar_length(*actor);

                    if (map::player->is_leader_of(mon))
                    {
                        render_data->clr_bg = clr_allied_mon;
                    }
                    else // Player is not leader of monster
                    {
                        // If monster is unaware of the monster, draw it with a
                        // colored background
                        if (mon->aware_of_player_counter_ <= 0)
                        {
                            render_data->clr_bg = clr_blue;
                        }
                        else // Monster is aware of player
                        {
                            // If the monster has any negative properties, draw
                            // it with a colored background
                            const bool has_temporary_negative_prop =
                                actor->prop_handler()
                                .has_temporary_negative_prop_mon();

                            if (has_temporary_negative_prop)
                            {
                                render_data->clr_bg = clr_magenta;
                            }
                        }
                    }
                }
            }
            else // Player cannot see actor
            {
                if (mon->player_aware_of_me_counter_ > 0)
                {
                    if (map::player->is_leader_of(mon))
                    {
                        render_data->is_aware_of_allied_mon_here = true;
                    }
                    else // Player is not leader of monster
                    {
                        render_data->is_aware_of_hostile_mon_here = true;
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    // Do some "post processing", and draw the map
    //--------------------------------------------------------------------------
    const bool use_light_fade = config::use_light_fade_effect();

    for (int x = 0; x < map_w; ++x)
    {
        for (int y = 0; y < map_h; ++y)
        {
            render_data = &game::render_array[x][y];

            const Cell& cell = map::cells[x][y];

            if (cell.is_seen_by_player)
            {
                if (render_data->is_light_fade_allowed)
                {
                    // Fade light with distance?
                    if (use_light_fade)
                    {
                        const int dist_from_player =
                            king_dist(map::player->pos, P(x, y));

                        if (dist_from_player > 1)
                        {
                            double div =
                                1.0 +
                                ((double(dist_from_player - 1) * 0.33));

                            div = std::min(2.0, div);

                            div_clr(render_data->clr, div);
                            div_clr(render_data->clr_bg, div);
                        }
                    }

                    // Fade dark cells
                    if (!cell.rigid->is_bottomless() &&
                        cell.is_dark &&
                        !cell.is_lit)
                    {
                        const double div = 1.75;

                        div_clr(render_data->clr, div);
                        div_clr(render_data->clr_bg, div);
                    }
                }
            }
            else if (cell.is_explored &&
                     !render_data->is_living_actor_seen_here)
            {
                const bool is_aware_of_hostile_mon_here =
                    render_data->is_aware_of_hostile_mon_here;

                const bool is_aware_of_allied_mon_here =
                    render_data->is_aware_of_allied_mon_here;

                // Set render array and the temporary render data to the
                // remembered cell
                game::render_array[x][y] = cell.player_visual_memory;

                *render_data = cell.player_visual_memory;

                render_data->is_aware_of_hostile_mon_here =
                    is_aware_of_hostile_mon_here;

                render_data->is_aware_of_allied_mon_here =
                    is_aware_of_allied_mon_here;

                const double div = 3.0;

                div_clr(render_data->clr, div);
                div_clr(render_data->clr_bg, div);
            }

            if (is_tile_mode)
            {
                // Walls are given perspective here. If the tile to be set is a
                // (top) wall tile, instead place a front wall tile on any of
                // the current conditions:
                // (1) Cell below is explored, and its tile is not a front or
                //     top wall.
                // (2) Cell below is unexplored.
                if (!render_data->is_living_actor_seen_here &&
                    !render_data->is_aware_of_hostile_mon_here &&
                    !render_data->is_aware_of_allied_mon_here)
                {
                    const auto tile_seen =
                        game::render_array_no_actors[x][y].tile;

                    const auto tile_mem =
                        cell.player_visual_memory.tile;

                    const bool is_tile_wall =
                        cell.is_seen_by_player ?
                        Wall::is_tile_any_wall_top(tile_seen) :
                        Wall::is_tile_any_wall_top(tile_mem);

                    if (is_tile_wall)
                    {
                        const auto* const f = cell.rigid;

                        const auto feature_id = f->id();

                        bool is_hidden_door = false;

                        if (feature_id == FeatureId::door)
                        {
                            is_hidden_door =
                                static_cast<const Door*>(f)->is_secret();
                        }

                        if ((y < map_h - 1) &&
                            (feature_id == FeatureId::wall || is_hidden_door))
                        {
                            if (map::cells[x][y + 1].is_explored)
                            {
                                const bool is_seen_below =
                                    map::cells[x][y + 1].is_seen_by_player;

                                const auto tile_below_seen =
                                    game::render_array_no_actors[x][y + 1].tile;

                                const auto tile_below_mem =
                                    map::cells[x][y + 1].
                                    player_visual_memory.tile;

                                const bool tile_below_is_wall_front =
                                    is_seen_below ?
                                    Wall::is_tile_any_wall_front(tile_below_seen) :
                                    Wall::is_tile_any_wall_front(tile_below_mem);

                                const bool tile_below_is_wall_top =
                                    is_seen_below ?
                                    Wall::is_tile_any_wall_top(tile_below_seen) :
                                    Wall::is_tile_any_wall_top(tile_below_mem);

                                bool tile_below_is_revealed_door =
                                    is_seen_below ?
                                    Door::is_tile_any_door(tile_below_seen) :
                                    Door::is_tile_any_door(tile_below_mem);

                                if (tile_below_is_wall_front ||
                                    tile_below_is_wall_top ||
                                    tile_below_is_revealed_door)
                                {
                                    if (feature_id == FeatureId::wall)
                                    {
                                        const auto* const wall =
                                            static_cast<const Wall*>(f);

                                        render_data->tile =
                                            wall->top_wall_tile();
                                    }
                                }
                                else if (feature_id == FeatureId::wall)
                                {
                                    const auto* const wall =
                                        static_cast<const Wall*>(f);

                                    render_data->tile =
                                        wall->front_wall_tile();
                                }
                                else if (is_hidden_door)
                                {
                                    render_data->tile =
                                        config::is_tiles_wall_full_square() ?
                                        TileId::wall_top :
                                        TileId::wall_front;
                                }
                            }
                            else // Cell below is not explored
                            {
                                const Rigid* wall = nullptr;

                                if (is_hidden_door)
                                {
                                    wall = static_cast<const Door*>(f)->mimic();
                                }
                                else
                                {
                                    wall = f;
                                }

                                render_data->tile =
                                    static_cast<const Wall*>(wall)->
                                    front_wall_tile();
                            }
                        }
                    }
                }
            }

            const P pos(x, y);

            // Draw exclamation mark if aware of monster here
            if (render_data->is_aware_of_hostile_mon_here ||
                render_data->is_aware_of_allied_mon_here)
            {
                // We should never see both a hostile AND an allied monster in
                // the same cell
                ASSERT(!render_data->is_aware_of_hostile_mon_here ||
                       !render_data->is_aware_of_allied_mon_here);

                const Clr clr_bg =
                    render_data->is_aware_of_hostile_mon_here ?
                    clr_gray :
                    clr_allied_mon;

                render_data->tile = TileId::empty;
                render_data->glyph = '!';
                render_data->clr = clr_black;
                render_data->clr_bg = clr_bg;
            }

            bool did_draw = false;

            // Draw tile here if tile mode, and a tile has been set
            if (is_tile_mode && (render_data->tile != TileId::empty))
            {
                io::draw_tile(render_data->tile,
                              Panel::map,
                              pos,
                              render_data->clr,
                              render_data->clr_bg);

                did_draw = true;
            }
            // Text mode, or no tile set - draw glyph?
            else if ((render_data->glyph != 0) &&
                     (render_data->glyph != ' '))
            {
                //
                // NOTE: It can happen that text is drawn on the map even in
                //       tiles mode - for example exclamation marks on cells
                //       with known, unseen actors
                //

                io::draw_glyph(render_data->glyph,
                               Panel::map,
                               pos,
                               render_data->clr,
                               true,
                               render_data->clr_bg);

                did_draw = true;
            }

            // Draw lifebar here?
            if (did_draw &&
                render_data->lifebar_length != -1)
            {
                game::draw_life_bar(pos, render_data->lifebar_length);
            }

            // if (!cell.is_explored)
            // {
            //     game::render_array[x][y] = CellRenderData();
            // }
        }
    }

    //--------------------------------------------------------------------------
    // Draw player character
    //--------------------------------------------------------------------------
    const P& pos = map::player->pos;

    Item* item = map::player->inv().item_in_slot(SlotId::wpn);

    const bool is_ghoul = player_bon::bg() == Bg::ghoul;

    const Clr clr = map::player->clr();

    Clr clr_bg = clr_black;

    bool uses_ranged_wpn = false;

    if (item)
    {
        uses_ranged_wpn =
            item->data().ranged.is_ranged_wpn;
    }

    const TileId tile =
        is_ghoul ? TileId::ghoul :
        uses_ranged_wpn ? TileId::player_firearm :
        TileId::player_melee;

    const char glyph = '@';

    auto& player_render_data = game::render_array[pos.x][pos.y];

    player_render_data.tile = tile;
    player_render_data.glyph = glyph;
    player_render_data.clr = clr;
    player_render_data.clr_bg = clr_bg;

    if (is_tile_mode)
    {
        io::draw_tile(tile,
                      Panel::map,
                      pos,
                      clr,
                      clr_bg);
    }
    else // Text mode
    {
        io::draw_glyph(glyph,
                       Panel::map,
                       pos,
                       clr,
                       true,
                       clr_bg);
    }

    const int life_bar_length = game::lifebar_length(*map::player);

    if (life_bar_length != -1)
    {
        game::draw_life_bar(pos, life_bar_length);
    }

    //
    // TODO: Reimplement somehow
    //
    // draw_player_shock_excl_marks();
}
