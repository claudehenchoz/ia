#include "init.hpp"

#include "rl_utils.hpp"
#include "state.hpp"
#include "io.hpp"
#include "init.hpp"
#include "main_menu.hpp"

#ifdef _WIN32
#undef main
#endif
int main(int argc, char** argv)
{
    TRACE_FUNC_BEGIN;

#ifdef NDEBUG
    (void)argc;
    (void)argv;
#else // NDEBUG
    for (int arg_nr = 0; arg_nr < argc; ++arg_nr)
    {
        const std::string arg_str = std::string(argv[arg_nr]);

        if (arg_str == "--demo-mapgen")
        {
            init::is_demo_mapgen = true;
        }
    }
#endif // NDEBUG

    rnd::seed();

    init::init_io();
    init::init_game();

    std::unique_ptr<State> main_menu_state(new MainMenuState);

    states::push(std::move(main_menu_state));

    // Loop while there is at least one state
    while (!states::is_empty())
    {
        states::start();

        if (states::is_empty())
        {
            break;
        }

        io::clear_screen();

        states::draw();

        io::update_screen();

        states::update();
    }

    init::cleanup_session();
    init::cleanup_game();
    init::cleanup_io();

    return 0;
}
