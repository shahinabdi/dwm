/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int barpad             = 8;        /* bar height = font height + barpad (was 2) */
static const char *fonts[]          = {
	"JetBrains Mono:size=11",                 /* text (has no icons, so icons fall through) */
	"JetBrainsMono Nerd Font Propo:size=14",  /* icons: own, bigger size */
};
static const char dmenufont[]       = "JetBrainsMono Nerd Font Mono:size=11";
static const char col_navy1[]       = "#16161e";
static const char col_navy2[]       = "#1a1b26";
static const char col_steel[]       = "#7aa2f7";
static const char col_text[]        = "#c0caf5";
static const char col_muted[]       = "#565f89";
static const char col_amber[]        = "#e0af68";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_muted, col_navy1, col_navy2 },
	[SchemeSel]  = { col_text,  col_navy2, col_steel },
};

/* tagging: workspace icons + app rules live in their own files */
#include "tags.h"
#include "rules.h"

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#include "layouts.c"   /* bstack, centeredmaster, grid, deck, spiral, dwindle (suckless patches) */

/* index matters: dwm-layout / the sidebar switch by index (keep in sync with scripts/dwm-layout) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },           /* 0 tile: master left, stack right (default) */
	{ "TTT",      bstack },         /* 1 bottom stack: master top, stack below */
	{ "|M|",      centeredmaster }, /* 2 centered master */
	{ "###",      grid },           /* 3 grid */
	{ "[D]",      deck },           /* 4 deck: stack shows one window at a time */
	{ "[@]",      spiral },         /* 5 fibonacci spiral */
	{ "[\\]",     dwindle },        /* 6 fibonacci dwindle */
	{ "[M]",      monocle },        /* 7 monocle: one window fullscreen */
	{ "><>",      NULL },           /* 8 floating */
	{ NULL,       NULL },           /* end marker for cyclelayout */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_navy1, "-nf", col_muted, "-sb", col_steel, "-sf", col_text, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *rofi[] = { "rofi", "-show", "drun", "-show-icons", NULL };

/* statuscmd: clicking a status block runs `statusclickcmd <block-id>` with $BUTTON set */
static const char statusclickcmd[] = "dwm-statusclick";

/* media keys (scripts live in ~/.local/bin, see dwm-btw/scripts) */
static const char *volup[]    = { "dwm-ctl", "vol-up",    NULL };
static const char *voldown[]  = { "dwm-ctl", "vol-down",  NULL };
static const char *volmute[]  = { "dwm-ctl", "vol-mute",  NULL };
static const char *micmute[]  = { "dwm-ctl", "mic-mute",  NULL };
static const char *brightup[] = { "dwm-ctl", "bright-up", NULL };
static const char *brightdn[] = { "dwm-ctl", "bright-down", NULL };
static const char *netmenu[]  = { "dwm-ctl", "net-menu",  NULL };
static const char *kbreset[]  = { "dwm-ctl", "kb-reset",  NULL };
static const char *helpcmd[]  = { "dwm-help", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = rofi } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_h,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_z,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_Right,  cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,   cyclelayout,    {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("dwm-layout menu") },
	{ MODKEY,                       XK_space,  spawn,          SHCMD("dwm-ctl kb-next") },
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("dwm-power suspend") },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("dwm-power lock") },
	{ MODKEY,                       XK_Escape, spawn,          SHCMD("dwm-power menu") },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("dwm-display menu") },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_semicolon, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_semicolon, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_a,      spawn,          SHCMD("qs -c arch-sidebar ipc call sidebar toggle") },
	{ MODKEY,                       XK_s,      view,           {.ui = ~0 } },
	{ MODKEY|ControlMask,           XK_s,      tag,            {.ui = ~0 } },
	/* clipboard history (dwm-clip) */
	{ MODKEY,                       XK_v,      spawn,          SHCMD("dwm-clip") },
	{ MODKEY|ShiftMask,             XK_v,      spawn,          SHCMD("dwm-clip clear") },
	/* screenshots (dwm-shot) */
	{ 0,                            XK_Print,  spawn,          SHCMD("dwm-shot full") },
	{ ShiftMask,                    XK_Print,  spawn,          SHCMD("dwm-shot window") },
	{ ControlMask,                  XK_Print,  spawn,          SHCMD("dwm-shot annotate") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("dwm-shot menu") },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("dwm-shot region") },
	/* colour picker */
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("dwm-color") },
	TAGKEYS(                        XK_ampersand,              0)
	TAGKEYS(                        XK_eacute,                 1)
	TAGKEYS(                        XK_quotedbl,               2)
	TAGKEYS(                        XK_apostrophe,             3)
	TAGKEYS(                        XK_parenleft,              4)
	TAGKEYS(                        XK_minus,                  5)
	TAGKEYS(                        XK_egrave,                 6)
	TAGKEYS(                        XK_underscore,             7)
	TAGKEYS(                        XK_ccedilla,               8)
	TAGKEYS(                        XK_agrave,                 9)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      restartwm,      {0} },
	{ 0,              XF86XK_AudioRaiseVolume,  spawn,         {.v = volup } },
	{ 0,              XF86XK_AudioLowerVolume,  spawn,         {.v = voldown } },
	{ 0,              XF86XK_AudioMute,         spawn,         {.v = volmute } },
	{ 0,              XF86XK_AudioMicMute,      spawn,         {.v = micmute } },
	{ 0,              XF86XK_MonBrightnessUp,   spawn,         {.v = brightup } },
	{ 0,              XF86XK_MonBrightnessDown, spawn,         {.v = brightdn } },
	{ MODKEY,                       XK_n,      spawn,          {.v = netmenu } },
	{ MODKEY|ShiftMask,             XK_k,      spawn,          {.v = kbreset } },
	{ MODKEY|ShiftMask,             XK_h,      spawn,          {.v = helpcmd } },
	{ MODKEY,                       XK_F1,     spawn,          {.v = helpcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[7]} },
	{ ClkLtSymbol,          0,              Button4,        cyclelayout,    {.i = -1 } },
	{ ClkLtSymbol,          0,              Button5,        cyclelayout,    {.i = +1 } },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        statusclick,    {0} },
	{ ClkStatusText,        0,              Button2,        statusclick,    {0} },
	{ ClkStatusText,        0,              Button3,        statusclick,    {0} },
	{ ClkStatusText,        0,              Button4,        statusclick,    {0} },
	{ ClkStatusText,        0,              Button5,        statusclick,    {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
