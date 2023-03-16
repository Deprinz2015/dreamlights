//
// Created by Niklas Koll on 20.07.22.
//

#include "Constants.h"

#ifdef SK9822_TYPE
const char *host = "ledsk9822";
//const char *model = "SK9822";
//const char *sn = "123-ABC-SK9822";
//const char *name = "Dreamlight-SK9822";
#endif
#ifdef WS2812B_TYPE
const char *host = "ledws2812b";
//const char *model = "WS2812B";
//const char *sn = "123-ABC-WS2812B";
//const char *name = "Dreamlight-WS2812B";
#endif

const char *main_config_file_name = "/config.cnf";
const char *effect_list_file_name = "/fxlist.cnf";
const char *preset_list_file_name = "/presets.cnf";
const char *preset_file_template = "/Presets/preset{id}.cnf";
const char *effect_config_template = "/Configs/fx{id}.cnf";
const char *effect_file_template = "/Effects/fx{id}.lua";