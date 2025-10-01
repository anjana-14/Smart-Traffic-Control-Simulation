# 🚦 Smart Traffic Control System - Complete Flowchart

## 📊 System Overview Flowchart

```mermaid
flowchart TD
    A[🚀 Program Start] --> B[🎲 Initialize Random Seed]
    B --> C[🖥️ Create SFML Window<br/>1200×980 pixels]
    C --> D{🪟 Window Created?}
    D -->|❌ No| E[💥 Exit with Error]
    D -->|✅ Yes| F[⚙️ Set Frame Rate Limit 60 FPS]
    F --> G[🔤 Load System Fonts]
    G --> H[🏗️ Create TrafficSystem Instance]
    H --> I[⏰ Initialize Simulation Clock]
    I --> J[🔄 Enter Main Game Loop]
    
    %% Main Loop
    J --> K[📥 Poll SFML Events]
    K --> L{🎮 Event Type?}
    
    %% Event Handling
    L -->|🔴 Close| M[🚪 Close Window]
    L -->|⌨️ KeyPressed| N[🎯 Process Keyboard Input]
    L -->|🔄 Other| O[📊 Update Simulation]
    
    %% Keyboard Processing
    N --> P{🔑 Which Key?}
    P -->|N,S,E,W| Q[🚶‍♂️ Request Pedestrian Crossing]
    P -->|1,2,3,4| R[🚗 Add Vehicle to Direction]
    P -->|T| S[🧠 Toggle Adaptive Mode]
    P -->|A| T[🤖 Toggle Auto Vehicles]
    P -->|+/-| U[⚡ Adjust Simulation Speed]
    P -->|M| V[🚑 Trigger Emergency Mode]
    P -->|C| W[📋 Toggle Config Menu]
    P -->|R| X[🔄 Reset System]
    P -->|Q| Y[🚪 Quit Application]
    
    %% Update Simulation
    Q --> O
    R --> O
    S --> O
    T --> O
    U --> O
    V --> O
    W --> O
    X --> O
    Y --> M
    
    O --> Z{⚡ Emergency Mode?}
    Z -->|✅ Yes| AA[⏱️ Decrement Emergency Timer]
    Z -->|❌ No| BB[⏱️ Decrement Main Timer]
    
    AA --> CC{⏰ Emergency Timer ≤ 0?}
    CC -->|✅ Yes| DD[🔄 Exit Emergency Mode<br/>Set Timer = 5]
    CC -->|❌ No| EE[📊 Update Display]
    DD --> EE
    
    BB --> FF{⏰ Main Timer ≤ 0?}
    FF -->|❌ No| GG[🚶‍♂️ Process Pedestrian Requests]
    FF -->|✅ Yes| HH{🚦 Current State?}
    
    %% State Transitions
    HH -->|🟢 NS_GREEN| II[🟡 Change to NS_YELLOW<br/>Timer = 5s]
    HH -->|🟡 NS_YELLOW| JJ[🟢 Change to EW_GREEN<br/>Calculate Adaptive Time]
    HH -->|🟢 EW_GREEN| KK[🟡 Change to EW_YELLOW<br/>Timer = 5s]
    HH -->|🟡 EW_YELLOW| LL[🟢 Change to NS_GREEN<br/>Calculate Adaptive Time]
    
    II --> MM[🚫 Reset NS Walk Signals]
    JJ --> NN[📊 Update Vehicle Stats<br/>Clear EW Vehicles]
    KK --> OO[🚫 Reset EW Walk Signals]
    LL --> PP[📊 Update Vehicle Stats<br/>Clear NS Vehicles]
    
    MM --> GG
    NN --> GG
    OO --> GG
    PP --> GG
    
    %% Pedestrian Processing
    GG --> QQ{🚶‍♂️ Pedestrian Requests?}
    QQ -->|✅ NS_GREEN + Requests| RR[🟢 Activate NS Walk Signals<br/>Update Stats]
    QQ -->|✅ EW_GREEN + Requests| SS[🟢 Activate EW Walk Signals<br/>Update Stats]
    QQ -->|❌ No Requests| TT[🤖 Auto Vehicle Generation]
    
    RR --> TT
    SS --> TT
    
    %% Auto Vehicle Generation
    TT --> UU{🎲 Random Vehicle?}
    UU -->|✅ Yes| VV[🚗 Add Random Vehicle<br/>Random Direction & Type]
    UU -->|❌ No| EE
    VV --> EE
    
    %% Rendering Pipeline
    EE --> WW[🎨 Clear Window Background]
    WW --> XX[🖼️ Draw Title & Main Panel]
    XX --> YY[🛣️ Draw Roads & Lane Markings]
    YY --> ZZ[🚦 Draw Traffic Lights]
    ZZ --> AAA[🚗 Draw Vehicles by Type]
    AAA --> BBB[🚶‍♂️ Draw Pedestrian Signals]
    BBB --> CCC[📊 Draw Statistics Panel]
    CCC --> DDD[🎮 Draw Control Instructions]
    DDD --> EEE{📋 Config Menu Open?}
    EEE -->|✅ Yes| FFF[📋 Draw Configuration Menu]
    EEE -->|❌ No| GGG[🖥️ Display Window]
    FFF --> GGG
    
    GGG --> HHH[⏱️ Frame Rate Control<br/>16ms delay]
    HHH --> III{🪟 Window Open?}
    III -->|✅ Yes| J
    III -->|❌ No| JJJ[🏁 Program End]
    
    M --> JJJ
    E --> JJJ

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef error fill:#F44336,stroke:#C62828,stroke-width:2px,color:#fff
    classDef state fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef render fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    classDef input fill:#8BC34A,stroke:#558B2F,stroke-width:2px,color:#fff
    
    class A,JJJ startEnd
    class B,C,F,G,H,I,WW,XX,YY,ZZ,AAA,BBB,CCC,DDD,FFF,GGG,HHH process
    class D,L,P,Z,CC,FF,HH,QQ,UU,EEE,III decision
    class E error
    class II,JJ,KK,LL,MM,NN,OO,PP,RR,SS state
    class K,N,Q,R,S,T,U,V,W,X,Y input
    class AA,BB,DD,GG,TT,UU,VV,EE render
```

## 🔧 Adaptive Algorithm Flowchart

```mermaid
flowchart TD
    A[🧠 Calculate Green Time Called] --> B{🎯 Adaptive Mode?}
    B -->|❌ No| C[⏰ Return Default 30s]
    B -->|✅ Yes| D{🚦 NS or EW Direction?}
    
    D -->|🔵 NS Direction| E[📊 Count NS Traffic<br/>North + South Vehicles]
    D -->|🔴 EW Direction| F[📊 Count EW Traffic<br/>East + West Vehicles]
    
    E --> G[📊 Count Cross Traffic<br/>East + West Vehicles]
    F --> H[📊 Count Cross Traffic<br/>North + South Vehicles]
    
    G --> I[🧮 Calculate Traffic Difference<br/>max(0, main_traffic - cross_traffic)]
    H --> I
    
    I --> J[⏱️ Calculate Extra Time<br/>min(40, traffic_diff × 2)]
    J --> K[✅ Return Final Time<br/>20 + extra_time seconds]
    
    C --> L[🏁 End]
    K --> L

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef calculation fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    
    class A,L startEnd
    class E,F,G,H,J,K process
    class B,D decision
    class C,I calculation
```

## 🚗 Vehicle Management Flowchart

```mermaid
flowchart TD
    A[🚗 Add Vehicle Called] --> B[🎲 Generate Random Vehicle Type]
    B --> C{🚙 Vehicle Type?}
    
    C -->|🚗| D[CAR: 45×22px, Steel Blue]
    C -->|🚛| E[TRUCK: 55×28px, Brown]
    C -->|🚌| F[BUS: 60×25px, Orange]
    C -->|🏍️| G[MOTORCYCLE: 35×18px, Green]
    
    D --> H[📍 Add to Direction Vector]
    E --> H
    F --> H
    G --> H
    
    H --> I[📈 Increment Vehicle Count]
    I --> J[🎨 Queue for Rendering]
    J --> K[✅ Vehicle Added]
    
    %% Auto Generation
    L[🤖 Auto Vehicle Generation] --> M{🎲 Random Check<br/>1 in 5 chance?}
    M -->|❌ No| N[⏭️ Skip Generation]
    M -->|✅ Yes| O[🎯 Pick Random Direction]
    O --> A
    
    %% Vehicle Clearing
    P[🚦 State Change Trigger] --> Q{🚦 Which Transition?}
    Q -->|🟡 NS_YELLOW → 🟢 EW_GREEN| R[🧹 Clear EW Vehicles<br/>Update Stats]
    Q -->|🟡 EW_YELLOW → 🟢 NS_GREEN| S[🧹 Clear NS Vehicles<br/>Update Stats]
    Q -->|🟢→🟡| T[⏭️ No Vehicle Clearing]
    
    R --> U[📊 Add to Vehicles Passed]
    S --> U
    T --> V[🔄 Continue]
    U --> V

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef vehicle fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef stats fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    
    class A,K,N,V startEnd
    class B,H,I,J,L,O,P,R,S,U process
    class C,M,Q decision
    class D,E,F,G vehicle
```

## 🚶‍♂️ Pedestrian System Flowchart

```mermaid
flowchart TD
    A[🚶‍♂️ Pedestrian Request] --> B[📝 Set Request Flag for Direction]
    B --> C[⏳ Wait for Compatible Green Phase]
    
    C --> D{🚦 Current State Check}
    D -->|🟢 NS_GREEN| E{📍 NS Request?}
    D -->|🟢 EW_GREEN| F{📍 EW Request?}
    D -->|🟡 Yellow States| G[⏳ Wait for Next Green]
    
    E -->|✅ Yes| H{⏰ Timer at Start?}
    E -->|❌ No| G
    F -->|✅ Yes| I{⏰ Timer at Start?}
    F -->|❌ No| G
    
    H -->|✅ Full Time| J[🟢 Activate NS Walk Signal<br/>WALK Display]
    H -->|❌ Partial| G
    I -->|✅ Full Time| K[🟢 Activate EW Walk Signal<br/>WALK Display]
    I -->|❌ Partial| G
    
    J --> L[📊 Increment Pedestrians Crossed]
    K --> L
    
    L --> M[🚫 Clear Request Flags]
    M --> N[🎨 Display WALK Signal]
    
    %% Signal Reset
    O[🚦 State Transition] --> P{🔄 Transition Type?}
    P -->|🟢→🟡 NS| Q[🚫 Reset NS Walk Signals<br/>Display STOP]
    P -->|🟢→🟡 EW| R[🚫 Reset EW Walk Signals<br/>Display STOP]
    
    Q --> S[🔴 Show STOP Signal]
    R --> S
    S --> T[✅ Pedestrian Cycle Complete]
    
    G --> U[⏳ Continue Waiting]
    U --> D
    N --> V[⏭️ Continue System]
    T --> V

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef pedestrian fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef signal fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    classDef wait fill:#795548,stroke:#5D4037,stroke-width:2px,color:#fff
    
    class A,T,V startEnd
    class B,C,L,M,O,Q,R process
    class D,E,F,H,I,P decision
    class J,K,N,S pedestrian
    class G,U wait
```

## 🚦 Traffic State Machine Flowchart

```mermaid
stateDiagram-v2
    [*] --> NS_GREEN : 🚀 System Start

    NS_GREEN --> NS_YELLOW : ⏰ Timer = 0<br/>🕐 5 seconds
    NS_YELLOW --> EW_GREEN : ⏰ Timer = 0<br/>🧮 Calculate Adaptive Time<br/>📊 Clear EW Vehicles
    EW_GREEN --> EW_YELLOW : ⏰ Timer = 0<br/>🕐 5 seconds  
    EW_YELLOW --> NS_GREEN : ⏰ Timer = 0<br/>🧮 Calculate Adaptive Time<br/>📊 Clear NS Vehicles

    state NS_GREEN {
        [*] --> ActiveGreen
        ActiveGreen --> PedestrianCheck : 🚶‍♂️ Check NS Requests
        PedestrianCheck --> WalkSignal : ✅ Activate Walk
        PedestrianCheck --> ActiveGreen : ❌ No Requests
        WalkSignal --> ActiveGreen : 🔄 Continue
    }

    state EW_GREEN {
        [*] --> ActiveGreen2
        ActiveGreen2 --> PedestrianCheck2 : 🚶‍♂️ Check EW Requests
        PedestrianCheck2 --> WalkSignal2 : ✅ Activate Walk
        PedestrianCheck2 --> ActiveGreen2 : ❌ No Requests
        WalkSignal2 --> ActiveGreen2 : 🔄 Continue
    }

    state EmergencyMode {
        [*] --> Emergency : 🚑 M Key Pressed
        Emergency --> EmergencyTimer : ⏰ 15 Second Timer
        EmergencyTimer --> [*] : ⏰ Timer Complete<br/>🔄 Return to Normal
    }

    NS_GREEN --> EmergencyMode : 🚑 Emergency Trigger
    NS_YELLOW --> EmergencyMode : 🚑 Emergency Trigger
    EW_GREEN --> EmergencyMode : 🚑 Emergency Trigger
    EW_YELLOW --> EmergencyMode : 🚑 Emergency Trigger
    EmergencyMode --> NS_GREEN : 🔄 Resume Normal Operation
```

## 🎨 Rendering Pipeline Flowchart

```mermaid
flowchart TD
    A[🎨 Start Rendering] --> B[🧹 Clear Window Background<br/>Color: RGB(30,30,50)]
    B --> C[📏 Calculate Scale Factors<br/>scaleX = scaleY = 1.0]
    
    C --> D[🏷️ Draw Title Text<br/>'SMART TRAFFIC CONTROL SYSTEM']
    D --> E[📋 Draw Main Panel<br/>1100×700px at (50,100)]
    
    E --> F[🛣️ Draw Intersection Roads]
    F --> G[📍 Calculate Center Point<br/>centerX=600, centerY=400]
    G --> H[🛤️ Draw Horizontal Road<br/>800×160px]
    H --> I[🛤️ Draw Vertical Road<br/>160×600px]
    I --> J[➖ Draw Lane Markings<br/>Dashed Lines]
    
    J --> K[🚦 Draw Traffic Light Boxes]
    K --> L[🔴 Draw North Traffic Light<br/>Red/Yellow/Green Circles]
    L --> M[🔴 Draw South Traffic Light<br/>Red/Yellow/Green Circles]
    M --> N[🔴 Draw East Traffic Light<br/>Red/Yellow/Green Circles]
    N --> O[🔴 Draw West Traffic Light<br/>Red/Yellow/Green Circles]
    
    O --> P[🚗 Vehicle Rendering Loop]
    P --> Q[🔵 Draw North Vehicles<br/>X=530, Y=120+i*30]
    Q --> R[🔵 Draw South Vehicles<br/>X=625, Y=520+i*30]
    R --> S[🔵 Draw East Vehicles<br/>X=978-i*25, Y=425]
    S --> T[🔵 Draw West Vehicles<br/>X=222+i*25, Y=330]
    
    T --> U{🚗 Vehicle Type Rendering}
    U -->|🚗 CAR| V[🔷 Blue Rectangle 45×22<br/>Label: 'CAR']
    U -->|🚛 TRUCK| W[🟤 Brown Rectangle 55×28<br/>Label: 'TRUCK']
    U -->|🚌 BUS| X[🟠 Orange Rectangle 60×25<br/>Label: 'BUS']
    U -->|🏍️ MOTORCYCLE| Y[🟢 Green Rectangle 35×18<br/>Label: 'BIKE']
    
    V --> Z[🚶‍♂️ Draw Pedestrian Signals]
    W --> Z
    X --> Z
    Y --> Z
    
    Z --> AA[🟢 North Signal: (700,170) WALK/STOP]
    AA --> BB[🟢 South Signal: (700,590) WALK/STOP]
    BB --> CC[🟢 East Signal: (820,500) WALK/STOP]
    CC --> DD[🟢 West Signal: (340,500) WALK/STOP]
    
    DD --> EE{🚶‍♂️ Request Indicators}
    EE -->|✅ Active| FF[💙 Draw Request Circle<br/>Blue Highlight + 'REQ']
    EE -->|❌ None| GG[📊 Draw Status Panel]
    FF --> GG
    
    GG --> HH[📋 Draw Status Panel<br/>1100×220px at (50,720)]
    HH --> II[🚦 Draw Current State Text<br/>Color-coded by State]
    II --> JJ[⏰ Draw Timer Display<br/>Countdown in Blue]
    JJ --> KK[📊 Draw Statistics<br/>Vehicles/Pedestrians/Time]
    KK --> LL[⚙️ Draw Settings Display<br/>Adaptive/Auto/Speed Status]
    
    LL --> MM[🎮 Draw Control Instructions]
    MM --> NN[📝 Controls Title: 'CONTROLS:']
    NN --> OO[🚶‍♂️ Pedestrian Controls Line]
    OO --> PP[🚗 Vehicle Controls Line]
    PP --> QQ[⚙️ System Controls Line]
    
    QQ --> RR{📋 Config Menu?}
    RR -->|✅ Open| SS[📋 Draw Configuration Panel<br/>400×300px Semi-transparent]
    RR -->|❌ Closed| TT[👣 Draw Footer Text]
    
    SS --> UU[🏷️ Config Title & Layout Info]
    UU --> VV[📝 Feature List Display]
    VV --> WW[🔄 Close Instructions]
    WW --> TT
    
    TT --> XX{🚑 Emergency Mode?}
    XX -->|✅ Active| YY[🚨 Draw Emergency Overlay<br/>Red Background + Text]
    XX -->|❌ Normal| ZZ[🖥️ Display Final Frame]
    YY --> ZZ
    
    ZZ --> AAA[⏱️ Frame Rate Control<br/>16ms Sleep]
    AAA --> BBB[🏁 Rendering Complete]

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef render fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    classDef vehicle fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef ui fill:#8BC34A,stroke:#558B2F,stroke-width:2px,color:#fff
    
    class A,BBB startEnd
    class B,C,G,HH,II,JJ,KK,LL,MM,NN,OO,PP,QQ,TT,UU,VV,WW,AAA process
    class EE,RR,XX decision
    class D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,Z,AA,BB,CC,DD,FF,SS,YY,ZZ render
    class V,W,X,Y vehicle
```

## ⌨️ Keyboard Input Handler Flowchart

```mermaid
flowchart TD
    A[⌨️ Key Press Event] --> B{🔑 Key Identification}
    
    %% Pedestrian Controls
    B -->|🔤 N| C[🚶‍♂️ Request North Crossing<br/>requests[NORTH] = true]
    B -->|🔤 S| D[🚶‍♂️ Request South Crossing<br/>requests[SOUTH] = true]
    B -->|🔤 E| E[🚶‍♂️ Request East Crossing<br/>requests[EAST] = true]
    B -->|🔤 W| F[🚶‍♂️ Request West Crossing<br/>requests[WEST] = true]
    
    %% Vehicle Addition
    B -->|🔢 1| G[🚗 Add Vehicle to North<br/>Random Type Generation]
    B -->|🔢 2| H[🚗 Add Vehicle to South<br/>Random Type Generation]
    B -->|🔢 3| I[🚗 Add Vehicle to East<br/>Random Type Generation]
    B -->|🔢 4| J[🚗 Add Vehicle to West<br/>Random Type Generation]
    
    %% System Controls
    B -->|🔤 T| K[🧠 Toggle Adaptive Mode<br/>adaptive_mode = !adaptive_mode]
    B -->|🔤 A| L[🤖 Toggle Auto Vehicles<br/>auto_vehicles = !auto_vehicles]
    B -->|➕ +| M[⚡ Speed Up<br/>min(5.0, speed + 0.5)]
    B -->|➖ -| N[🐌 Slow Down<br/>max(0.5, speed - 0.5)]
    B -->|🔤 M| O[🚑 Trigger Emergency<br/>15-second override]
    B -->|🔤 C| P[📋 Toggle Config Menu<br/>show_config_menu = !show_config_menu]
    B -->|🔤 R| Q[🔄 Reset Entire System<br/>Clear all data]
    B -->|🔤 Q| R[🚪 Quit Application<br/>Close window]
    
    %% Processing Results
    C --> S[✅ Pedestrian Request Queued]
    D --> S
    E --> S
    F --> S
    
    G --> T[✅ Vehicle Added to Queue]
    H --> T
    I --> T
    J --> T
    
    K --> U[✅ Adaptive Mode Toggled]
    L --> U
    M --> U
    N --> U
    O --> U
    P --> U
    
    Q --> V[🔄 System Reset Complete<br/>Return to NS_GREEN]
    R --> W[🏁 Application Termination]
    
    S --> X[🔄 Continue Main Loop]
    T --> X
    U --> X
    V --> X

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef pedestrian fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef vehicle fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    classDef system fill:#8BC34A,stroke:#558B2F,stroke-width:2px,color:#fff
    classDef result fill:#FF5722,stroke:#D84315,stroke-width:2px,color:#fff
    
    class A,W,X startEnd
    class B decision
    class C,D,E,F pedestrian
    class G,H,I,J,T vehicle
    class K,L,M,N,O,P,Q,R system
    class S,U,V result
```

## 🚑 Emergency Mode System Flowchart

```mermaid
flowchart TD
    A[🚑 Emergency Trigger<br/>M Key Press] --> B[🚨 Set Emergency Mode = true]
    B --> C[⏰ Set Emergency Timer = 15]
    C --> D[🔄 Enter Emergency Update Cycle]
    
    D --> E[⏱️ Decrement Emergency Timer]
    E --> F{⏰ Timer ≤ 0?}
    F -->|❌ No| G[🔴 Display Emergency State<br/>Red Warning Text]
    F -->|✅ Yes| H[🔄 Exit Emergency Mode]
    
    G --> I[🚨 Flash Emergency Background<br/>Red Semi-transparent]
    I --> J[📝 Show 'EMERGENCY VEHICLE PASSING']
    J --> K[⏱️ Show Emergency Timer]
    K --> L[🔄 Continue Emergency Cycle]
    L --> E
    
    H --> M[🔄 Set Emergency Mode = false]
    M --> N[⏰ Set Regular Timer = 5]
    N --> O[🟢 Resume Normal State Machine]
    O --> P[✅ Emergency Complete]
    
    %% Override Capabilities
    Q[🚑 Emergency Features] --> R[🚫 Override Current State<br/>Ignore Normal Timing]
    R --> S[🟢 All Directions Get RED<br/>Clear Intersection]
    S --> T[⚡ Priority Vehicle Passage<br/>15 Second Window]
    T --> U[🔄 Return to Previous State<br/>Resume Normal Operation]

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef emergency fill:#F44336,stroke:#C62828,stroke-width:3px,color:#fff
    classDef display fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef system fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    
    class A,P startEnd
    class B,C,E,H,M,N,Q,R,S,T,U process
    class F decision
    class D,G,I,J,K,L emergency
    class O system
```

## 📊 Statistics & Monitoring Flowchart

```mermaid
flowchart TD
    A[📊 Statistics System] --> B[⏰ High-Precision Timer<br/>sf::Clock total_time_clock]
    B --> C[📈 Vehicle Statistics Tracking]
    C --> D[🚶‍♂️ Pedestrian Statistics Tracking]
    
    D --> E[🚦 State Change Monitoring]
    E --> F{🔄 State Transition?}
    F -->|🟡→🟢 NS_YELLOW to EW_GREEN| G[📊 Count EW Vehicles Passed<br/>stats_vehicles_passed += EW count]
    F -->|🟡→🟢 EW_YELLOW to NS_GREEN| H[📊 Count NS Vehicles Passed<br/>stats_vehicles_passed += NS count]
    F -->|🟢→🟡 Green to Yellow| I[⏭️ No Vehicle Counting]
    
    G --> J[🧹 Clear EW Vehicle Arrays<br/>vehicles[EAST/WEST].clear()]
    H --> K[🧹 Clear NS Vehicle Arrays<br/>vehicles[NORTH/SOUTH].clear()]
    I --> L[🚶‍♂️ Pedestrian Request Processing]
    J --> L
    K --> L
    
    L --> M{🚶‍♂️ Walk Signal Activated?}
    M -->|✅ Yes| N[📈 Increment Pedestrians Crossed<br/>stats_pedestrians_crossed++]
    M -->|❌ No| O[📊 Real-time Display Update]
    N --> O
    
    O --> P[📺 Format Display Strings]
    P --> Q[🕐 Format Elapsed Time<br/>MM:SS format]
    Q --> R[📊 Vehicle Count: 'Vehicles: X']
    R --> S[🚶‍♂️ Pedestrian Count: 'Pedestrians: Y']
    S --> T[⏱️ Runtime: 'Time: MM:SS']
    T --> U[🎨 Render Statistics Panel]
    
    U --> V[📋 Settings Display]
    V --> W[🧠 Adaptive Status: 'Adaptive: ON/OFF']
    W --> X[🤖 Auto Vehicles: 'Auto Vehicles: ON/OFF']
    X --> Y[⚡ Speed Factor: 'Speed: X.Xx']
    Y --> Z[✅ Statistics Complete]

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef decision fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef stats fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef display fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    classDef data fill:#8BC34A,stroke:#558B2F,stroke-width:2px,color:#fff
    
    class A,Z startEnd
    class B,C,D,E,G,H,J,K,N,P,Q,R,S,T,V,W,X,Y process
    class F,M decision
    class I,L,O,U stats
    class display
```

## 🔄 System Reset Flowchart

```mermaid
flowchart TD
    A[🔄 Reset System Called<br/>R Key Press] --> B[🚦 Set State = NS_GREEN]
    B --> C[⏰ Set Timer = 30 seconds]
    C --> D[⏰ Set Original Green Time = 30]
    D --> E[🧠 Enable Adaptive Mode = true]
    E --> F[⚡ Set Simulation Speed = 1.0x]
    F --> G[🤖 Enable Auto Vehicles = true]
    G --> H[🚑 Disable Emergency Mode = false]
    H --> I[⏰ Reset Emergency Timer = 0]
    
    I --> J[📊 Reset Statistics Counters]
    J --> K[🚗 Reset Vehicles Passed = 0]
    K --> L[🚶‍♂️ Reset Pedestrians Crossed = 0]
    L --> M[🕐 Reset Start Time = current time]
    M --> N[⏰ Restart High-Precision Clock]
    
    N --> O[🔄 Clear All Direction Arrays]
    O --> P[🚫 Reset Walk Signals[4] = false]
    P --> Q[🚫 Reset Requests[4] = false]
    Q --> R[📊 Reset Vehicle Counts[4] = 0]
    R --> S[🧹 Clear Vehicle Vectors[4]]
    
    S --> T[📋 Reset Config Menu = false]
    T --> U[🎯 Reset Layout = STANDARD_4WAY]
    U --> V[✅ System Reset Complete]
    V --> W[🔄 Return to Main Loop]

    %% Styling
    classDef startEnd fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef process fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef reset fill:#FF5722,stroke:#D84315,stroke-width:2px,color:#fff
    classDef data fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef system fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    
    class A,V,W startEnd
    class B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U reset
```

## 🎮 Complete System Data Flow

```mermaid
flowchart LR
    %% Input Sources
    A[⌨️ Keyboard Input] --> B[🎮 Event Handler]
    C[⏰ Simulation Timer] --> D[📊 Update Engine]
    E[🎲 Random Generator] --> F[🤖 Auto Vehicle System]
    
    %% Core Processing
    B --> G[🧠 Traffic System Core]
    D --> G
    F --> G
    
    G --> H[🚦 State Machine]
    G --> I[🚗 Vehicle Manager]
    G --> J[🚶‍♂️ Pedestrian Controller]
    G --> K[📊 Statistics Tracker]
    
    %% State Management
    H --> L[🟢 NS_GREEN<br/>20-60 seconds]
    H --> M[🟡 NS_YELLOW<br/>5 seconds]
    H --> N[🟢 EW_GREEN<br/>20-60 seconds]
    H --> O[🟡 EW_YELLOW<br/>5 seconds]
    
    %% Data Storage
    I --> P[(🗄️ Vehicle Arrays<br/>4 directions × 8 vehicles)]
    J --> Q[(🗄️ Walk Signals<br/>4 boolean flags)]
    K --> R[(📈 Performance Data<br/>Counters & Timers)]
    
    %% Rendering Pipeline
    L --> S[🎨 Graphics Renderer]
    M --> S
    N --> S
    O --> S
    P --> S
    Q --> S
    R --> S
    
    S --> T[🛣️ Road Renderer]
    S --> U[🚦 Light Renderer]
    S --> V[🚗 Vehicle Renderer]
    S --> W[🚶‍♂️ Pedestrian Renderer]
    S --> X[📊 UI Renderer]
    
    %% Output Display
    T --> Y[🖥️ SFML Window<br/>1200×980 pixels]
    U --> Y
    V --> Y
    W --> Y
    X --> Y
    
    Y --> Z[👀 Visual Output<br/>60 FPS Display]

    %% Styling
    classDef input fill:#4CAF50,stroke:#2E7D32,stroke-width:3px,color:#fff
    classDef core fill:#2196F3,stroke:#1565C0,stroke-width:2px,color:#fff
    classDef state fill:#FF9800,stroke:#E65100,stroke-width:2px,color:#fff
    classDef data fill:#9C27B0,stroke:#6A1B9A,stroke-width:2px,color:#fff
    classDef render fill:#00BCD4,stroke:#00838F,stroke-width:2px,color:#fff
    classDef output fill:#8BC34A,stroke:#558B2F,stroke-width:3px,color:#fff
    
    class A,C,E input
    class B,D,F,G core
    class H,I,J,K,L,M,N,O state
    class P,Q,R data
    class S,T,U,V,W,X render
    class Y,Z output
```

## 🏗️ Class Structure & Memory Layout

```mermaid
classDiagram
    class TrafficSystem {
        🚦 TrafficState current_state
        ⏰ int timer
        ⏰ int original_green_time
        🚶‍♂️ bool walk_signals[4]
        🚶‍♂️ bool requests[4]
        📊 int vehicle_counts[4]
        🚗 vector~Vehicle~ vehicles[4]
        🧠 bool adaptive_mode
        ⚡ float simulation_speed
        🤖 bool auto_vehicles
        🚑 bool emergency_mode
        ⏰ int emergency_timer
        📈 int stats_vehicles_passed
        📈 int stats_pedestrians_crossed
        🕐 time_t start_time
        ⏰ sf::Clock total_time_clock
        📋 LayoutType current_layout
        📋 bool show_config_menu
        
        +🔄 reset()
        +📊 update()
        +🚶‍♂️ request_crossing(Direction)
        +🚗 add_vehicle(Direction)
        +🧠 toggle_adaptive_mode()
        +🤖 toggle_auto_vehicles()
        +⚡ speed_up()
        +⚡ slow_down()
        +🚑 trigger_emergency()
        +📋 toggle_config_menu()
        +📊 get_timer()
        +🚦 get_state()
        +🚶‍♂️ get_walk_signal(Direction)
        +🚗 get_vehicle_count(Direction)
        +🧮 calculate_green_time(bool)
        +🚫 reset_walk_signals(bool)
    }
    
    class Vehicle {
        🚗 VehicleType type
        📍 float position
        +🏗️ Vehicle(VehicleType, float)
    }
    
    class SFMLComponents {
        🖥️ RenderWindow window
        🔤 Font font
        ⏰ Clock simulationClock
        🎨 Various Shapes & Text
    }
    
    TrafficSystem --> Vehicle : contains 4×vectors
    TrafficSystem --> SFMLComponents : uses for rendering
    
    %% Enums
    class TrafficState {
        <<enumeration>>
        🟢 NS_GREEN
        🟡 NS_YELLOW  
        🟢 EW_GREEN
        🟡 EW_YELLOW
    }
    
    class Direction {
        <<enumeration>>
        ⬆️ NORTH
        ⬇️ SOUTH
        ➡️ EAST
        ⬅️ WEST
    }
    
    class VehicleType {
        <<enumeration>>
        🚗 CAR
        🚛 TRUCK
        🚌 BUS
        🏍️ MOTORCYCLE
    }
    
    TrafficSystem --> TrafficState : uses
    TrafficSystem --> Direction : uses
    Vehicle --> VehicleType : uses
```
