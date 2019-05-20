/* Key binding functions */
static void defaultgaps(const Arg *arg);
static void incrgaps(const Arg *arg);
static void incrigaps(const Arg *arg);
static void incrogaps(const Arg *arg);
static void incrohgaps(const Arg *arg);
static void incrovgaps(const Arg *arg);
static void incrihgaps(const Arg *arg);
static void incrivgaps(const Arg *arg);
static void togglegaps(const Arg *arg);
/* Layouts */
static void bstack(Monitor *m);
static void bstackhoriz(Monitor *m);
static void deck(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void dwindle(Monitor *m);
static void spiral(Monitor *m);
static void grid(Monitor *m);
static void tile(Monitor *m);
/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
static void setgaps(int oh, int ov, int ih, int iv);

/* Settings */
static int enablegaps = 1;

static void
setgaps(int oh, int ov, int ih, int iv)
{
	if (oh < 0) oh = 0;
	if (ov < 0) ov = 0;
	if (ih < 0) ih = 0;
	if (iv < 0) iv = 0;

	selmon->gappoh = oh;
	selmon->gappov = ov;
	selmon->gappih = ih;
	selmon->gappiv = iv;
	arrange(selmon);
}

static void
togglegaps(const Arg *arg)
{
	enablegaps = !enablegaps;
	arrange(NULL);
}

static void
defaultgaps(const Arg *arg)
{
	setgaps(gappoh, gappov, gappih, gappiv);
}

static void
incrgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov + arg->i,
		selmon->gappih + arg->i,
		selmon->gappiv + arg->i
	);
}

static void
incrigaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov,
		selmon->gappih + arg->i,
		selmon->gappiv + arg->i
	);
}

static void
incrogaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov + arg->i,
		selmon->gappih,
		selmon->gappiv
	);
}

static void
incrohgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh + arg->i,
		selmon->gappov,
		selmon->gappih,
		selmon->gappiv
	);
}

static void
incrovgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov + arg->i,
		selmon->gappih,
		selmon->gappiv
	);
}

static void
incrihgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov,
		selmon->gappih + arg->i,
		selmon->gappiv
	);
}

static void
incrivgaps(const Arg *arg)
{
	setgaps(
		selmon->gappoh,
		selmon->gappov,
		selmon->gappih,
		selmon->gappiv + arg->i
	);
}

static void
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc )
{
	unsigned int n, oe = enablegaps, ie = enablegaps;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (smartgaps && n == 1) {
		oe = 0; // outer gaps disabled when only one client
	}

	*oh = m->gappoh*oe; // outer horizontal gap
	*ov = m->gappov*oe; // outer vertical gap
	*ih = m->gappih*ie; // inner horizontal gap
	*iv = m->gappiv*ie; // inner vertical gap
	*nc = n;            // number of clients
}

/***
 * Layouts
 */

/*
 * Bottomstack layout + gaps
 * https://dwm.suckless.org/patches/bottomstack/
 */
static void
bstack(Monitor *m)
{
	int w, h, mh, mx, tx, ty, tw, oh, ov, ih, iv;
	unsigned int i, n, r;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	if (!m->nmaster) {
		mh = 0;
		tw = (m->ww - 2*ov - iv * (n - 1)) / n;
		ty = m->wy + oh;
	} else if (n > m->nmaster) {
		r = n - m->nmaster;
		mh = (m->wh - ih) * m->mfact;
		tw = (m->ww - 2*ov - iv * (r - 1)) / r;
		ty = m->wy + mh + ih + oh;
	} else {
		mh = m->wh - 2*oh;
		tw = m->ww;
		ty = m->wy + oh;
	}

	for (i = 0, mx = ov, tx = m->wx + ov, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			r = MIN(n, m->nmaster) - i;
			w = (m->ww - mx - ov - iv * (r - 1)) / r;
			resize(c, m->wx + mx, m->wy + oh, w - (2 * c->bw), mh - (2 * c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			r = n - i;
			h = m->wh - mh - 2*oh - (m->nmaster ? ih : 0);
			resize(c, tx, ty, tw - (2 * c->bw), h - (2 * c->bw), 0);
			if (tw <= m->ww)
				tx += WIDTH(c) + iv;
		}
	}
}

static void
bstackhoriz(Monitor *m)
{
	int w, mh, mx, tx, ty, th, oh, ov, ih, iv;
	unsigned int i, n, r;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	if (!m->nmaster) {
		mh = 0;
		th = (m->wh - 2*oh - ih * (n - 1)) / n;
		ty = m->wy + oh;
	} else if (n > m->nmaster) {
		mh = (m->wh - ih) * m->mfact;
		r  = n - m->nmaster;
		th = (m->wh - 2*oh - mh - ih * r) / r;
		ty = m->wy + mh + oh + (m->nmaster ? ih : 0);
	} else {
		th = m->wh;
		mh = m->wh - 2*oh;
		ty = m->wy;
	}
	for (i = 0, mx = ov, tx = m->wx + ov, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			r = MIN(n, m->nmaster) - i;
			w = (m->ww - ov - mx - iv * (r - 1)) / r;
			resize(c, m->wx + mx, m->wy + oh, w - (2 * c->bw), mh - (2 * c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, tx, ty, m->ww - (2 * c->bw) - 2*ov, th - (2 * c->bw), 0);
			if (th < m->wh)
				ty += HEIGHT(c) + ih;
		}
	}
}

/*
 * Deck layout + gaps
 * https://dwm.suckless.org/patches/deck/
 */
static void
deck(Monitor *m)
{
	int dn, oh, ov, ih, iv;
	unsigned int i, n, h, mw, my, r;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	dn = n - m->nmaster;
	if (dn > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "D %d", dn);

	if (n > m->nmaster)
		mw = m->nmaster ? (m->ww - iv) * m->mfact: 0;
	else
		mw = m->ww - 2*ov;
	for (i = 0, my = oh, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			r = MIN(n, m->nmaster) - i;
			h = (m->wh - my - oh - ih * (r - 1)) / r;
			resize(c, m->wx + ov, m->wy + my, mw - (2*c->bw), h - (2*c->bw), False);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, m->wx + mw + ov + (m->nmaster ? iv : 0), m->wy + oh, m->ww - mw - (2*c->bw) - 2*ov - (m->nmaster ? iv : 0), m->wh - (2*c->bw) - 2*oh, False);
		}
}

/*
 * Fibonacci layout + gaps
 * https://dwm.suckless.org/patches/fibonacci/
 */
static void
fibonacci(Monitor *m, int s)
{
	unsigned int i, n, nx, ny, nw, nh;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;
	
	nx = m->wx + ov;
	ny = oh;
	nw = m->ww - 2*ov;
	nh = m->wh - 2*oh;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if (i < n - 1) {
				if (i % 2)
					nh = (nh - ih) / 2;
				else
					nw = (nw - iv) / 2;

				if ((i % 4) == 2 && !s)
					nx += nw + iv;
				else if ((i % 4) == 3 && !s)
					ny += nh + ih;
			}
			if ((i % 4) == 0) {
				if (s)
					ny += nh + ih;
				else
					ny -= nh + ih;
			}
			else if ((i % 4) == 1)
				nx += nw + iv;
			else if ((i % 4) == 2)
				ny += nh + ih;
			else if ((i % 4) == 3) {
				if (s)
					nx += nw + iv;
				else
					nx -= nw + iv;
			}
			if (i == 0)	{
				if (n != 1)
					nw = (m->ww - iv) * m->mfact;
				ny = m->wy + oh;
			}
			else if (i == 1)
				nw = m->ww - nw - iv - 2*ov;
			i++;
		}
		resize(c, nx, ny, nw - 2 * c->bw, nh - 2 * c->bw, False);
	}
}

static void
dwindle(Monitor *m)
{
	fibonacci(m, 1);
}

static void
spiral(Monitor *m)
{
	fibonacci(m, 0);
}

/*
 * Gridmode layout + gaps
 * https://dwm.suckless.org/patches/gridmode/
 */
static void
grid(Monitor *m)
{
	unsigned int i, n, cx, cy, cw, ch, cols, rows;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	/* grid dimensions */
	for (rows = 0; rows <= n/2; rows++)
		if (rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (m->wh - 2*oh - ih * (rows - 1)) / (rows ? rows : 1);
	cw = (m->ww - 2*ov - iv * (cols - 1)) / (cols ? cols : 1);
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * (cw + iv) + ov;
		cy = m->wy + (i % rows) * (ch + ih) + oh;
		resize(c, cx, cy, cw - 2 * c->bw, ch - 2 * c->bw, False);
		i++;
	}
}

/*
 * Default tile layout + gaps
 */
static void
tile(Monitor *m)
{
	unsigned int i, n, h, r, mw, my, ty;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	if (n > m->nmaster)
		mw = m->nmaster ? (m->ww - iv) * m->mfact: 0;
	else
		mw = m->ww - 2*ov;
	for (i = 0, my = ty = oh, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			r = MIN(n, m->nmaster) - i;
			h = (m->wh - my - oh - ih * (r - 1)) / r;
			resize(c, m->wx + ov, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			r = n - i;
			h = (m->wh - ty - oh - ih * (r - 1)) / r;
			resize(c, m->wx + mw + ov + (m->nmaster ? iv : 0), m->wy + ty, m->ww - mw - (2*c->bw) - 2*ov - (m->nmaster ? iv : 0), h - (2*c->bw), 0);
			ty += HEIGHT(c) + ih;
		}
}