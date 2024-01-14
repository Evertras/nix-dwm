/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = ${toString gappx}; /* gaps between windows */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "${fontName}:size=${toString fontSize}" };
static const char col_background[]  = "${colorBackground}";
static const char col_text[]        = "${colorText}";
static const char col_primary[]     = "${colorPrimary}";
static const char *colors[][3]      = {
	/*                fg              bg              border   */
	[SchemeNorm]  = { col_text,       col_background, col_primary },
	[SchemeSel]   = { col_background, col_primary,    col_primary },
	[SchemeTitle] = { col_primary,    col_background, col_primary },
};

static const char *const autostart[] = {
	/* TODO: Make this configurable */
	"sh", "-c", "while true; do xsetroot -name \"$(date '+%a %m-%d %H:%M ')\"; sleep 1m; done", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* first entry is default */
	/* symbol     arrange function */
	{ "[|3",      tile },
	{ "vvv",      bstack },
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY ${modKey}
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper for spawning simple commands */
#define CMD(cmd) { .v = (const char*[]){ cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "${terminal}", NULL };
static const char *lockcmd[]  = { "${lock}", NULL };
static const char *browsercmd[] = { "${browser}", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	/* Modified from shift+c */
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	/* Repurposed to match i3's mod+space to spawn terminal, moved
	   regular space to shift+space to allow layout toggles */
	/*{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },*/
	{ MODKEY|ShiftMask,             XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_space,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	/* Media keys, make more configurable in the future - uses funcs installed
	   by nix in ~/.evertras/funcs, makes assumptions, but just want to get
	   this up and running before making it fancy. */
	{ 0,                       XF86XK_AudioLowerVolume,   spawn, CMD("volume-down") },
	{ 0,                       XF86XK_AudioRaiseVolume,   spawn, CMD("volume-up") },
	{ 0,                       XF86XK_AudioMute,          spawn, CMD("volume-mute-toggle") },
	{ 0,                       XF86XK_MonBrightnessUp,    spawn, SHCMD("brightness-change 10%+") },
	{ 0,                       XF86XK_MonBrightnessDown,  spawn, SHCMD("brightness-change 10%-") },
	/* Some other ideas for the future */
	/*
	{ 0,                       XF86XK_AudioPlay,          spawn, {.v = SHCMD("playerctl play-pause") } },
	{ 0,                       XF86XK_AudioNext,          spawn, {.v = SHCMD("playerctl next") } },
	{ 0,                       XF86XK_AudioPrev,          spawn, {.v = SHCMD("playerctl previous") } },
	{ 0,                       XF86XK_AudioStop,          spawn, {.v = SHCMD("playerctl stop") } },
	{ 0,                       XF86XK_AudioRewind,        spawn, {.v = SHCMD("playerctl position 10-") } },
	{ 0,                       XF86XK_AudioForward,       spawn, {.v = SHCMD("playerctl position 10+") } },
	{ 0,                       XF86XK_AudioMicMute,       spawn, {.v = SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") } },
	{ 0,                       XF86XK_Display,            spawn, {.v = SHCMD("autorandr --change") } },
	{ 0,                       XF86XK_TouchpadToggle,     spawn, {.v = SHCMD("touchpad-toggle") } },
	{ 0,                       XF86XK_TouchpadOff,        spawn, {.v = SHCMD("touchpad-toggle off") } },
	{ 0,                       XF86XK_TouchpadOn,         spawn, {.v = SHCMD("touchpad-toggle on") } },
	*/

	/* Additional keybinds added here */
	{ MODKEY,                       XK_Escape, spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_r,      spawn,          {.v = browsercmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

