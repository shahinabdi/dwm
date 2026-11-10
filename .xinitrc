setxkbmap -layout fr
xset r rate 250 40
feh --bg-fill "$HOME/dwm-btw/current.jpg"
picom --backend glx --vsync &
exec dwm