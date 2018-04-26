#pragma once
#include "stdafx.h"

#include "Core.h"

namespace Root
{
	namespace BugListener
	{

		namespace Visualization
		{
			using namespace Root::BugListener::Core;

			class Vizualizer
			{
			public:
				Vizualizer() = delete;
				~Vizualizer() = delete;

				static void Visualize(const TrackerItemCollection& issues);
			};


		}// namespace Visualization

	}// namespace BugListener
}// namespace Root


