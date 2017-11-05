#include "properties.hpp"

#include <algorithm>

#include "init.hpp"
#include "actor_player.hpp"
#include "msg_log.hpp"
#include "postmortem.hpp"
#include "io.hpp"
#include "actor_mon.hpp"
#include "inventory.hpp"
#include "map.hpp"
#include "explosion.hpp"
#include "player_bon.hpp"
#include "map_parsing.hpp"
#include "line_calc.hpp"
#include "actor_factory.hpp"
#include "feature_rigid.hpp"
#include "feature_mob.hpp"
#include "item.hpp"
#include "text_format.hpp"
#include "saving.hpp"
#include "game.hpp"
#include "map_travel.hpp"

namespace prop_data
{

PropDataT data[(size_t)PropId::END];

namespace
{

void add_prop_data(PropDataT& d)
{
    data[(size_t)d.id] = d;
    PropDataT blank;
    d = blank;
}

void init_data_list()
{
    PropDataT d;

    d.id = PropId::r_phys;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Physical Resistance";
    d.name_short = "rPhys";
    d.descr = "Cannot be harmed by plain physical force";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to physical harm.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to physical harm.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to physical harm.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to physical harm.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_fire;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Fire resistance";
    d.name_short = "rFire";
    d.descr = "Cannot be harmed by fire";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to fire.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to fire.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to fire.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to fire.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_poison;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Poison resistance";
    d.name_short = "rPoison";
    d.descr = "Cannot be harmed by poison";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to poison.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to poison.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to poison.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to poison.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_elec;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Electric resistance";
    d.name_short = "rElec";
    d.descr = "Cannot be harmed by electricity";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to electricity.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to electricity.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to electricity.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to electricity.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_acid;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Acid resistance";
    d.name_short = "rAcid";
    d.descr = "Cannot be harmed by acid";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to acid.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to acid.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to acid.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to acid.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_sleep;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Sleep resistance";
    d.name_short = "rSleep";
    d.descr = "Cannot faint or become hypnotized";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to sleep.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to sleep.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to sleep.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to sleep.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_fear;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Fear resistance";
    d.name_short = "rFear";
    d.descr = "Unaffected by fear";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to fear.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to fear.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to fear.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to fear.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_slow;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Slow resistance";
    d.name_short = "rSlow";
    d.descr = "Cannot be magically slowed";
    d.msg[(size_t)PropMsg::start_player] = "I feel steadfast.";
    d.msg[(size_t)PropMsg::start_mon] = "";
    d.msg[(size_t)PropMsg::end_player] = "I feel more susceptible to time.";
    d.msg[(size_t)PropMsg::end_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_conf;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Confusion resistance";
    d.name_short = "rConf";
    d.descr = "Cannot become confused";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to confusion.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to confusion.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to confusion.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to confusion.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_disease;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Disease resistance";
    d.name_short = "rDisease";
    d.descr = "Cannot become diseased";
    d.msg[(size_t)PropMsg::start_player] = "I feel resistant to disease.";
    d.msg[(size_t)PropMsg::start_mon] = "is resistant to disease.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to disease.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to disease.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_blind;
    d.name = "Blindness resistance";
    d.name_short = "rBlind";
    d.descr = "Cannot be blinded";
    d.msg[(size_t)PropMsg::start_player] = "";
    d.msg[(size_t)PropMsg::start_mon] = "";
    d.msg[(size_t)PropMsg::end_player] = "";
    d.msg[(size_t)PropMsg::end_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_para;
    d.name = "Paralysis resistance";
    d.name_short = "rPara";
    d.descr = "Cannot be paralyzed";
    d.msg[(size_t)PropMsg::start_player] = "";
    d.msg[(size_t)PropMsg::start_mon] = "";
    d.msg[(size_t)PropMsg::end_player] = "";
    d.msg[(size_t)PropMsg::end_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_breath;
    d.std_rnd_turns = Range(50, 100);
    d.name = "";
    d.name_short = "";
    d.descr = "Cannot be harmed by constricted breathing";
    d.msg[(size_t)PropMsg::start_player] = "I can breath without harm.";
    d.msg[(size_t)PropMsg::start_mon] = "can breath without harm.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::r_spell;
    d.name = "Spell Resistance";
    d.name_short = "rSpell";
    d.descr = "Cannot be affected by harmful spells";
    d.msg[(size_t)PropMsg::start_player] = "I defy harmful spells!";
    d.msg[(size_t)PropMsg::start_mon] = "is defying harmful spells.";
    d.msg[(size_t)PropMsg::end_player] = "I feel vulnerable to spells.";
    d.msg[(size_t)PropMsg::end_mon] = "is vulnerable to spells.";
    d.is_making_mon_aware = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false; // NOTE: This prop is tested anyway
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::lgt_sens;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Light sensitive";
    d.name_short = "lgtSensitive";
    d.descr = "Is vulnerable to light";
    d.msg[(size_t)PropMsg::start_player] = "I feel vulnerable to light!";
    d.msg[(size_t)PropMsg::start_mon] = "is vulnerable to light.";
    d.msg[(size_t)PropMsg::end_player] =
        "I no longer feel vulnerable to light.";
    d.msg[(size_t)PropMsg::end_mon] = "no longer is vulnerable to light.";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.msg[(size_t)PropMsg::res_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::blind;
    d.std_rnd_turns = Range(20, 30);
    d.name = "Blind";
    d.name_short = "Blind";
    d.descr = "Cannot see, -20% hit chance, -50% chance to evade attacks";
    d.msg[(size_t)PropMsg::start_player] = "I am blinded!";
    d.msg[(size_t)PropMsg::start_mon] = "is blinded.";
    d.msg[(size_t)PropMsg::end_player] = "I can see again!";
    d.msg[(size_t)PropMsg::end_mon] = "can see again.";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.msg[(size_t)PropMsg::res_mon] = "";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::deaf;
    d.std_rnd_turns = Range(200, 300);
    d.name = "Deaf";
    d.name_short = "Deaf";
    d.descr = "";
    d.msg[(size_t)PropMsg::start_player] = "I am deaf!";
    d.msg[(size_t)PropMsg::start_mon] = "";
    d.msg[(size_t)PropMsg::end_player] = "I can hear again.";
    d.msg[(size_t)PropMsg::end_mon] = "";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.msg[(size_t)PropMsg::res_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::fainted;
    d.std_rnd_turns = Range(100, 200);
    d.name = "Fainted";
    d.name_short = "Fainted";
    d.descr = "";
    d.msg[(size_t)PropMsg::start_player] = "I faint!";
    d.msg[(size_t)PropMsg::start_mon] = "faints.";
    d.msg[(size_t)PropMsg::end_player] = "I am awake.";
    d.msg[(size_t)PropMsg::end_mon] = "wakes up.";
    d.msg[(size_t)PropMsg::res_player] = "I resist fainting.";
    d.msg[(size_t)PropMsg::res_mon] = "resists fainting.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::burning;
    d.std_rnd_turns = Range(6, 8);
    d.name = "Burning";
    d.name_short = "Burning";
    d.descr =
        "Takes damage each turn, 50% chance to fail when attempting to read "
        "or cast spells";
    d.msg[(size_t)PropMsg::start_player] = "I am Burning!";
    d.msg[(size_t)PropMsg::start_mon] = "is burning.";
    d.msg[(size_t)PropMsg::end_player] = "The flames are put out.";
    d.msg[(size_t)PropMsg::end_mon] = "is no longer burning.";
    d.msg[(size_t)PropMsg::res_player] = "I resist burning.";
    d.msg[(size_t)PropMsg::res_mon] = "resists burning.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::poisoned;
    d.std_rnd_turns = Range(40, 80);
    d.name = "Poisoned";
    d.name_short = "Poisoned";
    d.descr = "Takes damage each turn";
    d.msg[(size_t)PropMsg::start_player] = "I am poisoned!";
    d.msg[(size_t)PropMsg::start_mon] = "is poisoned.";
    d.msg[(size_t)PropMsg::end_player] = "My body is cleansed from poisoning!";
    d.msg[(size_t)PropMsg::end_mon] = "is cleansed from poisoning.";
    d.msg[(size_t)PropMsg::res_player] = "I resist poisoning.";
    d.msg[(size_t)PropMsg::res_mon] = "resists poisoning.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::paralyzed;
    d.std_rnd_turns = Range(3, 5);
    d.name = "Paralyzed";
    d.name_short = "Paralyzed";
    d.descr = "Cannot move";
    d.msg[(size_t)PropMsg::start_player] = "I am paralyzed!";
    d.msg[(size_t)PropMsg::start_mon] = "is paralyzed.";
    d.msg[(size_t)PropMsg::end_player] = "I can move again!";
    d.msg[(size_t)PropMsg::end_mon] = "can move again.";
    d.msg[(size_t)PropMsg::res_player] = "I resist paralyzation.";
    d.msg[(size_t)PropMsg::res_mon] = "resists paralyzation.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::terrified;
    d.std_rnd_turns = Range(20, 30);
    d.name = "Terrified";
    d.name_short = "Terrified";
    d.descr =
        "Cannot perform melee attacks, -20% ranged hit chance, +20% chance to "
        "evade attacks";
    d.msg[(size_t)PropMsg::start_player] = "I am terrified!";
    d.msg[(size_t)PropMsg::start_mon] = "looks terrified.";
    d.msg[(size_t)PropMsg::end_player] = "I am no longer terrified!";
    d.msg[(size_t)PropMsg::end_mon] = "is no longer terrified.";
    d.msg[(size_t)PropMsg::res_player] = "I resist fear.";
    d.msg[(size_t)PropMsg::res_mon] = "resists fear.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::confused;
    d.std_rnd_turns = Range(80, 120);
    d.name = "Confused";
    d.name_short = "Confused";
    d.descr =
        "Occasionally moving in random directions, cannot read or cast spells, "
        "cannot search for hidden doors or traps";
    d.msg[(size_t)PropMsg::start_player] = "I am confused!";
    d.msg[(size_t)PropMsg::start_mon] = "looks confused.";
    d.msg[(size_t)PropMsg::end_player] = "I come to my senses.";
    d.msg[(size_t)PropMsg::end_mon] = "is no longer confused.";
    d.msg[(size_t)PropMsg::res_player] = "I manage to keep my head together.";
    d.msg[(size_t)PropMsg::res_mon] = "resists confusion.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::stunned;
    d.std_rnd_turns = Range(5, 9);
    d.name = "Stunned";
    d.name_short = "Stunned";
    d.descr = "";
    d.msg[(size_t)PropMsg::start_player] = "I am stunned!";
    d.msg[(size_t)PropMsg::start_mon] = "is stunned.";
    d.msg[(size_t)PropMsg::end_player] = "I am no longer stunned.";
    d.msg[(size_t)PropMsg::end_mon] = "is no longer stunned.";
    d.msg[(size_t)PropMsg::res_player] = "I resist stunning.";
    d.msg[(size_t)PropMsg::res_mon] = "resists stunning.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::slowed;
    d.std_rnd_turns = Range(16, 24);
    d.name = "Slowed";
    d.name_short = "Slowed";
    d.descr = "-50% speed";
    d.msg[(size_t)PropMsg::start_player] =
        "Everything around me seems to speed up.";
    d.msg[(size_t)PropMsg::start_mon] = "slows down.";
    d.msg[(size_t)PropMsg::end_player] =
        "Everything around me seems to slow down.";
    d.msg[(size_t)PropMsg::end_mon] = "speeds up.";
    d.msg[(size_t)PropMsg::res_player] = "I resist slowness.";
    d.msg[(size_t)PropMsg::res_mon] = "resists slowness.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::hasted;
    d.std_rnd_turns = Range(12, 16);
    d.name = "Hasted";
    d.name_short = "Hasted";
    d.descr = "+100% speed";
    d.msg[(size_t)PropMsg::start_player] =
        "Everything around me seems to slow down.";
    d.msg[(size_t)PropMsg::start_mon] = "speeds up.";
    d.msg[(size_t)PropMsg::end_player] =
        "Everything around me seems to speed up.";
    d.msg[(size_t)PropMsg::end_mon] = "slows down.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::clockwork_hasted;
    d.std_rnd_turns = Range(7, 11);
    d.name = "Extra hasted";
    d.name_short = "Hasted+";
    d.descr = "+300% speed";
    d.msg[(size_t)PropMsg::start_player] =
        "Everything around me suddenly seems very still.";
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::summoned;
    d.std_rnd_turns = Range(40, 80);
    d.msg[(size_t)PropMsg::end_mon] = "suddenly disappears.";
    d.name = "Summoned";
    d.descr = "Was magically summoned here";
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::flared;
    d.std_rnd_turns = Range(3, 4);
    d.msg[(size_t)PropMsg::start_mon] = "is perforated by a flare!";
    d.is_making_mon_aware = true;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::nailed;
    d.name = "Nailed";
    d.descr = "Fastened by a spike - tearing it out will be rather painful";
    d.msg[(size_t)PropMsg::start_player] = "I am fastened by a spike!";
    d.msg[(size_t)PropMsg::start_mon] = "is fastened by a spike.";
    d.msg[(size_t)PropMsg::end_player] = "I tear free!";
    d.msg[(size_t)PropMsg::end_mon] = "tears free!";
    d.is_making_mon_aware = true;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::wound;
    d.name = "Wounded";
    d.descr =
        "For each wound: -5% melee hit chance, -5% chance to evade attacks, "
        "-10% Hit Points, and reduced Hit Point generation rate - also, "
        "walking takes extra turns if more than two wounds are received";
    d.msg[(size_t)PropMsg::start_player] = "I am wounded!";
    d.msg[(size_t)PropMsg::res_player] = "I resist wounding!";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::hp_sap;
    d.name = "Life Sapped";
    d.descr = "Fewer Hit Points";
    d.msg[(size_t)PropMsg::start_player] = "My life force is sapped!";
    d.msg[(size_t)PropMsg::start_mon] = "is sapped of life.";
    d.msg[(size_t)PropMsg::end_player] = "My life force returns.";
    d.msg[(size_t)PropMsg::end_mon] = "looks restored.";
    d.msg[(size_t)PropMsg::res_player] = "I resist sapping.";
    d.msg[(size_t)PropMsg::res_mon] = "resists sapping.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::spi_sap;
    d.name = "Spirit Sapped";
    d.descr = "Fewer Spirit Points";
    d.msg[(size_t)PropMsg::start_player] = "My spirit is sapped!";
    d.msg[(size_t)PropMsg::start_mon] = "is sapped of spirit.";
    d.msg[(size_t)PropMsg::end_player] = "My spirit returns.";
    d.msg[(size_t)PropMsg::end_mon] = "looks restored.";
    d.msg[(size_t)PropMsg::res_player] = "I resist sapping.";
    d.msg[(size_t)PropMsg::res_mon] = "resists sapping.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::mind_sap;
    d.name = "Mind Sapped";
    d.descr = "Increased Shock";
    d.msg[(size_t)PropMsg::start_player] = "My mind is sapped!";
    d.msg[(size_t)PropMsg::end_player] = "My mind returns.";
    d.msg[(size_t)PropMsg::res_player] = "I resist sapping.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::infected;
    d.std_rnd_turns = Range(100, 100);
    d.name = "Infected";
    d.name_short = "Infected";
    d.descr = "A nasty infection, this should be treated immediately";
    d.msg[(size_t)PropMsg::start_player] = "I am infected!";
    d.msg[(size_t)PropMsg::start_mon] = "is infected.";
    d.msg[(size_t)PropMsg::end_player] = "My infection is cured!";
    d.msg[(size_t)PropMsg::end_mon] = "is no longer infected.";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.msg[(size_t)PropMsg::res_mon] = "";
    d.is_making_mon_aware = true;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::diseased;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Diseased";
    d.name_short = "Diseased";
    d.descr = "Affected by a horrible disease, -50% Hit Points";
    d.msg[(size_t)PropMsg::start_player] = "I am diseased!";
    d.msg[(size_t)PropMsg::start_mon] = "is diseased.";
    d.msg[(size_t)PropMsg::end_player] = "My disease is cured!";
    d.msg[(size_t)PropMsg::end_mon] = "is no longer diseased.";
    d.msg[(size_t)PropMsg::res_player] = "I resist disease.";
    d.msg[(size_t)PropMsg::res_mon] = "resists disease.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::descend;
    d.std_rnd_turns = Range(20, 30);
    d.name = "Descending";
    d.name_short = "Descend";
    d.descr = "Soon moved to a deeper level";
    d.msg[(size_t)PropMsg::start_player] = "I feel a sinking sensation.";
    d.msg[(size_t)PropMsg::end_player] = "";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::weakened;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Weakened";
    d.name_short = "Weakened";
    d.descr =
        "Halved melee damage, cannot bash doors or chests open, knock "
        "heavy objects over, etc";
    d.msg[(size_t)PropMsg::start_player] = "I feel weaker.";
    d.msg[(size_t)PropMsg::start_mon] = "looks weaker.";
    d.msg[(size_t)PropMsg::end_player] = "I feel stronger!";
    d.msg[(size_t)PropMsg::end_mon] = "looks stronger!";
    d.msg[(size_t)PropMsg::res_player] = "I resist weakness.";
    d.msg[(size_t)PropMsg::res_mon] = "resists weakness.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::frenzied;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Frenzied";
    d.name_short = "Frenzied";
    d.descr =
        "Cannot move away from seen enemies, +100% speed, +1 melee damage "
        "+10% melee hit chance, immune to confusion, fainting, fear, and "
        "weakening, cannot read or cast spells, becomes weakened when the "
        "frenzy ends";
    d.msg[(size_t)PropMsg::start_player] = "I feel ferocious!!!";
    d.msg[(size_t)PropMsg::start_mon] = "Looks ferocious!";
    d.msg[(size_t)PropMsg::end_player] = "I calm down.";
    d.msg[(size_t)PropMsg::end_mon] = "Calms down a little.";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.msg[(size_t)PropMsg::res_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::blessed;
    d.std_rnd_turns = Range(400, 600);
    d.name = "Blessed";
    d.name_short = "Blessed";
    d.descr =
        "Is generally more lucky, most actions are easier to perform, "
        "-2% shock from casting spells";
    d.msg[(size_t)PropMsg::start_player] = "I feel luckier.";
    d.msg[(size_t)PropMsg::end_player] = "I have normal luck.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::cursed;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Cursed";
    d.name_short = "Cursed";
    d.descr =
        "Is generally more unlucky, and most actions are more difficult to "
        "perform, +2% shock from casting spells";
    d.msg[(size_t)PropMsg::start_player] = "I feel misfortunate.";
    d.msg[(size_t)PropMsg::end_player] = "I feel more fortunate.";
    d.msg[(size_t)PropMsg::res_player] = "I resist misfortune.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::radiant;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Radiant";
    d.name_short = "Radiant";
    d.descr = "Emanating a bright light";
    d.msg[(size_t)PropMsg::start_player] = "A bright light shines around me.";
    d.msg[(size_t)PropMsg::end_player] = "It suddenly gets darker.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::invis;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Invisible";
    d.name_short = "Invis";
    d.descr = "Cannot be detected by normal sight";
    d.msg[(size_t)PropMsg::start_player] = "I am out of sight!";
    d.msg[(size_t)PropMsg::start_mon] = "is out of sight!";
    d.msg[(size_t)PropMsg::end_player] = "I am visible.";
    d.msg[(size_t)PropMsg::end_mon] = "is visible.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::cloaked;
    d.std_rnd_turns = Range(5, 7);
    d.name = "Cloaked";
    d.name_short = "Cloaked";
    d.descr =
        "Cannot be detected by normal sight, ends if attacking or casting "
        "spells";
    d.msg[(size_t)PropMsg::start_player] = "I am out of sight!";
    d.msg[(size_t)PropMsg::start_mon] = "is out of sight!";
    d.msg[(size_t)PropMsg::end_player] = "I am visible.";
    d.msg[(size_t)PropMsg::end_mon] = "is visible.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::see_invis;
    d.std_rnd_turns = Range(50, 100);
    d.name = "See Invisible";
    d.name_short = "SeeInvis";
    d.descr = "Can see invisible creatures, cannot be blinded";
    d.msg[(size_t)PropMsg::start_player] = "My eyes perceive the invisible.";
    d.msg[(size_t)PropMsg::start_mon] = "seems very keen.";
    d.msg[(size_t)PropMsg::end_player] =
        "My eyes can no longer perceive the invisible.";
    d.msg[(size_t)PropMsg::end_mon] = "seems less keen.";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::darkvis;
    d.std_rnd_turns = Range(50, 100);
    d.name = "";
    d.name_short = "";
    d.descr = "";
    d.msg[(size_t)PropMsg::start_player] = "";
    d.msg[(size_t)PropMsg::end_player] = "";
    d.msg[(size_t)PropMsg::res_player] = "";
    d.msg[(size_t)PropMsg::res_mon] = "";
    d.is_making_mon_aware = false;
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = true;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::tele_ctrl;
    d.std_rnd_turns = Range(50, 100);
    d.name = "Teleport Control";
    d.name_short = "TeleCtrl";
    d.descr = "Can control teleport destination";
    d.msg[(size_t)PropMsg::start_player] = "I feel in control.";
    d.msg[(size_t)PropMsg::end_player] = "I feel less in control.";
    d.allow_display_turns = true;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::spell_reflect;
    d.std_rnd_turns = Range(50, 100);
    d.name = "";
    d.name_short = "";
    d.descr = "";
    d.msg[(size_t)PropMsg::start_player] = "";
    d.msg[(size_t)PropMsg::end_player] = "";
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::aiming;
    d.std_rnd_turns = Range(1, 1);
    d.name = "Aiming";
    d.name_short = "Aiming";
    d.descr = "Increased range attack effectiveness";
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::good;
    add_prop_data(d);

    d.id = PropId::strangled;
    d.std_rnd_turns = Range(1, 1);
    d.name = "Strangled";
    d.name_short = "Strangled";
    d.descr = "";
    d.is_making_mon_aware = true;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::conflict;
    d.name = "Conflicted";
    d.name_short = "Conflict";
    d.descr = "Considers every creature as an enemy";
    d.std_rnd_turns = Range(10, 20);
    d.msg[(size_t)PropMsg::start_mon] = "Looks conflicted.";
    d.msg[(size_t)PropMsg::end_mon] = "Looks more determined.";
    d.is_making_mon_aware = true;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::bad;
    add_prop_data(d);

    d.id = PropId::poss_by_zuul;
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = true;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::flying;
    d.name_short = "Flying";
    d.name = "Flying";
    d.descr = "Can fly over low obstacles";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::ethereal;
    d.name_short = "Ethereal";
    d.name = "Ethereal";
    d.descr =
        "Can pass through solid objects, and is harder to hit with all attacks";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::ooze;
    d.name_short = "Ooze";
    d.name = "Ooze";
    d.descr =
        "Can move through narrow cracks and crevices such as piles of rubble, "
        "or beneath doors";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::burrowing;
    d.name_short = "Burrow";
    d.name = "Burrowing";
    d.descr = "Can move through and destroy walls";
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.allow_test_on_bot = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::waiting;
    d.std_rnd_turns = Range(1, 1);
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::disabled_attack;
    d.std_rnd_turns = Range(1, 1);
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::disabled_melee;
    d.std_rnd_turns = Range(1, 1);
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);

    d.id = PropId::disabled_ranged;
    d.std_rnd_turns = Range(1, 1);
    d.is_making_mon_aware = false;
    d.allow_display_turns = false;
    d.update_vision_when_start_or_end = false;
    d.alignment = PropAlignment::neutral;
    add_prop_data(d);
}

} // namespace

void init()
{
    init_data_list();
}

} // prop_data

// -----------------------------------------------------------------------------
// Property handler
// -----------------------------------------------------------------------------
PropHandler::PropHandler(Actor* owning_actor) :
    owning_actor_(owning_actor)
{
    // Reset the active props info
    std::fill(std::begin(active_props_info_),
              std::end(active_props_info_),
              0);
}

void PropHandler::init_natural_props()
{
    const ActorDataT& d = owning_actor_->data();

    // Add natural properties
    for (size_t i = 0; i < (size_t)PropId::END; ++i)
    {
        active_props_info_[i] = 0;

        if (d.natural_props[i])
        {
            Prop* const prop = mk_prop(PropId(i), PropTurns::indefinite);

            apply(prop,
                    PropSrc::intr,
                    true,
                    Verbosity::silent);
        }
    }
}

PropHandler::~PropHandler()
{
    for (Prop* prop : props_)
    {
#ifndef NDEBUG
        // For sanity check
        decr_active_props_info(prop->id());
#endif // NDEBUG
        delete prop;
    }

#ifndef NDEBUG
    // Sanity check: all active props info should be exactly zero now
    for (size_t i = 0; i < (size_t)PropId::END; ++i)
    {
        if (active_props_info_[i] != 0)
        {
            TRACE << "Active property info at id " << i
                  << " not zero" << std::endl;
            ASSERT(false);
        }
    }
#endif // NDEBUG
}

void PropHandler::save() const
{
    // Save intrinsic properties to file

    ASSERT(owning_actor_);

    int nr_intr_props_ = 0;

    for (Prop* prop : props_)
    {
        if (prop->src_ == PropSrc::intr)
        {
            ++nr_intr_props_;
        }
    }

    saving::put_int(nr_intr_props_);

    for (Prop* prop : props_)
    {
        if (prop->src_ == PropSrc::intr)
        {
            saving::put_int(int(prop->id()));
            saving::put_int(prop->nr_turns_left_);

            prop->save();
        }
    }
}

void PropHandler::load()
{
    // Load intrinsic properties from file

    ASSERT(owning_actor_);

    const int nr_props = saving::get_int();

    for (int i = 0; i < nr_props; ++i)
    {
        const auto prop_id = (PropId)saving::get_int();

        const int nr_turns = saving::get_int();

        const auto turns_init =
            (nr_turns == -1) ?
            PropTurns::indefinite :
            PropTurns::specific;

        Prop* const prop = mk_prop(prop_id,
                                   turns_init,
                                   nr_turns);

        prop->owning_actor_ = owning_actor_;

        prop->src_ = PropSrc::intr;

        props_.push_back(prop);

        incr_active_props_info(prop_id);

        prop->load();
    }
}

Prop* PropHandler::mk_prop(const PropId id,
                           PropTurns turns_init,
                           const int nr_turns) const
{
    ASSERT(id != PropId::END);

    // Prop turns init type should either be:
    // * "specific", and number of turns specified (> 0), OR
    // * NOT "specific" (i.e. "indefinite" or "std"), and number of turns NOT
    //   specified (-1)
    ASSERT(((turns_init == PropTurns::specific) && (nr_turns > 0)) ||
           ((turns_init != PropTurns::specific) && (nr_turns == -1)));

    switch (id)
    {
    case PropId::nailed:
        return new PropNailed(turns_init, nr_turns);

    case PropId::wound:
        return new PropWound(turns_init, nr_turns);

    case PropId::blind:
        return new PropBlind(turns_init, nr_turns);

    case PropId::deaf:
        return new PropDeaf(turns_init, nr_turns);

    case PropId::burning:
        return new PropBurning(turns_init, nr_turns);

    case PropId::flared:
        return new PropFlared(turns_init, nr_turns);

    case PropId::paralyzed:
        return new PropParalyzed(turns_init, nr_turns);

    case PropId::terrified:
        return new PropTerrified(turns_init, nr_turns);

    case PropId::weakened:
        return new PropWeakened(turns_init, nr_turns);

    case PropId::confused:
        return new PropConfused(turns_init, nr_turns);

    case PropId::stunned:
        return new PropStunned(turns_init, nr_turns);

    case PropId::waiting:
        return new PropWaiting(turns_init, nr_turns);

    case PropId::slowed:
        return new PropSlowed(turns_init, nr_turns);

    case PropId::hasted:
        return new PropHasted(turns_init, nr_turns);

    case PropId::clockwork_hasted:
        return new PropClockworkHasted(turns_init, nr_turns);

    case PropId::summoned:
        return new PropSummoned(turns_init, nr_turns);

    case PropId::infected:
        return new PropInfected(turns_init, nr_turns);

    case PropId::diseased:
        return new PropDiseased(turns_init, nr_turns);

    case PropId::descend:
        return new PropDescend(turns_init, nr_turns);

    case PropId::poisoned:
        return new PropPoisoned(turns_init, nr_turns);

    case PropId::fainted:
        return new PropFainted(turns_init, nr_turns);

    case PropId::frenzied:
        return new PropFrenzied(turns_init, nr_turns);

    case PropId::aiming:
        return new PropAiming(turns_init, nr_turns);

    case PropId::disabled_attack:
        return new PropDisabledAttack(turns_init, nr_turns);

    case PropId::disabled_melee:
        return new PropDisabledMelee(turns_init, nr_turns);

    case PropId::disabled_ranged:
        return new PropDisabledRanged(turns_init, nr_turns);

    case PropId::blessed:
        return new PropBlessed(turns_init, nr_turns);

    case PropId::cursed:
        return new PropCursed(turns_init, nr_turns);

    case PropId::r_acid:
        return new PropRAcid(turns_init, nr_turns);

    case PropId::r_conf:
        return new PropRConf(turns_init, nr_turns);

    case PropId::r_breath:
        return new PropRBreath(turns_init, nr_turns);

    case PropId::r_elec:
        return new PropRElec(turns_init, nr_turns);

    case PropId::r_fear:
        return new PropRFear(turns_init, nr_turns);

    case PropId::r_slow:
        return new PropRSlow(turns_init, nr_turns);

    case PropId::r_phys:
        return new PropRPhys(turns_init, nr_turns);

    case PropId::r_fire:
        return new PropRFire(turns_init, nr_turns);

    case PropId::r_spell:
        return new PropRSpell(turns_init, nr_turns);

    case PropId::r_poison:
        return new PropRPoison(turns_init, nr_turns);

    case PropId::r_sleep:
        return new PropRSleep(turns_init, nr_turns);

    case PropId::lgt_sens:
        return new PropLgtSens(turns_init, nr_turns);

    case PropId::poss_by_zuul:
        return new PropPossByZuul(turns_init, nr_turns);

    case PropId::flying:
        return new PropFlying(turns_init, nr_turns);

    case PropId::ethereal:
        return new PropEthereal(turns_init, nr_turns);

    case PropId::ooze:
        return new PropOoze(turns_init, nr_turns);

    case PropId::burrowing:
        return new PropBurrowing(turns_init, nr_turns);

    case PropId::radiant:
        return new PropRadiant(turns_init, nr_turns);

    case PropId::darkvis:
        return new PropDarkvis(turns_init, nr_turns);

    case PropId::r_disease:
        return new PropRDisease(turns_init, nr_turns);

    case PropId::r_blind:
        return new PropRBlind(turns_init, nr_turns);

    case PropId::r_para:
        return new PropRPara(turns_init, nr_turns);

    case PropId::tele_ctrl:
        return new PropTeleControl(turns_init, nr_turns);

    case PropId::spell_reflect:
        return new PropSpellReflect(turns_init, nr_turns);

    case PropId::strangled:
        return new PropStrangled(turns_init, nr_turns);

    case PropId::conflict:
        return new PropConflict(turns_init, nr_turns);

    case PropId::invis:
        return new PropInvisible(turns_init, nr_turns);

    case PropId::cloaked:
        return new PropCloaked(turns_init, nr_turns);

    case PropId::see_invis:
        return new PropSeeInvis(turns_init, nr_turns);

    case PropId::hp_sap:
        return new PropHpSap(turns_init, nr_turns);

    case PropId::spi_sap:
        return new PropSpiSap(turns_init, nr_turns);

    case PropId::mind_sap:
        return new PropMindSap(turns_init, nr_turns);

    case PropId::END:
        break;
    }

    return nullptr;
}

void PropHandler::apply(Prop* const prop,
                        PropSrc src,
                        const bool force_effect,
                        const Verbosity verbosity)
{
    ASSERT(prop);

    prop->owning_actor_ = owning_actor_;

    prop->src_ = src;

    const bool is_player = owning_actor_->is_player();

    const bool player_see_owner = map::player->can_see_actor(*owning_actor_);

    // Check if property is resisted
    if (!force_effect)
    {
        if (is_resisting_prop(prop->id()))
        {
            // Resist message
            if (verbosity == Verbosity::verbose &&
                owning_actor_->is_alive())
            {
                if (is_player)
                {
                    std::string msg = "";

                    prop->msg(PropMsg::res_player, msg);

                    if (!msg.empty())
                    {
                        msg_log::add(msg,
                                     clr_text,
                                     true);
                    }
                }
                else // Is a monster
                {
                    if (player_see_owner)
                    {
                        std::string msg = "";
                        prop->msg(PropMsg::res_mon, msg);

                        if (!msg.empty())
                        {
                            const std::string monster_name =
                                text_format::first_to_upper(
                                    owning_actor_->name_the());

                            msg_log::add(monster_name + " " + msg);
                        }
                    }
                }
            }

            delete prop;
            return;
        }
    }

    // This point reached means nothing is blocking the property.

    // Is this an intrinsic property, and actor already has an intrinsic
    // property of the same type? If so, the new property will just be "merged"
    // into the old one ("on_more()").
    if (prop->src_ == PropSrc::intr)
    {
        for (Prop* old_prop : props_)
        {
            // Is this an intrinsic property of the same type?
            if ((old_prop->src_ == PropSrc::intr) &&
                (prop->id() == old_prop->id()))
            {
                const int turns_left_old = old_prop->nr_turns_left_;
                const int turns_left_new = prop->nr_turns_left_;

                //
                // TODO: Should messages be printed here? It can get spammy...
                //

                // Start message
                // if (verbosity == Verbosity::verbose &&
                //     owning_actor_->is_alive())
                // {
                //     if (is_player)
                //     {
                //         std::string msg = "";
                //         prop->msg(PropMsg::start_player, msg);

                //         if (!msg.empty())
                //         {
                //             msg_log::add(msg, clr_text, true);
                //         }
                //     }
                //     else // Not player
                //     {
                //         if (player_see_owner)
                //         {
                //             std::string msg = "";
                //             prop->msg(PropMsg::start_mon, msg);

                //             if (!msg.empty())
                //             {
                //                 msg_log::add(
                //                     owning_actor_->name_the() + " " + msg);
                //             }
                //         }
                //     }
                // }

                old_prop->on_more(*prop);

                const bool is_turns_nr_indefinite =
                    (turns_left_old < 0) ||
                    (turns_left_new < 0);

                old_prop->nr_turns_left_ =
                    is_turns_nr_indefinite ?
                    -1 :
                    std::max(turns_left_old, turns_left_new);

                if (prop->turns_init_type() == PropTurns::indefinite)
                {
                    old_prop->turns_init_type_ = PropTurns::indefinite;
                }

                delete prop;
                return;
            }
        }
    }

    // This part reached means the property should be applied on its own

    props_.push_back(prop);

    incr_active_props_info(prop->id());

    prop->on_start();

    if (verbosity == Verbosity::verbose &&
        owning_actor_->is_alive())
    {
        if (prop->need_update_vision_when_start_or_end())
        {
            map::update_vision();
        }

        // Start message
        if (is_player)
        {
            std::string msg = "";

            prop->msg(PropMsg::start_player, msg);

            if (!msg.empty())
            {
                const bool is_interrupting =
                    (prop->alignment() != PropAlignment::good);

                msg_log::add(msg,
                             clr_text,
                             is_interrupting);
            }
        }
        else // Is monster
        {
            if (player_see_owner)
            {
                std::string msg = "";
                prop->msg(PropMsg::start_mon, msg);

                if (!msg.empty())
                {
                    const std::string actor_name_the =
                        text_format::first_to_upper(
                            owning_actor_->name_the());

                    msg_log::add(actor_name_the + " " + msg);
                }
            }
        }
    }
}

void PropHandler::add_prop_from_equipped_item(const Item* const item,
                                              Prop* const prop,
                                              const Verbosity verbosity)
{
    prop->item_applying_ = item;

    apply(prop,
          PropSrc::inv,
          true,
          verbosity);
}

Prop* PropHandler::prop(const PropId id) const
{
    if (has_prop(id))
    {
        for (Prop* const prop : props_)
        {
            if (prop->id() == id)
            {
                return prop;
            }
        }
    }

    return nullptr;
}

void PropHandler::remove_props_for_item(const Item* const item)
{
    for (size_t i = 0; i < props_.size(); /* No increment */)
    {
        Prop* const prop = props_[i];

        if (prop->item_applying_ == item)
        {
            ASSERT(prop->src_ == PropSrc::inv);

            ASSERT(prop->turns_init_type_ == PropTurns::indefinite);

            props_.erase(begin(props_) + i);

            decr_active_props_info(prop->id());

            on_prop_end(prop);
        }
        else // Property was not added by this item
        {
            ++i;
        }
    }
}

void PropHandler::apply_from_att(const Wpn& wpn,
                                 const bool is_melee)
{
    const auto& d = wpn.data();

    const auto& att_prop =
        is_melee ?
        d.melee.prop_applied :
        d.ranged.prop_applied;

    if (att_prop.prop &&
        rnd::percent(att_prop.pct_chance))
    {
        // If weapon damage is resisted by the defender, the property is
        // automatically resisted
        const DmgType dmg_type =
            is_melee ?
            d.melee.dmg_type :
            d.ranged.dmg_type;

        const bool is_dmg_resisted =
            is_resisting_dmg(dmg_type,
                             nullptr,
                             Verbosity::silent);

        if (!is_dmg_resisted)
        {
            const auto turns_init_type = att_prop.prop->turns_init_type();

            const int nr_turns =
                (turns_init_type == PropTurns::specific) ?
                att_prop.prop->nr_turns_left_ :
                -1;

            // Make a copy of the weapon effect
            auto* const prop_cpy = mk_prop(att_prop.prop->id(),
                                           att_prop.prop->turns_init_type_,
                                           nr_turns);

            apply(prop_cpy);
        }
    }
}

void PropHandler::incr_active_props_info(const PropId id)
{
    int& v = active_props_info_[(size_t)id];

#ifndef NDEBUG
    if (v < 0)
    {
        TRACE << "Tried to increment property with current value "
              << v << std::endl;

        ASSERT(false);
    }
#endif // NDEBUG

    ++v;
}

void PropHandler::decr_active_props_info(const PropId id)
{
    int& v = active_props_info_[(size_t)id];

#ifndef NDEBUG
    if (v <= 0)
    {
        TRACE << "Tried to decrement property with current value "
              << v << std::endl;

        ASSERT(false);
    }
#endif // NDEBUG

    --v;
}

void PropHandler::on_prop_end(Prop* const prop)
{
    if (prop->need_update_vision_when_start_or_end())
    {
        map::update_vision();
    }

    // Print end message if this is the last active property of this type
    if ((owning_actor_->state() == ActorState::alive) &&
        active_props_info_[(size_t)prop->id_] == 0)
    {
        if (owning_actor_->is_player())
        {
            std::string msg = "";

            prop->msg(PropMsg::end_player, msg);

            if (!msg.empty())
            {
                msg_log::add(msg);
            }
        }
        else // Not player
        {
            if (map::player->can_see_actor(*owning_actor_))
            {
                std::string msg = "";

                prop->msg(PropMsg::end_mon, msg);

                if (!msg.empty())
                {
                    const std::string actor_name_the =
                        text_format::first_to_upper(
                            owning_actor_->name_the());

                    msg_log::add(actor_name_the + " " + msg);
                }
            }
        }
    }

    prop->on_end();
}

bool PropHandler::end_prop(const PropId id, const bool run_prop_end_effects)
{
    for (auto it = begin(props_); it != end(props_); ++it)
    {
        Prop* const prop = *it;

        if ((prop->id_ == id) &&
            (prop->src_ == PropSrc::intr))
        {
            props_.erase(it);

            decr_active_props_info(prop->id_);

            if (run_prop_end_effects)
            {
                on_prop_end(prop);
            }

            delete prop;

            return true;
        }
    }

    return false;
}

void PropHandler::on_turn_begin()
{
    for (size_t i = 0; i < props_.size(); /* No increment */)
    {
        Prop* prop = props_[i];

        // Count down number of turns
        if (prop->nr_turns_left_ > 0)
        {
            ASSERT(prop->src_ == PropSrc::intr);

            --prop->nr_turns_left_;
        }

        // Aggravates monster?
        if (!owning_actor_->is_player() &&
            prop->is_making_mon_aware())
        {
            auto* mon = static_cast<Mon*>(owning_actor_);

            mon->become_aware_player(false);
        }

        //
        // NOTE: The property may return a nullptr here, if it removed itself
        //
        prop = prop->on_tick();

        if (prop)
        {
            // Property has not been removed
            ++i;
        }
    }
}

void PropHandler::on_turn_end()
{
    for (size_t i = 0; i < props_.size(); /* No increment */)
    {
        Prop* prop = props_[i];

        if (prop->is_finished())
        {
            props_.erase(begin(props_) + i);

            decr_active_props_info(prop->id());

            on_prop_end(prop);

            delete prop;
        }
        else  // Property has not been removed
        {
            ++i;
        }
    }
}

bool PropHandler::is_temporary_negative_prop_mon(const Prop& prop)
{
    ASSERT(owning_actor_ != map::player);

    const auto id = prop.id();

    const bool is_natural_prop =
        owning_actor_->data().natural_props[(size_t)id];

    return
        !is_natural_prop &&
        (prop.turns_init_type() != PropTurns::indefinite) &&
        (prop.alignment() == PropAlignment::bad);
}

std::vector<PropListEntry> PropHandler::temporary_negative_prop_list()
{
    ASSERT(owning_actor_ != map::player);

    auto prop_list = owning_actor_->prop_handler().props_list();

    // Remove all non-negative properties (we should not show temporary spell
    // resistance for example), and all natural properties (properties which all
    // monsters of this type starts with)
    for (auto it = begin(prop_list); it != end(prop_list);)
    {
        auto* const prop = it->prop;

        if (is_temporary_negative_prop_mon(*prop))
        {
            ++it;
        }
        else // Not a temporary negative property
        {
            it = prop_list.erase(it);
        }
    }

    return prop_list;
}

bool PropHandler::has_temporary_negative_prop_mon()
{
    ASSERT(owning_actor_ != map::player);

    for (const auto* const prop: props_)
    {
        if (is_temporary_negative_prop_mon(*prop))
        {
            return true;
        }
    }

    return false;
}

std::vector<StrAndClr> PropHandler::props_line() const
{
    std::vector<StrAndClr> line;

    for (Prop* prop : props_)
    {
        std::string str = prop->name_short();

        if (str.empty())
        {
            continue;
        }

        const int turns_left  = prop->nr_turns_left_;

        if (prop->turns_init_type() == PropTurns::indefinite)
        {
            // Indefinite intrinsic properties are printed in upper case
            if (prop->src() == PropSrc::intr)
            {
                str = text_format::all_to_upper(str);
            }
        }
        else // Not indefinite
        {
            // Player can see number of turns left on own properties with
            // Self-aware?
            if (owning_actor_->is_player() &&
                player_bon::traits[(size_t)Trait::self_aware] &&
                prop->allow_display_turns())
            {
                //
                // NOTE: Since turns left are decremented before the actors
                //       turn, and checked after the turn - "turns_left"
                //       practically represents how many more times the
                //       actor will act with the property enabled, EXCLUDING
                //       the current (ongoing) turn.
                //
                //       I.e. one "turns_left" means that the property will
                //       be enabled the whole next turn, while Zero
                //       "turns_left", means that it will only be active the
                //       current turn. However, from a players perspective,
                //       this is unintuitive; "one turn left" means the
                //       current turn, plus the next - but is likely
                //       interpreted as just the current turn. Therefore we
                //       add +1 to the displayed value, so that a displayed
                //       value of one means that the property will end after
                //       performing the next action.
                //
                const int turns_left_displayed = turns_left + 1;

                str += ":" + std::to_string(turns_left_displayed);
            }
        }

        const PropAlignment alignment = prop->alignment();

        const Clr clr =
            (alignment == PropAlignment::good) ? clr_msg_good :
            (alignment == PropAlignment::bad)  ? clr_msg_bad :
            clr_white;

        line.push_back(StrAndClr(str, clr));
    }

    return line;
}

//
// TODO: Lots of copy paste from the function above (props_line), some
//       refactoring needed here
//
std::vector<PropListEntry> PropHandler::props_list() const
{
    std::vector<PropListEntry> list;

    for (Prop* prop : props_)
    {
        const std::string name_long = prop->name();

        std::string name_short = prop->name_short();

        if (name_long.empty() && name_short.empty())
        {
            continue;
        }

        if (!name_short.empty())
        {
            const int turns_left  = prop->nr_turns_left_;

            if (prop->turns_init_type() == PropTurns::indefinite &&
                prop->src() == PropSrc::intr)
            {
                // Indefinite intrinsic properties are printed in upper case
                name_short = text_format::all_to_upper(name_short);
            }
            else // Not indefinite
            {
                // Player can see number of turns left on own properties with
                // Self-aware?
                if (owning_actor_->is_player() &&
                    player_bon::traits[(size_t)Trait::self_aware] &&
                    prop->allow_display_turns())
                {
                    //
                    // NOTE: Since turns left are decremented before the actors
                    //       turn, and checked after the turn - "turns_left"
                    //       practically represents how many more times the
                    //       actor will act with the property enabled, EXCLUDING
                    //       the current (ongoing) turn.
                    //
                    //       I.e. one "turns_left" means that the property will
                    //       be enabled the whole next turn, while Zero
                    //       "turns_left", means that it will only be active the
                    //       current turn. However, from a players perspective,
                    //       this is unintuitive; "one turn left" means the
                    //       current turn, plus the next - but is likely
                    //       interpreted as just the current turn. Therefore we
                    //       add +1 to the displayed value, so that a displayed
                    //       value of one means that the property will end after
                    //       performing the next action.
                    //
                    const int turns_left_displayed = turns_left + 1;

                    name_short += ":" + std::to_string(turns_left_displayed);
                }
            }
        }

        std::string title;

        if (!name_long.empty())
        {
            title = name_long;
        }

        if (!name_short.empty() &&
            name_short != name_long)
        {
            title += " (" + name_short + ")";
        }

        const PropAlignment alignment = prop->alignment();

        const Clr clr =
            (alignment == PropAlignment::good) ? clr_msg_good :
            (alignment == PropAlignment::bad)  ? clr_msg_bad :
            clr_white;

        const std::string descr = prop->descr();

        const size_t new_size = list.size() + 1;

        list.resize(new_size);

        auto& entry = list[new_size - 1];

        entry.title.str = title;

        entry.title.clr = clr;

        entry.descr = descr;

        entry.prop = prop;
    }

    return list;
}

bool PropHandler::is_resisting_prop(const PropId id) const
{
    for (Prop* p : props_)
    {
        if (p->is_resisting_other_prop(id))
        {
            return true;
        }
    }

    return false;
}

bool PropHandler::is_resisting_dmg(const DmgType dmg_type,
                                   const Actor* const attacker,
                                   const Verbosity verbosity) const
{
    DmgResistData res_data;

    for (Prop* p : props_)
    {
        res_data = p->is_resisting_dmg(dmg_type);

        if (res_data.is_resisted)
        {
            break;
        }
    }

    if (res_data.is_resisted &&
        (verbosity == Verbosity::verbose))
    {
        if (owning_actor_->is_player())
        {
            msg_log::add(res_data.resist_msg_player);
        }
        else // Is monster
        {
            // Print message if attacker is player, and player is aware of
            // the monster
            const auto* const mon =
                static_cast<const Mon*>(owning_actor_);

            const bool is_player_aware_of_mon =
                mon->player_aware_of_me_counter_ > 0;

            if ((attacker == map::player) &&
                is_player_aware_of_mon)
            {
                const bool can_player_see_mon =
                    map::player->can_see_actor(*owning_actor_);

                const std::string mon_name =
                    can_player_see_mon ?
                    text_format::first_to_upper(owning_actor_->name_the()) :
                    "It";

                msg_log::add(mon_name + " " + res_data.resist_msg_mon);
            }
        }
    }

    return res_data.is_resisted;
}

bool PropHandler::allow_see() const
{
    for (Prop* p : props_)
    {
        if (!p->allow_see())
        {
            return false;
        }
    }

    return true;
}

int PropHandler::affect_max_hp(const int hp_max) const
{
    int new_hp_max = hp_max;

    for (Prop* prop : props_)
    {
        new_hp_max = prop->affect_max_hp(new_hp_max);
    }

    return new_hp_max;
}

int PropHandler::affect_max_spi(const int spi_max) const
{
    int new_spi_max = spi_max;

    for (Prop* prop : props_)
    {
        new_spi_max = prop->affect_max_spi(new_spi_max);
    }

    return new_spi_max;
}

int PropHandler::affect_shock(const int shock) const
{
    int new_shock = shock;

    for (Prop* prop : props_)
    {
        new_shock = prop->affect_shock(new_shock);
    }

    return new_shock;
}

void PropHandler::affect_move_dir(const P& actor_pos, Dir& dir) const
{
    for (Prop* prop : props_)
    {
        prop->affect_move_dir(actor_pos, dir);
    }
}

bool PropHandler::allow_attack(const Verbosity verbosity) const
{
    for (Prop* prop : props_)
    {
        if (!prop->allow_attack_melee(verbosity) &&
            !prop->allow_attack_ranged(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_attack_melee(const Verbosity verbosity) const
{
    for (Prop* prop : props_)
    {
        if (!prop->allow_attack_melee(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_attack_ranged(const Verbosity verbosity) const
{
    for (Prop* prop : props_)
    {
        if (!prop->allow_attack_ranged(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_move() const
{
    for (Prop* prop : props_)
    {
        if (!prop->allow_move())
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_act() const
{
    for (Prop* prop : props_)
    {
        if (!prop->allow_act())
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_read_absolute(const Verbosity verbosity) const
{
    for (auto prop : props_)
    {
        if (!prop->allow_read_absolute(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_read_chance(const Verbosity verbosity) const
{
    for (auto prop : props_)
    {
        if (!prop->allow_read_chance(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_cast_intr_spell_absolute(
    const Verbosity verbosity) const
{
    for (auto prop : props_)
    {
        if (!prop->allow_cast_intr_spell_absolute(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_cast_intr_spell_chance(
    const Verbosity verbosity) const
{
    for (auto prop : props_)
    {
        if (!prop->allow_cast_intr_spell_chance(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_speak(const Verbosity verbosity) const
{
    for (auto prop : props_)
    {
        if (!prop->allow_speak(verbosity))
        {
            return false;
        }
    }

    return true;
}

bool PropHandler::allow_eat(const Verbosity verbosity) const
{
    for (auto prop : props_)
    {
        if (!prop->allow_eat(verbosity))
        {
            return false;
        }
    }

    return true;
}

void PropHandler::on_hit()
{
    for (Prop* prop : props_)
    {
        prop->on_hit();
    }
}

void PropHandler::on_death(const bool is_player_see_owning_actor)
{
    TRACE_FUNC_BEGIN_VERBOSE;

    for (Prop* prop : props_)
    {
        prop->on_death(is_player_see_owning_actor);
    }

    TRACE_FUNC_END_VERBOSE;
}

int PropHandler::ability_mod(const AbilityId ability) const
{
    int modifier = 0;

    for (Prop* prop : props_)
    {
        modifier += prop->ability_mod(ability);
    }

    return modifier;
}

bool PropHandler::affect_actor_clr(Clr& clr) const
{
    bool did_affect_clr = false;

    for (Prop* prop : props_)
    {
        if (prop->affect_actor_clr(clr))
        {
            did_affect_clr = true;

            // It's probably more likely that a color change due to a bad
            // property is critical information (e.g. burning), so then we stop
            // searching and use this color. If it's a good or neutral property
            // that affected the color, then we keep searching.
            if (prop->alignment() == PropAlignment::bad)
            {
                break;
            }
        }
    }

    return did_affect_clr;
}

// -----------------------------------------------------------------------------
// Properties
// -----------------------------------------------------------------------------
Prop::Prop(PropId id, PropTurns turns_init, int nr_turns) :
    id_                 (id),
    data_               (prop_data::data[(size_t)id]),
    nr_turns_left_      (nr_turns),
    turns_init_type_    (turns_init),
    owning_actor_       (nullptr),
    src_                (PropSrc::END),
    item_applying_      (nullptr)
{
    switch (turns_init)
    {
    case PropTurns::std:
#ifndef NDEBUG
        if (nr_turns_left_ != -1)
        {
            TRACE << "Prop turns is \"std\", but " << nr_turns_left_
                  << " turns specified" << std::endl;
            ASSERT(false);
        }
#endif // NDEBUG
        ASSERT(nr_turns_left_ == -1);
        nr_turns_left_ = data_.std_rnd_turns.roll();
        break;

    case PropTurns::indefinite:
#ifndef NDEBUG
        if (nr_turns_left_ != -1)
        {
            TRACE << "Prop turns is \"indefinite\", but " << nr_turns_left_
                  << " turns specified" << std::endl;
            ASSERT(false);
        }
#endif // NDEBUG

        nr_turns_left_ = -1; // Robustness for release builds
        break;

    case PropTurns::specific:
        // Use the number of turns specified in the ctor argument
        ASSERT(nr_turns_left_ > 0);
        break;
    }
}

void PropBlessed::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::cursed, false);

    bless_adjacent();
}

void PropBlessed::on_more(const Prop& new_prop)
{
    (void)new_prop;

    bless_adjacent();
}

void PropBlessed::bless_adjacent() const
{
    // "Bless" adjacent fountains
    const P& p = owning_actor_->pos;

    for (const P& d : dir_utils::dir_list_w_center)
    {
        const P p_adj(p + d);

        Cell& cell = map::cells[p_adj.x][p_adj.y];

        Rigid* const rigid = cell.rigid;

        if (rigid->id() == FeatureId::fountain)
        {
            Fountain* const fountain = static_cast<Fountain*>(rigid);

            const auto effect = fountain->effect();

            if (((int)effect > (int)FountainEffect::START_OF_BAD_EFFECTS) &&
                fountain->has_drinks_left())
            {
                fountain->set_effect(FountainEffect::refreshing);

                if (cell.is_seen_by_player)
                {
                    std::string name = fountain->name(Article::the);

                    name = text_format::first_to_lower(name);

                    msg_log::add("The water in " + name + " seems clearer.");
                }
            }
        }
    }
}

void PropCursed::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::blessed, false);

    curse_adjacent();

    // If this is a permanent curse, log it as a historic event
    if (owning_actor_->is_player() &&
        turns_init_type_ == PropTurns::indefinite)
    {
        game::add_history_event("A terrible curse was put upon me.");
    }
}

void PropCursed::on_more(const Prop& new_prop)
{
    (void)new_prop;

    curse_adjacent();
}

void PropCursed::on_end()
{
    // If this was a permanent curse, log it as a historic event
    if (owning_actor_->is_player() &&
        turns_init_type_ == PropTurns::indefinite)
    {
        game::add_history_event("A terrible curse was lifted from me.");
    }
}

void PropCursed::curse_adjacent() const
{
    // "Curse" adjacent fountains
    const P& p = owning_actor_->pos;

    for (const P& d : dir_utils::dir_list_w_center)
    {
        const P p_adj(p + d);

        Cell& cell = map::cells[p_adj.x][p_adj.y];

        Rigid* const rigid = cell.rigid;

        if (rigid->id() == FeatureId::fountain)
        {
            Fountain* const fountain = static_cast<Fountain*>(rigid);

            const auto effect = fountain->effect();

            if (((int)effect < (int)FountainEffect::START_OF_BAD_EFFECTS) &&
                fountain->has_drinks_left())
            {
                const int min = (int)FountainEffect::START_OF_BAD_EFFECTS + 1;
                const int max = (int)FountainEffect::END - 1;

                fountain->set_effect((FountainEffect)rnd::range(min, max));

                if (cell.is_seen_by_player)
                {
                    std::string name = fountain->name(Article::the);

                    name = text_format::first_to_lower(name);

                    msg_log::add("The water in " + name + " seems murkier.");
                }
            }
        }
    }
}

void PropSlowed::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::hasted, false);
}

void PropHasted::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::slowed, false);
}

void PropSummoned::on_end()
{
    owning_actor_->destroy_silent();
}

Prop* PropInfected::on_tick()
{
#ifndef NDEBUG
    ASSERT(!owning_actor_->prop_handler().has_prop(PropId::diseased));
#endif // NDEBUG

    // Don't trigger the effect if the player is currently treating the infction
    if (map::player->active_medical_bag)
    {
        ++nr_turns_left_;

        return this;
    }

    // Increase risk of disease the fewer turns left the infection has
    const int allow_disease_below_turns_left = 50;

    const int apply_disease_one_in = nr_turns_left_ - 1;

    if ((nr_turns_left_ <= allow_disease_below_turns_left) &&
        ((apply_disease_one_in <= 0) ||
         rnd::one_in(apply_disease_one_in)))
    {
        PropHandler& prop_hlr = owning_actor_->prop_handler();

        prop_hlr.apply(new PropDiseased(PropTurns::indefinite));

        //
        // NOTE: Disease ends infection, this property object is now deleted!
        //

        msg_log::more_prompt();

        return nullptr;
    }

    return this;
}

int PropDiseased::affect_max_hp(const int hp_max) const
{
#ifndef NDEBUG
    ASSERT(!owning_actor_->prop_handler().has_prop(PropId::infected));
#endif // NDEBUG

    return hp_max / 2;
}

void PropDiseased::on_start()
{
    // End infection
    owning_actor_->prop_handler().end_prop(PropId::infected, false);

    // If this is a permanent disease that the player caught, log it as a
    // historic event
    if (owning_actor_->is_player() && turns_init_type_ == PropTurns::indefinite)
    {
        game::add_history_event("Caught a horrible disease.");
    }
}

void PropDiseased::on_end()
{
#ifndef NDEBUG
    ASSERT(!owning_actor_->prop_handler().has_prop(PropId::infected));
#endif // NDEBUG

    // If this is a permanent disease that the player caught, log it as a
    // historic event
    if (owning_actor_->is_player() &&
        turns_init_type_ == PropTurns::indefinite)
    {
        game::add_history_event("My body was cured from a horrible disease.");
    }
}

bool PropDiseased::is_resisting_other_prop(const PropId prop_id) const
{
#ifndef NDEBUG
    ASSERT(!owning_actor_->prop_handler().has_prop(PropId::infected));
#endif // NDEBUG

    // Getting infected while already diseased is just annoying
    return prop_id == PropId::infected;
}

Prop* PropDescend::on_tick()
{
    ASSERT(owning_actor_->is_player());

    if (nr_turns_left_ <= 1)
    {
        game_time::is_magic_descend_nxt_std_turn = true;
    }

    return this;
}

void PropPossByZuul::on_death(const bool is_player_see_owning_actor)
{
    if (is_player_see_owning_actor)
    {
        const std::string& name1 =
            text_format::first_to_upper(
                owning_actor_->name_the());

        const std::string& name2 =
            actor_data::data[(size_t)ActorId::zuul].name_the;

        msg_log::add(name1 + " was possessed by " + name2 + "!");
    }

    owning_actor_->state_ = ActorState::destroyed;

    const P& pos = owning_actor_->pos;

    map::mk_gore(pos);

    map::mk_blood(pos);

    actor_factory::spawn(pos,
                         std::vector<ActorId> {ActorId::zuul},
                         MakeMonAware::yes);

    // Zuul is now free, allow him to spawn.
    actor_data::data[(size_t)ActorId::zuul].nr_left_allowed_to_spawn = -1;
}

Prop* PropPoisoned::on_tick()
{
    if (owning_actor_->is_alive() &&
        (game_time::turn_nr() % poison_dmg_n_turn) == 0)
    {
        if (owning_actor_->is_player())
        {
            msg_log::add("I am suffering from the poison!",
                         clr_msg_bad,
                         true);
        }
        else // Is monster
        {
            if (map::player->can_see_actor(*owning_actor_))
            {
                const std::string actor_name_the =
                    text_format::first_to_upper(
                        owning_actor_->name_the());

                msg_log::add(actor_name_the +
                             " suffers from poisoning!");
            }
        }

        owning_actor_->hit(1, DmgType::pure);
    }

    return this;
}

bool PropTerrified::allow_attack_melee(const Verbosity verbosity) const
{
    if (owning_actor_->is_player() && verbosity == Verbosity::verbose)
    {
        msg_log::add("I am too terrified to engage in close combat!");
    }

    return false;
}

bool PropTerrified::allow_attack_ranged(const Verbosity verbosity) const
{
    (void)verbosity;
    return true;
}

void PropTerrified::on_start()
{
    // If this is a monster, we reset its last direction moved. Otherwise it
    // would probably tend to move toward the player even while terrified (the
    // AI would typically use the idle movement algorithm, which favors stepping
    // in the same direction as the last move).

    if (!owning_actor_->is_player())
    {
        Mon* const mon = static_cast<Mon*>(owning_actor_);

        mon->last_dir_moved_ = Dir::center;
    }
}

void PropNailed::affect_move_dir(const P& actor_pos, Dir& dir)
{
    (void)actor_pos;

    if (dir != Dir::center)
    {
        if (owning_actor_->is_player())
        {
            msg_log::add("I struggle to tear out the spike!", clr_msg_bad);
        }
        else // Is monster
        {
            if (map::player->can_see_actor(*owning_actor_))
            {
                const std::string actor_name_the =
                    text_format::first_to_upper(
                        owning_actor_->name_the());

                msg_log::add(actor_name_the +  " struggles in pain!",
                             clr_msg_good);
            }
        }

        owning_actor_->hit(rnd::dice(1, 3), DmgType::physical);

        if (owning_actor_->is_alive())
        {

            // TODO: reimplement something affecting chance of success?

            if (rnd::one_in(4))
            {
                --nr_spikes_;

                if (nr_spikes_ > 0)
                {
                    if (owning_actor_->is_player())
                    {
                        msg_log::add("I rip out a spike from my flesh!");
                    }
                    else // Is monster
                    {
                        if (map::player->can_see_actor(*owning_actor_))
                        {
                            const std::string actor_name_the =
                                text_format::first_to_upper(
                                    owning_actor_->name_the());

                            msg_log::add(actor_name_the +
                                         " tears out a spike!");
                        }
                    }
                }
            }
        }

        dir = Dir::center;
    }
}

void PropWound::save() const
{
    saving::put_int(nr_wounds_);
}

void PropWound::load()
{
    nr_wounds_ = saving::get_int();
}

void PropWound::msg(const PropMsg msg_type, std::string& msg_ref) const
{
    switch (msg_type)
    {
    case PropMsg::start_player:
    case PropMsg::res_player:
        msg_ref = data_.msg[(size_t)msg_type];
        break;

    case PropMsg::end_player:
        msg_ref =
            (nr_wounds_ > 1) ?
            "All my wounds are healed!" :
            "A wound is healed!";
        break;

    case PropMsg::start_mon:
    case PropMsg::end_mon:
    case PropMsg::res_mon:
    case PropMsg::END:
        msg_ref = "";
        break;
    }
}

int PropWound::ability_mod(const AbilityId ability) const
{
    // A player with Survivalist receives no ability penalties
    if (owning_actor_->is_player() &&
        player_bon::traits[(size_t)Trait::survivalist])
    {
        return 0;
    }

    if (ability == AbilityId::melee)
    {
        return (nr_wounds_ * -5);
    }
    else if (ability == AbilityId::dodging)
    {
        return (nr_wounds_ * -5);
    }

    return 0;
}

int PropWound::affect_max_hp(const int hp_max) const
{
    const int pen_pct_per_wound = 10;

    int hp_pen_pct = nr_wounds_ * pen_pct_per_wound;

    // The HP penalty is halved for a player with Survivalist
    if (owning_actor_->is_player() &&
        player_bon::traits[(size_t)Trait::survivalist])
    {
        hp_pen_pct /= 2;
    }

    // Cap the penalty percentage
    hp_pen_pct = std::min(70, hp_pen_pct);

    return (hp_max * (100 - hp_pen_pct)) / 100;
}

void PropWound::heal_one_wound()
{
    ASSERT(nr_wounds_ > 0);

    --nr_wounds_;

    if (nr_wounds_ > 0)
    {
        msg_log::add("A wound is healed.");
    }
    else // This was the last wound
    {
        // End self
        owning_actor_->prop_handler().end_prop(id());
    }
}

void PropWound::on_more(const Prop& new_prop)
{
    (void)new_prop;

    ++nr_wounds_;

    if (nr_wounds_ >= 5)
    {
        if (owning_actor_ == map::player)
        {
            msg_log::add("I die from my wounds!");
        }

        owning_actor_->die(false, false, true);
    }
}

PropHpSap::PropHpSap(PropTurns turns_init, int nr_turns) :
    Prop        (PropId::hp_sap, turns_init, nr_turns),
    nr_drained_ (rnd::range(1, 3)) {}

void PropHpSap::save() const
{
    saving::put_int(nr_drained_);
}

void PropHpSap::load()
{
    nr_drained_ = saving::get_int();
}

int PropHpSap::affect_max_hp(const int hp_max) const
{
    return (hp_max - nr_drained_);
}

void PropHpSap::on_more(const Prop& new_prop)
{
    nr_drained_ += static_cast<const PropHpSap*>(&new_prop)->nr_drained_;
}

PropSpiSap::PropSpiSap(PropTurns turns_init, int nr_turns) :
    Prop        (PropId::spi_sap, turns_init, nr_turns),
    nr_drained_ (1) {}

void PropSpiSap::save() const
{
    saving::put_int(nr_drained_);
}

void PropSpiSap::load()
{
    nr_drained_ = saving::get_int();
}

int PropSpiSap::affect_max_spi(const int spi_max) const
{
    return (spi_max - nr_drained_);
}

void PropSpiSap::on_more(const Prop& new_prop)
{
    nr_drained_ += static_cast<const PropSpiSap*>(&new_prop)->nr_drained_;
}

PropMindSap::PropMindSap(PropTurns turns_init, int nr_turns) :
    Prop        (PropId::mind_sap, turns_init, nr_turns),
    nr_drained_ (rnd::range(1, 3)) {}

void PropMindSap::save() const
{
    saving::put_int(nr_drained_);
}

void PropMindSap::load()
{
    nr_drained_ = saving::get_int();
}

int PropMindSap::affect_shock(const int shock) const
{
    return (shock + nr_drained_);
}

void PropMindSap::on_more(const Prop& new_prop)
{
    nr_drained_ += static_cast<const PropMindSap*>(&new_prop)->nr_drained_;
}

bool PropConfused::allow_read_absolute(const Verbosity verbosity) const
{
    if (owning_actor_->is_player() && verbosity == Verbosity::verbose)
    {
        msg_log::add("I am too confused to read.");
    }

    return false;
}

bool PropConfused::allow_cast_intr_spell_absolute(
    const Verbosity verbosity) const
{
    if (owning_actor_->is_player() &&
        (verbosity == Verbosity::verbose))
    {
        msg_log::add("I am too confused to concentrate!");
    }

    return false;
}

bool PropConfused::allow_attack_melee(const Verbosity verbosity) const
{
    (void)verbosity;

    if (owning_actor_ != map::player)
    {
        return rnd::coin_toss();
    }

    return true;
}

bool PropConfused::allow_attack_ranged(const Verbosity verbosity) const
{
    (void)verbosity;

    if (owning_actor_ != map::player)
    {
        return rnd::coin_toss();
    }

    return true;
}

void PropConfused::affect_move_dir(const P& actor_pos, Dir& dir)
{
    if (dir == Dir::center)
    {
        return;
    }

    bool blocked[map_w][map_h];

    const R area_check_blocked(actor_pos - P(1, 1),
                               actor_pos + P(1, 1));

    map_parsers::BlocksActor(*owning_actor_, ParseActors::yes)
        .run(blocked,
             MapParseMode::overwrite,
             area_check_blocked);

    if (rnd::one_in(8))
    {
        std::vector<P> d_bucket;

        for (const P& d : dir_utils::dir_list)
        {
            const P tgt_p(actor_pos + d);

            if (!blocked[tgt_p.x][tgt_p.y])
            {
                d_bucket.push_back(d);
            }
        }

        if (!d_bucket.empty())
        {
            const P& d = rnd::element(d_bucket);

            dir = dir_utils::dir(d);
        }
    }
}

Prop* PropStrangled::on_tick()
{
    const int dmg = rnd::range(3, 4);

    owning_actor_->hit(dmg, DmgType::pure, DmgMethod::forced);

    return this;
}

bool PropStrangled::allow_speak(const Verbosity verbosity) const
{
    if (verbosity == Verbosity::verbose && owning_actor_->is_player())
    {
        msg_log::add("My throat is constricted.");
    }

    return false;
}

bool PropStrangled::allow_eat(const Verbosity verbosity) const
{
    if (verbosity == Verbosity::verbose && owning_actor_->is_player())
    {
        msg_log::add("My throat is constricted.");
    }

    return false;
}

void PropFrenzied::affect_move_dir(const P& actor_pos, Dir& dir)
{
    if (!owning_actor_->is_player() ||
        (dir == Dir::center))
    {
        return;
    }

    const auto seen_foes = owning_actor_->seen_foes();

    if (seen_foes.empty())
    {
        return;
    }

    std::vector<P> seen_foes_cells;

    seen_foes_cells.clear();

    for (auto* actor : seen_foes)
    {
        seen_foes_cells.push_back(actor->pos);
    }

    sort(begin(seen_foes_cells),
         end(seen_foes_cells),
         IsCloserToPos(actor_pos));

    const P& closest_mon_pos = seen_foes_cells[0];

    bool blocked[map_w][map_h];

    map_parsers::BlocksActor(*owning_actor_, ParseActors::no)
        .run(blocked);

    std::vector<P> line;

    line_calc::calc_new_line(actor_pos,
                             closest_mon_pos,
                             true,
                             999,
                             false,
                             line);

    if (line.size() > 1)
    {
        for (P& pos : line)
        {
            if (blocked[pos.x][pos.y])
            {
                return;
            }
        }

        dir = dir_utils::dir(line[1] - actor_pos);
    }
}

bool PropFrenzied::is_resisting_other_prop(const PropId prop_id) const
{
    return
        prop_id == PropId::confused ||
        prop_id == PropId::fainted ||
        prop_id == PropId::terrified ||
        prop_id == PropId::weakened;
}

void PropFrenzied::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::confused);
    owning_actor_->prop_handler().end_prop(PropId::fainted);
    owning_actor_->prop_handler().end_prop(PropId::terrified);
    owning_actor_->prop_handler().end_prop(PropId::weakened);
}

void PropFrenzied::on_end()
{
    // Only the player (except for Ghoul background) gets tired after a frenzy
    // (it looks weird for monsters)
    if (owning_actor_->is_player() &&
        (player_bon::bg() != Bg::ghoul))
    {
        owning_actor_->prop_handler().apply(new PropWeakened(PropTurns::std));
    }
}

bool PropFrenzied::allow_read_absolute(const Verbosity verbosity) const
{
    if (owning_actor_->is_player() && verbosity == Verbosity::verbose)
    {
        msg_log::add("I am too enraged to read!");
    }

    return false;
}

bool PropFrenzied::allow_cast_intr_spell_absolute(
    const Verbosity verbosity) const
{
    if (owning_actor_->is_player() &&
        (verbosity == Verbosity::verbose))
    {
        msg_log::add("I am too enraged to concentrate!");
    }

    return false;
}

Prop* PropBurning::on_tick()
{
    if (owning_actor_->is_player())
    {
        msg_log::add("AAAARGH IT BURNS!!!", clr_red_lgt);
    }

    owning_actor_->hit(rnd::dice(1, 3), DmgType::fire);

    return this;
}

bool PropBurning::allow_read_chance(const Verbosity verbosity) const
{
    if (!rnd::coin_toss())
    {
        if (owning_actor_->is_player() &&
            (verbosity == Verbosity::verbose))
        {
            msg_log::add("I fail to concentrate!");
        }

        return false;
    }

    return true;
}

bool PropBurning::allow_cast_intr_spell_chance(const Verbosity verbosity) const
{
    if (!rnd::coin_toss())
    {
        if (owning_actor_->is_player() &&
            (verbosity == Verbosity::verbose))
        {
            msg_log::add("I fail to concentrate!");
        }

        return false;
    }

    return true;
}

bool PropBurning::allow_attack_ranged(const Verbosity verbosity) const
{
    if (owning_actor_->is_player() &&
        (verbosity == Verbosity::verbose))
    {
        msg_log::add("Not while burning.");
    }

    return false;
}

bool PropBlind::allow_read_absolute(const Verbosity verbosity) const
{
    if (owning_actor_->is_player() &&
        (verbosity == Verbosity::verbose))
    {
        msg_log::add("I cannot read while blind.");
    }

    return false;
}

bool PropBlind::need_update_vision_when_start_or_end() const
{
    return owning_actor_->is_player();
}

void PropParalyzed::on_start()
{
    auto* const player = map::player;

    if (owning_actor_->is_player())
    {
        auto* const active_explosive = player->active_explosive;

        if (active_explosive)
        {
            active_explosive->on_player_paralyzed();
        }
    }
}

bool PropFainted::need_update_vision_when_start_or_end() const
{
    return owning_actor_->is_player();
}

Prop* PropFlared::on_tick()
{
    owning_actor_->hit(1, DmgType::fire);

    if (nr_turns_left_ <= 1)
    {
        owning_actor_->prop_handler().apply(new PropBurning(PropTurns::std));
        owning_actor_->prop_handler().end_prop(id());

        return nullptr;
    }

    return this;
}

DmgResistData PropRAcid::is_resisting_dmg(const DmgType dmg_type) const
{
    DmgResistData d;

    d.is_resisted = dmg_type == DmgType::acid;

    d.resist_msg_player = "I feel a faint burning sensation.";

    d.resist_msg_mon = "seems unaffected.";

    return d;
}

DmgResistData PropRElec::is_resisting_dmg(const DmgType dmg_type) const
{
    DmgResistData d;

    d.is_resisted = dmg_type == DmgType::electric;

    d.resist_msg_player = "I feel a faint tingle.";

    d.resist_msg_mon = "seems unaffected.";

    return d;
}

bool PropRConf::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::confused;
}

void PropRConf::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::confused);
}

bool PropRFear::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::terrified;
}

void PropRFear::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::terrified);

    if (owning_actor_->is_player() &&
        turns_init_type_ == PropTurns::indefinite)
    {
        insanity::on_permanent_rfear();
    }
}

bool PropRSlow::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::slowed;
}

void PropRSlow::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::slowed);
}

bool PropRPhys::is_resisting_other_prop(const PropId prop_id) const
{
    (void)prop_id;
    return false;
}

void PropRPhys::on_start()
{

}

DmgResistData PropRPhys::is_resisting_dmg(const DmgType dmg_type) const
{
    DmgResistData d;

    d.is_resisted = dmg_type == DmgType::physical;

    d.resist_msg_player = "I resist harm.";

    d.resist_msg_mon = "seems unharmed.";

    return d;
}

bool PropRFire::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::burning;
}

void PropRFire::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::burning);
}

DmgResistData PropRFire::is_resisting_dmg(const DmgType dmg_type) const
{
    DmgResistData d;

    d.is_resisted = dmg_type == DmgType::fire;

    d.resist_msg_player = "I feel warm.";

    d.resist_msg_mon = "seems unaffected.";

    return d;
}

bool PropRPoison::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::poisoned;
}

void PropRPoison::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::poisoned);
}

bool PropRSleep::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::fainted;
}

void PropRSleep::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::fainted);
}

bool PropRDisease::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::diseased || prop_id == PropId::infected;
}

void PropRDisease::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::diseased);
    owning_actor_->prop_handler().end_prop(PropId::infected);
}

bool PropRBlind::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::blind;
}

void PropRBlind::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::blind);
}

bool PropRPara::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::paralyzed;
}

void PropRPara::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::paralyzed);
}

bool PropSeeInvis::is_resisting_other_prop(const PropId prop_id) const
{
    return prop_id == PropId::blind;
}

void PropSeeInvis::on_start()
{
    owning_actor_->prop_handler().end_prop(PropId::blind);
}

Prop* PropBurrowing::on_tick()
{
    const P& p = owning_actor_->pos;

    map::cells[p.x][p.y].rigid->hit(1, // Doesn't matter
                                    DmgType::physical,
                                    DmgMethod::forced);

    return this;
}
