#include <bits/stdc++.h>

using namespace std;

struct Pos2d {
    double x, y;
};

struct Pos3d {
    double x, y, z;
};

struct Line2d {
    Pos2d l, r;
};

struct Line3d {
    Pos3d l, r;
};

struct Edge {
    char l, r;
};

struct Dot {
    char id;
    Pos3d pos;
    set<char> to;
};

double p2(double n) {
    return n * n;
}

double pi() {
	return acos(0) * 2;
}

double degrees(double radians) {
	return radians * (180 / pi());
}

double radians(double degrees) {
	return degrees * (pi() / 180);
}

Pos2d deltas(Line2d line) {
    return {
        line.r.x - line.l.x,
        line.r.y - line.l.y
    };
}

Pos3d deltas(Line3d line) {
    return {
        line.r.x - line.l.x,
        line.r.y - line.l.y,
        line.r.z - line.l.z
    };
}

double crossing(Line2d f, Line2d s) {
    Pos2d fDelta = deltas(f);
    Pos2d sDelta = deltas(s);
    double fStart = f.l.x;
    double sStart = s.l.x;
    double fSpeed = fDelta.y / fDelta.x;
    double sSpeed = sDelta.y / sDelta.x;
    return (fStart - sStart) / (sSpeed - fSpeed);
}

const double distToScreen = 3;
const double distToOrigin = 6;

double boost = 250;

Pos2d toScreen(Pos3d pos) {
    return {
        pos.x * distToScreen / (pos.z + distToOrigin) * boost,
        pos.y * distToScreen / (pos.z + distToOrigin) * boost
    };
}

Line2d toScreen(Line3d line) {
    return {
        toScreen(line.l),
        toScreen(line.r)
    };
}

bool isCloser(Line3d f, Line3d s) {
    Pos3d fDelta = deltas(f);
    Pos3d sDelta = deltas(s);
    Line2d fScreen = toScreen(f);
    Line2d sScreen = toScreen(s);
    Pos2d fScreenDelta = deltas(fScreen);
    Pos2d sScreenDelta = deltas(sScreen);
    double cross = crossing(fScreen, sScreen);
    if(max(fScreen.l.x, sScreen.l.x) <= cross
    || min(fScreen.r.x, sScreen.r.x) >= cross)
        return false;
    double fZ = f.l.z + fDelta.z * ((cross - fScreen.l.x) / fScreenDelta.x);
    double sZ = s.l.z + sDelta.z * ((cross - sScreen.l.x) / sScreenDelta.x);
    return fZ < sZ;
}

Pos3d rotateYaw(Pos3d pos, double yaw) {
    yaw = radians(yaw);

    double yz = sqrt(p2(pos.y) + p2(pos.z));

    double initYaw = acos(pos.y / yz);
    if(pos.z < 0) initYaw = -initYaw;

    pos.z = yz * sin(initYaw + yaw);
	pos.y = yz * cos(initYaw + yaw);

	return pos;
}

Pos3d rotatePitch(Pos3d pos, double pitch) {
    pitch = radians(pitch);

	double xz = sqrt(p2(pos.x) + p2(pos.z));

	double initPitch = acos(pos.z / xz);
	if(pos.x < 0) initPitch = -initPitch;

	pos.x = xz * sin(initPitch + pitch);
	pos.z = xz * cos(initPitch + pitch);

	return pos;
}

Pos3d aroundOrigin(Pos3d pos, double yaw, double pitch) {
    return rotatePitch(rotateYaw(pos, yaw), pitch);
}

