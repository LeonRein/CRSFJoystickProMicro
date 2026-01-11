#ifndef CALIBRATION
#define CALIBRATION

#define AXIS_LOW_EP 1000
#define AXIS_HIGH_EP 2000



#define CHANNEL_AUX_SW_MID 1510
#define CHANNEL_AUX_SW_HIGH 2011

typedef struct axis_config
{
    byte channel;
    byte id;
    int32_t low_ep;
    int32_t high_ep;
} axis_config;

#define NUM_AXIS 8
axis_config axis_map[NUM_AXIS] = {
    {1, 0, AXIS_LOW_EP, AXIS_HIGH_EP}, // A
    {2, 1, AXIS_LOW_EP, AXIS_HIGH_EP}, // E
    {3, 2, AXIS_LOW_EP, AXIS_HIGH_EP}, // T
    {4, 3, AXIS_LOW_EP, AXIS_HIGH_EP}, // R
    {5, 4, AXIS_LOW_EP, AXIS_HIGH_EP}, // Aux 1
    {6, 5, AXIS_LOW_EP, AXIS_HIGH_EP}, // Aux 2
    {7, 6, AXIS_LOW_EP, AXIS_HIGH_EP}, // Aux 3
    {8, 7, AXIS_LOW_EP, AXIS_HIGH_EP}  // Aux 4
};

// upper bound and lower bound constrain where button reports HIGH and is inclusive
// Typically use MID / HIGH
typedef struct btn_config
{
    byte channel;
    byte id;
    int lower_bound;
    int upper_bound;
    bool invert;
} btn_config;

#define NUM_BUTTONS 8
btn_config btn_map[NUM_BUTTONS] = {
    {
        5,                   // channel
        0,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        6,                   // channel
        1,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        7,                   // channel
        2,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        8,                   // channel
        3,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        9,                   // channel
        4,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        10,                  // channel
        5,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        11,                  // channel
        6,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        false                // invert
    },

    {
        12,                  // channel
        7,                   // id
        CHANNEL_AUX_SW_MID,  // lower bound
        CHANNEL_AUX_SW_HIGH, // upper bound
        true                 // invert  - EX FOR INVERT
    }};

// Channel1 - A
// Channel2 - E
// Channel3 - T
// Channel4 - R
// Channel5 - Aux 1
// Channel6 - Aux 2
// Channel7 - Aux 3
// Channel8 - Aux 4
// Channel9 - Aux 5
// Channel10 - Aux 6
// Channel11 - Aux 7
// Channel12 - Aux 8
// Channel13
// Channel14
// Channel15 - LQ
// Channel16 - RSSI

#endif
