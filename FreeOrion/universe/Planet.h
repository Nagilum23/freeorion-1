#ifndef _Planet_h_
#define _Planet_h_

#ifndef _UniverseObject_h_
#include "UniverseObject.h"
#endif

#ifndef _PopCenter_h_
#include "PopCenter.h"
#endif

#ifndef _ProdCenter_h_
#include "ProdCenter.h"
#endif


/** a class representing a FreeOrion planet.  A Planet is a type of UniverseObject, and has some UniverseObject decorators
   attached to it as well.  See PopCenter for notes on decorators.*/
class Planet : virtual public UniverseObject, public PopCenter, public ProdCenter
{
public:
   /** the types of planets in FreeOrion*/
   enum PlanetType {TOXIC,
                    RADIATED,
                    BARREN,
                    DESERT,
                    TUNDRA,
                    OCEAN,
                    TERRAN,
                    GAIA,
                    MAX_PLANET_TYPE
                   }; // others TBD (these are from the drek spreadsheet)

   /** the sizes of planets in FreeOrion*/
   enum PlanetSize {SZ_TINY,
                    SZ_SMALL,
                    SZ_MEDIUM,
                    SZ_LARGE,
                    SZ_HUGE
                   }; // others TBD (these are from the drek spreadsheet)

   /** \name Structors */ //@{
   Planet(); ///< default ctor
   Planet(PlanetType type, PlanetSize size); ///< general ctor taking just the planet's type and size
   Planet(const GG::XMLElement& elem); ///< ctor that constructs a Planet object from an XMLElement. \throw std::invalid_argument May throw std::invalid_argument if \a elem does not encode a Planet object
   //@}

   /** \name Accessors */ //@{
   PlanetType  Type() const {return m_type;}
   PlanetSize  Size() const {return m_size;}
   int         SystemID() const {return m_system_id;}
   
   /////////////////////////////////////////////////////////////////////////////
   // V0.1 ONLY!!!!
   int DefBases() const {return m_def_bases;}
   // V0.1 ONLY!!!!
   /////////////////////////////////////////////////////////////////////////////
   
   virtual UniverseObject::Visibility Visible(int empire_id) const; ///< returns the visibility status of this universe object relative to the input empire.
   virtual GG::XMLElement XMLEncode() const; ///< constructs an XMLElement from a Planet object
   virtual GG::XMLElement XMLEncode(int empire_id) const; ///< constructs an XMLElement from a planet object with visibility limited relative to the input empire
   //@}
  	
   /** \name Mutators */ //@{
   virtual void MovementPhase(std::vector<SitRepEntry>& sit_reps);
   virtual void PopGrowthProductionResearchPhase(std::vector<SitRepEntry>& sit_reps);
   void         SetSystemID(int system_id) {m_system_id = system_id;}

   /////////////////////////////////////////////////////////////////////////////
   // V0.1 ONLY!!!!
   void AdjustDefBases(int bases) {m_def_bases += bases; if (m_def_bases < 0) m_def_bases = 0;}
   // V0.1 ONLY!!!!
   /////////////////////////////////////////////////////////////////////////////

   //@}

private:
   PlanetType     m_type;
   PlanetSize     m_size;
   int            m_system_id;
   
   /////////////////////////////////////////////////////////////////////////////
   // V0.1 ONLY!!!!
   int            m_def_bases;
   // V0.1 ONLY!!!!
   /////////////////////////////////////////////////////////////////////////////
};

#endif // _Planet_h_

