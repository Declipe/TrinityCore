#include "BfWGGameObjectBuilding.h"
#include "Battleground.h"
#include "GameObject.h"
#include "Log.h"
#include "World.h"
#include "WorldStatePackets.h"

uint8 const WG_MAX_ATTACKTOWERS = 3;
uint8 const WG_MAX_TOWER = 7;
uint8 const WG_MAX_OBJ = 32;

StaticWintergraspTowerInfo const TowerData[WG_MAX_TOWER] =
{
    { BATTLEFIELD_WG_TOWER_FORTRESS_NW,   { BATTLEFIELD_WG_TEXT_NW_KEEPTOWER_DAMAGE,   BATTLEFIELD_WG_TEXT_NW_KEEPTOWER_DESTROY   } },
    { BATTLEFIELD_WG_TOWER_FORTRESS_SW,   { BATTLEFIELD_WG_TEXT_SW_KEEPTOWER_DAMAGE,   BATTLEFIELD_WG_TEXT_SW_KEEPTOWER_DESTROY   } },
    { BATTLEFIELD_WG_TOWER_FORTRESS_SE,   { BATTLEFIELD_WG_TEXT_SE_KEEPTOWER_DAMAGE,   BATTLEFIELD_WG_TEXT_SE_KEEPTOWER_DESTROY   } },
    { BATTLEFIELD_WG_TOWER_FORTRESS_NE,   { BATTLEFIELD_WG_TEXT_NE_KEEPTOWER_DAMAGE,   BATTLEFIELD_WG_TEXT_NE_KEEPTOWER_DESTROY   } },
    { BATTLEFIELD_WG_TOWER_SHADOWSIGHT,   { BATTLEFIELD_WG_TEXT_WESTERN_TOWER_DAMAGE,  BATTLEFIELD_WG_TEXT_WESTERN_TOWER_DESTROY  } },
    { BATTLEFIELD_WG_TOWER_WINTER_S_EDGE, { BATTLEFIELD_WG_TEXT_SOUTHERN_TOWER_DAMAGE, BATTLEFIELD_WG_TEXT_SOUTHERN_TOWER_DESTROY } },
    { BATTLEFIELD_WG_TOWER_FLAMEWATCH,    { BATTLEFIELD_WG_TEXT_EASTERN_TOWER_DAMAGE,  BATTLEFIELD_WG_TEXT_EASTERN_TOWER_DESTROY  } }
};

WintergraspTowerCannonData const TowerCannon[WG_MAX_TOWER] =
{
    {
        // NW
        190221,
        {
            { 5364.081543f, 2923.851562f, 421.708313f, 1.682360f },
            { 5322.998535f, 2923.642334f, 421.646881f, 1.552756f }
        },
        {
            { 5255.88f, 3047.63f, 438.499f, 3.13677f },
            { 5280.90f, 3071.32f, 438.499f, 1.62879f },
            // south side
            { 5266.245605f, 2976.668945f, 421.066528f, 3.038350f },
            // north side
            { 5352.319336f, 3061.093994f, 421.101410f, 1.601107f },
            { 5391.208496f, 3061.543945f, 419.617065f, 1.703184f }
        }
    },
    {
        // SW
        190373,
        {
            // no cannons at bottom
        },
        {
            { 5138.59f, 2935.16f, 439.845f, 3.11723f },
            { 5163.06f, 2959.52f, 439.846f, 1.47258f },
            // south side
            { 5148.086426f, 2862.041748f, 421.630615f, 3.034440f },
            // north side
            { 5234.837891f, 2948.438721f, 420.880768f, 1.624634f },
            // inside gate
            { 5264.302246f, 2861.482666f, 421.585876f, 3.131782f }
        }
    },
    {
        // SE
        190377,
        {
            // no cannons at bottom
        },
        {
            { 5163.945312f, 2722.740215f, 439.844727f, 4.802916f },
            { 5139.69f, 2747.40f, 439.844f, 3.17221f },
            // north side
            { 5236.270508f, 2733.033203f, 421.649200f, 4.539250f },
            // south side
            { 5149.091797f, 2820.385254f, 421.620453f, 3.242571f },
            // inside gate
            { 5265.062988f, 2819.833740f, 421.655670f, 3.241718f }
        }
    },
    {
        // NE
        190378,
        {
            { 5363.749512f, 2756.457275f, 421.629364f, 4.651138f },
            { 5322.226562f, 2756.932129f, 421.645691f, 4.706134f }
        },
        {
            { 5278.21f, 2607.23f, 439.755f, 4.71944f },
            { 5255.01f, 2631.98f, 439.755f, 3.15257f },
            // north side
            { 5350.896973f, 2616.032471f, 421.242920f, 4.649218f },
            { 5390.979980f, 2615.694824f, 421.125641f, 4.543185f },
            // south
            { 5264.946289f, 2704.760254f, 421.700623f, 3.086273f }
        }
    },
    {
        // WEST GO_WINTERGRASP_SHADOWSIGHT_TOWER
        190356,
        {
            { 4582.242676f, 3603.601318f, 402.89f, 5.617138f },
            { 4536.583008f, 3598.400391f, 402.89f, 3.999214f },
            { 4577.682129f, 3649.093506f, 402.89f, 0.896894f },
            { 4532.182617f, 3644.367188f, 402.89f, 2.448015f }
        },
        {
            { 4559.875488f, 3598.626953f, 426.539185f, 4.813988f },
            { 4581.895996f, 3626.438477f, 426.539062f, 0.117806f },
            { 4532.264648f, 3621.135254f, 426.539093f, 3.263357f },
            { 4554.439941f, 3648.943604f, 426.539703f, 1.692559f }
        }
    },
    {
        // SOUTH GO_WINTERGRASP_WINTER_S_EDGE_TOWER
        190357,
        {
            { 4421.640137f, 2799.935791f, 412.630920f, 5.459298f },
            { 4420.263184f, 2845.340332f, 412.630951f, 0.742197f },
            { 4375.524902f, 2798.990723f, 412.630951f, 4.022082f },
            { 4374.833008f, 2845.260010f, 412.630890f, 2.333508f }
        },
        {
            { 4423.430664f, 2822.762939f, 436.283142f, 6.223487f },
            { 4397.825684f, 2847.629639f, 436.283325f, 1.579430f },
            { 4398.814941f, 2797.266357f, 436.283051f, 4.703747f },
            { 4373.095215f, 2822.046143f, 436.283173f, 3.098424f }
        }
    },
    {
        // EAST GO_WINTERGRASP_FLAMEWATCH_TOWER
        190358,
        {
            { 4448.138184f, 1974.998779f, 441.995911f, 1.967238f },
            { 4489.559082f, 1955.785767f, 441.995453f, 0.252363f },
            { 4470.388184f, 1913.816284f, 441.995544f, 4.929419f },
            { 4428.538086f, 1933.141724f, 441.995148f, 3.515702f }
        },
        {
            { 4469.448242f, 1966.623779f, 465.647217f, 1.153573f },
            { 4481.996582f, 1933.658325f, 465.647186f, 5.873029f },
            { 4448.316406f, 1921.195557f, 465.647247f, 4.301141f },
            { 4436.086914f, 1954.907715f, 465.647064f, 2.738204f }
        }
    }
};

uint8 const WG_MAX_DEFENDERS = 58;
WintergraspGameObjectData const WGPortalAndDefenderData[WG_MAX_DEFENDERS] =
{
    // Player teleporter
    //                  Position                                QuaternionData                Horde    Alliance 
    { { 5153.408f, 2901.349f, 409.1913f, -0.06981169f }, { 0.f, 0.f, -0.03489876f, 0.9993908f }, 190763, 191575 },
    { { 5268.698f, 2666.421f, 409.0985f, -0.71558490f }, { 0.f, 0.f, -0.35020730f, 0.9366722f }, 190763, 191575 },
    { { 5197.050f, 2944.814f, 409.1913f,  2.33874000f }, { 0.f, 0.f,  0.92050460f, 0.3907318f }, 190763, 191575 },
    { { 5196.671f, 2737.345f, 409.1892f, -2.93213900f }, { 0.f, 0.f, -0.99452110f, 0.1045355f }, 190763, 191575 },
    { { 5314.580f, 3055.852f, 408.8620f,  0.54105060f }, { 0.f, 0.f,  0.26723770f, 0.9636307f }, 190763, 191575 },
    { { 5391.277f, 2828.094f, 418.6752f, -2.16420600f }, { 0.f, 0.f, -0.88294700f, 0.4694727f }, 190763, 191575 },
    { { 5153.931f, 2781.671f, 409.2455f,  1.65806200f }, { 0.f, 0.f,  0.73727700f, 0.6755905f }, 190763, 191575 },
    { { 5311.445f, 2618.931f, 409.0916f, -2.37364400f }, { 0.f, 0.f, -0.92718320f, 0.3746083f }, 190763, 191575 },
    { { 5269.208f, 3013.838f, 408.8276f, -1.76278200f }, { 0.f, 0.f, -0.77162460f, 0.6360782f }, 190763, 191575 },

    { { 5401.634f, 2853.667f, 418.6748f,  2.63544400f }, { 0.f, 0.f,  0.96814730f, 0.2503814f }, 192819, 192819 }, // return portal inside fortress, neutral
    // Vehicle teleporter
    { { 5314.515f, 2703.687f, 408.5502f, -0.89011660f }, { 0.f, 0.f, -0.43051050f, 0.9025856f }, 192951, 192951 },
    { { 5316.252f, 2977.042f, 408.5385f, -0.82030330f }, { 0.f, 0.f, -0.39874840f, 0.9170604f }, 192951, 192951 },

    // NE
    { { 5350.9f, 2622.48f, 444.649f, -1.56207f }, { 0.f, 0.f, -0.704015f, 0.710185f }, 192364, 192314 },            //  big flag
    { { 5278.43f, 2613.83f, 433.294f, -1.62316f }, { 0.f, 0.f, -0.725376f, 0.688353f }, 192339, 192487 },           //  the biggest flag
    { { 5260.82f, 2631.82f, 433.181f, 3.12412f }, { 0.f, 0.f, 0.999962f, 0.00873622f }, 192339, 192487 },           //  the biggest flag
    { { 5271.63f, 2704.83f, 445.183f, -3.12412f }, { 0.f, 0.f, -0.999962f, 0.00873622f }, 192367, 192313 },         //  big flag
    // inside
    { { 5392.28f, 2639.84f, 435.208f, 1.52716f }, { 0.f, 0.f, 0.691512f, 0.722365f }, 192370, 192333 },             //  big flag
    { { 5350.94f, 2640.43f, 435.264f, 1.56207f }, { 0.f, 0.f, 0.704015f, 0.710185f }, 192369, 192332 },             //  big flag
    { { 5289.782f, 2704.62f, 435.714f, 0.00872424f }, { 0.f, 0.f, 0.00436211f, 0.99999f }, 192368, 192331 },        //  big flag
    { { 5322.17f, 2763.2f, 444.974f, -1.56207f }, { 0.f, 0.f, -0.704015f, 0.710185f }, 192362, 192334 },            //  big flag
    { { 5363.72f, 2763.25f, 445.023f, -1.54462f }, { 0.f, 0.f, -0.697792f, 0.716301f }, 192363, 192335 },

    // SE
    { { 5236.31f, 2739.22f, 444.993f, -1.6057f }, { 0.f, 0.f, -0.719339f, 0.694659f }, 192363, 192312 },            //  big flag
    { { 5163.85f, 2729.68f, 433.328f, -1.6057f }, { 0.f, 0.f, -0.719339f, 0.694659f }, 192339, 192487 },            //  the biggest flag
    { { 5146.04f, 2747.3f, 433.527f, 3.12412f }, { 0.f, 0.f, 0.999962f, 0.00873622f }, 192339, 192487 },            //  the biggest flag
    { { 5160.28f, 2798.6f, 430.604f, -3.12412f }, { 0.f, 0.f, -0.999962f, 0.00873464f }, 192339, 192487 },          //  the biggest flag
    { { 5155.22f, 2820.63f, 444.979f, -3.11539f }, { 0.f, 0.f, -0.999914f, 0.0131009f }, 192349, 192299 },          //  big flag
    { { 5154.42f, 2828.93f, 409.189f, 3.14159f }, { 0.f, 0.f, 1.0f, 0.00000127f }, 192254, 192253 },                // flagpole
    // inside
    { { 5173.13f, 2820.96f, 435.658f, 0.0261791f }, { 0.f, 0.f, 0.0130892f, 0.999914f }, 192352, 192328 },          // big flag
    { { 5237.02f, 2757.36f, 435.626f, 1.55334f }, { 0.f, 0.f, 0.700908f, 0.713252f }, 192356, 192330 },             // big flag
    { { 5271.16f, 2820.11f, 445.109f, -3.13286f }, { 0.f, 0.f, -0.99999f, 0.00436634f }, 192351, 192307 },          // big flag

    // SW
    { { 5154.37f, 2853.23f, 409.183f, 3.14159f }, { 0.f, 0.f, 1.0f, 0.00000127f }, 192255, 192252 },                // flagpole
    { { 5154.35f, 2862.08f, 445.01f, 3.14159f }, { 0.f, 0.f, 1.0f, 0.00000127f }, 192336, 192292 },                 // big flag
    { { 5158.71f, 2882.9f, 431.274f, 3.14159f }, { 0.f, 0.f, -1.0f, 0.0f }, 192339, 192487 },                       //  the biggest flag
    { { 5145.11f, 2934.95f, 433.255f, -3.10665f }, { 0.f, 0.f, -0.999847f, 0.0174704f }, 192339, 192487 },          //  the biggest flag
    { { 5162.91f, 2952.6f, 433.368f, 1.5708f }, { 0.f, 0.f, 0.707108f, 0.707106f }, 192339, 192487 },               //  the biggest flag
    { { 5235.13f, 2942.12f, 444.279f, 1.58825f }, { 0.f, 0.f, 0.713251f, 0.700909f }, 192375, 192308 },             // big flag
    // inside
    { { 5172.34f, 2862.58f, 435.658f, 0.0f }, { 0.f, 0.f, 0.0f, 1.0f }, 192353, 192329 },                           // big flag
    { { 5235.32f, 2924.31f, 434.898f, -1.56207f }, { 0.f, 0.f, -0.704015f, 0.710185f }, 192357, 192329 },           // big flag
    { { 5270.55f, 2861.68f, 444.917f, -3.12412f }, { 0.f, 0.f, -0.999962f, 0.00873622f }, 192350, 192306 },         // big flag

    // NW
    { { 5272.55f, 2976.54f, 444.493f, 3.13286f }, { 0.f, 0.f, 0.99999f, 0.00436634f }, 192374, 192309 },            // big flag
    { { 5262.54f, 3047.93f, 431.965f, 3.12412f }, { 0.f, 0.f, 0.999962f, 0.00873622f }, 192339, 192487 },           //  the biggest flag
    { { 5280.89f, 3064.95f, 431.976f, 1.55334f }, { 0.f, 0.f, 0.700908f, 0.713252f }, 192339, 192487 },             //  the biggest flag
    { { 5352.2f, 3055.02f, 444.565f, 1.57952f }, { 0.f, 0.f, 0.710184f, 0.704016f }, 192376, 192310 },              // big flag
    // inside
    { { 5392.64f, 3036.97f, 433.649f, -1.51843f }, { 0.f, 0.f, -0.688352f, 0.725377f }, 192361, 192326 },           // big flag
    { { 5352.38f, 3036.95f, 435.111f, -1.56207f }, { 0.f, 0.f, -0.704015f, 0.710185f }, 192360, 192325 },           // big flag
    { { 5290.51f, 2976.56f, 435.087f, 0.00872424f }, { 0.f, 0.f, 0.00436211f, 0.99999f }, 192373, 192324 },         // big flag
    { { 5322.89f, 2917.14f, 445.154f, 1.56207f }, { 0.f, 0.f, 0.704015f, 0.710185f }, 192372, 192322 },             // big flag
    { { 5364.28f, 2917.26f, 445.332f, 1.58825f }, { 0.f, 0.f, 0.713251f, 0.700909f }, 192371, 192323 },             // big flag

    // keep inside
    // right
    { { 5371.45f, 2820.79f, 409.427f, 3.12412f }, { 0.f, 0.f, 0.999962f, 0.00873622f }, 192285, 192286 },           // flagpole
    { { 5397.31f, 2809.26f, 455.102f, 3.13286f }, { 0.f, 0.f, 0.99999f, 0.00436634f }, 192339, 192305 },            //  the biggest flag
    { { 5363.39f, 2781.28f, 435.634f, 1.58825f }, { 0.f, 0.f, 0.713251f, 0.700909f }, 192379, 192317 },             // big flag
    { { 5322.01f, 2781.13f, 435.673f, 1.57952f }, { 0.f, 0.f, 0.710184f, 0.704016f }, 192378, 192316 },             // big flag
    { { 5289.05f, 2820.23f, 435.674f, 0.0f }, { 0.f, 0.f, 0.0f, 1.0f }, 192355, 192320 },                           // big flag
    // left
    { { 5288.85f, 2861.82f, 435.59f, 0.0261791f }, { 0.f, 0.f, 0.0130892f, 0.999914f }, 192354, 192321 },           // big flag
    { { 5322.25f, 2898.95f, 435.643f, -1.57952f }, { 0.f, 0.f, -0.710184f, 0.704016f }, 192358, 192318 },           // big flag
    { { 5364.3f, 2899.22f, 435.691f, -1.55334f }, { 0.f, 0.f, -0.700908f, 0.713252f }, 192359, 192319 },            // big flag
    { { 5398.04f, 2873.01f, 455.204f, 3.13286f }, { 0.f, 0.f, 0.99999f, 0.00436634f }, 192338, 192304 },            //  the biggest flag
    { { 5372.42f, 2862.48f, 409.366f, 3.14159f }, { 0.f, 0.f, 1.0f, 0.00000127f }, 192284, 192287 }                 // flagpole
};

// 192414 : 0 in sql, 1 in header
// 192278 : 0 in sql, 3 in header
WintergraspGobjectWithCreatureData const GobjectWithCreatureData[WG_MAX_ATTACKTOWERS] =
{
    // West tower
    {
        190356,
        {
        //                  Position                                QuaternionData                 Horde   Alliance
        { { 4559.113f, 3606.216f, 419.9992f, 4.799657f }, { 0.f, 0.f, -0.67558960f, 0.73727790f }, 192488, 192501 },    // Flag on tower
        { { 4539.420f, 3622.490f, 420.0342f, 3.211419f }, { 0.f, 0.f, -0.99939060f, 0.03490613f }, 192488, 192501 },    // Flag on tower
        { { 4555.258f, 3641.648f, 419.9740f, 1.675514f }, { 0.f, 0.f,  0.74314400f, 0.66913150f }, 192488, 192501 },    // Flag on tower
        { { 4574.872f, 3625.911f, 420.0792f, 0.087266f }, { 0.f, 0.f,  0.04361916f, 0.99904820f }, 192488, 192501 },    // Flag on tower
        { { 4433.899f, 3534.142f, 360.2750f, 4.433136f }, { 0.f, 0.f, -0.79863550f, 0.60181500f }, 192271, 192278 },    // Flag near workshop
        { { 4572.933f, 3475.519f, 363.0090f, 1.422443f }, { 0.f, 0.f,  0.65275960f, 0.75756520f }, 192271, 192277 }     // Flag near bridge
        },
        {
            { { 4418.688477f, 3506.251709f, 358.975494f,  4.293305f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Roaming Guard
            { { 4412.451660f, 3510.044189f, 358.937592f,  4.293305f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Roaming Guard
            { { 4507.348633f, 3626.127441f, 392.607635f,  3.432202f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Roaming Guard
            { { 4558.034180f, 3577.825439f, 393.005157f,  4.453204f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Roaming Guard
            { { 4567.826660f, 3469.102051f, 361.778168f,  4.935128f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },      // Roaming Guard
            { { 4549.113281f, 3472.346924f, 362.905762f,  4.699521f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }       // Roaming Guard
        }
    },
    // South Tower
    {
        190357,
        {
            { { 4416.004f, 2822.666f, 429.8512f, 6.2657330f }, { 0.f, 0.f, -0.00872612f, 0.99996190f }, 192488, 192501 },    // Flag on tower
            { { 4398.819f, 2804.698f, 429.7920f, 4.6949370f }, { 0.f, 0.f, -0.71325020f, 0.70090960f }, 192488, 192501 },    // Flag on tower
            { { 4380.36f, 2822.38f, 429.882f, -3.10665f }, { 0.f, 0.f, -0.999847f, 0.0174704f }, 192488, 192501 },           // Flag on tower
            { { 4397.66f, 2840.3f, 429.922f, 1.58825f }, { 0.f, 0.f, 0.713251f, 0.700909f }, 192488, 192501 },               // Flag on tower
            { { 4387.622f, 2719.566f, 389.9351f, 4.7385700f }, { 0.f, 0.f, -0.69779010f, 0.71630230f }, 192366, 192414 },    // Flag near tower (east)
            { { 4517.79f, 2717.07f, 387.57f, -1.54462f }, { 0.f, 0.f, -0.697792f, 0.716301f }, 192449, 192415 },             // Flag near tower (east)
            { { 4464.124f, 2855.453f, 406.1106f, 0.8290324f }, { 0.f, 0.f,  0.40274720f, 0.91531130f }, 192366, 192429 },    // Flag near tower (middle)
            { { 4434.56f, 2883.45f, 406.025f, 0.759216f }, { 0.f, 0.f,  0.370556f, 0.92881f }, 192443, 192430 },             // Flag near tower (middle)
            { { 4349.9f, 2885.56f, 406.105f, 1.6057f }, { 0.f, 0.f, 0.719339f, 0.694659f }, 192442, 192431 },                // Flag near tower (west)
            { { 4526.457f, 2810.181f, 391.1997f, 3.2899610f }, { 0.f, 0.f, -0.99724960f, 0.07411628f }, 192271, 192278 },    // flagpole near bridge
            { { 4424.71f, 2975.6f, 367.387f, 1.69297f }, { 0.f, 0.f, 0.748956f, 0.66262f }, 192271, 192270 },                // flagpole west
            { { 4452.8f, 2639.0f, 358.552f, 1.4748f }, { 0.f, 0.f, 0.672366f, 0.740219f }, 192267, 192266 }                  // flagpole east
        },
        {
            { { 4452.859863f, 2808.870117f, 402.604004f, 6.056290f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4455.899902f, 2835.958008f, 401.122559f, 0.034907f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4412.649414f, 2953.792236f, 374.799957f, 0.980838f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Roaming Guard
            { { 4362.089844f, 2811.510010f, 407.337006f, 3.193950f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4412.290039f, 2753.790039f, 401.015015f, 5.829400f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4421.939941f, 2773.189941f, 400.894989f, 5.707230f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4459.826660f, 2639.718262f, 360.344360f, 4.528321f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4475.140625f, 2639.593750f, 360.647461f, 4.673613f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4396.082031f, 2875.900146f, 399.638947f, 1.602748f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4530.568848f, 2817.606689f, 389.410126f, 0.267566f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4524.830566f, 2842.446777f, 389.558563f, 6.024540f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }      // Standing Guard
        }
    },
    // East Tower
    {
        190358,
        {
            { { 4466.793f, 1960.418f, 459.1437f, 1.151916f }, { 0.f, 0.f,  0.5446386f, 0.8386708f }, 192488, 192501 },     // Flag on tower
            { { 4475.351f, 1937.031f, 459.0702f, 5.846854f }, { 0.f, 0.f, -0.2164392f, 0.9762961f }, 192488, 192501 },     // Flag on tower
            { { 4451.758f, 1928.104f, 459.0759f, 4.276057f }, { 0.f, 0.f, -0.8433914f, 0.5372996f }, 192488, 192501 },     // Flag on tower
            { { 4442.987f, 1951.898f, 459.0930f, 2.740162f }, { 0.f, 0.f,  0.9799242f, 0.1993704f }, 192488, 192501 },     // Flag on tower
            { { 4563.73f, 2171.15f, 367.68f, 1.30027f }, { 0.f, 0.f,  0.605294f, 0.796002f }, 192424, 192423 }             // flagpole
        },
        {
            { { 4501.060059f, 1990.280029f, 431.157013f, 1.029740f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4463.830078f, 2015.180054f, 430.299988f, 1.431170f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4494.580078f, 1943.760010f, 435.627014f, 6.195920f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4450.149902f, 1897.579956f, 435.045013f, 4.398230f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4428.870117f, 1906.869995f, 432.648010f, 3.996800f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4557.801270f, 2171.374268f, 368.884583f, 1.114372f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { { 4539.870605f, 2177.921631f, 369.504761f, 1.475655f }, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }      // Standing Guard
        }
    }
};

WintergraspBuildingSpawnData const WGGameObjectBuilding[WG_MAX_OBJ] =
{
    // Wall (Not spawned in db)
    // Entry  WS      X          Y          Z           O                rX   rY   rZ             rW             Type
    { 190219, 3749, { 5371.457f, 3047.472f, 407.5710f,  3.14159300f }, { 0.f, 0.f, -1.000000000f, 0.00000000f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190220, 3750, { 5331.264f, 3047.105f, 407.9228f,  0.05235888f }, { 0.f, 0.f,  0.026176450f, 0.99965730f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191795, 3764, { 5385.841f, 2909.490f, 409.7127f,  0.00872424f }, { 0.f, 0.f,  0.004362106f, 0.99999050f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191796, 3772, { 5384.452f, 2771.835f, 410.2704f,  3.14159300f }, { 0.f, 0.f, -1.000000000f, 0.00000000f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191799, 3762, { 5371.436f, 2630.610f, 408.8163f,  3.13285800f }, { 0.f, 0.f,  0.999990500f, 0.00436732f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191800, 3766, { 5301.838f, 2909.089f, 409.8661f,  0.00872424f }, { 0.f, 0.f,  0.004362106f, 0.99999050f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191801, 3770, { 5301.063f, 2771.411f, 409.9014f,  3.14159300f }, { 0.f, 0.f, -1.000000000f, 0.00000000f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191802, 3751, { 5280.197f, 2995.583f, 408.8249f,  1.61442800f }, { 0.f, 0.f,  0.722363500f, 0.69151360f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191803, 3752, { 5279.136f, 2956.023f, 408.6041f,  1.57079600f }, { 0.f, 0.f,  0.707106600f, 0.70710690f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191804, 3767, { 5278.685f, 2882.513f, 409.5388f,  1.57079600f }, { 0.f, 0.f,  0.707106600f, 0.70710690f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191806, 3769, { 5279.502f, 2798.945f, 409.9983f,  1.57079600f }, { 0.f, 0.f,  0.707106600f, 0.70710690f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191807, 3759, { 5279.937f, 2724.766f, 409.9452f,  1.56207000f }, { 0.f, 0.f,  0.704014800f, 0.71018530f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191808, 3760, { 5279.601f, 2683.786f, 409.8488f,  1.55334100f }, { 0.f, 0.f,  0.700908700f, 0.71325110f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191809, 3761, { 5330.955f, 2630.777f, 409.2826f,  3.13285800f }, { 0.f, 0.f,  0.999990500f, 0.00436732f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190369, 3753, { 5256.085f, 2933.963f, 409.3571f,  3.13285800f }, { 0.f, 0.f,  0.999990500f, 0.00436732f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190370, 3758, { 5257.463f, 2747.327f, 409.7427f, -3.13285800f }, { 0.f, 0.f, -0.999990500f, 0.00436732f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190371, 3754, { 5214.960f, 2934.089f, 409.1905f, -0.00872424f }, { 0.f, 0.f, -0.004362106f, 0.99999050f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190372, 3757, { 5215.821f, 2747.566f, 409.1884f, -3.13285800f }, { 0.f, 0.f, -0.999990500f, 0.00436732f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190374, 3755, { 5162.273f, 2883.043f, 410.2556f,  1.57952200f }, { 0.f, 0.f,  0.710185100f, 0.70401500f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 190376, 3756, { 5163.724f, 2799.838f, 409.2270f,  1.57952200f }, { 0.f, 0.f,  0.710185100f, 0.70401500f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },

    // Tower of keep (Not spawned in db)
    { 190221, 3711, { 5281.154f, 3044.588f, 407.8434f,  3.115388f }, { 0.f, 0.f,  0.9999142f, 0.013101960f }, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER }, // NW
    { 190373, 3713, { 5163.757f, 2932.228f, 409.1904f,  3.124123f }, { 0.f, 0.f,  0.9999619f, 0.008734641f }, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER }, // SW
    { 190377, 3714, { 5166.397f, 2748.368f, 409.1884f, -1.570796f }, { 0.f, 0.f, -0.7071066f, 0.707106900f }, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER }, // SE
    { 190378, 3712, { 5281.192f, 2632.479f, 409.0985f, -1.588246f }, { 0.f, 0.f, -0.7132492f, 0.700910500f }, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER }, // NE

    // Wall (with passage) (Not spawned in db)
    { 191797, 3765, { 5343.290f, 2908.860f, 409.5757f, 0.00872424f }, { 0.f, 0.f,  0.004362106f, 0.9999905f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191798, 3771, { 5342.719f, 2771.386f, 409.6249f, 3.14159300f }, { 0.f, 0.f, -1.000000000f, 0.0000000f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },
    { 191805, 3768, { 5279.126f, 2840.797f, 409.7826f, 1.57952200f }, { 0.f, 0.f,  0.710185100f, 0.7040150f }, BATTLEFIELD_WG_OBJECTTYPE_WALL },

    // South tower (Not spawned in db)
    { 190356, 3704, { 4557.173f, 3623.943f, 395.8828f,  1.675516f }, { 0.f, 0.f,  0.7431450f, 0.669130400f }, BATTLEFIELD_WG_OBJECTTYPE_TOWER }, // W
    { 190357, 3705, { 4398.172f, 2822.497f, 405.6270f, -3.124123f }, { 0.f, 0.f, -0.9999619f, 0.008734641f }, BATTLEFIELD_WG_OBJECTTYPE_TOWER }, // S
    { 190358, 3706, { 4459.105f, 1944.326f, 434.9912f, -2.002762f }, { 0.f, 0.f, -0.8422165f, 0.539139500f }, BATTLEFIELD_WG_OBJECTTYPE_TOWER }, // E

    // Door of forteress (Not spawned in db)
    { GO_WINTERGRASP_FORTRESS_GATE, 3763, { 5162.991f, 2841.232f, 410.1892f, -3.132858f }, { 0.f, 0.f, -0.9999905f, 0.00436732f }, BATTLEFIELD_WG_OBJECTTYPE_DOOR },

    // Last door (Not spawned in db)
    { GO_WINTERGRASP_VAULT_GATE, 3773, { 5397.108f, 2841.54f, 425.9014f, 3.141593f }, { 0.f, 0.f, -1.f, 0.f }, BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST },
};

void BattlefieldWG::FillBuildings()
{
    m_BuildingsInZone.resize(WG_MAX_OBJ);
    // Spawn all gameobjects
    for (uint8 i = 0; i < WG_MAX_OBJ; i++)
    {
        if (GameObject* go = SpawnGameObject(WGGameObjectBuilding[i].entry, WGGameObjectBuilding[i].pos, WGGameObjectBuilding[i].rot))
        {
            BfWGGameObjectBuilding* b = new BfWGGameObjectBuilding(this, WGGameObjectBuilding[i].type, WGGameObjectBuilding[i].WorldState);
            b->Init(go);
            if (!IsEnabled() && go->GetEntry() == GO_WINTERGRASP_VAULT_GATE)
                go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);

            m_BuildingsInZone[i] = b;
        }
    }
}

void BattlefieldWG::SpawnPortalAndDefenders()
{
    // Spawning portal defender
    for (uint8 i = 0; i < WG_MAX_DEFENDERS; ++i)
    {
        WintergraspGameObjectData const& defendersGo = WGPortalAndDefenderData[i];
        if (GameObject* go = SpawnGameObject(defendersGo.AllianceEntry, defendersGo.Pos, defendersGo.Rot))
        {
            DefenderPortalList[TEAM_ALLIANCE].push_back(go->GetGUID());
            go->SetRespawnTime(GetDefenderTeam() == TEAM_ALLIANCE ? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        }

        if (GameObject* go = SpawnGameObject(defendersGo.HordeEntry, defendersGo.Pos, defendersGo.Rot))
        {
            DefenderPortalList[TEAM_HORDE].push_back(go->GetGUID());
            go->SetRespawnTime(GetDefenderTeam() == TEAM_HORDE ? RESPAWN_IMMEDIATELY : RESPAWN_ONE_DAY);
        }
    }
}

void BattlefieldWG::UpdateAttackers()
{
    // Rebuild all wall
    for (BfWGGameObjectBuilding* building : m_BuildingsInZone)
    {
        building->Rebuild();
        building->UpdateTurretAttack(false);
    }
}

void BattlefieldWG::UpdateDefenders()
{
    // Update portals
    for (auto itr = DefenderPortalList[GetDefenderTeam()].begin(); itr != DefenderPortalList[GetDefenderTeam()].end(); ++itr)
        if (GameObject* portal = GetGameObject(*itr))
            portal->SetRespawnTime(RESPAWN_IMMEDIATELY);

    for (auto itr = DefenderPortalList[GetAttackerTeam()].begin(); itr != DefenderPortalList[GetAttackerTeam()].end(); ++itr)
        if (GameObject* portal = GetGameObject(*itr))
            portal->SetRespawnTime(RESPAWN_ONE_DAY);
}

BfWGGameObjectBuilding::BfWGGameObjectBuilding(BattlefieldWG* wg, WintergraspGameObjectBuildingType type, uint32 worldState)
{
    ASSERT(wg);
    _wg = wg;
    _teamControl = TEAM_NEUTRAL;
    _type = type;
    _worldState = worldState;
    _state = BATTLEFIELD_WG_OBJECTSTATE_NONE;
    _staticTowerInfo = nullptr;
}

BfWGGameObjectBuilding::~BfWGGameObjectBuilding()
{
    _wg = nullptr;
    _buildGUID.Clear();
    _teamControl = TEAM_ALLIANCE;
    _type = BATTLEFIELD_WG_OBJECTTYPE_DOOR;
    _worldState = 0;
    _state = BATTLEFIELD_WG_OBJECTSTATE_NONE;
    _staticTowerInfo = nullptr;
}

void BfWGGameObjectBuilding::Rebuild()
{
    switch (_type)
    {
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
        case BATTLEFIELD_WG_OBJECTTYPE_WALL:
            _teamControl = _wg->GetDefenderTeam();      // Objects that are part of the keep should be the defender's
            break;
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
            _teamControl = _wg->GetAttackerTeam();      // The towers in the south should be the attacker's
            break;
        default:
            _teamControl = TEAM_NEUTRAL;
            break;
    }

    if (GameObject* build = _wg->GetGameObject(_buildGUID))
    {
        // Rebuild gameobject
        if (build->IsDestructibleBuilding())
        {
            build->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, nullptr, true);
            if (build->GetEntry() == GO_WINTERGRASP_VAULT_GATE)
                if (GameObject* go = build->FindNearestGameObject(GO_WINTERGRASP_KEEP_COLLISION_WALL, 50.0f))
                    go->SetGoState(GO_STATE_ACTIVE);

            // Update worldstate
            _state = WintergraspGameObjectState(BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT - (_teamControl * 3));
            _wg->SendUpdateWorldState(_worldState, _state);
        }
        UpdateCreatureAndGo();
        build->SetFaction(WintergraspFaction[_teamControl]);
    }
}

void BfWGGameObjectBuilding::RebuildGate()
{
    if (GameObject* build = _wg->GetGameObject(_buildGUID))
    {
        if (build->IsDestructibleBuilding() && build->GetEntry() == GO_WINTERGRASP_VAULT_GATE)
        {
            if (GameObject* go = build->FindNearestGameObject(GO_WINTERGRASP_KEEP_COLLISION_WALL, 50.0f))
                go->SetGoState(GO_STATE_READY); //not GO_STATE_ACTIVE
        }
    }
}

void BfWGGameObjectBuilding::Damaged()
{
    // Update worldstate
    _state = WintergraspGameObjectState(BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE - (_teamControl * 3));
    _wg->SendUpdateWorldState(_worldState, _state);

    // Send warning message
    if (_staticTowerInfo)
        _wg->SendWarning(_staticTowerInfo->TextIds.Damaged);

    for (ObjectGuid guid : m_TurretTopList)
        if (Creature* creature = _wg->GetCreature(guid))
            _wg->HideNpc(creature);

    if (_type == BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER)
        _wg->UpdateDamagedTowerCount(_wg->GetDefenderTeam());
    else if (_type == BATTLEFIELD_WG_OBJECTTYPE_TOWER)
        _wg->UpdateDamagedTowerCount(_wg->GetAttackerTeam());
}

void BfWGGameObjectBuilding::Destroyed()
{
    // Update worldstate
    _state = WintergraspGameObjectState(BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY - (_teamControl * 3));
    _wg->SendUpdateWorldState(_worldState, _state);

    // Warn players
    if (_staticTowerInfo)
        _wg->SendWarning(_staticTowerInfo->TextIds.Destroyed);

    switch (_type)
    {
        // Inform the global wintergrasp script of the destruction of this object
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
            _wg->UpdatedDestroyedTowerCount(_teamControl);
            break;
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
            if (GameObject* build = _wg->GetGameObject(_buildGUID))
                if (GameObject* go = build->FindNearestGameObject(GO_WINTERGRASP_KEEP_COLLISION_WALL, 50.0f))
                    go->SetGoState(GO_STATE_ACTIVE);
            _wg->SetRelicInteractible(true);
            if (_wg->GetRelic())
                _wg->GetRelic()->RemoveFlag(GO_FLAG_IN_USE | GO_FLAG_NOT_SELECTABLE);
            else
                TC_LOG_ERROR("bg.battlefield", "Titan Relic not found.");
            break;
        default:
            break;
    }

    _wg->BrokenWallOrTower(_teamControl, this);
}

void BfWGGameObjectBuilding::Init(GameObject* go)
{
    if (!go)
        return;

    // GameObject associated to object
    _buildGUID = go->GetGUID();

    switch (_type)
    {
        case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
        case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
        case BATTLEFIELD_WG_OBJECTTYPE_WALL:
            _teamControl = _wg->GetDefenderTeam();           // Objects that are part of the keep should be the defender's
            break;
        case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
            _teamControl = _wg->GetAttackerTeam();           // The towers in the south should be the attacker's
            break;
        default:
            _teamControl = TEAM_NEUTRAL;
            break;
    }

    //TODO loading from worldstate for restart setups? need it?
    //_state = WintergraspGameObjectState(sWorld->getWorldState(_worldState));
    switch (_teamControl)
    {
        case TEAM_ALLIANCE: _state = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT; break;
            case TEAM_HORDE: _state = BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT; break;
            case TEAM_NEUTRAL: _state = BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT; break;
            default:
                break;
    }

    switch (_state)
    {
        case BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT:
        case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT:
        case BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT:
            go->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, nullptr, true);
            break;
        case BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DESTROY:
        case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY:
        case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY:
            go->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
            break;
        case BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DAMAGE:
        case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE:
        case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DAMAGE:
            go->SetDestructibleState(GO_DESTRUCTIBLE_DAMAGED);
            break;
        default:
            break;
    }

    int32 towerId = -1;
    switch (go->GetEntry())
    {
        case GO_WINTERGRASP_FORTRESS_TOWER_1:
            towerId = BATTLEFIELD_WG_TOWER_FORTRESS_NW;
            break;
        case GO_WINTERGRASP_FORTRESS_TOWER_2:
            towerId = BATTLEFIELD_WG_TOWER_FORTRESS_SW;
            break;
        case GO_WINTERGRASP_FORTRESS_TOWER_3:
            towerId = BATTLEFIELD_WG_TOWER_FORTRESS_SE;
            break;
        case GO_WINTERGRASP_FORTRESS_TOWER_4:
            towerId = BATTLEFIELD_WG_TOWER_FORTRESS_NE;
            break;
        case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
            towerId = BATTLEFIELD_WG_TOWER_SHADOWSIGHT;
            break;
        case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
            towerId = BATTLEFIELD_WG_TOWER_WINTER_S_EDGE;
            break;
        case GO_WINTERGRASP_FLAMEWATCH_TOWER:
            towerId = BATTLEFIELD_WG_TOWER_FLAMEWATCH;
            break;
    }

    if (towerId >= BATTLEFIELD_WG_TOWER_SHADOWSIGHT) // Attacker towers
    {
        // Spawn associate gameobjects
        for (WintergraspGameObjectData const& gobData : GobjectWithCreatureData[towerId - 4].GameObject)
        {
            if (GameObject* goHorde = _wg->SpawnGameObject(gobData.HordeEntry, gobData.Pos, gobData.Rot))
                m_attackerGameObjects[TEAM_HORDE].push_back(goHorde->GetGUID());

            if (GameObject* goAlliance = _wg->SpawnGameObject(gobData.AllianceEntry, gobData.Pos, gobData.Rot))
                m_attackerGameObjects[TEAM_ALLIANCE].push_back(goAlliance->GetGUID());
        }

        // Spawn associate npc bottom
        for (WintergraspObjectPositionData const& creatureData : GobjectWithCreatureData[towerId - 4].CreatureBottom)
        {
            if (Creature* creature = _wg->SpawnCreature(creatureData.HordeEntry, creatureData.Pos))
            {
                m_attackerCreatures[TEAM_HORDE].push_back(creature->GetGUID());
                if (_wg->isCreatureGuard(creature))
                    creature->GetMotionMaster()->MoveRandom(25.f);
            }

            if (Creature* creature = _wg->SpawnCreature(creatureData.AllianceEntry, creatureData.Pos))
            {
                m_attackerCreatures[TEAM_ALLIANCE].push_back(creature->GetGUID());
                if (_wg->isCreatureGuard(creature))
                    creature->GetMotionMaster()->MoveRandom(25.f);
            }
        }
    }

    if (towerId >= 0)
    {
        ASSERT(towerId < WG_MAX_TOWER);
        _staticTowerInfo = &TowerData[towerId];

        // Spawn Turret bottom
        for (Position const& turretPos : TowerCannon[towerId].TowerCannonBottom)
        {
            if (Creature* turret = _wg->SpawnCreature(NPC_WINTERGRASP_TOWER_CANNON, turretPos))
            {
                m_TowerCannonBottomList.push_back(turret->GetGUID());
                switch (go->GetEntry())
                {
                    case GO_WINTERGRASP_FORTRESS_TOWER_1:
                    case GO_WINTERGRASP_FORTRESS_TOWER_2:
                    case GO_WINTERGRASP_FORTRESS_TOWER_3:
                    case GO_WINTERGRASP_FORTRESS_TOWER_4:
                        turret->SetFaction(WintergraspFaction[_wg->GetDefenderTeam()]);
                        break;
                    case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
                    case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
                    case GO_WINTERGRASP_FLAMEWATCH_TOWER:
                        turret->SetFaction(WintergraspFaction[_wg->GetAttackerTeam()]);
                        break;
                }

                switch (_state)
                {
                    case BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DESTROY:
                    case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY:
                    case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY:
                        _wg->HideNpc(turret);
                        break;
                    default:
                        break;
                }
            }
        }

        // Spawn Turret top
        for (Position const& towerCannonPos : TowerCannon[towerId].TurretTop)
        {
            if (Creature* turret = _wg->SpawnCreature(NPC_WINTERGRASP_TOWER_CANNON, towerCannonPos))
            {
                m_TurretTopList.push_back(turret->GetGUID());
                switch (go->GetEntry())
                {
                    case GO_WINTERGRASP_FORTRESS_TOWER_1:
                    case GO_WINTERGRASP_FORTRESS_TOWER_2:
                    case GO_WINTERGRASP_FORTRESS_TOWER_3:
                    case GO_WINTERGRASP_FORTRESS_TOWER_4:
                        turret->SetFaction(WintergraspFaction[_wg->GetDefenderTeam()]);
                        break;
                    case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
                    case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
                    case GO_WINTERGRASP_FLAMEWATCH_TOWER:
                        turret->SetFaction(WintergraspFaction[_wg->GetAttackerTeam()]);
                        break;
                }

                switch (_state)
                {
                    case BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DESTROY:
                    case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY:
                    case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY:
                        _wg->HideNpc(turret);
                        break;
                    default:
                        break;
                }
            }
        }
        UpdateCreatureAndGo();
    }
}

void BfWGGameObjectBuilding::UpdateCreatureAndGo()
{
    for (ObjectGuid guid : m_attackerCreatures[_wg->GetDefenderTeam()])
        if (Creature* creature = _wg->GetCreature(guid))
            _wg->HideNpc(creature);

    for (ObjectGuid guid : m_attackerCreatures[_wg->GetAttackerTeam()])
        if (Creature* creature = _wg->GetCreature(guid))
        {
            _wg->ShowNpc(creature, true);
            if (_wg->isCreatureGuard(creature))
            {
                creature->SetRespawnDelay(INVITE_ACCEPT_WAIT_TIME);
                creature->GetMotionMaster()->MoveRandom(25.f);
            }
        }

    for (ObjectGuid guid : m_attackerGameObjects[_wg->GetDefenderTeam()])
        if (GameObject* go = _wg->GetGameObject(guid))
            go->SetRespawnTime(RESPAWN_ONE_DAY);

    for (ObjectGuid guid : m_attackerGameObjects[_wg->GetAttackerTeam()])
        if (GameObject* go = _wg->GetGameObject(guid))
            go->SetRespawnTime(RESPAWN_IMMEDIATELY);
}

void BfWGGameObjectBuilding::UpdateTurretAttack(bool disable)
{
    for (ObjectGuid guid : m_TowerCannonBottomList)
    {
        if (Creature* creature = _wg->GetCreature(guid))
        {
            if (disable)
                _wg->HideNpc(creature);
            else
                _wg->ShowNpc(creature, true);

            switch (_buildGUID.GetEntry())
            {
                case GO_WINTERGRASP_FORTRESS_TOWER_1:
                case GO_WINTERGRASP_FORTRESS_TOWER_2:
                case GO_WINTERGRASP_FORTRESS_TOWER_3:
                case GO_WINTERGRASP_FORTRESS_TOWER_4:
                {
                    creature->SetFaction(WintergraspFaction[_wg->GetDefenderTeam()]);
                    break;
                }
                case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
                case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
                case GO_WINTERGRASP_FLAMEWATCH_TOWER:
                {
                    creature->SetFaction(WintergraspFaction[_wg->GetAttackerTeam()]);
                    break;
                }
            }
        }
    }

    for (ObjectGuid guid : m_TurretTopList)
    {
        if (Creature* creature = _wg->GetCreature(guid))
        {
            if (disable)
                _wg->HideNpc(creature);
            else
                _wg->ShowNpc(creature, true);

            switch (_buildGUID.GetEntry())
            {
                case GO_WINTERGRASP_FORTRESS_TOWER_1:
                case GO_WINTERGRASP_FORTRESS_TOWER_2:
                case GO_WINTERGRASP_FORTRESS_TOWER_3:
                case GO_WINTERGRASP_FORTRESS_TOWER_4:
                {
                    creature->SetFaction(WintergraspFaction[_wg->GetDefenderTeam()]);
                    break;
                }
                case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
                case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
                case GO_WINTERGRASP_FLAMEWATCH_TOWER:
                {
                    creature->SetFaction(WintergraspFaction[_wg->GetAttackerTeam()]);
                    break;
                }
            }
        }
    }
}

void BfWGGameObjectBuilding::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(_worldState, _state);
}

void BfWGGameObjectBuilding::Save()
{
    sWorld->setWorldState(_worldState, _state);
}

void BfWGGameObjectBuilding::prepareDelete()
{
    if (GameObject* building = _wg->GetGameObject(_buildGUID))
    {
        building->SetRespawnTime(0);                  // not save respawn time
        building->Delete();
        building = nullptr;
    }

    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
    {
        for (ObjectGuid guid : m_attackerGameObjects[i])
            if (GameObject* go = _wg->GetGameObject(guid))
            {
                go->SetRespawnTime(0);                  // not save respawn time
                go->Delete();
                go = nullptr;
            }
        m_attackerGameObjects[i].clear();
    }

    for (int8 i = 0; i < PVP_TEAMS_COUNT; i++)
    {
        for (ObjectGuid guid : m_attackerCreatures[i])
            if (Creature* creature = _wg->GetCreature(guid))
                creature->ClearZoneScript();
        m_attackerCreatures[i].clear();
    }

    for (ObjectGuid guid : m_TowerCannonBottomList)
        if (Creature* creature = _wg->GetCreature(guid))
            creature->ClearZoneScript();
    m_TowerCannonBottomList.clear();

    for (ObjectGuid guid : m_TurretTopList)
        if (Creature* creature = _wg->GetCreature(guid))
            creature->ClearZoneScript();
    m_TurretTopList.clear();
}
