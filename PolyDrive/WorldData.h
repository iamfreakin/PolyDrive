#pragma once
#include <string>
#include <vector>
#include <map>

// 차량 타입 정의
enum class CarType { BUS, SPORTSCAR, TRUCK, SEDAN, COUNT };

namespace WorldData {
    // 초기 설정값
    const int INITIAL_MONEY = 5000;
    const int INITIAL_ENERGY = 100;

    // 도시 초기화 데이터 (이름, X좌표, Y좌표)
    struct CityInitData {
        std::string name;
        int x, y;
    };

    static std::vector<CityInitData> CITIES = {
        {"Gwangju", 5, 15},
        {"Jeonju", 5, 10},
        {"Daejeon", 10, 8},
        {"Busan", 18, 18},
        {"Seoul", 8, 2},
        {"Gangneung", 18, 3}
    };

    // 경로 초기화 데이터 (출발지 이름, 목적지 이름, 거리, 보상)
    struct RouteInitData {
        std::string from;
        std::string to;
        float distance;
        int reward;
    };

    static std::vector<RouteInitData> ROUTES = {
        {"Gwangju", "Jeonju", 90.0f, 300}, {"Gwangju", "Daejeon", 170.0f, 600}, {"Gwangju", "Busan", 260.0f, 1000}, {"Gwangju", "Seoul", 290.0f, 1200},
        {"Jeonju", "Gwangju", 90.0f, 300}, {"Jeonju", "Daejeon", 80.0f, 250}, {"Jeonju", "Seoul", 210.0f, 800},
        {"Daejeon", "Gwangju", 170.0f, 600}, {"Daejeon", "Jeonju", 80.0f, 250}, {"Daejeon", "Seoul", 160.0f, 550}, {"Daejeon", "Busan", 240.0f, 950},
        {"Busan", "Gwangju", 260.0f, 1000}, {"Busan", "Daejeon", 240.0f, 950}, {"Busan", "Gangneung", 300.0f, 1300},
        {"Seoul", "Gwangju", 290.0f, 1200}, {"Seoul", "Jeonju", 210.0f, 800}, {"Seoul", "Daejeon", 160.0f, 550}, {"Seoul", "Gangneung", 220.0f, 850},
        {"Gangneung", "Seoul", 220.0f, 850}, {"Gangneung", "Busan", 300.0f, 1300}
    };

    // 차량 이름 풀 (각 20개씩)
    static std::map<CarType, std::vector<std::string>> NAME_POOLS = {
        {CarType::BUS, {"Tayo", "Greyhound", "CityFlyer", "Express-1", "Schoolie", "SkyBus", "NightCrawler", "UrbanLink", "MetaBus", "EcoShuttle", "RapidTrans", "BlueWhale", "RoadGiant", "StreetKing", "LineMaster", "TourPro", "CommuteX", "GlidePath", "ApexBus", "NovaCruiser"}},
        {CarType::SPORTSCAR, {"Flash", "Bolt", "Ignis", "Vortex", "Apex", "Zenith", "Specter", "Fury", "Venom", "Phantom", "Racer-X", "Sonic", "DriftKing", "TurboS", "Velocity", "WildFire", "Mach-1", "NeonDash", "Stinger", "G-Force"}},
        {CarType::TRUCK, {"Optimus", "Hercules", "Titan", "Mammoth", "IronSide", "RoadWarrior", "Goliath", "Atlas", "Bison", "Tanker", "CargoMaster", "HeavyHaul", "LogiTech", "Rugged-1", "TerraForce", "MountainMover", "SteerMax", "WildBeast", "IronHorse", "Colossus"}},
        {CarType::SEDAN, {"Sonata", "Avante", "K5", "Grandeur", "Malibu", "Camry", "Accord", "Civic", "Comfort", "CityGlide", "UrbanSoft", "DailyDrive", "FamilyFirst", "Elite-S", "SilkRoad", "GentleRide", "Primo", "Grace", "Lumina", "Stellar"}}
    };
}
