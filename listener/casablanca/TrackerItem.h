#pragma once

#include "SupportedAPI.h"

namespace Root
{
	namespace BugListener
	{

		namespace Core
		{
			struct TrackerItem
			{
				std::string SourceURL;
				utility::string_t Text;
				utility::string_t ID;
				std::string API;
			};

		}// namespace Core

	}// namespace BugListener
}// namespace Root


