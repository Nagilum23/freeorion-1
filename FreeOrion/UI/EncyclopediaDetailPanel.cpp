#include "EncyclopediaDetailPanel.h"

#include "CUIControls.h"
#include "../universe/Tech.h"
#include "../universe/ShipDesign.h"
#include "../universe/Building.h"
#include "../universe/Special.h"
#include "../util/MultiplayerCommon.h"

#include <GG/DrawUtil.h>
#include <GG/StaticGraphic.h>
#include <GG/GUI.h>

namespace {
    const GG::X TEXT_MARGIN_X(3);
    const GG::Y TEXT_MARGIN_Y(3);
}

EncyclopediaDetailPanel::EncyclopediaDetailPanel(GG::X w, GG::Y h) :
    CUIWnd("", GG::X1, GG::Y1, w - 1, h - 1, GG::ONTOP | GG::CLICKABLE | GG::DRAGABLE | GG::RESIZABLE),
    m_tech(0),
    m_part(0),
    m_hull(0),
    m_building(0),
    m_design(0),
    m_special(0),
    m_name_text(0),
    m_cost_text(0),
    m_summary_text(0), 
    m_description_box(0),
    m_icon(0),
    m_other_icon(0)
{
    const int PTS = ClientUI::Pts();
    const int NAME_PTS = PTS*3/2;
    const int COST_PTS = PTS;
    const int SUMMARY_PTS = PTS*4/3;

    m_name_text =       new GG::TextControl(GG::X0, GG::Y0, GG::X(10), GG::Y(10), "", ClientUI::GetBoldFont(NAME_PTS),  ClientUI::TextColor());
    m_cost_text =       new GG::TextControl(GG::X0, GG::Y0, GG::X(10), GG::Y(10), "", ClientUI::GetFont(COST_PTS),      ClientUI::TextColor());
    m_summary_text =    new GG::TextControl(GG::X0, GG::Y0, GG::X(10), GG::Y(10), "", ClientUI::GetFont(SUMMARY_PTS),   ClientUI::TextColor());
    m_description_box = new CUILinkTextMultiEdit(GG::X0, GG::Y0, GG::X(10), GG::Y(10), "", GG::MULTI_WORDBREAK | GG::MULTI_READ_ONLY);
    m_description_box->SetColor(GG::CLR_ZERO);
    m_description_box->SetInteriorColor(GG::CLR_ZERO);

    AttachChild(m_name_text);
    AttachChild(m_cost_text);
    AttachChild(m_summary_text);
    AttachChild(m_description_box);

    DoLayout();
}

void EncyclopediaDetailPanel::DoLayout() {
    const int PTS = ClientUI::Pts();
    const int NAME_PTS = PTS*3/2;
    const int COST_PTS = PTS;
    const int SUMMARY_PTS = PTS*4/3;

    const int ICON_SIZE = 12 + NAME_PTS + COST_PTS + SUMMARY_PTS;

    // name
    GG::Pt ul = GG::Pt();
    GG::Pt lr = ul + GG::Pt(Width(), GG::Y(NAME_PTS + 4));
    m_name_text->SizeMove(ul, lr);

    // cost / turns
    ul += GG::Pt(GG::X0, m_name_text->Height());
    lr = ul + GG::Pt(Width(), GG::Y(COST_PTS + 4));
    m_cost_text->SizeMove(ul, lr);

    // one line summary
    ul += GG::Pt(GG::X0, m_cost_text->Height());
    lr = ul + GG::Pt(Width(), GG::Y(SUMMARY_PTS + 4));
    m_summary_text->SizeMove(ul, lr);

    // main verbose description (fluff, effects, unlocks, ...)
    ul = GG::Pt(GG::X1, ICON_SIZE + TEXT_MARGIN_Y + 1);
    lr = ul + GG::Pt(Width() - TEXT_MARGIN_X - BORDER_RIGHT, Height() - BORDER_BOTTOM - ul.y - TEXT_MARGIN_Y);
    m_description_box->SizeMove(ul, lr);

    // icon
    if (m_icon) {
        ul = GG::Pt(GG::X1, GG::Y1);
        lr = ul + GG::Pt(GG::X(ICON_SIZE), GG::Y(ICON_SIZE));
        m_icon->SizeMove(ul, lr);
    }
    // other icon
    if (m_other_icon) {
        lr = GG::Pt(Width() - BORDER_RIGHT, GG::Y(ICON_SIZE + 1));
        ul = lr - GG::Pt(GG::X(ICON_SIZE), GG::Y(ICON_SIZE));
        m_other_icon->SizeMove(ul, lr);
    }
}

void EncyclopediaDetailPanel::SizeMove(const GG::Pt& ul, const GG::Pt& lr) {
    GG::Pt old_size = GG::Wnd::LowerRight() - GG::Wnd::UpperLeft();

    // maybe later do something interesting with docking
    GG::Wnd::SizeMove(ul, lr);

    if (Visible() && old_size != GG::Wnd::Size())
        DoLayout();
}

GG::Pt EncyclopediaDetailPanel::ClientUpperLeft() const {
    return GG::Wnd::UpperLeft();
}

void EncyclopediaDetailPanel::Render() {
    GG::Pt ul = UpperLeft();
    GG::Pt lr = LowerRight();
    const GG::Y ICON_SIZE = m_summary_text->LowerRight().y - m_name_text->UpperLeft().y;
    GG::Pt cl_ul = ul + GG::Pt(BORDER_LEFT, ICON_SIZE + BORDER_BOTTOM);
    GG::Pt cl_lr = lr - GG::Pt(BORDER_RIGHT, BORDER_BOTTOM);

   // use GL to draw the lines
    glDisable(GL_TEXTURE_2D);
    GLint initial_modes[2];
    glGetIntegerv(GL_POLYGON_MODE, initial_modes);

    // draw background
    glPolygonMode(GL_BACK, GL_FILL);
    glBegin(GL_POLYGON);
        glColor(ClientUI::WndColor());
        glVertex(ul.x, ul.y);
        glVertex(lr.x, ul.y);
        glVertex(lr.x, lr.y - OUTER_EDGE_ANGLE_OFFSET);
        glVertex(lr.x - OUTER_EDGE_ANGLE_OFFSET, lr.y);
        glVertex(ul.x, lr.y);
        glVertex(ul.x, ul.y);
    glEnd();

    // draw outer border on pixel inside of the outer edge of the window
    glPolygonMode(GL_BACK, GL_LINE);
    glBegin(GL_POLYGON);
        glColor(ClientUI::WndOuterBorderColor());
        glVertex(ul.x, ul.y);
        glVertex(lr.x, ul.y);
        glVertex(lr.x, lr.y - OUTER_EDGE_ANGLE_OFFSET);
        glVertex(lr.x - OUTER_EDGE_ANGLE_OFFSET, lr.y);
        glVertex(ul.x, lr.y);
        glVertex(ul.x, ul.y);
    glEnd();

    // reset this to whatever it was initially
    glPolygonMode(GL_BACK, initial_modes[1]);

    // draw inner border, including extra resize-tab lines
    glBegin(GL_LINE_STRIP);
        glColor(ClientUI::WndInnerBorderColor());
        glVertex(cl_ul.x, cl_ul.y);
        glVertex(cl_lr.x, cl_ul.y);
        glVertex(cl_lr.x, cl_lr.y - INNER_BORDER_ANGLE_OFFSET);
        glVertex(cl_lr.x - INNER_BORDER_ANGLE_OFFSET, cl_lr.y);
        glVertex(cl_ul.x, cl_lr.y);
        glVertex(cl_ul.x, cl_ul.y);
    glEnd();
    glBegin(GL_LINES);
        // draw the extra lines of the resize tab
        glColor(ClientUI::WndInnerBorderColor());
        glVertex(cl_lr.x, cl_lr.y - RESIZE_HASHMARK1_OFFSET);
        glVertex(cl_lr.x - RESIZE_HASHMARK1_OFFSET, cl_lr.y);
        
        glVertex(cl_lr.x, cl_lr.y - RESIZE_HASHMARK2_OFFSET);
        glVertex(cl_lr.x - RESIZE_HASHMARK2_OFFSET, cl_lr.y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void EncyclopediaDetailPanel::LDrag(const GG::Pt& pt, const GG::Pt& move, GG::Flags<GG::ModKey> mod_keys) {
    if (m_drag_offset != GG::Pt(-GG::X1, -GG::Y1)) {  // resize-dragging
        GG::Pt new_lr = pt - m_drag_offset;

        // constrain to within parent
        if (GG::Wnd* parent = Parent()) {
            GG::Pt max_lr = parent->ClientLowerRight();
            new_lr.x = std::min(new_lr.x, max_lr.x);
            new_lr.y = std::min(new_lr.y, max_lr.y);
        }

        Resize(new_lr - UpperLeft());
    } else {    // normal-dragging
        GG::Pt final_move = move;

        if (GG::Wnd* parent = Parent()) {
            GG::Pt ul = UpperLeft(), lr = LowerRight();
            GG::Pt new_ul = ul + move, new_lr = lr + move;

            GG::Pt min_ul = parent->ClientUpperLeft() + GG::Pt(GG::X1, GG::Y1);
            GG::Pt max_lr = parent->ClientLowerRight();
            GG::Pt max_ul = max_lr - Size();

            new_ul.x = std::max(min_ul.x, std::min(max_ul.x, new_ul.x));
            new_ul.y = std::max(min_ul.y, std::min(max_ul.y, new_ul.y));

            final_move = new_ul - ul;
        }

        GG::Wnd::LDrag(pt, final_move, mod_keys);
    }
}

bool EncyclopediaDetailPanel::NothingSet() {
    return (!m_tech && !m_part &&  !m_hull && !m_building && !m_design && !m_special);
}

void EncyclopediaDetailPanel::Reset() {
    if (m_icon) {
        DeleteChild(m_icon);
        m_icon = 0;
    }
    if (m_other_icon) {
        DeleteChild(m_other_icon);
        m_other_icon = 0;
    }
    if (NothingSet()) {
        m_name_text->SetText("");
        m_summary_text->SetText("");
        m_cost_text->SetText("");
        m_description_box->SetText("");
        return;
    }

    // get details of item as applicable in order to set summary, cost, description TextControls
    std::string name = "";
    boost::shared_ptr<GG::Texture> texture;
    boost::shared_ptr<GG::Texture> other_texture;
    int turns;
    double cost;
    std::string cost_units = "";            // "PP" or "RP" or empty string, depending on whether and what something costs
    std::string general_type = "";          // general type of thing being shown, eg. "Building" or "Ship Part"
    std::string specific_type = "";         // specific type of thing; thing's purpose.  eg. "Farming" or "Colonization".  May be left blank for things without specific types (eg. specials)
    std::string detailed_description = "";
    GG::Clr color = GG::CLR_ZERO;
    
    using boost::io::str;
    if (m_part) {
        // Ship Parts
        name = UserString(m_part->Name());
        texture = ClientUI::PartTexture(m_part->Name());
        turns = m_part->BuildTime();
        cost = m_part->Cost();
        cost_units = UserString("ENC_PP");
        general_type = UserString("ENC_SHIP_PART");
        specific_type = UserString(boost::lexical_cast<std::string>(m_part->Class()));
        
        detailed_description = m_part->Description();
    } else if (m_hull) {
        // Ship Hulls
        name = UserString(m_hull->Name());
        texture = ClientUI::HullTexture(m_hull->Name());
        turns = m_hull->BuildTime();
        cost = m_hull->Cost();
        cost_units = UserString("ENC_PP");
        general_type = UserString("ENC_SHIP_HULL");
        // hulls have no specific types
        
        detailed_description = m_hull->Description();
    } else if (m_tech) {
        // Technologies
        name = UserString(m_tech->Name());
        texture = ClientUI::TechTexture(m_tech->Name());
        other_texture = ClientUI::CategoryIcon(m_tech->Category()); 
        color = ClientUI::CategoryColor(m_tech->Category());
        turns = m_tech->ResearchTurns();
        cost = m_tech->ResearchCost();
        cost_units = UserString("ENC_RP");
        general_type = str(FlexibleFormat(UserString("ENC_TECH_DETAIL_TYPE_STR"))
            % UserString(m_tech->Category())
            % UserString(boost::lexical_cast<std::string>(m_tech->Type()))
            % UserString(m_tech->ShortDescription()));
        
        detailed_description = UserString(m_tech->Description());
        if (!m_tech->Effects().empty()) {
            detailed_description += str(FlexibleFormat(UserString("ENC_EFFECTS_STR")) % EffectsDescription(m_tech->Effects()));
        }
        
        const std::vector<ItemSpec>& unlocked_items = m_tech->UnlockedItems();
        if (!unlocked_items.empty()) {
            detailed_description += UserString("ENC_TECH_DETAIL_UNLOCKS_SECTION_STR");
            for (unsigned int i = 0; i < unlocked_items.size(); ++i) {
                detailed_description += str(FlexibleFormat(UserString("ENC_TECH_DETAIL_UNLOCKED_ITEM_STR"))
                    % UserString(boost::lexical_cast<std::string>(unlocked_items[i].type))
                    % UserString(unlocked_items[i].name));
            }
        }
    } else if (m_building) {
        // Buildings
        name = UserString(m_building->Name());
        texture = ClientUI::BuildingTexture(m_building->Name());
        turns = m_building->BuildTime();
        cost = m_building->BuildCost();
        cost_units = UserString("ENC_PP");
        general_type = UserString("ENC_BUILDING_TYPE");
        
        detailed_description = UserString(m_building->Description());
        if (!m_building->Effects().empty()) {
            detailed_description += str(FlexibleFormat(UserString("ENC_EFFECTS_STR")) % EffectsDescription(m_building->Effects()));
        }
    } else if (m_design) {
        // Ship Designs
        name = m_design->Name();
        texture = ClientUI::ShipIcon(m_design->ID());
        turns = m_design->BuildTime();
        cost = m_design->Cost();
        cost_units = UserString("ENC_PP");
        general_type = UserString("ENC_SHIP_DESIGN");
        detailed_description = str(FlexibleFormat(UserString("ENC_SHIP_DESIGN_DESCRIPTION_STR"))
            % m_design->Description()
            % m_design->Attack()
            % m_design->Defense()
            % m_design->Speed());
    } else if (m_special) {
        // Specials
        name = UserString(m_special->Name());
        texture = ClientUI::SpecialTexture(m_special->Name());
    }

    // Create Icons
    if (texture) {
        m_icon =        new GG::StaticGraphic(GG::X0, GG::Y0, GG::X(10), GG::Y(10), texture,        GG::GRAPHIC_FITGRAPHIC | GG::GRAPHIC_PROPSCALE);
        if (color != GG::CLR_ZERO)
            m_icon->SetColor(color);
    }
    if (other_texture) {
        m_other_icon =  new GG::StaticGraphic(GG::X0, GG::Y0, GG::X(10), GG::Y(10), other_texture,  GG::GRAPHIC_FITGRAPHIC | GG::GRAPHIC_PROPSCALE);
        if (color != GG::CLR_ZERO)
            m_other_icon->SetColor(color);
    }

    if (m_icon) {
        m_icon->Show();
        AttachChild(m_icon);
    }
    if (m_other_icon) {
        m_other_icon->Show();
        AttachChild(m_other_icon);
    }

    // Set Text
    m_name_text->SetText(name);

    m_summary_text->SetText(str(FlexibleFormat(UserString("ENC_DETAIL_TYPE_STR"))
        % specific_type
        % general_type));
    if (color != GG::CLR_ZERO)
        m_summary_text->SetColor(color);

    m_cost_text->SetText(str(FlexibleFormat(UserString("ENC_COST_AND_TURNS_STR"))
        % static_cast<int>(cost + 0.5)
        % cost_units
        % turns));

    m_description_box->SetText(detailed_description);

    DoLayout();
}

void EncyclopediaDetailPanel::UnsetAll() {
    m_tech = 0;
    m_part = 0;
    m_hull = 0;
    m_building = 0;
    m_design = 0;
    m_special = 0;
}

void EncyclopediaDetailPanel::SetItem(const Tech* tech) {
    UnsetAll();
    m_tech = tech;
    Reset();
}

void EncyclopediaDetailPanel::SetItem(const PartType* part) {
    UnsetAll();
    m_part = part;
    Reset();
}

void EncyclopediaDetailPanel::SetItem(const HullType* hull) {
    UnsetAll();
    m_hull = hull;
    Reset();
}

void EncyclopediaDetailPanel::SetItem(const BuildingType* building) {
    UnsetAll();
    m_building = building;
    Reset();
}

void EncyclopediaDetailPanel::SetItem(const ShipDesign* design) {
    UnsetAll();
    m_design = design;
    Reset();
}

void EncyclopediaDetailPanel::SetItem(const Special* special) {
    UnsetAll();
    m_special = special;
    Reset();
}
