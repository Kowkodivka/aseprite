// Aseprite UI Library
// Copyright (C) 2001-2013  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef UI_MENU_H_INCLUDED
#define UI_MENU_H_INCLUDED
#pragma once

#include "base/signal.h"
#include "base/unique_ptr.h"
#include "ui/register_message.h"
#include "ui/widget.h"

namespace ui {

  class Accelerator;
  class MenuItem;
  class Timer;
  struct MenuBaseData;

  class Menu : public Widget
  {
  public:
    Menu();
    ~Menu();

    void showPopup(const gfx::Point& pos);

    // Returns the MenuItem that has as submenu this menu.
    MenuItem* getOwnerMenuItem() {
      return m_menuitem;
    }

  protected:
    virtual void onPaint(PaintEvent& ev) override;
    virtual void onResize(ResizeEvent& ev) override;
    virtual void onPreferredSize(PreferredSizeEvent& ev) override;

  private:
    void setOwnerMenuItem(MenuItem* ownerMenuItem) {
      m_menuitem = ownerMenuItem;
    }

    void closeAll();

    MenuItem* getHighlightedItem();
    void highlightItem(MenuItem* menuitem, bool click, bool open_submenu, bool select_first_child);
    void unhighlightItem();

    MenuItem* m_menuitem;         // From where the menu was open

    friend class MenuBox;
    friend class MenuItem;
  };

  class MenuBox : public Widget
  {
  public:
    MenuBox(WidgetType type = kMenuBoxWidget);
    ~MenuBox();

    Menu* getMenu();
    void setMenu(Menu* menu);

    MenuBaseData* getBase() {
      return m_base;
    }

    // Closes all menu-boxes and goes back to the normal state of the
    // menu-bar.
    void cancelMenuLoop();

  protected:
    virtual bool onProcessMessage(Message* msg) override;
    virtual void onResize(ResizeEvent& ev) override;
    virtual void onPreferredSize(PreferredSizeEvent& ev) override;
    MenuBaseData* createBase();

  private:
    void closePopup();

    MenuBaseData* m_base;

    friend class Menu;
  };

  class MenuBar : public MenuBox
  {
  public:
    MenuBar();
  };

  class MenuItem : public Widget
  {
  public:
    MenuItem(const std::string& text);
    ~MenuItem();

    Menu* getSubmenu();
    void setSubmenu(Menu* submenu);

    Accelerator* getAccel();
    void setAccel(Accelerator* accel);

    bool isHighlighted() const;
    void setHighlighted(bool state);

    // Returns true if the MenuItem has a submenu.
    bool hasSubmenu() const;

    // Returns true if the submenu is opened.
    bool hasSubmenuOpened() const {
      return (m_submenu_menubox != NULL);
    }

    // Returns the menu-box where the sub-menu has been opened, or just
    // NULL if the sub-menu is closed.
    MenuBox* getSubmenuContainer() const {
      return m_submenu_menubox;
    }

    // Fired when the menu item is clicked.
    Signal0<void> Click;

  protected:
    virtual bool onProcessMessage(Message* msg) override;
    virtual void onPaint(PaintEvent& ev) override;
    virtual void onPreferredSize(PreferredSizeEvent& ev) override;
    virtual void onClick();

  private:
    void openSubmenu(bool select_first);
    void closeSubmenu(bool last_of_close_chain);
    void startTimer();
    void stopTimer();
    void executeClick();

    Accelerator* m_accel;         // Hot-key
    bool m_highlighted;           // Is it highlighted?
    Menu* m_submenu;              // The sub-menu
    MenuBox* m_submenu_menubox;   // The opened menubox for this menu-item
    base::UniquePtr<Timer> m_submenu_timer; // Timer to open the submenu

    friend class Menu;
    friend class MenuBox;
  };

  extern RegisterMessage kOpenMenuItemMessage;
  extern RegisterMessage kCloseMenuItemMessage;
  extern RegisterMessage kClosePopupMessage;
  extern RegisterMessage kExecuteMenuItemMessage;

} // namespace ui

#endif
