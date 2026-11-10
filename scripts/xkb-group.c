/* xkb-group — print or switch the active XKB layout group.
 *   xkb-group         print current group index (0-based)
 *   xkb-group next    switch to the next group (wraps around)
 *   xkb-group N       switch to group N
 * build: cc -O2 -o ~/.local/bin/xkb-group xkb-group.c -lX11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

int
main(int argc, char *argv[])
{
	Display *dpy;
	XkbStateRec st;
	XkbDescPtr desc;
	int n = 1, g;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "xkb-group: cannot open display\n");
		return 1;
	}
	if (XkbGetState(dpy, XkbUseCoreKbd, &st) != Success) {
		fprintf(stderr, "xkb-group: cannot read keyboard state\n");
		return 1;
	}
	if (argc < 2) {
		printf("%d\n", st.group);
		XCloseDisplay(dpy);
		return 0;
	}
	if ((desc = XkbAllocKeyboard())) {
		if (XkbGetControls(dpy, XkbGroupsWrapMask, desc) == Success && desc->ctrls)
			n = desc->ctrls->num_groups;
		XkbFreeKeyboard(desc, 0, True);
	}
	if (n < 1)
		n = 1;
	g = strcmp(argv[1], "next") == 0 ? (st.group + 1) % n : atoi(argv[1]) % n;
	XkbLockGroup(dpy, XkbUseCoreKbd, g);
	XSync(dpy, False);
	printf("%d\n", g);
	XCloseDisplay(dpy);
	return 0;
}
