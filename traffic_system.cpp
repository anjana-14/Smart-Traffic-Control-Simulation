#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace sf;
using namespace std;

// Traffic light states
enum TrafficState {
    NS_GREEN,
    NS_YELLOW,
    EW_GREEN,
    EW_YELLOW
};

// Directions
enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NUM_DIRECTIONS
};

// Vehicle types
enum VehicleType {
    CAR,
    TRUCK,
    BUS,
    MOTORCYCLE,
    NUM_VEHICLE_TYPES
};

// Layout types
enum LayoutType {
    STANDARD_4WAY,
    T_INTERSECTION,
    ROUNDABOUT,
    NUM_LAYOUTS
};

// Traffic light colors
const Color RED(220, 20, 60);
const Color YELLOW(255, 215, 0);
const Color GREEN(50, 205, 50);
const Color OFF(40, 40, 40);
const Color BACKGROUND(30, 30, 50);
const Color PANEL(45, 45, 70);
const Color ROAD(60, 60, 80);
const Color LANE_MARKING(180, 180, 200);
const Color TEXT_COLOR(220, 220, 250);
const Color VEHICLE_COLOR(70, 130, 180);
const Color TRUCK_COLOR(139, 69, 19);
const Color BUS_COLOR(255, 165, 0);
const Color MOTORCYCLE_COLOR(50, 205, 50);
const Color HIGHLIGHT(0, 191, 255);

// System class
class TrafficSystem {
public:
    struct Vehicle {
        VehicleType type;
        float position;
        Vehicle(VehicleType t = CAR, float pos = 0) : type(t), position(pos) {}
    };

    TrafficSystem() {
        reset();
        current_layout = STANDARD_4WAY;
        show_config_menu = false;
    }

    void reset() {
        current_state = NS_GREEN;
        timer = 30;
        original_green_time = 30;  // Initialize original green time
        adaptive_mode = true;
        simulation_speed = 1.0f;
        auto_vehicles = true;
        emergency_mode = false;
        emergency_timer = 0;
        stats_vehicles_passed = 0;
        stats_pedestrians_crossed = 0;
        start_time = time(nullptr);
        total_time_clock.restart(); // Restart the high precision timer
        for (int i = 0; i < NUM_DIRECTIONS; i++) {
            walk_signals[i] = false;
            requests[i] = false;
            vehicle_counts[i] = 0;
            vehicles[i].clear();
        }
    }

    void update() {
        if (emergency_mode) {
            emergency_timer--;
            if (emergency_timer <= 0) {
                emergency_mode = false;
                timer = 5;
            }
            return;
        }

        timer--;

        if (timer <= 0) {
            switch (current_state) {
                case NS_GREEN:
                    current_state = NS_YELLOW;
                    timer = 5;
                    reset_walk_signals(true);
                    break;
                case NS_YELLOW:
                    current_state = EW_GREEN;
                    original_green_time = calculate_green_time(false);
                    timer = original_green_time;
                    stats_vehicles_passed += vehicle_counts[EAST] + vehicle_counts[WEST];
                    vehicles[EAST].clear();
                    vehicles[WEST].clear();
                    vehicle_counts[EAST] = 0;
                    vehicle_counts[WEST] = 0;
                    break;
                case EW_GREEN:
                    current_state = EW_YELLOW;
                    timer = 5;
                    reset_walk_signals(false);
                    break;
                case EW_YELLOW:
                    current_state = NS_GREEN;
                    original_green_time = calculate_green_time(true);
                    timer = original_green_time;
                    stats_vehicles_passed += vehicle_counts[NORTH] + vehicle_counts[SOUTH];
                    vehicles[NORTH].clear();
                    vehicles[SOUTH].clear();
                    vehicle_counts[NORTH] = 0;
                    vehicle_counts[SOUTH] = 0;
                    break;
            }
        }

                // Process pedestrian requests
        if (current_state == NS_GREEN && timer == original_green_time) {
            if (requests[NORTH] || requests[SOUTH]) {
                walk_signals[NORTH] = requests[NORTH];
                walk_signals[SOUTH] = requests[SOUTH];
                requests[NORTH] = false;
                requests[SOUTH] = false;
                stats_pedestrians_crossed++;
            }
        }
        else if (current_state == EW_GREEN && timer == original_green_time) {
            if (requests[EAST] || requests[WEST]) {
                walk_signals[EAST] = requests[EAST];
                walk_signals[WEST] = requests[WEST];
                requests[EAST] = false;
                requests[WEST] = false;
                stats_pedestrians_crossed++;
            }
        }

        // Add random vehicles
        if (auto_vehicles && rand() % 5 == 0) {
            Direction dir = static_cast<Direction>(rand() % NUM_DIRECTIONS);
            add_vehicle(dir);
        }
    }

    void request_crossing(Direction dir) {
        requests[dir] = true;
    }

    void add_vehicle(Direction dir) {
        VehicleType type = static_cast<VehicleType>(rand() % NUM_VEHICLE_TYPES);
        vehicles[dir].push_back(Vehicle(type, 0));
        vehicle_counts[dir]++;
    }

    void toggle_adaptive_mode() {
        adaptive_mode = !adaptive_mode;
    }

    void toggle_auto_vehicles() {
        auto_vehicles = !auto_vehicles;
    }

    void speed_up() {
        simulation_speed = min(5.0f, simulation_speed + 0.5f);
    }

    void slow_down() {
        simulation_speed = max(0.5f, simulation_speed - 0.5f);
    }

    void trigger_emergency() {
        emergency_mode = true;
        emergency_timer = 15;
    }

    int get_timer() const { return timer; }
    TrafficState get_state() const { return current_state; }
    bool get_walk_signal(Direction dir) const { return walk_signals[dir]; }
    bool get_request(Direction dir) const { return requests[dir]; }
    int get_vehicle_count(Direction dir) const { return vehicle_counts[dir]; }
    bool is_adaptive() const { return adaptive_mode; }
    bool is_auto_vehicles() const { return auto_vehicles; }
    float get_speed() const { return simulation_speed; }
    bool is_emergency() const { return emergency_mode; }
    int get_vehicles_passed() const { return stats_vehicles_passed; }
    int get_pedestrians_crossed() const { return stats_pedestrians_crossed; }

    time_t get_start_time() const { return start_time; }
    int get_elapsed_seconds() const { 
        return static_cast<int>(total_time_clock.getElapsedTime().asSeconds()); 
    }
    
    void toggle_config_menu() { show_config_menu = !show_config_menu; }
    bool is_config_menu_open() const { return show_config_menu; }
    LayoutType get_layout() const { return current_layout; }
    void set_layout(LayoutType layout) { current_layout = layout; }
    
    const std::vector<Vehicle>& get_vehicles(Direction dir) const { return vehicles[dir]; }

private:
    int calculate_green_time(bool is_ns) {
        if (!adaptive_mode) return 30;

        int main_traffic = 0;
        int cross_traffic = 0;

        if (is_ns) {
            main_traffic = vehicle_counts[NORTH] + vehicle_counts[SOUTH];
            cross_traffic = vehicle_counts[EAST] + vehicle_counts[WEST];
        } else {
            main_traffic = vehicle_counts[EAST] + vehicle_counts[WEST];
            cross_traffic = vehicle_counts[NORTH] + vehicle_counts[SOUTH];
        }

        int traffic_diff = max(0, main_traffic - cross_traffic);
        int extra_time = min(40, traffic_diff * 2);
        return 20 + extra_time;
    }

    void reset_walk_signals(bool reset_ns) {
        if (reset_ns) {
            walk_signals[NORTH] = false;
            walk_signals[SOUTH] = false;
        } else {
            walk_signals[EAST] = false;
            walk_signals[WEST] = false;
        }
    }

    TrafficState current_state;
    int timer;
    int original_green_time;  // Store the original green time to prevent glitches
    bool walk_signals[NUM_DIRECTIONS];
    bool requests[NUM_DIRECTIONS];
    int vehicle_counts[NUM_DIRECTIONS];
    std::vector<Vehicle> vehicles[NUM_DIRECTIONS];
    bool adaptive_mode;
    float simulation_speed;
    bool auto_vehicles;
    bool emergency_mode;
    int emergency_timer;
    int stats_vehicles_passed;
    int stats_pedestrians_crossed;
    time_t start_time;
    sf::Clock total_time_clock; // High precision timer for total elapsed time
    
    // New features
    LayoutType current_layout;
    bool show_config_menu;
};

// Format time as MM:SS
string format_time(time_t seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    stringstream ss;
    ss << setfill('0') << setw(2) << minutes << ":" << setw(2) << secs;
    return ss.str();
}

int main() {
    srand(time(nullptr));

    // Create window with fixed size
    RenderWindow window(VideoMode(1200, 980), "Smart Traffic Control System", Style::Close | Style::Titlebar);
    
    // Verify window creation
    if (!window.isOpen()) {
        cout << "ERROR: Failed to create window!" << endl;
        return -1;
    }
    
    window.setFramerateLimit(60);
    
    // Enable vertical sync if possible
    window.setVerticalSyncEnabled(true);

    // Load fonts - try multiple font paths
    Font font;
    bool fontLoaded = false;
    vector<string> fontPaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Arial.ttf",
        "arial.ttf"
    };
    
    for (const string& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            cout << "Font loaded from: " << path << endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        cout << "WARNING: No fonts found, text may not display properly" << endl;
        // Try to load the default font
        if (!font.loadFromFile("")) {
            cout << "Using system default font" << endl;
        }
    }

    // Create traffic system
    TrafficSystem trafficSystem;

    // Create clock for simulation updates
    Clock simulationClock;
    float simulationAccumulator = 0.0f;

    // Main loop
    while (window.isOpen()) {
        // Handle events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::N) trafficSystem.request_crossing(NORTH);
                else if (event.key.code == Keyboard::S) trafficSystem.request_crossing(SOUTH);
                else if (event.key.code == Keyboard::E) trafficSystem.request_crossing(EAST);
                else if (event.key.code == Keyboard::W) trafficSystem.request_crossing(WEST);
                else if (event.key.code == Keyboard::Num1) trafficSystem.add_vehicle(NORTH);
                else if (event.key.code == Keyboard::Num2) trafficSystem.add_vehicle(SOUTH);
                else if (event.key.code == Keyboard::Num3) trafficSystem.add_vehicle(EAST);
                else if (event.key.code == Keyboard::Num4) trafficSystem.add_vehicle(WEST);
                else if (event.key.code == Keyboard::T) trafficSystem.toggle_adaptive_mode();
                else if (event.key.code == Keyboard::A) trafficSystem.toggle_auto_vehicles();
                else if (event.key.code == Keyboard::Add) trafficSystem.speed_up();
                else if (event.key.code == Keyboard::Subtract) trafficSystem.slow_down();
                else if (event.key.code == Keyboard::M) trafficSystem.trigger_emergency();
                else if (event.key.code == Keyboard::C) trafficSystem.toggle_config_menu();
                else if (event.key.code == Keyboard::R) trafficSystem.reset();
                else if (event.key.code == Keyboard::Q) window.close();
            }
        }

        // Update simulation at the appropriate speed
        float deltaTime = simulationClock.restart().asSeconds();
        simulationAccumulator += deltaTime * trafficSystem.get_speed();
        while (simulationAccumulator >= 1.0f) {
            trafficSystem.update();
            simulationAccumulator -= 1.0f;
        }

        // Draw everything
        window.clear(BACKGROUND);
        
        // Use fixed window size (no need for test background with fixed size)
        Vector2u winSize = window.getSize();
        float scaleX = 1.0f; // Fixed scaling since we're using original 1200x980 size
        float scaleY = 1.0f;

        // Draw title
        Text title("SMART TRAFFIC CONTROL SYSTEM", font, (unsigned int)(32 * min(scaleX, scaleY)));
        title.setFillColor(HIGHLIGHT);
        title.setStyle(Text::Bold);
        title.setPosition(winSize.x/2 - title.getLocalBounds().width/2, 20);
        if (fontLoaded) {
            window.draw(title);
        } else {
            // Fallback: draw a simple rectangle if font fails
            RectangleShape titleBar(Vector2f(winSize.x * 0.8f, 40));
            titleBar.setFillColor(HIGHLIGHT);
            titleBar.setPosition(winSize.x * 0.1f, 20);
            window.draw(titleBar);
        }

        // Draw main panel with fixed size
        RectangleShape mainPanel(Vector2f(1100, 700));
        mainPanel.setFillColor(PANEL);
        mainPanel.setPosition(50, 100);
        mainPanel.setOutlineThickness(2);
        mainPanel.setOutlineColor(Color(80, 80, 120));
        window.draw(mainPanel);

        // Draw intersection - center it in the window
        int centerX = 600; // Fixed center for 1200px width
        int centerY = 400; // Fixed center position

        // Draw roads - fixed dimensions
        float roadWidth = 800; // Fixed road width
        float roadHeight = 160; // Fixed road height
        float vRoadWidth = 160; // Fixed vertical road width  
        float vRoadHeight = 600; // Fixed vertical road height
        
        RectangleShape hRoad(Vector2f(roadWidth, roadHeight));
        hRoad.setFillColor(ROAD);
        hRoad.setPosition(centerX - roadWidth/2, centerY - roadHeight/2);
        window.draw(hRoad);

        RectangleShape vRoad(Vector2f(vRoadWidth, vRoadHeight));
        vRoad.setFillColor(ROAD);
        vRoad.setPosition(centerX - vRoadWidth/2, centerY - vRoadHeight/2);
        window.draw(vRoad);

        // Draw lane markings - scale with roads
        int hMarkCount = (int)(roadWidth / 45);
        for (int i = 0; i < hMarkCount; i++) {
            RectangleShape mark1(Vector2f(30 * scaleX, 3 * scaleY));
            mark1.setFillColor(LANE_MARKING);
            mark1.setPosition(centerX - roadWidth/2 + 20 + i * (roadWidth / hMarkCount), centerY - 2);
            window.draw(mark1);
        }

        int vMarkCount = (int)(vRoadHeight / 40);
        for (int i = 0; i < vMarkCount; i++) {
            RectangleShape mark2(Vector2f(3 * scaleX, 30 * scaleY));
            mark2.setFillColor(LANE_MARKING);
            mark2.setPosition(centerX - 2, centerY - vRoadHeight/2 + 20 + i * (vRoadHeight / vMarkCount));
            window.draw(mark2);
        }

        // Draw traffic lights - fix positioning so circles stay within boxes
        // North light - positioned at north edge of intersection
        RectangleShape northLight(Vector2f(30 * scaleX, 100 * scaleY));
        northLight.setFillColor(Color(50, 50, 50));
        northLight.setPosition(centerX - (int)(15 * scaleX), centerY - (int)(160 * scaleY));
        window.draw(northLight);

        CircleShape northRed((int)(12 * min(scaleX, scaleY)));
        northRed.setFillColor(trafficSystem.get_state() == NS_GREEN || trafficSystem.get_state() == NS_YELLOW ? OFF : RED);
        northRed.setPosition(centerX - (int)(12 * min(scaleX, scaleY)), centerY - (int)(155 * scaleY));
        window.draw(northRed);

        CircleShape northYellow((int)(12 * min(scaleX, scaleY)));
        northYellow.setFillColor(trafficSystem.get_state() == NS_YELLOW ? YELLOW : OFF);
        northYellow.setPosition(centerX - (int)(12 * min(scaleX, scaleY)), centerY - (int)(125 * scaleY));
        window.draw(northYellow);

        CircleShape northGreen((int)(12 * min(scaleX, scaleY)));
        northGreen.setFillColor(trafficSystem.get_state() == NS_GREEN ? GREEN : OFF);
        northGreen.setPosition(centerX - (int)(12 * min(scaleX, scaleY)), centerY - (int)(95 * scaleY));
        window.draw(northGreen);

        Text northLabel("NORTH", font, (unsigned int)(12 * min(scaleX, scaleY)));
        northLabel.setFillColor(TEXT_COLOR);
        northLabel.setPosition(centerX - (int)(20 * scaleX), centerY - (int)(50 * scaleY));
        window.draw(northLabel);

        // South light - fix positioning and size
        RectangleShape southLight(Vector2f(30 * scaleX, 100 * scaleY));
        southLight.setFillColor(Color(50, 50, 50));
        southLight.setPosition(centerX - (int)(15 * scaleX), centerY + (int)(60 * scaleY));
        window.draw(southLight);

        CircleShape southRed((int)(12 * min(scaleX, scaleY)));
        southRed.setFillColor(trafficSystem.get_state() == NS_GREEN || trafficSystem.get_state() == NS_YELLOW ? OFF : RED);
        southRed.setPosition(centerX - (int)(12 * min(scaleX, scaleY)), centerY + (int)(65 * scaleY));
        window.draw(southRed);

        CircleShape southYellow((int)(12 * min(scaleX, scaleY)));
        southYellow.setFillColor(trafficSystem.get_state() == NS_YELLOW ? YELLOW : OFF);
        southYellow.setPosition(centerX - (int)(12 * min(scaleX, scaleY)), centerY + (int)(95 * scaleY));
        window.draw(southYellow);

        CircleShape southGreen((int)(12 * min(scaleX, scaleY)));
        southGreen.setFillColor(trafficSystem.get_state() == NS_GREEN ? GREEN : OFF);
        southGreen.setPosition(centerX - (int)(12 * min(scaleX, scaleY)), centerY + (int)(125 * scaleY));
        window.draw(southGreen);

        Text southLabel("SOUTH", font, (unsigned int)(12 * min(scaleX, scaleY)));
        southLabel.setFillColor(TEXT_COLOR);
        southLabel.setPosition(centerX - (int)(20 * scaleX), centerY + (int)(170 * scaleY));
        window.draw(southLabel);

                // East light - fix positioning and size
        RectangleShape eastLight(Vector2f(100 * scaleX, 30 * scaleY));
        eastLight.setFillColor(Color(50, 50, 50));
        eastLight.setPosition(centerX + (int)(60 * scaleX), centerY - (int)(15 * scaleY));
        window.draw(eastLight);

        CircleShape eastRed((int)(12 * min(scaleX, scaleY)));
        eastRed.setFillColor(trafficSystem.get_state() == EW_GREEN || trafficSystem.get_state() == EW_YELLOW ? OFF : RED);
        eastRed.setPosition(centerX + (int)(65 * scaleX), centerY - (int)(12 * scaleY));
        window.draw(eastRed);

        CircleShape eastYellow((int)(12 * min(scaleX, scaleY)));
        eastYellow.setFillColor(trafficSystem.get_state() == EW_YELLOW ? YELLOW : OFF);
        eastYellow.setPosition(centerX + (int)(95 * scaleX), centerY - (int)(12 * scaleY));
        window.draw(eastYellow);

        CircleShape eastGreen((int)(12 * min(scaleX, scaleY)));
        eastGreen.setFillColor(trafficSystem.get_state() == EW_GREEN ? GREEN : OFF);
        eastGreen.setPosition(centerX + (int)(125 * scaleX), centerY - (int)(12 * scaleY));
        window.draw(eastGreen);

        Text eastLabel("EAST", font, (unsigned int)(12 * min(scaleX, scaleY)));
        eastLabel.setFillColor(TEXT_COLOR);
        eastLabel.setPosition(centerX + (int)(170 * scaleX), centerY - (int)(10 * scaleY));
        window.draw(eastLabel);

        // West light - positioned at west edge of intersection
        RectangleShape westLight(Vector2f(100 * scaleX, 30 * scaleY));
        westLight.setFillColor(Color(50, 50, 50));
        westLight.setPosition(centerX - (int)(160 * scaleX), centerY - (int)(15 * scaleY));
        window.draw(westLight);

        CircleShape westRed((int)(12 * min(scaleX, scaleY)));
        westRed.setFillColor(trafficSystem.get_state() == EW_GREEN || trafficSystem.get_state() == EW_YELLOW ? OFF : RED);
        westRed.setPosition(centerX - (int)(155 * scaleX), centerY - (int)(12 * scaleY));
        window.draw(westRed);

        CircleShape westYellow((int)(12 * min(scaleX, scaleY)));
        westYellow.setFillColor(trafficSystem.get_state() == EW_YELLOW ? YELLOW : OFF);
        westYellow.setPosition(centerX - (int)(125 * scaleX), centerY - (int)(12 * scaleY));
        window.draw(westYellow);

        CircleShape westGreen((int)(12 * min(scaleX, scaleY)));
        westGreen.setFillColor(trafficSystem.get_state() == EW_GREEN ? GREEN : OFF);
        westGreen.setPosition(centerX - (int)(95 * scaleX), centerY - (int)(12 * scaleY));
        window.draw(westGreen);

        Text westLabel("WEST", font, (unsigned int)(12 * min(scaleX, scaleY)));
        westLabel.setFillColor(TEXT_COLOR);
        westLabel.setPosition(centerX - (int)(125 * scaleX), centerY + (int)(20 * scaleY));
        window.draw(westLabel);

                // Draw vehicle count displays - position them clearly away from intersection
        Text northCount("N: " + to_string(trafficSystem.get_vehicle_count(NORTH)), font, 16);
        northCount.setFillColor(TEXT_COLOR);
        northCount.setPosition(100, 150);
        window.draw(northCount);

        Text southCount("S: " + to_string(trafficSystem.get_vehicle_count(SOUTH)), font, 16);
        southCount.setFillColor(TEXT_COLOR);
        southCount.setPosition(100, 180);
        window.draw(southCount);

        Text eastCount("E: " + to_string(trafficSystem.get_vehicle_count(EAST)), font, 16);
        eastCount.setFillColor(TEXT_COLOR);
        eastCount.setPosition(100, 210);
        window.draw(eastCount);

        Text westCount("W: " + to_string(trafficSystem.get_vehicle_count(WEST)), font, 16);
        westCount.setFillColor(TEXT_COLOR);
        westCount.setPosition(100, 240);
        window.draw(westCount);

        // Draw pedestrian signals - positioned outside the road on sidewalks/corners
        // North pedestrian signal - positioned above the road
        RectangleShape pedNorth(Vector2f(40, 40));
        pedNorth.setFillColor(trafficSystem.get_walk_signal(NORTH) ? GREEN : RED);
        pedNorth.setPosition(centerX + 100, centerY - 230);
        window.draw(pedNorth);

        Text pedNorthText(trafficSystem.get_walk_signal(NORTH) ? "WALK" : "STOP", font, 14);
        pedNorthText.setFillColor(Color::Black);
        pedNorthText.setPosition(centerX + 105, centerY - 220);
        window.draw(pedNorthText);

        if (trafficSystem.get_request(NORTH)) {
            CircleShape reqNorth(8);
            reqNorth.setFillColor(HIGHLIGHT);
            reqNorth.setPosition(centerX + 145, centerY - 220);
            window.draw(reqNorth);
            
            Text reqText("REQ", font, 8);
            reqText.setFillColor(Color::White);
            reqText.setPosition(centerX + 148, centerY - 218);
            window.draw(reqText);
        }

        // South pedestrian signal - positioned below the road
        RectangleShape pedSouth(Vector2f(40, 40));
        pedSouth.setFillColor(trafficSystem.get_walk_signal(SOUTH) ? GREEN : RED);
        pedSouth.setPosition(centerX + 100, centerY + 190);
        window.draw(pedSouth);

        Text pedSouthText(trafficSystem.get_walk_signal(SOUTH) ? "WALK" : "STOP", font, 14);
        pedSouthText.setFillColor(Color::Black);
        pedSouthText.setPosition(centerX + 105, centerY + 200);
        window.draw(pedSouthText);

        if (trafficSystem.get_request(SOUTH)) {
            CircleShape reqSouth(8);
            reqSouth.setFillColor(HIGHLIGHT);
            reqSouth.setPosition(centerX + 145, centerY + 200);
            window.draw(reqSouth);
            
            Text reqText("REQ", font, 8);
            reqText.setFillColor(Color::White);
            reqText.setPosition(centerX + 148, centerY + 202);
            window.draw(reqText);
        }

        // East pedestrian signal - positioned to the right of the road
        RectangleShape pedEast(Vector2f(40, 40));
        pedEast.setFillColor(trafficSystem.get_walk_signal(EAST) ? GREEN : RED);
        pedEast.setPosition(centerX + 220, centerY + 100);
        window.draw(pedEast);

        Text pedEastText(trafficSystem.get_walk_signal(EAST) ? "WALK" : "STOP", font, 14);
        pedEastText.setFillColor(Color::Black);
        pedEastText.setPosition(centerX + 225, centerY + 110);
        window.draw(pedEastText);

        if (trafficSystem.get_request(EAST)) {
            CircleShape reqEast(8);
            reqEast.setFillColor(HIGHLIGHT);
            reqEast.setPosition(centerX + 265, centerY + 110);
            window.draw(reqEast);
            
            Text reqText("REQ", font, 8);
            reqText.setFillColor(Color::White);
            reqText.setPosition(centerX + 268, centerY + 112);
            window.draw(reqText);
        }

        // West pedestrian signal - positioned to the left of the road
        RectangleShape pedWest(Vector2f(40, 40));
        pedWest.setFillColor(trafficSystem.get_walk_signal(WEST) ? GREEN : RED);
        pedWest.setPosition(centerX - 260, centerY + 100);
        window.draw(pedWest);

        Text pedWestText(trafficSystem.get_walk_signal(WEST) ? "WALK" : "STOP", font, 14);
        pedWestText.setFillColor(Color::Black);
        pedWestText.setPosition(centerX - 255, centerY + 110);
        window.draw(pedWestText);

        if (trafficSystem.get_request(WEST)) {
            CircleShape reqWest(8);
            reqWest.setFillColor(HIGHLIGHT);
            reqWest.setPosition(centerX - 215, centerY + 110);
            window.draw(reqWest);
            
            Text reqText("REQ", font, 8);
            reqText.setFillColor(Color::White);
            reqText.setPosition(centerX - 212, centerY + 112);
            window.draw(reqText);
        }

        // Draw vehicles ON the roads with different types, colors, and sizes
        // Roads: Horizontal (200-1000, 320-480), Vertical (520-680, 100-700)
        
        // North lane vehicles (approaching from north)
        const auto& northVehicles = trafficSystem.get_vehicles(NORTH);
        for (int i = 0; i < min((int)northVehicles.size(), 8); i++) {
            VehicleType type = (i < northVehicles.size()) ? northVehicles[i].type : CAR;
            
            // Different sizes for different vehicle types
            Vector2f vehicleSize(45, 22);
            Color vehicleColor = VEHICLE_COLOR;
            string vehicleText = "CAR";
            
            switch(type) {
                case CAR: 
                    vehicleSize = Vector2f(45, 22);
                    vehicleColor = VEHICLE_COLOR; 
                    vehicleText = "CAR"; 
                    break;
                case TRUCK: 
                    vehicleSize = Vector2f(55, 28);
                    vehicleColor = TRUCK_COLOR; 
                    vehicleText = "TRUCK"; 
                    break;
                case BUS: 
                    vehicleSize = Vector2f(60, 25);
                    vehicleColor = BUS_COLOR; 
                    vehicleText = "BUS"; 
                    break;
                case MOTORCYCLE: 
                    vehicleSize = Vector2f(35, 18);
                    vehicleColor = MOTORCYCLE_COLOR; 
                    vehicleText = "BIKE"; 
                    break;
            }
            
            RectangleShape car(vehicleSize);
            car.setFillColor(vehicleColor);
            car.setPosition(centerX - 70, 120 + i * 30); // Left lane of vertical road, approaching intersection
            window.draw(car);

            Text carText(vehicleText, font, 10);
            carText.setFillColor(Color::White);
            carText.setPosition(centerX - 65, 125 + i * 30);
            window.draw(carText);
        }

        // South lane vehicles (approaching from south)  
        const auto& southVehicles = trafficSystem.get_vehicles(SOUTH);
        for (int i = 0; i < min((int)southVehicles.size(), 8); i++) {
            VehicleType type = (i < southVehicles.size()) ? southVehicles[i].type : CAR;
            
            // Different sizes for different vehicle types
            Vector2f vehicleSize(45, 22);
            Color vehicleColor = VEHICLE_COLOR;
            string vehicleText = "CAR";
            
            switch(type) {
                case CAR: 
                    vehicleSize = Vector2f(45, 22);
                    vehicleColor = VEHICLE_COLOR; 
                    vehicleText = "CAR"; 
                    break;
                case TRUCK: 
                    vehicleSize = Vector2f(55, 28);
                    vehicleColor = TRUCK_COLOR; 
                    vehicleText = "TRUCK"; 
                    break;
                case BUS: 
                    vehicleSize = Vector2f(60, 25);
                    vehicleColor = BUS_COLOR; 
                    vehicleText = "BUS"; 
                    break;
                case MOTORCYCLE: 
                    vehicleSize = Vector2f(35, 18);
                    vehicleColor = MOTORCYCLE_COLOR; 
                    vehicleText = "BIKE"; 
                    break;
            }
            
            RectangleShape car(vehicleSize);
            car.setFillColor(vehicleColor);
            car.setPosition(centerX + 25, centerY + 120 + i * 30); // Right lane of vertical road, approaching intersection
            window.draw(car);

            Text carText(vehicleText, font, 10);
            carText.setFillColor(Color::White);
            carText.setPosition(centerX + 30, centerY + 125 + i * 30);
            window.draw(carText);
        }

        // East lane vehicles (approaching from east)
        const auto& eastVehicles = trafficSystem.get_vehicles(EAST);
        for (int i = 0; i < min((int)eastVehicles.size(), 10); i++) {
            VehicleType type = (i < eastVehicles.size()) ? eastVehicles[i].type : CAR;

            RectangleShape car(Vector2f(22, 45));
            Color vehicleColor = VEHICLE_COLOR;
            string vehicleText = "CAR";

            switch(type) {
                case CAR: vehicleColor = VEHICLE_COLOR; vehicleText = "CAR"; break;
                case TRUCK: vehicleColor = TRUCK_COLOR; vehicleText = "TRUCK"; break;
                case BUS: vehicleColor = BUS_COLOR; vehicleText = "BUS"; break;
                case MOTORCYCLE: vehicleColor = MOTORCYCLE_COLOR; vehicleText = "BIKE"; break;
            }

            car.setFillColor(vehicleColor);
            car.setPosition(978 - i * 25, centerY + 25);
            window.draw(car);

            // Align label horizontally with the vehicle, keep y constant
            Text carText(vehicleText, font, 10);
            carText.setFillColor(Color::White);
            carText.setPosition(978 - i * 25, centerY + 50); // y is fixed, x matches vehicle
            window.draw(carText);
        }

        // West lane vehicles (approaching from west)
        const auto& westVehicles = trafficSystem.get_vehicles(WEST);
        for (int i = 0; i < min((int)westVehicles.size(), 10); i++) {
            VehicleType type = (i < westVehicles.size()) ? westVehicles[i].type : CAR;
            
            RectangleShape car(Vector2f(22, 45));
            Color vehicleColor = VEHICLE_COLOR;
            string vehicleText = "CAR";
            
            switch(type) {
                case CAR: vehicleColor = VEHICLE_COLOR; vehicleText = "CAR"; break;
                case TRUCK: vehicleColor = TRUCK_COLOR; vehicleText = "TRUCK"; break;
                case BUS: vehicleColor = BUS_COLOR; vehicleText = "BUS"; break;
                case MOTORCYCLE: vehicleColor = MOTORCYCLE_COLOR; vehicleText = "BIKE"; break;
            }
            
            car.setFillColor(vehicleColor);
            car.setPosition(222 + i * 25, 330); // centerY - 70 = 400 - 70 = 330
            window.draw(car);

            Text carText(vehicleText, font, 10);
            carText.setFillColor(Color::White);
            carText.setPosition(225 + i * 25, 340); // centerY - 60 = 400 - 60 = 340
            window.draw(carText);
        }

        // Draw emergency indicator
        if (trafficSystem.is_emergency()) {
            Text emergency("EMERGENCY VEHICLE PASSING", font, 32);
            emergency.setFillColor(Color::Red);
            emergency.setStyle(Text::Bold);
            emergency.setPosition(600 - emergency.getLocalBounds().width/2, 380); // centerX = 600, centerY = 400
            window.draw(emergency);
            
            // Draw flashing background
            RectangleShape emergencyBg(Vector2f(emergency.getLocalBounds().width + 20, 40));
            emergencyBg.setFillColor(Color(255, 0, 0, 100));
            emergencyBg.setPosition(600 - emergency.getLocalBounds().width/2 - 10, 375); // centerX - width/2 - 10, centerY - 25
            window.draw(emergencyBg);
            window.draw(emergency);
        }

        // Draw status panel with fixed size
        RectangleShape statusPanel(Vector2f(1100, 220));
        statusPanel.setFillColor(PANEL);
        statusPanel.setPosition(50, 720);
        statusPanel.setOutlineThickness(2);
        statusPanel.setOutlineColor(Color(80, 80, 120));
        window.draw(statusPanel);

        // Draw status text
        string stateText;
        Color stateColor;
        switch (trafficSystem.get_state()) {
            case NS_GREEN: 
                stateText = "NORTH-SOUTH GREEN";
                stateColor = GREEN;
                break;
            case NS_YELLOW: 
                stateText = "NORTH-SOUTH YELLOW";
                stateColor = YELLOW;
                break;
            case EW_GREEN: 
                stateText = "EAST-WEST GREEN";
                stateColor = GREEN;
                break;
            case EW_YELLOW: 
                stateText = "EAST-WEST YELLOW";
                stateColor = YELLOW;
                break;
        }

        Text stateDisplay(stateText, font, (unsigned int)(28 * min(scaleX, scaleY)));
        stateDisplay.setFillColor(stateColor);
        stateDisplay.setStyle(Text::Bold);
        stateDisplay.setPosition(100, 740);
        window.draw(stateDisplay);

        string timerText = "Time: " + to_string(trafficSystem.get_timer()) + "s";
        Text timerDisplay(timerText, font, (unsigned int)(28 * min(scaleX, scaleY)));
        timerDisplay.setFillColor(HIGHLIGHT);
        timerDisplay.setStyle(Text::Bold);
        timerDisplay.setPosition(100, 780);
        window.draw(timerDisplay);

        // Draw statistics
        string statsText = "Vehicles: " + to_string(trafficSystem.get_vehicles_passed()) + 
                          " | Pedestrians: " + to_string(trafficSystem.get_pedestrians_crossed()) +
                          " | Time: " + format_time(trafficSystem.get_elapsed_seconds());
        Text statsDisplay(statsText, font, (unsigned int)(20 * min(scaleX, scaleY)));
        statsDisplay.setFillColor(TEXT_COLOR);
        statsDisplay.setPosition(500, 780);
        window.draw(statsDisplay);

        // Draw settings
        string settingsText = "Adaptive: " + string(trafficSystem.is_adaptive() ? "ON" : "OFF") + 
                             " | Auto Vehicles: " + string(trafficSystem.is_auto_vehicles() ? "ON" : "OFF") +
                             " | Speed: " + to_string(trafficSystem.get_speed()) + "x";
        Text settingsDisplay(settingsText, font, (unsigned int)(18 * min(scaleX, scaleY)));
        settingsDisplay.setFillColor(TEXT_COLOR);
        settingsDisplay.setPosition(500, 740);
        window.draw(settingsDisplay);

        // Draw controls with better formatting and scaling
        Text controls("CONTROLS:", font, (unsigned int)(22 * min(scaleX, scaleY)));
        controls.setFillColor(HIGHLIGHT);
        controls.setStyle(Text::Bold);
        controls.setPosition(100, 820);
        window.draw(controls);

        Text controls1("Pedestrians: [N] North  [S] South  [E] East  [W] West", font, (unsigned int)(16 * min(scaleX, scaleY)));
        controls1.setFillColor(TEXT_COLOR);
        controls1.setPosition(100, 845);
        window.draw(controls1);

        Text controls2("Vehicles: [1] North  [2] South  [3] East  [4] West", font, (unsigned int)(16 * min(scaleX, scaleY)));
        controls2.setFillColor(TEXT_COLOR);
        controls2.setPosition(100, 870);
        window.draw(controls2);

        Text controls3("System: [T] Adaptive  [A] Auto-Vehicles  [+/-] Speed  [M] Emergency  [C] Config  [R] Reset  [Q] Quit", font, (unsigned int)(14 * min(scaleX, scaleY)));
        controls3.setFillColor(TEXT_COLOR);
        controls3.setPosition(100, 895);
        window.draw(controls3);

        // Draw configuration menu if open
        if (trafficSystem.is_config_menu_open()) {
            RectangleShape configPanel(Vector2f(400, 300));
            configPanel.setFillColor(Color(40, 40, 60, 240));
            configPanel.setPosition(400, 300);
            configPanel.setOutlineThickness(3);
            configPanel.setOutlineColor(HIGHLIGHT);
            window.draw(configPanel);
            
            Text configTitle("CONFIGURATION MENU", font, 24);
            configTitle.setFillColor(HIGHLIGHT);
            configTitle.setStyle(Text::Bold);
            configTitle.setPosition(450, 320);
            window.draw(configTitle);
            
            Text layoutText("Current Layout: " + string(trafficSystem.get_layout() == STANDARD_4WAY ? "4-Way Intersection" : 
                                                         trafficSystem.get_layout() == T_INTERSECTION ? "T-Intersection" : "Roundabout"), font, 18);
            layoutText.setFillColor(TEXT_COLOR);
            layoutText.setPosition(420, 360);
            window.draw(layoutText);
            
            Text configInstr1("Features Available:", font, 16);
            configInstr1.setFillColor(GREEN);
            configInstr1.setPosition(420, 400);
            window.draw(configInstr1);
            
            Text configInstr3("* Vehicle Types (Car/Truck/Bus/Bike)", font, 14);
            configInstr3.setFillColor(TEXT_COLOR);
            configInstr3.setPosition(430, 425);
            window.draw(configInstr3);
            
            Text configInstr4("* Pedestrian Signals (Walk/Stop)", font, 14);
            configInstr4.setFillColor(TEXT_COLOR);
            configInstr4.setPosition(430, 445);
            window.draw(configInstr4);
            
            Text configInstr5("* Enhanced Statistics", font, 14);
            configInstr5.setFillColor(TEXT_COLOR);
            configInstr5.setPosition(430, 465);
            window.draw(configInstr5);
            
            Text configClose("Press [C] to close", font, 16);
            configClose.setFillColor(HIGHLIGHT);
            configClose.setPosition(500, 520);
            window.draw(configClose);
        }

        // Draw footer
        Text footer("Smart Traffic Control System - Real-time Intersection Management", font, (unsigned int)(16 * min(scaleX, scaleY)));
        footer.setFillColor(Color(150, 150, 180));
        footer.setPosition(600 - footer.getLocalBounds().width/2, 920);
        window.draw(footer);

        // Force display and add small delay to prevent rendering issues
        window.display();
        
        // Add a small sleep to prevent overwhelming the graphics system
        sf::sleep(sf::milliseconds(16)); // ~60 FPS
    }

    return 0;
}