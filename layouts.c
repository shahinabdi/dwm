/* layouts.c — extra layouts for dwm-btw (included from config.h).
 * Ported from the suckless patches:
 *   bottomstack    https://dwm.suckless.org/patches/bottomstack/
 *   centeredmaster https://dwm.suckless.org/patches/centeredmaster/
 *   gridmode       https://dwm.suckless.org/patches/gridmode/
 *   deck           https://dwm.suckless.org/patches/deck/
 *   fibonacci      https://dwm.suckless.org/patches/fibonacci/
 */

/* TTT — master on top, the rest side by side underneath */
static void
bstack(Monitor *m)
{
	int w, h, mh, mx, tx, ty, tw;
	unsigned int i, n;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	if (n > m->nmaster) {
		mh = m->nmaster ? m->mfact * m->wh : 0;
		tw = m->ww / (n - m->nmaster);
		ty = m->wy + mh;
	} else {
		mh = m->wh;
		tw = m->ww;
		ty = m->wy;
	}
	for (i = mx = 0, tx = m->wx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy, w - (2 * c->bw), mh - (2 * c->bw), 0);
			mx += WIDTH(c);
		} else {
			h = m->wh - mh;
			resize(c, tx, ty, tw - (2 * c->bw), h - (2 * c->bw), 0);
			if (tw != m->ww)
				tx += WIDTH(c);
		}
	}
}

/* |M| — master in the middle, the rest split left and right */
static void
centeredmaster(Monitor *m)
{
	unsigned int i, n, h, mw, mx, my, oty, ety, tw;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	mw = m->ww;
	mx = my = 0;
	tw = mw;
	if (n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		tw = m->ww - mw;
		if (n - m->nmaster > 1) {
			mx = (m->ww - mw) / 2;
			tw = (m->ww - mw) / 2;
		}
	}
	oty = ety = 0;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx + mx, m->wy + my, mw - (2 * c->bw), h - (2 * c->bw), 0);
			my += HEIGHT(c);
		} else if ((i - m->nmaster) % 2) {
			h = (m->wh - ety) / ((1 + n - i) / 2);
			resize(c, m->wx, m->wy + ety, tw - (2 * c->bw), h - (2 * c->bw), 0);
			ety += HEIGHT(c);
		} else {
			h = (m->wh - oty) / ((1 + n - i) / 2);
			resize(c, m->wx + mx + mw, m->wy + oty, tw - (2 * c->bw), h - (2 * c->bw), 0);
			oty += HEIGHT(c);
		}
}

/* ### — every window gets an equal cell */
static void
grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	for (rows = 0; rows <= n / 2; rows++)
		if (rows * rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* last row / column takes the leftover pixels */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, 0);
	}
}

/* [D] — master on the left, the rest stacked like cards on the right (one visible) */
static void
deck(Monitor *m)
{
	unsigned int i, n, h, mw, my;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	if (n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[D%d]", n - m->nmaster);
	} else
		mw = m->ww;
	for (i = my = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i);
			resize(c, m->wx, m->wy + my, mw - (2 * c->bw), h - (2 * c->bw), 0);
			my += HEIGHT(c);
		} else
			resize(c, m->wx + mw, m->wy, m->ww - mw - (2 * c->bw), m->wh - (2 * c->bw), 0);
}

/* fibonacci: each new window takes half of the previous one (s=1 dwindle, s=0 spiral) */
static void
fibonacci(Monitor *mon, int s)
{
	unsigned int i, n, nx, ny, nw, nh;
	Client *c;

	for (n = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	nx = mon->wx;
	ny = 0;
	nw = mon->ww;
	nh = mon->wh;
	for (i = 0, c = nexttiled(mon->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2 * c->bw) || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if (i < n - 1) {
				if (i % 2)
					nh /= 2;
				else
					nw /= 2;
				if ((i % 4) == 2 && !s)
					nx += nw;
				else if ((i % 4) == 3 && !s)
					ny += nh;
			}
			if ((i % 4) == 0) {
				if (s)
					ny += nh;
				else
					ny -= nh;
			} else if ((i % 4) == 1)
				nx += nw;
			else if ((i % 4) == 2)
				ny += nh;
			else if ((i % 4) == 3) {
				if (s)
					nx += nw;
				else
					nx -= nw;
			}
			if (i == 0) {
				if (n != 1)
					nw = mon->ww * mon->mfact;
				ny = mon->wy;
			} else if (i == 1)
				nw = mon->ww - nw;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, 0);
	}
}

/* [\] — dwindle: windows shrink towards the bottom-right corner */
static void
dwindle(Monitor *m)
{
	fibonacci(m, 1);
}

/* [@] — spiral: windows shrink in a spiral towards the centre */
static void
spiral(Monitor *m)
{
	fibonacci(m, 0);
}
