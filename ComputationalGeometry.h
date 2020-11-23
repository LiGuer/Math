#ifndef COMPUTATIONAL_GEOMETRY_H
#define COMPUTATIONAL_GEOMETRY_H
struct position { double x, y; };
struct triangle { position p[3]; };
struct segment { point p[2]; };
/*--------------------------------[ CircumCircle 三角形外接圆 ]--------------------------------*/
doubel CircumCircle(triangle& tri, pposition& center)
{
	double x[3], y[3];
	for (int i = 0; i < 3; i++) {
		x[i] = tri.p[i].x;
		y[i] = tri.p[i].y;
	}
	//三角形外接圆圆心
	double t[3];
	for (int i = 0; i < 3; i++) {
		t[i] = x[i] * x[i] + y[i] * y[i];
	}
	double temp = (x[2] - x[1]) * (y[1] - y[0])
		- (x[1] - x[0]) * (y[2] - y[1]);
	center.x = (-(t[1] - t[0]) * (y[2] - y[1])
		+ (t[2] - t[1]) * (y[1] - y[0])) / (2 * temp);
	center.y = (+(t[1] - t[0]) * (x[2] - x[1])
		- (t[2] - t[1]) * (x[1] - x[0])) / (2 * temp);
	//三角形外接圆半径
	double a = sqrt(pow((x[0] - x[1]), 2) + pow((y[0] - y[1]), 2));
	double b = sqrt(pow((x[0] - x[2]), 2) + pow((y[0] - y[2]), 2));
	double c = sqrt(pow((x[1] - x[2]), 2) + pow((y[1] - y[2]), 2));
	double p = (a + b + c) / 2;
	double S = sqrt(p * (p - a) * (p - b) * (p - c));
	double radius = a * b * c / (4 * S);
	return radius;
}
/*--------------------------------[ segment 线段 ]--------------------------------*/
int CrossProduct(point origin, point a, point b)	//叉乘
{
	a.x -= origin.x; a.y -= origin.y;
	b.x -= origin.x; b.y -= origin.y;
	double ans = a.x * b.y - a.y * b.x;
	if (ans > 0)return 1;
	else if (ans < 0)return -1;
	return 0;
}
bool OnSegments_judge(segment seg, point point)
{
	seg.p[0].x += -point.x + seg.p[0].y - point.y;
	seg.p[1].x += -point.x + seg.p[1].y - point.y;
	if (seg.p[0].x * seg.p[1].x <= 0)return true;
	return false;
}
bool SegmentsIntersect_judge(segment a, segment b)
{
	int dir_a1 = CrossProduct(a.p[0], a.p[1], b.p[0]);
	int dir_a2 = CrossProduct(a.p[0], a.p[1], b.p[1]);
	int dir_b1 = CrossProduct(b.p[0], b.p[1], a.p[0]);
	int dir_b2 = CrossProduct(b.p[0], b.p[1], a.p[1]);
	if (dir_a1 == 0)
		if (OnSegments_judge(a, b.p[0])) return true; else {}
	else if (dir_a2 == 0)
		if (OnSegments_judge(a, b.p[1])) return true; else {}
	else if (dir_b1 == 0)
		if (OnSegments_judge(b, a.p[0])) return true; else {}
	else if (dir_b2 == 0)
		if (OnSegments_judge(b, a.p[1])) return true; else {}
	else if (dir_a1 != dir_a2 && dir_b1 != dir_b2) return true;
	return false;
}
/*--------------------------------[ ConvexHull 凸包 ]--------------------------------*/
struct position {
	double x, y, angle = 0;
};
stack<position> ansPoint;
bool cmd(position a, position b) { return a.angle < b.angle; }

position findCur(position arr[], int N)
{
	position mincur = { 99999 ,99999 ,-1 };
	for (int i = 1; i < N; i++) {
		if (arr[i].y < mincur.y || (arr[i].y == mincur.y
			&& arr[i].x < mincur.x)) {
			mincur.angle = i;
			mincur.y = arr[i].y;
			mincur.x = arr[i].x;
		}
	}return mincur;
}
void setAngle(position arr[], int N, int cur)
{
	for (int i = 0; i < N; i++) {
		double b = sqrt(pow(arr[i].x - arr[cur].x, 2) +
			pow(arr[i].y + arr[cur].y, 2));
		double c = arr[i].x - arr[cur].x;
		arr[i].angle = c / b;
	}
}
bool CrossProduct(position a, position b)
{
	double crossproduct = a.x * b.y - a.y * b.x;
	if (crossproduct >= 0)return false;
	else true;
}
void ConvexHull(position arr[], int N, int cur)
{
	for (int i = 0; i < N; i++) {
		if (i == cur)continue;
		while (!ansPoint.empty()) {
			position prePoint = ansPoint.top();
			ansPoint.pop();
			if (ansPoint.empty()) {
				ansPoint.push(prePoint);
				ansPoint.push(arr[i]);
				break;
			}
			position a = { prePoint.x - ansPoint.top().x,
						   prePoint.y - ansPoint.top().y };
			position b = { arr[i].x - prePoint.x,arr[i].y - prePoint.y };
			if (CrossProduct(a, b)) {
				ansPoint.push(prePoint);
				ansPoint.push(arr[i]);
				break;
			}
			else {}
		}
		if (ansPoint.empty())ansPoint.push(arr[i]);
	}ansPoint.push(arr[cur]);
}
/*--------------------------------[ Delaunay 三角剖分 ]--------------------------------*/
struct position {
	double x, y;
	bool friend operator<(position a, position b) {
		if (a.x != b.x)return a.x < b.x;
		return a.y < b.y;
	}
	bool friend operator==(position a, position b) {
		if (a.x == b.x && a.y == b.y)return true;
		return false;
	}
};
struct edge {
	position a, b;
	bool friend operator==(edge e1, edge e2) {
		if ((e1.a == e2.a && e1.b == e2.b) ||
			(e1.a == e2.b && e1.b == e2.a))return true;
		return false;
	}
	bool friend operator<(edge e1, edge e2) {
		position mine1 = e1.a < e1.b ? e1.a : e1.b;
		position mine2 = e2.a < e2.b ? e2.a : e2.b;
		if (mine1 < mine2)return true;
		if (mine2 < mine1)return false;
		position maxe1 = e1.a < e1.b ? e1.b : e1.a;
		position maxe2 = e2.a < e2.b ? e2.b : e2.a;
		if (maxe1 < maxe2)return true;
		if (maxe2 < maxe1)return false;
	}
};
struct triangle { position a, b, c; };
triangle trians[10 * MAXN + 1], tritemp[10 * MAXN + 1];
int trianspos = 0, tritemppos = 0;

triangle SuperTriangle(position arr[], int N)
{
	position mincur = { arr[0].x ,99999 },
		maxcur = { arr[N - 1].x ,-99999 };
	for (int i = 1; i < N; i++) {
		if (arr[i].y < mincur.y) mincur.y = arr[i].y;
		if (arr[i].y > maxcur.y) maxcur.y = arr[i].y;
	}
	position length = { maxcur.x - mincur.x , maxcur.y - mincur.y };
	double mid_x = (maxcur.x + mincur.x) / 2;
	position a = { mincur.x - length.x - 2,mincur.y - 2 },
		b = { maxcur.x + length.x + 2,mincur.y - 2 },
		c = { mid_x ,maxcur.y + length.y + 2 };
	tritemp[tritemppos++] = { a,b,c };
	triangle supertriangle = { a,b,c };
	return supertriangle;
}
int judge(triangle tri, position point)
{
	double x[3] = { tri.a.x, tri.b.x, tri.c.x };
	double y[3] = { tri.a.y, tri.b.y, tri.c.y };
	double a = sqrt(pow((x[0] - x[1]), 2) + pow((y[0] - y[1]), 2));
	double b = sqrt(pow((x[0] - x[2]), 2) + pow((y[0] - y[2]), 2));
	double c = sqrt(pow((x[1] - x[2]), 2) + pow((y[1] - y[2]), 2));
	double p = (a + b + c) / 2;
	double S = sqrt(p * (p - a) * (p - b) * (p - c));
	double radius = a * b * c / (4 * S);

	position center;
	double t[3];
	for (int i = 0; i < 3; i++) {
		t[i] = x[i] * x[i] + y[i] * y[i];
	}
	double temp = (x[2] - x[1]) * (y[1] - y[0]) - (x[1] - x[0]) * (y[2] - y[1]);
	center.x = (-(t[1] - t[0]) * (y[2] - y[1]) + (t[2] - t[1]) * (y[1] - y[0])) / (2 * temp);
	center.y = (+(t[1] - t[0]) * (x[2] - x[1]) - (t[2] - t[1]) * (x[1] - x[0])) / (2 * temp);
	if (point.x > center.x + radius)return 1;
	double s = sqrt(pow((point.x - center.x), 2) + pow((point.y - center.y), 2));
	if (s < radius)return 0;
	return -1;
}
void deletearr(triangle arr[], int cur, int N)
{
	for (int i = cur + 1; i < N; i++)
		arr[i - 1] = arr[i];
}
bool supertriangle_judge(triangle t1, triangle t2)
{
	position p[6] = { t1.a,t1.b,t1.c,t2.a,t2.b,t2.c };
	sort(p, p + 6);
	for (int i = 0; i < 5; i++)
		if (p[i] == p[i + 1])
			return true;
	return false;
}
vector<edge> edgetemp;
void Delaunay(position p)
{
	edgetemp.clear();
	for (int i = 0; i < tritemppos; i++) {
		if (judge(tritemp[i], p) == 1) {
			trians[trianspos++] = tritemp[i];
			deletearr(tritemp, i, tritemppos);
			tritemppos--; i--;
		}
		else if (judge(tritemp[i], p) == 0) {
			edge temp[3] = { { tritemp[i].a, tritemp[i].b } ,
							 { tritemp[i].a, tritemp[i].c } ,
							 { tritemp[i].b, tritemp[i].c } };
			for (int j = 0; j < 3; j++) edgetemp.push_back(temp[j]);
			deletearr(tritemp, i, tritemppos);
			tritemppos--; i--;
		}
		else {}
	}
	sort(edgetemp.begin(), edgetemp.end());
	for (int i = 0; i < edgetemp.size() - 1; i++) {
		int flag = 0;
		while (i + 1 < edgetemp.size() && edgetemp[i] == edgetemp[i + 1]) {
			flag = 1;
			edgetemp.erase(edgetemp.begin() + i + 1);
		}
		if (flag) {
			edgetemp.erase(edgetemp.begin() + i); i--;
		}
	}
	for (int i = 0; i < edgetemp.size(); i++)
		tritemp[tritemppos++] = { edgetemp[i].a, edgetemp[i].b, p };
}

#endif