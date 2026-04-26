#pragma once
#include <string>
#include <vector>
#include <map>

// 도시 이동 경로 정보
struct Route {
    std::string destination;
    float distance;
    int baseReward;
};

// 차량 타입 정의
enum class CarType { BUS, SPORTSCAR, TRUCK, SEDAN, COUNT };

namespace WorldData {
    // 초기 설정값
    const int INITIAL_MONEY = 5000;
    const int INITIAL_ENERGY = 100;

    // 도시별 이동 가능 경로 데이터
    static std::map<std::string, std::vector<Route>> CITY_ROUTES = {
        {"Gwangju", {{"Jeonju", 90.0f, 300}, {"Daejeon", 170.0f, 600}, {"Busan", 260.0f, 1000}, {"Seoul", 290.0f, 1200}}},
        {"Jeonju", {{"Gwangju", 90.0f, 300}, {"Daejeon", 80.0f, 250}, {"Seoul", 210.0f, 800}}},
        {"Daejeon", {{"Gwangju", 170.0f, 600}, {"Jeonju", 80.0f, 250}, {"Seoul", 160.0f, 550}, {"Busan", 240.0f, 950}}},
        {"Busan", {{"Gwangju", 260.0f, 1000}, {"Daejeon", 240.0f, 950}, {"Gangneung", 300.0f, 1300}}},
        {"Seoul", {{"Gwangju", 290.0f, 1200}, {"Jeonju", 210.0f, 800}, {"Daejeon", 160.0f, 550}, {"Gangneung", 220.0f, 850}}},
        {"Gangneung", {{"Seoul", 220.0f, 850}, {"Busan", 300.0f, 1300}}}
    };

    // 차량 이름 풀 (각 20개씩)
    static std::map<CarType, std::vector<std::string>> NAME_POOLS = {
        {CarType::BUS, {"Tayo", "Greyhound", "CityFlyer", "Express-1", "Schoolie", "SkyBus", "NightCrawler", "UrbanLink", "MetaBus", "EcoShuttle", "RapidTrans", "BlueWhale", "RoadGiant", "StreetKing", "LineMaster", "TourPro", "CommuteX", "GlidePath", "ApexBus", "NovaCruiser"}},
        {CarType::SPORTSCAR, {"Flash", "Bolt", "Ignis", "Vortex", "Apex", "Zenith", "Specter", "Fury", "Venom", "Phantom", "Racer-X", "Sonic", "DriftKing", "TurboS", "Velocity", "WildFire", "Mach-1", "NeonDash", "Stinger", "G-Force"}},
        {CarType::TRUCK, {"Optimus", "Hercules", "Titan", "Mammoth", "IronSide", "RoadWarrior", "Goliath", "Atlas", "Bison", "Tanker", "CargoMaster", "HeavyHaul", "LogiTech", "Rugged-1", "TerraForce", "MountainMover", "SteerMax", "WildBeast", "IronHorse", "Colossus"}},
        {CarType::SEDAN, {"Sonata", "Avante", "K5", "Grandeur", "Malibu", "Camry", "Accord", "Civic", "Comfort", "CityGlide", "UrbanSoft", "DailyDrive", "FamilyFirst", "Elite-S", "SilkRoad", "GentleRide", "Primo", "Grace", "Lumina", "Stellar"}}
    };
}
