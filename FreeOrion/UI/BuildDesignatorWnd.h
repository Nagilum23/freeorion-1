// -*- C++ -*-
#ifndef _BuildDesignatorWnd_h_
#define _BuildDesignatorWnd_h_

#include "../universe/Enums.h"

#include <GG/Wnd.h>


class SidePanel;

class BuildDesignatorWnd : public GG::Wnd
{
public:
    /** \name Signal Types */ //@{
    typedef boost::signal<void (BuildType, const std::string&, int, int)> AddBuildToQueueSignalType; ///< emitted when the indicated build is indicated by the user
    typedef boost::signal<void (int, int)> BuildQuantityChangedSignalType; ///< emitted when the quantity of items in a single build queue item is changed by the user
    typedef boost::signal<void (int)> SystemSelectedSignalType; ///< emitted when system selection is required.
    //@}

    /** \name Slot Types */ //@{
    typedef AddBuildToQueueSignalType::slot_type AddBuildToQueueSlotType; ///< type of functor(s) invoked on a BuildPickedSignalType
    typedef BuildQuantityChangedSignalType::slot_type BuildQuantityChangedSlotType; ///< type of functor(s) invoked on a BuildQuantityChangedSignalType
    //@}

    /** \name Structors */ //@{
    BuildDesignatorWnd(int w, int h);
    ~BuildDesignatorWnd();
    //@}

    /** \name Accessors */ //@{
    const std::set<BuildType>& GetBuildTypesShown() const;
    const std::pair<bool, bool>& GetAvailabilitiesShown() const; // .first -> available items; .second -> unavailable items

    virtual bool InWindow(const GG::Pt& pt) const;
    virtual bool InClient(const GG::Pt& pt) const;

    GG::Rect MapViewHole() const;
    int QueueIndexShown() const;
    //@}

    /** \name Mutators */ //@{
    void CenterOnBuild(int queue_idx);
    void SelectSystem(int system);
    void SelectPlanet(int planet);
    void Reset();
    void Clear();

    void ShowType(BuildType type);
    void ShowAllTypes();
    void HideType(BuildType type);
    void HideAllTypes();
    void ToggleType(BuildType type);
    void ToggleAllTypes();
    
    void ShowAvailability(bool available);
    void HideAvailability(bool available);
    void ToggleAvailabilitly(bool available);
    //@}

    mutable AddBuildToQueueSignalType AddBuildToQueueSignal;
    mutable BuildQuantityChangedSignalType BuildQuantityChangedSignal;
    mutable SystemSelectedSignalType SystemSelectedSignal;

private:
    class BuildDetailPanel;
    class BuildSelector;

    void BuildItemRequested(BuildType build_type, const std::string& item, int num_to_build);
    void BuildQuantityChanged(int queue_idx, int quantity);
    void SelectDefaultPlanet(int system);

    BuildDetailPanel*  m_build_detail_panel;
    BuildSelector*     m_build_selector;
    SidePanel*         m_side_panel;
    int                m_build_location;
    GG::Rect           m_map_view_hole;
    std::map<int, int> m_system_default_planets;

    std::set<boost::signals::connection> m_misc_connections;
};

#endif // _BuildDesignatorWnd_h_
