My dwm 6.2 build with custom patches. Refer to [https://dwm.suckless.org/](https://dwm.suckless.org/) for details on the dwm window manager, how to install it and how it works.

---

Included patches:

   - [alpha](patches/dwm-alpha-6.2.diff)
      - adds transparency for the status bar
      - refer to [https://dwm.suckless.org/patches/alpha/](https://dwm.suckless.org/patches/alpha/)

   - [cyclelayouts](patches/dwm-cyclelayouts-6.2.diff)
      - let's you cycle through all your layouts
      - useful for demo purposes
      - refer to [https://dwm.suckless.org/patches/cyclelayouts/](https://dwm.suckless.org/patches/cyclelayouts/)

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

---

I have included my personal keybindings (`config.h`) here for reference; feel free to delete this to start fresh with default configuration (from `config.def.h`). Most notable I guess is binding `rotatestack` and `cyclelayouts` to the mouse wheel which can be rather convenient when having many windows in the stack. I also mapped middle-click to move a window to master (`zoom`) rather than toggling floating mode which you enable anyway when dragging a window.

I should also note that to start various applications I have additional keybindings through sxhkd.

---

Example screenshot with exaggerated gaps:

![https://github.com/bakkeby/dwm-vanitygaps/wiki/vanitygaps.jpg](https://github.com/bakkeby/dwm-vanitygaps/wiki/vanitygaps.jpg)
