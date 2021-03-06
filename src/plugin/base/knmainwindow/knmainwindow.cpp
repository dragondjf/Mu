/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QMainWindow>

#include "knglobal.h"
#include "knmainwindowcontainer.h"
#include "knmainwindowheaderplugin.h"

#include "knlocalemanager.h"

#include "knmainwindow.h"

#include <QDebug>

KNMainWindow::KNMainWindow(QObject *parent) :
    KNMainWindowPlugin(parent)
{
    //Initial the container.
    m_container=new KNMainWindowContainer;
    //Connect translate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMainWindow::retranslate);
}

void KNMainWindow::retranslate()
{
    //Check if the current category is usable.
    for(auto i=m_categoryList.begin();
        i!=m_categoryList.end();
        ++i)
    {
        QString categoryTitle=(*i).plugin->caption();
        //Check if i is current category.
        if((*i).index==m_currentCategory)
        {
            //Update the header text.
            setHeaderText(categoryTitle);
        }
        //Update the preference text.
        m_preferencePlugin->setCategoryText((*i).index,
                                            categoryTitle);
    }
}

void KNMainWindow::addCategoryPlugin(KNCategoryPlugin *plugin)
{
    //Generate category plugin item
    CategoryPluginItem currentCategory;
    currentCategory.plugin=plugin;
    //Add header widget and central widget.
    addHeaderWidget(plugin->headerWidget());
    addCentralWidget(plugin->centralWidget());
    //Add preference panel.
    currentCategory.index=m_preferencePlugin->addCategory(plugin);
    //Add the category item in to the list.
    m_categoryList.append(currentCategory);
    //If this is the first category, set it to button.
    if(m_categoryList.size()==1)
    {
        //Change the current category.
        m_currentCategory=currentCategory.index;
        //Enable extras for the plugin.
        plugin->enablePlatformExtras();
        //Set the icon and text to the first category.
        setHeaderIcon(plugin->icon());
        setHeaderText(plugin->caption());
    }
}

void KNMainWindow::setMainWindow(QMainWindow *mainWindow)
{
    if(m_mainWindow==nullptr)
    {
        m_mainWindow=mainWindow;
        //Set properties.
        m_mainWindow->setAutoFillBackground(true);
        m_mainWindow->setContentsMargins(0,0,0,0);
        //Set palette.
        QPalette pal=m_mainWindow->palette();
        pal.setColor(QPalette::Window, QColor(0,0,0));
        m_mainWindow->setPalette(pal);
        //Change parent relationship.
        m_container->setParent(m_mainWindow);
        //Set the container to the central widget.
        m_mainWindow->setCentralWidget(m_container);
    }
}

void KNMainWindow::setHeader(KNMainWindowHeaderPlugin *plugin)
{
    if(m_headerPlugin==nullptr)
    {
        m_headerPlugin=plugin;
        m_container->setHeaderWidget(m_headerPlugin->headerWidget());
        connect(plugin, &KNMainWindowHeaderPlugin::requireShowCategorySwitcher,
                this, &KNMainWindow::showCategorySwitcher);
    }
}

void KNMainWindow::setHeaderIcon(const QPixmap &icon)
{
    m_headerPlugin->setHeaderIcon(icon);
}

void KNMainWindow::setHeaderText(const QString &text)
{
    m_headerPlugin->setHeaderText(text);
}

void KNMainWindow::setCategoryStack(KNMainWindowCategoryStackPlugin *plugin)
{
    if(m_categoryStackPlugin==nullptr)
    {
        m_categoryStackPlugin=plugin;
        m_container->setCategoryStack(m_categoryStackPlugin->stackedWidget());
    }
}

void KNMainWindow::setCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin)
{
    if(m_categorySwitcherPlugin==nullptr)
    {
        m_categorySwitcherPlugin=plugin;
        m_container->setCategorySwitcher(m_categorySwitcherPlugin->switcherWidget());
        //Link the plugin.
        connect(m_categorySwitcherPlugin, &KNMainWindowCategorySwitcherPlugin::requireShowPreference,
                this, &KNMainWindow::showPreference);
        connect(m_categorySwitcherPlugin, &KNMainWindowCategorySwitcherPlugin::requireResetHeaderButton,
                this, &KNMainWindow::restoreHeaderButton);
    }
}

void KNMainWindow::setPreferencePanel(KNPreferencePlugin *plugin)
{
    if(m_preferencePlugin==nullptr)
    {
        m_preferencePlugin=plugin;
        m_container->setPreferencePanel(m_preferencePlugin->preferencePanel());
        //Link the plugin.
        connect(m_preferencePlugin, &KNPreferencePlugin::requireHidePreference,
                this, &KNMainWindow::hidePreference);
    }
}

void KNMainWindow::showPreference()
{
    m_container->showPreference();
}

void KNMainWindow::hidePreference()
{
    m_container->hidePreference();
}

void KNMainWindow::showCategorySwitcher()
{
    m_categorySwitcherPlugin->showSwitcher();
}

void KNMainWindow::restoreHeaderButton()
{
    m_headerPlugin->restoreHeaderButton();
}

void KNMainWindow::addHeaderWidget(QWidget *widget)
{
    m_headerPlugin->addHeaderWidget(widget);
}

void KNMainWindow::addCentralWidget(QWidget *widget)
{
    m_categoryStackPlugin->addCentralWidget(widget);
}
