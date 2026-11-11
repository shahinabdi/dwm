/* rules.h — which app opens on which workspace (included from config.h).
 *
 * Find an app's class with:  xprop WM_CLASS   (then click the window)
 *   WM_CLASS(STRING) = "instance", "class"
 * class / instance / title are matched as substrings; start with '=' for an exact match
 * (e.g. "=obs", because "obs" is also inside Obsidian's class "md.obsidian.Obsidian").
 *
 * tags:        1 << (workspace - 1)      (0 = current workspace)
 * switchtotag: 1 = jump to that workspace when the app opens, 0 = open it there quietly
 */
static const Rule rules[] = {
	/* class                  instance  title  tags     isfloating  monitor  switchtotag */
	{ "kitty",                NULL,     NULL,  1 << 0,  0,          -1,      1 },
	{ "Brave-browser",        NULL,     NULL,  1 << 1,  0,          -1,      1 },
	{ "VSCode",               NULL,     NULL,  1 << 2,  0,          -1,      1 },  /* com.microsoft.VSCode */
	{ "Code",                 NULL,     NULL,  1 << 2,  0,          -1,      1 },  /* older VS Code builds */
	{ "Thunar",               NULL,     NULL,  1 << 3,  0,          -1,      1 },
	{ "proton-mail",          NULL,     NULL,  1 << 4,  0,          -1,      1 },
	{ "Obsidian",             NULL,     NULL,  1 << 5,  0,          -1,      1 },  /* md.obsidian.Obsidian */
	{ "=vlc",                 NULL,     NULL,  1 << 6,  0,          -1,      1 },  /* VLC (exact) */
	{ "=obs",                 NULL,     NULL,  1 << 9,  0,          -1,      1 },  /* OBS Studio (exact) */

	/* floating helpers */
	{ "Gimp",                 NULL,     NULL,  0,       1,          -1,      0 },
};
