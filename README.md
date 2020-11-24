This old obsolete dwm project is very much out of date and has been archived.

For dwm refer to [dwm-flexipatch](https://github.com/bakkeby/dwm-flexipatch) and for individual patches refer to [this](https://github.com/bakkeby/patches/wiki) page instead.

---

My dwm 6.2 build with custom patches. Refer to [https://dwm.suckless.org/](https://dwm.suckless.org/) for details on the dwm window manager, how to install it and how it works.

---

How does this build / fork of dwm stand out compared to so many others? Due to the nature of dwm and arbitrary restrictions such as not exceeding a certain number of lines of code, individual patches can be applied in order to tailor it to your needs. The side effect of this is that most patches, and in turn most builds, are essentially hardcoded to offer specific features.

In this build the vast majority of patches that have been applied can be configured on or off. If you have ever been curious about trying out dwm, but have been discouraged by manual patching, then this may be a good starting point to see what a "fully fledged" dwm can look like. Want to try out the `pertag` patch? Just flip a config and recompile. Once you have found out what works for you and what don't then you should be in a better position to choose patches should you want to start patching from scratch.

---

Patches included:

   - [alpha](https://github.com/bakkeby/patches/tree/master/dwm/dwm-alpha-6.2.diff)
      - adds transparency for the status bar
      - refer to [https://dwm.suckless.org/patches/alpha/](https://dwm.suckless.org/patches/alpha/)

   - [attachx](https://github.com/bakkeby/patches/tree/master/dwm/dwm-attachx-6.2.diff)
      - by default any new (non-floating) window you open in dwm becomes the new master and there are several patches that alter this behaviour
      - this patch combines several of these attach modes into one having a configuration option to switch between them
      - this avoids having to hardcode one specific behaviour which means that someone else can take your build and just flip a config setting to change where new windows are placed in the stack
      - one could easily add an option to alter between these modes via keyboard shortcuts, but one generally stick to one mode so I have left this out
      - this patch includes the following attach modes:
         - 0 - master (default behaviour): new windows become the new master
         - 1 - [attachabove](https://dwm.suckless.org/patches/attachabove/): new window is placed above selected client
         - 2 - [attachaside](https://dwm.suckless.org/patches/attachaside/): new window is placed on top of the stack
         - 3 - [attachbelow](https://dwm.suckless.org/patches/attachbelow/): new window is placed below selected client
         - 4 - [attachbottom](https://dwm.suckless.org/patches/attachbottom/): new window is placed at the bottom of the stack

   - [autostart](https://github.com/bakkeby/patches/tree/master/dwm/dwm-autostart-6.2.diff)
      - adds automatic running of `autostart.sh` and `autostart_blocking.sh` on startup
      - be aware that dwm will not start up as long as `autostart_blocking.sh` is running and will stay completely unresponsive until it has finished
      - this version reads the scripts from `~/.config/dwm/` rather than `~/.dwm/` as in the original patch
      - this patch is strictly not needed as you can include everything in `.xprofile`, `.xinitrc` or `.xsessionrc` depending on your system
      - I added this to avoid certain conflicts when switching between window managers
      - refer to [https://dwm.suckless.org/patches/autostart/](https://dwm.suckless.org/patches/autostart/)

   - [center](https://github.com/bakkeby/patches/tree/master/dwm/dwm-center-6.2.diff)
      - adds an _iscentered_ rule to automatically center clients on the current monitor.
      - I added a fix for multi-monitor support (original fix did not take monitor x and y positions into account)
      - I also added automatic centering of floating popups
      - refer to [https://dwm.suckless.org/patches/center/](https://dwm.suckless.org/patches/center/)

   - [cfacts](https://github.com/bakkeby/patches/tree/master/dwm/dwm-cfacts-6.2.diff)
      - provides the ability to assign different weights to clients in their respective stack in tiled layout
      - in my vanitygaps build I also added cfacts support to bottomstack, centeredmaster and deck layouts that can be found in [dwm-vanitygaps-cfacts-6.2.diff](https://github.com/bakkeby/patches/tree/master/dwm/dwm-vanitygaps-cfacts-6.2.diff)
      - refer to [https://dwm.suckless.org/patches/cfacts/](https://dwm.suckless.org/patches/cfacts/)

   - [cyclelayouts](https://github.com/bakkeby/patches/tree/master/dwm/dwm-cyclelayouts-6.2.diff)
      - lets you cycle through all your layouts
      - useful for demo purposes
      - refer to [https://dwm.suckless.org/patches/cyclelayouts/](https://dwm.suckless.org/patches/cyclelayouts/)

   - [fancybar](https://github.com/bakkeby/patches/tree/master/dwm/dwm-fancybar-6.2.diff)
      - shows the titles of all visible windows in the status bar (as opposed to showing just the selected one)
      - refer to [https://dwm.suckless.org/patches/fancybar/](https://dwm.suckless.org/patches/fancybar/)

   - [flextile](https://github.com/bakkeby/patches/tree/master/dwm/dwm-flextile-6.2.diff)
      - flextile is a flexible version of the original tile layout that supports horizontal and vertical split as well as three different stack modes: horizontal, vertical, deck
      - the original patch (referred to above) has been ported to 6.2 as-is and incorporates the pertag, nmaster and bottom stack layouts
      - the 5.8.2 patch has a custom solution for pertag and overrides the Monitor struct, which makes it incompatible with the (now) better implemented pertag patch
      - for this reason I ended up changing flextile quite significantly:
         - removing the incorporated / custom pertag patch (either you have it or you don't)
         - dwm now comes with nmaster so no need for the patch to provide that
         - added vanitygaps
         - added cfacts
         - added gappless grid mode as a stack option
         - incorporated the centered master layout (both horizontal and vertical split)
         - added option to set predefined layouts via keyboard shortcuts
      - the end result is that this one layout can replace the following layouts:
         - tile
         - deck
         - monocle
         - centeredmaster
         - bstack
         - bstackhoriz
         - gapplessgrid
         - columns (col)
      - I added this expanded version of flextile as [dwm-flextile-pertag-cfacts-vanitygaps-grid-centered-6.2.diff](patches/dwm-flextile-pertag-cfacts-vanitygaps-grid-centered-6.2.diff)
      - refer to [https://dwm.suckless.org/patches/flextile/](https://dwm.suckless.org/patches/flextile/)

   - [focusonnetactive](https://github.com/bakkeby/patches/tree/master/dwm/dwm-focusonnetactive-6.2.diff)
      - rather than highlighting a tab as urgent, activate the tab and the urgent window instead
      - I added an ever so small fix for multi-monitor support
      - refer to [https://dwm.suckless.org/patches/focusonnetactive/](https://dwm.suckless.org/patches/focusonnetactive/)

   - [losefullscreen](https://github.com/bakkeby/patches/tree/master/dwm/dwm-losefullscreen-6.2.diff)
      - by default in dwm it is possible to make an application fullscreen, then use the focusstack keybindings to focus on other windows beneath the current window
      - it is also possible to spawn new windows (e.g. a terminal) that end up getting focus while the previous window remains in fullscreen
      - this patch ensures that in such scenarios the previous window loses fullscreen

   - [pertag](https://github.com/bakkeby/patches/tree/master/dwm/dwm-pertag-6.2.diff)
      - the pertag patch keeps layout, mwfact, barpos and nmaster settings per tag, rather than the default behaviour of having everything global across all tags
      - in my build I ended up making using pertag configurable (should one want to disable this functionality)
      - I also ended up integrating the `pertag_without_bar` patch that avoids storing bar configuration on a per tag basis (i.e. whether bar is enabled or not) making it configurable as well
      - should anyone be interested in the configurable version of this patch then I have uploaded it as [dwm-pertag-configurable-6.2.diff](patches/dwm-pertag-configurable-6.2.diff)
      - refer to [https://dwm.suckless.org/patches/pertag/](https://dwm.suckless.org/patches/pertag/)

   - [resizecorners](https://github.com/bakkeby/patches/tree/master/dwm/dwm-resizecorners-6.2.diff)
      - by default, windows only resize from the bottom right corner
      - with this patch the mouse is warped to the nearest corner and you resize from there
      - refer to [https://dwm.suckless.org/patches/resizecorners/](https://dwm.suckless.org/patches/resizecorners/)

   - [rotatestack](https://github.com/bakkeby/patches/tree/master/dwm/dwm-rotatestack-6.2.diff)
      - let's you rotate through the stack
      - this is especially convenient when mapped to the mouse scrollwheel
      - refer to [https://dwm.suckless.org/patches/rotatestack/](https://dwm.suckless.org/patches/rotatestack/)

   - [savefloats](https://github.com/bakkeby/patches/tree/master/dwm/dwm-savefloats-6.2.diff)
      - saves size and position of every floating window before it is forced into tiled mode
      - if the window is made floating again then the old dimensions will be restored
      - the original savefloats patch worked great when toggling floating <--> tiled in a tiled layout, but the floating window dimensions were not carried over when switching to a floating layout
      - so I ended up adding a patch for this patch to also restore floating window dimensions when going into floating layout mode and I uploaded this as [dwm-savefloats-configurable-6.2.diff](patches/dwm-savefloats-configurable-6.2.diff)
      - refer to [https://dwm.suckless.org/patches/save_floats/](https://dwm.suckless.org/patches/save_floats/)

   - [statuspadding](https://github.com/bakkeby/patches/tree/master/dwm/dwm-statuspadding-6.2.diff)
      - adds configuration options for horizontal and vertical padding in the status bar
      - refer to [https://dwm.suckless.org/patches/statuspadding/](https://dwm.suckless.org/patches/statuspadding/)

   - [switchtag](https://github.com/bakkeby/patches/tree/master/dwm/dwm-switchtag-6.2.diff)
      - dwm allow you to set application specific rules so that you can have your browser, for example, start up on tag 9 optionally on a given monitor
      - when you open your browser it is then automatically moved to the configured tag, but you have to manually enable the tag to see the newly opened application
      - this patch adds an extra configuration option for individual rules where
         - 0 is default behaviour
         - 1 automatically moves you to the tag of the newly opened application and
         - 2 enables the tag of the newly opened application in addition to your existing enabled tags
      - opening the application a second time, however, will not result in the tag changing automatically as application rules do not apply in this situation (they are only highlighted as urgent)
      - for this reason I would recommend combining this with the `focusonnetactive` patch

   - [systray](https://github.com/bakkeby/patches/tree/master/dwm/dwm-systray-6.2.diff)
      - a systray in dwm is something that I thought I did not need, but eventually for work related reasons I did and standalone systray applications did not work that well for me
      - getting a system tray up and running in dwm is fairly straightforward based on the patches available on the reference site, but once you add the [alpha](https://github.com/bakkeby/patches/tree/master/dwm/dwm-alpha-6.2.diff) patch into the mix it is a different story entirely
      - managing to get the system tray to play well with a transparent menu bar has arguably been the most challenging patch that I have applied so far
      - for reference I have prepared two patches
         - a combined alpha and systray patch on top of 6.2: [dwm-alpha-and-systray-6.2.diff](https://github.com/bakkeby/patches/tree/master/dwm/dwm-alpha-and-systray-6.2.diff)
         - a standalone patch applying my custom systray on top of an already applied 6.2 alpha patch: [dwm-systray-on-top-of-alpha-6.2.diff](https://github.com/bakkeby/patches/tree/master/dwm/dwm-systray-on-top-of-alpha-6.2.diff)
      - refer to [https://dwm.suckless.org/patches/systray/](https://dwm.suckless.org/patches/systray/)

   - [tagallmon](https://github.com/bakkeby/patches/tree/master/dwm/dwm-tagallmon-6.2.diff)
      - dwm comes with a function `tagmon` that allows you to send a window to an adjacent monitor
      - this patch replicates the same behaviour, but for all visible windows on the current monitor
      - let's you _swoosh_ everything to the side before opening something else while reassuring onlookers that yes you do live up to wearing those sunglasses while working

   - [tagmonfixfs](https://github.com/bakkeby/patches/tree/master/dwm/dwm-tagmonfixfs-6.2.diff)
      - if you try to send a fullscreen window to an adjacent monitor using `tagmon` then the window is moved behind the scenes, but it remains in fullscreen on the original monitor until you exit fullscreen view (at which point it will appear on the adjacent monitor)
      - this patch allows a fullscreen window to be moved to an adjacent monitor while remaining in fullscreen

   - [tagswapmon](https://github.com/bakkeby/patches/tree/master/dwm/dwm-tagswapmon-6.2.diff)
      - similarly to `tagallmon` this patch allows you to swap all visible windows on the current monitor with all visible windows on an adjacent monitor
      - this may be useful if you have a large main (master) display and one or more smaller secondary (stack) displays

   - [togglefullscreen](https://github.com/bakkeby/patches/tree/master/dwm/dwm-togglefullscreen-6.2.diff)
      - a simple patch that lets you toggle fullscreen on and off using a single keybinding
      - this is more practical than you might think as many programs have inconsistent shortcuts for going into fullscreen

   - [vanitygaps](https://github.com/bakkeby/patches/blob/master/dwm/dwm-vanitygaps-6.2.diff)
      - there are a few patches out there that add fixed gaps between windows, with or without gaps betwen the screen frame and clients
      - vanity gaps do this as well, with the difference that the size of inner and outer gaps can be configured separately (as well as the size of horizontal and vertical gaps, respectively)
      - additionally the size of the gaps can be altered on the fly using keyboard shortcuts (in practice though this is mostly for demo-purposes)
      - besides vanity reasons (read: unixporn), gaps can also remove a bit of the claustrophobic feel that tiling window managers often impose on new users
      - includes smartgaps support: whether or not to include outer gaps when a single window is open
      - includes the option of toggling gaps on and off
      - includes a series of vanitygap-enabled layouts (nobody should need this many though, the idea being that you remove the ones you don't need):
         - the default tile layout
         - [bottomstack](https://dwm.suckless.org/patches/bottomstack/)
         - [centeredmaster](https://dwm.suckless.org/patches/centeredmaster/)
         - [deck](https://dwm.suckless.org/patches/deck/)
         - [fibonacci](https://dwm.suckless.org/patches/fibonacci/)
         - [gridmode](https://dwm.suckless.org/patches/gridmode/)
         - [gapplessgrid](https://dwm.suckless.org/patches/gaplessgrid/)
         - [horizgrid](https://dwm.suckless.org/patches/horizgrid/)
         - [nrowgrid](https://dwm.suckless.org/patches/nrowgrid/)
         - [flextile](https://dwm.suckless.org/patches/flextile/) (refer to the flextile patch for this)
      - [cfacts](https://dwm.suckless.org/patches/cfacts/) support for tile, bottomstack, centeredmaster and deck layouts can be found in [dwm-cfacts-vanitygaps-6.2.diff](https://github.com/bakkeby/patches/blob/master/dwm/dwm-cfacts-vanitygaps-6.2.diff)

   - [windowrolerule](https://github.com/bakkeby/patches/tree/master/dwm/dwm-windowrolerule-6.2.diff)
      - sometimes a single application opens different windows depending on the task at hand and this is often reflected in the `WM_WINDOW_ROLE(STRING)` x property
      - this patch adds the role field to the rule configuration so that one can differentiate between, say, Firefox "browser" vs "Preferences" vs "Manager" or Google-chrome "browser" vs "pop-up"

   - [zoomswap](https://github.com/bakkeby/patches/tree/master/dwm/dwm-zoomswap-6.2.diff)
      - the default behaviour in dwm when using zoom (i.e. moving a window to become the new master) is to use _pop_ to re-attach the window on top of the chain
      - this has the side effect of moving every window down as well, resulting in every window on the screen changing position
      - the zoomswap patch changes this behaviour so that the current master swaps position with the other window that is to become the new master
      - applying zoom on the current master will result in it swapping back to the previous master (if it exists)
      - in my build I ended up making using zoomtag configurable (should one want to disable this functionality)
      - I also ended up integrating it with the `pertag` patch so that the previous master can be remembered on a per-tag basis
      - should anyone be interested in the pertag-compatible version of this patch then I have uploaded it as [dwm-zoomswap-pertag-configurable-6.2.diff](patches/dwm-zoomswap-pertag-configurable-6.2.diff)
      - refer to [https://dwm.suckless.org/patches/zoomswap/](https://dwm.suckless.org/patches/zoomswap/)

---

I have included my personal keybindings (`config.h`) here for reference; feel free to delete this to start fresh with default configuration (from `config.def.h`). Most notable I guess is binding `rotatestack` and `cyclelayouts` to the mouse wheel which can be rather convenient when having many windows in the stack. I also mapped middle-click to move a window to master (`zoom`) rather than toggling floating mode which you enable anyway when dragging a window.

I should also note that to start various applications I have additional keybindings through `sxhkd`.

---

Example screenshot:

![https://github.com/bakkeby/dwm-vanitygaps/wiki/bstack.jpg](https://github.com/bakkeby/dwm-vanitygaps/wiki/bstack.jpg)

Example screenshot with exaggerated gaps:

![https://github.com/bakkeby/dwm-vanitygaps/wiki/vanitygaps.jpg](https://github.com/bakkeby/dwm-vanitygaps/wiki/vanitygaps.jpg)

Flextile layout with grid stack:

![https://github.com/bakkeby/dwm-vanitygaps/wiki/gridstack.jpg](https://github.com/bakkeby/dwm-vanitygaps/wiki/gridstack.jpg)
