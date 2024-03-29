/*
 * Copyright (c) 2018-2024, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include <peelo/unicode/ctype/_utils.hpp>

namespace peelo::unicode::ctype
{
  /**
   * Converts given Unicode code point into lowercase.
   */
  inline char32_t
  tolower(char32_t c)
  {
    static const std::unordered_map<char32_t, char32_t> tolower_map =
    {{
      { 125216, 125250 }, { 125214, 125248 }, { 125212, 125246 },
      { 125210, 125244 }, { 125206, 125240 }, { 71871, 71903 },
      { 71870, 71902 }, { 71869, 71901 }, { 71868, 71900 },
      { 71867, 71899 }, { 71866, 71898 }, { 71865, 71897 },
      { 71864, 71896 }, { 71863, 71895 }, { 71861, 71893 },
      { 71859, 71891 }, { 71857, 71889 }, { 71855, 71887 },
      { 71853, 71885 }, { 71851, 71883 }, { 71849, 71881 },
      { 71847, 71879 }, { 71845, 71877 }, { 71843, 71875 },
      { 71841, 71873 }, { 68782, 68846 }, { 68780, 68844 },
      { 68774, 68838 }, { 68772, 68836 }, { 68770, 68834 },
      { 68764, 68828 }, { 68762, 68826 }, { 68758, 68822 },
      { 68750, 68814 }, { 66964, 67003 }, { 66962, 67001 },
      { 66961, 67000 }, { 66960, 66999 }, { 66959, 66998 },
      { 66958, 66997 }, { 66957, 66996 }, { 66953, 66992 },
      { 66951, 66990 }, { 66949, 66988 }, { 66948, 66987 },
      { 66947, 66986 }, { 66946, 66985 }, { 66945, 66984 },
      { 66944, 66983 }, { 66943, 66982 }, { 66942, 66981 },
      { 66941, 66980 }, { 66940, 66979 }, { 66770, 66810 },
      { 66768, 66808 }, { 66766, 66806 }, { 66764, 66804 },
      { 66762, 66802 }, { 66761, 66801 }, { 66760, 66800 },
      { 66759, 66799 }, { 66758, 66798 }, { 66757, 66797 },
      { 66756, 66796 }, { 66755, 66795 }, { 66754, 66794 },
      { 66752, 66792 }, { 66750, 66790 }, { 66748, 66788 },
      { 66746, 66786 }, { 66744, 66784 }, { 66742, 66782 },
      { 66740, 66780 }, { 66738, 66778 }, { 66736, 66776 },
      { 66599, 66639 }, { 66598, 66638 }, { 66597, 66637 },
      { 66596, 66636 }, { 66595, 66635 }, { 66594, 66634 },
      { 66593, 66633 }, { 66592, 66632 }, { 66591, 66631 },
      { 66590, 66630 }, { 66589, 66629 }, { 66588, 66628 },
      { 66587, 66627 }, { 66585, 66625 }, { 66583, 66623 },
      { 66581, 66621 }, { 66579, 66619 }, { 66574, 66614 },
      { 66572, 66612 }, { 66571, 66611 }, { 66568, 66608 },
      { 66565, 66605 }, { 66564, 66604 }, { 66563, 66603 },
      { 66562, 66602 }, { 66561, 66601 }, { 66560, 66600 },
      { 65338, 65370 }, { 65337, 65369 }, { 65336, 65368 },
      { 65335, 65367 }, { 65334, 65366 }, { 65333, 65365 },
      { 65332, 65364 }, { 65331, 65363 }, { 125194, 125228 },
      { 7344, 4336 }, { 125192, 125226 }, { 7342, 4334 },
      { 125190, 125224 }, { 7340, 4332 }, { 68755, 68819 },
      { 42828, 42829 }, { 125188, 125222 }, { 7338, 4330 },
      { 125186, 125220 }, { 7336, 4328 }, { 125184, 125218 },
      { 7334, 4326 }, { 68749, 68813 }, { 42822, 42823 },
      { 7332, 4324 }, { 7330, 4322 }, { 7328, 4320 },
      { 7326, 4318 }, { 7324, 4316 }, { 68745, 68809 },
      { 42818, 42819 }, { 5106, 5114 }, { 7322, 4314 },
      { 7320, 4312 }, { 7318, 4310 }, { 4266, 11530 },
      { 4259, 11523 }, { 66928, 66967 }, { 932, 964 },
      { 4257, 11521 }, { 7316, 4308 }, { 68737, 68801 },
      { 42810, 42811 }, { 5098, 43962 }, { 1365, 1413 },
      { 7314, 4306 }, { 42808, 42809 }, { 5096, 43960 },
      { 1363, 1411 }, { 7312, 4304 }, { 42806, 42807 },
      { 5094, 43958 }, { 1361, 1409 }, { 1359, 1407 },
      { 66938, 66977 }, { 8013, 8005 }, { 1357, 1405 },
      { 66936, 66975 }, { 8011, 8003 }, { 1355, 1403 },
      { 1353, 1401 }, { 1351, 1399 }, { 1349, 1397 },
      { 1347, 1395 }, { 1345, 1393 }, { 1343, 1391 },
      { 1341, 1389 }, { 1339, 1387 }, { 1337, 1385 },
      { 1336, 1384 }, { 1335, 1383 }, { 1334, 1382 },
      { 1333, 1381 }, { 1332, 1380 }, { 1331, 1379 },
      { 1330, 1378 }, { 221, 253 }, { 11311, 11359 },
      { 1329, 1377 }, { 7983, 7975 }, { 11310, 11358 },
      { 1326, 1327 }, { 217, 249 }, { 7980, 7972 },
      { 11307, 11355 }, { 1324, 1325 }, { 7978, 7970 },
      { 11305, 11353 }, { 256, 257 }, { 7327, 4319 },
      { 1322, 1323 }, { 213, 245 }, { 11303, 11351 },
      { 7325, 4317 }, { 1320, 1321 }, { 211, 243 },
      { 11301, 11349 }, { 7323, 4315 }, { 1318, 1319 },
      { 209, 241 }, { 11299, 11347 }, { 1316, 1317 },
      { 207, 239 }, { 11297, 11345 }, { 1314, 1315 },
      { 205, 237 }, { 93790, 93822 }, { 11295, 11343 },
      { 1312, 1313 }, { 203, 235 }, { 93788, 93820 },
      { 11293, 11341 }, { 1310, 1311 }, { 201, 233 },
      { 7964, 7956 }, { 93786, 93818 }, { 11291, 11339 },
      { 1308, 1309 }, { 199, 231 }, { 7962, 7954 },
      { 93784, 93816 }, { 11289, 11337 }, { 1306, 1307 },
      { 197, 229 }, { 7960, 7952 }, { 93782, 93814 },
      { 11287, 11335 }, { 1272, 1273 }, { 7926, 7927 },
      { 1270, 1271 }, { 7924, 7925 }, { 1268, 1269 },
      { 7922, 7923 }, { 1266, 1267 }, { 7920, 7921 },
      { 1264, 1265 }, { 7918, 7919 }, { 1262, 1263 },
      { 7916, 7917 }, { 1260, 1261 }, { 7914, 7915 },
      { 1258, 1259 }, { 7912, 7913 }, { 1256, 1257 },
      { 7910, 7911 }, { 1254, 1255 }, { 7908, 7909 },
      { 1252, 1253 }, { 7906, 7907 }, { 1250, 1251 },
      { 7904, 7905 }, { 1248, 1249 }, { 7902, 7903 },
      { 1246, 1247 }, { 7900, 7901 }, { 1244, 1245 },
      { 7898, 7899 }, { 1242, 1243 }, { 7896, 7897 },
      { 1240, 1241 }, { 7894, 7895 }, { 1238, 1239 },
      { 7892, 7893 }, { 1236, 1237 }, { 7890, 7891 },
      { 1234, 1235 }, { 7888, 7889 }, { 1232, 1233 },
      { 7886, 7887 }, { 1229, 1230 }, { 1227, 1228 },
      { 1225, 1226 }, { 1223, 1224 }, { 1221, 1222 },
      { 1219, 1220 }, { 1217, 1218 }, { 1216, 1231 },
      { 7870, 7871 }, { 1214, 1215 }, { 7868, 7869 },
      { 1212, 1213 }, { 7866, 7867 }, { 1210, 1211 },
      { 7864, 7865 }, { 1208, 1209 }, { 7862, 7863 },
      { 1206, 1207 }, { 7860, 7861 }, { 7696, 7697 },
      { 1042, 1074 }, { 1204, 1205 }, { 7858, 7859 },
      { 7694, 7695 }, { 1040, 1072 }, { 1202, 1203 },
      { 7856, 7857 }, { 7692, 7693 }, { 1038, 1118 },
      { 1200, 1201 }, { 7854, 7855 }, { 7690, 7691 },
      { 1036, 1116 }, { 1198, 1199 }, { 7852, 7853 },
      { 7688, 7689 }, { 1034, 1114 }, { 1196, 1197 },
      { 7850, 7851 }, { 7686, 7687 }, { 1032, 1112 },
      { 1194, 1195 }, { 7848, 7849 }, { 7684, 7685 },
      { 1030, 1110 }, { 1192, 1193 }, { 66771, 66811 },
      { 7846, 7847 }, { 7682, 7683 }, { 1028, 1108 },
      { 1190, 1191 }, { 66769, 66809 }, { 7844, 7845 },
      { 7680, 7681 }, { 1026, 1106 }, { 1188, 1189 },
      { 66767, 66807 }, { 7842, 7843 }, { 1186, 1187 },
      { 66765, 66805 }, { 7840, 7841 }, { 1184, 1185 },
      { 66763, 66803 }, { 7838, 223 }, { 1182, 1183 },
      { 73, 305 }, { 1180, 1181 }, { 1178, 1179 },
      { 1176, 1177 }, { 1174, 1175 }, { 66753, 66793 },
      { 7828, 7829 }, { 1172, 1173 }, { 66751, 66791 },
      { 7826, 7827 }, { 1170, 1171 }, { 66749, 66789 },
      { 7824, 7825 }, { 1168, 1169 }, { 66747, 66787 },
      { 7822, 7823 }, { 1166, 1167 }, { 66745, 66785 },
      { 7820, 7821 }, { 1164, 1165 }, { 66743, 66783 },
      { 7818, 7819 }, { 1162, 1163 }, { 66741, 66781 },
      { 7816, 7817 }, { 71862, 71894 }, { 1152, 1153 },
      { 7806, 7807 }, { 71860, 71892 }, { 1150, 1151 },
      { 7804, 7805 }, { 71858, 71890 }, { 1148, 1149 },
      { 7802, 7803 }, { 71856, 71888 }, { 1146, 1147 },
      { 7800, 7801 }, { 71854, 71886 }, { 1144, 1145 },
      { 7798, 7799 }, { 71852, 71884 }, { 1142, 1143 },
      { 7796, 7797 }, { 71850, 71882 }, { 1140, 1141 },
      { 7794, 7795 }, { 71848, 71880 }, { 1138, 1139 },
      { 7792, 7793 }, { 71846, 71878 }, { 1136, 1137 },
      { 7790, 7791 }, { 71844, 71876 }, { 1134, 1135 },
      { 7788, 7789 }, { 71842, 71874 }, { 1132, 1133 },
      { 7786, 7787 }, { 71840, 71872 }, { 1130, 1131 },
      { 7784, 7785 }, { 1128, 1129 }, { 7782, 7783 },
      { 1126, 1127 }, { 7780, 7781 }, { 1124, 1125 },
      { 7778, 7779 }, { 1070, 1102 }, { 7724, 7725 },
      { 7722, 7723 }, { 7720, 7721 }, { 1064, 1096 },
      { 7718, 7719 }, { 1062, 1094 }, { 7716, 7717 },
      { 1060, 1092 }, { 7714, 7715 }, { 1058, 1090 },
      { 7712, 7713 }, { 1056, 1088 }, { 7710, 7711 },
      { 1054, 1086 }, { 7708, 7709 }, { 7706, 7707 },
      { 7704, 7705 }, { 1048, 1080 }, { 7702, 7703 },
      { 1046, 1078 }, { 7700, 7701 }, { 4288, 11552 },
      { 4286, 11550 }, { 4285, 11549 }, { 1039, 1119 },
      { 4283, 11547 }, { 1037, 1117 }, { 4282, 11546 },
      { 4280, 11544 }, { 4278, 11542 }, { 4276, 11540 },
      { 4274, 11538 }, { 42572, 42573 }, { 42570, 42571 },
      { 8185, 8057 }, { 42564, 42565 }, { 42562, 42563 },
      { 42560, 42561 }, { 11506, 11507 }, { 8171, 8059 },
      { 68759, 68823 }, { 42832, 42833 }, { 406, 617 },
      { 8169, 8161 }, { 68757, 68821 }, { 42830, 42831 },
      { 404, 611 }, { 68753, 68817 }, { 42826, 42827 },
      { 400, 603 }, { 11490, 11491 }, { 914, 946 },
      { 904, 941 }, { 902, 940 }, { 7993, 7985 },
      { 922, 954 }, { 920, 952 }, { 918, 950 },
      { 886, 887 }, { 11458, 11459 }, { 882, 883 },
      { 11456, 11457 }, { 11452, 11453 }, { 1052, 1084 },
      { 8123, 8049 }, { 11450, 11451 }, { 1050, 1082 },
      { 8121, 8113 }, { 11448, 11449 }, { 11446, 11447 },
      { 895, 1011 }, { 11444, 11445 }, { 11442, 11443 },
      { 11440, 11441 }, { 11432, 11433 }, { 125200, 125234 },
      { 7350, 4342 }, { 68765, 68829 }, { 412, 623 },
      { 42838, 42839 }, { 66570, 66610 }, { 574, 11366 },
      { 125198, 125232 }, { 7348, 4340 }, { 68763, 68827 },
      { 42836, 42837 }, { 125196, 125230 }, { 7346, 4338 },
      { 68761, 68825 }, { 408, 409 }, { 42834, 42835 },
      { 66566, 66606 }, { 570, 11365 }, { 1302, 1303 },
      { 193, 225 }, { 93778, 93810 }, { 11283, 11331 },
      { 11436, 11437 }, { 219, 251 }, { 11309, 11357 },
      { 990, 991 }, { 1360, 1408 }, { 125199, 125233 },
      { 7349, 4341 }, { 278, 279 }, { 8041, 8033 },
      { 11434, 11435 }, { 988, 989 }, { 68756, 68820 },
      { 403, 608 }, { 1068, 1100 }, { 8139, 8053 },
      { 11466, 11467 }, { 917, 949 }, { 1358, 1406 },
      { 66937, 66976 }, { 8012, 8004 }, { 125197, 125231 },
      { 7347, 4339 }, { 276, 277 }, { 1298, 1299 },
      { 93774, 93806 }, { 11279, 11327 }, { 908, 972 },
      { 7979, 7971 }, { 11306, 11354 }, { 4260, 11524 },
      { 66929, 66968 }, { 933, 965 }, { 473, 474 },
      { 1296, 1297 }, { 7950, 7942 }, { 93772, 93804 },
      { 11277, 11325 }, { 11304, 11352 }, { 11430, 11431 },
      { 984, 985 }, { 68752, 68816 }, { 399, 601 },
      { 11462, 11463 }, { 913, 945 }, { 1354, 1402 },
      { 1294, 1295 }, { 7948, 7940 }, { 93770, 93802 },
      { 11275, 11323 }, { 11302, 11350 }, { 4256, 11520 },
      { 929, 961 }, { 469, 470 }, { 11428, 11429 },
      { 1352, 1400 }, { 1292, 1293 }, { 7946, 7938 },
      { 93768, 93800 }, { 11273, 11321 }, { 11300, 11348 },
      { 11426, 11427 }, { 66932, 66971 }, { 936, 968 },
      { 4263, 11527 }, { 68748, 68812 }, { 5109, 5117 },
      { 395, 396 }, { 1350, 1398 }, { 66956, 66995 },
      { 8031, 8023 }, { 1290, 1291 }, { 7944, 7936 },
      { 93766, 93798 }, { 11271, 11319 }, { 11298, 11346 },
      { 11424, 11425 }, { 66930, 66969 }, { 934, 966 },
      { 4261, 11525 }, { 68746, 68810 }, { 5107, 5115 },
      { 393, 598 }, { 1348, 1396 }, { 66954, 66993 },
      { 8029, 8021 }, { 1288, 1289 }, { 93764, 93796 },
      { 11269, 11317 }, { 93791, 93823 }, { 11296, 11344 },
      { 11422, 11423 }, { 68744, 68808 }, { 5105, 5113 },
      { 391, 392 }, { 8154, 8054 }, { 11454, 11455 },
      { 7976, 7968 }, { 905, 942 }, { 1346, 1394 },
      { 66952, 66991 }, { 8027, 8019 }, { 1286, 1287 },
      { 93762, 93794 }, { 11267, 11315 }, { 93789, 93821 },
      { 11294, 11342 }, { 42576, 42577 }, { 1300, 1301 },
      { 93776, 93808 }, { 11281, 11329 }, { 11308, 11356 },
      { 4262, 11526 }, { 66931, 66970 }, { 935, 967 },
      { 475, 476 }, { 11420, 11421 }, { 1344, 1392 },
      { 66950, 66989 }, { 8025, 8017 }, { 125202, 125236 },
      { 7352, 4344 }, { 68767, 68831 }, { 42840, 42841 },
      { 1274, 1275 }, { 7928, 7929 }, { 192, 224 },
      { 93777, 93809 }, { 11282, 11330 }, { 437, 438 },
      { 125193, 125227 }, { 272, 273 }, { 7343, 4335 },
      { 11400, 11401 }, { 125204, 125238 }, { 7354, 4346 },
      { 68769, 68833 }, { 416, 417 }, { 42842, 42843 },
      { 1276, 1277 }, { 7930, 7931 }, { 194, 226 },
      { 93779, 93811 }, { 11284, 11332 }, { 195, 227 },
      { 1304, 1305 }, { 93780, 93812 }, { 11285, 11333 },
      { 42594, 42595 }, { 66576, 66616 }, { 580, 649 },
      { 1278, 1279 }, { 7932, 7933 }, { 196, 228 },
      { 93781, 93813 }, { 11286, 11334 }, { 1338, 1386 },
      { 125208, 125242 }, { 7358, 4350 }, { 68773, 68837 },
      { 420, 421 }, { 42846, 42847 }, { 66578, 66618 },
      { 582, 583 }, { 1280, 1281 }, { 7934, 7935 },
      { 7961, 7953 }, { 93783, 93815 }, { 11288, 11336 },
      { 7997, 7989 }, { 926, 958 }, { 1340, 1388 },
      { 66580, 66620 }, { 584, 585 }, { 1282, 1283 },
      { 7963, 7955 }, { 93785, 93817 }, { 11290, 11338 },
      { 42600, 42601 }, { 1342, 1390 }, { 1284, 1285 },
      { 93760, 93792 }, { 11265, 11313 }, { 7965, 7957 },
      { 93787, 93819 }, { 11292, 11340 }, { 11438, 11439 },
      { 4265, 11529 }, { 66934, 66973 }, { 8009, 8001 },
      { 938, 970 }, { 992, 993 }, { 65314, 65346 },
      { 68760, 68824 }, { 407, 616 }, { 8170, 8058 },
      { 7992, 7984 }, { 921, 953 }, { 7313, 4305 },
      { 381, 382 }, { 5095, 43959 }, { 1362, 1410 },
      { 125201, 125235 }, { 7351, 4343 }, { 280, 281 },
      { 8043, 8035 }, { 68754, 68818 }, { 401, 402 },
      { 1066, 1098 }, { 8137, 8051 }, { 11464, 11465 },
      { 915, 947 }, { 1356, 1404 }, { 66935, 66974 },
      { 939, 971 }, { 8010, 8002 }, { 125195, 125229 },
      { 7345, 4337 }, { 274, 275 }, { 7315, 4307 },
      { 68736, 68800 }, { 5097, 43961 }, { 1364, 1412 },
      { 125203, 125237 }, { 7353, 4345 }, { 282, 283 },
      { 8045, 8037 }, { 7317, 4309 }, { 68738, 68802 },
      { 385, 595 }, { 5099, 43963 }, { 1366, 1414 },
      { 125205, 125239 }, { 284, 285 }, { 8047, 8039 },
      { 11374, 625 }, { 68766, 68830 }, { 413, 626 },
      { 68739, 68803 }, { 42812, 42813 }, { 5100, 43964 },
      { 386, 387 }, { 11476, 11477 }, { 7998, 7990 },
      { 927, 959 }, { 7319, 4311 }, { 68740, 68804 },
      { 5101, 43965 }, { 125207, 125241 }, { 7357, 4349 },
      { 286, 287 }, { 11376, 594 }, { 68768, 68832 },
      { 415, 629 }, { 68741, 68805 }, { 42814, 42815 },
      { 5102, 43966 }, { 388, 389 }, { 11478, 11479 },
      { 7321, 4313 }, { 68742, 68806 }, { 5103, 43967 },
      { 125209, 125243 }, { 7359, 4351 }, { 288, 289 },
      { 11378, 11379 }, { 125211, 125245 }, { 290, 291 },
      { 125213, 125247 }, { 292, 293 }, { 125215, 125249 },
      { 294, 295 }, { 68776, 68840 }, { 423, 424 },
      { 8186, 8060 }, { 66933, 66972 }, { 8008, 8000 },
      { 937, 969 }, { 4264, 11528 }, { 258, 259 },
      { 7329, 4321 }, { 125217, 125251 }, { 296, 297 },
      { 68778, 68842 }, { 425, 643 }, { 68751, 68815 },
      { 42824, 42825 }, { 398, 477 }, { 11488, 11489 },
      { 260, 261 }, { 7331, 4323 }, { 298, 299 },
      { 262, 263 }, { 7333, 4325 }, { 300, 301 },
      { 11390, 575 }, { 125185, 125219 }, { 264, 265 },
      { 7335, 4327 }, { 302, 303 }, { 11392, 11393 },
      { 68784, 68848 }, { 431, 432 }, { 125187, 125221 },
      { 266, 267 }, { 7337, 4329 }, { 304, 105 },
      { 11394, 11395 }, { 68786, 68850 }, { 433, 650 },
      { 125189, 125223 }, { 268, 269 }, { 7339, 4331 },
      { 306, 307 }, { 11396, 11397 }, { 435, 436 },
      { 125191, 125225 }, { 270, 271 }, { 7341, 4333 },
      { 308, 309 }, { 11398, 11399 }, { 439, 658 },
      { 440, 441 }, { 42582, 42583 }, { 444, 445 },
      { 42586, 42587 }, { 994, 995 }, { 65316, 65348 },
      { 455, 457 }, { 996, 997 }, { 65318, 65350 },
      { 998, 999 }, { 65320, 65352 }, { 4273, 11537 },
      { 1027, 1107 }, { 202, 234 }, { 42628, 42629 },
      { 1000, 1001 }, { 65322, 65354 }, { 4275, 11539 },
      { 1029, 1109 }, { 204, 236 }, { 42630, 42631 },
      { 461, 462 }, { 1002, 1003 }, { 65324, 65356 },
      { 4277, 11541 }, { 1031, 1111 }, { 206, 238 },
      { 42632, 42633 }, { 463, 464 }, { 1004, 1005 },
      { 65326, 65358 }, { 4279, 11543 }, { 1033, 1113 },
      { 208, 240 }, { 42634, 42635 }, { 1006, 1007 },
      { 65328, 65360 }, { 4281, 11545 }, { 1035, 1115 },
      { 210, 242 }, { 42636, 42637 }, { 4284, 11548 },
      { 4287, 11551 }, { 1041, 1073 }, { 216, 248 },
      { 42642, 42643 }, { 11360, 11361 }, { 4289, 11553 },
      { 1043, 1075 }, { 218, 250 }, { 42644, 42645 },
      { 4290, 11554 }, { 1044, 1076 }, { 7698, 7699 },
      { 1017, 1010 }, { 11362, 619 }, { 4291, 11555 },
      { 1045, 1077 }, { 220, 252 }, { 42646, 42647 },
      { 1018, 1019 }, { 11364, 637 }, { 4293, 11557 },
      { 1047, 1079 }, { 222, 254 }, { 42648, 42649 },
      { 4295, 11559 }, { 42650, 42651 }, { 4268, 11532 },
      { 1022, 892 }, { 4270, 11534 }, { 1024, 1104 },
      { 1053, 1085 }, { 4272, 11536 }, { 4301, 11565 },
      { 1055, 1087 }, { 1057, 1089 }, { 1059, 1091 },
      { 1061, 1093 }, { 1063, 1095 }, { 526, 527 },
      { 9398, 9424 }, { 1069, 1101 }, { 528, 529 },
      { 9400, 9426 }, { 1071, 1103 }, { 530, 531 },
      { 9402, 9428 }, { 532, 533 }, { 9404, 9430 },
      { 9406, 9432 }, { 536, 537 }, { 9408, 9434 },
      { 538, 539 }, { 9410, 9436 }, { 9412, 9438 },
      { 9414, 9440 }, { 544, 414 }, { 9416, 9442 },
      { 546, 547 }, { 9418, 9444 }, { 548, 549 },
      { 9420, 9446 }, { 550, 551 }, { 9422, 9448 },
      { 552, 553 }, { 554, 555 }, { 556, 557 },
      { 558, 559 }, { 560, 561 }, { 562, 563 },
      { 66569, 66609 }, { 573, 410 }, { 66573, 66613 },
      { 577, 578 }, { 1120, 1121 }, { 7774, 7775 },
      { 66575, 66615 }, { 579, 384 }, { 1122, 1123 },
      { 7776, 7777 }, { 66577, 66617 }, { 581, 652 },
      { 66582, 66622 }, { 586, 587 }, { 66584, 66624 },
      { 588, 589 }, { 310, 311 }, { 5024, 43888 },
      { 66586, 66626 }, { 590, 591 }, { 5026, 43890 },
      { 7977, 7969 }, { 906, 943 }, { 7981, 7973 },
      { 910, 973 }, { 11460, 11461 }, { 7982, 7974 },
      { 911, 974 }, { 916, 948 }, { 919, 951 },
      { 7994, 7986 }, { 923, 955 }, { 7995, 7987 },
      { 924, 956 }, { 7996, 7988 }, { 925, 957 },
      { 7999, 7991 }, { 928, 960 }, { 68743, 68807 },
      { 42816, 42817 }, { 5104, 5112 }, { 390, 596 },
      { 8153, 8145 }, { 11480, 11481 }, { 931, 963 },
      { 4258, 11522 }, { 68747, 68811 }, { 42820, 42821 },
      { 5108, 5116 }, { 394, 599 }, { 11484, 11485 },
      { 986, 987 }, { 471, 472 }, { 1012, 952 },
      { 1015, 1016 }, { 4267, 11531 }, { 1021, 891 },
      { 4269, 11533 }, { 198, 230 }, { 42624, 42625 },
      { 1023, 893 }, { 4271, 11535 }, { 200, 232 },
      { 42626, 42627 }, { 1025, 1105 }, { 7726, 7727 },
      { 7728, 7729 }, { 7730, 7731 }, { 7732, 7733 },
      { 7734, 7735 }, { 7736, 7737 }, { 7738, 7739 },
      { 7740, 7741 }, { 7742, 7743 }, { 7744, 7745 },
      { 7746, 7747 }, { 7748, 7749 }, { 7750, 7751 },
      { 7752, 7753 }, { 7754, 7755 }, { 7756, 7757 },
      { 7758, 7759 }, { 7760, 7761 }, { 7762, 7763 },
      { 7764, 7765 }, { 7766, 7767 }, { 7768, 7769 },
      { 7770, 7771 }, { 7772, 7773 }, { 7808, 7809 },
      { 7810, 7811 }, { 66737, 66777 }, { 7812, 7813 },
      { 66739, 66779 }, { 7814, 7815 }, { 7872, 7873 },
      { 7874, 7875 }, { 7876, 7877 }, { 7878, 7879 },
      { 7880, 7881 }, { 7882, 7883 }, { 7884, 7885 },
      { 7945, 7937 }, { 93767, 93799 }, { 11272, 11320 },
      { 7947, 7939 }, { 93769, 93801 }, { 11274, 11322 },
      { 7949, 7941 }, { 93771, 93803 }, { 11276, 11324 },
      { 880, 881 }, { 7951, 7943 }, { 93773, 93805 },
      { 11278, 11326 }, { 66965, 67004 }, { 8040, 8032 },
      { 11367, 11368 }, { 8042, 8034 }, { 11369, 11370 },
      { 8044, 8036 }, { 11371, 11372 }, { 975, 983 },
      { 8046, 8038 }, { 11373, 593 }, { 1049, 1081 },
      { 8120, 8112 }, { 1051, 1083 }, { 8122, 8048 },
      { 1065, 1097 }, { 8136, 8050 }, { 1067, 1099 },
      { 8138, 8052 }, { 8152, 8144 }, { 8155, 8055 },
      { 11482, 11483 }, { 8168, 8160 }, { 8172, 8165 },
      { 11499, 11500 }, { 8184, 8056 }, { 8187, 8061 },
      { 42566, 42567 }, { 8486, 969 }, { 8490, 107 },
      { 8491, 229 }, { 8498, 8526 }, { 42877, 7545 },
      { 8544, 8560 }, { 497, 499 }, { 42923, 604 },
      { 8545, 8561 }, { 42924, 609 }, { 8546, 8562 },
      { 42925, 620 }, { 8547, 8563 }, { 500, 501 },
      { 42926, 618 }, { 8548, 8564 }, { 8549, 8565 },
      { 502, 405 }, { 42928, 670 }, { 8550, 8566 },
      { 503, 447 }, { 42929, 647 }, { 8551, 8567 },
      { 504, 505 }, { 42930, 669 }, { 8552, 8568 },
      { 42931, 43859 }, { 8553, 8569 }, { 506, 507 },
      { 42932, 42933 }, { 8554, 8570 }, { 8555, 8571 },
      { 508, 509 }, { 42934, 42935 }, { 8556, 8572 },
      { 8557, 8573 }, { 510, 511 }, { 42936, 42937 },
      { 8558, 8574 }, { 8559, 8575 }, { 512, 513 },
      { 42938, 42939 }, { 8579, 8580 }, { 9399, 9425 },
      { 9401, 9427 }, { 9403, 9429 }, { 9405, 9431 },
      { 9407, 9433 }, { 9409, 9435 }, { 9411, 9437 },
      { 9413, 9439 }, { 9415, 9441 }, { 9417, 9443 },
      { 9419, 9445 }, { 9421, 9447 }, { 9423, 9449 },
      { 11264, 11312 }, { 93761, 93793 }, { 11266, 11314 },
      { 93763, 93795 }, { 11268, 11316 }, { 93765, 93797 },
      { 11270, 11318 }, { 93775, 93807 }, { 11280, 11328 },
      { 4292, 11556 }, { 11363, 7549 }, { 11375, 592 },
      { 11381, 11382 }, { 11391, 576 }, { 11402, 11403 },
      { 11404, 11405 }, { 11406, 11407 }, { 11408, 11409 },
      { 11410, 11411 }, { 11412, 11413 }, { 11414, 11415 },
      { 11416, 11417 }, { 11418, 11419 }, { 11468, 11469 },
      { 11470, 11471 }, { 11472, 11473 }, { 11474, 11475 },
      { 11486, 11487 }, { 11501, 11502 }, { 42568, 42569 },
      { 42574, 42575 }, { 42578, 42579 }, { 42580, 42581 },
      { 42584, 42585 }, { 42588, 42589 }, { 42590, 42591 },
      { 42592, 42593 }, { 42596, 42597 }, { 42598, 42599 },
      { 42602, 42603 }, { 42604, 42605 }, { 212, 244 },
      { 42638, 42639 }, { 214, 246 }, { 42640, 42641 },
      { 366, 367 }, { 5080, 43944 }, { 42792, 42793 },
      { 368, 369 }, { 5082, 43946 }, { 42794, 42795 },
      { 374, 375 }, { 5088, 43952 }, { 370, 371 },
      { 5084, 43948 }, { 42796, 42797 }, { 42802, 42803 },
      { 376, 255 }, { 5090, 43954 }, { 372, 373 },
      { 5086, 43950 }, { 42798, 42799 }, { 42804, 42805 },
      { 5092, 43956 }, { 68771, 68835 }, { 418, 419 },
      { 42844, 42845 }, { 68775, 68839 }, { 422, 640 },
      { 42848, 42849 }, { 68777, 68841 }, { 42850, 42851 },
      { 68779, 68843 }, { 42852, 42853 }, { 68781, 68845 },
      { 428, 429 }, { 42854, 42855 }, { 68783, 68847 },
      { 430, 648 }, { 42856, 42857 }, { 68785, 68849 },
      { 42858, 42859 }, { 434, 651 }, { 42860, 42861 },
      { 42862, 42863 }, { 42873, 42874 }, { 42875, 42876 },
      { 452, 454 }, { 42878, 42879 }, { 42880, 42881 },
      { 42882, 42883 }, { 458, 460 }, { 42884, 42885 },
      { 42886, 42887 }, { 465, 466 }, { 42891, 42892 },
      { 467, 468 }, { 42893, 613 }, { 42896, 42897 },
      { 42898, 42899 }, { 42902, 42903 }, { 478, 479 },
      { 42904, 42905 }, { 480, 481 }, { 42906, 42907 },
      { 482, 483 }, { 42908, 42909 }, { 484, 485 },
      { 42910, 42911 }, { 486, 487 }, { 42912, 42913 },
      { 488, 489 }, { 42914, 42915 }, { 490, 491 },
      { 42916, 42917 }, { 492, 493 }, { 42918, 42919 },
      { 494, 495 }, { 42920, 42921 }, { 42922, 614 },
      { 514, 515 }, { 42940, 42941 }, { 516, 517 },
      { 42942, 42943 }, { 518, 519 }, { 42944, 42945 },
      { 520, 521 }, { 42946, 42947 }, { 522, 523 },
      { 42948, 42900 }, { 42949, 642 }, { 524, 525 },
      { 42950, 7566 }, { 42951, 42952 }, { 42953, 42954 },
      { 534, 535 }, { 42960, 42961 }, { 540, 541 },
      { 42966, 42967 }, { 542, 543 }, { 42968, 42969 },
      { 66567, 66607 }, { 571, 572 }, { 42997, 42998 },
      { 5025, 43889 }, { 313, 314 }, { 5027, 43891 },
      { 5028, 43892 }, { 315, 316 }, { 5029, 43893 },
      { 5030, 43894 }, { 317, 318 }, { 5031, 43895 },
      { 5032, 43896 }, { 319, 320 }, { 5033, 43897 },
      { 5034, 43898 }, { 321, 322 }, { 5035, 43899 },
      { 5036, 43900 }, { 323, 324 }, { 5037, 43901 },
      { 5038, 43902 }, { 325, 326 }, { 5039, 43903 },
      { 5040, 43904 }, { 327, 328 }, { 5041, 43905 },
      { 5042, 43906 }, { 5043, 43907 }, { 330, 331 },
      { 5044, 43908 }, { 5045, 43909 }, { 332, 333 },
      { 5046, 43910 }, { 5047, 43911 }, { 334, 335 },
      { 5048, 43912 }, { 5049, 43913 }, { 336, 337 },
      { 5050, 43914 }, { 5051, 43915 }, { 338, 339 },
      { 5052, 43916 }, { 5053, 43917 }, { 340, 341 },
      { 5054, 43918 }, { 5055, 43919 }, { 342, 343 },
      { 5056, 43920 }, { 5057, 43921 }, { 344, 345 },
      { 5058, 43922 }, { 5059, 43923 }, { 346, 347 },
      { 5060, 43924 }, { 5061, 43925 }, { 348, 349 },
      { 5062, 43926 }, { 5063, 43927 }, { 350, 351 },
      { 5064, 43928 }, { 5065, 43929 }, { 352, 353 },
      { 5066, 43930 }, { 5067, 43931 }, { 354, 355 },
      { 5068, 43932 }, { 5069, 43933 }, { 356, 357 },
      { 5070, 43934 }, { 5071, 43935 }, { 358, 359 },
      { 5072, 43936 }, { 5073, 43937 }, { 360, 361 },
      { 42786, 42787 }, { 5074, 43938 }, { 5075, 43939 },
      { 362, 363 }, { 42788, 42789 }, { 5076, 43940 },
      { 5077, 43941 }, { 364, 365 }, { 42790, 42791 },
      { 5078, 43942 }, { 5079, 43943 }, { 5081, 43945 },
      { 5083, 43947 }, { 5085, 43949 }, { 5087, 43951 },
      { 5089, 43953 }, { 377, 378 }, { 5091, 43955 },
      { 379, 380 }, { 5093, 43957 }, { 65313, 65345 },
      { 65315, 65347 }, { 65317, 65349 }, { 65319, 65351 },
      { 65321, 65353 }, { 65323, 65355 }, { 65325, 65357 },
      { 65327, 65359 }, { 65329, 65361 }, { 65330, 65362 },
    }};

    if (c >= 'A' && c <= 'Z')
    {
      return c + 32;
    }

    return utils::case_lookup(tolower_map, c);
  }
}
