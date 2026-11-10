xrdb -merge "$HOME/.Xresources"
xsetroot -cursor_name left_ptr

# keyboard: the system-wide layout lives in /etc/X11/xorg.conf.d/00-keyboard.conf
# (set with `sudo localectl set-x11-keymap fr`) so hot-plugged / reconnected
# keyboards also get fr. This line is only a fallback.
setxkbmap -layout fr
xset r rate 250 40

# let user services (pipewire, etc.) see the X display
systemctl --user import-environment DISPLAY XAUTHORITY

feh --bg-fill "$HOME/dwm-btw/current.jpg"
picom --backend glx --vsync &
dunst &
dwm-status &

exec dwm
