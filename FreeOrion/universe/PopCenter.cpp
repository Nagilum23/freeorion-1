#include "PopCenter.h"

#include "../util/AppInterface.h"
#include "../util/DataTable.h"
#include "../util/MultiplayerCommon.h"
#include "../util/OptionsDB.h"
#include "Planet.h"

#include <algorithm>

#include <boost/lexical_cast.hpp>

#include <stdexcept>

using boost::lexical_cast;

namespace {
    DataTableMap& PlanetDataTables()
    {
        static DataTableMap map;
        if (map.empty()) {
            std::string settings_dir = GetOptionsDB().Get<std::string>("settings-dir");
            if (!settings_dir.empty() && settings_dir[settings_dir.size() - 1] != '/')
                settings_dir += '/';
            LoadDataTables(settings_dir + "planet_tables.txt", map);
        }
        return map;
    }

    double MaxPopModFromObject(const UniverseObject* object)
    {
        if (const Planet* planet = universe_object_cast<const Planet*>(object))
            return PlanetDataTables()["PlanetMaxPop"][planet->Size()][planet->Environment()];
        return 0.0;
    }

    double MaxHealthModFromObject(const UniverseObject* object)
    {
        if (const Planet* planet = universe_object_cast<const Planet*>(object))
            return PlanetDataTables()["PlanetEnvHealthMod"][0][planet->Environment()];
        return 0.0;
    }
}

PopCenter::PopCenter(int race) :
    m_race(race), m_allocated_food(0.0)
{
}

PopCenter::PopCenter() :
    m_race(-1), m_allocated_food(0.0)
{
}
   
PopCenter::~PopCenter()
{
}

void PopCenter::Init(double max_pop_mod, double max_health_mod)
{
    InsertMeter(METER_POPULATION, Meter());
    InsertMeter(METER_HEALTH, Meter());
    Reset(max_pop_mod, max_health_mod);
}

double PopCenter::Inhabitants() const
{
    return GetMeter(METER_POPULATION)->Current();    // TO DO: Something fancy for different races
}

double PopCenter::FuturePopGrowth() const
{
    double max = GetMeter(METER_POPULATION)->Max();
    double cur = GetMeter(METER_POPULATION)->Current();
    Logger().debugStream() << "PopCenter::FuturePopGrowth  growth max: " << FuturePopGrowthMax() << "  allocated food: " << AllocatedFood();

    return std::max(-cur, std::min(FuturePopGrowthMax(), std::min(AllocatedFood(), max) - cur));
}

double PopCenter::FuturePopGrowthMax() const
{
    double max_pop = GetMeter(METER_POPULATION)->Max();
    double cur_pop = GetMeter(METER_POPULATION)->Current();
    double cur_health = GetMeter(METER_HEALTH)->Current();

    if (20.0 < cur_health) {
        return std::min(max_pop - cur_pop, cur_pop * (((max_pop + 1.0) - cur_pop) / (max_pop + 1.0)) * (cur_health - 20.0) * 0.01);
    } else if (cur_health == 20.0) {
        return 0.0;
    } else { // cur_health < 20.0
        return std::max(-cur_pop, -cur_pop*(  exp( (cur_health-20)*(cur_health-20) / (400/log(2.0)) ) - 1  ));
    }
}

double PopCenter::FutureHealthGrowth() const
{
    double max = GetMeter(METER_HEALTH)->Max();
    double cur = GetMeter(METER_HEALTH)->Current();
    return std::min(max - cur, cur * (((max + 1.0) - cur) / (max + 1.0)));
}

double PopCenter::ProjectedCurrentMeter(MeterType type) const
{
    switch (type) {
    case METER_POPULATION:
        return GetMeter(METER_POPULATION)->InitialCurrent() + FuturePopGrowth();
        break;
    case METER_HEALTH:
        return GetMeter(METER_HEALTH)->InitialCurrent() + FutureHealthGrowth();
        break;
    default:
        const UniverseObject* obj = dynamic_cast<const UniverseObject*>(this);
        if (obj)
            return obj->ProjectedCurrentMeter(type);
        else
            throw std::runtime_error("PopCenter::ProjectedCurrentMeter couldn't convert this pointer to UniverseObject*");
    }
}

double PopCenter::MeterPoints(MeterType type) const
{
    switch (type) {
    case METER_POPULATION:
    case METER_HEALTH:
        return GetMeter(type)->InitialCurrent();    // health and population point values are equal to meter values
        break;
    default:
        const UniverseObject* obj = dynamic_cast<const UniverseObject*>(this);
        if (obj)
            return obj->MeterPoints(type);
        else
            throw std::runtime_error("PopCenter::MeterPoints couldn't convert this pointer to UniverseObject*");
    }
}

double PopCenter::ProjectedMeterPoints(MeterType type) const
{
    switch (type) {
    case METER_POPULATION:
    case METER_HEALTH:
        return ProjectedCurrentMeter(type);
        break;
    default:
        const UniverseObject* obj = dynamic_cast<const UniverseObject*>(this);
        if (obj)
            return obj->ProjectedMeterPoints(type);
        else
            throw std::runtime_error("PopCenter::ProjectedMeterPoints couldn't convert this pointer to UniverseObject*");
    }
}

void PopCenter::ApplyUniverseTableMaxMeterAdjustments(MeterType meter_type)
{
    UniverseObject* object = GetObjectSignal();
    assert(object);

    if (meter_type == INVALID_METER_TYPE || meter_type == METER_POPULATION)
        GetMeter(METER_POPULATION)->AdjustMax(MaxPopModFromObject(object));

    if (meter_type == INVALID_METER_TYPE || meter_type == METER_HEALTH)
        GetMeter(METER_HEALTH)->AdjustMax(MaxHealthModFromObject(object));
}

void PopCenter::PopGrowthProductionResearchPhase()
{
    UniverseObject* object = GetObjectSignal();
    assert(object);
    Meter* pop = GetMeter(METER_POPULATION);
    Meter* health = GetMeter(METER_HEALTH);

    double pop_adjustment =         FuturePopGrowth();

    pop->AdjustCurrent(pop_adjustment);

    double health_initial_current = health->InitialCurrent();
    double health_initial_max =     health->InitialMax();


    if (health_initial_current < health_initial_max) {
        double health_adjustment = (((health_initial_max + 1.0) - health_initial_current) / (health_initial_max + 1.0));
        health->AdjustCurrent(health_adjustment);
        Logger().debugStream() << "PopCenter::PopGrowthProductionResearchPhase adjusted current health to: " << health->Current();
    }
}

void PopCenter::Reset(double max_pop_mod, double max_health_mod)
{
    GetMeter(METER_POPULATION)->Set(0.0, max_pop_mod);
    GetMeter(METER_HEALTH)->Set(max_health_mod, max_health_mod);
    m_race = -1;
    m_allocated_food = 0.0;
}
