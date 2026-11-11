/* keyboard.h — keyboard profile for the keys that differ between layouts.
 * Chosen at build time:   make                    -> AZERTY (default)
 *                         make KEYBOARD=qwerty    -> QWERTY
 * (dwm matches the key's *unshifted* symbol, so the workspace keys must be the symbols the
 *  top row produces without Shift on that keyboard.)                                         */

#if defined(KEYBOARD_QWERTY)
/* QWERTY (US / UK / most of the world): number row 1 … 9 0 */
#define TK1  XK_1
#define TK2  XK_2
#define TK3  XK_3
#define TK4  XK_4
#define TK5  XK_5
#define TK6  XK_6
#define TK7  XK_7
#define TK8  XK_8
#define TK9  XK_9
#define TK10 XK_0
#define KEY_MONPREV XK_comma     /* Mod + ,   previous screen */
#define KEY_MONNEXT XK_period    /* Mod + .   next screen     */
#else
/* AZERTY (French): & é " ' ( - è _ ç à */
#define TK1  XK_ampersand
#define TK2  XK_eacute
#define TK3  XK_quotedbl
#define TK4  XK_apostrophe
#define TK5  XK_parenleft
#define TK6  XK_minus
#define TK7  XK_egrave
#define TK8  XK_underscore
#define TK9  XK_ccedilla
#define TK10 XK_agrave
#define KEY_MONPREV XK_comma     /* Mod + ,   previous screen */
#define KEY_MONNEXT XK_semicolon /* Mod + ;   next screen (the key with "." on Shift) */
#endif
