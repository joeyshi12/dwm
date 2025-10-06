#include <X11/XF86keysym.h>
#include "movestack.c"

/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int underlinepx = 2; /* underline pixel on selected tag */
static const unsigned int snap = 32; /* snap pixel */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1; /* 0 means bottom bar */
static const char *fonts[] = { "JetBrains Mono:style=Bold:size=11", "SauceCodePro Nerd Font Mono:size=20" };
static const char col_ocean[] = "#0b2a47";
static const char col_sky[] = "#3B5A83";
static const char col_teal[] = "#1f3d5c";
static const char col_cloud[] = "#d8e1e8";
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_cloud, col_ocean, col_teal },
	[SchemeSel]  = { col_cloud, col_teal,  col_sky  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "󰟵" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance  title  tags mask  isfloating  monitor */
	{ "Brave-browser",  NULL,     NULL,  0,         0,          -1 },
	{ "krita",          NULL,     NULL,  1 << 2,    0,          -1 },
	{ "Inkscape",       NULL,     NULL,  1 << 2,    0,          -1 },
	{ "obsidian",       NULL,     NULL,  1 << 2,    0,          -1 },
	{ "vesktop",        NULL,     NULL,  1 << 3,    0,          -1 },
	{ "discord-ptb",    NULL,     NULL,  1 << 3,    0,          -1 },
	{ "KeePassXC",      NULL,     NULL,  1 << 4,    0,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *roficmd[] = { "rofi", "-show", "drun", "-theme", "/home/joey/.config/rofi/config.rasi", "-show-icons" };
static const char *termcmd[] = { "kitty", NULL };
static const char *flameshotcmd[] = { "flameshot", "gui", NULL };
static const char *upvolcmd[]   = { "/home/joey/.dotfiles/scripts/volume_notify.sh", "up", NULL };
static const char *downvolcmd[] = { "/home/joey/.dotfiles/scripts/volume_notify.sh", "down", NULL };
static const char *mutevolcmd[] = { "/home/joey/.dotfiles/scripts/volume_notify.sh", "mute", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,                       spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask,             XK_Return,                  spawn,          {.v = termcmd } },
	{ 0,                            XK_Print,                   spawn,          {.v = flameshotcmd } },
    { 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = downvolcmd } },
    { 0,                            XF86XK_AudioMute,           spawn,          {.v = mutevolcmd } },
    { 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = upvolcmd } },
	{ MODKEY,                       XK_b,                       togglebar,      {0} },
	{ MODKEY,                       XK_j,                       focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                       focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                       incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                       setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                       setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_j,                       movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                       movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return,                  zoom,           {0} },
	{ MODKEY,                       XK_Tab,                     view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                       killclient,     {0} },
	{ MODKEY,                       XK_t,                       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                   setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                   togglefloating, {0} },
	{ MODKEY,                       XK_0,                       view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                       tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                   focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                  tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                                       0)
	TAGKEYS(                        XK_2,                                       1)
	TAGKEYS(                        XK_3,                                       2)
	TAGKEYS(                        XK_4,                                       3)
	TAGKEYS(                        XK_5,                                       4)
	{ MODKEY|ShiftMask,             XK_q,                       quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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
