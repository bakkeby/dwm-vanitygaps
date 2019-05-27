My dwm 6.2 build with custom patches. Refer to [https://dwm.suckless.org/](https://dwm.suckless.org/) for details on the dwm window manager, how to install it and how it works.

---

Included patches:

   - [alpha](patches/dwm-alpha-6.2.diff)
      - adds transparency for the status bar
      - refer to [https://dwm.suckless.org/patches/alpha/](https://dwm.suckless.org/patches/alpha/)

   - [attachx](patches/dwm-attachx-6.2.diff)
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

   - [cyclelayouts](patches/dwm-cyclelayouts-6.2.diff)
      - let's you cycle through all your layouts
      - useful for demo purposes
      - refer to [https://dwm.suckless.org/patches/cyclelayouts/](https://dwm.suckless.org/patches/cyclelayouts/)

   - [focusonnetactive](patches/dwm-focusonnetactive-6.2.diff)
      - rather than highlighting a tab as urgent, activate the tab and the urgent window instead
      - I added an ever so small fix for multi-monitor support
      - refer to [https://dwm.suckless.org/patches/focusonnetactive/](https://dwm.suckless.org/patches/focusonnetactive/)

   - [resizecorners](patches/dwm-resizecorners-6.2.diff)
      - by default, windows only resize from the bottom right corner
      - with this patch the mouse is warped to the nearest corner and you resize from there
      - refer to [https://dwm.suckless.org/patches/resizecorners/](https://dwm.suckless.org/patches/resizecorners/)

   - [rotatestack](patches/dwm-rotatestack-6.2.diff)
      - let's you rotate through the stack
      - this is especially convenient when mapped to the mouse scrollwheel
      - refer to [https://dwm.suckless.org/patches/rotatestack/](https://dwm.suckless.org/patches/rotatestack/)

   - [statuspadding](dwm-statuspadding-6.2.diff)
      - adds configuration options for horizontal and vertical padding in the status bar
      - refer to [https://dwm.suckless.org/patches/statuspadding/](https://dwm.suckless.org/patches/statuspadding/)

   - [switchtag](patches/dwm-switchtag-6.2.diff)
      - dwm allow you to set application specific rules so that you can have your browser, for example, start up on tag 9 optionally on a given monitor
      - when you open your browser it is then automatically moved to the configured tag, but you have to manually enable the tag to see the newly opened application
      - this patch adds an extra configuration option for individual rules where
         - 0 is default behaviour
         - 1 automatically moves you to the tag of the newly opened application and
         - 2 enables the tag of the newly opened application in addition to your existing enabled tags
      - opening the application a second time, however, will not result in the tag changing automatically as application rules do not apply in this situation (they are only highlighted as urgent)
      - for this reason I would recommend combining this with the `focusonnetactive` patch

   - [tagallmon](patches/dwm-tagallmon-6.2.diff)
      - dwm comes with a function `tagmon` that allows you to send a window to an adjacent monitor
      - this patch replicates the same behaviour, but for all visible windows on the current monitor
      - let's you _swoosh_ everything to the side before opening something else while reassuring onlookers that yes you do live up to wearing those sunglasses while working

   - [tagswapmon](patches/dwm-tagswapmon-6.2.diff)
      - similarly to `tagallmon` this patch allows you to swap all visible windows on the current monitor with all visible windows on an adjacent monitor
      - this may be useful if you have a large main (master) display and one or more smaller secondary (stack) displays

   - [togglefullscreen](patches/dwm-togglefullscreen-6.2.diff)
      - a simple patch that lets you toggle fullscreen on and off using a single keybinding
      - this is more practical than you might think as many programs have inconsitent shortcuts for going into fullscreen
    
   - [vanitygaps](patches/dwm-vanitygaps-6.2.diff)
      - there are a few patches out there that add fixed gaps between windows, with or without gaps betwen the screen frame and clients
      - vanity gaps do this as well, with the difference that the size of inner and outer gaps can be configured separately (as well as the size of horizontal and vertical gaps, respectively)
      - additionally the size of the gaps can be altered on the fly using keyboard shortcuts (in practice though this is mostly for demo-purposes)
      - besides vanity reasons (read: unixporn), gaps can also remove a bit of the claustrophobic feel that tiling window managers often impose on new users
      - includes smartgaps support: whether or not to include outer borders when a single window is open 
      - includes the option of toggling gaps on and off
      - includes a series of vanitygap-enabled layouts (nobody should need this many though, the idea being that you remove the ones you don't need):
         - the default tile layout
         - [bottomstack](https://dwm.suckless.org/patches/bottomstack/)
         - [centeredmaster](https://dwm.suckless.org/patches/centeredmaster/)
         - [deck](https://dwm.suckless.org/patches/deck/)
         - [fibonacci](https://dwm.suckless.org/patches/fibonacci/)
         - [gridmode](https://dwm.suckless.org/patches/gridmode/)

   - [windowrolerule](patches/dwm-windowrolerule-6.2.diff)
      - sometimes a single application opens different windows depending on the task at hand and this is often reflected in the `WM_WINDOW_ROLE(STRING)` x property
      - this patch adds the role field to the rule configuration so that one can differentiate between, say, Firefox "browser" vs "Preferences" vs "Manager" or Google-chrome "browser" vs "pop-up"

---

I have included my personal keybindings (`config.h`) here for reference; feel free to delete this to start fresh with default configuration (from `config.def.h`). Most notable I guess is binding `rotatestack` and `cyclelayouts` to the mouse wheel which can be rather convenient when having many windows in the stack. I also mapped middle-click to move a window to master (`zoom`) rather than toggling floating mode which you enable anyway when dragging a window.

I should also note that to start various applications I have additional keybindings through sxhkd.

---

Example screenshot with exaggerated gaps:

![https://github.com/bakkeby/dwm-vanitygaps/wiki/vanitygaps.jpg](https://github.com/bakkeby/dwm-vanitygaps/wiki/vanitygaps.jpg)
