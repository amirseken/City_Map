#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
#include "courier_verify.h"

using ece297test::relative_error;
using ece297test::courier_path_is_legal;


SUITE(extreme_newyork_public) {
    TEST(extreme_newyork) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        std::vector<unsigned> result_path;
        bool is_legal;
        
        deliveries = {DeliveryInfo(61841, 117003), DeliveryInfo(35544, 80546), DeliveryInfo(103555, 134237), DeliveryInfo(130455, 96271), DeliveryInfo(88315, 25975), DeliveryInfo(92635, 71177), DeliveryInfo(57515, 92503), DeliveryInfo(17114, 116939), DeliveryInfo(35401, 29202), DeliveryInfo(5690, 132041), DeliveryInfo(133620, 9961), DeliveryInfo(9700, 118066), DeliveryInfo(124179, 143139), DeliveryInfo(32947, 11824), DeliveryInfo(131147, 70069), DeliveryInfo(105426, 49128), DeliveryInfo(1258, 16990), DeliveryInfo(70549, 27084), DeliveryInfo(60090, 8570), DeliveryInfo(75584, 51317), DeliveryInfo(32450, 49713), DeliveryInfo(129867, 115919), DeliveryInfo(63339, 145664), DeliveryInfo(123223, 67980), DeliveryInfo(24232, 11395), DeliveryInfo(136605, 97522), DeliveryInfo(91570, 94319), DeliveryInfo(55890, 52181), DeliveryInfo(25661, 57608), DeliveryInfo(38068, 120577), DeliveryInfo(53152, 42744), DeliveryInfo(61574, 27454), DeliveryInfo(25093, 134514), DeliveryInfo(75829, 40602), DeliveryInfo(126448, 78542), DeliveryInfo(132959, 52811), DeliveryInfo(59456, 120877), DeliveryInfo(27444, 6617), DeliveryInfo(95389, 114401), DeliveryInfo(48803, 34314), DeliveryInfo(145896, 90755), DeliveryInfo(120076, 45029), DeliveryInfo(105657, 56315), DeliveryInfo(14261, 109483), DeliveryInfo(120535, 50305), DeliveryInfo(110079, 25699), DeliveryInfo(136253, 106846), DeliveryInfo(88321, 25369), DeliveryInfo(64803, 120136), DeliveryInfo(126393, 18069), DeliveryInfo(137126, 54794), DeliveryInfo(130630, 124618), DeliveryInfo(104838, 40897), DeliveryInfo(7271, 43798), DeliveryInfo(28981, 29953), DeliveryInfo(71781, 18745), DeliveryInfo(124022, 137898), DeliveryInfo(12706, 115707), DeliveryInfo(109230, 55696), DeliveryInfo(111572, 34404), DeliveryInfo(97769, 11379), DeliveryInfo(96012, 47326), DeliveryInfo(12656, 92375), DeliveryInfo(131225, 30454), DeliveryInfo(8342, 32005), DeliveryInfo(32919, 103087), DeliveryInfo(11702, 51863), DeliveryInfo(82209, 117368), DeliveryInfo(59670, 7753), DeliveryInfo(129847, 138905), DeliveryInfo(47200, 134988), DeliveryInfo(4711, 8647), DeliveryInfo(33544, 3275), DeliveryInfo(50034, 107673), DeliveryInfo(115484, 76410), DeliveryInfo(34470, 39829), DeliveryInfo(93170, 93463), DeliveryInfo(24244, 136840), DeliveryInfo(124835, 43108), DeliveryInfo(83516, 67874), DeliveryInfo(50440, 44847), DeliveryInfo(36783, 122669), DeliveryInfo(97888, 132242), DeliveryInfo(30439, 16938), DeliveryInfo(95852, 61239), DeliveryInfo(127733, 55474), DeliveryInfo(103054, 100816), DeliveryInfo(67180, 5067), DeliveryInfo(113624, 98930), DeliveryInfo(91152, 41251), DeliveryInfo(6767, 62606), DeliveryInfo(63582, 84585), DeliveryInfo(100440, 130536), DeliveryInfo(43187, 38374), DeliveryInfo(114546, 82139), DeliveryInfo(88417, 107542), DeliveryInfo(68424, 84996), DeliveryInfo(92437, 89242), DeliveryInfo(81409, 28781), DeliveryInfo(92727, 74462), DeliveryInfo(57111, 29857), DeliveryInfo(89590, 47850), DeliveryInfo(8669, 41801), DeliveryInfo(117217, 76087), DeliveryInfo(21462, 93807), DeliveryInfo(119122, 40388), DeliveryInfo(111325, 133346), DeliveryInfo(136421, 125080), DeliveryInfo(82506, 56862), DeliveryInfo(2282, 37584), DeliveryInfo(21240, 95677), DeliveryInfo(71340, 76490), DeliveryInfo(75798, 73156), DeliveryInfo(55638, 43351), DeliveryInfo(79901, 100305), DeliveryInfo(27945, 80741), DeliveryInfo(95008, 106514), DeliveryInfo(122641, 118945), DeliveryInfo(50297, 46398), DeliveryInfo(32566, 135772), DeliveryInfo(137092, 13189), DeliveryInfo(125005, 7722), DeliveryInfo(110775, 136835), DeliveryInfo(46147, 116523), DeliveryInfo(15492, 1106), DeliveryInfo(106237, 72387), DeliveryInfo(35411, 75127), DeliveryInfo(108852, 4738)};
        depots = {143366};
        {
        	ECE297_TIME_CONSTRAINT(30000);
        	
        	result_path = traveling_courier(deliveries, depots);
        }
        
        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);
        
        if(is_legal) {
        	double path_cost = compute_path_travel_time(result_path);
        	std::cout << "QoR extreme_newyork: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_newyork: INVALID" << std::endl;
        }
        
    } //extreme_newyork

    TEST(extreme_multi_newyork) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        std::vector<unsigned> result_path;
        bool is_legal;
        
        deliveries = {DeliveryInfo(74717, 41044), DeliveryInfo(135023, 141288), DeliveryInfo(105095, 54703), DeliveryInfo(63954, 68717), DeliveryInfo(144480, 64957), DeliveryInfo(130760, 102336), DeliveryInfo(110297, 116925), DeliveryInfo(66498, 104502), DeliveryInfo(75502, 37669), DeliveryInfo(118680, 34285), DeliveryInfo(90631, 51499), DeliveryInfo(58354, 116925), DeliveryInfo(118680, 54703), DeliveryInfo(118680, 52683), DeliveryInfo(16146, 112867), DeliveryInfo(74717, 2331), DeliveryInfo(123492, 90042), DeliveryInfo(94657, 116906), DeliveryInfo(125437, 118339), DeliveryInfo(64132, 71957), DeliveryInfo(38950, 9706), DeliveryInfo(134035, 713), DeliveryInfo(42044, 141288), DeliveryInfo(144480, 122966), DeliveryInfo(42786, 143105), DeliveryInfo(74717, 87863), DeliveryInfo(43663, 41870), DeliveryInfo(3553, 81019), DeliveryInfo(131399, 97735), DeliveryInfo(142574, 48508), DeliveryInfo(130393, 3783), DeliveryInfo(142574, 101336), DeliveryInfo(143997, 98843), DeliveryInfo(93994, 47548), DeliveryInfo(133055, 13100), DeliveryInfo(39196, 108403), DeliveryInfo(74717, 143583), DeliveryInfo(102963, 82900), DeliveryInfo(144567, 60030), DeliveryInfo(59980, 112129), DeliveryInfo(96474, 89173), DeliveryInfo(76528, 99740), DeliveryInfo(115236, 136818), DeliveryInfo(138438, 100400), DeliveryInfo(130393, 3783), DeliveryInfo(110349, 1390), DeliveryInfo(48240, 114032), DeliveryInfo(69697, 70765), DeliveryInfo(144480, 40716), DeliveryInfo(97175, 119361), DeliveryInfo(59136, 3335), DeliveryInfo(11666, 10942), DeliveryInfo(79788, 67844), DeliveryInfo(97175, 62074), DeliveryInfo(94657, 11009), DeliveryInfo(85446, 71599), DeliveryInfo(94657, 137771), DeliveryInfo(14545, 118851), DeliveryInfo(15129, 109888), DeliveryInfo(54170, 64957), DeliveryInfo(36431, 90042), DeliveryInfo(19359, 111568), DeliveryInfo(93994, 46905), DeliveryInfo(144480, 27387), DeliveryInfo(74717, 116925), DeliveryInfo(53968, 24818), DeliveryInfo(100043, 64957), DeliveryInfo(143997, 143553), DeliveryInfo(90425, 87155), DeliveryInfo(79527, 110359), DeliveryInfo(130760, 103000), DeliveryInfo(74488, 115480), DeliveryInfo(16492, 127499), DeliveryInfo(82023, 37669), DeliveryInfo(73285, 498), DeliveryInfo(99893, 34285), DeliveryInfo(145422, 121700), DeliveryInfo(107913, 116925), DeliveryInfo(67200, 87155), DeliveryInfo(91088, 49741), DeliveryInfo(4375, 11091), DeliveryInfo(50819, 34297), DeliveryInfo(51125, 28810), DeliveryInfo(60926, 141288), DeliveryInfo(68509, 45395), DeliveryInfo(144480, 102336), DeliveryInfo(54962, 109888), DeliveryInfo(97041, 27379), DeliveryInfo(118680, 10942), DeliveryInfo(25941, 109913), DeliveryInfo(39196, 27379), DeliveryInfo(134457, 141288), DeliveryInfo(126183, 34285), DeliveryInfo(130393, 54703), DeliveryInfo(74717, 9526), DeliveryInfo(60843, 2650), DeliveryInfo(18845, 116925), DeliveryInfo(84869, 34285), DeliveryInfo(130760, 145256), DeliveryInfo(94657, 79547), DeliveryInfo(28986, 40263), DeliveryInfo(145862, 18061), DeliveryInfo(94657, 94560), DeliveryInfo(115243, 141710), DeliveryInfo(43008, 114032), DeliveryInfo(93994, 54703), DeliveryInfo(93994, 102336), DeliveryInfo(74717, 55001), DeliveryInfo(100522, 135648), DeliveryInfo(142574, 41076), DeliveryInfo(133813, 38966), DeliveryInfo(74717, 13436), DeliveryInfo(130393, 106461), DeliveryInfo(38754, 43621), DeliveryInfo(130760, 16028), DeliveryInfo(1989, 10942), DeliveryInfo(7265, 102336), DeliveryInfo(85378, 104675), DeliveryInfo(142574, 122238), DeliveryInfo(74717, 1586), DeliveryInfo(17610, 22150), DeliveryInfo(65482, 34285), DeliveryInfo(99953, 114032), DeliveryInfo(84354, 12649), DeliveryInfo(130760, 3783), DeliveryInfo(144480, 99713), DeliveryInfo(118680, 100338), DeliveryInfo(41525, 43873), DeliveryInfo(134035, 114098), DeliveryInfo(95059, 81693), DeliveryInfo(117703, 109888), DeliveryInfo(23350, 106461), DeliveryInfo(94657, 58681), DeliveryInfo(119783, 112717), DeliveryInfo(8074, 29868), DeliveryInfo(144480, 46539), DeliveryInfo(120937, 49457), DeliveryInfo(47828, 3783), DeliveryInfo(4024, 100454), DeliveryInfo(144480, 109888), DeliveryInfo(94657, 122966), DeliveryInfo(39196, 88387), DeliveryInfo(130760, 12565), DeliveryInfo(93994, 109913), DeliveryInfo(89887, 119897), DeliveryInfo(39196, 43551), DeliveryInfo(122563, 141288), DeliveryInfo(39177, 37669), DeliveryInfo(97068, 123439), DeliveryInfo(143997, 53034), DeliveryInfo(34249, 16091), DeliveryInfo(144480, 50025), DeliveryInfo(117409, 64957), DeliveryInfo(118680, 36588), DeliveryInfo(94657, 10942), DeliveryInfo(94657, 97842), DeliveryInfo(99117, 23615), DeliveryInfo(93994, 106499), DeliveryInfo(144480, 134286), DeliveryInfo(130760, 64321), DeliveryInfo(134035, 106461), DeliveryInfo(67942, 122966), DeliveryInfo(140064, 46539), DeliveryInfo(130393, 55579), DeliveryInfo(74717, 65707), DeliveryInfo(93994, 109913), DeliveryInfo(5531, 19930), DeliveryInfo(66580, 122966), DeliveryInfo(114561, 90042), DeliveryInfo(45543, 13436), DeliveryInfo(77746, 73709), DeliveryInfo(12569, 136502), DeliveryInfo(127403, 109913), DeliveryInfo(142574, 137425), DeliveryInfo(52634, 70429), DeliveryInfo(107262, 27148), DeliveryInfo(20694, 13436), DeliveryInfo(118680, 102336), DeliveryInfo(60702, 13417), DeliveryInfo(125655, 87155), DeliveryInfo(41857, 64321), DeliveryInfo(59136, 109888), DeliveryInfo(142574, 27379), DeliveryInfo(130760, 77708), DeliveryInfo(39196, 109913), DeliveryInfo(10830, 75678), DeliveryInfo(59136, 41772), DeliveryInfo(57454, 87155), DeliveryInfo(39196, 46554), DeliveryInfo(144286, 45020), DeliveryInfo(39196, 37669), DeliveryInfo(86987, 27379)};
        depots = {36, 136736};
        {
        	ECE297_TIME_CONSTRAINT(30000);
        	
        	result_path = traveling_courier(deliveries, depots);
        }
        
        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);
        
        if(is_legal) {
        	double path_cost = compute_path_travel_time(result_path);
        	std::cout << "QoR extreme_multi_newyork: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_multi_newyork: INVALID" << std::endl;
        }
        
    } //extreme_multi_newyork

} //extreme_newyork_public

