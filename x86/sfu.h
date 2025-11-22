#pragma once

inline float exp2f_sfu(float xx) {
  constexpr uint32_t exp2_table[][3] = {
      {0x3e775402, 0x3e571e37, 0x3f0c6372},
      {0x3e7a057a, 0x3e51a5b8, 0x3f0d153f},
      {0x3e7cbe73, 0x3e4c0831, 0x3f0dce8e},
      {0x3e7f7f03, 0x3e4644ff, 0x3f0e8f9a},
      {0x3e8123a0, 0x3e405b7a, 0x3f0f589f},
      {0x3e828b9f, 0x3e3a4afa, 0x3f1029d9},
      {0x3e83f789, 0x3e3412d3, 0x3f110387},
      {0x3e85676a, 0x3e2db258, 0x3f11e5ea},
      {0x3e86db4d, 0x3e2728d9, 0x3f12d141},
      {0x3e88533c, 0x3e2075a1, 0x3f13c5cf},
      {0x3e89cf43, 0x3e1997fb, 0x3f14c3d9},
      {0x3e8b4f6d, 0x3e128f2f, 0x3f15cba4},
      {0x3e8cd3c7, 0x3e0b5a81, 0x3f16dd77},
      {0x3e8e5c5b, 0x3e03f933, 0x3f17f99b},
      {0x3e8fe935, 0x3df8d50b, 0x3f192058},
      {0x3e917a62, 0x3de95b68, 0x3f1a51fc},
      {0x3e930fed, 0x3dd983ef, 0x3f1b8ed3},
      {0x3e94a9e2, 0x3dc94d0f, 0x3f1cd72b},
      {0x3e96484f, 0x3db8b52d, 0x3f1e2b56},
      {0x3e97eb3e, 0x3da7baac, 0x3f1f8ba5},
      {0x3e9992be, 0x3d965be7, 0x3f20f86d},
      {0x3e9b3eda, 0x3d849732, 0x3f227202},
      {0x3e9cef9f, 0x3d64d5b8, 0x3f23f8bd},
      {0x3e9ea51b, 0x3d3faa5b, 0x3f258cf6},
      {0x3ea05f5a, 0x3d19a8d1, 0x3f272f08},
      {0x3ea21e6b, 0x3ce59b1f, 0x3f28df51},
      {0x3ea3e259, 0x3c962a03, 0x3f2a9e2f},
      {0x3ea5ab33, 0x3c09ee08, 0x3f2c6c02},
      {0x3ea77907, 0xbae053f9, 0x3f2e492e},
      {0x3ea94be3, 0xbc45a48d, 0x3f303618},
      {0x3eab23d3, 0xbcb977ab, 0x3f323325},
      {0x3ead00e8, 0xbd08fe92, 0x3f3440bf},
      {0x3eaee32e, 0xbd363542, 0x3f365f51},
      {0x3eb0cab5, 0xbd6463de, 0x3f388f48},
      {0x3eb2b78b, 0xbd89c737, 0x3f3ad113},
      {0x3eb4a9be, 0xbda1dc83, 0x3f3d2524},
      {0x3eb6a15f, 0xbdba73e5, 0x3f3f8bee},
      {0x3eb89e7b, 0xbdd38f79, 0x3f4205e8},
      {0x3ebaa123, 0xbded315f, 0x3f44938b},
      {0x3ebca965, 0xbe03ade1, 0x3f473550},
      {0x3ebeb751, 0xbe11086a, 0x3f49ebb6},
      {0x3ec0caf8, 0xbe1ea966, 0x3f4cb73c},
      {0x3ec2e469, 0xbe2c91f8, 0x3f4f9864},
      {0x3ec503b4, 0xbe3ac345, 0x3f528fb3},
      {0x3ec728e9, 0xbe493e75, 0x3f559db2},
      {0x3ec9541a, 0xbe5804b6, 0x3f58c2e9},
      {0x3ecb8556, 0xbe67173b, 0x3f5bffe5},
      {0x3ecdbcae, 0xbe767739, 0x3f5f5537},
      {0x3ecffa35, 0xbe8312f6, 0x3f62c371},
      {0x3ed23dfa, 0xbe8b124a, 0x3f664b27},
      {0x3ed4880f, 0xbe933a39, 0x3f69ecf2},
      {0x3ed6d886, 0xbe9b8b69, 0x3f6da96d},
      {0x3ed92f70, 0xbea4067f, 0x3f718135},
      {0x3edb8ce1, 0xbeacac25, 0x3f7574ed},
      {0x3eddf0e9, 0xbeb57d07, 0x3f798537},
      {0x3ee05b9b, 0xbebe79d1, 0x3f7db2bb},
      {0x3ee2cd09, 0xbec7a334, 0x3f80ff12},
      {0x3ee54548, 0xbed0f9e4, 0x3f833410},
      {0x3ee7c468, 0xbeda7e95, 0x3f8578b0},
      {0x3eea4a7f, 0xbee431ff, 0x3f87cd4b},
      {0x3eecd79e, 0xbeee14de, 0x3f8a323e},
      {0x3eef6bdb, 0xbef827ed, 0x3f8ca7e7},
      {0x3ef20747, 0xbf0135f7, 0x3f8f2ea4},
      {0x3ef4a9f8, 0xbf0670d1, 0x3f91c6d8},
  };

  if (std::isnan(xx)) {  // 输入为nan，则输出nan
    return xx;
  }
  if (xx > 127.0f) {  // 输入大于门限，则输出为inf
    return std::numeric_limits<float>::infinity();
  }
  if (xx < -127.0f) {  // 输入小于门限，则输出为0
    return 0.0f;
  }

  // 1) 缩小范围
  // 2^x = 2^(i + f) = 2^i * 2^f
  constexpr float offset = 1.0;
  float px = floorf(xx);
  asm volatile("" ::: "memory");
  float x = xx - px + offset;
  // 2）多项式拟合（泰勒级数展开）近似计算
  constexpr int m = 6;
  int x1 = (((*(uint32_t *)(&x)) & 0x7FFFFF) >> (23 - m));  // 取23bit尾数的高m bit
  float C2 = *(float *)(&exp2_table[x1][0]);
  float C1 = *(float *)(&exp2_table[x1][1]);
  float C0 = *(float *)(&exp2_table[x1][2]);
  float y = C2 * x * x + C1 * x + C0;
  // 3）恢复范围
  x = ldexpf(y, px);
  return x;
}

inline float logf_sfu(float xx) {
  constexpr uint32_t log_table[][3] = {
      {0xbffc0d8c, 0x407e0534, 0xc00bdd6e},
      {0xbff46a4b, 0x407a245a, 0xc00ae15a},
      {0xbfed1e95, 0x4076615d, 0xc009e918},
      {0xbfe62545, 0x4072baea, 0xc008f48a},
      {0xbfdf7993, 0x406f2fbf, 0xc0080395},
      {0xbfd91710, 0x406bbeaf, 0xc007161e},
      {0xbfd2f99d, 0x4068669e, 0xc0062c0c},
      {0xbfcd1d63, 0x40652680, 0xc0054546},
      {0xbfc77ed0, 0x4061fd56, 0xc00461b5},
      {0xbfc21a8e, 0x405eea32, 0xc0038141},
      {0xbfbced82, 0x405bec2f, 0xc002a3d6},
      {0xbfb7f4c4, 0x40590277, 0xc001c95f},
      {0xbfb32d9b, 0x40562c3e, 0xc000f1c9},
      {0xbfae957d, 0x405368c3, 0xc0001cfe},
      {0xbfaa2a09, 0x4050b74f, 0xbffe95dd},
      {0xbfa5e902, 0x404e1733, 0xbffcf70f},
      {0xbfa1d051, 0x404b87ca, 0xbffb5d71},
      {0xbf9dddfd, 0x40490877, 0xbff9c8e1},
      {0xbf9a102d, 0x404698a3, 0xbff83941},
      {0xbf966525, 0x404437c1, 0xbff6ae71},
      {0xbf92db41, 0x4041e548, 0xbff52855},
      {0xbf8f70f6, 0x403fa0b7, 0xbff3a6cf},
      {0xbf8c24d2, 0x403d6993, 0xbff229c5},
      {0xbf88f574, 0x403b3f66, 0xbff0b11d},
      {0xbf85e191, 0x403921be, 0xbfef3cbc},
      {0xbf82e7f2, 0x40371032, 0xbfedcc8b},
      {0xbf80076f, 0x40350a59, 0xbfec6071},
      {0xbf7a7ddf, 0x40330fd2, 0xbfeaf857},
      {0xbf751ad8, 0x4031203f, 0xbfe99427},
      {0xbf6fe3d4, 0x402f3b45, 0xbfe833cc},
      {0xbf6ad6f7, 0x402d6090, 0xbfe6d731},
      {0xbf65f282, 0x402b8fcb, 0xbfe57e41},
      {0xbf6134c9, 0x4029c8a9, 0xbfe428e9},
      {0xbf5c9c37, 0x40280adc, 0xbfe2d715},
      {0xbf58274c, 0x4026561d, 0xbfe188b5},
      {0xbf53d49b, 0x4024aa25, 0xbfe03db5},
      {0xbf4fa2ca, 0x402306b2, 0xbfdef604},
      {0xbf4b908d, 0x40216b82, 0xbfddb192},
      {0xbf479cac, 0x401fd859, 0xbfdc704f},
      {0xbf43c5fb, 0x401e4cfa, 0xbfdb322a},
      {0xbf400b5d, 0x401cc92c, 0xbfd9f714},
      {0xbf3c6bc4, 0x401b4cb8, 0xbfd8befe},
      {0xbf38e62c, 0x4019d769, 0xbfd789da},
      {0xbf3579a0, 0x4018690c, 0xbfd6579a},
      {0xbf322534, 0x40170170, 0xbfd52830},
      {0xbf2ee807, 0x4015a066, 0xbfd3fb8e},
      {0xbf2bc143, 0x401445bf, 0xbfd2d1a9},
      {0xbf28b01b, 0x4012f151, 0xbfd1aa72},
      {0xbf25b3cb, 0x4011a2ef, 0xbfd085de},
      {0xbf22cb98, 0x40105a72, 0xbfcf63e1},
      {0xbf1ff6cf, 0x400f17b2, 0xbfce446f},
      {0xbf1d34c5, 0x400dda89, 0xbfcd277d},
      {0xbf1a84d4, 0x400ca2d2, 0xbfcc0cff},
      {0xbf17e662, 0x400b7069, 0xbfcaf4ec},
      {0xbf1558d6, 0x400a432b, 0xbfc9df39},
      {0xbf12dba1, 0x40091af9, 0xbfc8cbdb},
      {0xbf106e38, 0x4007f7b1, 0xbfc7bac9},
      {0xbf0e1017, 0x4006d935, 0xbfc6abf8},
      {0xbf0bc0bf, 0x4005bf66, 0xbfc59f5f},
      {0xbf097fb5, 0x4004aa27, 0xbfc494f6},
      {0xbf074c84, 0x4003995c, 0xbfc38cb3},
      {0xbf0526bc, 0x40028ceb, 0xbfc2868d},
      {0xbf030df1, 0x400184b8, 0xbfc1827b},
      {0xbf0101ba, 0x400080a9, 0xbfc08076},
  };
  if (std::isinf(xx) && xx > 0.0f)  // 输入+inf，则输出+inf
  {
    return xx;
  }
  if (std::isnan(xx))  // 输入nan，则输出nan
  {
    return xx;
  }
  if (xx == 0.0f)  // 输入为0，则输出-inf
  {
    return -std::numeric_limits<float>::infinity();
  }
  if (xx < 0.0f)  // 输入小于0，则输出nan
  {
    return -std::numeric_limits<float>::quiet_NaN();
  }
  // 1) 缩小范围
  // log(x) = log(2^e * m) = e*log(2) + log(m)
  int e;
  float x = frexpf(xx, &e);  // 将单精度浮点数分解为归一化尾数和指数。x = sign*2^e*m，其中，m是归一化尾数，范围在[0.5,1)；e是指数
  // 多项式拟合（泰勒级数展开）近似计算log(m)
  constexpr int m = 6;
  int x1 = (((*(uint32_t *)(&x)) & 0x7FFFFF) >> (23 - m));  // 取23bit尾数的高m bit
  float C2 = *(float *)(&log_table[x1][0]);
  float C1 = *(float *)(&log_table[x1][1]);
  float C0 = *(float *)(&log_table[x1][2]);
  float y = C2 * x * x + C1 * x + C0;
  // 3）恢复范围，计算e*log(2) + log(m)
  y = y + 0.69314717556f * (float)e;
  return y;
}

constexpr uint32_t sin_table[][3] = {
    {0xbb7ffe8d, 0x3f8100cb, 0xbf8080cc},
    {0xbc3ffb29, 0x3f8308b9, 0xbf8188d4},
    {0xbc9ff558, 0x3f851875, 0xbf8298f1},
    {0xbcdfe31c, 0x3f872fdc, 0xbf83b131},
    {0xbd0fe171, 0x3f894eca, 0xbf84d1a0},
    {0xbd2fc856, 0x3f8b7519, 0xbf85fa47},
    {0xbd4fa43e, 0x3f8da2a1, 0xbf872b2e},
    {0xbd6f732c, 0x3f8fd73a, 0xbf88645b},
    {0xbd879992, 0x3f9212b9, 0xbf89a5d3},
    {0xbd977114, 0x3f9454f2, 0xbf8aef98},
    {0xbda73f1f, 0x3f969dba, 0xbf8c41ac},
    {0xbdb702b6, 0x3f98ece0, 0xbf8d9c0c},
    {0xbdc6badd, 0x3f9b4235, 0xbf8efeb7},
    {0xbdd66698, 0x3f9d9d88, 0xbf9069a7},
    {0xbde604ed, 0x3f9ffea7, 0xbf91dcd6},
    {0xbdf594e2, 0x3fa2655d, 0xbf93583a},
    {0xbe028abf, 0x3fa4d174, 0xbf94dbc9},
    {0xbe0a42e4, 0x3fa742b7, 0xbf966776},
    {0xbe11f265, 0x3fa9b8ee, 0xbf97fb31},
    {0xbe1998c7, 0x3fac33df, 0xbf9996e9},
    {0xbe21358f, 0x3faeb350, 0xbf9b3a8c},
    {0xbe28c844, 0x3fb13705, 0xbf9ce602},
    {0xbe30506d, 0x3fb3bec2, 0xbf9e9935},
    {0xbe37cd91, 0x3fb64a48, 0xbfa0540a},
    {0xbe3f3f38, 0x3fb8d959, 0xbfa21665},
    {0xbe46a4eb, 0x3fbb6bb3, 0xbfa3e028},
    {0xbe4dfe33, 0x3fbe0116, 0xbfa5b131},
    {0xbe554a9c, 0x3fc0993e, 0xbfa7895d},
    {0xbe5c89b1, 0x3fc333e8, 0xbfa96887},
    {0xbe63bafd, 0x3fc5d0d0, 0xbfab4e87},
    {0xbe6ade0d, 0x3fc86faf, 0xbfad3b33},
    {0xbe71f26f, 0x3fcb103e, 0xbfaf2e5d},
    {0xbe78f7b2, 0x3fcdb236, 0xbfb127d6},
    {0xbe7fed66, 0x3fd0554e, 0xbfb3276e},
    {0xbe83698e, 0x3fd2f93c, 0xbfb52cf0},
    {0xbe86d432, 0x3fd59db6, 0xbfb73825},
    {0xbe8a3669, 0x3fd8426f, 0xbfb948d6},
    {0xbe8d8ffc, 0x3fdae71c, 0xbfbb5ec6},
    {0xbe90e0b7, 0x3fdd8b70, 0xbfbd79b7},
    {0xbe942863, 0x3fe02f1b, 0xbfbf996b},
    {0xbe9766cd, 0x3fe2d1d0, 0xbfc1bd9d},
    {0xbe9a9bc1, 0x3fe5733e, 0xbfc3e609},
    {0xbe9dc70b, 0x3fe81316, 0xbfc61266},
    {0xbea0e878, 0x3feab106, 0xbfc8426c},
    {0xbea3ffd7, 0x3fed4cbd, 0xbfca75cd},
    {0xbea70cf7, 0x3fefe5e8, 0xbfccac3b},
    {0xbeaa0fa5, 0x3ff27c34, 0xbfcee564},
    {0xbead07b3, 0x3ff50f4e, 0xbfd120f3},
    {0xbeaff4f0, 0x3ff79ee2, 0xbfd35e93},
    {0xbeb2d72d, 0x3ffa2a9b, 0xbfd59dec},
    {0xbeb5ae3e, 0x3ffcb224, 0xbfd7dea0},
};
constexpr uint32_t cos_table[][3] = {
    {0xbefffdb7, 0x3f7ffdb4, 0x3f000127},
    {0xbeffedb7, 0x3f7fed70, 0x3f00096c},
    {0xbeffcdb9, 0x3f7fcc6c, 0x3f001a75},
    {0xbeff9dbd, 0x3f7f9a2c, 0x3f0034c5},
    {0xbeff5dc8, 0x3f7f5633, 0x3f0058e4},
    {0xbeff0ddd, 0x3f7f0005, 0x3f00875b},
    {0xbefeae02, 0x3f7e9728, 0x3f00c0b7},
    {0xbefe3e3b, 0x3f7e1b23, 0x3f010585},
    {0xbefdbe91, 0x3f7d8b7f, 0x3f015654},
    {0xbefd2f0b, 0x3f7ce7c5, 0x3f01b3b7},
    {0xbefc8fb2, 0x3f7c2f81, 0x3f021e41},
    {0xbefbe090, 0x3f7b6241, 0x3f029688},
    {0xbefb21b0, 0x3f7a7f92, 0x3f031d22},
    {0xbefa531f, 0x3f798706, 0x3f03b2aa},
    {0xbef974e8, 0x3f78782e, 0x3f0457b8},
    {0xbef8871a, 0x3f77529f, 0x3f050cea},
    {0xbef789c3, 0x3f7615ee, 0x3f05d2db},
    {0xbef67cf4, 0x3f74c1b4, 0x3f06aa2c},
    {0xbef560bd, 0x3f735589, 0x3f07937b},
    {0xbef43530, 0x3f71d10a, 0x3f088f68},
    {0xbef2fa61, 0x3f7033d4, 0x3f099e97},
    {0xbef1b061, 0x3f6e7d88, 0x3f0ac1a9},
    {0xbef05746, 0x3f6cadc8, 0x3f0bf941},
    {0xbeeeef27, 0x3f6ac438, 0x3f0d4605},
    {0xbeed7818, 0x3f68c07f, 0x3f0ea897},
    {0xbeebf232, 0x3f66a247, 0x3f10219e},
    {0xbeea5d8c, 0x3f64693a, 0x3f11b1bf},
    {0xbee8ba41, 0x3f621507, 0x3f13599f},
    {0xbee7086b, 0x3f5fa55e, 0x3f1519e4},
    {0xbee54824, 0x3f5d19f2, 0x3f16f333},
    {0xbee37988, 0x3f5a727a, 0x3f18e634},
    {0xbee19cb5, 0x3f57aeac, 0x3f1af38a},
    {0xbedfb1c9, 0x3f54ce45, 0x3f1d1bdb},
    {0xbeddb8e1, 0x3f51d101, 0x3f1f5fcc},
    {0xbedbb21e, 0x3f4eb6a2, 0x3f21c000},
    {0xbed99da0, 0x3f4b7eeb, 0x3f243d1c},
    {0xbed77b88, 0x3f4829a1, 0x3f26d7c1},
    {0xbed54bf8, 0x3f44b68e, 0x3f299092},
    {0xbed30f13, 0x3f41257e, 0x3f2c682f},
    {0xbed0c4fe, 0x3f3d7640, 0x3f2f5f37},
    {0xbece6ddd, 0x3f39a8a6, 0x3f327648},
    {0xbecc09d4, 0x3f35bc84, 0x3f35adff},
    {0xbec9990c, 0x3f31b1b4, 0x3f3906f8},
    {0xbec71ba9, 0x3f2d880f, 0x3f3c81cb},
    {0xbec491d5, 0x3f293f76, 0x3f401f10},
    {0xbec1fbb8, 0x3f24d7c9, 0x3f43df5e},
    {0xbebf597b, 0x3f2050ed, 0x3f47c346},
    {0xbebcab49, 0x3f1baaca, 0x3f4bcb5c},
    {0xbeb9f14c, 0x3f16e54c, 0x3f4ff82f},
    {0xbeb72bb0, 0x3f120061, 0x3f544a4a},
    {0xbeb45aa2, 0x3f0cfbfc, 0x3f58c238},
};
inline float sinf_sfu(float xx) {
  constexpr float FOPI = 1.27323954473516;
  constexpr float T24M1 = 16777215.;
  if (std::isinf(xx))  // 输入为inf，则输出-nan
  {
    return -std::numeric_limits<float>::quiet_NaN();
  }
  if (std::isnan(xx))  // 输入为nan，则输出nan
  {
    return xx;
  }
  if (fabs(xx) > T24M1) {
    return 0.0f;
  }
  if (fabs(xx) <= 1.0e-4f) {
    return xx;
  }
  int sign = 1;
  float x = xx;
  if (xx < 0) {  // 取绝对值
    sign = -1;
    x = -xx;
  }
  // 1）缩小范围
  // 把一个圆平均分成8份，根据sin性质，缩小范围到[0,pi/4)或[pi/2,3*pi/4)
  int j = FOPI * x;  // 即j=floor(x/(PI/4))。把[0,2*pi)平均分成8个区间，j表示落在哪个区间
  float y = j;
  j &= 7;  // 对应性质：sin(x) = sin(x + 2*pi)
  constexpr float offset = 1.0;
  const uint32_t (*table)[3];
  switch (j) {
    case 0:
      x = x - y * PIO4F + offset;
      table = sin_table;
      break;
    case 1:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = cos_table;
      break;
    case 2:
      x = x - y * PIO4F + offset;
      table = cos_table;
      break;
    case 3:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = sin_table;
      break;
    case 4:
      x = x - y * PIO4F + offset;
      table = sin_table;
      sign = -sign;
      break;
    case 5:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = cos_table;
      sign = -sign;
      break;
    case 6:
      x = x - y * PIO4F + offset;
      table = cos_table;
      sign = -sign;
      break;
    default:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = sin_table;
      sign = -sign;
      break;
  }
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  constexpr int m = 6;
  int x1 = (((*(uint32_t *)(&x)) & 0x7FFFFF) >> (23 - m));  // 取23bit尾数的高m bit
  float C2 = *(float *)(&table[x1][0]);
  float C1 = *(float *)(&table[x1][1]);
  float C0 = *(float *)(&table[x1][2]);
  y = C2 * x * x + C1 * x + C0;
  // 3）符号位补偿
  if (sign < 0) {
    y = -y;
  }
  return (y);
}

inline float cosf_sfu(float xx) {
  constexpr float FOPI = 1.27323954473516;
  constexpr float T24M1 = 16777215.;
  if (std::isinf(xx))  // 输入为inf，则输出-nan
  {
    return -std::numeric_limits<float>::quiet_NaN();
  }
  if (std::isnan(xx))  // 输入为nan，则输出nan
  {
    return xx;
  }
  if (fabs(xx) > T24M1) {
    return 1.0f;
  }
  if (fabs(xx) <= 1.0e-4f) {
    return 1.0f;
  }
  int sign = 1;
  float x = xx;
  if (xx < 0) {  // 取绝对值
    x = -xx;
  }
  // 1）缩小范围
  int j = FOPI * x;  // 即j=floor(x/(PI/4))。把[0,2*pi)平均分成8个区间，j表示落在哪个区间
  float y = j;
  j &= 7;
  constexpr float offset = 1.0;
  const uint32_t (*table)[3];
  switch (j) {
    case 0:
      x = x - y * PIO4F + offset;
      table = cos_table;
      break;
    case 1:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = sin_table;
      break;
    case 2:
      x = x - y * PIO4F + offset;
      table = sin_table;
      sign = -sign;
      break;
    case 3:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = cos_table;
      sign = -sign;
      break;
    case 4:
      x = x - y * PIO4F + offset;
      table = cos_table;
      sign = -sign;
      break;
    case 5:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = sin_table;
      sign = -sign;
      break;
    case 6:
      x = x - y * PIO4F + offset;
      table = sin_table;
      break;
    default:
      x = PIO4F - (x - y * PIO4F) + offset;
      table = cos_table;
      break;
  }
  // 2）多项式拟合（泰勒级数展开）近似计算cos(x)
  constexpr int m = 6;
  int x1 = (((*(uint32_t *)(&x)) & 0x7FFFFF) >> (23 - m));  // 取23bit尾数的高m bit
  float C2 = *(float *)(&table[x1][0]);
  float C1 = *(float *)(&table[x1][1]);
  float C0 = *(float *)(&table[x1][2]);
  y = C2 * x * x + C1 * x + C0;
  // 3）符号位补偿
  if (sign < 0) {
    y = -y;
  }
  return (y);
}

inline float rcp_sfu(float xx) {
  constexpr uint32_t rcp_table[][3] = {
      {0x40fa1b00, 0xc13d0b28, 0x40be8467},
      {0x40eed2ea, 0xc13750a8, 0x40bb9bbf},
      {0x40e435f3, 0xc131d7d2, 0x40b8c97c},
      {0x40da379e, 0xc12c9cc8, 0x40b60c9f},
      {0x40d0cc81, 0xc1279bf7, 0x40b3643a},
      {0x40c7ea2c, 0xc122d209, 0x40b0cf6a},
      {0x40bf8710, 0xc11e3be8, 0x40ae4d59},
      {0x40b79a66, 0xc119d6b3, 0x40abdd3e},
      {0x40b01c1f, 0xc1159fbc, 0x40a97e5b},
      {0x40a904d4, 0xc1119482, 0x40a72ffc},
      {0x40a24db3, 0xc10db2b1, 0x40a4f177},
      {0x409bf072, 0xc109f81b, 0x40a2c229},
      {0x4095e747, 0xc10662b5, 0x40a0a17c},
      {0x40902cd5, 0xc102f099, 0x409e8edd},
      {0x408abc2a, 0xc0ff3ff6, 0x409c89c3},
      {0x408590b0, 0xc0f8de61, 0x409a91ab},
      {0x4080a62a, 0xc0f2b94c, 0x4098a61a},
      {0x4077f14e, 0xc0eccdc4, 0x4096c699},
      {0x406f0900, 0xc0e71903, 0x4094f2b8},
      {0x40668ca1, 0xc0e1986d, 0x40932a0c},
      {0x405e75e1, 0xc0dc498f, 0x40916c2f},
      {0x4056bee1, 0xc0d72a17, 0x408fb8c1},
      {0x404f6229, 0xc0d237d8, 0x408e0f64},
      {0x40485a9d, 0xc0cd70c4, 0x408c6fc0},
      {0x4041a37e, 0xc0c8d2ea, 0x408ad980},
      {0x403b3858, 0xc0c45c75, 0x40894c55},
      {0x40351506, 0xc0c00baa, 0x4087c7f1},
      {0x402f35a5, 0xc0bbdee5, 0x40864c0b},
      {0x40299694, 0xc0b7d49b, 0x4084d85b},
      {0x4024346b, 0xc0b3eb52, 0x40836c9e},
      {0x401f0bfb, 0xc0b021a8, 0x40820895},
      {0x401a1a45, 0xc0ac764c, 0x4080ac00},
      {0x40155c7c, 0xc0a8e7fc, 0x407ead4a},
      {0x4010cffc, 0xc0a5758b, 0x407c1095},
      {0x400c724c, 0xc0a21dd7, 0x40798174},
      {0x40084117, 0xc09edfcf, 0x4076ff7e},
      {0x40043a2d, 0xc09bba6e, 0x40748a4f},
      {0x40005b7d, 0xc098acbd, 0x40722185},
      {0x3ff9462d, 0xc095b5d1, 0x406fc4c5},
      {0x3ff21e47, 0xc092d4c9, 0x406d73b5},
      {0x3feb3bd5, 0xc09008d0, 0x406b2dfe},
      {0x3fe49b94, 0xc08d511a, 0x4068f34f},
      {0x3fde3a70, 0xc08aace6, 0x4066c357},
      {0x3fd81580, 0xc0881b7a, 0x40649dca},
      {0x3fd22a05, 0xc0859c27, 0x4062825f},
      {0x3fcc7565, 0xc0832e43, 0x406070ce},
      {0x3fc6f52b, 0xc080d12d, 0x405e68d2},
      {0x3fc1a702, 0xc07d089a, 0x405c6a2b},
      {0x3fbc88b5, 0xc0788e1f, 0x405a7498},
      {0x3fb7982e, 0xc07431ce, 0x405887db},
      {0x3fb2d372, 0xc06ff29d, 0x4056a3ba},
      {0x3fae389f, 0xc06bcf8a, 0x4054c7fa},
      {0x3fa9c5ed, 0xc067c79f, 0x4052f466},
      {0x3fa579aa, 0xc063d9f0, 0x405128c7},
      {0x3fa1523c, 0xc060059b, 0x404f64eb},
      {0x3f9d4e19, 0xc05c49c8, 0x404da89e},
      {0x3f996bcf, 0xc058a5a8, 0x404bf3b1},
      {0x3f95a9fd, 0xc0551874, 0x404a45f5},
      {0x3f920751, 0xc051a16d, 0x40489f3e},
      {0x3f8e828b, 0xc04e3fdb, 0x4046ff5f},
      {0x3f8b1a7b, 0xc04af310, 0x4045662e},
      {0x3f87cdfe, 0xc047ba61, 0x4043d383},
      {0x3f849bff, 0xc044952e, 0x40424736},
      {0x3f818376, 0xc04182da, 0x4040c120},
  };
  if (xx == 0.0f)  // 输入为0，则输出nan
  {
    return std::numeric_limits<float>::quiet_NaN();
  }
  int sign = 1;
  float x = xx;
  if (xx < 0) {  // 取绝对值
    sign = -1;
    x = -xx;
  }
  // 1) 缩小范围
  // x = sign*2^e*m => 1/x = sign*(2^-e)*(1/m)
  int e;
  x = frexpf(x, &e);  // 将单精度浮点数分解为归一化尾数和指数。x = sign*2^e*m，其中，m是归一化尾数，范围在[0.5,1)；e是指数
  // 多项式拟合（泰勒级数展开）近似计算log(m)
  constexpr int m = 6;
  int x1 = (((*(uint32_t *)(&x)) & 0x7FFFFF) >> (23 - m));  // 取23bit尾数的高m bit
  float C2 = *(float *)(&rcp_table[x1][0]);
  float C1 = *(float *)(&rcp_table[x1][1]);
  float C0 = *(float *)(&rcp_table[x1][2]);
  float y = C2 * x * x + C1 * x + C0;
  // 3）恢复范围
  y = ldexpf(y, -e);
  if (sign < 0) {
    y = -y;
  }
  return y;
}