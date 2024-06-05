/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 3; /* border pixel of windows */

static const unsigned int gappih = 10; /* horiz inner gap between windows */
static const unsigned int gappiv = 10; /* vert inner gap between windows */
static const unsigned int gappoh =
    10; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    10; /* vert outer gap between windows and screen edge */
static const unsigned int systraypinning =
    1; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayonleft =
    1; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 3; /* systray spacing */
static const int          systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const int          showsystray = 1;  /* 0 means no systray */
static const unsigned int snap        = 32; /* snap pixel */
static const int          showbar     = 1;  /* 0 means no bar */
static const int          topbar      = 1;  /* 0 means bottom bar */
static int                smartgaps   = 0;
static const int          usealtbar   = 0; /* 1 means use non-dwm status bar */
static const int          extrabar    = 0;
static const int          statussep   = ';';
static const char* altbarclass = "Polybar"; /* Alternate bar class name */
static const char* altbarcmd =
    "$HOME/.config/polybar/start.sh"; /* Alternate bar launch command */
static const char* fonts[]     = {"monospace:size=10"};
static const char  dmenufont[] = "monospace:size=10";
static const char  col_gray1[] = "#1e2030";
static const char  col_gray2[] = "#363a4f";
static const char  col_gray3[] = "#a5adcb";
static const char  col_gray4[] = "#cad3f5";
static const char  col_cyan[]  = "#7dc4e4";
static const char* colors[][3] = {
  /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel]  = {col_gray2, col_cyan,  col_cyan },
};

/* tagging */
static const char* tags[] = {"I", "II", "III", "IV", "V"};

static const Rule rules[] = {
  /* xprop(1):
  *	WM_CLASS(STRING) = instance, class
  *	WM_NAME(STRING) = title
  */
  /* class      instance    title       tags mask     isfloating   monitor */
    {"Gimp",    NULL, NULL, 0,      1, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.525; /* factor of master area size [0.05..0.95] */
static const int   nmaster = 1;   /* number of clients in master area */
static const int   resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT                                                           \
    1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
  /* symbol     arrange function */
    {"[]=",  tile                  }, /* first entry is default */
    {"[M]",  monocle               },
    {"[@]",  spiral                },
    {"[\\]", dwindle               },
    {"H[]",  deck                  },
    {"TTT",  bstack                },
    {"===",  bstackhoriz           },
    {"HHH",  grid                  },
    {"###",  nrowgrid              },
    {"---",  horizgrid             },
    {":::",  gaplessgrid           },
    {"|M|",  centeredmaster        },
    {">M>",  centeredfloatingmaster},
    {"><>",  NULL                  }, /* no layout function means floating behavior */
    {NULL,   NULL                  },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
    {MODKEY, KEY, view, {.ui = 1 << TAG}},                                     \
        {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},             \
        {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                      \
        {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
    {                                                                          \
        .v = (const char*[]) { "/bin/sh", "-c", cmd, NULL }                    \
    }

/* commands */

/* commands */
static const char* dmenucmd[] = {"dmenu_drun", NULL};
static const char* roficmd[]  = {"rofi", "-show", "drun", NULL};
static const char* emojicmd[] = {"rofi", "-show", "emoji", NULL};
static const char* termcmd[]  = {"qterminal", NULL};
static const char* fm_cmd[]   = {"thunar", NULL};
// static const char *browser_cmd[] = {"chromium", NULL};
// static const char *browser_cmd[] = {"chrome", NULL};
static const char* browser_cmd[] = {"chromium", NULL};

static const char* volup[]   = {"pamixer", "--increase", "5", NULL};
static const char* voldown[] = {"pamixer", "--decrease", "5", NULL};
static const char* mute[]    = {"pamixer", "--toggle-mute", NULL};
static const char* acpi[]    = {"/home/ezntek/.local/bin/sendacpi", NULL};
static const char* cfon[]  = {"/home/ezntek/.local/bin/cloudflare", "on", NULL};
static const char* cfoff[] = {"/home/ezntek/.local/bin/cloudflare", "off",
                              NULL};
// #define MIXERCMD "/home/ezntek/.local/bin/dumbmixer"
// static const char *volup[] = {MIXERCMD, "raisevol", NULL};
// static const char *voldown[] = {MIXERCMD, "lowervol", NULL};
// static const char *mute[] = {MIXERCMD, "togglemute", NULL};

static const char* screenshotcmd[]    = {"/home/ezntek/.local/bin/screenshot",
                                         NULL};
static const char* fs_screenshotcmd[] = {"/home/ezntek/.local/bin/screenshot",
                                         "fs", NULL};

static const char* brightup[]   = {"brightnessctl", "s", "5%+", NULL};
static const char* brightdown[] = {"brightnessctl", "s", "5%-", NULL};

static const Key keys[] = {
  /* modifier                     key        function        argument */
    {MODKEY,                          XK_space,                 spawn,          {.v = roficmd}         },
    {MODKEY | Mod1Mask,               XK_space,                 spawn,          {.v = dmenucmd}        },
    {MODKEY | ControlMask,            XK_space,                 spawn,          {.v = emojicmd}        },
    {MODKEY,                          XK_Return,                spawn,          {.v = termcmd}         },
    {MODKEY | ShiftMask,              XK_Return,                spawn,          {.v = browser_cmd}     },
    {MODKEY | ControlMask,            XK_Return,                spawn,          {.v = fm_cmd}          },
    {MODKEY,                          XK_a,                     spawn,          {.v = acpi}            },
    {MODKEY,                          XK_c,                     spawn,          {.v = cfon}            },
    {MODKEY | ShiftMask,              XK_c,                     spawn,          {.v = cfoff}           },
    {MODKEY | ShiftMask,              XK_e,                     pushdown,       {0}                    },
    {MODKEY | ShiftMask,              XK_u,                     pushup,         {0}                    },
 // { MODKEY,                    XK_b,      togglebar,      {0} },
    {MODKEY,                          XK_i,                     focusstackvis,  {.i = +1}              },
    {MODKEY,                          XK_n,                     focusstackvis,  {.i = -1}              },
    {MODKEY | ShiftMask,              XK_i,                     focusstackhid,  {.i = +1}              },
    {MODKEY | ShiftMask,              XK_n,                     focusstackhid,  {.i = -1}              },
    {MODKEY,                          XK_y,                     incnmaster,     {.i = +1}              },
    {MODKEY,                          XK_o,                     incnmaster,     {.i = -1}              },
    {MODKEY | ControlMask,            XK_n,                     setmfact,       {.f = -0.025}          },
    {MODKEY | ControlMask,            XK_i,                     setmfact,       {.f = +0.025}          },
    {MODKEY | ControlMask,            XK_u,                     setcfact,       {.f = -0.3}            },
    {MODKEY | ControlMask,            XK_e,                     setcfact,       {.f = +0.3}            },
    {MODKEY,                          XK_t,                     zoom,           {0}                    },
    {MODKEY,                          XK_f,                     fullscreen,     {0}                    },
    {MODKEY | ShiftMask,              XK_r,                     reloadconfig,   {0}                    },
    {MODKEY | ShiftMask,              XK_s,                     spawn,          {.v = screenshotcmd}   },
    {MODKEY | ControlMask,            XK_s,                     spawn,          {.v = fs_screenshotcmd}},
 // some gap shit (all hail vanitygaps?)
    {MODKEY,                          XK_equal,                 incrgaps,       {.i = +1}              },
    {MODKEY,                          XK_minus,                 incrgaps,       {.i = -1}              },
    {MODKEY | Mod1Mask,               XK_i,                     incrigaps,      {.i = +1}              },
    {MODKEY | Mod1Mask | ShiftMask,   XK_i,                     incrigaps,      {.i = -1}              },
    {MODKEY | Mod1Mask,               XK_o,                     incrogaps,      {.i = +1}              },
    {MODKEY | Mod1Mask | ShiftMask,   XK_o,                     incrogaps,      {.i = -1}              },
    {MODKEY | Mod1Mask,               XK_6,                     incrihgaps,     {.i = +1}              },
    {MODKEY | Mod1Mask | ShiftMask,   XK_6,                     incrihgaps,     {.i = -1}              },
    {MODKEY | Mod1Mask,               XK_7,                     incrivgaps,     {.i = +1}              },
    {MODKEY | Mod1Mask | ShiftMask,   XK_7,                     incrivgaps,     {.i = -1}              },
    {MODKEY | Mod1Mask,               XK_8,                     incrohgaps,     {.i = +1}              },
    {MODKEY | Mod1Mask | ControlMask, XK_8,                     incrohgaps,     {.i = -1}              },
    {MODKEY | Mod1Mask,               XK_9,                     incrovgaps,     {.i = +1}              },
    {MODKEY | Mod1Mask | ShiftMask,   XK_9,                     incrovgaps,     {.i = -1}              },
 // { MODKEY,                    XK_Tab,    view,           {0} },
    {MODKEY,                          XK_w,                     killclient,     {0}                    },
 /*{ MODKEY,                     XK_t,      setlayout,      {.v =
  &layouts[0]} }, { MODKEY,                       XK_f,      setlayout, {.v =
  &layouts[1]} }, { MODKEY,                       XK_m,      setlayout, {.v =
  &layouts[2]} },*/
  // { MODKEY,                    XK_space,  setlayout,      {0} },
    {MODKEY | ShiftMask,              XK_f,                     togglefloating, {0}                    },
 // { MODKEY,                    XK_0,      view,           {.ui = ~0 } },
  // { MODKEY|ShiftMask,          XK_0,      tag,            {.ui = ~0 } },
    {ControlMask | ShiftMask,         XK_n,                     focusmon,       {.i = -1}              },
    {ControlMask | ShiftMask,         XK_i,                     focusmon,       {.i = +1}              },
    {ControlMask | Mod1Mask,          XK_n,                     tagmon,         {.i = -1}              },
    {ControlMask | Mod1Mask,          XK_i,                     tagmon,         {.i = +1}              },
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4)
  // TAGKEYS(                        XK_6,                      5)
  // TAGKEYS(                        XK_7,                      6)
  // TAGKEYS(                        XK_8,                      7)
  // TAGKEYS(                        XK_9,                      8)
  //{MODKEY|ShiftMask, XK_q, quit, {0}},
    {0,                               XF86XK_AudioLowerVolume,  spawn,          {.v = voldown}         },
    {0,                               XF86XK_AudioRaiseVolume,  spawn,          {.v = volup}           },
    {0,                               XF86XK_AudioMute,         spawn,          {.v = mute}            },
    {0,                               XF86XK_MonBrightnessUp,   spawn,          {.v = brightup}        },
    {0,                               XF86XK_MonBrightnessDown, spawn,          {.v = brightdown}      },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function argument */
    {ClkLtSymbol,   0,      Button1, setlayout,      {0}               },
    {ClkLtSymbol,   0,      Button3, setlayout,      {.v = &layouts[2]}},
    {ClkWinTitle,   0,      Button2, zoom,           {0}               },
    {ClkStatusText, 0,      Button2, spawn,          {.v = termcmd}    },
    {ClkClientWin,  MODKEY, Button1, movemouse,      {0}               },
    {ClkClientWin,  MODKEY, Button2, togglefloating, {0}               },
    {ClkClientWin,  MODKEY, Button3, resizemouse,    {0}               },
    {ClkTagBar,     0,      Button1, view,           {0}               },
    {ClkTagBar,     0,      Button3, toggleview,     {0}               },
    {ClkTagBar,     MODKEY, Button1, tag,            {0}               },
    {ClkTagBar,     MODKEY, Button3, toggletag,      {0}               },
};
