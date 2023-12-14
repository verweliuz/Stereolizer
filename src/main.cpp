#include <SDL2/SDL.h>
#include "colors.cpp"
#include "presets.cpp"

#define SDL_main() SDL_main(int argv, char** args)

using namespace std;

enum class Mode {
    MENU,
    VIEW,
    MOVING_DOT
};

enum class ActionType {
    CREATE,
    LINK
};

struct Action {
    ActionType type;
    char f, s;
    Pos3d pos;
};

bool init();
void start();
void events();
void render();
void exit();

const int fps = 60;
const int width = 640, height = 640;
const char *title = "Stereolizer | SDL2";

const double rotationSpeed = 1.0;
const double zMovingSpeed = 0.5;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

Mode mode = Mode::VIEW;
char movingDot = 0;

int modifiers;
vector<bool> mouse(4);
double mouseX = 0, mouseY = 0;

bool running;
double scale;
map<char, Dot> dots;
stack<Action> actions;

void setScale(float n) {
    SDL_RenderSetScale(renderer, n, n);
    scale = n;
}

void setColor(int rgba) {
    SDL_SetRenderDrawColor(renderer,
    (rgba >> 24) % 256, (rgba >> 16) % 256,
    (rgba >> 8) % 256, rgba % 256);
}

int main() {

    if(init())
        start();
    else return -1;

    return 0;
}

bool init() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize the SDL2 library\n";
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height, 0
    );

    if(!window)
    {
        cout << "Failed to create window\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    return true;
}

void start() {

    cube(dots);

    running = true;

    while(running) {

        auto start = chrono::system_clock::now();

        events();
        render();

        auto end = chrono::system_clock::now();

        double delta = chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 1e-3;

        SDL_Delay(1000.0 / fps - delta);

    }

    exit();
}

const double targetDistance = 15;

char emptyChar() {
    if('Z' - 'A' < dots.size())
        return 0;

    for(char i = 'A'; i <= 'Z'; i++) {
        if(dots.count(i)) continue;
        return i;
    }

    return 0;
}

double distToDot(char id) {
    if(!dots.count(id)) return 0;

    Pos2d pos = toScreen(dots[id].pos);

    return p2(pos.x - mouseX + width / 2)
         + p2(pos.y - mouseY + width / 2);
}

char findTarget() {
    char closest = 0;
    
    for(auto x : dots) {
        if(x.first == movingDot) continue;
        double dist = distToDot(x.first);
        if(dist < p2(targetDistance)) {
            if(!closest || dist < distToDot(closest)) {
                closest = x.first;
            }
        }
    }

    return closest;
}

void unlinkDots(char f, char s) {
    if(dots.count(f)) dots[f].to.erase(s);
    if(dots.count(s)) dots[s].to.erase(f);
}

void removeDot(char id) {
    if(!dots.count(id) || dots.size() == 1) return;

    for(auto x : dots[id].to) {
        dots[x].to.erase(id);
    }

    dots.erase(id);
}

char copyDot(char id) {
    if(!dots.count(id)) return 0;

    char empty = emptyChar();
    if(!empty) return 0;

    dots[empty] = {empty, dots[id].pos, {id}};
    actions.push({ActionType::CREATE, empty});

    dots[id].to.insert(empty);

    return empty;
}

void rotateDots(double dx, double dy) {
    if(!dots.size()) return;

    double pitch = -dx * rotationSpeed;
    double yaw = dy * rotationSpeed;

    for(auto &x : dots) {
        x.second.pos = aroundOrigin(
            x.second.pos, yaw, pitch
        );
    }

}

void moveDot(double x, double y) {
    if(!movingDot) return;

    double z = dots[movingDot].pos.z;
    double mul = (z + distToOrigin) / (distToScreen * boost);

    dots[movingDot].pos.x = (x - width / 2) * mul;
    dots[movingDot].pos.y = (y - height / 2) * mul;

}

void linkDots(char f, char s) {
    if(!dots.count(f) || !dots.count(s)) return;

    dots[f].to.insert(s);
    dots[s].to.insert(f);

    actions.push({ActionType::LINK, f, s});
}

void reverseAction() {
    if(!actions.size()) return;

    Action action = actions.top();
    actions.pop();
    if(action.type == ActionType::CREATE) {
        if(dots.count(action.f)) {
            removeDot(action.f);
        } else reverseAction();
    } else if(action.type == ActionType::LINK) {
        if(dots.count(action.f) && dots[action.f].to.count(action.s)) {
            unlinkDots(action.f, action.s);
        } else reverseAction();
    }

}

void events() {

    char target = findTarget();

    while(SDL_PollEvent(&event)) {

        if(event.type == SDL_QUIT) {
			running = false;
			break;
		} else if(event.type == SDL_MOUSEMOTION) {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        } else if(event.type == SDL_MOUSEBUTTONDOWN) {
            mouse[event.button.button] = true;
        } else if(event.type == SDL_MOUSEBUTTONUP) {
            mouse[event.button.button] = false;
        } else if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            modifiers = event.key.keysym.mod;
        }

        switch (mode) {
        
        case Mode::MENU: {
            /* code */
            break;
        }

        case Mode::VIEW: {
            switch (event.type) {
            
            case SDL_MOUSEBUTTONDOWN: {
                switch (event.button.button) {
                
                case SDL_BUTTON_LEFT: {
                    if(dots.count(target)) {
                        mode = Mode::MOVING_DOT;
                        movingDot = target;
                    }
                    break;
                }

                case SDL_BUTTON_MIDDLE: {
                    char copy = copyDot(target);
                    if(dots.count(copy)) {
                        mode = Mode::MOVING_DOT;
                        movingDot = copy;
                    }
                    break;
                }
                
                default: 
                    break;
                }

                break;
            }

            case SDL_MOUSEMOTION: {
                if(mouse[SDL_BUTTON_RIGHT] && !modifiers)
                    rotateDots(event.motion.xrel, event.motion.yrel);

                break;
            }

            case SDL_KEYDOWN: {
                if(!modifiers && (
                    event.key.keysym.sym == SDLK_DELETE ||
                    event.key.keysym.sym == SDLK_BACKSPACE
                )) removeDot(target);

                else if(modifiers & KMOD_CTRL && (event.key.keysym.sym == SDLK_z))
                    reverseAction();

                else if(!modifiers && (event.key.keysym.sym = SDLK_PLUS)) {
                    char copy = copyDot(target);
                    if(dots.count(copy)) {
                        mode = Mode::MOVING_DOT;
                        movingDot = copy;
                    }
                }

                break;
            }

            case SDL_MOUSEWHEEL: {
                if(event.wheel.y < 0) {
                    boost *= 0.9;
                } else {
                    boost *= 1.1;
                }

                break;
            }
            
            default:
                break;
            }
            break;
        }

        case Mode::MOVING_DOT: {
            switch (event.type) {
            
            case SDL_MOUSEBUTTONUP: {
                switch (event.button.button) {
                
                case SDL_BUTTON_LEFT: {
                    mode = Mode::VIEW;
                    movingDot = 0;
                    break;
                }
                
                default:
                    break;
                }

                break;
            }

            case SDL_MOUSEMOTION: {
                moveDot(event.motion.x, event.motion.y);

                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                if(target && dots[movingDot].to.size() == 1) {
                    linkDots(*dots[movingDot].to.begin(), target);
                    removeDot(movingDot);
                    mode = Mode::VIEW;
                    movingDot = 0;
                }

                break;
            }
            
            default:
                break;
            }
            break;
        }
        
        default:
            break;
        }

        if(mode == Mode::MENU) {

            switch (event.type) {
            
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                
                case SDL_BUTTON_LEFT:
                    if(target) {
                        mode = Mode::MOVING_DOT;
                        movingDot = target;
                    }
                    break;
                
                default:
                    break;
                }
                break;
            
            default:
                break;
            }

        }

    }

}

void renderLine(Line3d line) {

    Line2d onScreen = toScreen(line);

    setColor(black);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderDrawLineF(renderer,
        (onScreen.l.x + width / 2) / scale,
        (onScreen.l.y + height / 2) / scale,
        (onScreen.r.x + width / 2) / scale,
        (onScreen.r.y + height / 2) / scale
    );

}

bool edgeComparator(Edge a, Edge b) {
    return isCloser(
        {dots[a.l].pos, dots[a.r].pos},
        {dots[b.l].pos, dots[b.r].pos}
    );
}

void renderLines() {

    map<char, bool> visited;

    vector<Edge> edges;
    for(auto x : dots) {
        char id = x.first;
        Dot dot = x.second;
        visited[id] = true;
        for(auto y : dot.to) {
            if(visited[y]) continue;
            edges.push_back({id, y});
        }
    }
    for(int i = 0; i < edges.size(); i++) {
        for(int j = i + 1; j < edges.size(); j++) {
            if(edgeComparator(edges[j - 1], edges[j])) {
                swap(edges[j - 1], edges[j]);
            }
        }
    }

    for(auto x : edges) {
        renderLine({
            dots[x.l].pos,
            dots[x.r].pos
        });
    }

}

void renderCircle(int x, int y, double radius) {
    // radius *= boost / initBoost;
    for(int w = 0; w < radius * 2; w++) {
        for(int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if(p2(dx) + p2(dy) <= p2(radius)) {
                SDL_RenderDrawPoint(renderer,
                    (x + dx + width / 2) / scale,
                    (y + dy + height / 2) / scale
                );
            }
        }
    }
}

const double dotSize = 20;

void renderDot(char id) {

    Dot dot = dots[id];
    Pos2d onScreen = toScreen(dot.pos);

    setColor((id == movingDot || id == findTarget()) ? red : black);
    renderCircle(onScreen.x, onScreen.y, dotSize / (dot.pos.z + distToOrigin));

    setColor(black);

}

void renderDots() {

    for(auto x : dots) {
        renderDot(x.first);
    }

}

const int gridWidth = 30, gridHeight = 30;

void renderBackground() {

    setColor(lightGray);

    for(int i = 1; i < gridWidth; i++) {
        double step = width * 1.0 / gridWidth;
        SDL_RenderDrawLineF(renderer,
                            (i * step) / scale,
                            (0) / scale,
                            (i * step) / scale,
                            (width) / scale
        );
    }

    for(int i = 1; i < gridHeight; i++) {
        double step = height * 1.0 / gridHeight;
        SDL_RenderDrawLineF(renderer,
                            (0) / scale,
                            (i * step) / scale,
                            (width) / scale,
                            (i * step) / scale
        );
    }

}

void render() {

    setScale(1);
    setColor(white);
    SDL_RenderClear(renderer);

    renderBackground();

    if(dots.size()) {
        renderLines();
        renderDots();
    }

    SDL_RenderPresent(renderer);

}

void exit() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

