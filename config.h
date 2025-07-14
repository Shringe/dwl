/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }
/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const int smartgaps                 = 0;  /* 1 means no outer gap when there is only one window */
static int gaps                            = 1;  /* 1 means gaps between windows are added */
static const unsigned int gappx            = 4;  /* gap pixel between windows */
static const unsigned int borderpx         = 2;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x000000ff);
static const float bordercolor[]           = COLOR(0x45475aff);
static const float focuscolor[]            = COLOR(0x89b4faff);
static const float urgentcolor[]           = COLOR(0xf38ba8ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.1f, 0.1f, 0.1f, 0.0f}; /* You can also use glsl colors */

static const int opacity = 0; /* flag to enable opacity */
static const float opacity_inactive = 0.9;
static const float opacity_active = 1.0;

static const int shadow = 0; /* flag to enable shadow */
static const int shadow_only_floating = 1; /* only apply shadow to floating windows */
static const float shadow_color[4] = COLOR(0x0000FFff);
static const float shadow_color_focus[4] = COLOR(0xFF0000ff);
static const int shadow_blur_sigma = 20;
static const int shadow_blur_sigma_focus = 40;
static const char *const shadow_ignore_list[] = { NULL }; /* list of app-id to ignore */

static const int corner_radius = 8; /* 0 disables corner_radius */
static const int corner_radius_inner = 9; /* 0 disables corner_radius */
static const int corner_radius_only_floating = 0; /* only apply corner_radius and corner_radius_inner to floating windows */

static const int blur = 1; /* flag to enable blur */
static const int blur_xray = 0; /* flag to make transparent fs and floating windows display your background */
static const int blur_ignore_transparent = 1;
static const struct blur_data blur_data = {
	.radius = 5,
	.num_passes = 2,
	.noise = (float)0.02,
	.brightness = (float)0.9,
	.contrast = (float)0.9,
	.saturation = (float)1.1,
};

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	/* examples: */
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact nmaster scale layout       rotate/reflect              x  y  resx resy rate mode adaptive*/
	/* example of a HiDPI laptop monitor at 120Hz:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 0, 0, 0, 0, 120.000f, 1, 1},
	* mode let's the user decide on how dwl should implement the modes:
	* -1 Sets a custom mode following the users choice
	* All other number's set the mode at the index n, 0 is the standard mode; see wlr-randr
	*/
	/* defaults */
	{ "HDMI-A-1", 0.5f,  1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 2560, 0, 0, 0, 0.0f, 5, 0},
	{ "DP-1",     0.5f,  2,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, 0, 0, 0, 0, 0.0f, 1, 0},
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL, -1, -1, 0, 0, 0.0f, 0 ,1},
};

/* keyboard */
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = NULL,
};

static const int repeat_rate = 35;
static const int repeat_delay = 500;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 0;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT;
static const double accel_speed = -0.675;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
	{ MODKEY,                    KEY,            view,            {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview,      {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tag,             {.ui = 1 << TAG} }, \
	{ MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag, {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
// #define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
// Screenshots
static const char *screenshot_fullscreen[] = { "sh", "-c",
  "grim \"$HOME/Pictures/screenshots/dwl/$(date +%Y-%m-%d_%H-%m-%s).png\"", NULL };

static const char *screenshot_selection[] = { "sh", "-c",
  "grim -g \"$(slurp)\" \"$HOME/Pictures/screenshots/dwl/$(date +%Y-%m-%d_%H-%m-%s).png\"", NULL };

// Media
static const char *volumeup[]             = { "pactl", "set-sink-volume",   "@DEFAULT_SINK@",   "+5%",    NULL };
static const char *volumedown[]           = { "pactl", "set-sink-volume",   "@DEFAULT_SINK@",   "-5%",    NULL };
static const char *volumemute[]           = { "pactl", "set-sink-mute",     "@DEFAULT_SINK@",   "toggle", NULL };

static const char *micvolup[]             = { "pactl", "set-source-volume", "@DEFAULT_SOURCE@", "+5%",    NULL };
static const char *micvoldown[]           = { "pactl", "set-source-volume", "@DEFAULT_SOURCE@", "-5%",    NULL };
static const char *micvolmute[]           = { "pactl", "set-source-mute",   "@DEFAULT_SOURCE@", "toggle", NULL };

static const char *brightnessup[]         = { "brightnessctl", "set",       "5%+",              NULL };
static const char *brightnessdown[]       = { "brightnessctl", "set",       "5%-",              NULL };

static const char *playershuffletoggle[]  = { "playerctl", "shuffle",       "Toggle",           NULL };
static const char *playerlooptrack[]      = { "playerctl", "loop",          "Track",            NULL };
static const char *playerloopnone[]       = { "playerctl", "loop",          "None",             NULL };
static const char *playerloopplaylist[]   = { "playerctl", "loop",          "Playlist",         NULL };

static const char *playervolup[]          = { "playerctl", "volume",        "0.1+",             NULL };
static const char *playervoldown[]        = { "playerctl", "volume",        "0.1-",             NULL };
static const char *playervolmute[]        = { "playerctl", "volume",        "0"   ,             NULL };

static const char *playerplaypause[]      = { "playerctl", "play-pause",    NULL };
static const char *playernext[]           = { "playerctl", "next",          NULL };
static const char *playerprev[]           = { "playerctl", "previous",      NULL };

// Menus
static const char *menucmd[] = { "wofi", "--show", "drun", NULL };
static const char *emojicmd[] = { "wofi-emoji", NULL };
static const char *cliphistcmd[] = { "sh", "-c", 
  "cliphist list | wofi --dmenu | cliphist decode | wl-copy", NULL };

// Apps
static const char *termcmd[] = { "wezterm", NULL };
static const char *browsercmd[] = { "zen", NULL };
static const char *notescmd[] = { "joplin-desktop", NULL };

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function          argument */
  // Screenshots
  { 0,                         XKB_KEY_Print,      spawn,            {.v = screenshot_fullscreen } },
  { WLR_MODIFIER_SHIFT,        XKB_KEY_Print,      spawn,            {.v = screenshot_selection } },

  // Media
  { 0,  XKB_KEY_XF86AudioRaiseVolume,                  spawn,        {.v = volumeup } },
  { 0,  XKB_KEY_XF86AudioLowerVolume,                  spawn,        {.v = volumedown } },
  { 0,  XKB_KEY_XF86AudioMute,                         spawn,        {.v = volumemute } },

  { WLR_MODIFIER_CTRL,  XKB_KEY_XF86AudioRaiseVolume,  spawn,        {.v = micvolup } },
  { WLR_MODIFIER_CTRL,  XKB_KEY_XF86AudioLowerVolume,  spawn,        {.v = micvoldown } },
  { WLR_MODIFIER_CTRL,  XKB_KEY_XF86AudioMute,         spawn,        {.v = micvolmute } },

  { 0,  XKB_KEY_XF86MonBrightnessUp,                   spawn,        {.v = brightnessup } },
  { 0,  XKB_KEY_XF86MonBrightnessDown,                 spawn,        {.v = brightnessdown } },

  { WLR_MODIFIER_SHIFT, XKB_KEY_XF86MonBrightnessUp,   spawn,        {.v = playershuffletoggle } },
  { WLR_MODIFIER_SHIFT, XKB_KEY_XF86MonBrightnessDown, spawn,        {.v = playerlooptrack } },
  { WLR_MODIFIER_CTRL,  XKB_KEY_XF86MonBrightnessUp,   spawn,        {.v = playerloopnone } },
  { WLR_MODIFIER_CTRL,  XKB_KEY_XF86MonBrightnessDown, spawn,        {.v = playerloopplaylist } },

  { WLR_MODIFIER_SHIFT, XKB_KEY_XF86AudioRaiseVolume,  spawn,        {.v = playervolup } },
  { WLR_MODIFIER_SHIFT, XKB_KEY_XF86AudioLowerVolume,  spawn,        {.v = playervoldown } },
  { WLR_MODIFIER_SHIFT, XKB_KEY_XF86AudioMute,         spawn,        {.v = playervolmute } },

  { 0,  XKB_KEY_XF86AudioPlay,                         spawn,        {.v = playerplaypause } },
  { 0,  XKB_KEY_XF86AudioNext,                         spawn,        {.v = playernext } },
  { 0,  XKB_KEY_XF86AudioPrev,                         spawn,        {.v = playerprev } },

  // Menus
	{ MODKEY,                    XKB_KEY_s,          spawn,            {.v = menucmd } },
	{ MODKEY,                    XKB_KEY_c,          spawn,            {.v = emojicmd } },
	{ MODKEY,                    XKB_KEY_d,          spawn,            {.v = cliphistcmd } },

  // Apps
	{ MODKEY,                    XKB_KEY_a,          spawn,            {.v = notescmd } },
	{ MODKEY,                    XKB_KEY_r,          spawn,            {.v = browsercmd } },
	{ MODKEY,                    XKB_KEY_Return,     spawn,            {.v = termcmd } },

  // DWL
	{ MODKEY,                    XKB_KEY_e,          focusstack,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_i,          focusstack,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_n,          incnmaster,       {.i = +1} },
	{ MODKEY,                    XKB_KEY_o,          incnmaster,       {.i = -1} },
	{ MODKEY,                    XKB_KEY_l,          setmfact,         {.f = -0.05f} },
	{ MODKEY,                    XKB_KEY_u,          setmfact,         {.f = +0.05f} },
	{ MODKEY,                    XKB_KEY_y,          zoom,             {0} },
	{ MODKEY,                    XKB_KEY_Tab,        view,             {0} },
	{ MODKEY,                    XKB_KEY_w,          killclient,       {0} },
	{ MODKEY,                    XKB_KEY_j,          setlayout,        {.v = &layouts[0]} },
	{ MODKEY,                    XKB_KEY_m,          setlayout,        {.v = &layouts[1]} },
	{ MODKEY,                    XKB_KEY_k,          setlayout,        {.v = &layouts[2]} },
	{ MODKEY,                    XKB_KEY_space,      setlayout,        {0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_space,      togglefloating,   {0} },
	{ MODKEY,                    XKB_KEY_t,          togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_F5,         togglefullscreenadaptivesync, {0} },
  // { MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_G,          togglefullscreenkeyinhibit, {0} },
	{ MODKEY,                    XKB_KEY_g,          togglegaps,       {0} },
	{ MODKEY,                    XKB_KEY_0,          view,             {.ui = ~0} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_parenright, tag,              {.ui = ~0} },
	{ MODKEY,                    XKB_KEY_comma,      focusmon,         {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY,                    XKB_KEY_period,     focusmon,         {.i = WLR_DIRECTION_RIGHT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_less,       tagmon,           {.i = WLR_DIRECTION_LEFT} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_greater,    tagmon,           {.i = WLR_DIRECTION_RIGHT} },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                       0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_at,                           1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_numbersign,                   2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_dollar,                       3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                      4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_asciicircum,                  5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_ampersand,                    6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_asterisk,                     7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenleft,                    8),
	{ MODKEY|WLR_MODIFIER_CTRL,  XKB_KEY_q,          quit,             {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_MIDDLE, togglefloating, {0} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },
};
