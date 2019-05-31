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
static void centeredmaster(Monitor *m);
static void centeredfloatingmaster(Monitor *m);
static void deck(Monitor *m);
static void dwindle(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void grid(Monitor *m);
static void nrowgrid(Monitor *m);
static void spiral(Monitor *m);
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
 * Centred master layout + gaps
 * https://dwm.suckless.org/patches/centeredmaster/
 */
void
centeredmaster(Monitor *m)
{
	int oh, ov, ih, iv;
	unsigned int i, n, r, h, mw, mx, my, oty, ety, tw;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	/* initialize areas */
	mw = m->ww - 2*ov;
	mx = ov;
	my = oh;
	tw = mw;

	if (!m->nmaster) {
		mw = 0;
		tw = m->ww - 2*ov;
		if (n > 1) {
			/* only one client */
			tw = (m->ww - 2*ov - iv) / 2;
			mx = ov + tw;
		}
	} else if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		mw = (m->ww - 2*ov) * m->mfact;
		tw = m->ww - mw - 2*ov - iv;

		if (n - m->nmaster > 1) {
			/* only one client */
			mx = (m->ww - mw) / 2;
			tw = (m->ww - mw - 2*ov - 2*iv ) / 2;
		}
	}

	oty = oh;
	ety = oh;
	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked vertically, in the center
		 * of the screen */
		r = (MIN(n, m->nmaster) - i);
		h = (m->wh - my - oh - ih*(r - 1)) / r;
		resize(c, m->wx + mx, m->wy + my, mw - (2*c->bw),
		       h - (2*c->bw), 0);
		my += HEIGHT(c) + ih;
	} else {
		/* stack clients are stacked vertically */
		if ((i - m->nmaster) % 2 ) {
			r = ((1 + n - i) / 2);
			h = (m->wh - ety - oh - ih*(r - 1)) / r;
			resize(c, m->wx + ov, m->wy + ety, tw - (2*c->bw),
			       h - (2*c->bw), 0);
			ety += HEIGHT(c) + ih;
		} else {
			r = ((1 + n - i) / 2);
			h = (m->wh - oty - oh - ih*(r - 1)) / r;
			resize(c, m->wx + mx + mw + iv, m->wy + oty,
			       tw - (2*c->bw), h - (2*c->bw), 0);
			oty += HEIGHT(c) + ih;
		}
	}
}

void
centeredfloatingmaster(Monitor *m)
{
	int oh, ov, ih, iv;
	float mivf;
	unsigned int i, r, n, w, mh, mw, mx, mxo, my, myo, tx;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;

	/* initialize nmaster area */
	if (!m->nmaster) {
		mh = m->wh - 2*oh;
		mw = m->ww - 2*ov;
		mx = mxo = 0;
		my = myo = 0;
	} else if (n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->ww * m->mfact;
			mh = m->wh * 0.9;
		} else {
			mh = m->wh * m->mfact;
			mw = m->ww * 0.9;
		}
		mx = mxo = (m->ww - mw) / 2;
		my = myo = (m->wh - mh - 2*oh) / 2;
	} else {
		/* go fullscreen if all clients are in the master area */
		mh = m->wh - 2*oh;
		mw = m->ww - 2*ov;
		mx = mxo = ov;
		my = myo = oh;
	}

	for(i = 0, tx = ov, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
	if (i < m->nmaster) {
		/* nmaster clients are stacked horizontally, in the center
		 * of the screen */
		mivf = 0.8; // master inner vertical gap factor
		r = (MIN(n, m->nmaster) - i);
		w = (mw + mxo - mx - iv*mivf*(r - 1)) / r;
		resize(c, m->wx + mx, m->wy + my, w - (2*c->bw),
		       mh - (2*c->bw), 0);
		mx += WIDTH(c) + iv*mivf;
		focus(c);
	} else {
		/* stack clients are stacked horizontally */
		r = (n - i);
		w = (m->ww - tx - ov - iv*(r - 1)) / r;
		resize(c, m->wx + tx, m->wy + oh, w - (2*c->bw),
		       m->wh - (2*c->bw) - 2*oh, 0);
		tx += WIDTH(c) + iv;
	}
	restack(m);
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
 * Gappless grid layout + gaps (ironically)
 * https://dwm.suckless.org/patches/gaplessgrid/
 */
void
gaplessgrid(Monitor *m) {
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n);
	if(n == 0)
		return;

	/* grid dimensions */
	for(cols = 0; cols <= n/2; cols++)
		if(cols*cols >= n)
			break;
	if(n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	/* window geometries */
	cw = cols ? (m->ww - 2*ov - iv*(cols - 1)) / cols : m->ww - 2*ov;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for(i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if(i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? (m->wh - 2*oh - ih*(rows - 1)) / rows : m->wh - 2*oh;
		cx = m->wx + ov + cn*(cw + iv);
		cy = m->wy + oh + rn*(ch + ih);
		resize(c, cx, cy, cw - 2 * c->bw, ch - 2 * c->bw, False);
		rn++;
		if(rn >= rows) {
			rn = 0;
			cn++;
		}
	}
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
 * Horizontal grid layout + gaps
 * https://dwm.suckless.org/patches/horizgrid/
 */
void
horizgrid(Monitor *m) {
	Client *c;
	unsigned int n, i;
	int w = 0, oh, ov, ih, iv;
	int ntop, nbottom = 0;

	/* Count windows */
	getgaps(m, &oh, &ov, &ih, &iv, &n);

	if (n == 0)
		return;
	else if (n == 1) { /* Just fill the whole screen */
		c = nexttiled(m->clients);
		resize(c, m->wx + ov, m->wy + oh, m->ww - 2*ov - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
	} else if (n == 2) { /* Split vertically */
		w = (m->ww - 2*ov - iv) / 2;
		c = nexttiled(m->clients);
		resize(c, m->wx + ov, m->wy + oh, w - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
		c = nexttiled(c->next);
		resize(c, m->wx + ov + w + iv, m->wy + oh, w - (2*c->bw), m->wh - 2*oh - (2*c->bw), False);
	} else {
		ntop = n / 2;
		nbottom = n - ntop;
		for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
			if (i < ntop)
				resize(
					c,
					m->wx + ov + i * ((m->ww - 2*ov - iv*(ntop - 1)) / ntop + iv),
					m->wy + oh,
					(m->ww - 2*ov - iv*(ntop - 1)) / ntop - (2*c->bw),
					(m->wh - 2*oh - ih) / 2 - (2*c->bw),
					False
				);
			else
				resize(
					c,
					m->wx + ov + (i - ntop) * ((m->ww - 2*ov - iv*(nbottom - 1)) / nbottom + iv),
					m->wy + oh + ih + (m->wh - 2*oh - ih) / 2,
					(m->ww - 2*ov - iv*(nbottom - 1)) / nbottom - (2*c->bw),
					(m->wh - 2*oh - ih) / 2 - (2*c->bw),
					False
				);
		}
	}
}

/*
 * nrowgrid layout + gaps
 * https://dwm.suckless.org/patches/nrowgrid/
 */
void
nrowgrid(Monitor *m)
{
	unsigned int n = 0, i = 0, ri = 0, ci = 0;  /* counters */
	int oh, ov, ih, iv;                         /* vanitygap settings */
	unsigned int cx, cy, cw, ch;                /* client geometry */
	unsigned int uw = 0, uh = 0, uc = 0;        /* utilization trackers */
	unsigned int cols, rows = m->nmaster + 1;
	Client *c;

	/* count clients */
	getgaps(m, &oh, &ov, &ih, &iv, &n);

	/* nothing to do here */
	if (n == 0)
		return;

	/* force 2 clients to always split vertically */
	if (nrg_force_vsplit && n == 2)
		rows = 1;

	/* never allow empty rows */
	if (n < rows)
		rows = n;

	/* define first row */
	cols = n / rows;
	uc = cols;
	cy = m->wy + oh;
	ch = (m->wh - 2*oh - ih*(rows - 1)) / rows;
	uh = ch;

	for (c = nexttiled(m->clients); c; c = nexttiled(c->next), i++, ci++) {
		if (ci == cols) {
			uw = 0;
			ci = 0;
			ri++;

			/* next row */
			cols = (n - uc) / (rows - ri);
			uc += cols;
			cy = m->wy + oh + uh + ih;
			uh += ch + ih;
		}

		cx = m->wx + ov + uw;
		cw = (m->ww - 2*ov - uw) / (cols - ci);
		uw += cw + iv;

		resize(c, cx, cy, cw - 2 * c->bw, ch - 2 * c->bw, 0);
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