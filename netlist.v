// Benchmark "../b05_comb" written by ABC on Fri Oct 25 15:55:42 2024

module b05_comb  ( 
    \EN_DISP_reg/NET0131 , \FLAG_reg/NET0131 , \MAR_reg[0]/NET0131 ,
    \MAR_reg[1]/NET0131 , \MAR_reg[2]/NET0131 , \MAR_reg[3]/NET0131 ,
    \MAR_reg[4]/NET0131 , \MAX_reg[0]/NET0131 , \MAX_reg[1]/NET0131 ,
    \MAX_reg[2]/NET0131 , \MAX_reg[3]/NET0131 , \MAX_reg[4]/NET0131 ,
    \MAX_reg[5]/NET0131 , \MAX_reg[6]/NET0131 , \MAX_reg[7]/NET0131 ,
    \MAX_reg[8]/NET0131 , \NUM_reg[0]/NET0131 , \NUM_reg[1]/NET0131 ,
    \NUM_reg[2]/NET0131 , \NUM_reg[3]/NET0131 , \NUM_reg[4]/NET0131 ,
    \RES_DISP_reg/NET0131 , START_pad, \STATO_reg[0]/NET0131 ,
    \STATO_reg[1]/NET0131 , \STATO_reg[2]/NET0131 , \TEMP_reg[0]/NET0131 ,
    \TEMP_reg[1]/NET0131 , \TEMP_reg[2]/NET0131 , \TEMP_reg[3]/NET0131 ,
    \TEMP_reg[4]/NET0131 , \TEMP_reg[5]/NET0131 , \TEMP_reg[6]/NET0131 ,
    \TEMP_reg[7]/NET0131 , \TEMP_reg[8]/NET0131 ,
    \DISPMAX2[0]_pad , \DISPMAX2[1]_pad , \DISPMAX2[2]_pad ,
    \DISPMAX2[4]_pad , \DISPMAX2[5]_pad , \DISPMAX2[6]_pad ,
    \DISPMAX3[0]_pad , \DISPMAX3[1]_pad , \DISPMAX3[2]_pad ,
    \DISPMAX3[3]_pad , \DISPMAX3[4]_pad , \DISPMAX3[5]_pad ,
    \DISPMAX3[6]_pad , \DISPNUM1[0]_pad , \DISPNUM1[6]_pad ,
    \DISPNUM2[0]_pad , \DISPNUM2[1]_pad , \DISPNUM2[2]_pad ,
    \DISPNUM2[3]_pad , \DISPNUM2[4]_pad , \DISPNUM2[5]_pad ,
    \DISPNUM2[6]_pad , SIGN_pad, _al_n0, _al_n1, \g2955/_2_ , \g2956/_2_ ,
    \g2957/_2_ , \g2958/_2_ , \g2959/_2_ , \g2960/_2_ , \g2961/_2_ ,
    \g2962/_2_ , \g2963/_2_ , \g2985/_0_ , \g3032/_0_ , \g3033/_0_ ,
    \g3034/_0_ , \g3038/_0_ , \g3247/_0_ , \g3279/_0_ , \g3309/_0_ ,
    \g3335/_0_ , \g3336/_0_ , \g3337/_0_ , \g3338/_0_ , \g3339/_0_ ,
    \g3340/_0_ , \g3341/_0_ , \g3360/_0_ , \g3361/_0_ , \g3369/_0_ ,
    \g3373/_0_ , \g3382/_0_ , \g3451/_0_ , \g3475/_0_ , \g3490/_0_ ,
    \g3514/_0_ , \g3774/_1_ , \g4511/_0_   );
  input  \EN_DISP_reg/NET0131 , \FLAG_reg/NET0131 , \MAR_reg[0]/NET0131 ,
    \MAR_reg[1]/NET0131 , \MAR_reg[2]/NET0131 , \MAR_reg[3]/NET0131 ,
    \MAR_reg[4]/NET0131 , \MAX_reg[0]/NET0131 , \MAX_reg[1]/NET0131 ,
    \MAX_reg[2]/NET0131 , \MAX_reg[3]/NET0131 , \MAX_reg[4]/NET0131 ,
    \MAX_reg[5]/NET0131 , \MAX_reg[6]/NET0131 , \MAX_reg[7]/NET0131 ,
    \MAX_reg[8]/NET0131 , \NUM_reg[0]/NET0131 , \NUM_reg[1]/NET0131 ,
    \NUM_reg[2]/NET0131 , \NUM_reg[3]/NET0131 , \NUM_reg[4]/NET0131 ,
    \RES_DISP_reg/NET0131 , START_pad, \STATO_reg[0]/NET0131 ,
    \STATO_reg[1]/NET0131 , \STATO_reg[2]/NET0131 , \TEMP_reg[0]/NET0131 ,
    \TEMP_reg[1]/NET0131 , \TEMP_reg[2]/NET0131 , \TEMP_reg[3]/NET0131 ,
    \TEMP_reg[4]/NET0131 , \TEMP_reg[5]/NET0131 , \TEMP_reg[6]/NET0131 ,
    \TEMP_reg[7]/NET0131 , \TEMP_reg[8]/NET0131 ;
  output \DISPMAX2[0]_pad , \DISPMAX2[1]_pad , \DISPMAX2[2]_pad ,
    \DISPMAX2[4]_pad , \DISPMAX2[5]_pad , \DISPMAX2[6]_pad ,
    \DISPMAX3[0]_pad , \DISPMAX3[1]_pad , \DISPMAX3[2]_pad ,
    \DISPMAX3[3]_pad , \DISPMAX3[4]_pad , \DISPMAX3[5]_pad ,
    \DISPMAX3[6]_pad , \DISPNUM1[0]_pad , \DISPNUM1[6]_pad ,
    \DISPNUM2[0]_pad , \DISPNUM2[1]_pad , \DISPNUM2[2]_pad ,
    \DISPNUM2[3]_pad , \DISPNUM2[4]_pad , \DISPNUM2[5]_pad ,
    \DISPNUM2[6]_pad , SIGN_pad, _al_n0, _al_n1, \g2955/_2_ , \g2956/_2_ ,
    \g2957/_2_ , \g2958/_2_ , \g2959/_2_ , \g2960/_2_ , \g2961/_2_ ,
    \g2962/_2_ , \g2963/_2_ , \g2985/_0_ , \g3032/_0_ , \g3033/_0_ ,
    \g3034/_0_ , \g3038/_0_ , \g3247/_0_ , \g3279/_0_ , \g3309/_0_ ,
    \g3335/_0_ , \g3336/_0_ , \g3337/_0_ , \g3338/_0_ , \g3339/_0_ ,
    \g3340/_0_ , \g3341/_0_ , \g3360/_0_ , \g3361/_0_ , \g3369/_0_ ,
    \g3373/_0_ , \g3382/_0_ , \g3451/_0_ , \g3475/_0_ , \g3490/_0_ ,
    \g3514/_0_ , \g3774/_1_ , \g4511/_0_ ;
  wire new_n96, new_n97, new_n98, new_n99, new_n100, new_n101, new_n102,
    new_n103, new_n104, new_n105, new_n106, new_n107, new_n108, new_n109,
    new_n110, new_n111, new_n112, new_n113, new_n114, new_n115, new_n116,
    new_n117, new_n118, new_n119, new_n120, new_n121, new_n122, new_n123,
    new_n124, new_n125, new_n126, new_n127, new_n128, new_n129, new_n130,
    new_n131, new_n133, new_n135, new_n136, new_n137, new_n138, new_n139,
    new_n141, new_n142, new_n143, new_n144, new_n145, new_n146, new_n148,
    new_n150, new_n151, new_n153, new_n154, new_n156, new_n157, new_n158,
    new_n159, new_n160, new_n161, new_n162, new_n163, new_n164, new_n165,
    new_n166, new_n167, new_n168, new_n169, new_n170, new_n171, new_n172,
    new_n173, new_n174, new_n175, new_n176, new_n177, new_n178, new_n179,
    new_n180, new_n181, new_n182, new_n183, new_n184, new_n185, new_n186,
    new_n187, new_n188, new_n189, new_n190, new_n191, new_n192, new_n193,
    new_n194, new_n195, new_n196, new_n197, new_n198, new_n199, new_n200,
    new_n201, new_n202, new_n203, new_n204, new_n205, new_n206, new_n207,
    new_n208, new_n209, new_n210, new_n211, new_n212, new_n213, new_n214,
    new_n215, new_n216, new_n217, new_n218, new_n219, new_n220, new_n221,
    new_n222, new_n223, new_n224, new_n225, new_n227, new_n228, new_n229,
    new_n231, new_n232, new_n233, new_n234, new_n236, new_n237, new_n238,
    new_n240, new_n241, new_n243, new_n245, new_n246, new_n247, new_n249,
    new_n250, new_n253, new_n254, new_n255, new_n256, new_n257, new_n258,
    new_n259, new_n260, new_n261, new_n262, new_n263, new_n264, new_n265,
    new_n266, new_n267, new_n268, new_n269, new_n270, new_n271, new_n272,
    new_n273, new_n274, new_n275, new_n276, new_n277, new_n278, new_n279,
    new_n280, new_n281, new_n282, new_n283, new_n285, new_n287, new_n288,
    new_n290, new_n292, new_n294, new_n295, new_n296, new_n298, new_n299,
    new_n300, new_n301, new_n306, new_n307, new_n308, new_n309, new_n310,
    new_n311, new_n312, new_n313, new_n314, new_n315, new_n316, new_n317,
    new_n318, new_n319, new_n320, new_n321, new_n322, new_n323, new_n324,
    new_n325, new_n326, new_n327, new_n328, new_n329, new_n330, new_n331,
    new_n332, new_n333, new_n334, new_n335, new_n336, new_n337, new_n338,
    new_n339, new_n340, new_n341, new_n342, new_n343, new_n344, new_n345,
    new_n346, new_n347, new_n348, new_n349, new_n350, new_n351, new_n352,
    new_n353, new_n354, new_n355, new_n356, new_n357, new_n358, new_n359,
    new_n360, new_n361, new_n362, new_n363, new_n364, new_n365, new_n366,
    new_n367, new_n368, new_n369, new_n370, new_n371, new_n372, new_n373,
    new_n374, new_n375, new_n376, new_n377, new_n378, new_n379, new_n380,
    new_n381, new_n382, new_n383, new_n384, new_n385, new_n386, new_n387,
    new_n388, new_n389, new_n390, new_n391, new_n392, new_n393, new_n394,
    new_n395, new_n396, new_n397, new_n398, new_n399, new_n400, new_n401,
    new_n402, new_n403, new_n404, new_n405, new_n406, new_n407, new_n408,
    new_n409, new_n410, new_n411, new_n412, new_n413, new_n414, new_n415,
    new_n416, new_n417, new_n418, new_n419, new_n420, new_n421, new_n422,
    new_n423, new_n424, new_n425, new_n426, new_n427, new_n428, new_n429,
    new_n430, new_n431, new_n432, new_n433, new_n434, new_n435, new_n436,
    new_n437, new_n438, new_n439, new_n440, new_n441, new_n442, new_n443,
    new_n444, new_n445, new_n446, new_n447, new_n448, new_n449, new_n450,
    new_n451, new_n452, new_n453, new_n454, new_n455, new_n456, new_n457,
    new_n458, new_n459, new_n460, new_n461, new_n462, new_n463, new_n464,
    new_n465, new_n466, new_n467, new_n468, new_n469, new_n470, new_n471,
    new_n472, new_n473, new_n474, new_n475, new_n476, new_n477, new_n478,
    new_n479, new_n480, new_n481, new_n482, new_n483, new_n484, new_n485,
    new_n486, new_n487, new_n488, new_n489, new_n490, new_n491, new_n492,
    new_n493, new_n494, new_n495, new_n496, new_n497, new_n498, new_n499,
    new_n500, new_n501, new_n502, new_n503, new_n504, new_n505, new_n506,
    new_n507, new_n508, new_n509, new_n510, new_n511, new_n512, new_n513,
    new_n514, new_n515, new_n516, new_n517, new_n518, new_n519, new_n520,
    new_n521, new_n522, new_n523, new_n524, new_n525, new_n526, new_n527,
    new_n528, new_n529, new_n530, new_n531, new_n532, new_n533, new_n534,
    new_n535, new_n536, new_n537, new_n538, new_n539, new_n540, new_n541,
    new_n542, new_n543, new_n544, new_n545, new_n546, new_n547, new_n548,
    new_n549, new_n550, new_n551, new_n552, new_n553, new_n554, new_n555,
    new_n556, new_n557, new_n558, new_n559, new_n560, new_n561, new_n562,
    new_n563, new_n564, new_n565, new_n566, new_n567, new_n568, new_n569,
    new_n570, new_n571, new_n572, new_n573, new_n574, new_n575, new_n576,
    new_n577, new_n578, new_n579, new_n580, new_n581, new_n582, new_n583,
    new_n584, new_n585, new_n586, new_n587, new_n588, new_n589, new_n590,
    new_n591, new_n592, new_n593, new_n594, new_n595, new_n596, new_n597,
    new_n598, new_n599, new_n600, new_n601, new_n602, new_n603, new_n604,
    new_n605, new_n606, new_n607, new_n608, new_n609, new_n610, new_n611,
    new_n612, new_n613, new_n614, new_n615, new_n616, new_n617, new_n618,
    new_n619, new_n620, new_n621, new_n622, new_n623, new_n624, new_n625,
    new_n626, new_n627, new_n628, new_n629, new_n630, new_n631, new_n632,
    new_n633, new_n634, new_n635, new_n636, new_n637, new_n638, new_n639,
    new_n640, new_n641, new_n642, new_n643, new_n644, new_n645, new_n646,
    new_n647, new_n649, new_n650, new_n651, new_n652, new_n653, new_n654,
    new_n655, new_n656, new_n657, new_n659, new_n660, new_n661, new_n662,
    new_n663, new_n664, new_n665, new_n666, new_n667, new_n668, new_n669,
    new_n670, new_n672, new_n673, new_n674, new_n675, new_n676, new_n677,
    new_n678, new_n679, new_n680, new_n681, new_n683, new_n684, new_n685,
    new_n686, new_n687, new_n688, new_n689, new_n690, new_n691, new_n693,
    new_n694, new_n695, new_n696, new_n697, new_n698, new_n699, new_n700,
    new_n701, new_n702, new_n703, new_n705, new_n706, new_n707, new_n708,
    new_n709, new_n710, new_n711, new_n712, new_n713, new_n715, new_n716,
    new_n717, new_n718, new_n719, new_n720, new_n721, new_n722, new_n723,
    new_n725, new_n726, new_n727, new_n728, new_n729, new_n730, new_n731,
    new_n732, new_n733, new_n735, new_n736, new_n737, new_n738, new_n739,
    new_n740, new_n741, new_n743, new_n744, new_n745, new_n746, new_n747,
    new_n748, new_n749, new_n750, new_n751, new_n752, new_n753, new_n754,
    new_n756, new_n757, new_n758, new_n759, new_n760, new_n761, new_n763,
    new_n764, new_n765, new_n766, new_n767, new_n768, new_n770, new_n771,
    new_n772, new_n773, new_n774, new_n775, new_n776, new_n777, new_n778,
    new_n780, new_n781, new_n782, new_n783, new_n784, new_n785, new_n786,
    new_n788, new_n789, new_n790, new_n792, new_n793, new_n794, new_n795,
    new_n796, new_n797, new_n799, new_n800, new_n802, new_n803, new_n805,
    new_n806, new_n807, new_n809, new_n810, new_n812, new_n813, new_n814,
    new_n816, new_n817, new_n819, new_n820, new_n821, new_n823, new_n824,
    new_n825, new_n826, new_n828, new_n829, new_n830, new_n832, new_n833,
    new_n834, new_n835, new_n836, new_n838, new_n839, new_n841, new_n842,
    new_n845, new_n847, new_n850, new_n851, new_n852, new_n853, new_n854;
  INVx1_ASAP7_75t_L         g000(.A(\MAX_reg[4]/NET0131 ), .Y(new_n96));
  INVx1_ASAP7_75t_L         g001(.A(\MAX_reg[8]/NET0131 ), .Y(new_n97));
  NOR4xp25_ASAP7_75t_L      g002(.A(\MAX_reg[0]/NET0131 ), .B(\MAX_reg[1]/NET0131 ), .C(\MAX_reg[2]/NET0131 ), .D(\MAX_reg[3]/NET0131 ), .Y(new_n98));
  INVx1_ASAP7_75t_L         g003(.A(\MAX_reg[2]/NET0131 ), .Y(new_n99));
  O2A1O1Ixp33_ASAP7_75t_L   g004(.A1(\MAX_reg[0]/NET0131 ), .A2(\MAX_reg[1]/NET0131 ), .B(\MAX_reg[8]/NET0131 ), .C(new_n99), .Y(new_n100));
  INVx1_ASAP7_75t_L         g005(.A(\MAX_reg[0]/NET0131 ), .Y(new_n101));
  INVx1_ASAP7_75t_L         g006(.A(\MAX_reg[1]/NET0131 ), .Y(new_n102));
  AOI211xp5_ASAP7_75t_L     g007(.A1(new_n101), .A2(new_n102), .B(\MAX_reg[2]/NET0131 ), .C(new_n97), .Y(new_n103));
  NOR2xp33_ASAP7_75t_L      g008(.A(new_n100), .B(new_n103), .Y(new_n104));
  OAI21xp33_ASAP7_75t_L     g009(.A1(\MAX_reg[0]/NET0131 ), .A2(new_n102), .B(\MAX_reg[8]/NET0131 ), .Y(new_n105));
  NOR2xp33_ASAP7_75t_L      g010(.A(\MAX_reg[8]/NET0131 ), .B(\MAX_reg[1]/NET0131 ), .Y(new_n106));
  INVx1_ASAP7_75t_L         g011(.A(new_n106), .Y(new_n107));
  A2O1A1Ixp33_ASAP7_75t_L   g012(.A1(\MAX_reg[0]/NET0131 ), .A2(new_n102), .B(new_n105), .C(new_n107), .Y(new_n108));
  NOR2xp33_ASAP7_75t_L      g013(.A(new_n108), .B(new_n104), .Y(new_n109));
  A2O1A1Ixp33_ASAP7_75t_L   g014(.A1(new_n96), .A2(new_n98), .B(new_n97), .C(new_n109), .Y(new_n110));
  INVx1_ASAP7_75t_L         g015(.A(\MAX_reg[3]/NET0131 ), .Y(new_n111));
  NAND4xp25_ASAP7_75t_L     g016(.A(new_n101), .B(new_n102), .C(new_n99), .D(new_n111), .Y(new_n112));
  NOR2xp33_ASAP7_75t_L      g017(.A(new_n96), .B(new_n112), .Y(new_n113));
  NOR2xp33_ASAP7_75t_L      g018(.A(\MAX_reg[4]/NET0131 ), .B(new_n98), .Y(new_n114));
  OAI21xp33_ASAP7_75t_L     g019(.A1(new_n114), .A2(new_n113), .B(\MAX_reg[8]/NET0131 ), .Y(new_n115));
  NAND4xp25_ASAP7_75t_L     g020(.A(new_n101), .B(new_n102), .C(new_n99), .D(\MAX_reg[3]/NET0131 ), .Y(new_n116));
  OAI31xp33_ASAP7_75t_L     g021(.A1(\MAX_reg[0]/NET0131 ), .A2(\MAX_reg[1]/NET0131 ), .A3(\MAX_reg[2]/NET0131 ), .B(new_n111), .Y(new_n117));
  NAND2xp33_ASAP7_75t_L     g022(.A(\MAX_reg[3]/NET0131 ), .B(new_n97), .Y(new_n118));
  A2O1A1Ixp33_ASAP7_75t_L   g023(.A1(new_n116), .A2(new_n117), .B(new_n97), .C(new_n118), .Y(new_n119));
  INVx1_ASAP7_75t_L         g024(.A(new_n119), .Y(new_n120));
  O2A1O1Ixp33_ASAP7_75t_L   g025(.A1(new_n96), .A2(\MAX_reg[8]/NET0131 ), .B(new_n115), .C(new_n120), .Y(new_n121));
  INVx1_ASAP7_75t_L         g026(.A(new_n121), .Y(new_n122));
  OAI21xp33_ASAP7_75t_L     g027(.A1(\MAX_reg[4]/NET0131 ), .A2(new_n112), .B(\MAX_reg[8]/NET0131 ), .Y(new_n123));
  INVx1_ASAP7_75t_L         g028(.A(new_n123), .Y(new_n124));
  OAI21xp33_ASAP7_75t_L     g029(.A1(new_n126), .A2(new_n98), .B(new_n128), .Y(new_n125));
  OAI21xp33_ASAP7_75t_L     g030(.A1(\MAX_reg[8]/NET0131 ), .A2(\MAX_reg[4]/NET0131 ), .B(new_n127), .Y(new_n126));
  NAND2xp33_ASAP7_75t_L     g031(.A(\MAX_reg[4]/NET0131 ), .B(\MAX_reg[8]/NET0131 ), .Y(new_n127));
  NAND2xp33_ASAP7_75t_L     g032(.A(\MAX_reg[4]/NET0131 ), .B(new_n98), .Y(new_n128));
  INVx1_ASAP7_75t_L         g033(.A(new_n125), .Y(new_n129));
  A2O1A1O1Ixp25_ASAP7_75t_L g034(.A1(new_n104), .A2(new_n108), .B(new_n120), .C(new_n129), .D(new_n124), .Y(new_n130));
  INVx1_ASAP7_75t_L         g035(.A(\RES_DISP_reg/NET0131 ), .Y(new_n131));
  NOR2xp33_ASAP7_75t_L      g036(.A(\EN_DISP_reg/NET0131 ), .B(new_n131), .Y(\g3774/_1_ ));
  INVx1_ASAP7_75t_L         g037(.A(\g3774/_1_ ), .Y(new_n133));
  O2A1O1Ixp33_ASAP7_75t_L   g038(.A1(new_n110), .A2(new_n122), .B(new_n130), .C(new_n133), .Y(\DISPMAX2[0]_pad ));
  AOI21xp33_ASAP7_75t_L     g039(.A1(new_n101), .A2(\MAX_reg[1]/NET0131 ), .B(new_n97), .Y(new_n135));
  NAND2xp33_ASAP7_75t_L     g040(.A(\MAX_reg[0]/NET0131 ), .B(new_n102), .Y(new_n136));
  A2O1A1O1Ixp25_ASAP7_75t_L g041(.A1(new_n135), .A2(new_n136), .B(new_n106), .C(new_n104), .D(new_n120), .Y(new_n137));
  NAND3xp33_ASAP7_75t_L     g042(.A(new_n121), .B(new_n123), .C(new_n109), .Y(new_n138));
  NAND2xp33_ASAP7_75t_L     g043(.A(\g3774/_1_ ), .B(new_n138), .Y(new_n139));
  O2A1O1Ixp33_ASAP7_75t_L   g044(.A1(new_n125), .A2(new_n137), .B(new_n123), .C(new_n139), .Y(\DISPMAX2[1]_pad ));
  A2O1A1Ixp33_ASAP7_75t_L   g045(.A1(new_n101), .A2(new_n102), .B(new_n97), .C(\MAX_reg[2]/NET0131 ), .Y(new_n141));
  OAI211xp5_ASAP7_75t_L     g046(.A1(\MAX_reg[0]/NET0131 ), .A2(\MAX_reg[1]/NET0131 ), .B(new_n99), .C(\MAX_reg[8]/NET0131 ), .Y(new_n142));
  NAND2xp33_ASAP7_75t_L     g047(.A(new_n142), .B(new_n141), .Y(new_n143));
  OAI21xp33_ASAP7_75t_L     g048(.A1(new_n143), .A2(new_n119), .B(new_n125), .Y(new_n144));
  NAND2xp33_ASAP7_75t_L     g049(.A(new_n108), .B(new_n104), .Y(new_n145));
  OAI21xp33_ASAP7_75t_L     g050(.A1(new_n119), .A2(new_n145), .B(new_n123), .Y(new_n146));
  O2A1O1Ixp33_ASAP7_75t_L   g051(.A1(new_n144), .A2(new_n146), .B(new_n130), .C(new_n139), .Y(\DISPMAX2[2]_pad ));
  NOR2xp33_ASAP7_75t_L      g052(.A(new_n144), .B(new_n146), .Y(new_n148));
  O2A1O1Ixp33_ASAP7_75t_L   g053(.A1(new_n110), .A2(new_n122), .B(new_n148), .C(new_n133), .Y(\DISPMAX2[4]_pad ));
  A2O1A1Ixp33_ASAP7_75t_L   g054(.A1(new_n145), .A2(new_n119), .B(new_n125), .C(new_n123), .Y(new_n150));
  NOR2xp33_ASAP7_75t_L      g055(.A(new_n150), .B(new_n148), .Y(new_n151));
  O2A1O1Ixp33_ASAP7_75t_L   g056(.A1(new_n110), .A2(new_n122), .B(new_n151), .C(new_n133), .Y(\DISPMAX2[5]_pad ));
  INVx1_ASAP7_75t_L         g057(.A(new_n138), .Y(new_n153));
  NOR2xp33_ASAP7_75t_L      g058(.A(new_n131), .B(new_n153), .Y(new_n154));
  O2A1O1Ixp33_ASAP7_75t_L   g059(.A1(new_n144), .A2(new_n146), .B(new_n154), .C(\EN_DISP_reg/NET0131 ), .Y(\DISPMAX2[6]_pad ));
  INVx1_ASAP7_75t_L         g060(.A(new_n115), .Y(new_n156));
  INVx1_ASAP7_75t_L         g061(.A(new_n144), .Y(new_n157));
  O2A1O1Ixp33_ASAP7_75t_L   g062(.A1(new_n101), .A2(\MAX_reg[1]/NET0131 ), .B(new_n135), .C(new_n106), .Y(new_n158));
  NOR2xp33_ASAP7_75t_L      g063(.A(new_n143), .B(new_n158), .Y(new_n159));
  NAND2xp33_ASAP7_75t_L     g064(.A(new_n120), .B(new_n159), .Y(new_n160));
  NAND3xp33_ASAP7_75t_L     g065(.A(new_n157), .B(new_n160), .C(new_n123), .Y(new_n161));
  OAI21xp33_ASAP7_75t_L     g066(.A1(new_n159), .A2(new_n120), .B(new_n163), .Y(new_n162));
  NAND2xp33_ASAP7_75t_L     g067(.A(new_n120), .B(new_n159), .Y(new_n163));
  A2O1A1Ixp33_ASAP7_75t_L   g068(.A1(new_n129), .A2(new_n159), .B(new_n124), .C(new_n119), .Y(new_n164));
  OAI211xp5_ASAP7_75t_L     g069(.A1(new_n150), .A2(new_n162), .B(new_n161), .C(new_n164), .Y(new_n165));
  OAI21xp33_ASAP7_75t_L     g070(.A1(new_n167), .A2(new_n168), .B(new_n170), .Y(new_n166));
  NOR2xp33_ASAP7_75t_L      g071(.A(new_n104), .B(new_n108), .Y(new_n167));
  OAI21xp33_ASAP7_75t_L     g072(.A1(new_n119), .A2(new_n104), .B(new_n169), .Y(new_n168));
  NAND2xp33_ASAP7_75t_L     g073(.A(new_n104), .B(new_n119), .Y(new_n169));
  NOR2xp33_ASAP7_75t_L      g074(.A(new_n124), .B(new_n129), .Y(new_n170));
  O2A1O1Ixp33_ASAP7_75t_L   g075(.A1(new_n143), .A2(new_n158), .B(new_n119), .C(new_n125), .Y(new_n171));
  A2O1A1Ixp33_ASAP7_75t_L   g076(.A1(new_n135), .A2(new_n136), .B(new_n106), .C(new_n143), .Y(new_n172));
  NAND2xp33_ASAP7_75t_L     g077(.A(new_n158), .B(new_n104), .Y(new_n173));
  AOI211xp5_ASAP7_75t_L     g078(.A1(new_n172), .A2(new_n173), .B(new_n124), .C(new_n171), .Y(new_n174));
  A2O1A1O1Ixp25_ASAP7_75t_L g079(.A1(new_n158), .A2(new_n119), .B(new_n125), .C(new_n123), .D(new_n143), .Y(new_n175));
  NAND4xp25_ASAP7_75t_L     g080(.A(new_n125), .B(new_n119), .C(new_n123), .D(new_n104), .Y(new_n176));
  OAI31xp33_ASAP7_75t_L     g081(.A1(new_n148), .A2(new_n174), .A3(new_n175), .B(new_n176), .Y(new_n177));
  AOI211xp5_ASAP7_75t_L     g082(.A1(new_n165), .A2(new_n166), .B(new_n156), .C(new_n177), .Y(new_n178));
  NAND2xp33_ASAP7_75t_L     g083(.A(new_n136), .B(new_n135), .Y(new_n179));
  A2O1A1Ixp33_ASAP7_75t_L   g084(.A1(new_n179), .A2(new_n107), .B(new_n143), .C(new_n119), .Y(new_n180));
  A2O1A1Ixp33_ASAP7_75t_L   g085(.A1(new_n180), .A2(new_n129), .B(new_n124), .C(new_n158), .Y(new_n181));
  OR3x1_ASAP7_75t_L         g086(.A(new_n171), .B(new_n124), .C(new_n158), .Y(new_n182));
  NAND4xp25_ASAP7_75t_L     g087(.A(new_n157), .B(new_n160), .C(new_n123), .D(new_n158), .Y(new_n183));
  A2O1A1Ixp33_ASAP7_75t_L   g088(.A1(new_n182), .A2(new_n181), .B(new_n148), .C(new_n183), .Y(new_n184));
  O2A1O1Ixp33_ASAP7_75t_L   g089(.A1(new_n110), .A2(new_n122), .B(new_n184), .C(new_n124), .Y(new_n185));
  O2A1O1Ixp33_ASAP7_75t_L   g090(.A1(new_n157), .A2(new_n161), .B(new_n123), .C(new_n153), .Y(new_n186));
  A2O1A1O1Ixp25_ASAP7_75t_L g091(.A1(new_n101), .A2(new_n123), .B(new_n185), .C(new_n178), .D(new_n186), .Y(new_n187));
  NAND2xp33_ASAP7_75t_L     g092(.A(new_n120), .B(new_n145), .Y(new_n188));
  NAND2xp33_ASAP7_75t_L     g093(.A(new_n119), .B(new_n159), .Y(new_n189));
  NAND2xp33_ASAP7_75t_L     g094(.A(new_n189), .B(new_n188), .Y(new_n190));
  O2A1O1Ixp33_ASAP7_75t_L   g095(.A1(new_n125), .A2(new_n145), .B(new_n123), .C(new_n120), .Y(new_n191));
  AOI211xp5_ASAP7_75t_L     g096(.A1(new_n130), .A2(new_n190), .B(new_n191), .C(new_n148), .Y(new_n192));
  INVx1_ASAP7_75t_L         g097(.A(new_n166), .Y(new_n193));
  AO221x2_ASAP7_75t_L       g098(.A1(new_n172), .A2(new_n173), .B1(new_n180), .B2(new_n129), .C(new_n124), .Y(new_n194));
  INVx1_ASAP7_75t_L         g099(.A(new_n175), .Y(new_n195));
  INVx1_ASAP7_75t_L         g100(.A(new_n176), .Y(new_n196));
  AOI31xp33_ASAP7_75t_L     g101(.A1(new_n161), .A2(new_n194), .A3(new_n195), .B(new_n196), .Y(new_n197));
  OAI211xp5_ASAP7_75t_L     g102(.A1(new_n193), .A2(new_n192), .B(new_n115), .C(new_n197), .Y(new_n198));
  AO21x2_ASAP7_75t_L        g103(.A1(new_n181), .A2(new_n182), .B(new_n148), .Y(new_n199));
  NOR3xp33_ASAP7_75t_L      g104(.A(new_n146), .B(new_n108), .C(new_n144), .Y(new_n200));
  AOI21xp33_ASAP7_75t_L     g105(.A1(new_n199), .A2(new_n202), .B(new_n203), .Y(new_n201));
  NOR2xp33_ASAP7_75t_L      g106(.A(new_n200), .B(new_n124), .Y(new_n202));
  NOR2xp33_ASAP7_75t_L      g107(.A(new_n124), .B(new_n138), .Y(new_n203));
  INVx1_ASAP7_75t_L         g108(.A(new_n186), .Y(new_n204));
  OAI31xp33_ASAP7_75t_L     g109(.A1(new_n198), .A2(\MAX_reg[0]/NET0131 ), .A3(new_n201), .B(new_n204), .Y(new_n205));
  A2O1A1Ixp33_ASAP7_75t_L   g110(.A1(new_n188), .A2(new_n189), .B(new_n150), .C(new_n164), .Y(new_n206));
  O2A1O1Ixp33_ASAP7_75t_L   g111(.A1(new_n148), .A2(new_n206), .B(new_n166), .C(new_n156), .Y(new_n207));
  NAND3xp33_ASAP7_75t_L     g112(.A(new_n207), .B(new_n123), .C(new_n197), .Y(new_n208));
  A2O1A1O1Ixp25_ASAP7_75t_L g113(.A1(new_n181), .A2(new_n182), .B(new_n148), .C(new_n183), .D(new_n153), .Y(new_n209));
  OAI211xp5_ASAP7_75t_L     g114(.A1(new_n193), .A2(new_n192), .B(new_n123), .C(new_n115), .Y(new_n210));
  OAI311xp33_ASAP7_75t_L    g115(.A1(new_n210), .A2(\MAX_reg[0]/NET0131 ), .A3(new_n209), .B1(new_n208), .C1(new_n204), .Y(new_n211));
  A2O1A1Ixp33_ASAP7_75t_L   g116(.A1(new_n201), .A2(new_n124), .B(new_n198), .C(new_n204), .Y(new_n212));
  OAI211xp5_ASAP7_75t_L     g117(.A1(new_n187), .A2(new_n205), .B(new_n211), .C(new_n212), .Y(new_n213));
  NAND2xp33_ASAP7_75t_L     g118(.A(new_n138), .B(new_n184), .Y(new_n214));
  A2O1A1O1Ixp25_ASAP7_75t_L g119(.A1(new_n207), .A2(new_n214), .B(new_n178), .C(new_n123), .D(new_n186), .Y(new_n215));
  INVx1_ASAP7_75t_L         g120(.A(new_n215), .Y(new_n216));
  INVx1_ASAP7_75t_L         g121(.A(new_n207), .Y(new_n217));
  O2A1O1Ixp33_ASAP7_75t_L   g122(.A1(new_n209), .A2(new_n217), .B(new_n198), .C(new_n124), .Y(new_n218));
  NOR2xp33_ASAP7_75t_L      g123(.A(\MAX_reg[0]/NET0131 ), .B(new_n210), .Y(new_n219));
  NOR3xp33_ASAP7_75t_L      g124(.A(new_n218), .B(new_n186), .C(new_n219), .Y(new_n220));
  O2A1O1Ixp33_ASAP7_75t_L   g125(.A1(new_n193), .A2(new_n192), .B(new_n115), .C(new_n124), .Y(new_n221));
  INVx1_ASAP7_75t_L         g126(.A(new_n221), .Y(new_n222));
  NOR2xp33_ASAP7_75t_L      g127(.A(new_n177), .B(new_n201), .Y(new_n223));
  O2A1O1Ixp33_ASAP7_75t_L   g128(.A1(new_n114), .A2(new_n113), .B(\MAX_reg[8]/NET0131 ), .C(\MAX_reg[0]/NET0131 ), .Y(new_n224));
  A2O1A1Ixp33_ASAP7_75t_L   g129(.A1(new_n223), .A2(new_n224), .B(new_n222), .C(\g3774/_1_ ), .Y(new_n225));
  A2O1A1O1Ixp25_ASAP7_75t_L g130(.A1(new_n216), .A2(new_n213), .B(new_n220), .C(new_n222), .D(new_n225), .Y(\DISPMAX3[0]_pad ));
  AOI21xp33_ASAP7_75t_L     g131(.A1(new_n178), .A2(new_n185), .B(new_n186), .Y(new_n227));
  NAND4xp25_ASAP7_75t_L     g132(.A(new_n214), .B(new_n207), .C(new_n101), .D(new_n123), .Y(new_n228));
  A2O1A1O1Ixp25_ASAP7_75t_L g133(.A1(new_n204), .A2(new_n228), .B(new_n227), .C(new_n208), .D(new_n205), .Y(new_n229));
  O2A1O1Ixp33_ASAP7_75t_L   g134(.A1(new_n220), .A2(new_n229), .B(new_n222), .C(new_n133), .Y(\DISPMAX3[1]_pad ));
  NOR2xp33_ASAP7_75t_L      g135(.A(\MAX_reg[4]/NET0131 ), .B(new_n112), .Y(new_n231));
  O2A1O1Ixp33_ASAP7_75t_L   g136(.A1(new_n97), .A2(new_n231), .B(new_n217), .C(new_n215), .Y(new_n232));
  INVx1_ASAP7_75t_L         g137(.A(new_n208), .Y(new_n233));
  A2O1A1O1Ixp25_ASAP7_75t_L g138(.A1(new_n178), .A2(new_n185), .B(new_n233), .C(new_n101), .D(new_n186), .Y(new_n234));
  AOI31xp33_ASAP7_75t_L     g139(.A1(new_n232), .A2(new_n234), .A3(new_n211), .B(new_n133), .Y(\DISPMAX3[2]_pad ));
  OAI21xp33_ASAP7_75t_L     g140(.A1(\MAX_reg[0]/NET0131 ), .A2(new_n210), .B(new_n215), .Y(new_n236));
  NOR2xp33_ASAP7_75t_L      g141(.A(new_n97), .B(new_n231), .Y(new_n237));
  A2O1A1Ixp33_ASAP7_75t_L   g142(.A1(new_n138), .A2(new_n237), .B(new_n218), .C(new_n211), .Y(new_n238));
  AOI31xp33_ASAP7_75t_L     g143(.A1(new_n238), .A2(new_n236), .A3(new_n222), .B(new_n133), .Y(\DISPMAX3[3]_pad ));
  AOI31xp33_ASAP7_75t_L     g144(.A1(new_n208), .A2(new_n204), .A3(new_n228), .B(new_n187), .Y(new_n240));
  NOR4xp25_ASAP7_75t_L      g145(.A(new_n198), .B(new_n124), .C(new_n214), .D(new_n186), .Y(new_n241));
  AOI31xp33_ASAP7_75t_L     g146(.A1(new_n240), .A2(new_n236), .A3(new_n241), .B(new_n133), .Y(\DISPMAX3[4]_pad ));
  O2A1O1Ixp33_ASAP7_75t_L   g147(.A1(new_n227), .A2(new_n205), .B(new_n212), .C(new_n215), .Y(new_n243));
  A2O1A1O1Ixp25_ASAP7_75t_L g148(.A1(new_n211), .A2(new_n243), .B(new_n220), .C(new_n222), .D(new_n133), .Y(\DISPMAX3[5]_pad ));
  AO21x2_ASAP7_75t_L        g149(.A1(new_n224), .A2(new_n223), .B(new_n222), .Y(new_n245));
  OAI31xp33_ASAP7_75t_L     g150(.A1(new_n218), .A2(new_n186), .A3(new_n219), .B(new_n227), .Y(new_n246));
  A2O1A1O1Ixp25_ASAP7_75t_L g151(.A1(new_n166), .A2(new_n165), .B(new_n156), .C(new_n123), .D(new_n131), .Y(new_n247));
  AOI31xp33_ASAP7_75t_L     g152(.A1(new_n246), .A2(new_n245), .A3(new_n247), .B(\EN_DISP_reg/NET0131 ), .Y(\DISPMAX3[6]_pad ));
  O2A1O1Ixp33_ASAP7_75t_L   g153(.A1(\NUM_reg[1]/NET0131 ), .A2(\NUM_reg[2]/NET0131 ), .B(\NUM_reg[3]/NET0131 ), .C(\NUM_reg[4]/NET0131 ), .Y(new_n249));
  INVx1_ASAP7_75t_L         g154(.A(new_n249), .Y(new_n250));
  NOR2xp33_ASAP7_75t_L      g155(.A(new_n250), .B(new_n133), .Y(\DISPNUM1[0]_pad ));
  NOR2xp33_ASAP7_75t_L      g156(.A(\RES_DISP_reg/NET0131 ), .B(\EN_DISP_reg/NET0131 ), .Y(\DISPNUM1[6]_pad ));
  INVx1_ASAP7_75t_L         g157(.A(\NUM_reg[3]/NET0131 ), .Y(new_n253));
  NOR2xp33_ASAP7_75t_L      g158(.A(\NUM_reg[2]/NET0131 ), .B(\NUM_reg[1]/NET0131 ), .Y(new_n254));
  OAI21xp33_ASAP7_75t_L     g159(.A1(new_n254), .A2(\NUM_reg[4]/NET0131 ), .B(new_n256), .Y(new_n255));
  NAND2xp33_ASAP7_75t_L     g160(.A(new_n253), .B(new_n254), .Y(new_n256));
  INVx1_ASAP7_75t_L         g161(.A(new_n255), .Y(new_n257));
  NOR2xp33_ASAP7_75t_L      g162(.A(new_n253), .B(new_n254), .Y(new_n258));
  INVx1_ASAP7_75t_L         g163(.A(\NUM_reg[1]/NET0131 ), .Y(new_n259));
  INVx1_ASAP7_75t_L         g164(.A(\NUM_reg[2]/NET0131 ), .Y(new_n260));
  O2A1O1Ixp33_ASAP7_75t_L   g165(.A1(\NUM_reg[3]/NET0131 ), .A2(\NUM_reg[4]/NET0131 ), .B(new_n259), .C(new_n260), .Y(new_n261));
  O2A1O1Ixp33_ASAP7_75t_L   g166(.A1(\NUM_reg[4]/NET0131 ), .A2(new_n258), .B(new_n254), .C(new_n261), .Y(new_n262));
  INVx1_ASAP7_75t_L         g167(.A(new_n262), .Y(new_n263));
  NOR2xp33_ASAP7_75t_L      g168(.A(new_n257), .B(new_n263), .Y(new_n264));
  A2O1A1Ixp33_ASAP7_75t_L   g169(.A1(\NUM_reg[2]/NET0131 ), .A2(\NUM_reg[3]/NET0131 ), .B(\NUM_reg[4]/NET0131 ), .C(new_n259), .Y(new_n265));
  NAND2xp33_ASAP7_75t_L     g170(.A(\NUM_reg[1]/NET0131 ), .B(new_n249), .Y(new_n266));
  NAND2xp33_ASAP7_75t_L     g171(.A(new_n265), .B(new_n266), .Y(new_n267));
  NOR2xp33_ASAP7_75t_L      g172(.A(new_n267), .B(new_n263), .Y(new_n268));
  NAND2xp33_ASAP7_75t_L     g173(.A(new_n255), .B(new_n268), .Y(new_n269));
  INVx1_ASAP7_75t_L         g174(.A(new_n269), .Y(new_n270));
  INVx1_ASAP7_75t_L         g175(.A(new_n268), .Y(new_n271));
  O2A1O1Ixp33_ASAP7_75t_L   g176(.A1(\NUM_reg[0]/NET0131 ), .A2(new_n263), .B(new_n271), .C(new_n257), .Y(new_n272));
  INVx1_ASAP7_75t_L         g177(.A(\NUM_reg[0]/NET0131 ), .Y(new_n273));
  INVx1_ASAP7_75t_L         g178(.A(new_n267), .Y(new_n274));
  A2O1A1Ixp33_ASAP7_75t_L   g179(.A1(new_n274), .A2(new_n273), .B(new_n262), .C(new_n255), .Y(new_n275));
  O2A1O1Ixp33_ASAP7_75t_L   g180(.A1(\NUM_reg[0]/NET0131 ), .A2(new_n269), .B(new_n272), .C(new_n275), .Y(new_n276));
  A2O1A1Ixp33_ASAP7_75t_L   g181(.A1(new_n265), .A2(new_n266), .B(new_n262), .C(new_n255), .Y(new_n277));
  O2A1O1Ixp33_ASAP7_75t_L   g182(.A1(new_n264), .A2(new_n270), .B(new_n276), .C(new_n277), .Y(new_n278));
  NOR2xp33_ASAP7_75t_L      g183(.A(\NUM_reg[0]/NET0131 ), .B(new_n257), .Y(new_n279));
  O2A1O1Ixp33_ASAP7_75t_L   g184(.A1(new_n262), .A2(new_n274), .B(new_n255), .C(new_n279), .Y(new_n280));
  NAND2xp33_ASAP7_75t_L     g185(.A(\NUM_reg[4]/NET0131 ), .B(new_n258), .Y(new_n281));
  NOR2xp33_ASAP7_75t_L      g186(.A(\NUM_reg[0]/NET0131 ), .B(new_n271), .Y(new_n282));
  A2O1A1Ixp33_ASAP7_75t_L   g187(.A1(new_n282), .A2(new_n281), .B(new_n255), .C(\g3774/_1_ ), .Y(new_n283));
  O2A1O1Ixp33_ASAP7_75t_L   g188(.A1(new_n280), .A2(new_n278), .B(new_n255), .C(new_n283), .Y(\DISPNUM2[0]_pad ));
  NAND2xp33_ASAP7_75t_L     g189(.A(new_n273), .B(new_n270), .Y(new_n285));
  A2O1A1O1Ixp25_ASAP7_75t_L g190(.A1(new_n264), .A2(new_n285), .B(new_n280), .C(new_n255), .D(new_n133), .Y(\DISPNUM2[1]_pad ));
  INVx1_ASAP7_75t_L         g191(.A(new_n264), .Y(new_n287));
  O2A1O1Ixp33_ASAP7_75t_L   g192(.A1(\NUM_reg[0]/NET0131 ), .A2(new_n267), .B(new_n263), .C(new_n257), .Y(new_n288));
  O2A1O1Ixp33_ASAP7_75t_L   g193(.A1(\NUM_reg[0]/NET0131 ), .A2(new_n287), .B(new_n288), .C(new_n133), .Y(\DISPNUM2[2]_pad ));
  A2O1A1O1Ixp25_ASAP7_75t_L g194(.A1(new_n254), .A2(new_n250), .B(new_n261), .C(new_n267), .D(new_n257), .Y(new_n290));
  O2A1O1Ixp33_ASAP7_75t_L   g195(.A1(new_n279), .A2(new_n290), .B(new_n275), .C(new_n133), .Y(\DISPNUM2[3]_pad ));
  AOI211xp5_ASAP7_75t_L     g196(.A1(new_n267), .A2(\NUM_reg[0]/NET0131 ), .B(new_n257), .C(new_n263), .Y(new_n292));
  O2A1O1Ixp33_ASAP7_75t_L   g197(.A1(new_n257), .A2(new_n271), .B(new_n292), .C(new_n133), .Y(\DISPNUM2[4]_pad ));
  INVx1_ASAP7_75t_L         g198(.A(new_n277), .Y(new_n294));
  A2O1A1O1Ixp25_ASAP7_75t_L g199(.A1(new_n254), .A2(new_n250), .B(new_n261), .C(new_n271), .D(new_n257), .Y(new_n295));
  O2A1O1Ixp33_ASAP7_75t_L   g200(.A1(new_n273), .A2(new_n269), .B(new_n295), .C(new_n275), .Y(new_n296));
  A2O1A1O1Ixp25_ASAP7_75t_L g201(.A1(new_n294), .A2(new_n296), .B(new_n280), .C(new_n255), .D(new_n133), .Y(\DISPNUM2[5]_pad ));
  INVx1_ASAP7_75t_L         g202(.A(\NUM_reg[4]/NET0131 ), .Y(new_n298));
  O2A1O1Ixp33_ASAP7_75t_L   g203(.A1(\NUM_reg[1]/NET0131 ), .A2(\NUM_reg[2]/NET0131 ), .B(\NUM_reg[3]/NET0131 ), .C(new_n298), .Y(new_n299));
  O2A1O1Ixp33_ASAP7_75t_L   g204(.A1(new_n298), .A2(new_n299), .B(new_n282), .C(new_n255), .Y(new_n300));
  O2A1O1Ixp33_ASAP7_75t_L   g205(.A1(new_n279), .A2(new_n290), .B(new_n269), .C(new_n300), .Y(new_n301));
  AOI31xp33_ASAP7_75t_L     g206(.A1(new_n301), .A2(\RES_DISP_reg/NET0131 ), .A3(new_n255), .B(\EN_DISP_reg/NET0131 ), .Y(\DISPNUM2[6]_pad ));
  AOI21xp33_ASAP7_75t_L     g207(.A1(new_n97), .A2(\RES_DISP_reg/NET0131 ), .B(\EN_DISP_reg/NET0131 ), .Y(SIGN_pad));
  INVx1_ASAP7_75t_L         g208(.A(\MAR_reg[0]/NET0131 ), .Y(new_n306));
  INVx1_ASAP7_75t_L         g209(.A(\MAR_reg[2]/NET0131 ), .Y(new_n307));
  INVx1_ASAP7_75t_L         g210(.A(\MAR_reg[3]/NET0131 ), .Y(new_n308));
  NAND2xp33_ASAP7_75t_L     g211(.A(\MAR_reg[4]/NET0131 ), .B(new_n308), .Y(new_n309));
  INVx1_ASAP7_75t_L         g212(.A(\MAR_reg[1]/NET0131 ), .Y(new_n310));
  NAND4xp25_ASAP7_75t_L     g213(.A(new_n310), .B(new_n307), .C(\MAR_reg[3]/NET0131 ), .D(\MAR_reg[4]/NET0131 ), .Y(new_n311));
  O2A1O1Ixp33_ASAP7_75t_L   g214(.A1(new_n307), .A2(new_n309), .B(new_n311), .C(\MAR_reg[0]/NET0131 ), .Y(new_n312));
  INVx1_ASAP7_75t_L         g215(.A(\MAR_reg[4]/NET0131 ), .Y(new_n313));
  NOR3xp33_ASAP7_75t_L      g216(.A(new_n313), .B(\MAR_reg[2]/NET0131 ), .C(\MAR_reg[3]/NET0131 ), .Y(new_n314));
  NOR3xp33_ASAP7_75t_L      g217(.A(new_n307), .B(\MAR_reg[3]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n315));
  O2A1O1Ixp33_ASAP7_75t_L   g218(.A1(new_n314), .A2(new_n315), .B(\MAR_reg[1]/NET0131 ), .C(new_n312), .Y(new_n316));
  A2O1A1Ixp33_ASAP7_75t_L   g219(.A1(new_n308), .A2(\MAR_reg[2]/NET0131 ), .B(\MAR_reg[1]/NET0131 ), .C(new_n313), .Y(new_n317));
  OAI21xp33_ASAP7_75t_L     g220(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n319), .B(new_n317), .Y(new_n318));
  OAI21xp33_ASAP7_75t_L     g221(.A1(\MAR_reg[4]/NET0131 ), .A2(\MAR_reg[3]/NET0131 ), .B(new_n320), .Y(new_n319));
  NAND2xp33_ASAP7_75t_L     g222(.A(\MAR_reg[3]/NET0131 ), .B(\MAR_reg[4]/NET0131 ), .Y(new_n320));
  INVx1_ASAP7_75t_L         g223(.A(new_n318), .Y(new_n321));
  INVx1_ASAP7_75t_L         g224(.A(\STATO_reg[0]/NET0131 ), .Y(new_n322));
  INVx1_ASAP7_75t_L         g225(.A(\STATO_reg[1]/NET0131 ), .Y(new_n323));
  NOR2xp33_ASAP7_75t_L      g226(.A(\STATO_reg[2]/NET0131 ), .B(new_n323), .Y(new_n324));
  NAND2xp33_ASAP7_75t_L     g227(.A(new_n322), .B(new_n324), .Y(new_n325));
  O2A1O1Ixp33_ASAP7_75t_L   g228(.A1(new_n306), .A2(new_n321), .B(new_n316), .C(new_n325), .Y(new_n326));
  INVx1_ASAP7_75t_L         g229(.A(\STATO_reg[2]/NET0131 ), .Y(new_n327));
  NOR3xp33_ASAP7_75t_L      g230(.A(new_n327), .B(\STATO_reg[0]/NET0131 ), .C(\STATO_reg[1]/NET0131 ), .Y(new_n328));
  NOR2xp33_ASAP7_75t_L      g231(.A(\STATO_reg[2]/NET0131 ), .B(\STATO_reg[1]/NET0131 ), .Y(new_n329));
  O2A1O1Ixp33_ASAP7_75t_L   g232(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[0]/NET0131 ), .C(new_n326), .Y(new_n330));
  O2A1O1Ixp33_ASAP7_75t_L   g233(.A1(\MAR_reg[3]/NET0131 ), .A2(new_n307), .B(new_n310), .C(\MAR_reg[4]/NET0131 ), .Y(new_n331));
  NOR3xp33_ASAP7_75t_L      g234(.A(new_n308), .B(\MAR_reg[2]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n332));
  OAI31xp33_ASAP7_75t_L     g235(.A1(new_n331), .A2(new_n314), .A3(new_n332), .B(\MAR_reg[0]/NET0131 ), .Y(new_n333));
  NAND3xp33_ASAP7_75t_L     g236(.A(new_n307), .B(new_n308), .C(\MAR_reg[4]/NET0131 ), .Y(new_n334));
  NAND3xp33_ASAP7_75t_L     g237(.A(new_n307), .B(new_n313), .C(\MAR_reg[3]/NET0131 ), .Y(new_n335));
  NAND2xp33_ASAP7_75t_L     g238(.A(new_n335), .B(new_n334), .Y(new_n336));
  NAND3xp33_ASAP7_75t_L     g239(.A(new_n308), .B(new_n313), .C(\MAR_reg[2]/NET0131 ), .Y(new_n337));
  O2A1O1Ixp33_ASAP7_75t_L   g240(.A1(\MAR_reg[1]/NET0131 ), .A2(new_n309), .B(new_n337), .C(\MAR_reg[0]/NET0131 ), .Y(new_n338));
  NAND2xp33_ASAP7_75t_L     g241(.A(\MAR_reg[1]/NET0131 ), .B(new_n313), .Y(new_n339));
  O2A1O1Ixp33_ASAP7_75t_L   g242(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n313), .B(new_n339), .C(new_n306), .Y(new_n340));
  NOR3xp33_ASAP7_75t_L      g243(.A(new_n338), .B(new_n336), .C(new_n340), .Y(new_n341));
  NAND2xp33_ASAP7_75t_L     g244(.A(\MAX_reg[7]/NET0131 ), .B(new_n341), .Y(new_n342));
  INVx1_ASAP7_75t_L         g245(.A(\MAX_reg[7]/NET0131 ), .Y(new_n343));
  NOR2xp33_ASAP7_75t_L      g246(.A(\MAR_reg[4]/NET0131 ), .B(\MAR_reg[3]/NET0131 ), .Y(new_n344));
  NOR3xp33_ASAP7_75t_L      g247(.A(new_n313), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[3]/NET0131 ), .Y(new_n345));
  A2O1A1Ixp33_ASAP7_75t_L   g248(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n344), .B(new_n345), .C(new_n306), .Y(new_n346));
  NOR2xp33_ASAP7_75t_L      g249(.A(\MAR_reg[4]/NET0131 ), .B(new_n310), .Y(new_n347));
  A2O1A1Ixp33_ASAP7_75t_L   g250(.A1(new_n307), .A2(\MAR_reg[4]/NET0131 ), .B(new_n347), .C(\MAR_reg[0]/NET0131 ), .Y(new_n348));
  NAND4xp25_ASAP7_75t_L     g251(.A(new_n346), .B(new_n334), .C(new_n348), .D(new_n335), .Y(new_n349));
  NOR3xp33_ASAP7_75t_L      g252(.A(new_n306), .B(new_n308), .C(\MAR_reg[2]/NET0131 ), .Y(new_n350));
  NAND2xp33_ASAP7_75t_L     g253(.A(\MAR_reg[3]/NET0131 ), .B(new_n313), .Y(new_n351));
  NAND2xp33_ASAP7_75t_L     g254(.A(new_n313), .B(new_n307), .Y(new_n352));
  AOI21xp33_ASAP7_75t_L     g255(.A1(new_n352), .A2(new_n351), .B(new_n354), .Y(new_n353));
  NAND2xp33_ASAP7_75t_L     g256(.A(\MAR_reg[1]/NET0131 ), .B(\MAR_reg[0]/NET0131 ), .Y(new_n354));
  NAND2xp33_ASAP7_75t_L     g257(.A(\MAR_reg[2]/NET0131 ), .B(new_n308), .Y(new_n355));
  NAND3xp33_ASAP7_75t_L     g258(.A(new_n306), .B(new_n308), .C(\MAR_reg[4]/NET0131 ), .Y(new_n356));
  OAI21xp33_ASAP7_75t_L     g259(.A1(new_n355), .A2(new_n358), .B(new_n356), .Y(new_n357));
  NOR2xp33_ASAP7_75t_L      g260(.A(new_n310), .B(new_n306), .Y(new_n358));
  NOR3xp33_ASAP7_75t_L      g261(.A(new_n313), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[2]/NET0131 ), .Y(new_n359));
  NOR4xp25_ASAP7_75t_L      g262(.A(new_n357), .B(new_n353), .C(new_n350), .D(new_n359), .Y(new_n360));
  MAJIxp5_ASAP7_75t_L       g263(.A(new_n362), .B(new_n343), .C(new_n349), .Y(new_n361));
  NOR2xp33_ASAP7_75t_L      g264(.A(\MAX_reg[6]/NET0131 ), .B(new_n360), .Y(new_n362));
  NOR3xp33_ASAP7_75t_L      g265(.A(\MAR_reg[1]/NET0131 ), .B(\MAR_reg[3]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n363));
  NAND3xp33_ASAP7_75t_L     g266(.A(\MAR_reg[1]/NET0131 ), .B(\MAR_reg[2]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n364));
  OAI21xp33_ASAP7_75t_L     g267(.A1(\MAR_reg[4]/NET0131 ), .A2(new_n366), .B(new_n364), .Y(new_n365));
  NAND2xp33_ASAP7_75t_L     g268(.A(\MAR_reg[3]/NET0131 ), .B(\MAR_reg[1]/NET0131 ), .Y(new_n366));
  OAI31xp33_ASAP7_75t_L     g269(.A1(new_n365), .A2(new_n332), .A3(new_n363), .B(new_n306), .Y(new_n367));
  NAND2xp33_ASAP7_75t_L     g270(.A(\MAR_reg[4]/NET0131 ), .B(\MAR_reg[3]/NET0131 ), .Y(new_n368));
  INVx1_ASAP7_75t_L         g271(.A(new_n368), .Y(new_n369));
  A2O1A1Ixp33_ASAP7_75t_L   g272(.A1(new_n369), .A2(\MAR_reg[2]/NET0131 ), .B(new_n332), .C(\MAR_reg[1]/NET0131 ), .Y(new_n370));
  OAI211xp5_ASAP7_75t_L     g273(.A1(new_n314), .A2(new_n315), .B(\MAR_reg[0]/NET0131 ), .C(\MAR_reg[1]/NET0131 ), .Y(new_n371));
  NAND3xp33_ASAP7_75t_L     g274(.A(new_n367), .B(new_n370), .C(new_n371), .Y(new_n372));
  NAND3xp33_ASAP7_75t_L     g275(.A(new_n306), .B(new_n308), .C(\MAR_reg[2]/NET0131 ), .Y(new_n373));
  NOR2xp33_ASAP7_75t_L      g276(.A(\MAR_reg[4]/NET0131 ), .B(\MAR_reg[2]/NET0131 ), .Y(new_n374));
  NAND3xp33_ASAP7_75t_L     g277(.A(\MAR_reg[1]/NET0131 ), .B(\MAR_reg[2]/NET0131 ), .C(\MAR_reg[3]/NET0131 ), .Y(new_n375));
  INVx1_ASAP7_75t_L         g278(.A(new_n375), .Y(new_n376));
  OAI21xp33_ASAP7_75t_L     g279(.A1(\MAR_reg[1]/NET0131 ), .A2(\MAR_reg[2]/NET0131 ), .B(\MAR_reg[3]/NET0131 ), .Y(new_n377));
  O2A1O1Ixp33_ASAP7_75t_L   g280(.A1(\MAR_reg[2]/NET0131 ), .A2(\MAR_reg[4]/NET0131 ), .B(new_n377), .C(new_n306), .Y(new_n378));
  AOI211xp5_ASAP7_75t_L     g281(.A1(new_n310), .A2(new_n374), .B(new_n376), .C(new_n378), .Y(new_n379));
  NOR3xp33_ASAP7_75t_L      g282(.A(new_n313), .B(\MAR_reg[0]/NET0131 ), .C(\MAR_reg[3]/NET0131 ), .Y(new_n380));
  NOR3xp33_ASAP7_75t_L      g283(.A(new_n310), .B(new_n307), .C(\MAR_reg[4]/NET0131 ), .Y(new_n381));
  NOR4xp25_ASAP7_75t_L      g284(.A(new_n332), .B(new_n381), .C(new_n345), .D(new_n380), .Y(new_n382));
  NAND4xp25_ASAP7_75t_L     g285(.A(new_n379), .B(\MAX_reg[5]/NET0131 ), .C(new_n373), .D(new_n382), .Y(new_n383));
  AOI31xp33_ASAP7_75t_L     g286(.A1(new_n379), .A2(new_n373), .A3(new_n382), .B(\MAX_reg[5]/NET0131 ), .Y(new_n384));
  AOI31xp33_ASAP7_75t_L     g287(.A1(new_n96), .A2(new_n383), .A3(new_n372), .B(new_n384), .Y(new_n385));
  NAND2xp33_ASAP7_75t_L     g288(.A(\MAX_reg[6]/NET0131 ), .B(new_n360), .Y(new_n386));
  OAI21xp33_ASAP7_75t_L     g289(.A1(new_n388), .A2(new_n385), .B(new_n361), .Y(new_n387));
  NAND2xp33_ASAP7_75t_L     g290(.A(new_n386), .B(new_n342), .Y(new_n388));
  NOR2xp33_ASAP7_75t_L      g291(.A(\MAR_reg[3]/NET0131 ), .B(new_n313), .Y(new_n389));
  NOR3xp33_ASAP7_75t_L      g292(.A(new_n368), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[2]/NET0131 ), .Y(new_n390));
  A2O1A1Ixp33_ASAP7_75t_L   g293(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n389), .B(new_n390), .C(new_n306), .Y(new_n391));
  A2O1A1Ixp33_ASAP7_75t_L   g294(.A1(new_n389), .A2(new_n307), .B(new_n315), .C(\MAR_reg[1]/NET0131 ), .Y(new_n392));
  NAND4xp25_ASAP7_75t_L     g295(.A(new_n333), .B(\MAX_reg[0]/NET0131 ), .C(new_n391), .D(new_n392), .Y(new_n393));
  AND3x1_ASAP7_75t_L        g296(.A(\MAR_reg[0]/NET0131 ), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n394));
  AOI31xp33_ASAP7_75t_L     g297(.A1(new_n310), .A2(\MAR_reg[2]/NET0131 ), .A3(new_n344), .B(new_n394), .Y(new_n395));
  NOR2xp33_ASAP7_75t_L      g298(.A(\MAR_reg[4]/NET0131 ), .B(new_n308), .Y(new_n396));
  NOR2xp33_ASAP7_75t_L      g299(.A(\MAR_reg[2]/NET0131 ), .B(\MAR_reg[1]/NET0131 ), .Y(new_n397));
  A2O1A1Ixp33_ASAP7_75t_L   g300(.A1(new_n396), .A2(new_n307), .B(new_n397), .C(new_n306), .Y(new_n398));
  NAND4xp25_ASAP7_75t_L     g301(.A(new_n398), .B(new_n311), .C(new_n375), .D(new_n395), .Y(new_n399));
  MAJIxp5_ASAP7_75t_L       g302(.A(new_n393), .B(new_n102), .C(new_n399), .Y(new_n400));
  INVx1_ASAP7_75t_L         g303(.A(new_n344), .Y(new_n401));
  NAND2xp33_ASAP7_75t_L     g304(.A(\MAR_reg[1]/NET0131 ), .B(\MAR_reg[0]/NET0131 ), .Y(new_n402));
  OAI211xp5_ASAP7_75t_L     g305(.A1(new_n310), .A2(new_n307), .B(new_n308), .C(\MAR_reg[4]/NET0131 ), .Y(new_n403));
  OAI211xp5_ASAP7_75t_L     g306(.A1(new_n401), .A2(new_n402), .B(new_n403), .C(new_n373), .Y(new_n404));
  NAND3xp33_ASAP7_75t_L     g307(.A(new_n310), .B(new_n307), .C(\MAR_reg[4]/NET0131 ), .Y(new_n405));
  O2A1O1Ixp33_ASAP7_75t_L   g308(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n351), .B(new_n405), .C(new_n306), .Y(new_n406));
  OAI21xp33_ASAP7_75t_L     g309(.A1(\MAR_reg[4]/NET0131 ), .A2(\MAR_reg[3]/NET0131 ), .B(new_n408), .Y(new_n407));
  NAND2xp33_ASAP7_75t_L     g310(.A(\MAR_reg[3]/NET0131 ), .B(\MAR_reg[4]/NET0131 ), .Y(new_n408));
  NOR2xp33_ASAP7_75t_L      g311(.A(\MAR_reg[1]/NET0131 ), .B(new_n306), .Y(new_n409));
  A2O1A1Ixp33_ASAP7_75t_L   g312(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n313), .B(new_n407), .C(new_n409), .Y(new_n410));
  NOR4xp25_ASAP7_75t_L      g313(.A(new_n310), .B(new_n307), .C(new_n313), .D(\MAR_reg[3]/NET0131 ), .Y(new_n411));
  O2A1O1Ixp33_ASAP7_75t_L   g314(.A1(new_n332), .A2(new_n359), .B(new_n306), .C(new_n411), .Y(new_n412));
  NOR3xp33_ASAP7_75t_L      g315(.A(new_n368), .B(new_n306), .C(\MAR_reg[2]/NET0131 ), .Y(new_n413));
  INVx1_ASAP7_75t_L         g316(.A(new_n413), .Y(new_n414));
  NAND3xp33_ASAP7_75t_L     g317(.A(new_n412), .B(new_n410), .C(new_n414), .Y(new_n415));
  OAI32xp33_ASAP7_75t_L     g318(.A1(new_n99), .A2(new_n404), .A3(new_n406), .B1(new_n415), .B2(new_n111), .Y(new_n416));
  NAND2xp33_ASAP7_75t_L     g319(.A(\MAR_reg[2]/NET0131 ), .B(new_n313), .Y(new_n417));
  AOI311xp33_ASAP7_75t_L    g320(.A1(new_n401), .A2(new_n417), .A3(new_n368), .B(new_n306), .C(\MAR_reg[1]/NET0131 ), .Y(new_n418));
  NAND4xp25_ASAP7_75t_L     g321(.A(new_n308), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[2]/NET0131 ), .D(\MAR_reg[4]/NET0131 ), .Y(new_n419));
  A2O1A1Ixp33_ASAP7_75t_L   g322(.A1(new_n335), .A2(new_n405), .B(\MAR_reg[0]/NET0131 ), .C(new_n419), .Y(new_n420));
  NOR4xp25_ASAP7_75t_L      g323(.A(new_n418), .B(new_n420), .C(new_n111), .D(new_n413), .Y(new_n421));
  O2A1O1Ixp33_ASAP7_75t_L   g324(.A1(\MAR_reg[1]/NET0131 ), .A2(new_n313), .B(new_n351), .C(\MAR_reg[2]/NET0131 ), .Y(new_n422));
  A2O1A1Ixp33_ASAP7_75t_L   g325(.A1(\MAR_reg[0]/NET0131 ), .A2(new_n422), .B(new_n404), .C(new_n99), .Y(new_n423));
  OAI31xp33_ASAP7_75t_L     g326(.A1(new_n418), .A2(new_n420), .A3(new_n413), .B(new_n111), .Y(new_n424));
  OA21x2_ASAP7_75t_L        g327(.A1(new_n423), .A2(new_n421), .B(new_n424), .Y(new_n425));
  OAI21xp33_ASAP7_75t_L     g328(.A1(new_n400), .A2(new_n416), .B(new_n425), .Y(new_n426));
  INVx1_ASAP7_75t_L         g329(.A(\MAX_reg[5]/NET0131 ), .Y(new_n427));
  O2A1O1Ixp33_ASAP7_75t_L   g330(.A1(\MAR_reg[1]/NET0131 ), .A2(\MAR_reg[2]/NET0131 ), .B(\MAR_reg[3]/NET0131 ), .C(new_n374), .Y(new_n428));
  OAI221xp5_ASAP7_75t_L     g331(.A1(\MAR_reg[1]/NET0131 ), .A2(new_n352), .B1(new_n306), .B2(new_n428), .C(new_n375), .Y(new_n429));
  NAND3xp33_ASAP7_75t_L     g332(.A(new_n310), .B(new_n308), .C(\MAR_reg[4]/NET0131 ), .Y(new_n430));
  NAND3xp33_ASAP7_75t_L     g333(.A(new_n313), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[2]/NET0131 ), .Y(new_n431));
  NAND5xp2_ASAP7_75t_L      g334(.A(new_n335), .B(new_n430), .C(new_n356), .D(new_n373), .E(new_n431), .Y(new_n432));
  NOR3xp33_ASAP7_75t_L      g335(.A(new_n429), .B(new_n427), .C(new_n432), .Y(new_n433));
  AOI21xp33_ASAP7_75t_L     g336(.A1(\MAX_reg[6]/NET0131 ), .A2(new_n360), .B(new_n433), .Y(new_n434));
  NOR2xp33_ASAP7_75t_L      g337(.A(new_n343), .B(new_n349), .Y(new_n435));
  AND3x1_ASAP7_75t_L        g338(.A(new_n367), .B(new_n370), .C(new_n371), .Y(new_n436));
  AOI21xp33_ASAP7_75t_L     g339(.A1(\MAX_reg[4]/NET0131 ), .A2(new_n436), .B(new_n435), .Y(new_n437));
  NOR2xp33_ASAP7_75t_L      g340(.A(new_n310), .B(new_n351), .Y(new_n438));
  A2O1A1Ixp33_ASAP7_75t_L   g341(.A1(new_n389), .A2(new_n307), .B(new_n315), .C(new_n310), .Y(new_n439));
  NOR2xp33_ASAP7_75t_L      g342(.A(\MAR_reg[0]/NET0131 ), .B(new_n439), .Y(new_n440));
  A2O1A1O1Ixp25_ASAP7_75t_L g343(.A1(new_n307), .A2(new_n369), .B(new_n438), .C(\MAR_reg[0]/NET0131 ), .D(new_n440), .Y(new_n441));
  OAI21xp33_ASAP7_75t_L     g344(.A1(new_n441), .A2(\MAX_reg[8]/NET0131 ), .B(new_n443), .Y(new_n442));
  NAND2xp33_ASAP7_75t_L     g345(.A(\MAX_reg[8]/NET0131 ), .B(new_n441), .Y(new_n443));
  AOI311xp33_ASAP7_75t_L    g346(.A1(new_n426), .A2(new_n434), .A3(new_n437), .B(new_n442), .C(new_n387), .Y(new_n444));
  INVx1_ASAP7_75t_L         g347(.A(\MAX_reg[6]/NET0131 ), .Y(new_n445));
  INVx1_ASAP7_75t_L         g348(.A(new_n350), .Y(new_n446));
  NAND3xp33_ASAP7_75t_L     g349(.A(new_n448), .B(new_n449), .C(\MAR_reg[1]/NET0131 ), .Y(new_n447));
  NOR2xp33_ASAP7_75t_L      g350(.A(\MAR_reg[4]/NET0131 ), .B(new_n306), .Y(new_n448));
  NAND2xp33_ASAP7_75t_L     g351(.A(\MAR_reg[2]/NET0131 ), .B(new_n308), .Y(new_n449));
  NOR2xp33_ASAP7_75t_L      g352(.A(\MAR_reg[3]/NET0131 ), .B(new_n307), .Y(new_n450));
  O2A1O1Ixp33_ASAP7_75t_L   g353(.A1(new_n306), .A2(new_n310), .B(new_n450), .C(new_n380), .Y(new_n451));
  NAND4xp25_ASAP7_75t_L     g354(.A(new_n451), .B(new_n446), .C(new_n447), .D(new_n405), .Y(new_n452));
  MAJIxp5_ASAP7_75t_L       g355(.A(new_n454), .B(\MAX_reg[7]/NET0131 ), .C(new_n341), .Y(new_n453));
  NAND2xp33_ASAP7_75t_L     g356(.A(new_n445), .B(new_n452), .Y(new_n454));
  OAI21xp33_ASAP7_75t_L     g357(.A1(new_n432), .A2(new_n429), .B(new_n427), .Y(new_n455));
  OAI31xp33_ASAP7_75t_L     g358(.A1(new_n436), .A2(\MAX_reg[4]/NET0131 ), .A3(new_n433), .B(new_n455), .Y(new_n456));
  NOR2xp33_ASAP7_75t_L      g359(.A(new_n445), .B(new_n452), .Y(new_n457));
  AOI21xp33_ASAP7_75t_L     g360(.A1(new_n456), .A2(new_n459), .B(new_n453), .Y(new_n458));
  NOR2xp33_ASAP7_75t_L      g361(.A(new_n457), .B(new_n435), .Y(new_n459));
  O2A1O1Ixp33_ASAP7_75t_L   g362(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n309), .B(new_n337), .C(new_n310), .Y(new_n460));
  A2O1A1O1Ixp25_ASAP7_75t_L g363(.A1(new_n309), .A2(new_n351), .B(\MAR_reg[2]/NET0131 ), .C(new_n317), .D(new_n306), .Y(new_n461));
  NOR4xp25_ASAP7_75t_L      g364(.A(new_n461), .B(new_n101), .C(new_n312), .D(new_n460), .Y(new_n462));
  NAND3xp33_ASAP7_75t_L     g365(.A(\MAR_reg[0]/NET0131 ), .B(\MAR_reg[1]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n463));
  OAI21xp33_ASAP7_75t_L     g366(.A1(new_n465), .A2(new_n401), .B(new_n463), .Y(new_n464));
  NAND2xp33_ASAP7_75t_L     g367(.A(\MAR_reg[2]/NET0131 ), .B(new_n310), .Y(new_n465));
  A2O1A1Ixp33_ASAP7_75t_L   g368(.A1(new_n313), .A2(\MAR_reg[3]/NET0131 ), .B(new_n310), .C(new_n307), .Y(new_n466));
  NOR3xp33_ASAP7_75t_L      g369(.A(new_n468), .B(new_n469), .C(new_n464), .Y(new_n467));
  NOR2xp33_ASAP7_75t_L      g370(.A(\MAR_reg[0]/NET0131 ), .B(new_n466), .Y(new_n468));
  NAND2xp33_ASAP7_75t_L     g371(.A(new_n375), .B(new_n311), .Y(new_n469));
  MAJIxp5_ASAP7_75t_L       g372(.A(new_n462), .B(\MAX_reg[1]/NET0131 ), .C(new_n467), .Y(new_n470));
  O2A1O1Ixp33_ASAP7_75t_L   g373(.A1(new_n332), .A2(new_n359), .B(\MAR_reg[0]/NET0131 ), .C(new_n404), .Y(new_n471));
  AOI21xp33_ASAP7_75t_L     g374(.A1(\MAX_reg[2]/NET0131 ), .A2(new_n471), .B(new_n421), .Y(new_n472));
  OAI21xp33_ASAP7_75t_L     g375(.A1(new_n423), .A2(new_n421), .B(new_n424), .Y(new_n473));
  A2O1A1Ixp33_ASAP7_75t_L   g376(.A1(new_n470), .A2(new_n472), .B(new_n473), .C(new_n434), .Y(new_n474));
  INVx1_ASAP7_75t_L         g377(.A(new_n437), .Y(new_n475));
  OAI21xp33_ASAP7_75t_L     g378(.A1(new_n441), .A2(new_n97), .B(new_n477), .Y(new_n476));
  NAND2xp33_ASAP7_75t_L     g379(.A(new_n97), .B(new_n441), .Y(new_n477));
  O2A1O1Ixp33_ASAP7_75t_L   g380(.A1(new_n475), .A2(new_n474), .B(new_n458), .C(new_n476), .Y(new_n478));
  OAI21xp33_ASAP7_75t_L     g381(.A1(new_n478), .A2(new_n444), .B(new_n480), .Y(new_n479));
  NAND2xp33_ASAP7_75t_L     g382(.A(new_n333), .B(new_n316), .Y(new_n480));
  OAI211xp5_ASAP7_75t_L     g383(.A1(new_n475), .A2(new_n474), .B(new_n458), .C(new_n476), .Y(new_n481));
  NAND2xp33_ASAP7_75t_L     g384(.A(new_n383), .B(new_n386), .Y(new_n482));
  O2A1O1Ixp33_ASAP7_75t_L   g385(.A1(new_n400), .A2(new_n416), .B(new_n425), .C(new_n482), .Y(new_n483));
  A2O1A1Ixp33_ASAP7_75t_L   g386(.A1(new_n483), .A2(new_n437), .B(new_n387), .C(new_n442), .Y(new_n484));
  NAND3xp33_ASAP7_75t_L     g387(.A(new_n484), .B(new_n481), .C(\MAX_reg[0]/NET0131 ), .Y(new_n485));
  NAND2xp33_ASAP7_75t_L     g388(.A(new_n485), .B(new_n479), .Y(new_n486));
  O2A1O1Ixp33_ASAP7_75t_L   g389(.A1(new_n397), .A2(new_n332), .B(new_n306), .C(new_n390), .Y(new_n487));
  NAND4xp25_ASAP7_75t_L     g390(.A(new_n487), .B(\TEMP_reg[1]/NET0131 ), .C(new_n375), .D(new_n395), .Y(new_n488));
  INVx1_ASAP7_75t_L         g391(.A(\TEMP_reg[0]/NET0131 ), .Y(new_n489));
  A2O1A1O1Ixp25_ASAP7_75t_L g392(.A1(new_n307), .A2(new_n389), .B(new_n315), .C(\MAR_reg[1]/NET0131 ), .D(new_n489), .Y(new_n490));
  NAND2xp33_ASAP7_75t_L     g393(.A(new_n391), .B(new_n490), .Y(new_n491));
  AOI31xp33_ASAP7_75t_L     g394(.A1(new_n487), .A2(new_n375), .A3(new_n395), .B(\TEMP_reg[1]/NET0131 ), .Y(new_n492));
  O2A1O1Ixp33_ASAP7_75t_L   g395(.A1(new_n461), .A2(new_n491), .B(new_n488), .C(new_n492), .Y(new_n493));
  INVx1_ASAP7_75t_L         g396(.A(\TEMP_reg[2]/NET0131 ), .Y(new_n494));
  A2O1A1Ixp33_ASAP7_75t_L   g397(.A1(\MAR_reg[0]/NET0131 ), .A2(new_n422), .B(new_n404), .C(new_n494), .Y(new_n495));
  NOR3xp33_ASAP7_75t_L      g398(.A(new_n307), .B(\MAR_reg[0]/NET0131 ), .C(\MAR_reg[3]/NET0131 ), .Y(new_n496));
  AOI31xp33_ASAP7_75t_L     g399(.A1(\MAR_reg[0]/NET0131 ), .A2(\MAR_reg[1]/NET0131 ), .A3(new_n344), .B(new_n496), .Y(new_n497));
  A2O1A1Ixp33_ASAP7_75t_L   g400(.A1(new_n396), .A2(new_n307), .B(new_n359), .C(\MAR_reg[0]/NET0131 ), .Y(new_n498));
  NAND4xp25_ASAP7_75t_L     g401(.A(new_n497), .B(new_n498), .C(\TEMP_reg[2]/NET0131 ), .D(new_n403), .Y(new_n499));
  NAND3xp33_ASAP7_75t_L     g402(.A(new_n493), .B(new_n495), .C(new_n499), .Y(new_n500));
  NAND3xp33_ASAP7_75t_L     g403(.A(new_n333), .B(new_n490), .C(new_n391), .Y(new_n501));
  NAND2xp33_ASAP7_75t_L     g404(.A(new_n499), .B(new_n495), .Y(new_n502));
  A2O1A1Ixp33_ASAP7_75t_L   g405(.A1(new_n488), .A2(new_n501), .B(new_n492), .C(new_n502), .Y(new_n503));
  NAND4xp25_ASAP7_75t_L     g406(.A(new_n333), .B(\TEMP_reg[0]/NET0131 ), .C(new_n391), .D(new_n392), .Y(new_n504));
  OAI31xp33_ASAP7_75t_L     g407(.A1(new_n461), .A2(new_n312), .A3(new_n460), .B(new_n489), .Y(new_n505));
  NAND2xp33_ASAP7_75t_L     g408(.A(new_n504), .B(new_n505), .Y(new_n506));
  AOI21xp33_ASAP7_75t_L     g409(.A1(new_n500), .A2(new_n503), .B(new_n506), .Y(new_n507));
  NOR3xp33_ASAP7_75t_L      g410(.A(new_n378), .B(new_n345), .C(new_n496), .Y(new_n508));
  INVx1_ASAP7_75t_L         g411(.A(\TEMP_reg[5]/NET0131 ), .Y(new_n509));
  OAI31xp33_ASAP7_75t_L     g412(.A1(\MAR_reg[0]/NET0131 ), .A2(\MAR_reg[3]/NET0131 ), .A3(new_n313), .B(new_n375), .Y(new_n510));
  AOI211xp5_ASAP7_75t_L     g413(.A1(new_n308), .A2(\MAR_reg[1]/NET0131 ), .B(\MAR_reg[2]/NET0131 ), .C(\MAR_reg[4]/NET0131 ), .Y(new_n511));
  NOR4xp25_ASAP7_75t_L      g414(.A(new_n510), .B(new_n511), .C(new_n509), .D(new_n381), .Y(new_n512));
  NAND2xp33_ASAP7_75t_L     g415(.A(new_n508), .B(new_n512), .Y(new_n513));
  AOI31xp33_ASAP7_75t_L     g416(.A1(new_n412), .A2(new_n410), .A3(new_n414), .B(\TEMP_reg[3]/NET0131 ), .Y(new_n514));
  NAND4xp25_ASAP7_75t_L     g417(.A(new_n367), .B(\TEMP_reg[4]/NET0131 ), .C(new_n370), .D(new_n371), .Y(new_n515));
  AOI31xp33_ASAP7_75t_L     g418(.A1(new_n367), .A2(new_n370), .A3(new_n371), .B(\TEMP_reg[4]/NET0131 ), .Y(new_n516));
  AOI31xp33_ASAP7_75t_L     g419(.A1(new_n379), .A2(new_n373), .A3(new_n382), .B(\TEMP_reg[5]/NET0131 ), .Y(new_n517));
  A2O1A1O1Ixp25_ASAP7_75t_L g420(.A1(new_n515), .A2(new_n514), .B(new_n516), .C(new_n513), .D(new_n517), .Y(new_n518));
  INVx1_ASAP7_75t_L         g421(.A(\TEMP_reg[6]/NET0131 ), .Y(new_n519));
  NOR2xp33_ASAP7_75t_L      g422(.A(new_n519), .B(new_n452), .Y(new_n520));
  O2A1O1Ixp33_ASAP7_75t_L   g423(.A1(\MAR_reg[2]/NET0131 ), .A2(\MAR_reg[4]/NET0131 ), .B(new_n351), .C(new_n310), .Y(new_n521));
  A2O1A1Ixp33_ASAP7_75t_L   g424(.A1(new_n307), .A2(\MAR_reg[3]/NET0131 ), .B(new_n521), .C(\MAR_reg[0]/NET0131 ), .Y(new_n522));
  NAND4xp25_ASAP7_75t_L     g425(.A(new_n522), .B(\TEMP_reg[6]/NET0131 ), .C(new_n451), .D(new_n405), .Y(new_n523));
  A2O1A1Ixp33_ASAP7_75t_L   g426(.A1(new_n396), .A2(new_n307), .B(new_n359), .C(new_n306), .Y(new_n524));
  INVx1_ASAP7_75t_L         g427(.A(\TEMP_reg[3]/NET0131 ), .Y(new_n525));
  NOR2xp33_ASAP7_75t_L      g428(.A(new_n525), .B(new_n413), .Y(new_n526));
  NAND4xp25_ASAP7_75t_L     g429(.A(new_n410), .B(new_n526), .C(new_n524), .D(new_n419), .Y(new_n527));
  NAND4xp25_ASAP7_75t_L     g430(.A(new_n523), .B(new_n513), .C(new_n515), .D(new_n527), .Y(new_n528));
  AOI31xp33_ASAP7_75t_L     g431(.A1(new_n497), .A2(new_n498), .A3(new_n403), .B(\TEMP_reg[2]/NET0131 ), .Y(new_n529));
  A2O1A1O1Ixp25_ASAP7_75t_L g432(.A1(new_n488), .A2(new_n501), .B(new_n492), .C(new_n499), .D(new_n529), .Y(new_n530));
  NAND2xp33_ASAP7_75t_L     g433(.A(new_n519), .B(new_n452), .Y(new_n531));
  OAI221xp5_ASAP7_75t_L     g434(.A1(new_n518), .A2(new_n520), .B1(new_n528), .B2(new_n530), .C(new_n531), .Y(new_n532));
  INVx1_ASAP7_75t_L         g435(.A(\TEMP_reg[7]/NET0131 ), .Y(new_n533));
  A2O1A1Ixp33_ASAP7_75t_L   g436(.A1(new_n309), .A2(new_n351), .B(\MAR_reg[2]/NET0131 ), .C(new_n348), .Y(new_n534));
  NOR3xp33_ASAP7_75t_L      g437(.A(new_n534), .B(new_n533), .C(new_n338), .Y(new_n535));
  O2A1O1Ixp33_ASAP7_75t_L   g438(.A1(new_n389), .A2(new_n396), .B(new_n307), .C(new_n340), .Y(new_n536));
  A2O1A1O1Ixp25_ASAP7_75t_L g439(.A1(new_n337), .A2(new_n430), .B(\MAR_reg[0]/NET0131 ), .C(new_n536), .D(\TEMP_reg[7]/NET0131 ), .Y(new_n537));
  O2A1O1Ixp33_ASAP7_75t_L   g440(.A1(new_n535), .A2(new_n537), .B(new_n532), .C(new_n507), .Y(new_n538));
  AND4x1_ASAP7_75t_L        g441(.A(new_n410), .B(new_n526), .C(new_n524), .D(new_n419), .Y(new_n539));
  AOI21xp33_ASAP7_75t_L     g442(.A1(new_n529), .A2(new_n527), .B(new_n514), .Y(new_n540));
  NAND2xp33_ASAP7_75t_L     g443(.A(new_n513), .B(new_n515), .Y(new_n541));
  NAND3xp33_ASAP7_75t_L     g444(.A(new_n515), .B(new_n513), .C(new_n499), .Y(new_n542));
  OAI32xp33_ASAP7_75t_L     g445(.A1(new_n493), .A2(new_n542), .A3(new_n539), .B1(new_n540), .B2(new_n541), .Y(new_n543));
  AOI21xp33_ASAP7_75t_L     g446(.A1(new_n513), .A2(new_n516), .B(new_n517), .Y(new_n544));
  OAI21xp33_ASAP7_75t_L     g447(.A1(new_n452), .A2(\TEMP_reg[6]/NET0131 ), .B(new_n546), .Y(new_n545));
  NAND2xp33_ASAP7_75t_L     g448(.A(\TEMP_reg[6]/NET0131 ), .B(new_n452), .Y(new_n546));
  OAI21xp33_ASAP7_75t_L     g449(.A1(new_n543), .A2(new_n548), .B(new_n550), .Y(new_n547));
  OAI21xp33_ASAP7_75t_L     g450(.A1(new_n545), .A2(new_n544), .B(new_n549), .Y(new_n548));
  NAND2xp33_ASAP7_75t_L     g451(.A(new_n545), .B(new_n544), .Y(new_n549));
  NAND2xp33_ASAP7_75t_L     g452(.A(new_n545), .B(new_n543), .Y(new_n550));
  NAND2xp33_ASAP7_75t_L     g453(.A(new_n499), .B(new_n492), .Y(new_n551));
  NAND3xp33_ASAP7_75t_L     g454(.A(new_n501), .B(new_n488), .C(new_n499), .Y(new_n552));
  NOR2xp33_ASAP7_75t_L      g455(.A(new_n413), .B(new_n420), .Y(new_n553));
  A2O1A1Ixp33_ASAP7_75t_L   g456(.A1(new_n553), .A2(new_n410), .B(\TEMP_reg[3]/NET0131 ), .C(new_n527), .Y(new_n554));
  NAND4xp25_ASAP7_75t_L     g457(.A(new_n552), .B(new_n554), .C(new_n551), .D(new_n495), .Y(new_n555));
  INVx1_ASAP7_75t_L         g458(.A(\TEMP_reg[1]/NET0131 ), .Y(new_n556));
  OAI21xp33_ASAP7_75t_L     g459(.A1(\MAR_reg[0]/NET0131 ), .A2(new_n466), .B(new_n311), .Y(new_n557));
  NOR4xp25_ASAP7_75t_L      g460(.A(new_n557), .B(new_n556), .C(new_n376), .D(new_n464), .Y(new_n558));
  A2O1A1Ixp33_ASAP7_75t_L   g461(.A1(new_n334), .A2(new_n337), .B(new_n310), .C(\TEMP_reg[0]/NET0131 ), .Y(new_n559));
  AOI211xp5_ASAP7_75t_L     g462(.A1(new_n318), .A2(\MAR_reg[0]/NET0131 ), .B(new_n312), .C(new_n559), .Y(new_n560));
  NOR2xp33_ASAP7_75t_L      g463(.A(new_n558), .B(new_n560), .Y(new_n561));
  OAI31xp33_ASAP7_75t_L     g464(.A1(new_n557), .A2(new_n376), .A3(new_n464), .B(new_n556), .Y(new_n562));
  NOR3xp33_ASAP7_75t_L      g465(.A(new_n404), .B(new_n494), .C(new_n406), .Y(new_n563));
  OAI21xp33_ASAP7_75t_L     g466(.A1(new_n563), .A2(new_n562), .B(new_n495), .Y(new_n564));
  NOR2xp33_ASAP7_75t_L      g467(.A(new_n539), .B(new_n514), .Y(new_n565));
  A2O1A1Ixp33_ASAP7_75t_L   g468(.A1(new_n561), .A2(new_n499), .B(new_n564), .C(new_n565), .Y(new_n566));
  NAND3xp33_ASAP7_75t_L     g469(.A(new_n566), .B(new_n555), .C(new_n506), .Y(new_n567));
  INVx1_ASAP7_75t_L         g470(.A(new_n506), .Y(new_n568));
  NOR3xp33_ASAP7_75t_L      g471(.A(new_n560), .B(new_n558), .C(new_n563), .Y(new_n569));
  NOR3xp33_ASAP7_75t_L      g472(.A(new_n569), .B(new_n565), .C(new_n564), .Y(new_n570));
  AOI31xp33_ASAP7_75t_L     g473(.A1(new_n552), .A2(new_n551), .A3(new_n495), .B(new_n554), .Y(new_n571));
  OAI21xp33_ASAP7_75t_L     g474(.A1(new_n571), .A2(new_n570), .B(new_n568), .Y(new_n572));
  NAND2xp33_ASAP7_75t_L     g475(.A(new_n567), .B(new_n572), .Y(new_n573));
  OA22x2_ASAP7_75t_L        g476(.A1(new_n528), .A2(new_n530), .B1(new_n520), .B2(new_n518), .Y(new_n574));
  NOR2xp33_ASAP7_75t_L      g477(.A(\TEMP_reg[6]/NET0131 ), .B(new_n360), .Y(new_n575));
  NAND3xp33_ASAP7_75t_L     g478(.A(new_n536), .B(\TEMP_reg[7]/NET0131 ), .C(new_n346), .Y(new_n576));
  OAI21xp33_ASAP7_75t_L     g479(.A1(\TEMP_reg[7]/NET0131 ), .A2(new_n341), .B(new_n576), .Y(new_n577));
  NAND3xp33_ASAP7_75t_L     g480(.A(new_n560), .B(new_n562), .C(new_n488), .Y(new_n578));
  OAI21xp33_ASAP7_75t_L     g481(.A1(new_n558), .A2(new_n492), .B(new_n501), .Y(new_n579));
  AO21x2_ASAP7_75t_L        g482(.A1(new_n578), .A2(new_n579), .B(new_n506), .Y(new_n580));
  NAND3xp33_ASAP7_75t_L     g483(.A(new_n578), .B(new_n579), .C(new_n506), .Y(new_n581));
  NAND2xp33_ASAP7_75t_L     g484(.A(new_n581), .B(new_n580), .Y(new_n582));
  AOI21xp33_ASAP7_75t_L     g485(.A1(new_n574), .A2(new_n584), .B(new_n582), .Y(new_n583));
  NOR2xp33_ASAP7_75t_L      g486(.A(new_n577), .B(new_n575), .Y(new_n584));
  NAND4xp25_ASAP7_75t_L     g487(.A(new_n547), .B(new_n538), .C(new_n583), .D(new_n573), .Y(new_n585));
  OAI21xp33_ASAP7_75t_L     g488(.A1(new_n558), .A2(new_n560), .B(new_n562), .Y(new_n586));
  NAND2xp33_ASAP7_75t_L     g489(.A(new_n527), .B(new_n529), .Y(new_n587));
  A2O1A1Ixp33_ASAP7_75t_L   g490(.A1(new_n410), .A2(new_n553), .B(\TEMP_reg[3]/NET0131 ), .C(new_n587), .Y(new_n588));
  INVx1_ASAP7_75t_L         g491(.A(new_n516), .Y(new_n589));
  NAND2xp33_ASAP7_75t_L     g492(.A(new_n515), .B(new_n589), .Y(new_n590));
  NOR2xp33_ASAP7_75t_L      g493(.A(new_n563), .B(new_n539), .Y(new_n591));
  A2O1A1Ixp33_ASAP7_75t_L   g494(.A1(new_n586), .A2(new_n591), .B(new_n588), .C(new_n590), .Y(new_n592));
  A2O1A1Ixp33_ASAP7_75t_L   g495(.A1(new_n488), .A2(new_n501), .B(new_n492), .C(new_n591), .Y(new_n593));
  NAND4xp25_ASAP7_75t_L     g496(.A(new_n593), .B(new_n515), .C(new_n589), .D(new_n540), .Y(new_n594));
  NAND3xp33_ASAP7_75t_L     g497(.A(new_n594), .B(new_n592), .C(new_n568), .Y(new_n595));
  AO21x2_ASAP7_75t_L        g498(.A1(new_n514), .A2(new_n515), .B(new_n516), .Y(new_n596));
  AOI21xp33_ASAP7_75t_L     g499(.A1(new_n436), .A2(\TEMP_reg[4]/NET0131 ), .B(new_n539), .Y(new_n597));
  O2A1O1Ixp33_ASAP7_75t_L   g500(.A1(new_n564), .A2(new_n569), .B(new_n597), .C(new_n596), .Y(new_n598));
  NAND2xp33_ASAP7_75t_L     g501(.A(new_n373), .B(new_n430), .Y(new_n599));
  OAI211xp5_ASAP7_75t_L     g502(.A1(\MAR_reg[3]/NET0131 ), .A2(new_n310), .B(new_n307), .C(new_n313), .Y(new_n600));
  NAND4xp25_ASAP7_75t_L     g503(.A(new_n600), .B(new_n356), .C(new_n375), .D(new_n431), .Y(new_n601));
  NOR4xp25_ASAP7_75t_L      g504(.A(new_n601), .B(new_n509), .C(new_n378), .D(new_n599), .Y(new_n602));
  O2A1O1Ixp33_ASAP7_75t_L   g505(.A1(new_n429), .A2(new_n432), .B(new_n509), .C(new_n602), .Y(new_n603));
  XNOR2x2_ASAP7_75t_L       g506(.A(new_n598), .B(new_n605), .Y(new_n604));
  XOR2x2_ASAP7_75t_L        g507(.A(new_n603), .B(new_n506), .Y(new_n605));
  A2O1A1O1Ixp25_ASAP7_75t_L g508(.A1(new_n499), .A2(new_n586), .B(new_n529), .C(new_n527), .D(new_n514), .Y(new_n606));
  NAND2xp33_ASAP7_75t_L     g509(.A(new_n576), .B(new_n523), .Y(new_n607));
  NOR2xp33_ASAP7_75t_L      g510(.A(new_n520), .B(new_n535), .Y(new_n608));
  NOR3xp33_ASAP7_75t_L      g511(.A(new_n534), .B(new_n533), .C(new_n338), .Y(new_n609));
  O2A1O1Ixp33_ASAP7_75t_L   g512(.A1(\TEMP_reg[7]/NET0131 ), .A2(new_n341), .B(new_n531), .C(new_n609), .Y(new_n610));
  A2O1A1O1Ixp25_ASAP7_75t_L g513(.A1(new_n513), .A2(new_n516), .B(new_n517), .C(new_n608), .D(new_n610), .Y(new_n611));
  OAI21xp33_ASAP7_75t_L     g514(.A1(new_n441), .A2(\TEMP_reg[8]/NET0131 ), .B(new_n613), .Y(new_n612));
  NAND2xp33_ASAP7_75t_L     g515(.A(\TEMP_reg[8]/NET0131 ), .B(new_n441), .Y(new_n613));
  OAI311xp33_ASAP7_75t_L    g516(.A1(new_n606), .A2(new_n541), .A3(new_n607), .B1(new_n611), .C1(new_n612), .Y(new_n614));
  A2O1A1Ixp33_ASAP7_75t_L   g517(.A1(\MAR_reg[0]/NET0131 ), .A2(new_n318), .B(new_n491), .C(new_n488), .Y(new_n615));
  NAND2xp33_ASAP7_75t_L     g518(.A(new_n499), .B(new_n527), .Y(new_n616));
  A2O1A1Ixp33_ASAP7_75t_L   g519(.A1(new_n615), .A2(new_n562), .B(new_n616), .C(new_n540), .Y(new_n617));
  NOR2xp33_ASAP7_75t_L      g520(.A(new_n541), .B(new_n607), .Y(new_n618));
  O2A1O1Ixp33_ASAP7_75t_L   g521(.A1(new_n338), .A2(new_n534), .B(new_n533), .C(new_n575), .Y(new_n619));
  OAI22xp33_ASAP7_75t_L     g522(.A1(new_n619), .A2(new_n609), .B1(new_n544), .B2(new_n607), .Y(new_n620));
  OA21x2_ASAP7_75t_L        g523(.A1(new_n441), .A2(\TEMP_reg[8]/NET0131 ), .B(new_n622), .Y(new_n621));
  NAND2xp33_ASAP7_75t_L     g524(.A(\TEMP_reg[8]/NET0131 ), .B(new_n441), .Y(new_n622));
  A2O1A1Ixp33_ASAP7_75t_L   g525(.A1(new_n617), .A2(new_n618), .B(new_n620), .C(new_n621), .Y(new_n623));
  NAND2xp33_ASAP7_75t_L     g526(.A(new_n623), .B(new_n614), .Y(new_n624));
  OAI311xp33_ASAP7_75t_L    g527(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n486), .Y(new_n625));
  AO21x2_ASAP7_75t_L        g528(.A1(new_n532), .A2(new_n577), .B(new_n507), .Y(new_n626));
  INVx1_ASAP7_75t_L         g529(.A(\TEMP_reg[4]/NET0131 ), .Y(new_n627));
  OAI21xp33_ASAP7_75t_L     g530(.A1(\MAR_reg[1]/NET0131 ), .A2(new_n401), .B(new_n335), .Y(new_n628));
  O2A1O1Ixp33_ASAP7_75t_L   g531(.A1(new_n307), .A2(new_n368), .B(new_n335), .C(new_n310), .Y(new_n629));
  O2A1O1Ixp33_ASAP7_75t_L   g532(.A1(new_n365), .A2(new_n628), .B(new_n306), .C(new_n629), .Y(new_n630));
  AOI211xp5_ASAP7_75t_L     g533(.A1(new_n334), .A2(new_n337), .B(new_n306), .C(new_n310), .Y(new_n631));
  AOI21xp33_ASAP7_75t_L     g534(.A1(new_n633), .A2(new_n630), .B(new_n602), .Y(new_n632));
  NOR2xp33_ASAP7_75t_L      g535(.A(new_n631), .B(new_n627), .Y(new_n633));
  A2O1A1Ixp33_ASAP7_75t_L   g536(.A1(new_n529), .A2(new_n527), .B(new_n514), .C(new_n632), .Y(new_n634));
  NAND4xp25_ASAP7_75t_L     g537(.A(new_n586), .B(new_n632), .C(new_n499), .D(new_n527), .Y(new_n635));
  NAND2xp33_ASAP7_75t_L     g538(.A(new_n531), .B(new_n523), .Y(new_n636));
  AOI31xp33_ASAP7_75t_L     g539(.A1(new_n635), .A2(new_n634), .A3(new_n544), .B(new_n636), .Y(new_n637));
  AND4x1_ASAP7_75t_L        g540(.A(new_n634), .B(new_n635), .C(new_n544), .D(new_n636), .Y(new_n638));
  NOR3xp33_ASAP7_75t_L      g541(.A(new_n570), .B(new_n571), .C(new_n568), .Y(new_n639));
  AOI21xp33_ASAP7_75t_L     g542(.A1(new_n566), .A2(new_n555), .B(new_n506), .Y(new_n640));
  OAI22xp33_ASAP7_75t_L     g543(.A1(new_n638), .A2(new_n637), .B1(new_n639), .B2(new_n640), .Y(new_n641));
  OAI211xp5_ASAP7_75t_L     g544(.A1(new_n577), .A2(new_n532), .B(new_n580), .C(new_n581), .Y(new_n642));
  NOR4xp25_ASAP7_75t_L      g545(.A(new_n641), .B(new_n626), .C(new_n642), .D(new_n595), .Y(new_n643));
  AOI21xp33_ASAP7_75t_L     g546(.A1(new_n643), .A2(new_n645), .B(new_n646), .Y(new_n644));
  NOR2xp33_ASAP7_75t_L      g547(.A(new_n604), .B(new_n101), .Y(new_n645));
  NOR2xp33_ASAP7_75t_L      g548(.A(new_n101), .B(new_n624), .Y(new_n646));
  NAND2xp33_ASAP7_75t_L     g549(.A(\STATO_reg[0]/NET0131 ), .B(new_n324), .Y(new_n647));
  A2O1A1Ixp33_ASAP7_75t_L   g550(.A1(new_n644), .A2(new_n625), .B(new_n647), .C(new_n330), .Y(\g2955/_2_ ));
  NOR2xp33_ASAP7_75t_L      g551(.A(new_n325), .B(new_n467), .Y(new_n649));
  O2A1O1Ixp33_ASAP7_75t_L   g552(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[1]/NET0131 ), .C(new_n649), .Y(new_n650));
  OAI21xp33_ASAP7_75t_L     g553(.A1(new_n478), .A2(new_n444), .B(new_n399), .Y(new_n651));
  NAND3xp33_ASAP7_75t_L     g554(.A(new_n484), .B(new_n481), .C(\MAX_reg[1]/NET0131 ), .Y(new_n652));
  NAND2xp33_ASAP7_75t_L     g555(.A(new_n652), .B(new_n651), .Y(new_n653));
  OAI311xp33_ASAP7_75t_L    g556(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n653), .Y(new_n654));
  AOI21xp33_ASAP7_75t_L     g557(.A1(new_n643), .A2(new_n656), .B(new_n657), .Y(new_n655));
  NOR2xp33_ASAP7_75t_L      g558(.A(new_n604), .B(new_n102), .Y(new_n656));
  NOR2xp33_ASAP7_75t_L      g559(.A(new_n102), .B(new_n624), .Y(new_n657));
  A2O1A1Ixp33_ASAP7_75t_L   g560(.A1(new_n655), .A2(new_n654), .B(new_n647), .C(new_n650), .Y(\g2956/_2_ ));
  INVx1_ASAP7_75t_L         g561(.A(new_n404), .Y(new_n659));
  INVx1_ASAP7_75t_L         g562(.A(new_n422), .Y(new_n660));
  O2A1O1Ixp33_ASAP7_75t_L   g563(.A1(new_n306), .A2(new_n660), .B(new_n659), .C(new_n325), .Y(new_n661));
  O2A1O1Ixp33_ASAP7_75t_L   g564(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[2]/NET0131 ), .C(new_n661), .Y(new_n662));
  INVx1_ASAP7_75t_L         g565(.A(new_n471), .Y(new_n663));
  OAI21xp33_ASAP7_75t_L     g566(.A1(new_n478), .A2(new_n444), .B(new_n663), .Y(new_n664));
  NAND3xp33_ASAP7_75t_L     g567(.A(new_n484), .B(new_n481), .C(\MAX_reg[2]/NET0131 ), .Y(new_n665));
  NAND2xp33_ASAP7_75t_L     g568(.A(new_n665), .B(new_n664), .Y(new_n666));
  OAI311xp33_ASAP7_75t_L    g569(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n666), .Y(new_n667));
  AOI21xp33_ASAP7_75t_L     g570(.A1(new_n643), .A2(new_n669), .B(new_n670), .Y(new_n668));
  NOR2xp33_ASAP7_75t_L      g571(.A(new_n604), .B(new_n99), .Y(new_n669));
  NOR2xp33_ASAP7_75t_L      g572(.A(new_n99), .B(new_n624), .Y(new_n670));
  A2O1A1Ixp33_ASAP7_75t_L   g573(.A1(new_n668), .A2(new_n667), .B(new_n647), .C(new_n662), .Y(\g2957/_2_ ));
  INVx1_ASAP7_75t_L         g574(.A(new_n415), .Y(new_n672));
  NOR2xp33_ASAP7_75t_L      g575(.A(new_n325), .B(new_n672), .Y(new_n673));
  O2A1O1Ixp33_ASAP7_75t_L   g576(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[3]/NET0131 ), .C(new_n673), .Y(new_n674));
  OAI21xp33_ASAP7_75t_L     g577(.A1(new_n478), .A2(new_n444), .B(new_n415), .Y(new_n675));
  NAND3xp33_ASAP7_75t_L     g578(.A(new_n484), .B(new_n481), .C(\MAX_reg[3]/NET0131 ), .Y(new_n676));
  NAND2xp33_ASAP7_75t_L     g579(.A(new_n676), .B(new_n675), .Y(new_n677));
  OAI311xp33_ASAP7_75t_L    g580(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n677), .Y(new_n678));
  AOI21xp33_ASAP7_75t_L     g581(.A1(new_n643), .A2(new_n680), .B(new_n681), .Y(new_n679));
  NOR2xp33_ASAP7_75t_L      g582(.A(new_n604), .B(new_n111), .Y(new_n680));
  NOR2xp33_ASAP7_75t_L      g583(.A(new_n111), .B(new_n624), .Y(new_n681));
  A2O1A1Ixp33_ASAP7_75t_L   g584(.A1(new_n679), .A2(new_n678), .B(new_n647), .C(new_n674), .Y(\g2958/_2_ ));
  O2A1O1Ixp33_ASAP7_75t_L   g585(.A1(new_n306), .A2(new_n392), .B(new_n630), .C(new_n325), .Y(new_n683));
  O2A1O1Ixp33_ASAP7_75t_L   g586(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[4]/NET0131 ), .C(new_n683), .Y(new_n684));
  OAI21xp33_ASAP7_75t_L     g587(.A1(new_n478), .A2(new_n444), .B(new_n372), .Y(new_n685));
  NAND3xp33_ASAP7_75t_L     g588(.A(new_n484), .B(new_n481), .C(\MAX_reg[4]/NET0131 ), .Y(new_n686));
  NAND2xp33_ASAP7_75t_L     g589(.A(new_n686), .B(new_n685), .Y(new_n687));
  OAI311xp33_ASAP7_75t_L    g590(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n687), .Y(new_n688));
  AOI21xp33_ASAP7_75t_L     g591(.A1(new_n643), .A2(new_n690), .B(new_n691), .Y(new_n689));
  NOR2xp33_ASAP7_75t_L      g592(.A(new_n604), .B(new_n96), .Y(new_n690));
  NOR2xp33_ASAP7_75t_L      g593(.A(new_n96), .B(new_n624), .Y(new_n691));
  A2O1A1Ixp33_ASAP7_75t_L   g594(.A1(new_n689), .A2(new_n688), .B(new_n647), .C(new_n684), .Y(\g2959/_2_ ));
  NOR2xp33_ASAP7_75t_L      g595(.A(new_n432), .B(new_n429), .Y(new_n693));
  NOR2xp33_ASAP7_75t_L      g596(.A(new_n325), .B(new_n693), .Y(new_n694));
  O2A1O1Ixp33_ASAP7_75t_L   g597(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[5]/NET0131 ), .C(new_n694), .Y(new_n695));
  INVx1_ASAP7_75t_L         g598(.A(new_n693), .Y(new_n696));
  OAI21xp33_ASAP7_75t_L     g599(.A1(new_n478), .A2(new_n444), .B(new_n696), .Y(new_n697));
  NAND3xp33_ASAP7_75t_L     g600(.A(new_n484), .B(new_n481), .C(\MAX_reg[5]/NET0131 ), .Y(new_n698));
  NAND2xp33_ASAP7_75t_L     g601(.A(new_n698), .B(new_n697), .Y(new_n699));
  OAI311xp33_ASAP7_75t_L    g602(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n699), .Y(new_n700));
  AOI21xp33_ASAP7_75t_L     g603(.A1(new_n643), .A2(new_n702), .B(new_n703), .Y(new_n701));
  NOR2xp33_ASAP7_75t_L      g604(.A(new_n604), .B(new_n427), .Y(new_n702));
  NOR2xp33_ASAP7_75t_L      g605(.A(new_n427), .B(new_n624), .Y(new_n703));
  A2O1A1Ixp33_ASAP7_75t_L   g606(.A1(new_n701), .A2(new_n700), .B(new_n647), .C(new_n695), .Y(\g2960/_2_ ));
  NOR2xp33_ASAP7_75t_L      g607(.A(new_n325), .B(new_n360), .Y(new_n705));
  O2A1O1Ixp33_ASAP7_75t_L   g608(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[6]/NET0131 ), .C(new_n705), .Y(new_n706));
  OAI21xp33_ASAP7_75t_L     g609(.A1(new_n478), .A2(new_n444), .B(new_n452), .Y(new_n707));
  NAND3xp33_ASAP7_75t_L     g610(.A(new_n484), .B(new_n481), .C(\MAX_reg[6]/NET0131 ), .Y(new_n708));
  NAND2xp33_ASAP7_75t_L     g611(.A(new_n708), .B(new_n707), .Y(new_n709));
  OAI311xp33_ASAP7_75t_L    g612(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n709), .Y(new_n710));
  AOI21xp33_ASAP7_75t_L     g613(.A1(new_n643), .A2(new_n712), .B(new_n713), .Y(new_n711));
  NOR2xp33_ASAP7_75t_L      g614(.A(new_n604), .B(new_n445), .Y(new_n712));
  NOR2xp33_ASAP7_75t_L      g615(.A(new_n445), .B(new_n624), .Y(new_n713));
  A2O1A1Ixp33_ASAP7_75t_L   g616(.A1(new_n711), .A2(new_n710), .B(new_n647), .C(new_n706), .Y(\g2961/_2_ ));
  A2O1A1O1Ixp25_ASAP7_75t_L g617(.A1(new_n337), .A2(new_n430), .B(\MAR_reg[0]/NET0131 ), .C(new_n536), .D(new_n325), .Y(new_n715));
  O2A1O1Ixp33_ASAP7_75t_L   g618(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[7]/NET0131 ), .C(new_n715), .Y(new_n716));
  OAI21xp33_ASAP7_75t_L     g619(.A1(new_n478), .A2(new_n444), .B(new_n349), .Y(new_n717));
  NAND3xp33_ASAP7_75t_L     g620(.A(new_n484), .B(new_n481), .C(\MAX_reg[7]/NET0131 ), .Y(new_n718));
  NAND2xp33_ASAP7_75t_L     g621(.A(new_n718), .B(new_n717), .Y(new_n719));
  OAI311xp33_ASAP7_75t_L    g622(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n719), .Y(new_n720));
  AOI21xp33_ASAP7_75t_L     g623(.A1(new_n643), .A2(new_n722), .B(new_n723), .Y(new_n721));
  NOR2xp33_ASAP7_75t_L      g624(.A(new_n604), .B(new_n343), .Y(new_n722));
  NOR2xp33_ASAP7_75t_L      g625(.A(new_n343), .B(new_n624), .Y(new_n723));
  A2O1A1Ixp33_ASAP7_75t_L   g626(.A1(new_n721), .A2(new_n720), .B(new_n647), .C(new_n716), .Y(\g2962/_2_ ));
  NOR2xp33_ASAP7_75t_L      g627(.A(new_n325), .B(new_n441), .Y(new_n725));
  O2A1O1Ixp33_ASAP7_75t_L   g628(.A1(new_n328), .A2(new_n329), .B(\MAX_reg[8]/NET0131 ), .C(new_n725), .Y(new_n726));
  AOI21xp33_ASAP7_75t_L     g629(.A1(new_n483), .A2(new_n437), .B(new_n387), .Y(new_n727));
  OAI21xp33_ASAP7_75t_L     g630(.A1(new_n727), .A2(new_n441), .B(new_n729), .Y(new_n728));
  NAND2xp33_ASAP7_75t_L     g631(.A(\MAX_reg[8]/NET0131 ), .B(new_n727), .Y(new_n729));
  OAI311xp33_ASAP7_75t_L    g632(.A1(new_n585), .A2(new_n595), .A3(new_n604), .B1(new_n624), .C1(new_n728), .Y(new_n730));
  AOI21xp33_ASAP7_75t_L     g633(.A1(new_n643), .A2(new_n732), .B(new_n733), .Y(new_n731));
  NOR2xp33_ASAP7_75t_L      g634(.A(new_n604), .B(new_n97), .Y(new_n732));
  NOR2xp33_ASAP7_75t_L      g635(.A(new_n97), .B(new_n624), .Y(new_n733));
  A2O1A1Ixp33_ASAP7_75t_L   g636(.A1(new_n731), .A2(new_n730), .B(new_n647), .C(new_n726), .Y(\g2963/_2_ ));
  INVx1_ASAP7_75t_L         g637(.A(\FLAG_reg/NET0131 ), .Y(new_n735));
  INVx1_ASAP7_75t_L         g638(.A(START_pad), .Y(new_n736));
  A2O1A1Ixp33_ASAP7_75t_L   g639(.A1(new_n736), .A2(new_n327), .B(new_n322), .C(new_n323), .Y(new_n737));
  INVx1_ASAP7_75t_L         g640(.A(new_n647), .Y(new_n738));
  AO21x2_ASAP7_75t_L        g641(.A1(new_n740), .A2(new_n643), .B(new_n741), .Y(new_n739));
  NOR2xp33_ASAP7_75t_L      g642(.A(new_n604), .B(\FLAG_reg/NET0131 ), .Y(new_n740));
  NAND2xp33_ASAP7_75t_L     g643(.A(new_n738), .B(new_n624), .Y(new_n741));
  A2O1A1Ixp33_ASAP7_75t_L   g644(.A1(new_n325), .A2(new_n737), .B(new_n735), .C(new_n739), .Y(\g2985/_0_ ));
  NAND2xp33_ASAP7_75t_L     g645(.A(new_n323), .B(new_n322), .Y(new_n743));
  INVx1_ASAP7_75t_L         g646(.A(new_n325), .Y(new_n744));
  INVx1_ASAP7_75t_L         g647(.A(new_n329), .Y(new_n745));
  NOR2xp33_ASAP7_75t_L      g648(.A(new_n322), .B(new_n745), .Y(new_n746));
  INVx1_ASAP7_75t_L         g649(.A(new_n746), .Y(new_n747));
  NOR2xp33_ASAP7_75t_L      g650(.A(START_pad), .B(new_n747), .Y(new_n748));
  NOR2xp33_ASAP7_75t_L      g651(.A(new_n744), .B(new_n748), .Y(new_n749));
  AOI31xp33_ASAP7_75t_L     g652(.A1(new_n614), .A2(new_n623), .A3(\FLAG_reg/NET0131 ), .B(new_n259), .Y(new_n750));
  OAI21xp33_ASAP7_75t_L     g653(.A1(\NUM_reg[1]/NET0131 ), .A2(\NUM_reg[0]/NET0131 ), .B(new_n752), .Y(new_n751));
  NAND2xp33_ASAP7_75t_L     g654(.A(\NUM_reg[0]/NET0131 ), .B(\NUM_reg[1]/NET0131 ), .Y(new_n752));
  NOR2xp33_ASAP7_75t_L      g655(.A(new_n751), .B(new_n624), .Y(new_n753));
  A2O1A1Ixp33_ASAP7_75t_L   g656(.A1(new_n753), .A2(\FLAG_reg/NET0131 ), .B(new_n750), .C(new_n738), .Y(new_n754));
  A2O1A1Ixp33_ASAP7_75t_L   g657(.A1(new_n743), .A2(new_n749), .B(new_n259), .C(new_n754), .Y(\g3032/_0_ ));
  AOI31xp33_ASAP7_75t_L     g658(.A1(new_n614), .A2(new_n623), .A3(\FLAG_reg/NET0131 ), .B(new_n260), .Y(new_n756));
  NAND2xp33_ASAP7_75t_L     g659(.A(\NUM_reg[1]/NET0131 ), .B(\NUM_reg[0]/NET0131 ), .Y(new_n757));
  OAI21xp33_ASAP7_75t_L     g660(.A1(new_n757), .A2(new_n260), .B(new_n759), .Y(new_n758));
  NAND2xp33_ASAP7_75t_L     g661(.A(new_n260), .B(new_n757), .Y(new_n759));
  NOR2xp33_ASAP7_75t_L      g662(.A(new_n758), .B(new_n624), .Y(new_n760));
  A2O1A1Ixp33_ASAP7_75t_L   g663(.A1(new_n760), .A2(\FLAG_reg/NET0131 ), .B(new_n756), .C(new_n738), .Y(new_n761));
  A2O1A1Ixp33_ASAP7_75t_L   g664(.A1(new_n743), .A2(new_n749), .B(new_n260), .C(new_n761), .Y(\g3033/_0_ ));
  AOI31xp33_ASAP7_75t_L     g665(.A1(new_n614), .A2(new_n623), .A3(\FLAG_reg/NET0131 ), .B(new_n298), .Y(new_n763));
  NOR3xp33_ASAP7_75t_L      g666(.A(new_n757), .B(new_n260), .C(new_n253), .Y(new_n764));
  OAI21xp33_ASAP7_75t_L     g667(.A1(new_n764), .A2(\NUM_reg[4]/NET0131 ), .B(new_n766), .Y(new_n765));
  NAND2xp33_ASAP7_75t_L     g668(.A(\NUM_reg[4]/NET0131 ), .B(new_n764), .Y(new_n766));
  NOR2xp33_ASAP7_75t_L      g669(.A(new_n765), .B(new_n624), .Y(new_n767));
  A2O1A1Ixp33_ASAP7_75t_L   g670(.A1(new_n767), .A2(\FLAG_reg/NET0131 ), .B(new_n763), .C(new_n738), .Y(new_n768));
  A2O1A1Ixp33_ASAP7_75t_L   g671(.A1(new_n743), .A2(new_n749), .B(new_n298), .C(new_n768), .Y(\g3034/_0_ ));
  A2O1A1Ixp33_ASAP7_75t_L   g672(.A1(new_n322), .A2(new_n329), .B(new_n328), .C(\NUM_reg[3]/NET0131 ), .Y(new_n770));
  INVx1_ASAP7_75t_L         g673(.A(new_n624), .Y(new_n771));
  NOR2xp33_ASAP7_75t_L      g674(.A(new_n260), .B(new_n757), .Y(new_n772));
  NOR2xp33_ASAP7_75t_L      g675(.A(new_n735), .B(new_n774), .Y(new_n773));
  OAI21xp33_ASAP7_75t_L     g676(.A1(new_n772), .A2(\NUM_reg[3]/NET0131 ), .B(new_n775), .Y(new_n774));
  NAND2xp33_ASAP7_75t_L     g677(.A(\NUM_reg[3]/NET0131 ), .B(new_n772), .Y(new_n775));
  A2O1A1Ixp33_ASAP7_75t_L   g678(.A1(new_n735), .A2(\NUM_reg[3]/NET0131 ), .B(new_n773), .C(new_n771), .Y(new_n776));
  OAI31xp33_ASAP7_75t_L     g679(.A1(new_n624), .A2(new_n744), .A3(new_n748), .B(\NUM_reg[3]/NET0131 ), .Y(new_n777));
  A2O1A1O1Ixp25_ASAP7_75t_L g680(.A1(new_n736), .A2(new_n746), .B(new_n744), .C(\NUM_reg[3]/NET0131 ), .D(new_n738), .Y(new_n778));
  A2O1A1Ixp33_ASAP7_75t_L   g681(.A1(new_n776), .A2(new_n777), .B(new_n778), .C(new_n770), .Y(\g3038/_0_ ));
  NOR2xp33_ASAP7_75t_L      g682(.A(\STATO_reg[0]/NET0131 ), .B(new_n745), .Y(new_n780));
  INVx1_ASAP7_75t_L         g683(.A(new_n780), .Y(new_n781));
  INVx1_ASAP7_75t_L         g684(.A(new_n328), .Y(new_n782));
  NOR2xp33_ASAP7_75t_L      g685(.A(new_n307), .B(new_n402), .Y(new_n783));
  INVx1_ASAP7_75t_L         g686(.A(new_n783), .Y(new_n784));
  O2A1O1Ixp33_ASAP7_75t_L   g687(.A1(new_n308), .A2(new_n784), .B(new_n313), .C(new_n782), .Y(new_n785));
  A2O1A1O1Ixp25_ASAP7_75t_L g688(.A1(new_n736), .A2(new_n746), .B(new_n744), .C(\MAR_reg[4]/NET0131 ), .D(new_n785), .Y(new_n786));
  A2O1A1Ixp33_ASAP7_75t_L   g689(.A1(new_n647), .A2(new_n781), .B(new_n313), .C(new_n786), .Y(\g3247/_0_ ));
  INVx1_ASAP7_75t_L         g690(.A(new_n743), .Y(new_n788));
  NOR2xp33_ASAP7_75t_L      g691(.A(new_n647), .B(new_n693), .Y(new_n789));
  O2A1O1Ixp33_ASAP7_75t_L   g692(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[5]/NET0131 ), .C(new_n789), .Y(new_n790));
  OAI21xp33_ASAP7_75t_L     g693(.A1(new_n325), .A2(new_n693), .B(new_n790), .Y(\g3279/_0_ ));
  NOR2xp33_ASAP7_75t_L      g694(.A(new_n368), .B(new_n784), .Y(new_n792));
  NAND2xp33_ASAP7_75t_L     g695(.A(\MAR_reg[3]/NET0131 ), .B(new_n792), .Y(new_n793));
  NAND2xp33_ASAP7_75t_L     g696(.A(new_n308), .B(new_n783), .Y(new_n794));
  NAND2xp33_ASAP7_75t_L     g697(.A(\MAR_reg[3]/NET0131 ), .B(new_n784), .Y(new_n795));
  A2O1A1O1Ixp25_ASAP7_75t_L g698(.A1(new_n794), .A2(new_n795), .B(new_n792), .C(new_n793), .D(new_n782), .Y(new_n796));
  O2A1O1Ixp33_ASAP7_75t_L   g699(.A1(new_n744), .A2(new_n748), .B(\MAR_reg[3]/NET0131 ), .C(new_n796), .Y(new_n797));
  A2O1A1Ixp33_ASAP7_75t_L   g700(.A1(new_n647), .A2(new_n781), .B(new_n308), .C(new_n797), .Y(\g3309/_0_ ));
  O2A1O1Ixp33_ASAP7_75t_L   g701(.A1(new_n306), .A2(new_n321), .B(new_n316), .C(new_n647), .Y(new_n799));
  O2A1O1Ixp33_ASAP7_75t_L   g702(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[0]/NET0131 ), .C(new_n799), .Y(new_n800));
  A2O1A1Ixp33_ASAP7_75t_L   g703(.A1(new_n316), .A2(new_n333), .B(new_n325), .C(new_n800), .Y(\g3335/_0_ ));
  NOR2xp33_ASAP7_75t_L      g704(.A(new_n647), .B(new_n441), .Y(new_n802));
  O2A1O1Ixp33_ASAP7_75t_L   g705(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[8]/NET0131 ), .C(new_n802), .Y(new_n803));
  OAI21xp33_ASAP7_75t_L     g706(.A1(new_n325), .A2(new_n441), .B(new_n803), .Y(\g3336/_0_ ));
  NOR2xp33_ASAP7_75t_L      g707(.A(new_n376), .B(new_n557), .Y(new_n805));
  NOR2xp33_ASAP7_75t_L      g708(.A(new_n647), .B(new_n467), .Y(new_n806));
  O2A1O1Ixp33_ASAP7_75t_L   g709(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[1]/NET0131 ), .C(new_n806), .Y(new_n807));
  A2O1A1Ixp33_ASAP7_75t_L   g710(.A1(new_n395), .A2(new_n805), .B(new_n325), .C(new_n807), .Y(\g3337/_0_ ));
  O2A1O1Ixp33_ASAP7_75t_L   g711(.A1(new_n306), .A2(new_n660), .B(new_n659), .C(new_n647), .Y(new_n809));
  O2A1O1Ixp33_ASAP7_75t_L   g712(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[2]/NET0131 ), .C(new_n809), .Y(new_n810));
  A2O1A1Ixp33_ASAP7_75t_L   g713(.A1(new_n659), .A2(new_n498), .B(new_n325), .C(new_n810), .Y(\g3338/_0_ ));
  NAND2xp33_ASAP7_75t_L     g714(.A(\TEMP_reg[3]/NET0131 ), .B(new_n746), .Y(new_n812));
  A2O1A1Ixp33_ASAP7_75t_L   g715(.A1(new_n553), .A2(new_n410), .B(new_n647), .C(new_n812), .Y(new_n813));
  O2A1O1Ixp33_ASAP7_75t_L   g716(.A1(new_n328), .A2(new_n780), .B(\TEMP_reg[3]/NET0131 ), .C(new_n813), .Y(new_n814));
  A2O1A1Ixp33_ASAP7_75t_L   g717(.A1(new_n410), .A2(new_n553), .B(new_n325), .C(new_n814), .Y(\g3339/_0_ ));
  O2A1O1Ixp33_ASAP7_75t_L   g718(.A1(new_n306), .A2(new_n392), .B(new_n630), .C(new_n647), .Y(new_n816));
  O2A1O1Ixp33_ASAP7_75t_L   g719(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[4]/NET0131 ), .C(new_n816), .Y(new_n817));
  A2O1A1Ixp33_ASAP7_75t_L   g720(.A1(new_n630), .A2(new_n371), .B(new_n325), .C(new_n817), .Y(\g3340/_0_ ));
  A2O1A1O1Ixp25_ASAP7_75t_L g721(.A1(new_n307), .A2(\MAR_reg[3]/NET0131 ), .B(new_n521), .C(\MAR_reg[0]/NET0131 ), .D(new_n357), .Y(new_n819));
  NOR2xp33_ASAP7_75t_L      g722(.A(new_n647), .B(new_n360), .Y(new_n820));
  O2A1O1Ixp33_ASAP7_75t_L   g723(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[6]/NET0131 ), .C(new_n820), .Y(new_n821));
  A2O1A1Ixp33_ASAP7_75t_L   g724(.A1(new_n819), .A2(new_n405), .B(new_n325), .C(new_n821), .Y(\g3341/_0_ ));
  O2A1O1Ixp33_ASAP7_75t_L   g725(.A1(new_n736), .A2(new_n322), .B(new_n329), .C(new_n738), .Y(new_n823));
  OAI21xp33_ASAP7_75t_L     g726(.A1(new_n310), .A2(\MAR_reg[0]/NET0131 ), .B(new_n825), .Y(new_n824));
  NAND2xp33_ASAP7_75t_L     g727(.A(\MAR_reg[0]/NET0131 ), .B(new_n310), .Y(new_n825));
  A2O1A1Ixp33_ASAP7_75t_L   g728(.A1(new_n369), .A2(new_n783), .B(new_n824), .C(new_n328), .Y(new_n826));
  A2O1A1Ixp33_ASAP7_75t_L   g729(.A1(new_n823), .A2(new_n325), .B(new_n310), .C(new_n826), .Y(\g3360/_0_ ));
  INVx1_ASAP7_75t_L         g730(.A(\EN_DISP_reg/NET0131 ), .Y(new_n828));
  INVx1_ASAP7_75t_L         g731(.A(new_n792), .Y(new_n829));
  A2O1A1Ixp33_ASAP7_75t_L   g732(.A1(new_n829), .A2(new_n328), .B(new_n324), .C(\EN_DISP_reg/NET0131 ), .Y(new_n830));
  A2O1A1Ixp33_ASAP7_75t_L   g733(.A1(new_n828), .A2(new_n736), .B(new_n747), .C(new_n830), .Y(\g3361/_0_ ));
  OAI21xp33_ASAP7_75t_L     g734(.A1(new_n402), .A2(new_n307), .B(new_n833), .Y(new_n832));
  NAND2xp33_ASAP7_75t_L     g735(.A(new_n307), .B(new_n402), .Y(new_n833));
  NOR2xp33_ASAP7_75t_L      g736(.A(new_n832), .B(new_n792), .Y(new_n834));
  O2A1O1Ixp33_ASAP7_75t_L   g737(.A1(START_pad), .A2(new_n747), .B(new_n325), .C(new_n307), .Y(new_n835));
  A2O1A1O1Ixp25_ASAP7_75t_L g738(.A1(\MAR_reg[2]/NET0131 ), .A2(new_n792), .B(new_n834), .C(new_n328), .D(new_n835), .Y(new_n836));
  A2O1A1Ixp33_ASAP7_75t_L   g739(.A1(new_n647), .A2(new_n781), .B(new_n307), .C(new_n836), .Y(\g3369/_0_ ));
  O2A1O1Ixp33_ASAP7_75t_L   g740(.A1(new_n368), .A2(new_n784), .B(\MAR_reg[0]/NET0131 ), .C(new_n782), .Y(new_n838));
  A2O1A1O1Ixp25_ASAP7_75t_L g741(.A1(new_n736), .A2(new_n746), .B(new_n744), .C(\MAR_reg[0]/NET0131 ), .D(new_n838), .Y(new_n839));
  A2O1A1Ixp33_ASAP7_75t_L   g742(.A1(new_n647), .A2(new_n781), .B(new_n306), .C(new_n839), .Y(\g3373/_0_ ));
  A2O1A1O1Ixp25_ASAP7_75t_L g743(.A1(new_n337), .A2(new_n430), .B(\MAR_reg[0]/NET0131 ), .C(new_n536), .D(new_n647), .Y(new_n841));
  O2A1O1Ixp33_ASAP7_75t_L   g744(.A1(new_n746), .A2(new_n788), .B(\TEMP_reg[7]/NET0131 ), .C(new_n841), .Y(new_n842));
  A2O1A1Ixp33_ASAP7_75t_L   g745(.A1(new_n346), .A2(new_n536), .B(new_n325), .C(new_n842), .Y(\g3382/_0_ ));
  OAI31xp33_ASAP7_75t_L     g746(.A1(new_n829), .A2(new_n736), .A3(new_n782), .B(new_n647), .Y(\g3451/_0_ ));
  A2O1A1Ixp33_ASAP7_75t_L   g747(.A1(new_n829), .A2(new_n323), .B(new_n327), .C(new_n322), .Y(new_n845));
  A2O1A1Ixp33_ASAP7_75t_L   g748(.A1(new_n782), .A2(new_n747), .B(START_pad), .C(new_n845), .Y(\g3475/_0_ ));
  A2O1A1Ixp33_ASAP7_75t_L   g749(.A1(\STATO_reg[1]/NET0131 ), .A2(new_n327), .B(new_n328), .C(\RES_DISP_reg/NET0131 ), .Y(new_n847));
  A2O1A1Ixp33_ASAP7_75t_L   g750(.A1(new_n131), .A2(new_n736), .B(new_n747), .C(new_n847), .Y(\g3490/_0_ ));
  OAI221xp5_ASAP7_75t_L     g751(.A1(new_n736), .A2(new_n747), .B1(new_n782), .B2(new_n792), .C(new_n325), .Y(\g3514/_0_ ));
  OAI21xp33_ASAP7_75t_L     g752(.A1(new_n273), .A2(\FLAG_reg/NET0131 ), .B(new_n851), .Y(new_n850));
  NAND2xp33_ASAP7_75t_L     g753(.A(\FLAG_reg/NET0131 ), .B(new_n273), .Y(new_n851));
  NAND2xp33_ASAP7_75t_L     g754(.A(new_n850), .B(new_n771), .Y(new_n852));
  NAND2xp33_ASAP7_75t_L     g755(.A(\NUM_reg[0]/NET0131 ), .B(new_n624), .Y(new_n853));
  OAI31xp33_ASAP7_75t_L     g756(.A1(new_n748), .A2(new_n744), .A3(new_n788), .B(\NUM_reg[0]/NET0131 ), .Y(new_n854));
  A2O1A1Ixp33_ASAP7_75t_L   g757(.A1(new_n852), .A2(new_n853), .B(new_n647), .C(new_n854), .Y(\g4511/_0_ ));
  assign                    _al_n0 = 1'b0;
  assign                    _al_n1 = 1'b1;
endmodule


