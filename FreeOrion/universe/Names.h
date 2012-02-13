// -*- C++ -*-
#ifndef _Names_h_
#define _Names_h_

#include <GG/adobe/name.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>


#define NAMES_SEQ_1                             \
    (Activation)                                \
    (AddedBefore)                               \
    (AddedSince)                                \
    (AddSpecial)                                \
    (AddStarlanes)                              \
    (Adequate)                                  \
    (Affiliation)                               \
    (Age)                                       \
    (Alignment)                                 \
    (AlignmentEffects)                          \
    (All)                                       \
    (AllocatedFood)                             \
    (AllyOf)                                    \
    (And)                                       \
    (AntiFighterDamage)                         \
    (AntiShipDamage)                            \
    (AnyEmpire)                                 \
    (Armed)                                     \
    (Armour)                                    \
    (Application)                               \
    (Asteroids)                                 \
    (Barren)                                    \
    (BattleSpeed)                               \
    (BlackHole)                                 \
    (Blue)                                      \
    (Bomber)                                    \
    (BuildCost)                                 \
    (Building)                                  \
    (BuildingType)                              \
    (BuildTime)                                 \
    (CanColonize)                               \
    (CanProduceShips)                           \
    (Capacity)                                  \
    (Capital)                                   \
    (Capture)                                   \
    (CaptureResult)                             \
    (Category)                                  \
    (Class)                                     \
    (Colony)                                    \
    (Colour)                                    \
    (Condition)                                 \
    (Construction)                              \
    (ContainedBy)                               \
    (Contains)                                  \
    (Count)                                     \
    (CreateBuilding)                            \
    (CreatedOnTurn)                             \
    (CreatePlanet)                              \
    (CreateShip)                                \
    (CreationTurn)                              \
    (CurrentTurn)                               \
    (Damage)                                    \
    (Data)                                      \
    (Defense)                                   \
    (Description)                               \
    (Desert)                                    \
    (Design)                                    \
    (DesignHasHull)                             \
    (DesignHasPart)                             \
    (DesignHasPartClass)                        \
    (DesignID)                                  \
    (DesignName)                                \
    (Destination)                               \
    (Destroy)                                   \
    (Detection)                                 \
    (Distance)                                  \
    (DistanceToSource)                          \
    (Effects)                                   \
    (EffectsGroup)                              \
    (EffectsGroups)                             \
    (Empire)                                    \
    (Endpoint)                                  \
    (EnemyOf)                                   \
    (Environment)                               \
    (Environments)                              \
    (ExploredByEmpire)                          \
    (External)                                  \
    (Farming)                                   \
    (Fighters)                                  \
    (FighterType)                               \
    (FighterWeaponRange)                        \
    (FinalDestinationID)                        \
    (Fleet)                                     \
    (FleetID)                                   \
    (FleetSupplyableByEmpire)                   \
    (Foci)                                      \
    (Focus)                                     \
    (FocusType)                                 \
    (FoodAllocationForMaxGrowth)                \
    (FoodConsumption)                           \
    (FoodStockpile)                             \
    (Fuel)                                      \
    (GasGiant)                                  \
    (GenerateSitrepMessage)                     \
    (GiveEmpireTech)                            \
    (Good)                                      \
    (Graphic)                                   \
    (HasSpecial)                                \
    (HasSpecialSinceTurn)                       \
    (Health)                                    \
    (High)                                      \
    (Homeworld)                                 \
    (Hostile)                                   \
    (Huge)                                      \
    (Hull)                                      \
    (Icon)                                      \
    (ID)                                        \
    (Industry)                                  \
    (Inferno)                                   \
    (InSystem)                                  \
    (Interceptor)                               \
    (Internal)                                  \
    (Item)                                      \
    (Jumps)                                     \
    (Label)                                     \
    (Large)                                     \
    (LastTurnBattleHere)                        \
    (LaunchRate)                                \
    (LocalCandidate)                            \
    (Location)                                  \
    (Low)                                       \
    (Max)                                       \
    (MaxDefense)                                \
    (MaxFuel)                                   \
    (MaximumNumberOf)                           \
    (MaxShield)                                 \
    (MaxStructure)                              \
    (MaxTroops)                                 \
    (Mean)                                      \
    (Medium)                                    \
    (Message)                                   \
    (Meter)                                     \
    (Min)                                       \
    (MineralStockpile)                          \
    (MinimumNumberOf)                           \
    (Mining)                                    \
    (Missiles)                                  \
    (Mode)                                      \
    (Model)                                     \
    (ModeNumberOf)                              \
    (Monster)                                   \
    (MonsterFleet)                              \
    (MountableSlotTypes)                        \
    (MoveTo)                                    \
    (Name)                                      \
    (Neutron)                                   \
    (NextBetterPlanetType)                      \
    (ClockwiseNextPlanetType)                   \
    (CounterClockwiseNextPlanetType)            \
    (NextLargerPlanetSize)                      \
    (NextSmallerPlanetSize)                     \
    (NextOlderStarType)                         \
    (NextYoungerStarType)                       \
    (NextSystemID)                              \
    (NoStar)                                    \
    (Not)                                       \
    (Number)                                    \
    (NumberOf)                                  \
    (NumShips)                                  \
    (Object)                                    \
    (ObjectType)                                \
    (Ocean)                                     \
    (Or)                                        \
    (Orange)                                    \
    (OwnedBy)                                   \
    (Owner)                                     \
    (OwnerFoodStockpile)                        \
    (OwnerHasTech)                              \
    (OwnerMineralStockpile)                     \
    (OwnerTradeStockpile)                       \
    (Parameters)                                \
    (Part)                                      \
    (PartClass)

#define NAMES_SEQ_2                             \
    (PartName)                                  \
    (Parts)                                     \
    (Planet)                                    \
    (Planetbound)                               \
    (PlanetEnvironment)                         \
    (PlanetID)                                  \
    (PlanetSize)                                \
    (PlanetType)                                \
    (Playable)                                  \
    (PointDefense)                              \
    (Poor)                                      \
    (Population)                                \
    (PopulationCenter)                          \
    (Position)                                  \
    (Prerequisites)                             \
    (PreviousSystemID)                          \
    (Probability)                               \
    (ProducedByEmpire)                          \
    (ProducedByEmpireID)                        \
    (Producible)                                \
    (Product)                                   \
    (ProductionCenter)                          \
    (Progress)                                  \
    (Property)                                  \
    (Radiated)                                  \
    (Random)                                    \
    (Range)                                     \
    (Reason)                                    \
    (Red)                                       \
    (Refinement)                                \
    (RemoveSpecial)                             \
    (RemoveStarlanes)                           \
    (Research)                                  \
    (Researchable)                              \
    (ResearchCost)                              \
    (ResearchTurns)                             \
    (ResourceSupplyConnected)                   \
    (ResupplyableBy)                            \
    (Retain)                                    \
    (RMS)                                       \
    (ROF)                                       \
    (RootCandidate)                             \
    (Scope)                                     \
    (SetAntiFighterDamage)                      \
    (SetAntiShipDamage)                         \
    (SetBattleSpeed)                            \
    (SetCapacity)                               \
    (SetConstruction)                           \
    (SetDamage)                                 \
    (SetDefense)                                \
    (SetDestination)                            \
    (SetDetection)                              \
    (SetEmpireCapital)                          \
    (SetEmpireFoodStockpile)                    \
    (SetEmpireMeter)                            \
    (SetEmpireMineralStockpile)                 \
    (SetEmpireTechProgress)                     \
    (SetEmpireTradeStockpile)                   \
    (SetFarming)                                \
    (SetFighterWeaponRange)                     \
    (SetFoodConsumption)                        \
    (SetFuel)                                   \
    (SetHealth)                                 \
    (SetIndustry)                               \
    (SetLaunchRate)                             \
    (SetMaxDefense)                             \
    (SetMaxFuel)                                \
    (SetMaxShield)                              \
    (SetMaxStructure)                           \
    (SetMaxTroops)                              \
    (SetMining)                                 \
    (SetOwner)                                  \
    (SetPlanetSize)                             \
    (SetPlanetType)                             \
    (SetPopulation)                             \
    (SetRange)                                  \
    (SetResearch)                               \
    (SetROF)                                    \
    (SetShield)                                 \
    (SetSpecies)                                \
    (SetSpeed)                                  \
    (SetStarlaneSpeed)                          \
    (SetStarType)                               \
    (SetStealth)                                \
    (SetStructure)                              \
    (SetSupply)                                 \
    (SetTargetConstruction)                     \
    (SetTargetFarming)                          \
    (SetTargetHealth)                           \
    (SetTargetIndustry)                         \
    (SetTargetMining)                           \
    (SetTargetPopulation)                       \
    (SetTargetResearch)                         \
    (SetTargetTrade)                            \
    (SetTrade)                                  \
    (SetTroops)                                 \
    (Shield)                                    \
    (ShipDesign)                                \
    (Ship)                                      \
    (ShipHull)                                  \
    (ShipPart)                                  \
    (Ships)                                     \
    (Short_Description)                         \
    (ShortRange)                                \
    (Size)                                      \
    (Slot)                                      \
    (Slots)                                     \
    (SlotType)                                  \
    (Small)                                     \
    (SortBy)                                    \
    (SortKey)                                   \
    (Source)                                    \
    (Spacebound)                                \
    (SpawnLimit)                                \
    (SpawnRate)                                 \
    (Special)                                   \
    (Species)                                   \
    (Speed)                                     \
    (Spread)                                    \
    (StackingGroup)                             \
    (Star)                                      \
    (StarlaneSpeed)                             \
    (StarType)                                  \
    (Stationary)                                \
    (StDev)                                     \
    (Stealth)                                   \
    (Structure)                                 \
    (Sum)                                       \
    (Supply)                                    \
    (Swamp)                                     \
    (System)                                    \
    (SystemID)                                  \
    (Tag)                                       \
    (Target)                                    \
    (TargetConstruction)                        \
    (TargetFarming)                             \
    (TargetHealth)                              \
    (TargetIndustry)                            \
    (TargetMining)                              \
    (TargetPopulation)                          \
    (TargetResearch)                            \
    (TargetTrade)                               \
    (Tech)                                      \
    (TechType)                                  \
    (Terran)                                    \
    (TheEmpire)                                 \
    (Theory)                                    \
    (Tiny)                                      \
    (Toxic)                                     \
    (Trade)                                     \
    (TradeStockpile)                            \
    (Troops)                                    \
    (Tundra)                                    \
    (Turn)                                      \
    (Type)                                      \
    (Uninhabitable)                             \
    (Unlock)                                    \
    (Unproducible)                              \
    (Unresearchable)                            \
    (Value)                                     \
    (Victory)                                   \
    (VisibleToEmpire)                           \
    (White)                                     \
    (WithinDistance)                            \
    (WithinStarlaneJumps)                       \
    (Yellow)

#define DECLARE_NAME(r, _, elem) extern adobe::aggregate_name_t BOOST_PP_CAT(elem, _name);
BOOST_PP_SEQ_FOR_EACH(DECLARE_NAME, _, NAMES_SEQ_1)
BOOST_PP_SEQ_FOR_EACH(DECLARE_NAME, _, NAMES_SEQ_2)
#undef DECLARE_NAME

#endif
