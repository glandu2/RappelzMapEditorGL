#include "Terrain.h"
#include "GLTexture.h"
#include "GLProgram.h"
#include "TerrainBlendMap.h"
#include "TerrainTextureArray.h"
#include <algorithm>

#include <stdio.h>
#include <string.h>

#define VBO_BUFFER_OFFSET(x) (reinterpret_cast<void*>(x))

#include <GL/glew.h>



static TextureFile TEXTURES[] = {
	{21101, 2, "grass@normal.dds"},
	{21102, 2, "grass@rough.dds"},
	{21103, 2, "grass@darken_rough.dds"},
	{21104, 2, "grass@yellow.dds"},
	{21105, 2, "grass@brown_02.dds"},
	{21106, 2, "grass@darken_rough.dds"},
	{21107, 2, "grass@normal_02.dds"},
	{21108, 2, "grass@normal_03.dds"},
	{21109, 2, "grass@rough_02.dds"},
	{21110, 2, "grass@normal_04.dds"},
	{21201, 2, "grass@yellow.dds"},
	{21202, 2, "grass@orange.dds"},
	{21203, 2, "grass@orange_02.dds"},
	{21204, 2, "grass@yellow_02.dds"},
	{21205, 2, "grass@yellow_03.dds"},
	{21206, 2, "grass@brown.dds"},
	{21207, 2, "grass@dark_red.dds"},
	{21208, 2, "grass@darken_brown.dds"},
	{21209, 2, "common_yellow@grass001.dds"},
	{21301, 2, "earth@normal.dds"},
	{21302, 2, "earth@dry.dds"},
	{21401, 2, "rock@gray.dds"},
	{21402, 2, "rock@darken_gray.dds"},
	{21403, 2, "common_rock001.dds"},
	{21404, 2, "common_terrain_rock001.dds"},
	{21405, 2, "common_terrain_rock001_2.dds"},
	{21406, 2, "common_terrain_rock001_4.dds"},
	{21407, 2, "common_terrain_rock001_3.dds"},
	{21408, 2, "common_terrain_rock001_5.dds"},
	{21501, 2, "grass@stone001.dds"},
	{44361, 2, "de_marduka_soil_tile01.dds"},
	{44362, 2, "de_marduka_soil_tile02.dds"},
	{44363, 2, "de_marduka_soil_tile03.dds"},
	{44364, 2, "de_marduka_soil_tile04.dds"},
	{44367, 2, "de_marduka_soil_tile05.dds"},
	{44368, 2, "de_marduka_gravel_tile01.dds"},
	{44369, 2, "de_marduka_sand_tile01.dds"},
	{44370, 2, "de_marduka_gravel_tile02.dds"},
	{44371, 2, "de_marduka_grass_tile01.dds"},
	{44381, 2, "de_mare_stone_ctile01.dds"},
	{44382, 2, "de_mare_stone_ctile02.dds"},
	{44383, 2, "de_mare_grass_tile02.dds"},
	{44384, 2, "de_mare_grass_tile01.dds"},
	{44389, 2, "de_cityruin_rock_ctile01.dds"},
	{44390, 2, "de_cityruin_rock_tile02.dds"},
	{44395, 2, "de_cityruin_rock_tile01.dds"},
	{44397, 2, "de_cityruin_rock_ctile02.dds"},
	{44476, 2, "de_elvenforrest_grass_tile01.dds"},
	{44468, 2, "de_elvenforrest_grass_tile02.dds"},
	{44469, 2, "de_elvenforrest_grass_tile03.dds"},
	{44470, 2, "de_elvenforrest_grass_tile04.dds"},
	{44471, 2, "de_elvenforrest_grass_tile05.dds"},
	{44472, 2, "de_elvenforrest_grass_tile06.dds"},
	{44473, 2, "de_elvenforrest_grass_tile07.dds"},
	{44479, 2, "de_fairyhouse_stone_01.dds"},
	{41101, 4, "grass@normal.dds"},
	{41102, 4, "grass@rough.dds"},
	{41103, 4, "grass@darken_rough.dds"},
	{41104, 4, "grass@yellow.dds"},
	{41105, 4, "grass@brown_02.dds"},
	{41106, 4, "grass@darken_rough.dds"},
	{41107, 4, "grass@normal_02.dds"},
	{41108, 4, "grass@normal_03.dds"},
	{41109, 4, "grass@rough_02.dds"},
	{41110, 4, "grass@normal_04.dds"},
	{41201, 4, "grass@yellow.dds"},
	{41202, 4, "grass@orange.dds"},
	{41203, 4, "grass@orange_02.dds"},
	{41204, 4, "grass@yellow_02.dds"},
	{41205, 4, "grass@yellow_03.dds"},
	{41206, 4, "grass@brown.dds"},
	{41207, 4, "grass@dark_red.dds"},
	{41301, 4, "earth@normal.dds"},
	{41302, 4, "earth@dry.dds"},
	{41401, 4, "rock@gray.dds"},
	{41402, 4, "rock@darken_gray.dds"},
	{41501, 4, "grass@stone001.dds"},
	{22101, 2, "zone002_grass001.dds"},
	{22102, 2, "zone002_grass002.dds"},
	{22103, 2, "zone002_grass003.dds"},
	{22104, 2, "zone002_grass004.dds"},
	{22105, 2, "zone002_grass005.dds"},
	{44341, 2, "w002_katan_t_grass002.dds"},
	{44342, 2, "w002_katan_t_grass001.dds"},
	{22301, 2, "zone002_earth001.dds"},
	{22302, 2, "zone002_earth002.dds"},
	{22303, 2, "zone002_earth003.dds"},
	{22304, 2, "zone003_earth001.dds"},
	{22305, 2, "zone003_mud@dry001.dds"},
	{22306, 2, "zone002_mud@dry001.dds"},
	{22307, 2, "zone002_mud001.dds"},
	{22308, 2, "zone003_mud@dry002.dds"},
	{22401, 2, "zone002_rock001.dds"},
	{22402, 2, "zone002_rock002.dds"},
	{22403, 2, "asvil_terrain_rock998.dds"},
	{22404, 2, "asvil_terrain_rock999.dds"},
	{22405, 2, "zone004_terrain_rock001_1.dds"},
	{22406, 2, "zone004_terrain_rock001_2.dds"},
	{22501, 2, "zone003_stone001.dds"},
	{44531, 2, "zone004_terrain_rock001_3.dds"},
	{42610, 2, "ceriu_barrier_gravel_tile01.dds"},
	{42611, 2, "ceriu_barrier_stone_tile01.dds"},
	{42613, 2, "ceriu_barrier_grass_tile01.dds"},
	{44421, 2, "kathan_messi_grass_tile01.dds"},
	{44424, 2, "kathan_messi_grass_tile02.dds"},
	{44423, 2, "kathan_messi_grass_tile03.dds"},
	{42101, 4, "zone002_grass001.dds"},
	{42102, 4, "zone002_grass002.dds"},
	{42103, 4, "zone002_grass003.dds"},
	{42104, 4, "zone002_grass004.dds"},
	{42105, 4, "zone002_grass005.dds"},
	{42401, 4, "zone002_rock001.dds"},
	{42402, 4, "zone002_rock002.dds"},
	{42501, 4, "zone003_stone001.dds"},
	{0, 2, "zone005_grass@normal002.dds"},
	{23102, 2, "zone005_grass@normal001.dds"},
	{23103, 2, "zone005_grass@normal003.dds"},
	{23104, 2, "zone005_grass@normal004.dds"},
	{23105, 2, "zone005_grass@yellow001.dds"},
	{44339, 2, "zone005_grass@yellow003.dds"},
	{44340, 2, "zone005_grass@yellow004.dds"},
	{23106, 2, "zone005_grass@yellow002.dds"},
	{23301, 2, "zone005_earth@normal001.dds"},
	{23302, 2, "zone005_earth@normal002.dds"},
	{23303, 2, "zone005_earth@normal003.dds"},
	{23304, 2, "zone005_earth@dry001.dds"},
	{23305, 2, "zone005_brick001.dds"},
	{23401, 2, "zone005_rock@gray001.dds"},
	{23402, 2, "zone005_rock@gray002.dds"},
	{23403, 2, "zone005_rock@gray003.dds"},
	{23501, 2, "zone005_stone001.dds"},
	{23502, 2, "zone005_stone002.dds"},
	{23503, 2, "zone005_stone003.dds"},
	{23504, 2, "zone005_stone004.dds"},
	{23505, 2, "novice_terrain_rock001.dds"},
	{23506, 2, "novice_terrain_rock001_2.dds"},
	{23507, 2, "novice_terrain_rock001_3.dds"},
	{44399, 2, "horaizen_yak_grass_tile01.dds"},
	{44401, 2, "horaizen_yak_soil_tile01.dds"},
	{44396, 2, "horaizen_halite_ sand_tile02.dds"},
	{44391, 2, "horaizen_halite_ rock_tile01.dds"},
	{44392, 2, "horaizen_halite_ sand_tile01.dds"},
	{42614, 2, "horaizen_halite_ rock_tile02.dds"},
	{42615, 2, "horaizen_halite_ rock_tile03.dds"},
	{43101, 4, "zone005_grass@normal002.dds"},
	{43102, 4, "zone005_grass@normal001.dds"},
	{43103, 4, "zone005_grass@normal003.dds"},
	{43104, 4, "zone005_grass@normal004.dds"},
	{43105, 4, "zone005_grass@yellow001.dds"},
	{43301, 4, "zone005_earth@normal001.dds"},
	{43302, 4, "zone005_earth@normal002.dds"},
	{43303, 4, "zone005_earth@normal003.dds"},
	{43304, 4, "zone005_earth@dry001.dds"},
	{43401, 4, "zone005_rock@gray001.dds"},
	{43402, 4, "zone005_rock@gray002.dds"},
	{43403, 4, "zone005_rock@gray003.dds"},
	{43501, 4, "zone005_stone001.dds"},
	{43502, 4, "zone005_stone002.dds"},
	{43503, 4, "zone005_stone003.dds"},
	{43504, 4, "zone005_stone004.dds"},
	{43505, 4, "novice_terrain_rock001.dds"},
	{43506, 4, "novice_terrain_rock001_2.dds"},
	{43507, 4, "novice_terrain_rock001_3.dds"},
	{21601, 2, "brick@templar001.dds"},
	{21602, 2, "zone001_templ_brick001.dds"},
	{21603, 2, "zone001_templ_brick002.dds"},
	{21604, 2, "zone001_templ_brick003.dds"},
	{21605, 2, "zone001_templ_brick004.dds"},
	{21606, 2, "zone001_templ_brick005.dds"},
	{21607, 2, "zone001_templ_brick006.dds"},
	{22601, 2, "zone002_brick001.dds"},
	{22602, 2, "zone003_brick001.dds"},
	{20601, 2, "common_terrain_brick001.dds"},
	{20602, 2, "common_terrain_brick002.dds"},
	{20603, 2, "common_terrain_brick003.dds"},
	{20604, 2, "common_terrain_brick004.dds"},
	{41601, 4, "brick@templar001.dds"},
	{41602, 4, "zone001_templ_brick001.dds"},
	{41603, 4, "zone001_templ_brick002.dds"},
	{41604, 4, "zone001_templ_brick003.dds"},
	{41605, 4, "zone001_templ_brick004.dds"},
	{41606, 4, "zone001_templ_brick005.dds"},
	{41607, 4, "zone001_templ_brick006.dds"},
	{42601, 4, "zone002_brick001.dds"},
	{42602, 4, "zone003_brick001.dds"},
	{24101, 2, "common_terrain_grass001.dds"},
	{24102, 2, "common_terrain_grass001_2.dds"},
	{24103, 2, "common_terrain_grass002.dds"},
	{24104, 2, "common_terrain_grass002_2.dds"},
	{44313, 2, "w004_rondo_t_grass010.dds"},
	{44314, 2, "w004_rondo_t_grass004.dds"},
	{44315, 2, "w004_rondo_t_grass003.dds"},
	{44316, 2, "w004_rondo_t_grass002.dds"},
	{44317, 2, "w004_rondo_t_grass001.dds"},
	{44327, 2, "w004_rondo_t_grass010.dds"},
	{44328, 2, "w004_rondo_t_grass011.dds"},
	{44329, 2, "w004_rondo_t_grass012.dds"},
	{44330, 2, "w004_rondo_t_grass013.dds"},
	{44331, 2, "w004_rondo_t_grass014.dds"},
	{44332, 2, "w004_rondo_t_flower_03.dds"},
	{44333, 2, "w004_rondo_t_flower.dds"},
	{44334, 2, "w004_rondo_t_etc001.dds"},
	{44336, 2, "w004_rondo_t_grass015.dds"},
	{44338, 2, "w004_rondo_t_grass017.dds"},
	{24301, 2, "common_terrain_earth001.dds"},
	{24302, 2, "common_terrain_earth001_2.dds"},
	{44309, 2, "w004_rondo_t_earth001.dds"},
	{44318, 2, "w004_rondo_t_earth002.dds"},
	{44322, 2, "w004_rondo_t_etc001.dds"},
	{44323, 2, "w004_rondo_t_rock002.dds"},
	{44324, 2, "w004_rondo_t_rock002_2.dds"},
	{44325, 2, "w004_rondo_t_earth004.dds"},
	{44326, 2, "w004_rondo_t_earth002.dds"},
	{44357, 2, "w004_rondo_v001_p_terrain_13.dds"},
	{44304, 2, "w004_rondo_v001_p_terrain_14.dds"},
	{44305, 2, "w004_rondo_v001_p_terrain_15.dds"},
	{44310, 2, "w004_rondo_v001_t_brick003.dds"},
	{44311, 2, "w004_rondo_v001_t_brick002.dds"},
	{44312, 2, "w004_rondo_v001_t_brick001.dds"},
	{44356, 2, "rondoh_market_stone_ctile01.dds"},
	{44306, 2, "w004_crystal_rock001.dds"},
	{44307, 2, "w004_crystal_rock001_2.dds"},
	{44308, 2, "w004_crystal_rock001_3.dds"},
	{44319, 2, "w004_crystal_rock001_5.dds"},
	{44320, 2, "w004_crystal_rock001_6.dds"},
	{44321, 2, "w004_crystal_rock001_4.dds"},
	{44380, 2, "w004_crystal_rock001_7.dds"},
	{44378, 2, "w004_crystal_rock001_8.dds"},
	{44379, 2, "w004_crystal_rock001_9.dds"},
	{44385, 2, "rondoh_palmir_rock_tile01.dds"},
	{44386, 2, "rondoh_palmir_sand_tile01.dds"},
	{44387, 2, "rondoh_palmir_rock_tile02.dds"},
	{44388, 2, "rondoh_palmir_gravel_tile01.dds"},
	{44483, 2, "ceriuh_ceriuh_stone_tile03.dds"},
	{44484, 2, "ceriuh_ceriuh_gravel_tile01.dds"},
	{44486, 2, "ceriuh_ceriuh_gravel_tile02.dds"},
	{44490, 2, "ceriuh_ceriuh_sand_tile01.dds"},
	{44491, 2, "ceriuh_ceriuh_sand_tile02.dds"},
	{44492, 2, "ceriuh_ceriuh_soil_tile01.dds"},
	{44465, 2, "rondoh_orchard_stone_tile01.dds"},
	{44466, 2, "rondoh_crimsonfarm_grass_tile03.dds"},
	{44425, 2, "rondoh_crimsonfarm_brick_tile01.dds"},
	{44467, 2, "rondoh_crimsonfarm_grass_tile02.dds"},
	{44427, 2, "rondoh_crimsonfarm_grass_tile01.dds"},
	{44428, 2, "rondoh_crimsonfarm_soil_tile02.dds"},
	{44429, 2, "rondoh_crimsonfarm_soil_tile01.dds"},
	{44430, 2, "event_halloween_cemetry_grass02.dds"},
	{44431, 2, "rondoh_orchard_stone_tile03.dds"},
	{44433, 2, "rondoh_crimsonfarm_brick_tile02.dds"},
	{44436, 2, "rondoh_orchard_stone_tile02.dds"},
	{44434, 4, "rondoh_orchard_stone_tile03.dds"},
	{44435, 4, "rondoh_crimsonfarm_brick_tile02.dds"},
	{44437, 4, "rondoh_orchard_stone_tile02.dds"},
	{44480, 4, "ceriuh_ceriuh_stone_tile01.dds"},
	{44481, 4, "ceriuh_ceriuh_stone_tile02.dds"},
	{44482, 4, "ceriuh_ceriuh_stone_tile03.dds"},
	{44485, 4, "ceriuh_ceriuh_gravel_tile01.dds"},
	{44488, 4, "ceriuh_ceriuh_gravel_tile02.dds"},
	{44489, 4, "ceriuh_ceriuh_gravel_tile03.dds"},
	{44493, 4, "ceriuh_ceriuh_sand_tile01.dds"},
	{44494, 4, "ceriuh_ceriuh_sand_tile02.dds"},
	{44495, 4, "ceriuh_ceriuh_soil_tile01.dds"},
	{44101, 4, "common_terrain_grass001.dds"},
	{44102, 4, "common_terrain_grass001_2.dds"},
	{44103, 4, "common_terrain_grass002.dds"},
	{44104, 4, "common_terrain_grass002_2.dds"},
	{44301, 4, "common_terrain_earth001.dds"},
	{44302, 4, "common_terrain_earth001_2.dds"},
	{44303, 2, "dun_t_black.dds"},
	{44344, 2, "dungeon_crystal_tile001a.dds"},
	{44345, 2, "dungeon_crystal_tile002a.dds"},
	{44346, 2, "dungeon_crystal_tile003a.dds"},
	{44347, 2, "dungeon_crystal_tile004.dds"},
	{44348, 2, "dungeon_crystal_tile005.dds"},
	{44349, 2, "dungeon_crystal_tile006.dds"},
	{44350, 2, "dungeon_crystal_tile009.dds"},
	{44351, 2, "dungeon_crystal_tile008.dds"},
	{44352, 2, "dungeon_crystal_tile007.dds"},
	{44353, 2, "dungeon_crystal_tile010.dds"},
	{44354, 2, "dungeon_crystal_ctile01.dds"},
	{44355, 2, "dungeon_crystal_ctile02.dds"},
	{44377, 2, "ceriu_moonlight_sand_ctile01.dds"},
	{44358, 2, "ceriu_moonlight_sand_ctile02.dds"},
	{44359, 2, "ceriu_moonlight_sand_ctile03.dds"},
	{44372, 2, "dungeon_moonlight_soil_ctile05.dds"},
	{44373, 2, "dungeon_moonlight_soil_ctile04.dds"},
	{44374, 2, "dungeon_moonlight_soil_ctile08.dds"},
	{44375, 2, "dungeon_moonlight_soil_ctile06.dds"},
	{44376, 2, "dungeon_moonlight_soil_ctile07.dds"},
	{44404, 2, "dungeon_palmir_ctile01.dds"},
	{44403, 2, "dungeon_palmir_stone_tile01.dds"},
	{44405, 2, "dungeon_palmir_stone_tile02.dds"},
	{44406, 2, "dungeon_palmir_stone_tile03.dds"},
	{44407, 2, "dungeon_palmir_stone_tile05.dds"},
	{44408, 2, "dungeon_palmir_secret_ctile01.dds"},
	{44409, 2, "dungeon_palmir_secret_ctile02.dds"},
	{44410, 2, "dungeon_palmir_secret_ctile03.dds"},
	{44411, 2, "dungeon_palmir_stone_tile04.dds"},
	{44412, 2, "dungeon_palmir_stone_tile07.dds"},
	{44413, 2, "dungeon_palmir_stone_tile08.dds"},
	{44414, 2, "dungeon_palmir_crypt_ctile01.dds"},
	{44415, 2, "dungeon_palmir_crypt_ctile02.dds"},
	{44416, 2, "dungeon_palmir_crypt_ctile03.dds"},
	{44417, 2, "dungeon_palmir_ctile01_1.dds"},
	{44418, 2, "dungeon_palmir_stone_tile03_1.dds"},
	{44419, 2, "dungeon_palmir_stone_tile05_1.dds"},
	{44420, 2, "dungeon_palmir_stone_tile08_1.dds"},
	{44552, 2, "dungeon_palmir_crypt_ctile01_a.dds"},
	{44553, 2, "dungeon_palmir_crypt_ctile02_a.dds"},
	{44554, 2, "dungeon_crystal_tile011.dds"},
	{44445, 2, "event_halloween_nightmare_rock01.dds"},
	{44456, 2, "event_halloween_nightmare_rock02.dds"},
	{44457, 2, "event_halloween_nightmare_grass01.dds"},
	{44458, 2, "event_halloween_nightmare_earth01.dds"},
	{44422, 2, "event_halloween_cemetry_grass02.dds"},
	{44461, 2, "event_halloween_cemetry_stone_ctile01.dds"},
	{44462, 2, "event_halloween_cemetry_grass01.dds"},
	{44463, 2, "event_halloween_cemetry_earth01.dds"},
	{44474, 2, "lost secret_island_snow_01.dds"},
	{44475, 2, "lost secret_island_snow_02.dds"},
	{44477, 2, "lost secret_island_snow_03.dds"},
	{44478, 2, "lost secret_island_stone_07.dds"},
	{44510, 2, "lost secret_island_snow_04.dds"},
	{44511, 2, "lost secret_island_tile_01.dds"},
	{44500, 4, "lost secret_island_stone_01.dds"},
	{44501, 4, "lost secret_island_stone_02.dds"},
	{44502, 4, "lost secret_island_stone_03.dds"},
	{44503, 4, "lost secret_island_stone_04.dds"},
	{44504, 4, "lost secret_island_stone_06.dds"},
	{44506, 4, "lost secret_island_stone_08.dds"},
	{44507, 4, "lost secret_island_stone_09.dds"},
	{44508, 4, "lost secret_island_stone_10.dds"},
	{44505, 2, "lost secret_island_stone_05.dds"},
	{44515, 2, "dungeon_bearload_stone_01.dds"},
	{44516, 2, "dungeon_bearload_stone_02.dds"},
	{44517, 2, "dungeon_bearload_stone_03.dds"},
	{44518, 2, "dungeon_bearload_stone_04.dds"},
	{44519, 2, "dungeon_bearload_stone_05.dds"},
	{44520, 2, "dungeon_bearload_stone_06.dds"},
	{44521, 2, "dungeon_bearload_stone_07.dds"},
	{44522, 2, "dungeon_bearload_rock_01.dds"},
	{44523, 2, "dungeon_bearload_soil_01.dds"},
	{44524, 2, "dungeon_bearload_sand_01.dds"},
	{44525, 2, "dungeon_bearload_sand_02.dds"},
	{44526, 2, "dungeon_bearload_brick_01.dds"},
	{44527, 2, "dungeon_bearload_enteroom_tex.dds"},
	{44528, 2, "dungeon_mineshaft_rock_001.dds"},
	{44529, 2, "dungeon_mineshaft_rock_002.dds"},
	{44530, 2, "dungeon_mineshaft_brick001.dds"},
	{44531, 2, "novice_terrain_rock004.dds"},
	{44532, 2, "novice_terrain_rock008.dds"},
	{44533, 2, "novice_terrain_rock009.dds"},
	{44534, 2, "novice_terrain_rock010.dds"},
	{44535, 4, "dungeon_elkassia_tile001.dds"},
	{44536, 2, "dungeon_elkassia_tile002.dds"},
	{44538, 2, "dethmatch_tile001.dds"},
	{44539, 2, "dethmatch_tile003.dds"},
	{44540, 2, "dethmatch_tile005.dds"},
	{44541, 2, "dethmatch_tile007.dds"},
	{44542, 4, "lostisland_grass@brown_02.dds"},
	{44543, 4, "lostisland_grass@rough.dds"},
	{44544, 4, "lostisland_grass@normal_02.dds"},
	{44545, 2, "lostisland_terrain_rock001.dds"},
	{44546, 2, "lost_island_snow_03_dark.dds"},
	{44547, 2, "lost_island_snow_03_blue.dds"},
	{44548, 2, "lost_island_iceblue.dds"},
	{44549, 2, "lost_island_iceviolet.dds"},
	{44550, 2, "lost_island_icegreen.dds"},
	{44551, 2, "lostisland_earth@normal.dds"},
	{44555, 2, "dungeon_lavaload_tile_01.dds"},
	{44556, 2, "dungeon_lavaload_tile_02.dds"},
	{44557, 2, "battle_arena_slaughter_start_tile01.dds"},
	{44558, 2, "battle_arena_slaughter_start_tile02.dds"},
	{44559, 2, "battle_arena_slaughter_start_tile03.dds"},
	{44560, 2, "battle_arena_slaughter_start_tile04.dds"}
};

#define TEXTURE_COUNT (sizeof(TEXTURES)/sizeof(TextureFile))

Terrain::Terrain() : textureLayers(new TerrainTextureArray), blendMap(new TerrainBlendMap), shaderProgram(new GLProgram)
{
}

Terrain::~Terrain() {

}

struct TextureInfo {
	int index;
	float worldSize;
	const char* name;
};

bool Terrain::loadFromNfm(const char* filename) {
	NFM_HEADER_V22 header;

	shaderProgram->loadShaders("vertex.glsl", "fragment.glsl");

	std::vector<unsigned short> usedTextures;
	std::vector<const char*> usedTexturesName;

	FILE* file = fopen(filename, "rb");
	if(!file) {
		printf("Cant open file %s\n", filename);
		return false;
	}

	fread(&header, 1, sizeof(header), file);
	if(strcmp(header.szSign, "nFlavor Map")) {
		printf("Not a NFM map\n");
		return false;
	}

	if(header.dwVersion != 22)
		printf("Warning: map is version %d\n", header.dwVersion);

	fseek(file, header.dwTerrainSegmentOffset, SEEK_SET);
	fread(&segments, 1, sizeof(segments), file);
	fclose(file);

	for(int segmentY = 0; segmentY < 64; segmentY++) {
		for(int tileY = 0; tileY < 6; tileY++) {
			for(int segmentX = 0; segmentX < 64; segmentX++) {
				for(int tileX = 0; tileX < 6; tileX++) {
					vertices.push_back(glm::vec3(segmentX*6*42 + tileX*42, segmentY*6*42 + tileY*42, segments[segmentY][segmentX].vertices[tileY][tileX].height));
				}
			}
		}
	}

	for(int segmentY = 0; segmentY < 64; segmentY++) {
		for(int segmentX = 0; segmentX < 64; segmentX++) {
			for(int texLayer = 0; texLayer < 3; texLayer++) {
				for(int i = 0; i < TEXTURE_COUNT; i++) {
					if(TEXTURES[i].id == segments[segmentY][segmentX].tile[texLayer] &&
							std::find(usedTextures.begin(), usedTextures.end(), i) == usedTextures.end())
					{
						usedTextures.push_back(i);
					}
				}
			}
		}
	}

	assert(usedTextures.size() < 256 && "Too many used texture in map !");

	for(size_t i = 0; i < usedTextures.size(); i++) {
		usedTexturesName.push_back(TEXTURES[usedTextures[i]].filename);
	}

	textureLayers->loadDDS(usedTexturesName);

	for(int blendMapY = 0; blendMapY < BLENDMAP_SIZE; blendMapY++) {
		for(int blendMapX = 0; blendMapX < BLENDMAP_SIZE; blendMapX++) {
			int segmentX = blendMapX/(4*6);
			int segmentY = blendMapY/(4*6);
			int tileX = (blendMapX/4) % 6;
			int tileY = (blendMapY/4) % 6;
			int posInTileX = blendMapX % 4; //texture splat dot position
			int posInTileY = blendMapY % 4;

			Segment* segment = &segments[segmentY][segmentX];
			Vertex* vertex = &segment->vertices[tileY][tileX];

			TerrainBlendMap::BlendElement blendElement;
			memset(&blendElement, 0, sizeof(blendElement));

			blendElement.alpha1 = (vertex->textureBlend[0] >> (2*posInTileX + 8*posInTileY)) & 0x3;
			blendElement.alpha2 = (vertex->textureBlend[1] >> (2*posInTileX + 8*posInTileY)) & 0x3;

			for(size_t i = 1; i < usedTextures.size(); i++) {
				if(segment->tile[0] == TEXTURES[usedTextures.at(i)].id) {
					blendElement.texIndices[0] = i;
				}

				if(segment->tile[1] == TEXTURES[usedTextures.at(i)].id) {
					blendElement.texIndices[1] = i;
				}

				if(segment->tile[2] == TEXTURES[usedTextures.at(i)].id) {
					blendElement.texIndices[2] = i;
				}
			}
			blendMap->imgData.push_back(blendElement);
		}
	}

	for(int y = 0; y < 64*6-1; y++) {
		for(int x = 0; x < 64*6-1; x++) {
			unsigned int v00 = y*64*6 + x;
			unsigned int v01 = y*64*6 + x+1;
			unsigned int v10 = (y+1)*64*6 + x;
			unsigned int v11 = (y+1)*64*6 + x+1;

			indices.push_back(v00);
			indices.push_back(v01);
			indices.push_back(v10);

			indices.push_back(v10);
			indices.push_back(v01);
			indices.push_back(v11);
		}
	}

	return true;
}

bool Terrain::loadToGpu() {
	// Create the VAO
	if(glId == 0)
		glGenVertexArrays(1, &glId);

	glBindVertexArray(glId);

	// Create the buffer for the vertices atttributes
	if(vertexVboId == 0)
		glGenBuffers(1, &vertexVboId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, VBO_BUFFER_OFFSET(0));

	if(indexVboId == 0)
		glGenBuffers(1, &indexVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	shaderProgram->loadToGpu();
	blendMap->loadToGpu();
	textureLayers->loadToGpu();

	return true;
}

void Terrain::unloadFromGpu() {
	/*blendMap->unloadFromGpu();
	for(size_t i = 0; i < textures.size(); i++)
		textures[i]->unloadFromGpu();*/

	if(glId)
		glDeleteVertexArrays(1, &glId);
	glId = 0;

	if(vertexVboId)
		glDeleteBuffers(1, &vertexVboId);
	vertexVboId = 0;

	if(indexVboId)
		glDeleteBuffers(1, &indexVboId);
	indexVboId = 0;
}

void Terrain::select(int batch) {
	glUseProgram(shaderProgram->getId());

	glBindVertexArray(glId);

	bindTextureUnit(GL_TEXTURE_2D, 0, blendMap->getId(), "blendMap");
	bindTextureUnit(GL_TEXTURE_2D_ARRAY, 1, textureLayers->getId(), "terrainTextures");
}

void Terrain::bindTextureUnit(int textureType, int texUnit, unsigned int texId, const char* uniformName) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(textureType, texId);
	unsigned int blendMapUniform = glGetUniformLocation(shaderProgram->getId(), uniformName);
	glUniform1i(blendMapUniform, texUnit);
}

void Terrain::unselect() {
	glBindVertexArray(0);
}
