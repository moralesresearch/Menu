#include <QDebug>

#include "actionsearch.h"

#include <QMenuBar>
#include <QMenu>
#include <QList>
#include <QAction>
#include <QDebug>

void ActionSearch::clear() {
	actions.clear();
}

void ActionSearch::update(QMenuBar *menu)
{
	for (auto menuAction: menu->actions())
	{
		readMenuActions(menuAction->menu());
	}
}

void ActionSearch::execute(QString actionName)
{
	if (actions.contains(actionName)) {
		actions[actionName]->trigger();
	}
}

void ActionSearch::readMenuActions(QMenu* menu)
{
	// See https://doc.qt.io/qt-5/qaction.html#menu
	// If a QAction does not have menu in it, then
	// the pointer returned by QAction::menu will be 
	// null.
	if (!menu)
		return;

	QString menuName = menu->title().replace("&", "");


	for (auto action: menu->actions())
	{
		if (action->menu() != NULL)
		{
			readMenuActions(action->menu());
			continue;
		}

		QString actionName = action->text().replace("&", "");
		if (!actionName.isEmpty() && action->isEnabled())
		{
			// TODO: we might want to have a multilevel menuName
            // actionName += "\n(" + menuName +")";
            actionName = menuName + " > " + actionName;
            if (action->shortcut().toString() != "") {
                actionName = actionName + " (" + action->shortcut().toString() +")";
            }
			actions.insert(actionName, action);
		}

	}
}

