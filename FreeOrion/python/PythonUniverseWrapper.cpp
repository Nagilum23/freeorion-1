#include "../universe/Universe.h"
#include "../universe/UniverseObject.h"
#include "../universe/Fleet.h"
#include "../universe/Ship.h"
#include "../universe/ShipDesign.h"
#include "../universe/Building.h"
#include "../universe/ResourceCenter.h"
#include "../universe/PopCenter.h"
#include "../universe/Planet.h"
#include "../universe/System.h"
#include "../universe/Special.h"

#include <boost/python.hpp>

namespace {
    const UniverseObject*   (Universe::*UniverseGetObject)(int) =   &Universe::Object;
    const Fleet*            (Universe::*UniverseGetFleet)(int) =    &Universe::Object;
    const Ship*             (Universe::*UniverseGetShip)(int) =     &Universe::Object;
    const Planet*           (Universe::*UniverseGetPlanet)(int) =   &Universe::Object;
    const System*           (Universe::*UniverseGetSystem)(int) =   &Universe::Object;
    const Building*         (Universe::*UniverseGetBuilding)(int) = &Universe::Object;
}

namespace FreeOrionPython {
    using boost::python::class_;
    using boost::python::bases;
    using boost::python::no_init;
    using boost::noncopyable;
    using boost::python::return_value_policy;
    using boost::python::copy_const_reference;
    using boost::python::reference_existing_object;
    using boost::python::return_by_value;
    using boost::python::return_internal_reference;

    void WrapUniverseClasses() {
        ////////////////////
        //    Universe    //
        ////////////////////
        class_<Universe, noncopyable>("universe", no_init)
            .def("getObject",                   UniverseGetObject,              return_value_policy<reference_existing_object>())
            .def("getFleet",                    UniverseGetFleet,               return_value_policy<reference_existing_object>())
            .def("getShip",                     UniverseGetShip,                return_value_policy<reference_existing_object>())
            .def("getPlanet",                   UniverseGetPlanet,              return_value_policy<reference_existing_object>())
            .def("getSystem",                   UniverseGetSystem,              return_value_policy<reference_existing_object>())
            .def("getBuilding",                 UniverseGetBuilding,            return_value_policy<reference_existing_object>())
            .def("getSpecial",                  GetSpecial,                     return_value_policy<reference_existing_object>())

            .add_property("allObjectIDs",       make_function(&Universe::FindObjectIDs<UniverseObject>, return_value_policy<return_by_value>()))
            .add_property("systemIDs",          make_function(&Universe::FindObjectIDs<System>,         return_value_policy<return_by_value>()))
            .add_property("fleetIDs",           make_function(&Universe::FindObjectIDs<Fleet>,          return_value_policy<return_by_value>()))

            .def("systemHasStarlane",           &Universe::SystemReachable)
            .def("systemsConnected",            &Universe::SystemsConnected)

            // put as part of universe class so one doesn't need a UniverseObject object in python to access these
            .def_readonly("invalidObjectID",    &UniverseObject::INVALID_OBJECT_ID)
            .def_readonly("invalidObjectAge",   &UniverseObject::INVALID_OBJECT_AGE)
        ;

        ////////////////////
        // UniverseObject //
        ////////////////////
        class_<UniverseObject, noncopyable>("universeObject", no_init)
            .add_property("id",                 &UniverseObject::ID)
            .add_property("name",               make_function(&UniverseObject::Name,        return_value_policy<copy_const_reference>()))
            .add_property("x",                  &UniverseObject::X)
            .add_property("y",                  &UniverseObject::Y)
            .add_property("systemID",           &UniverseObject::SystemID)
            .add_property("unowned",            &UniverseObject::Unowned)
            .add_property("owners",             make_function(&UniverseObject::Owners,      return_internal_reference<>()))
            .def("ownedBy",                     &UniverseObject::OwnedBy)
            .def("whollyOwnedBy",               &UniverseObject::WhollyOwnedBy)
            .add_property("creationTurn",       &UniverseObject::CreationTurn)
            .add_property("ageInTurns",         &UniverseObject::AgeInTurns)
            .add_property("specials",           make_function(&UniverseObject::Specials,    return_internal_reference<>()))
        ;

        ///////////////////
        //     Fleet     //
        ///////////////////
        class_<Fleet, bases<UniverseObject>, noncopyable>("fleet", no_init)
            .add_property("finalDestinationID",         &Fleet::FinalDestinationID)
            .add_property("nextSystemID",               &Fleet::NextSystemID)
            .add_property("speed",                      &Fleet::Speed)
            .add_property("canChangeDirectionEnRoute",  &Fleet::CanChangeDirectionEnRoute)
            .add_property("hasArmedShips",              &Fleet::HasArmedShips)
            .add_property("numShips",                   &Fleet::NumShips)
            .def("containsShipID",                      &Fleet::ContainsShip)
            .add_property("shipIDs",                    make_function(&Fleet::ShipIDs,      return_internal_reference<>()))
        ;

        //////////////////
        //     Ship     //
        //////////////////
        class_<Ship, bases<UniverseObject>, noncopyable>("ship", no_init)
            .add_property("design",             make_function(&Ship::Design,        return_value_policy<reference_existing_object>()))
            .add_property("fleetID",            &Ship::FleetID)
            .add_property("getFleet",           make_function(&Ship::GetFleet,      return_value_policy<reference_existing_object>()))
            .add_property("isArmed",            &Ship::IsArmed)
            .add_property("speed",              &Ship::Speed)
        ;

        //////////////////
        //  ShipDesign  //
        //////////////////
        class_<ShipDesign, noncopyable>("shipDesign", no_init)
            .add_property("name",               make_function(&ShipDesign::Name,    return_value_policy<copy_const_reference>()))
        ;

        //////////////////
        //   Building   //
        //////////////////
        class_<Building, bases<UniverseObject>, noncopyable>("building", no_init)
            .def("getBuildingType",             &Building::GetBuildingType,         return_value_policy<reference_existing_object>())
            .add_property("operating",          &Building::Operating)
            .def("getPlanet",                   &Building::GetPlanet,               return_value_policy<reference_existing_object>())
        ;

        //////////////////
        // BuildingType //
        //////////////////
        class_<BuildingType, noncopyable>("buildingType", no_init)
            .add_property("name",               make_function(&BuildingType::Name,          return_value_policy<copy_const_reference>()))
            .add_property("description",        make_function(&BuildingType::Description,   return_value_policy<copy_const_reference>()))
            .add_property("buildCost",          &BuildingType::BuildCost)
            .add_property("buildTime",          &BuildingType::BuildTime)
            .add_property("maintenanceCost",    &BuildingType::MaintenanceCost)
            .def("captureResult",               &BuildingType::GetCaptureResult)
        ;

        ////////////////////
        // ResourceCenter //
        ////////////////////
        class_<ResourceCenter, noncopyable>("resourceCenter", no_init)
            .add_property("primaryFocus",       &ResourceCenter::PrimaryFocus)
            .add_property("secondaryFocus",     &ResourceCenter::SecondaryFocus)
        ;

        ///////////////////
        //   PopCenter   //
        ///////////////////
        class_<PopCenter, noncopyable>("popCenter", no_init)
            .add_property("inhabitants",        &PopCenter::Inhabitants)
            .add_property("availableFood",      &PopCenter::AvailableFood)
        ;

        //////////////////
        //    Planet    //
        //////////////////
        class_<Planet, bases<UniverseObject, PopCenter, ResourceCenter>, noncopyable>("planet", no_init)
            .add_property("size",               &Planet::Size)
            .add_property("type",               &Planet::Type)
            .add_property("buildingIDs",        make_function(&Planet::Buildings,   return_internal_reference<>()))
        ;

        //////////////////
        //    System    //
        //////////////////
        class_<System, bases<UniverseObject>, noncopyable>("system", no_init)
            .add_property("starType",           &System::Star)
            .add_property("numOrbits",          &System::Orbits)
            .add_property("numStarlanes",       &System::Starlanes)
            .add_property("numWormholes",       &System::Wormholes)
            .def("HasStarlaneToSystemID",       &System::HasStarlaneTo)
            .def("HasWormholeToSystemID",       &System::HasWormholeTo)
            .add_property("allObjectIDs",       make_function(&System::FindObjectIDs<UniverseObject>,   return_value_policy<return_by_value>()))
            .add_property("planetIDs",          make_function(&System::FindObjectIDs<Planet>,           return_value_policy<return_by_value>()))
            .add_property("fleetIDs",           make_function(&System::FindObjectIDs<Fleet>,            return_value_policy<return_by_value>()))
        ;

        /////////////////
        //   Special   //
        /////////////////
        class_<Special, noncopyable>("special", no_init)
            .add_property("name",               make_function(&Special::Name,           return_value_policy<copy_const_reference>()))
            .add_property("description",        make_function(&Special::Description,    return_value_policy<copy_const_reference>()))
        ;
    }
}