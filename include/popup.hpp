#ifndef POPUP_HPP
#define POPUP_HPP

#include <vector>
#include <string>

#include "audio.hpp"

namespace popup
{

void show_msg(const std::string& msg,
              const std::string& title = "",
              const SfxId sfx = SfxId::END,
              const int w_change = 0);

int show_menu_msg(const std::string& msg,
                  const std::vector<std::string>& choices,
                  const std::string& title = "",
                  const SfxId sfx = SfxId::END);

} // popup

#endif
