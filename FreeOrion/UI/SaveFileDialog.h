// -*- C++ -*-
//SaveFileDialog.h
#ifndef _SaveFileDialog_h_
#define _SaveFileDialog_h_

#ifndef _CUIWnd_h_
#include "CUIWnd.h"
#endif

#include "CUIControls.h"

#include <GG/ListBox.h>
#include <GG/Layout.h>

#include <set>
#include <string>

class CUIButton;
class SaveFileRow;
class SaveFileListBox;

/** A dialog for choosing save files.
 * Shows some additional infoormation in the listing and more
 * in a preview section to the side.
 */
class SaveFileDialog : public CUIWnd {
public:
    /** \name Structors */ //@{
    /// Constructor
    /// @param extension The extension to enforce on the file name
    /// @param load If set to true, only allow choosing existing files
    SaveFileDialog (const std::string& extension, bool load = false );
    ~SaveFileDialog(); //!< dtor
    //@}

    /** \name Mutators */ //@{
    virtual void ModalInit(); //< Called when dialog is shown. Overrides
    virtual void KeyPress ( GG::Key key, boost::uint32_t key_code_point,
                            GG::Flags<GG::ModKey> mod_keys );
    //@}

    /// Get the chosen save files full path
    std::string Result() const;

    private:
    void Confirm();                         //!< when m_save_btn button is pressed
    void DoubleClickRow ( GG::ListBox::iterator row );
    void Cancel();                          //!< when m_load_btn button is pressed
    void SelectionChanged ( const GG::ListBox::SelectionSet& files );   //!< When file selection changes.
    void BrowseDirectories();
    void UpdateDirectory ( const std::string& newdir );                 //!< Change current directory

    void UpdatePreviewList();
    bool CheckChoiceValidity();             //!< Disables confirm if filename invalid. Returns false if not valid.
    void FileNameEdited ( const std::string& filename ); //!< Called when the filename changes

    GG::Layout*         m_layout;           //!< The layout of the dialog

    SaveFileListBox*    m_file_list;        //!< The list of available saves
    CUIEdit*            m_name_edit;        //!< The file name edit control
    CUIEdit*            m_current_dir_edit; //!< The editor for the save directory
    CUIButton*          m_browse_dir_btn;   //!< Button to browse directories
    CUIButton*          m_confirm_btn;      //!< Button to confirm choice

    std::string         m_loaded_dir;       //!< The directory whose contents are currently shown
    std::string         m_extension;        //!< The save game file name extension
    bool                m_load_only;        //!< Whether we are loading

    /// Remove copy ctor, assign
    SaveFileDialog ( const SaveFileDialog& );
    SaveFileDialog& operator= ( const SaveFileDialog& );
};

#endif // _SaveFileDialog_h_
