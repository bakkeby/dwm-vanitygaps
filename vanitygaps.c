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
static void flextile(Monitor *m);
static void gaplessgrid(Monitor *m);
static void grid(Monitor *m);
static void horizgrid(Monitor *m);
static void nrowgrid(Monitor *m);
static void spiral(Monitor *m);
static void tile(Monitor *m);
/* Internals */
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc, float *mf, float *sf);
static void setgaps(int oh, int ov, int ih, int iv);
static void setflexsymbols(Monitor *m, unsigned int n);

/* Settings */
static int enablegaps = 1;

/* Named flextile constants */
#define LAYOUT 0
#define MASTER 1
#define STACK 2
#define SPLIT_VERTICAL 1   // master stack vertical split
#define SPLIT_HORIZONTAL 2 // master stack horizontal split
#define SPLIT_CENTERED_V 3 // centered master vertical split
#define SPLIT_CENTERED_H 4 // centered master horizontal split
#define LEFT_TO_RIGHT 1    // clients are stacked horizontally
#define TOP_TO_BOTTOM 2    // clients are stacked vertically
#define MONOCLE 3          // clients are stacked in deck / monocle mode
#define GRID 4             // clients are stacked in grid mode

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
getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc, float *mf, float *sf)
{
	unsigned int n, oe = enablegaps, ie = enablegaps;
	float mfacts = 0, sfacts = 0;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (m->nmaster && n < m->nmaster)
			mfacts += c->cfact;
		else
			sfacts += c->cfact;
	}
	if (smartgaps && n == 1) {
		oe = 0; // outer gaps disabled when only one client
	}

	*oh = m->gappoh*oe; // outer horizontal gap
	*ov = m->gappov*oe; // outer vertical gap
	*ih = m->gappih*ie; // inner horizontal gap
	*iv = m->gappiv*ie; // inner vertical gap
	*nc = n;            // number of clients
	*mf = mfacts;       // total factor of master area
	*sf = sfacts;       // total factor of stack area
}

/***
 * Layouts
 */

/*
 * Bottomstack layout + gaps + cfacts
 * https://dwm.suckless.org/patches/bottomstack/
 */
static void
bstack(Monitor *m)
{
	unsigned int i, n;
	int mx, my, mh, mw, sx = 0, sy = 0, sh = 0, sw = 0, oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv * (MIN(n, m->nmaster) - 1);

	if (m->nmaster && n > m->nmaster) {
		sh = (mh - ih) * (1 - m->mfact);
		mh = (mh - ih) * m->mfact;
		sx = mx;
		sy = my + mh + ih;
		sw = m->ww - 2*ov - iv * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw * (c->cfact / sfacts) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
	}
}

static void
bstackhoriz(Monitor *m)
{
	unsigned int i, n;
	int mx, my, mh, mw, sx = 0, sy = 0, sh = 0, sw = 0, oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv * (MIN(n, m->nmaster) - 1);

	if (m->nmaster && n > m->nmaster) {
		sh = (mh - ih) * (1 - m->mfact);
		mh = (mh - ih) * m->mfact;
		sx = mx;
		sy = my + mh + ih;
		sh = m->wh - mh - 2*oh - ih * (n - m->nmaster);
		sw = m->ww - 2*ov;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (c->cfact / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
	}
}

/*
 * Centred master layout + gaps + cfacts
 * https://dwm.suckless.org/patches/centeredmaster/
 */
void
centeredmaster(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int lx = 0, ly = 0, lw = 0, lh = 0;
	int rx = 0, ry = 0, rw = 0, rh = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts, lfacts = 0, rfacts = 0;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	mfacts = 0;
	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) {
		if (!m->nmaster || n < m->nmaster)
			mfacts += c->cfact; // total factor of master area
		else if ((n - m->nmaster) % 2)
			lfacts += c->cfact; // total factor of left hand stacke area
		else
			rfacts += c->cfact; // total factor of right hand stack area
	}

	/* initialize areas */
	mx = m->wx + ov;
	my = m->wy + oh;
	mh = m->wh - 2*oh - ih * ((!m->nmaster ? n : MIN(n, m->nmaster)) - 1);
	mw = m->ww - 2*ov;
	lh = m->wh - 2*oh - ih * (((n - m->nmaster) / 2) - 1);
	rh = m->wh - 2*oh - ih * (((n - m->nmaster) / 2) - ((n - m->nmaster) % 2 ? 0 : 1));

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (n - m->nmaster > 1) {
			/* ||<-S->|<---M--->|<-S->|| */
			mw = (m->ww - 2*ov - 2*iv) * m->mfact;
			lw = (m->ww - mw - 2*ov - 2*iv) / 2;
			mx += lw + iv;
		} else {
			/* ||<---M--->|<-S->|| */
			mw = (mw - iv) * m->mfact;
			lw = m->ww - mw - iv - 2*ov;
		}
		rw = lw;
		lx = m->wx + ov;
		ly = m->wy + oh;
		rx = mx + mw + iv;
		ry = m->wy + oh;
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (!m->nmaster || i < m->nmaster) {
			/* nmaster clients are stacked vertically, in the center of the screen */
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			/* stack clients are stacked vertically */
			if ((i - m->nmaster) % 2 ) {
				resize(c, lx, ly, lw - (2*c->bw), lh * (c->cfact / lfacts) - (2*c->bw), 0);
				ly += HEIGHT(c) + ih;
			} else {
				resize(c, rx, ry, rw - (2*c->bw), rh * (c->cfact / rfacts) - (2*c->bw), 0);
				ry += HEIGHT(c) + ih;
			}
		}
	}
}

void
centeredfloatingmaster(Monitor *m)
{
	unsigned int i, n;
	float mivf, mfacts, sfacts;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	mivf = 0.8; // master inner vertical gap factor

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh;
	sw = mw = m->ww - 2*ov - iv*(n - 1);

	if (m->nmaster && n > m->nmaster) {
		/* go mfact box in the center if more than nmaster clients */
		if (m->ww > m->wh) {
			mw = m->ww * m->mfact - iv*mivf*(MIN(n, m->nmaster) - 1);
			mh = m->wh * 0.9;
		} else {
			mw = m->ww * 0.9 - iv*mivf*(MIN(n, m->nmaster) - 1);
			mh = m->wh * m->mfact;
		}
		mx = m->wx + (m->ww - mw) / 2;
		my = m->wy + (m->wh - mh - 2*oh) / 2;

		sx = m->wx + ov;
		sy = m->wy + oh;
		sh = m->wh - 2*oh;
		sw = m->ww - 2*ov - iv*(n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i < m->nmaster) {
			/* nmaster clients are stacked horizontally, in the center of the screen */
			resize(c, mx, my, mw * (c->cfact / mfacts) - (2*c->bw), mh - (2*c->bw), 0);
			mx += WIDTH(c) + iv*mivf;
			focus(c);
		} else {
			/* stack clients are stacked horizontally */
			resize(c, sx, sy, sw * (c->cfact / sfacts) - (2*c->bw), sh - (2*c->bw), 0);
			sx += WIDTH(c) + iv;
		}
	}
	restack(m);
}

/*
 * Deck layout + gaps + cfacts
 * https://dwm.suckless.org/patches/deck/
 */
static void
deck(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sy = my;
		sh = m->wh - 2*oh;
	}

	if (n - m->nmaster > 0) /* override layout symbol */
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "D %d", n - m->nmaster);

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh - (2*c->bw), 0);
		}
}

/*
 * Fibonacci layout + gaps
 * https://dwm.suckless.org/patches/fibonacci/
 */
static void
fibonacci(Monitor *m, int s)
{
	unsigned int i, n;
	int nx, ny, nw, nh;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	nx = m->wx + ov;
	ny = oh;
	nw = m->ww - 2*ov;
	nh = m->wh - 2*oh;

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		if ((i % 2 && nh / 2 > 2*c->bw)
		   || (!(i % 2) && nw / 2 > 2*c->bw)) {
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
					nw = (m->ww - 2*ov - iv) * m->mfact;
				ny = m->wy + oh;
			}
			else if (i == 1)
				nw = m->ww - nw - iv - 2*ov;
			i++;
		}

		resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), False);
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

/* Flextile layout + gaps + cfacts + grid
 * https://dwm.suckless.org/patches/flextile/
 */
static void
flextile(Monitor *m) {
	unsigned int i, n, nc = 0, sc = 0, lt, cn = 0, rn = 0, cc = 0; // counters
	int cols = 1, rows = 1;
	int x, y, h, w;     // master x, y, height, width
	int sx, sy, sh, sw; // stack x, y, height, width
	int ox, oy;         // other stack x, y (centered layout)
	int oh, ov, ih, iv; // gaps outer/inner horizontal/vertical

	float facts, sfacts, ofacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &facts, &sfacts);
	setflexsymbols(m, n);

	if (n == 0)
		return;

	/* No outer gap if full screen monocle */
	if ((!m->nmaster && m->ltaxis[STACK] == MONOCLE) || (n <= m->nmaster && m->ltaxis[MASTER] == MONOCLE)) {
		ox = sx = x = m->wx;
		oy = sy = y = m->wy;
		sh = h = m->wh;
		sw = w = m->ww;
	} else {
		ox = sx = x = m->wx + ov;
		oy = sy = y = m->wy + oh;
		sh = h = m->wh - 2*oh;
		sw = w = m->ww - 2*ov;
	}
	ofacts = sfacts;
	sc = n - m->nmaster;


	/* Split master into master + stack if we have enough clients */
	if (m->nmaster && n > m->nmaster) {
		if (abs(m->ltaxis[LAYOUT]) == SPLIT_VERTICAL
				|| (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_V && n == m->nmaster + 1)) {
			sw = (w - iv) * (1 - m->mfact);
			w = (w - iv) * m->mfact;
			if (m->ltaxis[LAYOUT] < 0) // mirror
				x = sx + sw + iv;
			else
				sx = x + w + iv;
		} else if (abs(m->ltaxis[LAYOUT]) == SPLIT_HORIZONTAL
				|| (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_H && n == m->nmaster + 1)) {
			sh = (h - ih) * (1 - m->mfact);
			h = (h - ih) * m->mfact;
			if (m->ltaxis[LAYOUT] < 0) // mirror
				y = sy + sh + ih;
			else
				sy = y + h + ih;
		} else if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_V) {
			sw = (w - 2*iv) * (1 - m->mfact) / 2;
			w = (w - 2*iv) * m->mfact;
			x = sx + sw + iv;
			if (m->ltaxis[LAYOUT] < 0) // mirror
				ox = x + w + iv;
			else
				sx = x + w + iv;
		} else if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_H) {
			sh = (h - 2*ih) * (1 - m->mfact) / 2;
			h = (h - 2*ih) * m->mfact;
			y = sy + sh + ih;
			if (m->ltaxis[LAYOUT] < 0) // mirror
				oy = y + h + ih;
			else
				sy = y + h + ih;
		}

		if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_V || abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_H) {
			sc = (n - m->nmaster) / 2 + ((n - m->nmaster) % 2 > 0 ? 1 : 0);
			facts = sfacts = ofacts = 0;
			for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
				if (i < m->nmaster)
					facts += c->cfact; // total factor of master area
				else if (sc && i < m->nmaster + sc)
					sfacts += c->cfact; // total factor of first stack area
				else
					ofacts += c->cfact; // total factor of second stack area
			}
		}
	}

	for (i = 0, lt = MASTER, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
		if (i == 0 || (m->nmaster && i == m->nmaster) || i == (m->nmaster + sc)) {
			nc = MIN(n, m->nmaster);
			if (!m->nmaster || i == m->nmaster) { // switch to stack area
				x = sx, y = sy, h = sh, w = sw, facts = sfacts, lt = STACK;
				nc = sc;
			} else if (i > 0 && i == (m->nmaster + sc)) { // switch to second stack area
				x = ox, y = oy, h = sh, w = sw, nc = n - i, facts = ofacts;
			}

			if (m->ltaxis[lt] == LEFT_TO_RIGHT)
				w -= iv * (nc - 1);
			else if (m->ltaxis[lt] == TOP_TO_BOTTOM)
				h -= ih * (nc - 1);
			else if (m->ltaxis[lt] == GRID) {
				/* grid dimensions */
				for (cols = 1; cols <= nc/2; cols++)
					if (cols*cols >= nc)
						break;
				if (nc == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
					cols = 2;
				rows = nc/cols;
				cn = rn = cc = 0; // reset cell no, row no, client count
			}
		}

		if (m->ltaxis[lt] == LEFT_TO_RIGHT) {
			resize(c, x, y, w * (c->cfact / facts) - (2*c->bw), h - (2*c->bw), 0);
			x = x + WIDTH(c) + iv;
		} else if (m->ltaxis[lt] == TOP_TO_BOTTOM) {
			resize(c, x, y, w - (2*c->bw), h * (c->cfact / facts) - (2*c->bw), 0);
			y = y + HEIGHT(c) + ih;
		} else if (m->ltaxis[lt] == MONOCLE) {
			resize(c, x, y, w - (2*c->bw), h - (2*c->bw), 0);
		} else if (m->ltaxis[lt] == GRID) {
			if (cc/rows + 1 > cols - nc%cols)
				rows = nc/cols + 1;
			resize(c,
				x + cn*((w - iv*(cols - 1)) / cols + iv),
				y + rn*((h - ih*(rows - 1)) / rows + ih),
				(w - iv*(cols - 1)) / cols,
				(h - ih*(rows - 1)) / rows,
				0);
			rn++;
			cc++;
			if (rn >= rows) {
				rn = 0;
				cn++;
			}
		}
	}
}


static void
setflexsymbols(Monitor *m, unsigned int n) {
	char sym1 = 61, sym2 = 93, sym3 = 61, sym = 0;

	/* Predefined layouts */
	/* bstack */
	if (abs(m->ltaxis[LAYOUT]) == SPLIT_HORIZONTAL && m->ltaxis[MASTER] == LEFT_TO_RIGHT && m->ltaxis[STACK] == LEFT_TO_RIGHT) {
		snprintf(m->ltsymbol, sizeof m->ltsymbol, (m->ltaxis[LAYOUT] < 0 ? "⚍⚍⚍" : "⚎⚎⚎"));
		return;
	}

	/* bstackhoriz */
	if (abs(m->ltaxis[LAYOUT]) == SPLIT_HORIZONTAL && m->ltaxis[MASTER] == LEFT_TO_RIGHT && m->ltaxis[STACK] == TOP_TO_BOTTOM) {
		snprintf(m->ltsymbol, sizeof m->ltsymbol, (m->ltaxis[LAYOUT] < 0 ? "☳☳☳" : "☶☶☶"));
		return;
	}

	/* centered master horizontal split */
	if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_H && m->ltaxis[MASTER] == TOP_TO_BOTTOM && m->ltaxis[STACK] == TOP_TO_BOTTOM) {
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "☰☰☰");
		return;
	}

	if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_H && m->ltaxis[MASTER] == LEFT_TO_RIGHT && m->ltaxis[STACK] == LEFT_TO_RIGHT) {
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "☵☵☵");
		return;
	}

	/* monocle */
	if (n <= 1 && ((!m->nmaster && m->ltaxis[STACK] == MONOCLE) || (n <= m->nmaster && m->ltaxis[MASTER] == MONOCLE))) {
		snprintf(m->ltsymbol, sizeof m->ltsymbol, "[M]");
		return;
	}

	/* Layout symbols */
	if (abs(m->ltaxis[LAYOUT]) == SPLIT_VERTICAL) {
		if (m->nmaster > 1 || m->ltaxis[MASTER] == MONOCLE)
			sym2 = 124; // |
		else if (m->ltaxis[LAYOUT] < 0)
			sym2 = 91; // [
		else
			sym2 = 93; // ]
	} else if (abs(m->ltaxis[LAYOUT]) == SPLIT_HORIZONTAL) {
		if (m->nmaster > 1 || m->ltaxis[MASTER] == MONOCLE)
			sym2 = 58; // :
		else if (m->ltaxis[LAYOUT] < 0)
			sym2 = 91; // [
		else
			sym2 = 93; // ]
	} else if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_V) {
		if (m->ltaxis[LAYOUT] < 0)
			sym2 = 87; // W
		else
			sym2 = 77; // M
	} else if (abs(m->ltaxis[LAYOUT]) == SPLIT_CENTERED_H) {
		if (m->ltaxis[LAYOUT] < 0)
			sym2 = 87; // W
		else
			sym2 = 77; // M
	}

	if (m->ltaxis[MASTER] == LEFT_TO_RIGHT)
		sym1 = 124; // | ⏸
	else if (m->ltaxis[MASTER] == TOP_TO_BOTTOM)
		sym1 = 61; // =
	else if (m->ltaxis[MASTER] == MONOCLE)
		sym1 = MIN(n, m->nmaster);
	else if (m->ltaxis[MASTER] == GRID)
		sym1 = 35; // #

	if (m->ltaxis[STACK] == LEFT_TO_RIGHT)
		sym3 = 124; // |
	else if (m->ltaxis[STACK] == TOP_TO_BOTTOM)
		sym3 = 61; // =
	else if (m->ltaxis[STACK] == MONOCLE)
		sym3 = n - m->nmaster;
	else if (m->ltaxis[STACK] == GRID)
		sym3 = 35; // #


	/* Generic symbols */
	if (!m->nmaster) {
		if (m->ltaxis[STACK] == MONOCLE) {
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%d%c", 91, sym3, 93);
		} else {
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%c%c", sym3, sym3, sym3);
		}
		return;
	}

	if (n <= m->nmaster) {
		if (m->ltaxis[MASTER] == MONOCLE) {
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%d%c", 91, sym1, 93);
		} else {
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%c%c", 91, sym1, 93);
		}
	} else {
		if (m->ltaxis[LAYOUT] < 0) {
			sym = sym1;
			sym1 = sym3;
			sym3 = sym;
		}
		if (m->nmaster == 1 && abs(m->ltaxis[LAYOUT]) <= SPLIT_HORIZONTAL && m->ltaxis[MASTER] != MONOCLE) {
			if (m->ltaxis[LAYOUT] > 0)
				sym1 = 91;
			else
				sym3 = 93;
		}
		if (m->ltaxis[MASTER] == MONOCLE && m->ltaxis[STACK] == MONOCLE)
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%d%c%d", sym1, sym2, sym3);
		else if ((m->nmaster && m->ltaxis[MASTER] == MONOCLE && m->ltaxis[LAYOUT] > 0) || (m->ltaxis[STACK] == MONOCLE && m->ltaxis[LAYOUT] < 0))
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%d%c%c", sym1, sym2, sym3);
		else if ((m->ltaxis[STACK] == MONOCLE && m->ltaxis[LAYOUT] > 0) || (m->nmaster && m->ltaxis[MASTER] == MONOCLE && m->ltaxis[LAYOUT] < 0))
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%c%d", sym1, sym2, n - m->nmaster);
		else
			snprintf(m->ltsymbol, sizeof m->ltsymbol, "%c%c%c", sym1, sym2, sym3);
	}
}

/*
 * Gappless grid layout + gaps (ironically)
 * https://dwm.suckless.org/patches/gaplessgrid/
 */
void
gaplessgrid(Monitor *m)
{
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);
	if (n == 0)
		return;

	/* grid dimensions */
	for (cols = 0; cols <= n/2; cols++)
		if (cols*cols >= n)
			break;
	if (n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	/* window geometries */
	cw = cols ? (m->ww - 2*ov - iv*(cols - 1)) / cols : m->ww - 2*ov;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for (i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if (i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = rows ? (m->wh - 2*oh - ih*(rows - 1)) / rows : m->wh - 2*oh;
		cx = m->wx + ov + cn*(cw + iv);
		cy = m->wy + oh + rn*(ch + ih);
		resize(c, cx, cy, cw - 2*c->bw, ch - 2*c->bw, False);
		rn++;
		if (rn >= rows) {
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
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

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
		resize(c, cx, cy, cw - 2*c->bw, ch - 2*c->bw, False);
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
	float mfacts, sfacts;

	/* Count windows */
	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	if (n == 1) { /* Just fill the whole screen */
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
	float mfacts, sfacts;
	Client *c;

	/* count clients */
	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

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

		resize(c, cx, cy, cw - (2*c->bw), ch - (2*c->bw), 0);
	}
}

/*
 * Default tile layout + gaps + cfacts
 */
static void
tile(Monitor *m)
{
	unsigned int i, n;
	int mx = 0, my = 0, mh = 0, mw = 0;
	int sx = 0, sy = 0, sh = 0, sw = 0;
	int oh, ov, ih, iv;
	float mfacts, sfacts;
	Client *c;

	getgaps(m, &oh, &ov, &ih, &iv, &n, &mfacts, &sfacts);

	if (n == 0)
		return;

	sx = mx = m->wx + ov;
	sy = my = m->wy + oh;
	sh = mh = m->wh - 2*oh - ih * (MIN(n, m->nmaster) - 1);
	sw = mw = m->ww - 2*ov;

	if (m->nmaster && n > m->nmaster) {
		sw = (mw - iv) * (1 - m->mfact);
		mw = (mw - iv) * m->mfact;
		sx = mx + mw + iv;
		sy = my;
		sh = m->wh - 2*oh - ih * (n - m->nmaster - 1);
	}

	for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			resize(c, mx, my, mw - (2*c->bw), mh * (c->cfact / mfacts) - (2*c->bw), 0);
			my += HEIGHT(c) + ih;
		} else {
			resize(c, sx, sy, sw - (2*c->bw), sh * (c->cfact / sfacts) - (2*c->bw), 0);
			sy += HEIGHT(c) + ih;
		}
}
