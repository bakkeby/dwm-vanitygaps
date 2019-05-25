/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "UbuntuMono Nerd Font Mono:size=12" };
static const char dmenufont[]       = "UbuntuMono Nerd Font Mono:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#330000";  //"#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "⌨₁", "＃₂", "📩₃", "💢₄", "💻₅", "₆", "₇", "🌍₈", "💾₉" };
//static const char *tags[] = { "₁⌨", "₂＃", "₃📩", "₄💢", "₅💻", "₆", "₇", "₈🌍", "₉💾" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title   tags mask  switchtag  isfloating   monitor */
	{ "Gimp",           NULL,       NULL,   1 << 4,    1,         1,           -1 },
	{ "Firefox",        NULL,       NULL,   1 << 7,    1,         0,           -1 },
	{ "Google-chrome",  NULL,       NULL,   1 << 7,    1,         0,            2 },
	{ "Slack",          NULL,       NULL,   1 << 1,    1,         0,            1 },
	{ "Sublime_text",   NULL,       NULL,   1 << 6,    1,         0,           -1 },
	{ "Diffuse",        NULL,       NULL,   1 << 3,    1,         0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[]☰",	tile },    /* first entry is default */
	{ "[M]",	monocle },
	{ "⧉⟧⧠",	NULL },    /* no layout function means floating behavior */
	{ "=M=",	centeredmaster },
	{ "⧉M⧠",	centeredfloatingmaster },
	{ "⚎⚎⚎",	bstack },
	{ "☰☰☰",	bstackhoriz },
	{ "𝍖𝍖𝍖",	grid },
	{ "⟦@⟧",	spiral },
	{ "⟦➘⟧",	dwindle },
	{ "D[]",	deck },
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "gnome-terminal", NULL };

static Key keys[] = {
	/* modifier                     key        function           argument */
	{ MODKEY,                       XK_d,      spawn,             {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,             {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,         {0} },
	{ MODKEY,                       XK_j,      focusstack,        {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,        {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_j,      rotatestack,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_k,      rotatestack,       {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,        {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,        {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,          {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,          {.f = +0.05} },
	{ MODKEY,                       XK_m,      zoom,              {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,          {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,          {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,         {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,         {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,         {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,         {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,        {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,        {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,        {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,        {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,        {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,       {0} },
	{ MODKEY,                       XK_Tab,    view,              {0} },
	{ Mod1Mask,                     XK_Tab,    view,              {0} },
	{ Mod1Mask|ControlMask,         XK_Tab,    view,              {0} },
	{ MODKEY,                       XK_q,      killclient,        {0} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_1,      setlayout,         {.v = &layouts[0]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_2,      setlayout,         {.v = &layouts[1]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_3,      setlayout,         {.v = &layouts[2]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_4,      setlayout,         {.v = &layouts[3]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_5,      setlayout,         {.v = &layouts[4]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_6,      setlayout,         {.v = &layouts[5]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_7,      setlayout,         {.v = &layouts[6]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_8,      setlayout,         {.v = &layouts[7]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_9,      setlayout,         {.v = &layouts[8]} },
	{ MODKEY|Mod1Mask|ControlMask,  XK_0,      setlayout,         {.v = &layouts[9]} },
	{ MODKEY,                       XK_space,  setlayout,         {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating,    {0} },
	{ MODKEY,                       XK_f,      togglefullscreen,  {0} },
	{ MODKEY,                       XK_0,      view,              {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,               {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,          {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,          {.i = +1 } },
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,       {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_comma,  tagmon,            {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_period, tagmon,            {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_comma,  tagallmon,         {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_period, tagallmon,         {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_comma,  tagswapmon,        {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_period, tagswapmon,        {.i = -1 } },
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
	TAGKEYS(                        XK_F1,                        0)
	TAGKEYS(                        XK_F2,                        1)
	TAGKEYS(                        XK_F3,                        2)
	TAGKEYS(                        XK_F4,                        3)
	TAGKEYS(                        XK_F5,                        4)
	TAGKEYS(                        XK_F6,                        5)
	TAGKEYS(                        XK_F7,                        6)
	TAGKEYS(                        XK_F8,                        7)
	TAGKEYS(                        XK_F9,                        8)
	{ MODKEY|ShiftMask,             XK_q,      quit,              {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask         button          function        argument */
	{ ClkLtSymbol,          0,                 Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                 Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                 Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                 Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,            Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY|Mod1Mask,   Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,            Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY,            Button4,        rotatestack,    {.i = +1 } },
	{ ClkClientWin,         MODKEY,            Button5,        rotatestack,    {.i = -1 } },
	{ ClkClientWin,         MODKEY,            Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY|Mod1Mask,   Button4,        cyclelayout,    {.i = -1 } },
	{ ClkClientWin,         MODKEY|Mod1Mask,   Button5,        cyclelayout,    {.i = +1 } },
	{ ClkTagBar,            0,                 Button1,        view,           {0} },
	{ ClkTagBar,            0,                 Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,            Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,            Button3,        toggletag,      {0} },
};
