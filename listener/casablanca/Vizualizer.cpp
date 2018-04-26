#include "stdafx.h"
#include "Vizualizer.h"

#include "SupportedAPI.h"

#include "nana\1.5.0\include\nana\gui.hpp"
#include "nana\1.5.0\include\nana\gui\widgets\listbox.hpp"
#include "nana\1.5.0\include\nana\gui\widgets\textbox.hpp"
#include "nana\1.5.0\include\nana\gui\widgets\button.hpp"
#include "nana\1.5.0\include\nana\system\platform.hpp"

//Overload the operator<< for oresolver use custom types binded to a listbox entry
nana::listbox::oresolver& operator<<(nana::listbox::oresolver& ores, const Root::BugListener::Core::TrackerItem& ti)
{
	return ores << ti.ID << ti.Text;
}


namespace Root
{
	namespace BugListener
	{
		namespace Visualization
		{
			using namespace nana;

			void Vizualizer::Visualize(const TrackerItemCollection& issues)
			{
				const int MARGIN = 10, WIDTH = 1000, HEIGHT = 700;

				form fm(API::make_center(WIDTH, HEIGHT), nana::appearance{ true, true, true, false, true, false, false });
				fm.caption("BugListener v0.2");

				const int lsbox_width = WIDTH - 2 * MARGIN;
				listbox lsbox(fm, rectangle{ MARGIN, MARGIN, (unsigned int)lsbox_width, HEIGHT - 2*MARGIN });
				lsbox.append_header("ID", (unsigned int)(lsbox_width*0.25f));
				lsbox.append_header("Description", (unsigned int)(lsbox_width*0.75f));
				lsbox.append(REDMINE_API.name()); // index at 1
				lsbox.append(JIRA_API.name()); // index at 2

				for (const TrackerItem& ti : issues)
				{
					if (IS_REDMINE_API(ti.API.c_str()))
					{
						lsbox.at(1).append(ti, true);
					}
					else if (IS_JIRA_API(ti.API.c_str()))
					{
						lsbox.at(2).append(ti, true);
					}
					else
					{
						assert(false); // not supported
					}
				}

				TrackerItem ti_current;
				lsbox.events().selected([&](const arg_listbox& arg) {
					ti_current = arg.item.value<TrackerItem>();
				});
				lsbox.events().category_dbl_click([&](const arg_listbox_category& arg) {
					ti_current = TrackerItem(); // kind of Reset functionality
				});
				lsbox.events().dbl_click([&](const arg_mouse& arg) {
					if (!ti_current.SourceURL.empty())
					{
						system::open_url(ti_current.SourceURL);
					}
				});

				fm.show();
				exec(); // main window loop
			}

		}// namespace Visualization

	}// namespace BugListener
}// namespace Root
